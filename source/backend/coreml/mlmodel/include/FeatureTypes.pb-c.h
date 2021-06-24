/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: FeatureTypes.proto */

#ifndef PROTOBUF_C_FeatureTypes_2eproto__INCLUDED
#define PROTOBUF_C_FeatureTypes_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _CoreML__Specification__Int64FeatureType CoreML__Specification__Int64FeatureType;
typedef struct _CoreML__Specification__DoubleFeatureType CoreML__Specification__DoubleFeatureType;
typedef struct _CoreML__Specification__StringFeatureType CoreML__Specification__StringFeatureType;
typedef struct _CoreML__Specification__SizeRange CoreML__Specification__SizeRange;
typedef struct _CoreML__Specification__ImageFeatureType CoreML__Specification__ImageFeatureType;
typedef struct _CoreML__Specification__ImageFeatureType__ImageSize CoreML__Specification__ImageFeatureType__ImageSize;
typedef struct _CoreML__Specification__ImageFeatureType__EnumeratedImageSizes CoreML__Specification__ImageFeatureType__EnumeratedImageSizes;
typedef struct _CoreML__Specification__ImageFeatureType__ImageSizeRange CoreML__Specification__ImageFeatureType__ImageSizeRange;
typedef struct _CoreML__Specification__ArrayFeatureType CoreML__Specification__ArrayFeatureType;
typedef struct _CoreML__Specification__ArrayFeatureType__Shape CoreML__Specification__ArrayFeatureType__Shape;
typedef struct _CoreML__Specification__ArrayFeatureType__EnumeratedShapes CoreML__Specification__ArrayFeatureType__EnumeratedShapes;
typedef struct _CoreML__Specification__ArrayFeatureType__ShapeRange CoreML__Specification__ArrayFeatureType__ShapeRange;
typedef struct _CoreML__Specification__DictionaryFeatureType CoreML__Specification__DictionaryFeatureType;
typedef struct _CoreML__Specification__SequenceFeatureType CoreML__Specification__SequenceFeatureType;
typedef struct _CoreML__Specification__FeatureType CoreML__Specification__FeatureType;


/* --- enums --- */

/*
 * Assumes raw (decompressed) format
 */
typedef enum _CoreML__Specification__ImageFeatureType__ColorSpace {
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__COLOR_SPACE__INVALID_COLOR_SPACE = 0,
  /*
   *  8 bits per pixel
   */
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__COLOR_SPACE__GRAYSCALE = 10,
  /*
   * 32 bits per pixel: RGBA with A channel ignored
   */
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__COLOR_SPACE__RGB = 20,
  /*
   * 32 bits per pixel: BGRA with A channel ignored
   */
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__COLOR_SPACE__BGR = 30
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__COLOR_SPACE)
} CoreML__Specification__ImageFeatureType__ColorSpace;
typedef enum _CoreML__Specification__ArrayFeatureType__ArrayDataType {
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ARRAY_DATA_TYPE__INVALID_ARRAY_DATA_TYPE = 0,
  /*
   * 0x10000 | 32
   */
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ARRAY_DATA_TYPE__FLOAT32 = 65568,
  /*
   * 0x10000 | 64
   */
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ARRAY_DATA_TYPE__DOUBLE = 65600,
  /*
   * 0x20000 | 32
   */
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ARRAY_DATA_TYPE__INT32 = 131104
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ARRAY_DATA_TYPE)
} CoreML__Specification__ArrayFeatureType__ArrayDataType;

/* --- messages --- */

/*
 **
 * The 64-bit integer feature type.
 */
struct  _CoreML__Specification__Int64FeatureType
{
  ProtobufCMessage base;
};
#define CORE_ML__SPECIFICATION__INT64_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__int64_feature_type__descriptor) \
     }


/*
 **
 * The double-precision floating point number feature type.
 */
struct  _CoreML__Specification__DoubleFeatureType
{
  ProtobufCMessage base;
};
#define CORE_ML__SPECIFICATION__DOUBLE_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__double_feature_type__descriptor) \
     }


