#include <crypto/WukSSC.hh>

constexpr wByte sbox[256] = {
    0x80, 0x30, 0x4f, 0x85, 0x65, 0x8c, 0x6a, 0xaa, 0x1b, 0x87, 0xcd, 0xb1, 0x53, 0x6e, 0x39, 0x28,
    0x8d, 0xb3, 0xca, 0x0d, 0xa0, 0xfa, 0x72, 0x10, 0x44, 0x35, 0x31, 0x41, 0x9e, 0x79, 0xba, 0x9f,
    0x15, 0xbc, 0xf6, 0x26, 0x27, 0xf9, 0x8f, 0xc9, 0xb7, 0x7f, 0x70, 0x5a, 0x60, 0x24, 0x40, 0x1d,
    0x13, 0xd3, 0x89, 0x5c, 0xe6, 0x81, 0xf2, 0x0e, 0x2d, 0xb2, 0xd8, 0x04, 0xda, 0x2e, 0x50, 0x47,
    0x71, 0x06, 0x02, 0xe9, 0xea, 0xa1, 0xe7, 0xf8, 0xfb, 0x18, 0x7c, 0x7a, 0xbb, 0xfc, 0xa3, 0xb8,
    0x63, 0x3f, 0xb6, 0x97, 0xc2, 0x01, 0x6c, 0x5d, 0x3c, 0xa7, 0x58, 0xf4, 0xa2, 0x46, 0x82, 0xa9,
    0xce, 0xf3, 0xcf, 0x93, 0x45, 0x2c, 0xbf, 0xeb, 0xe8, 0x7d, 0x3a, 0xbe, 0x2f, 0x11, 0xac, 0xd9,
    0x95, 0xee, 0x9c, 0x32, 0xb5, 0xc3, 0x0a, 0x1f, 0x8e, 0x98, 0xe2, 0x1e, 0xd6, 0x17, 0xe1, 0xae,
    0x43, 0x56, 0xe4, 0x3e, 0xf1, 0xd4, 0x48, 0xcc, 0xe5, 0xc6, 0x49, 0xa4, 0x55, 0x1c, 0x6b, 0x77,
    0xcb, 0xef, 0x7b, 0x36, 0x8a, 0xc8, 0x23, 0xb0, 0x99, 0x29, 0xd1, 0xc5, 0x52, 0x3d, 0x05, 0x33,
    0x4a, 0xc7, 0x22, 0x94, 0xf5, 0x3b, 0x69, 0xff, 0xe3, 0x59, 0x5f, 0xd7, 0x96, 0x4e, 0x64, 0x00,
    0x25, 0x66, 0xb4, 0x88, 0x9b, 0xc4, 0x54, 0x37, 0xfe, 0xa5, 0x5b, 0x4b, 0x57, 0x09, 0xd5, 0x62,
    0xe0, 0x8b, 0xad, 0xdb, 0x0c, 0x73, 0x0f, 0x2a, 0xfd, 0x90, 0x42, 0x2b, 0x21, 0x9d, 0x0b, 0x20,
    0x6f, 0xdc, 0xab, 0x92, 0x08, 0xde, 0x68, 0x74, 0xec, 0xaf, 0x1a, 0x51, 0x19, 0x76, 0xc0, 0x7e,
    0x38, 0x78, 0x61, 0x86, 0xd2, 0xf0, 0x12, 0x84, 0x75, 0xdf, 0x34, 0xb9, 0x91, 0x67, 0xed, 0xdd,
    0x4d, 0xbd, 0x14, 0xd0, 0x5e, 0x16, 0xa6, 0x6d, 0x03, 0xc1, 0x4c, 0x9a, 0xa8, 0xf7, 0x83, 0x07
};

