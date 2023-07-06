// #include <image/wmkc_png.c>
// #include <wmkc_hexdump.c>
// #include <wmkc_winapi.c>
// #include <wmkc_dict.c>
// #include <wmkc_sort.c>
// #include <wmkc_key.c>
// #include <crypto/snc_412.c>

// #include <network/wmkc_http.c>
// #include <network/wmkc_ddos.c>
// #include <network/wmkc_net.c>
// #include <crypto/wmkc_crypto.c>
// #include <crypto/snc.c>
// #include <wmkc_binascii.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
// #include <wmkc_base64.c>
// #include <wmkc_object.c>
#include <wmkc_random.c>
// #include <wmkc_stream.c>
#include <wmkc_struct.c>
// #include <wmkc_thread.c>
#include <wmkc_basic.c>
// #include <wmkc_coder.c>
#include <wmkc_img.c>
// #include <wmkc_chat.c>
// #include <wmkc_file.c>
// #include <wmkc_hash.c>
// #include <wmkc_time.c>
// #include <wmkc_math.c>
#include <wmkc_misc.c>
// #include <wmkc_pad.c>

// #include <zlib/zlib.h>
// #include <libpng/png.h>
// #include <cjson/cJSON.h>
// #include <iconv/iconv.h>
// #include <openssl/evp.h>

#if 0
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

wmkcVoid test()
{
    wmkcImg_png_obj *ihdr = wmkcNull;
    wmkcImg_png_chunk_obj *chunk = wmkcNull;
    wmkcErr_obj error;

    error = wmkcImg_png_setIHDR(&ihdr, 0x05B6, 0x0624,
                        wmkcImg_png_bitDeep_8,
                        wmkcImg_png_colorType_rgba,
                        0, 0, 0);
    if(error.code) printf("%s\n", error.message);

    printf("IHDR Size: "); wmkcMisc_PRINT(ihdr->size, 4, 4, 0, 0);
    printf("IHDR Name: "); wmkcMisc_PRINT(ihdr->name, 4, 4, 0, 0);
    printf("IHDR Data: "); wmkcMisc_PRINT(ihdr->data, ihdr->size_n, 32, 1, 0);
    printf("IHDR CRC:  "); wmkcMisc_PRINT(ihdr->crc, 4, 4, 1, 0);

    error = wmkcImg_png_buildChunk(&chunk, ihdr);
    if(error.code) printf("%s\n", error.message);

    printf("Chunk data:\n");
    wmkcMisc_PRINT(chunk->data, chunk->size, 32, chunk->size % 32, 0);

    wmkcMem_free(chunk->data);
    wmkcMem_free(chunk);
    wmkcMem_free(ihdr);
}

wmkc_u32 main(wmkc_u32 argc, char **argv)
{
    void (*func_call)() = test;
    func_call();
    return 0;
}