/*
 **
 * The string feature type.
 */
struct  _CoreML__Specification__StringFeatureType
{
  ProtobufCMessage base;
};
#define CORE_ML__SPECIFICATION__STRING_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__string_feature_type__descriptor) \
     }


struct  _CoreML__Specification__SizeRange
{
  ProtobufCMessage base;
  uint64_t lowerbound;
  /*
   * negative value means unbound otherwise upperbound is included in range
   */
  int64_t upperbound;
};
#define CORE_ML__SPECIFICATION__SIZE_RANGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__size_range__descriptor) \
    , 0, 0 }


struct  _CoreML__Specification__ImageFeatureType__ImageSize
{
  ProtobufCMessage base;
  uint64_t width;
  uint64_t height;
};
#define CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__IMAGE_SIZE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__image_feature_type__image_size__descriptor) \
    , 0, 0 }


struct  _CoreML__Specification__ImageFeatureType__EnumeratedImageSizes
{
  ProtobufCMessage base;
  size_t n_sizes;
  CoreML__Specification__ImageFeatureType__ImageSize **sizes;
};
#define CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__ENUMERATED_IMAGE_SIZES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__image_feature_type__enumerated_image_sizes__descriptor) \
    , 0,NULL }


struct  _CoreML__Specification__ImageFeatureType__ImageSizeRange
{
  ProtobufCMessage base;
  CoreML__Specification__SizeRange *widthrange;
  CoreML__Specification__SizeRange *heightrange;
};
#define CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__IMAGE_SIZE_RANGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__image_feature_type__image_size_range__descriptor) \
    , NULL, NULL }


typedef enum {
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__SIZE_FLEXIBILITY__NOT_SET = 0,
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__SIZE_FLEXIBILITY_ENUMERATED_SIZES = 21,
  CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__SIZE_FLEXIBILITY_IMAGE_SIZE_RANGE = 31
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__SIZE_FLEXIBILITY)
} CoreML__Specification__ImageFeatureType__SizeFlexibilityCase;

/*
 **
 * The image feature type.
 */
struct  _CoreML__Specification__ImageFeatureType
{
  ProtobufCMessage base;
  int64_t width;
  int64_t height;
  CoreML__Specification__ImageFeatureType__ColorSpace colorspace;
  CoreML__Specification__ImageFeatureType__SizeFlexibilityCase size_flexibility_case;
  union {
    CoreML__Specification__ImageFeatureType__EnumeratedImageSizes *enumeratedsizes;
    CoreML__Specification__ImageFeatureType__ImageSizeRange *imagesizerange;
  };
};
#define CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__image_feature_type__descriptor) \
    , 0, 0, CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__COLOR_SPACE__INVALID_COLOR_SPACE, CORE_ML__SPECIFICATION__IMAGE_FEATURE_TYPE__SIZE_FLEXIBILITY__NOT_SET, {0} }


struct  _CoreML__Specification__ArrayFeatureType__Shape
{
  ProtobufCMessage base;
  size_t n_shape;
  int64_t *shape;
};
#define CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__array_feature_type__shape__descriptor) \
    , 0,NULL }


struct  _CoreML__Specification__ArrayFeatureType__EnumeratedShapes
{
  ProtobufCMessage base;
  size_t n_shapes;
  CoreML__Specification__ArrayFeatureType__Shape **shapes;
};
#define CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ENUMERATED_SHAPES__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__array_feature_type__enumerated_shapes__descriptor) \
    , 0,NULL }


struct  _CoreML__Specification__ArrayFeatureType__ShapeRange
{
  ProtobufCMessage base;
  /*
   * sizeRanges.size() must be length 1 or 3
   * sizeRanges[d] specifies the allowed range for dimension d
   */
  size_t n_sizeranges;
  CoreML__Specification__SizeRange **sizeranges;
};
#define CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE_RANGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__array_feature_type__shape_range__descriptor) \
    , 0,NULL }


