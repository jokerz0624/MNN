#if defined(__ANDROID__) || defined(__aarch64__)

#include "Arm82Functions.hpp"
#include "Arm82OptFunc.hpp"
#include "Arm82WinogradOptFunc.hpp"
#include "Arm82Vec.hpp"
#include "backend/cpu/compute/CommonOptFunction.h"

#if defined(MNN_USE_NEON)
#include <arm_neon.h>
#endif

extern "C" {
// (UP_DIV(l,8), e, 8) -> (UP_DIV(e,eP), l, eP)
void Arm82MNNPackForMatMul_A(float* destOrigin, float const** sourceGroup, const int32_t* info, const int32_t* el);

// C(UP_DIV(h,8), e, h8) = B(UP_DIV(h,hP), l, hP) * A(l, eP), hP = 24
// parameter: [aStride, l, h, cStride, bExtraStride]
// aStride in parameter is deprecated (useless), but for code clean, just retain it
void MNNPackedMatMulFP16(float* C, const float* A, const float* B, const size_t* parameter, const float* postParameters, const float* bias);

// C(UP_DIV(h,8), e, h8) = B(UP_DIV(h,hP), l, hP) * A(l, e), hP = 24, e >= 1
// parameter: [aStride, l, h, cStride, bExtraStride]
void MNNPackedMatMulRemainFP16(float* C, const float* A, const float* B, size_t eSize, const size_t* parameter, const float* postParameters, const float* bias);

void MNNConvDwF23MulTransUnitFP16(FLOAT16 **cacheLine, const FLOAT16 *weight, FLOAT16 *dest, size_t ow);

void MNNConvDwF23SourceTransUnitFP16(const FLOAT16 *source, FLOAT16 *dest, size_t unit);

void MNNConvRunForLineDepthwiseFP16(float* dst, const float* src, const float* weight, size_t width, size_t src_w_setup,
                                size_t fw, size_t fh, size_t dilateX_step, size_t dilateY_step, size_t height, size_t srcHStep, size_t dstHStep);
}

using Vec = MNN::Math::Vec<FLOAT16, 8>;

namespace MNN {

static void MNNMatrixAddFP16(FLOAT16* C, const FLOAT16* A, const FLOAT16* B, size_t widthC8, size_t cStride, size_t aStride, size_t bStride, size_t height) {
    for (int y = 0; y < height; ++y) {
        auto a = A + aStride * y, b = B + bStride * y;
        auto c = C + cStride * y;
        for (int x = 0; x < widthC8; ++x) {
            vst1q_f16(c + x * 8, vaddq_f16(vld1q_f16(a + x * 8), vld1q_f16(b + x * 8)));
        }
    }
}
static void MNNMatrixSubFP16(FLOAT16* C, const FLOAT16* A, const FLOAT16* B, size_t widthC8, size_t cStride, size_t aStride, size_t bStride, size_t height) {
    for (int y = 0; y < height; ++y) {
        auto a = A + aStride * y, b = B + bStride * y;
        auto c = C + cStride * y;
        for (int x = 0; x < widthC8; ++x) {
            vst1q_f16(c + x * 8, vsubq_f16(vld1q_f16(a + x * 8), vld1q_f16(b + x * 8)));
        }
    }
}

static void Arm82MNNPackForMatMul_B(float* destC, const float* sourceC, size_t h, size_t l, bool transpose) {
    auto dest = (int16_t*)destC;
    auto source = (int16_t*)sourceC;
    int ePack, lPack, hPack;
    Arm82MNNGetMatMulPackMode(&ePack, &lPack, &hPack);
    auto hP = (int)h / hPack;
    auto hR = (int)hP * hPack;
    if (hR != h) {
        ::memset(dest, 0, UP_DIV(h, hPack) * hPack * l * sizeof(FLOAT16));
    }
    if (!transpose) {
        for (int y = 0; y < hP; ++y) {
            auto destY = dest + y * hPack * l;
            auto sourceY = source + y * hPack;
            for (int x = 0; x < l; ++x) {
                ::memcpy(destY + hPack * x, sourceY + x * h, hPack * sizeof(FLOAT16));
            }
        }
        auto hRemain = h - hR;
        if (hRemain > 0) {
            auto destY = dest + hP * hPack * l;
            auto sourceY = source + hP * hPack;
            for (int x = 0; x < l; ++x) {
                ::memcpy(destY + hPack * x, sourceY + x * h, hRemain * sizeof(FLOAT16));
            }
        }
        return;
    }
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < l; ++x) {
            dest[(y / hPack * l + x) * hPack + y % hPack] = source[y * l + x];
        }
    }
}

