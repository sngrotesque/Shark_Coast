#include <network/wmkc_net.c>
#include <image/wmkc_png.c>
#include <crypto/snc.c>
#include <wmkc_binascii.c>
#include <wmkc_hexdump.c>
#include <wmkc_common.c>
#include <wmkc_winapi.c>
#include <wmkc_object.c>
#include <wmkc_random.c>
#include <wmkc_struct.c>
#include <wmkc_thread.c>
#include <wmkc_base64.c>
#include <wmkc_stream.c>
#include <wmkc_coder.c>
#include <wmkc_chat.c>
#include <wmkc_dict.c>
#include <wmkc_file.c>
#include <wmkc_misc.c>
#include <wmkc_math.c>
#include <wmkc_time.c>
#include <wmkc_hash.c>
#include <wmkc_sort.c>
#include <wmkc_pad.c>
#include <wmkc_key.c>

#include <zlib/zlib.h>
#include <libpng/png.h>
#include <cjson/cJSON.h>
#include <iconv/iconv.h>

#define CIPHER_TEST true

#if defined(WMKC_SNC) && (CIPHER_TEST)
static wmkcByte testKey[96] = {
    0x67, 0x65, 0x5b, 0x7b, 0x33, 0x78, 0x74, 0x6e, 0x49, 0x62, 0x6d, 0x3b, 0x7a, 0x77, 0x5d, 0x53,
    0x71, 0x52, 0x4a, 0x30, 0x3e, 0x2e, 0x63, 0x24, 0x67, 0x3b, 0x6e, 0x61, 0x6b, 0x3a, 0x21, 0x73,
    0x30, 0x66, 0x6c, 0x23, 0x74, 0x76, 0x45, 0x5d, 0x21, 0x3d, 0x50, 0x79, 0x60, 0x6c, 0x5d, 0x75,
    0x70, 0x6a, 0x68, 0x66, 0x52, 0x6e, 0x55, 0x4c, 0x7c, 0x51, 0x4e, 0x28, 0x31, 0x72, 0x7a, 0x40,
    0x49, 0x4e, 0x3b, 0x36, 0x6b, 0x69, 0x54, 0x37, 0x36, 0x70, 0x30, 0x3e, 0x3d, 0x60, 0x73, 0x42,
    0x5f, 0x61, 0x6f, 0x79, 0x7a, 0x36, 0x5e, 0x6d, 0x35, 0x40, 0x5f, 0x6d, 0x63, 0x57, 0x49, 0x31};
static wmkcByte testIv[32] = {
    0x7b, 0x75, 0x76, 0x37, 0x24, 0x59, 0x72, 0x29, 0x6e, 0x40, 0x5f, 0x65, 0x2d, 0x55, 0x75, 0x68,
    0x3d, 0x41, 0x78, 0x35, 0x48, 0x50, 0x7d, 0x73, 0x60, 0x4e, 0x33, 0x6f, 0x23, 0x47, 0x4c, 0x36};
#endif

void test()
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    wmkcByte inbuf[16] = {
        0x0, 0x1, 0x2, 0x3,
        0x4, 0x5, 0x6, 0x7,
        0x8, 0x9, 0xa, 0xb,
        0xc, 0xd, 0xe, 0xf};
    wmkcByte outbuf[16] = {};
    wmkc_u32 inbuf_size = 16;
    wmkc_u32 outbuf_size = sizeof outbuf;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_ctr(), NULL, testKey, testIv);
    EVP_EncryptUpdate(ctx, outbuf, &outbuf_size, inbuf, inbuf_size);
    EVP_CIPHER_CTX_free(ctx);

    wmkcMisc_PRINT(outbuf, outbuf_size, 16, 0, 0);
}

int main(int argc, char **argv)
{
    test();

    return 0;
}