typedef enum {
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE_FLEXIBILITY__NOT_SET = 0,
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE_FLEXIBILITY_ENUMERATED_SHAPES = 21,
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE_FLEXIBILITY_SHAPE_RANGE = 31
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE_FLEXIBILITY)
} CoreML__Specification__ArrayFeatureType__ShapeFlexibilityCase;

typedef enum {
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__DEFAULT_OPTIONAL_VALUE__NOT_SET = 0,
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__DEFAULT_OPTIONAL_VALUE_INT_DEFAULT_VALUE = 41,
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__DEFAULT_OPTIONAL_VALUE_FLOAT_DEFAULT_VALUE = 51,
  CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__DEFAULT_OPTIONAL_VALUE_DOUBLE_DEFAULT_VALUE = 61
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__DEFAULT_OPTIONAL_VALUE)
} CoreML__Specification__ArrayFeatureType__DefaultOptionalValueCase;

/*
 **
 * The array feature type.
 */
struct  _CoreML__Specification__ArrayFeatureType
{
  ProtobufCMessage base;
  size_t n_shape;
  int64_t *shape;
  CoreML__Specification__ArrayFeatureType__ArrayDataType datatype;
  CoreML__Specification__ArrayFeatureType__ShapeFlexibilityCase shape_flexibility_case;
  union {
    CoreML__Specification__ArrayFeatureType__EnumeratedShapes *enumeratedshapes;
    CoreML__Specification__ArrayFeatureType__ShapeRange *shaperange;
  };
  CoreML__Specification__ArrayFeatureType__DefaultOptionalValueCase default_optional_value_case;
  union {
    int32_t intdefaultvalue;
    float floatdefaultvalue;
    double doubledefaultvalue;
  };
};
#define CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__array_feature_type__descriptor) \
    , 0,NULL, CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__ARRAY_DATA_TYPE__INVALID_ARRAY_DATA_TYPE, CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__SHAPE_FLEXIBILITY__NOT_SET, {0}, CORE_ML__SPECIFICATION__ARRAY_FEATURE_TYPE__DEFAULT_OPTIONAL_VALUE__NOT_SET, {0} }


typedef enum {
  CORE_ML__SPECIFICATION__DICTIONARY_FEATURE_TYPE__KEY_TYPE__NOT_SET = 0,
  CORE_ML__SPECIFICATION__DICTIONARY_FEATURE_TYPE__KEY_TYPE_INT64_KEY_TYPE = 1,
  CORE_ML__SPECIFICATION__DICTIONARY_FEATURE_TYPE__KEY_TYPE_STRING_KEY_TYPE = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__DICTIONARY_FEATURE_TYPE__KEY_TYPE)
} CoreML__Specification__DictionaryFeatureType__KeyTypeCase;

/*
 **
 * The dictionary feature type.
 */
struct  _CoreML__Specification__DictionaryFeatureType
{
  ProtobufCMessage base;
  CoreML__Specification__DictionaryFeatureType__KeyTypeCase key_type_case;
  union {
    CoreML__Specification__Int64FeatureType *int64keytype;
    CoreML__Specification__StringFeatureType *stringkeytype;
  };
};
#define CORE_ML__SPECIFICATION__DICTIONARY_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__dictionary_feature_type__descriptor) \
    , CORE_ML__SPECIFICATION__DICTIONARY_FEATURE_TYPE__KEY_TYPE__NOT_SET, {0} }


typedef enum {
  CORE_ML__SPECIFICATION__SEQUENCE_FEATURE_TYPE__TYPE__NOT_SET = 0,
  CORE_ML__SPECIFICATION__SEQUENCE_FEATURE_TYPE__TYPE_INT64_TYPE = 1,
  CORE_ML__SPECIFICATION__SEQUENCE_FEATURE_TYPE__TYPE_STRING_TYPE = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__SEQUENCE_FEATURE_TYPE__TYPE)
} CoreML__Specification__SequenceFeatureType__TypeCase;

