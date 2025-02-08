#include <crypto/WukFEA.hh>
#include <crypto/WukUtils.hh>
#include <WukPadding.hh>
#include <WukMisc.hh>

#include <crypto/WukFEA.cc>
#include <crypto/WukFEA_ECB.cc>
#include <crypto/WukFEA_CBC.cc>
#include <crypto/WukFEA_CTR.cc>
#include <crypto/WukFEA_CFB.cc>
#include <crypto/WukUtils.cc>
#include <WukPadding.cc>
#include <WukMisc.cc>

#include <iostream>
#include <chrono>

#include <WukRandom.hh>
#include <WukRandom.cc>

const wByte global_key[32] = {
    0x25, 0xb4, 0xbe, 0x61, 0xc4, 0xd5, 0xd6, 0x7c,
    0xa7, 0xd7, 0x5d, 0x02, 0xea, 0x2a, 0x06, 0x1a,
    0xa9, 0x79, 0xcc, 0x98, 0xc7, 0x37, 0x6b, 0x43,
    0x1e, 0x69, 0xf5, 0x56, 0xcb, 0x3d, 0x9a, 0x4e};
const wByte global_iv[16] = {
    0xc7, 0x28, 0x41, 0xd5, 0x82, 0xdd, 0x6e, 0x8e,
    0xb2, 0x6b, 0xd3, 0x74, 0xee, 0xd0, 0x96, 0xb3};

void test1(wuk::crypto::mode xcrypt_mode = wuk::crypto::mode::CBC)
{
    switch (xcrypt_mode) {
        case wuk::crypto::mode::ECB:
            std::cout << wuk::color::fore::cyan
                    << "Current mode: ECB"
                    << wuk::color::all::reset
                    << std::endl;
            break;
        case wuk::crypto::mode::CBC:
        std::cout << wuk::color::fore::cyan
                << "Current mode: CBC"
                << wuk::color::all::reset
                << std::endl;
            break;
        case wuk::crypto::mode::CTR:
        std::cout << wuk::color::fore::cyan
                << "Current mode: CTR"
                << wuk::color::all::reset
                << std::endl;
            break;
        case wuk::crypto::mode::CFB:
        std::cout << wuk::color::fore::cyan
                << "Current mode: CFB"
                << wuk::color::all::reset
                << std::endl;
            break;
    }

    wuk::crypto::Counter counter(std::string{"hello,world.\n"}, 2190);
    wuk::crypto::FEA fea(global_key, global_iv, counter);

    const char *_plaintext = {
        "MiSide"
    };
    wSize length = strlen(_plaintext);

    wByte *padded = wuk::x923_pad(reinterpret_cast<const wByte *>(_plaintext),
                                    length, wuk::crypto::WUK_FEA_BL);

    std::cout << "Plaintext (Padded):\n";
    wuk::misc::print_hex(padded, length, 16, true, true);

    switch (xcrypt_mode) {
        case wuk::crypto::mode::ECB:
            for (wU32 i = 0; i < length; i += wuk::crypto::WUK_FEA_BL) {
                fea.encrypt(padded + i, length, xcrypt_mode);
            }
            break;
        case wuk::crypto::mode::CTR:
            fea.encrypt(padded, length, xcrypt_mode);
            // 重置上下文，用于解密
            fea = wuk::crypto::FEA(global_key, global_iv, counter);
            break;
        default:
            fea.encrypt(padded, length, xcrypt_mode);
            break;
    }

    std::cout << "Ciphertext:\n";
    wuk::misc::print_hex(padded, length, 16, true, true);
    // wuk::misc::print_pybytes(padded, length, true);

    switch (xcrypt_mode) {
        case wuk::crypto::mode::ECB:
            for (wU32 i = 0; i < length; i += wuk::crypto::WUK_FEA_BL) {
                fea.decrypt(padded + i, length, xcrypt_mode);
            }
            break;
        default:
            fea.decrypt(padded, length, xcrypt_mode);
            break;
    }

    // std::cout << "Plaintext (Unpadded):\n";
    // wByte *unpadded = wuk::x923_unpad(padded, length);
    // wuk::misc::print_hex(unpadded, length, 16, true, true);
    // wuk::m_free(unpadded);

    wuk::m_free(padded);
}

int main()
{
    try {
        // test1(wuk::crypto::mode::ECB);
        test1(wuk::crypto::mode::CBC);
        // test1(wuk::crypto::mode::CFB);
        // test1(wuk::crypto::mode::CTR);
    } catch (wuk::Exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