static void MNNScaleAndAddBiasFP16(FLOAT16* dst, const FLOAT16* src, const FLOAT16* bias, const FLOAT16* alpha, size_t planeNumber,
                        size_t biasNumber) {
    for (int z = 0; z < biasNumber; ++z) {
        FLOAT16* dstZ         = dst + planeNumber * 8 * z;
        const FLOAT16* srcZ   = src + planeNumber * 8 * z;
#ifdef MNN_USE_NEON
        auto biasZ = vld1q_f16(bias + 8 * z), alphaZ = vld1q_f16(alpha + 8 * z);
#else
        auto biasZ = bias + 8 * z, alphaZ = alpha + 8 * z;
#endif
        for (int p = 0; p < planeNumber; ++p) {
            FLOAT16* dstX       = dstZ + 8 * p;
            const FLOAT16* srcX = srcZ + 8 * p;
#ifdef MNN_USE_NEON
            auto res = vaddq_f16(vmulq_f16(vld1q_f16(srcX), alphaZ), biasZ);
            vst1q_f16(dstX, res);
#else
            for (int k = 0; k < 8; ++k) {
                dstX[k] = srcX[k] * alphaZ[k] + biasZ[k];
            }
#endif
        }
    }
}

static void MNNGridSampleComputeCordFP16(FLOAT16* dst, const FLOAT16* src, size_t inH, size_t inW, size_t outH, size_t outW, size_t stride, bool alignCorners) {

#ifdef MNN_USE_NEON
    float16x8_t zero = vdupq_n_f16(0);
    float16x8_t one = vdupq_n_f16(1);
    float16x8_t half = vdupq_n_f16(0.5f);
    float16x8_t a = alignCorners ? one : zero;
    float16x8_t b = alignCorners ? zero : one;
    float16x8_t inW_sub_a = vsubq_f16(vdupq_n_f16(inW), a);
    float16x8_t inH_sub_a = vsubq_f16(vdupq_n_f16(inH), a);

    int area = outH * outW;
    int areaC8 = area / 8;
    int areaRemain = area - areaC8 * 8;
    for (int i = 0; i < areaC8; ++i) {
        auto cordH = vld2q_f16(src);
        // float16x8_t x = cordH.val[0];
        // float16x8_t y = cordH.val[1];
        cordH.val[0] = vmulq_f16(half, vsubq_f16(vmulq_f16(vaddq_f16(one, cordH.val[0]), inW_sub_a), b));
        cordH.val[1] = vmulq_f16(half, vsubq_f16(vmulq_f16(vaddq_f16(one, cordH.val[1]), inH_sub_a), b));
        vst2q_f16(dst, cordH);

        src += 16;
        dst += 16;
    }

    for (int i = 0; i < areaRemain; ++i) {
        float16x8_t x = vdupq_n_f16(src[0]);
        float16x8_t y = vdupq_n_f16(src[1]);
        x = vmulq_f16(half, vsubq_f16(vmulq_f16(vaddq_f16(one, x), inW_sub_a), b));
        y = vmulq_f16(half, vsubq_f16(vmulq_f16(vaddq_f16(one, y), inH_sub_a), b));
        dst[0] = x[0];
        dst[1] = y[0];

        src += 2;
        dst += 2;
    }
#else
    FLOAT16 zero = (FLOAT16)0;
    FLOAT16 one = (FLOAT16)1;
    FLOAT16 half = (FLOAT16)0.5f;

    FLOAT16 a = alignCorners ? one : zero;
    FLOAT16 b = alignCorners ? zero : one;
    for (auto h = 0; h < outH; ++h) {
        auto __gridPtr = src + h * stride;
        auto cordH = dst + h * outW * 2;
        for (auto w = 0; w < outW; ++w) {
            auto x = __gridPtr[2 * w + 0];
            auto y = __gridPtr[2 * w + 1];
            cordH[2 * w + 0] = ((one + x) * (inW - a) - b) * half;
            cordH[2 * w + 1] = ((one + y) * (inH - a) - b) * half;
        }
    }
#endif
}