/*
 **
 * The Sequence feature type.
 */
struct  _CoreML__Specification__SequenceFeatureType
{
  ProtobufCMessage base;
  /*
   * Range of allowed size/length/count of sequence
   */
  CoreML__Specification__SizeRange *sizerange;
  CoreML__Specification__SequenceFeatureType__TypeCase type_case;
  union {
    CoreML__Specification__Int64FeatureType *int64type;
    CoreML__Specification__StringFeatureType *stringtype;
  };
};
#define CORE_ML__SPECIFICATION__SEQUENCE_FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__sequence_feature_type__descriptor) \
    , NULL, CORE_ML__SPECIFICATION__SEQUENCE_FEATURE_TYPE__TYPE__NOT_SET, {0} }


typedef enum {
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE__NOT_SET = 0,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_INT64_TYPE = 1,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_DOUBLE_TYPE = 2,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_STRING_TYPE = 3,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_IMAGE_TYPE = 4,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_MULTI_ARRAY_TYPE = 5,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_DICTIONARY_TYPE = 6,
  CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE_SEQUENCE_TYPE = 7
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE)
} CoreML__Specification__FeatureType__TypeCase;

/*
 **
 * A feature, which may be optional.
 */
struct  _CoreML__Specification__FeatureType
{
  ProtobufCMessage base;
  protobuf_c_boolean isoptional;
  CoreML__Specification__FeatureType__TypeCase type_case;
  union {
    CoreML__Specification__Int64FeatureType *int64type;
    CoreML__Specification__DoubleFeatureType *doubletype;
    CoreML__Specification__StringFeatureType *stringtype;
    CoreML__Specification__ImageFeatureType *imagetype;
    CoreML__Specification__ArrayFeatureType *multiarraytype;
    CoreML__Specification__DictionaryFeatureType *dictionarytype;
    CoreML__Specification__SequenceFeatureType *sequencetype;
  };
};
#define CORE_ML__SPECIFICATION__FEATURE_TYPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&core_ml__specification__feature_type__descriptor) \
    , 0, CORE_ML__SPECIFICATION__FEATURE_TYPE__TYPE__NOT_SET, {0} }


/* CoreML__Specification__Int64FeatureType methods */
void   core_ml__specification__int64_feature_type__init
                     (CoreML__Specification__Int64FeatureType         *message);
size_t core_ml__specification__int64_feature_type__get_packed_size
                     (const CoreML__Specification__Int64FeatureType   *message);