constexpr wByte rsbox[256] = {
    0xaf, 0x55, 0x42, 0xf8, 0x3b, 0x9e, 0x41, 0xff, 0xd4, 0xbd, 0x76, 0xce, 0xc4, 0x13, 0x37, 0xc6,
    0x17, 0x6d, 0xe6, 0x30, 0xf2, 0x20, 0xf5, 0x7d, 0x49, 0xdc, 0xda, 0x08, 0x8d, 0x2f, 0x7b, 0x77,
    0xcf, 0xcc, 0xa2, 0x96, 0x2d, 0xb0, 0x23, 0x24, 0x0f, 0x99, 0xc7, 0xcb, 0x65, 0x38, 0x3d, 0x6c,
    0x01, 0x1a, 0x73, 0x9f, 0xea, 0x19, 0x93, 0xb7, 0xe0, 0x0e, 0x6a, 0xa5, 0x58, 0x9d, 0x83, 0x51,
    0x2e, 0x1b, 0xca, 0x80, 0x18, 0x64, 0x5d, 0x3f, 0x86, 0x8a, 0xa0, 0xbb, 0xfa, 0xf0, 0xad, 0x02,
    0x3e, 0xdb, 0x9c, 0x0c, 0xb6, 0x8c, 0x81, 0xbc, 0x5a, 0xa9, 0x2b, 0xba, 0x33, 0x57, 0xf4, 0xaa,
    0x2c, 0xe2, 0xbf, 0x50, 0xae, 0x04, 0xb1, 0xed, 0xd6, 0xa6, 0x06, 0x8e, 0x56, 0xf7, 0x0d, 0xd0,
    0x2a, 0x40, 0x16, 0xc5, 0xd7, 0xe8, 0xdd, 0x8f, 0xe1, 0x1d, 0x4b, 0x92, 0x4a, 0x69, 0xdf, 0x29,
    0x00, 0x35, 0x5e, 0xfe, 0xe7, 0x03, 0xe3, 0x09, 0xb3, 0x32, 0x94, 0xc1, 0x05, 0x10, 0x78, 0x26,
    0xc9, 0xec, 0xd3, 0x63, 0xa3, 0x70, 0xac, 0x53, 0x79, 0x98, 0xfb, 0xb4, 0x72, 0xcd, 0x1c, 0x1f,
    0x14, 0x45, 0x5c, 0x4e, 0x8b, 0xb9, 0xf6, 0x59, 0xfc, 0x5f, 0x07, 0xd2, 0x6e, 0xc2, 0x7f, 0xd9,
    0x97, 0x0b, 0x39, 0x11, 0xb2, 0x74, 0x52, 0x28, 0x4f, 0xeb, 0x1e, 0x4c, 0x21, 0xf1, 0x6b, 0x66,
    0xde, 0xf9, 0x54, 0x75, 0xb5, 0x9b, 0x89, 0xa1, 0x95, 0x27, 0x12, 0x90, 0x87, 0x0a, 0x60, 0x62,
    0xf3, 0x9a, 0xe4, 0x31, 0x85, 0xbe, 0x7c, 0xab, 0x3a, 0x6f, 0x3c, 0xc3, 0xd1, 0xef, 0xd5, 0xe9,
    0xc0, 0x7e, 0x7a, 0xa8, 0x82, 0x88, 0x34, 0x46, 0x68, 0x43, 0x44, 0x67, 0xd8, 0xee, 0x71, 0x91,
    0xe5, 0x84, 0x36, 0x61, 0x5b, 0xa4, 0x22, 0xfd, 0x47, 0x25, 0x15, 0x48, 0x4d, 0xc8, 0xb8, 0xa7
};

void wuk::crypto::SSC::keystream_update()
{
    for (wI32 i = WUK_SSC_KSLEN - 1; i >= 0; --i) {
        if (this->counter[i] != 0xff) {
            this->counter[i]++;
            break;
        }
        this->counter[i] = 0x00;
    }
}

wuk::crypto::SSC::SSC(const wByte *key, const wByte *iv)
{
    
}