static Vec MNNGridSampleLoadSampleFP16(int h, int w, const FLOAT16 *buffer, int height, int width, bool padMode) {
    if (h < 0 || h >= height || w < 0 || w >= width) {
        if(padMode == true) { //padMode == BorderMode_ZEROS
            return (FLOAT16)0;
        }
        // Clearly, CLAMP is the right way to go for GridSamplePaddingMode_BORDER
        // For GridSamplePaddingMode_REFLECTION, since we have reflected the values into (-1, 1),
        // the leftover reflections degrade to GridSamplePaddingMode_BORDER
        h = h < 0 ? 0 : (h > (height - 1) ? (height - 1) : h);
        w = w < 0 ? 0 : (w > (width - 1) ? (width - 1) : w);
    }
    return Vec::load(buffer + h * width * 8 + w * 8);
}

static void MNNGridSampleInterpFP16(FLOAT16* outputPtr, const FLOAT16* inputPtr, const FLOAT16* cordPtr, size_t inH, size_t inW, size_t outW, bool sampleMode, bool padMode) {
    for (auto ow = 0; ow < outW; ++ow) {
        auto w_fp16 = cordPtr[2 * ow + 0];
        auto h_fp16 = cordPtr[2 * ow + 1];
        float w = (float)(w_fp16);
        float h = (float)(h_fp16);
        Vec interp;

        if (sampleMode == true) { //sampleMode == SampleMode_NEAREST
            int nh, nw;
#ifdef MNN_USE_NEON
            nh = vcvtms_s32_f32(h + 0.5f);
            nw = vcvtms_s32_f32(w + 0.5f);
#else
            nh = ::floor(h + 0.5f);
            nw = ::floor(w + 0.5f);
#endif
            interp = MNNGridSampleLoadSampleFP16(nh, nw, inputPtr, inH, inW, padMode);
        } else { //sampleMode == GridSampleMode_BILINEAR
            int w0_h, w0_w, w1_h, w1_w; 
#ifdef MNN_USE_NEON
            w0_h = vcvtms_s32_f32(h);
            w0_w = vcvtms_s32_f32(w);
            w1_h = vcvtps_s32_f32(h);
            w1_w = vcvtps_s32_f32(w);
#else
            w0_h = ::floor(h);
            w0_w = ::floor(w);
            w1_h = ::ceil(h);
            w1_w = ::ceil(w);
#endif
            auto oneV = Vec((FLOAT16)1);

            Vec i00 = MNNGridSampleLoadSampleFP16(w0_h, w0_w, inputPtr, inH, inW, padMode);
            Vec i01 = MNNGridSampleLoadSampleFP16(w0_h, w1_w, inputPtr, inH, inW, padMode);
            Vec i10 = MNNGridSampleLoadSampleFP16(w1_h, w0_w, inputPtr, inH, inW, padMode);
            Vec i11 = MNNGridSampleLoadSampleFP16(w1_h, w1_w, inputPtr, inH, inW, padMode);
            auto f0 = Vec((FLOAT16)w1_w - w_fp16);
            auto f1 = oneV - f0;
            auto h0 = Vec((FLOAT16)w1_h - h_fp16);
            auto h1 = oneV - h0;

            Vec i0 = i00 * f0 + i01 * f1;
            Vec i1 = i10 * f0 + i11 * f1;

            interp = i0 * h0 + i1 * h1;
        }

        Vec::save(outputPtr + 8 * ow, interp);
    }
}

