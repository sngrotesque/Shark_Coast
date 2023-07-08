// #include <wmkc_hexdump.c>
// #include <wmkc_winapi.c>
// #include <wmkc_dict.c>
// #include <wmkc_sort.c>
// #include <crypto/snc_412.c>

// #include <network/wmkc_http.c>
// #include <network/wmkc_ddos.c>
#include <network/wmkc_net.c>
#include <network/wmkc_tp.c>
// #include <crypto/wmkc_crypto.c>
#include <crypto/snc.c>
#include <crypto/rsa.c>
#include <wmkc_binascii.c>
#include <wmkc_memory.c>
#include <wmkc_common.c>
// #include <wmkc_base64.c>
// #include <wmkc_object.c>
#include <wmkc_random.c>
// #include <wmkc_stream.c>
#include <wmkc_struct.c>
// #include <wmkc_thread.c>
#include <wmkc_basic.c>
#include <wmkc_coder.c>
// #include <wmkc_chat.c>
#include <wmkc_file.c>
#include <wmkc_hash.c>
// #include <wmkc_time.c>
// #include <wmkc_math.c>
#include <wmkc_misc.c>
#include <wmkc_img.c>
// #include <wmkc_pad.c>

#if 1
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
    wmkcRsa_obj *rsa = wmkcNull;
    wmkcRsa_new(&rsa);

    rsa->key_bit = 2048;

    printf("rsa:           %p\n", rsa);
    printf("rsa->key_bit:  %016x\n", rsa->key_bit);
    printf("rsa->pub_enc:  %p\n", rsa->pub_enc);
    printf("rsa->pub_dec:  %p\n", rsa->pub_dec);
    printf("rsa->pri_enc:  %p\n", rsa->pri_enc);
    printf("rsa->pri_dec:  %p\n", rsa->pri_dec);
    printf("rsa->pub:      %p\n", rsa->pub);
    printf("rsa->pub->key: %p\n", rsa->pub->key);
    printf("rsa->pub->n:   %p\n", rsa->pub->n);
    printf("rsa->pub->e:   %p\n", rsa->pub->e);
    printf("rsa->pri:      %p\n", rsa->pri);
    printf("rsa->pri->key: %p\n", rsa->pri->key);
    printf("rsa->pri->n:   %p\n", rsa->pri->n);
    printf("rsa->pri->e:   %p\n", rsa->pri->e);
    printf("rsa->pri->d:   %p\n", rsa->pri->d);
    printf("rsa->pri->q:   %p\n", rsa->pri->q);
    printf("rsa->pri->p:   %p\n", rsa->pri->p);

    wmkcRsa_newkeys(rsa, 2048);

}

wmkc_u32 main(wmkc_u32 argc, wmkcChar **argv)
{
    void (*func_call)() = test;
    func_call();
    return 0;
}
