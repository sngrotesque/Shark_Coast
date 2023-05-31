#include <wmkc_base.h>

// Base64编码表
WMKC_PRIVATE_CONST(wmkcByte) _B64ET[64] = {
    65,   66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,
    79,   80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  97,  98,
    99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
    113, 114, 115, 116, 117, 118, 119, 120, 121, 122,  48,  49,  50,  51,
    52,   53,  54,  55,  56,  57,  43,  47};

// Base64解码表
WMKC_PRIVATE_CONST(wmkcByte) _B64DT[123] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  62, 0,  0,  0,  63, 52, 53, 54, 55, 56, 57,
    58, 59, 60, 61, 0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
    7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
    25, 0,  0,  0,  0,  0,  0,  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51};

/**
 * @brief 获取数据经过Base64编码后的长度
 * @authors SN-Grotesque
 * 
 * 此函数通过获取wmkc对象的size成员来计算编码后的长度
 * 也就是说size成员必须被赋值为正确的值。
 * 
 * @note 此函数无需改动，如果需要进行错误检查，那么请在编码函数中进行。
 * @param src 此参数是一个wmkc对象，此函数通过此对象的size成员获取编码后的长度。
 * @return 此函数返回一个wmkcSize类型变量，代表编码后的长度值。
*/
WMKC_PUBLIC(wmkcSize) wmkcBase64_encode_size WMKC_OPEN_API
WMKC_OF((wmkc_obj *src))
{
    if(src->size % 3)
        return (src->size / 3 + 1) * 4;
    return src->size / 3 * 4;
}

/**
 * @brief 获取数据经过Base64解码后的长度
 * @authors SN-Grotesque
 * 
 * 此函数通过获取wmkc对象的size成员来计算编码后的长度
 * 也就是说size成员必须被赋值为正确的值。
 * 
 * @note 此函数无需进行错误检查（请在解码函数进行），此函数
 *       应该可以再优化，但目前未想到好的方案。
 * @param src 此参数是一个wmkc对象，此函数通过此对象的size成员获取解码后的长度。
 * @return 此函数返回一个wmkcSize类型变量，代表解码后的长度值。
*/
WMKC_PUBLIC(wmkcSize) wmkcBase64_decode_size WMKC_OPEN_API
WMKC_OF((wmkc_obj *src))
{
    if ((src->buf[src->size - 1] & src->buf[src->size - 2]) == WMKC_BASE64_PAD)
        return src->size / 4 * 3 - 2;
    else if (src->buf[src->size - 1] == WMKC_BASE64_PAD)
        return src->size / 4 * 3 - 1;
    else
        return src->size / 4 * 3;
}

/**
 * @brief 新建一个wmkcBase64对象
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcBase64对象申请内存空间。
 * 
 * @note 无。
 * @param obj 此参数是一个wmkcBase64对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_new WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj))
{
    wmkcErr_obj error;

    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcBase64_new: obj is NULL.");
    }
    if(!wmkcMemoryNew(wmkcBase64_obj *, (*obj), sizeof(wmkcBase64_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcBase64_new: (*obj) failed to apply for memory.");
    }
    if(!wmkcMemoryNew(wmkc_obj *, (*obj)->src, sizeof(wmkc_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcBase64_new: (*obj)->src failed to apply for memory.");
    }
    if(!wmkcMemoryNew(wmkc_obj *, (*obj)->dst, sizeof(wmkc_obj))) {
        wmkcErr_return(error, wmkcErr_ErrMemory,
            "wmkcBase64_new: (*obj)->dst failed to apply for memory.");
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief 释放一个wmkcBase64对象
 * @authors SN-Grotesque
 * 
 * 此函数为wmkcBase64对象释放内存空间。
 * 
 * @note 无。
 * @param obj 此参数是一个wmkcBase64对象的地址。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_free WMKC_OPEN_API
WMKC_OF((wmkcBase64_obj **obj))
{
    wmkcErr_obj error;

    if(!obj) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcBase64_free: obj is NULL.");
    }
    wmkcMemoryFree((*obj)->src);
    wmkcMemoryFree((*obj)->dst);
    wmkcMemoryFree((*obj));

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief Base64编码函数
 * @authors SN-Grotesque
 * 
 * 此函数将src中的内容编码后赋值到dst对象中。
 * 
 * @note 无。
 * @param src 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @param dst 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_Encode WMKC_OPEN_API
WMKC_OF((wmkc_obj *dst, wmkc_obj *src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcBase64_Encode: dst or src is null.");
    }
    if(!dst->buf || !src->buf) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBase64_Encode: dst->buf or src->buf is null.");
    }
    if(!dst->size || !src->size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBase64_Encode: dst->size or src->size is null.");
    }
    wmkcSize dst_i, src_i;

    for (dst_i = src_i = 0; dst_i < dst->size - 2; src_i += 3, dst_i += 4) {
        dst->buf[dst_i]   = _B64ET[src->buf[src_i]           >> 2];
        dst->buf[dst_i+1] = _B64ET[(src->buf[src_i]   & 0x3) << 4 | (src->buf[src_i+1] >> 4)];
        dst->buf[dst_i+2] = _B64ET[(src->buf[src_i+1] & 0xf) << 2 | (src->buf[src_i+2] >> 6)];
        dst->buf[dst_i+3] = _B64ET[src->buf[src_i+2]  & 0x3f];
    }

    switch(src->size % 3) {
        case 1:
            dst->buf[dst_i - 2] = WMKC_BASE64_PAD;
            dst->buf[dst_i - 1] = WMKC_BASE64_PAD;
            break;
        case 2:
            dst->buf[dst_i - 1] = WMKC_BASE64_PAD;
            break;
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}

/**
 * @brief Base64解码函数
 * @authors SN-Grotesque
 * 
 * 此函数将src中的内容解码后赋值到dst对象中。
 * 
 * @note 无。
 * @param src 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @param dst 此参数是一个wmkc对象，应至少包含数据与数据长度。
 * @return 返回一个wmkcErr对象，code为0代表无错误，如果为
 *         其他值，那么需检查message与code。
*/
WMKC_PUBLIC(wmkcErr_obj) wmkcBase64_Decode WMKC_OPEN_API
WMKC_OF((wmkc_obj *dst, wmkc_obj *src))
{
    wmkcErr_obj error;
    if(!dst || !src) {
        wmkcErr_return(error, wmkcErr_ErrNULL, "wmkcBase64_Decode: dst or src is null.");
    }
    if(!dst->buf || !src->buf) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBase64_Decode: dst->buf or src->buf is null.");
    }
    if(!dst->size || !src->size) {
        wmkcErr_return(error, wmkcErr_ErrNULL,
            "wmkcBase64_Decode: dst->size or src->size is null.");
    }
    wmkcSize src_i, dst_i;

    for (src_i = dst_i = 0; src_i < src->size - 2; dst_i += 3, src_i += 4) {
        dst->buf[dst_i]   = (_B64DT[src->buf[src_i]]   << 2) |  (_B64DT[src->buf[src_i+1]]  >> 4);
        dst->buf[dst_i+1] = (_B64DT[src->buf[src_i+1]] << 4) | ((_B64DT[src->buf[src_i+2]]) >> 2);
        dst->buf[dst_i+2] = (_B64DT[src->buf[src_i+2]] << 6) |   _B64DT[src->buf[src_i+3]];
    }

    wmkcErr_return(error, wmkcErr_OK, "OK.");
}