static void MNNScaleAndAddBiasOutside(FLOAT16* dst, const FLOAT16* src, const FLOAT16* bias, const FLOAT16* alpha, size_t planeNumber,
                               size_t biasNumber) {
    for (size_t p = 0; p < planeNumber; ++p) {
        FLOAT16* dstPlane       = dst + p * biasNumber;
        const FLOAT16* srcPlane = src + p * biasNumber;
        for (int z = 0; z < biasNumber; ++z) {
            dstPlane[z] = srcPlane[z] * alpha[z] + bias[z];
        }
    }
}

static void MNNAddBiasFP16(FLOAT16* dst, const FLOAT16* bias, size_t planeNumber, size_t biasNumber) {
    using Vec = MNN::Math::Vec<FLOAT16, 8>;
    for (int i = 0; i < biasNumber; ++i) {
        auto b = Vec::load(bias + i * 8);
        for (int j = 0; j < planeNumber; ++j) {
            auto dstPtr = dst + (i * planeNumber + j) * 8;
            Vec::save(dstPtr, Vec::load(dstPtr) + b);
        }
    }
}
static void MNNAddBiasReluFP16(FLOAT16* dst, const FLOAT16* bias, size_t planeNumber, size_t biasNumber) {
    using Vec = MNN::Math::Vec<FLOAT16, 8>;
    Vec zero((FLOAT16)0);
    for (int i = 0; i < biasNumber; ++i) {
        auto b = Vec::load(bias + i * 8);
        for (int j = 0; j < planeNumber; ++j) {
            auto dstPtr = dst + (i * planeNumber + j) * 8;
            auto result = Vec::max(Vec::load(dstPtr) + b, zero);
            Vec::save(dstPtr, result);
        }
    }
}
static void MNNAddBiasRelu6FP16(FLOAT16* dst, const FLOAT16* bias, size_t planeNumber, size_t biasNumber) {
    using Vec = MNN::Math::Vec<FLOAT16, 8>;
    Vec zero((FLOAT16)0), six((FLOAT16)6);
    for (int i = 0; i < biasNumber; ++i) {
        auto b = Vec::load(bias + i * 8);
        for (int j = 0; j < planeNumber; ++j) {
            auto dstPtr = dst + (i * planeNumber + j) * 8;
            auto result = Vec::min(Vec::max(Vec::load(dstPtr) + b, zero), six);
            Vec::save(dstPtr, result);
        }
    }
}

static void MNNCopyC8WithStrideFP16(const FLOAT16* source, FLOAT16* dest, size_t srcStride, size_t dstStride, size_t count) {
    using Vec = MNN::Math::Vec<FLOAT16, 8>;
    for (int i = 0; i < count; ++i) {
        auto srcPtr = source + i * srcStride;
        auto dstPtr = dest + i * dstStride;
        Vec::save(dstPtr, Vec::load(srcPtr));
    }
}

static void MNNAddC8WithStrideFP16(const FLOAT16* source, FLOAT16* dest, size_t srcStride, size_t dstStride, size_t count) {
    using Vec = MNN::Math::Vec<FLOAT16, 8>;
    for (int i = 0; i < count; ++i) {
        auto srcPtr = source + i * srcStride;
        auto dstPtr = dest + i * dstStride;
        auto value = Vec::load(dstPtr) + Vec::load(srcPtr);
        Vec::save(dstPtr, value);
    }
}