size_t core_ml__specification__int64_feature_type__pack
                     (const CoreML__Specification__Int64FeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__int64_feature_type__pack_to_buffer
                     (const CoreML__Specification__Int64FeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__Int64FeatureType *
       core_ml__specification__int64_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__int64_feature_type__free_unpacked
                     (CoreML__Specification__Int64FeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__DoubleFeatureType methods */
void   core_ml__specification__double_feature_type__init
                     (CoreML__Specification__DoubleFeatureType         *message);
size_t core_ml__specification__double_feature_type__get_packed_size
                     (const CoreML__Specification__DoubleFeatureType   *message);
size_t core_ml__specification__double_feature_type__pack
                     (const CoreML__Specification__DoubleFeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__double_feature_type__pack_to_buffer
                     (const CoreML__Specification__DoubleFeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__DoubleFeatureType *
       core_ml__specification__double_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__double_feature_type__free_unpacked
                     (CoreML__Specification__DoubleFeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__StringFeatureType methods */
void   core_ml__specification__string_feature_type__init
                     (CoreML__Specification__StringFeatureType         *message);
size_t core_ml__specification__string_feature_type__get_packed_size
                     (const CoreML__Specification__StringFeatureType   *message);
size_t core_ml__specification__string_feature_type__pack
                     (const CoreML__Specification__StringFeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__string_feature_type__pack_to_buffer
                     (const CoreML__Specification__StringFeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__StringFeatureType *
       core_ml__specification__string_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__string_feature_type__free_unpacked
                     (CoreML__Specification__StringFeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__SizeRange methods */
void   core_ml__specification__size_range__init
                     (CoreML__Specification__SizeRange         *message);
size_t core_ml__specification__size_range__get_packed_size
                     (const CoreML__Specification__SizeRange   *message);
size_t core_ml__specification__size_range__pack
                     (const CoreML__Specification__SizeRange   *message,
                      uint8_t             *out);
size_t core_ml__specification__size_range__pack_to_buffer
                     (const CoreML__Specification__SizeRange   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__SizeRange *
       core_ml__specification__size_range__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__size_range__free_unpacked
                     (CoreML__Specification__SizeRange *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__ImageFeatureType__ImageSize methods */
void   core_ml__specification__image_feature_type__image_size__init
                     (CoreML__Specification__ImageFeatureType__ImageSize         *message);
/* CoreML__Specification__ImageFeatureType__EnumeratedImageSizes methods */
void   core_ml__specification__image_feature_type__enumerated_image_sizes__init
                     (CoreML__Specification__ImageFeatureType__EnumeratedImageSizes         *message);
/* CoreML__Specification__ImageFeatureType__ImageSizeRange methods */
void   core_ml__specification__image_feature_type__image_size_range__init
                     (CoreML__Specification__ImageFeatureType__ImageSizeRange         *message);
/* CoreML__Specification__ImageFeatureType methods */
void   core_ml__specification__image_feature_type__init
                     (CoreML__Specification__ImageFeatureType         *message);
size_t core_ml__specification__image_feature_type__get_packed_size
                     (const CoreML__Specification__ImageFeatureType   *message);
size_t core_ml__specification__image_feature_type__pack
                     (const CoreML__Specification__ImageFeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__image_feature_type__pack_to_buffer
                     (const CoreML__Specification__ImageFeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__ImageFeatureType *
       core_ml__specification__image_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__image_feature_type__free_unpacked
                     (CoreML__Specification__ImageFeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__ArrayFeatureType__Shape methods */
void   core_ml__specification__array_feature_type__shape__init
                     (CoreML__Specification__ArrayFeatureType__Shape         *message);
/* CoreML__Specification__ArrayFeatureType__EnumeratedShapes methods */
void   core_ml__specification__array_feature_type__enumerated_shapes__init
                     (CoreML__Specification__ArrayFeatureType__EnumeratedShapes         *message);
/* CoreML__Specification__ArrayFeatureType__ShapeRange methods */
void   core_ml__specification__array_feature_type__shape_range__init
                     (CoreML__Specification__ArrayFeatureType__ShapeRange         *message);
/* CoreML__Specification__ArrayFeatureType methods */
void   core_ml__specification__array_feature_type__init
                     (CoreML__Specification__ArrayFeatureType         *message);
size_t core_ml__specification__array_feature_type__get_packed_size
                     (const CoreML__Specification__ArrayFeatureType   *message);
size_t core_ml__specification__array_feature_type__pack
                     (const CoreML__Specification__ArrayFeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__array_feature_type__pack_to_buffer
                     (const CoreML__Specification__ArrayFeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__ArrayFeatureType *
       core_ml__specification__array_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__array_feature_type__free_unpacked
                     (CoreML__Specification__ArrayFeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__DictionaryFeatureType methods */
void   core_ml__specification__dictionary_feature_type__init
                     (CoreML__Specification__DictionaryFeatureType         *message);
size_t core_ml__specification__dictionary_feature_type__get_packed_size
                     (const CoreML__Specification__DictionaryFeatureType   *message);
size_t core_ml__specification__dictionary_feature_type__pack
                     (const CoreML__Specification__DictionaryFeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__dictionary_feature_type__pack_to_buffer
                     (const CoreML__Specification__DictionaryFeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__DictionaryFeatureType *
       core_ml__specification__dictionary_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__dictionary_feature_type__free_unpacked
                     (CoreML__Specification__DictionaryFeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__SequenceFeatureType methods */
void   core_ml__specification__sequence_feature_type__init
                     (CoreML__Specification__SequenceFeatureType         *message);
size_t core_ml__specification__sequence_feature_type__get_packed_size
                     (const CoreML__Specification__SequenceFeatureType   *message);
size_t core_ml__specification__sequence_feature_type__pack
                     (const CoreML__Specification__SequenceFeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__sequence_feature_type__pack_to_buffer
                     (const CoreML__Specification__SequenceFeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__SequenceFeatureType *
       core_ml__specification__sequence_feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__sequence_feature_type__free_unpacked
                     (CoreML__Specification__SequenceFeatureType *message,
                      ProtobufCAllocator *allocator);
/* CoreML__Specification__FeatureType methods */
void   core_ml__specification__feature_type__init
                     (CoreML__Specification__FeatureType         *message);
size_t core_ml__specification__feature_type__get_packed_size
                     (const CoreML__Specification__FeatureType   *message);
size_t core_ml__specification__feature_type__pack
                     (const CoreML__Specification__FeatureType   *message,
                      uint8_t             *out);
size_t core_ml__specification__feature_type__pack_to_buffer
                     (const CoreML__Specification__FeatureType   *message,
                      ProtobufCBuffer     *buffer);
CoreML__Specification__FeatureType *
       core_ml__specification__feature_type__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   core_ml__specification__feature_type__free_unpacked
                     (CoreML__Specification__FeatureType *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*CoreML__Specification__Int64FeatureType_Closure)
                 (const CoreML__Specification__Int64FeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__DoubleFeatureType_Closure)
                 (const CoreML__Specification__DoubleFeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__StringFeatureType_Closure)
                 (const CoreML__Specification__StringFeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__SizeRange_Closure)
                 (const CoreML__Specification__SizeRange *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ImageFeatureType__ImageSize_Closure)
                 (const CoreML__Specification__ImageFeatureType__ImageSize *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ImageFeatureType__EnumeratedImageSizes_Closure)
                 (const CoreML__Specification__ImageFeatureType__EnumeratedImageSizes *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ImageFeatureType__ImageSizeRange_Closure)
                 (const CoreML__Specification__ImageFeatureType__ImageSizeRange *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ImageFeatureType_Closure)
                 (const CoreML__Specification__ImageFeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ArrayFeatureType__Shape_Closure)
                 (const CoreML__Specification__ArrayFeatureType__Shape *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ArrayFeatureType__EnumeratedShapes_Closure)
                 (const CoreML__Specification__ArrayFeatureType__EnumeratedShapes *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ArrayFeatureType__ShapeRange_Closure)
                 (const CoreML__Specification__ArrayFeatureType__ShapeRange *message,
                  void *closure_data);
typedef void (*CoreML__Specification__ArrayFeatureType_Closure)
                 (const CoreML__Specification__ArrayFeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__DictionaryFeatureType_Closure)
                 (const CoreML__Specification__DictionaryFeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__SequenceFeatureType_Closure)
                 (const CoreML__Specification__SequenceFeatureType *message,
                  void *closure_data);
typedef void (*CoreML__Specification__FeatureType_Closure)
                 (const CoreML__Specification__FeatureType *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor core_ml__specification__int64_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__double_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__string_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__size_range__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__image_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__image_feature_type__image_size__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__image_feature_type__enumerated_image_sizes__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__image_feature_type__image_size_range__descriptor;
extern const ProtobufCEnumDescriptor    core_ml__specification__image_feature_type__color_space__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__array_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__array_feature_type__shape__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__array_feature_type__enumerated_shapes__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__array_feature_type__shape_range__descriptor;
extern const ProtobufCEnumDescriptor    core_ml__specification__array_feature_type__array_data_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__dictionary_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__sequence_feature_type__descriptor;
extern const ProtobufCMessageDescriptor core_ml__specification__feature_type__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_FeatureTypes_2eproto__INCLUDED */