static void MNNAxByClampBroadcastC8FP16(float* CF, const float* AF, const float* BF, size_t width, size_t cStride, size_t aStride, size_t height, const float* parameters) {
    auto C = (FLOAT16*)CF;
    auto A = (FLOAT16*)AF;
    auto B = (FLOAT16*)BF;
    using Vec = MNN::Math::Vec<FLOAT16, 8>;
    auto minF = Vec(parameters[2]);
    auto maxF = Vec(parameters[3]);
    auto beta = Vec(parameters[1]);
    for (int y = 0; y < height; ++y) {
        auto a = A + aStride * y;
        auto b = B + 8 * y;
        auto bv = Vec::load(b);
        auto c = C + cStride * y;
        for (int x = 0; x < width; ++x) {
            auto av = Vec::load(a + 8 * x);
            auto cv = av + bv * beta;
            cv = Vec::min(cv, maxF);
            cv = Vec::max(cv, minF);
            Vec::save(c + 8 * x, cv);
        }
    }
}

void ARM82MultiAndDestTransformCommon(FLOAT16 **cacheLine, const FLOAT16 *weight, FLOAT16 *dest, int cacheLineSize, int ow) {
    constexpr int pack = 8;
    int unit = ow / 2;
    MNN_ASSERT(cacheLineSize >= 1);
    for (int x = 0; x < unit; ++x) {
        int offset = 4 * pack * x, i = 0;
        Vec m0 = Vec::load(weight + i * 4 * pack) * Vec::load(cacheLine[i] + offset);
        Vec m1 = Vec::load(weight + (i * 4 + 1) * pack) * Vec::load(cacheLine[i] + offset + pack * 1);
        Vec m2 = Vec::load(weight + (i * 4 + 2) * pack) * Vec::load(cacheLine[i] + offset + pack * 2);
        Vec m3 = Vec::load(weight + (i * 4 + 3) * pack) * Vec::load(cacheLine[i] + offset + pack * 3);
        for (i = 1; i < cacheLineSize; ++i) {
            m0 = m0 + Vec::load(weight + i * 4 * pack) * Vec::load(cacheLine[i] + offset);
            m1 = m1 + Vec::load(weight + (i * 4 + 1) * pack) * Vec::load(cacheLine[i] + offset + pack * 1);
            m2 = m2 + Vec::load(weight + (i * 4 + 2) * pack) * Vec::load(cacheLine[i] + offset + pack * 2);
            m3 = m3 + Vec::load(weight + (i * 4 + 3) * pack) * Vec::load(cacheLine[i] + offset + pack * 3);
        }
        auto o0 = m0 + m1 + m2;
        auto o1 = m1 - m2 + m3;
        Vec::save(dest + (2 * x + 0) * pack, o0);
        Vec::save(dest + (2 * x + 1) * pack, o1);
    }
    if (unit * 2 < ow) {
        int offset = 4 * pack * unit, i = 0;
        Vec m0 = Vec::load(weight + i * 4 * pack) * Vec::load(cacheLine[i] + offset);
        Vec m1 = Vec::load(weight + (i * 4 + 1) * pack) * Vec::load(cacheLine[i] + offset + pack);
        Vec m2 = Vec::load(weight + (i * 4 + 2) * pack) * Vec::load(cacheLine[i] + offset + pack * 2);
        for (i = 1; i < cacheLineSize; ++i) {
            m0 = m0 + Vec::load(weight + i * 4 * pack) * Vec::load(cacheLine[i] + offset);
            m1 = m1 + Vec::load(weight + (i * 4 + 1) * pack) * Vec::load(cacheLine[i] + offset + pack);
            m2 = m2 + Vec::load(weight + (i * 4 + 2) * pack) * Vec::load(cacheLine[i] + offset + pack * 2);
        }
        auto o0 = m0 + m1 + m2;
        Vec::save(dest + 2 * unit * pack, o0);
    }
}
// unit: winograd unit (output is w/2)
void ARM82SourceTransformCommon(const FLOAT16 *source, FLOAT16 *dest, int unit, int iw, int pad, int su, int eu) {
    constexpr int pack = 8; // float16x8
    for (int x = 0; x < su; ++x) {
        auto dstX = dest + 4 * pack * x;
        auto sx   = x * 2 - (int)pad;
        auto ex   = sx + 4;
        auto clampSx = std::max(sx, 0);
        auto clampEx = std::min(ex, (int)iw);
        Vec v[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        for (int i = clampSx; i < clampEx; ++i) {
            v[i - sx] = Vec::load(source + pack * i);
        }
        auto m0 = v[0] - v[2];
        auto m1 = v[1] + v[2];
        auto m2 = v[2] - v[1];
        auto m3 = v[3] - v[1];
        Vec::save(dstX + pack * 0, m0);
        Vec::save(dstX + pack * 1, m1);
        Vec::save(dstX + pack * 2, m2);
        Vec::save(dstX + pack * 3, m3);
    }
    MNNConvDwF23SourceTransUnitFP16(source + pack * (su * 2 - pad), dest + 4 * pack * su, eu - su);
    for (int x = eu; x < unit; ++x) {
        auto dstX = dest + 4 * pack * x;
        auto sx   = x * 2 - (int)pad;
        auto ex   = sx + 4;
        auto clampSx = std::max(sx, 0);
        auto clampEx = std::min(ex, (int)iw);
        Vec v[4] = {0.0f, 0.0f, 0.0f, 0.0f};
        for (int i = clampSx; i < clampEx; ++i) {
            v[i - sx] = Vec::load(source + pack * i);
        }
        auto m0 = v[0] - v[2];
        auto m1 = v[1] + v[2];
        auto m2 = v[2] - v[1];
        auto m3 = v[3] - v[1];
        Vec::save(dstX + pack * 0, m0);
        Vec::save(dstX + pack * 1, m1);
        Vec::save(dstX + pack * 2, m2);
        Vec::save(dstX + pack * 3, m3);
    }
}

void ARM82StrassenMerge(FLOAT16* c11, FLOAT16* c12, FLOAT16* c21, FLOAT16* c22, FLOAT16* xAddr,
                          size_t cStride, size_t eSub, size_t hSub) {
    const int pack = 8;
    for (int y = 0; y < hSub; ++y) {
        auto c11Y = c11 + y * cStride;
        auto c12Y = c12 + y * cStride;
        auto c22Y = c22 + y * cStride;
        auto c21Y = c21 + y * cStride;
        auto xY = xAddr + y * eSub * pack;
        for (int x = 0; x < eSub; ++x) {
            auto xv = vld1q_f16(xY + x * pack);
            auto c21v = vld1q_f16(c21Y + x * pack);
            auto c11v = vld1q_f16(c11Y + x * pack);
            auto c22v = vld1q_f16(c22Y + x * pack);
            auto c12v = vld1q_f16(c12Y + x * pack);
            c12v = c12v + xv;
            c21v = c12v + c21v;
            c12v = c22v + c12v;
            c22v = c22v + c21v;
            c12v = c11v + c12v;
            vst1q_f16(c12Y + x * pack, c12v);
            vst1q_f16(c22Y + x * pack, c22v);
            vst1q_f16(c21Y + x * pack, c21v);
        }
    }
}

void MNNUnpackTransposeInt16C8(int16_t* dst, const int16_t* src, size_t area, size_t depth) {
    if (1 == area) {
        ::memcpy(dst, src, depth * sizeof(int16_t));
        return;
    }
    int c      = (int)depth;
    int cDiv4  = c / 8;
    int cAlign = cDiv4 * 8;
    if (cAlign == c) {
        for (int hi = 0; hi < area; ++hi) {
            auto srcHeight = src + hi * 8;
            auto dstHeight = dst + hi * cDiv4 * 8;
            for (int ci = 0; ci < cDiv4; ++ci) {
                vst1q_s16(dstHeight + ci * 8, vld1q_s16(srcHeight + 8 * ci * area));
            }
        }
        return;
    }

    for (int hi = 0; hi < area; ++hi) {
        auto srcHeight = src + hi * 8;
        auto dstHeight = dst + hi * c;
        for (int ci = 0; ci < cDiv4; ++ci) {
            vst1q_s16(dstHeight + ci * 8, vld1q_s16(srcHeight + 8 * ci * area));
        }
    }

    int cReamin   = c - cAlign;
    auto srcAlign = src + area * cAlign;
    auto dstAlign = dst + cAlign;

    for (int hi = 0; hi < area; ++hi) {
        auto srcHeight = srcAlign + hi * 8;
        auto dstHeight = dstAlign + hi * c;

        for (int ci = 0; ci < cReamin; ++ci) {
            dstHeight[ci] = srcHeight[ci];
        }
    }
}

void MNNPackTransposeInt16C8(int16_t* dst, const int16_t* src, size_t area, size_t depth) {
    if (depth == 8) {
        ::memcpy(dst, src, area * depth * sizeof(int16_t));
        return;
    }
    int c      = (int)depth;
    int cDiv4  = c / 8;
    int cAlign = cDiv4 * 8;
    for (int hi = 0; hi < area; ++hi) {
        auto srcHeight = (src + hi * c);
        auto dstHeight = (dst + hi * 8);
        for (int ci = 0; ci < cDiv4; ++ci) {
            vst1q_s16(dstHeight + ci * area * 8, vld1q_s16(srcHeight + 8 * ci));
        }
    }

    if (cAlign == c) {
        return;
    }

    int cReamin   = c - cAlign;
    auto srcAlign = src + cAlign;
    auto dstAlign = dst + area * cAlign;

    for (int hi = 0; hi < area; ++hi) {
        auto srcHeight = srcAlign + hi * c;
        auto dstHeight = dstAlign + hi * 8;
        for (int i = 0; i < 8; ++i) {
            dstHeight[i] = 0;
        }
        for (int ci = 0; ci < cReamin; ++ci) {
            dstHeight[ci] = srcHeight[ci];
        }
    }
}

static void MNNConvRunForUnitDepthWiseFP16(float* dst, const float* src, const float* weight, size_t fw, size_t fh,
                                           size_t weight_y_step, size_t dilateX_step, size_t dilateY_step) {
    int fx, fy;
    Vec dstValue(0.0f);
    auto src_z    = (const FLOAT16*)src;
    auto weight_z = (const FLOAT16*)weight;
    for (fy = 0; fy < fh; ++fy) {
        auto src_y    = src_z + fy * dilateY_step;
        auto weight_y = weight_z + fy * weight_y_step;
        for (fx = 0; fx < fw; ++fx) {
            auto weight_x = weight_y + 8 * fx;
            auto src_x    = src_y + fx * dilateX_step;
            dstValue = dstValue + Vec::load(src_x) * Vec::load(weight_x);
        }
    }
    Vec::save((FLOAT16*)dst, dstValue);
}

static void _MNNDeconvRunForUnitDepthWise(const FLOAT16* dst, FLOAT16* src, const FLOAT16* weight, size_t fw, size_t fh,
                                  size_t weight_y_step, size_t dilateX_step, size_t dilateY_step) {
    int fx, fy;
    auto src_z          = src;
    auto weight_z = weight;
    Vec dstV           = Vec::load(dst);
    for (fy = 0; fy < fh; ++fy) {
        auto src_y          = src_z + fy * dilateY_step;
        auto weight_y = weight_z + fy * weight_y_step;
        for (fx = 0; fx < fw; ++fx) {
            Vec weight_x = Vec::load(weight_y + 8 * fx);
            Vec src_x    = Vec::load(src_y + fx * dilateX_step);
            Vec::save(src_y + fx * dilateX_step, src_x + weight_x * dstV);
        }
    }
}
static void _MNNDeconvRunForLineDepthwise(const FLOAT16* dst, FLOAT16* src, const FLOAT16* weight, size_t width, size_t src_w_setup,
                                  size_t fw, size_t fh, size_t dilateX_step, size_t dilateY_step) {
    int dx;
    for (dx = 0; dx < width; ++dx) {
        auto dst_x = dst + dx * 8;
        auto src_dx      = src + src_w_setup * dx;
        _MNNDeconvRunForUnitDepthWise(dst_x, src_dx, weight, fw, fh, fw * 8, dilateX_step, dilateY_step);
    }
}

static CoreFunctions* gInstance = nullptr;
bool Arm82Functions::init() {
#define FUNC_PTR_ASSIGN(dst, src) dst = (decltype(dst))src
    gInstance = new CoreFunctions;
    FUNC_PTR_ASSIGN(gInstance->MNNFp32ToLowp, MNNQuantizeFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNLowpToFp32, MNNDequantizeFP16);
    gInstance->bytes = 2;
    
    // Packed
    gInstance->pack = 8;
    FUNC_PTR_ASSIGN(gInstance->MNNPackCUnit, MNNPackC8FP16);
    FUNC_PTR_ASSIGN(gInstance->MNNUnpackCUnit, MNNUnPackC8FP16);
    FUNC_PTR_ASSIGN(gInstance->MNNPackCUnitTranspose, MNNPackTransposeInt16C8);
    FUNC_PTR_ASSIGN(gInstance->MNNUnpackCUnitTranspose, MNNUnpackTransposeInt16C8);
    FUNC_PTR_ASSIGN(gInstance->MNNConvRunForUnitDepthWise, MNNConvRunForUnitDepthWiseFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNConvRunForLineDepthwise, MNNConvRunForLineDepthwiseFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNAxByClampBroadcastUnit, MNNAxByClampBroadcastC8FP16);
    FUNC_PTR_ASSIGN(gInstance->MNNConvDwF23MulTransUnit, MNNConvDwF23MulTransUnitFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNSourceTransformCommonF23, ARM82SourceTransformCommon);
    FUNC_PTR_ASSIGN(gInstance->MNNMultiAndDestTransformCommon23, ARM82MultiAndDestTransformCommon);
    FUNC_PTR_ASSIGN(gInstance->MNNMatrixSub, MNNMatrixSubFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNMatrixAdd, MNNMatrixAddFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNStrassenMergeCFunction, ARM82StrassenMerge);
    gInstance->penalty = 2.0f;
    FUNC_PTR_ASSIGN(gInstance->MNNScaleAndAddBias, MNNScaleAndAddBiasFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNGridSampleComputeCord, MNNGridSampleComputeCordFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNGridSampleInterp, MNNGridSampleInterpFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNCopyC4WithStride, MNNCopyC8WithStrideFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNAddC4WithStride, MNNAddC8WithStrideFP16);
    
    // MatMul
    FUNC_PTR_ASSIGN(gInstance->MNNPackedMatMul, MNNPackedMatMulFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNPackedMatMulRemain, MNNPackedMatMulRemainFP16);
    FUNC_PTR_ASSIGN(gInstance->MNNPackC4ForMatMul_A, Arm82MNNPackForMatMul_A);
    FUNC_PTR_ASSIGN(gInstance->MNNGetMatMulPackMode, Arm82MNNGetMatMulPackMode);
    FUNC_PTR_ASSIGN(gInstance->MNNPackForMatMul_B, Arm82MNNPackForMatMul_B);
    
    FUNC_PTR_ASSIGN(gInstance->chooseWinoSourceTransform, Arm82WinogradFunction::chooseSourceTransform);
    FUNC_PTR_ASSIGN(gInstance->chooseWinoDestTransform, Arm82WinogradFunction::chooseDestTransform);

    gInstance->MNNDeconvRunForLineDepthwise = (decltype(gInstance->MNNDeconvRunForLineDepthwise))_MNNDeconvRunForLineDepthwise;
    gInstance->MNNDeconvRunForUnitDepthWise = (decltype(gInstance->MNNDeconvRunForUnitDepthWise))_MNNDeconvRunForUnitDepthWise;
    return true;
}

CoreFunctions* Arm82Functions::get() {
    return gInstance;
}
};
#endif
