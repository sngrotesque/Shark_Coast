#include <crypto/fea.hpp>

static const wmkcByte sbox[4][256] = {
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x9f, 0x0e, 0x3b, 0xe0, 0xe2, 0xa4, 0x8d, 0x91, 0x45, 0x77, 0xc7, 0xbf, 0xd8, 0x4c, 0xb4, 0x64,
        0x9a, 0x94, 0xe3, 0x62, 0x21, 0x57, 0x51, 0x66, 0x1a, 0xca, 0x31, 0xb1, 0x29, 0x06, 0x3a, 0xa6,
        0xed, 0x37, 0x85, 0xdf, 0x0a, 0x7f, 0xd6, 0xe6, 0xc2, 0xaf, 0x3d, 0x1d, 0x90, 0x75, 0x7a, 0x55,
        0xb3, 0xf9, 0x86, 0xd5, 0xc9, 0x52, 0x42, 0x74, 0x98, 0x61, 0x36, 0x4f, 0xbe, 0x02, 0x99, 0x84,
        0x50, 0x23, 0x22, 0x6f, 0x65, 0xf6, 0x27, 0x35, 0x8f, 0x97, 0xe4, 0x67, 0x40, 0x32, 0x70, 0xc1,
        0xec, 0xe5, 0x54, 0xef, 0x33, 0x1f, 0x53, 0x95, 0x10, 0xea, 0xd0, 0x14, 0x81, 0x20, 0x8c, 0x11,
        0xf3, 0x4d, 0x09, 0x2a, 0xd3, 0x63, 0x89, 0x59, 0x82, 0xc5, 0x0b, 0x05, 0x13, 0xe7, 0xae, 0xda,
        0x1c, 0xb5, 0x87, 0xff, 0x2b, 0x69, 0x49, 0x18, 0x0c, 0xce, 0x3c, 0x7c, 0x44, 0x5d, 0x71, 0x25,
        0x4b, 0xc3, 0x04, 0x7d, 0x6b, 0x41, 0xab, 0xb2, 0x08, 0x2e, 0x2f, 0x7b, 0x2d, 0xdc, 0xb8, 0x00,
        0x39, 0x03, 0x83, 0x46, 0x9e, 0x88, 0xde, 0x8e, 0xfd, 0xd7, 0xfe, 0x78, 0xd2, 0xf1, 0x0d, 0x0f,
        0x68, 0x16, 0xf5, 0xe9, 0xcc, 0x93, 0xa1, 0x8b, 0xf8, 0x28, 0xc0, 0x6c, 0x8a, 0xe1, 0xf2, 0x5e,
        0x80, 0x5a, 0x9b, 0x6a, 0xbc, 0xcd, 0xcb, 0x6d, 0x17, 0x5b, 0xa5, 0xfb, 0x3e, 0xbd, 0x19, 0x4e,
        0xfa, 0xb7, 0x92, 0xb6, 0xf7, 0x7e, 0xc4, 0x1e, 0x76, 0x24, 0xb9, 0xba, 0x30, 0xb0, 0xa0, 0xdb,
        0x6e, 0xa2, 0x4a, 0x43, 0x2c, 0x73, 0x96, 0xee, 0x48, 0xd1, 0xaa, 0xd9, 0x01, 0x15, 0x72, 0xa8,
        0x3f, 0xad, 0x5c, 0xa9, 0xd4, 0xa7, 0xeb, 0xc6, 0x38, 0xf0, 0xfc, 0xbb, 0x9c, 0xdd, 0x47, 0x1b,
        0x60, 0xe8, 0x07, 0x9d, 0xf4, 0x12, 0x26, 0xc8, 0x34, 0x58, 0xa3, 0x79, 0xcf, 0x5f, 0xac, 0x56
    },
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x7f, 0xf3, 0xe8, 0x1d, 0x28, 0x93, 0x03, 0x2b, 0x1e, 0xdc, 0xcc, 0x44, 0x7e, 0x6c, 0x6e, 0xd1,
        0x88, 0xea, 0xd5, 0xef, 0x34, 0xc4, 0x17, 0x49, 0x39, 0x0b, 0xfe, 0xe0, 0x9e, 0xd0, 0x32, 0x5b,
        0x19, 0x2d, 0xb5, 0x54, 0x79, 0x71, 0xaf, 0x8e, 0x20, 0x94, 0x11, 0x64, 0x13, 0xb6, 0xc7, 0xb9,
        0xd7, 0x8a, 0xdd, 0x80, 0x2a, 0x60, 0x70, 0x65, 0xdf, 0x7b, 0xf5, 0x10, 0xae, 0x08, 0xda, 0x1a,
        0x9c, 0x00, 0xd9, 0x6a, 0xaa, 0x84, 0xcf, 0x92, 0x48, 0xe3, 0xce, 0xc2, 0x06, 0xa2, 0x1c, 0x62,
        0x02, 0xb2, 0xee, 0x35, 0xb8, 0x82, 0x7c, 0x67, 0xfb, 0x3e, 0x4f, 0x5f, 0x22, 0x36, 0xec, 0x25,
        0x3c, 0x87, 0x38, 0x77, 0x8f, 0xf8, 0x0d, 0x6b, 0x18, 0x9b, 0x26, 0x3d, 0x4d, 0xe1, 0xc0, 0xf7,
        0xd8, 0xcb, 0x97, 0xf2, 0xac, 0xbb, 0xad, 0x95, 0x15, 0xbd, 0xe2, 0x96, 0xf6, 0x2f, 0x31, 0xbe,
        0xf0, 0xca, 0x5d, 0xbf, 0x75, 0x21, 0x46, 0xb4, 0xf9, 0x83, 0x86, 0x0f, 0x76, 0x05, 0xd3, 0xab,
        0x8b, 0x3a, 0xc5, 0x99, 0x91, 0xc1, 0x2e, 0x24, 0x68, 0xba, 0xd6, 0x4c, 0xfc, 0x90, 0x12, 0x7a,
        0xa3, 0x3b, 0xfd, 0x07, 0x1f, 0xa8, 0x63, 0x16, 0x9d, 0xff, 0xc3, 0x33, 0xd4, 0xe6, 0x40, 0xfa,
        0x14, 0x37, 0x23, 0x1b, 0x4e, 0x09, 0x57, 0x0a, 0x27, 0xe4, 0xa0, 0xf4, 0x9a, 0xed, 0x0e, 0x4a,
        0xd2, 0x50, 0x89, 0xde, 0x7d, 0x59, 0x2c, 0x3f, 0xb1, 0xf1, 0xc8, 0x01, 0x04, 0xc6, 0x45, 0xb3,
        0xdb, 0xa4, 0x66, 0x6f, 0x0c, 0x41, 0x73, 0x61, 0xe7, 0x8d, 0x55, 0xe9, 0x5e, 0xbc, 0x98, 0xcd,
        0x56, 0x52, 0x43, 0x30, 0x9f, 0xa9, 0x53, 0xeb, 0xa7, 0x47, 0x81, 0x69, 0x51, 0x58, 0xa1, 0x78,
        0x5a, 0xa6, 0xb0, 0xe5, 0xb7, 0x85, 0x8c, 0x4b, 0xa5, 0x42, 0x29, 0x74, 0xc9, 0x6d, 0x72, 0x5c
    },
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0xe8, 0xe7, 0xbf, 0x3e, 0x33, 0x7b, 0xb0, 0x60, 0xc4, 0x59, 0x24, 0xc6, 0xdf, 0x9f, 0x14, 0x13,
        0x58, 0xfc, 0x8b, 0x63, 0xe0, 0x4b, 0xaf, 0xe6, 0xf2, 0xda, 0x3f, 0x7f, 0xfb, 0x92, 0x8e, 0xae,
        0x2d, 0xcb, 0x69, 0x48, 0xdd, 0x7c, 0x8f, 0xfe, 0xb9, 0x3a, 0xd5, 0xd9, 0xea, 0x42, 0xac, 0x9d,
        0x93, 0xce, 0x1d, 0x0e, 0x7d, 0x68, 0x87, 0x78, 0x84, 0x54, 0xa0, 0x36, 0x00, 0x3c, 0x07, 0xb6,
        0x6f, 0xf0, 0xed, 0x0f, 0x5c, 0x77, 0xe1, 0x35, 0x37, 0x27, 0x51, 0xc2, 0x32, 0xc7, 0x79, 0x06,
        0xf1, 0x6a, 0x55, 0x34, 0x2c, 0x29, 0xc5, 0x74, 0x0c, 0x8a, 0x26, 0xd3, 0x02, 0x9e, 0xec, 0xa2,
        0x0a, 0x88, 0x71, 0x25, 0x1c, 0xc3, 0xbe, 0xf6, 0x22, 0x2b, 0x6d, 0xd0, 0xc0, 0x47, 0x83, 0x5d,
        0x62, 0x2f, 0x30, 0x2a, 0xf7, 0x04, 0xd8, 0xde, 0x67, 0xef, 0x1e, 0x45, 0xb8, 0xb7, 0x81, 0xf9,
        0x17, 0xd1, 0x03, 0x98, 0x91, 0x66, 0x12, 0x2e, 0xbb, 0x08, 0xf4, 0x95, 0xc9, 0x5e, 0xb2, 0x09,
        0x64, 0x23, 0xb5, 0xd2, 0x20, 0x4f, 0x7a, 0x4d, 0xf5, 0x39, 0x11, 0x76, 0x1a, 0x89, 0xcf, 0xca,
        0x43, 0xb3, 0xb4, 0xd4, 0xc8, 0x0b, 0x3b, 0x05, 0x53, 0x1b, 0xc1, 0x31, 0x3d, 0x86, 0xa6, 0xa1,
        0x52, 0xee, 0x8d, 0x4c, 0x73, 0x44, 0x80, 0xd7, 0x49, 0x97, 0x19, 0x99, 0x85, 0xad, 0x41, 0xdc,
        0x9a, 0xab, 0x72, 0x16, 0xdb, 0x4e, 0xa9, 0x65, 0x5a, 0xe4, 0xe3, 0xf3, 0xbc, 0x7e, 0x70, 0xaa,
        0x8c, 0x1f, 0xe9, 0xa4, 0x6e, 0x61, 0x82, 0xb1, 0xcd, 0x5f, 0x18, 0x4a, 0xd6, 0x6b, 0x96, 0xa5,
        0xa8, 0x56, 0x57, 0xcc, 0x01, 0x46, 0x9b, 0xe2, 0xeb, 0xfd, 0xfa, 0xbd, 0x40, 0xf8, 0x9c, 0xe5,
        0x38, 0xa3, 0x50, 0x15, 0x5b, 0xa7, 0x6c, 0x0d, 0x28, 0x21, 0x75, 0xff, 0xba, 0x10, 0x90, 0x94
    },
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x16, 0xcf, 0x5b, 0x3b, 0x9f, 0xcd, 0xfe, 0x4f, 0x5f, 0x8a, 0xb7, 0x5e, 0x7b, 0x25, 0xea, 0xf5,
        0xcc, 0x49, 0xd7, 0xf4, 0x30, 0xb5, 0xc7, 0x07, 0x75, 0x4c, 0x98, 0x6f, 0xa6, 0x04, 0x68, 0xe4,
        0xc6, 0x32, 0xc1, 0x9d, 0xf2, 0x85, 0xb3, 0xa8, 0x46, 0x01, 0x79, 0xd5, 0xa9, 0xc3, 0x3e, 0xe5,
        0xae, 0x83, 0xd6, 0xd3, 0x7f, 0xbe, 0x08, 0x96, 0xbb, 0xc2, 0x97, 0x13, 0xdd, 0x9a, 0x41, 0x8f,
        0x2e, 0x5c, 0x2d, 0x73, 0xb9, 0x39, 0xfb, 0x20, 0xc8, 0xd0, 0x06, 0x71, 0xc0, 0x50, 0x0e, 0x53,
        0x35, 0xf1, 0xfa, 0xee, 0xfd, 0xb8, 0x26, 0x54, 0x69, 0x87, 0xb1, 0xf7, 0x1d, 0x55, 0x18, 0xa3,
        0x38, 0x72, 0xe8, 0x9b, 0xf9, 0x2a, 0x10, 0x7d, 0x74, 0x3d, 0xe6, 0xeb, 0x57, 0xb0, 0xbd, 0x3f,
        0x40, 0xe0, 0xe9, 0xf3, 0xa1, 0xff, 0x52, 0x1a, 0x24, 0x8d, 0x62, 0xa4, 0x34, 0x77, 0xde, 0x51,
        0x86, 0xd8, 0x00, 0x8c, 0xbc, 0xe1, 0x4b, 0x80, 0x5a, 0x61, 0x6d, 0x3a, 0xd9, 0xf0, 0x5d, 0xd1,
        0xa2, 0xba, 0xe3, 0x21, 0x17, 0xc4, 0x93, 0x37, 0x33, 0x6c, 0xdb, 0x19, 0x90, 0xb2, 0x15, 0xec,
        0xb4, 0xd4, 0x7e, 0x02, 0x91, 0x70, 0x89, 0x78, 0xd2, 0x44, 0xcb, 0x1f, 0x65, 0x48, 0xc5, 0x64,
        0x12, 0x1c, 0xed, 0x05, 0x7c, 0x60, 0x45, 0x6b, 0x09, 0xa0, 0xca, 0x23, 0x59, 0x31, 0x67, 0x66,
        0x0b, 0x88, 0xf8, 0xdf, 0xdc, 0x03, 0x7a, 0xaa, 0x28, 0x4d, 0x0f, 0xa7, 0x4e, 0x9e, 0xaf, 0x8b,
        0x6e, 0x2f, 0x81, 0xab, 0x27, 0xad, 0xf6, 0xc9, 0x29, 0xce, 0x58, 0x0c, 0x1b, 0xda, 0x95, 0xa5,
        0x9c, 0x43, 0x4a, 0x56, 0x3c, 0x2b, 0x1e, 0xe2, 0x76, 0x82, 0x36, 0x22, 0xfc, 0x47, 0x14, 0xe7,
        0xef, 0xac, 0x99, 0x11, 0x63, 0x0a, 0x94, 0x6a, 0xbf, 0x92, 0x84, 0xb6, 0x8e, 0x2c, 0x0d, 0x42
    }
};

static const wmkcByte rsbox[4][256] = {
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x8f, 0xdc, 0x3d, 0x91, 0x82, 0x6b, 0x1d, 0xf2, 0x88, 0x62, 0x24, 0x6a, 0x78, 0x9e, 0x01, 0x9f,
        0x58, 0x5f, 0xf5, 0x6c, 0x5b, 0xdd, 0xa1, 0xb8, 0x77, 0xbe, 0x18, 0xef, 0x70, 0x2b, 0xc7, 0x55,
        0x5d, 0x14, 0x42, 0x41, 0xc9, 0x7f, 0xf6, 0x46, 0xa9, 0x1c, 0x63, 0x74, 0xd4, 0x8c, 0x89, 0x8a,
        0xcc, 0x1a, 0x4d, 0x54, 0xf8, 0x47, 0x3a, 0x21, 0xe8, 0x90, 0x1e, 0x02, 0x7a, 0x2a, 0xbc, 0xe0,
        0x4c, 0x85, 0x36, 0xd3, 0x7c, 0x08, 0x93, 0xee, 0xd8, 0x76, 0xd2, 0x80, 0x0d, 0x61, 0xbf, 0x3b,
        0x40, 0x16, 0x35, 0x56, 0x52, 0x2f, 0xff, 0x15, 0xf9, 0x67, 0xb1, 0xb9, 0xe2, 0x7d, 0xaf, 0xfd,
        0xf0, 0x39, 0x13, 0x65, 0x0f, 0x44, 0x17, 0x4b, 0xa0, 0x75, 0xb3, 0x84, 0xab, 0xb7, 0xd0, 0x43,
        0x4e, 0x7e, 0xde, 0xd5, 0x37, 0x2d, 0xc8, 0x09, 0x9b, 0xfb, 0x2e, 0x8b, 0x7b, 0x83, 0xc5, 0x25,
        0xb0, 0x5c, 0x68, 0x92, 0x3f, 0x22, 0x32, 0x72, 0x95, 0x66, 0xac, 0xa7, 0x5e, 0x06, 0x97, 0x48,
        0x2c, 0x07, 0xc2, 0xa5, 0x11, 0x57, 0xd6, 0x49, 0x38, 0x3e, 0x10, 0xb2, 0xec, 0xf3, 0x94, 0x00,
        0xce, 0xa6, 0xd1, 0xfa, 0x05, 0xba, 0x1f, 0xe5, 0xdf, 0xe3, 0xda, 0x86, 0xfe, 0xe1, 0x6e, 0x29,
        0xcd, 0x1b, 0x87, 0x30, 0x0e, 0x71, 0xc3, 0xc1, 0x8e, 0xca, 0xcb, 0xeb, 0xb4, 0xbd, 0x3c, 0x0b,
        0xaa, 0x4f, 0x28, 0x81, 0xc6, 0x69, 0xe7, 0x0a, 0xf7, 0x34, 0x19, 0xb6, 0xa4, 0xb5, 0x79, 0xfc,
        0x5a, 0xd9, 0x9c, 0x64, 0xe4, 0x33, 0x26, 0x99, 0x0c, 0xdb, 0x6f, 0xcf, 0x8d, 0xed, 0x96, 0x23,
        0x03, 0xad, 0x04, 0x12, 0x4a, 0x51, 0x27, 0x6d, 0xf1, 0xa3, 0x59, 0xe6, 0x50, 0x20, 0xd7, 0x53,
        0xe9, 0x9d, 0xae, 0x60, 0xf4, 0xa2, 0x45, 0xc4, 0xa8, 0x31, 0xc0, 0xbb, 0xea, 0x98, 0x9a, 0x73
    },
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x41, 0xcb, 0x50, 0x06, 0xcc, 0x8d, 0x4c, 0xa3, 0x3d, 0xb5, 0xb7, 0x19, 0xd4, 0x66, 0xbe, 0x8b,
        0x3b, 0x2a, 0x9e, 0x2c, 0xb0, 0x78, 0xa7, 0x16, 0x68, 0x20, 0x3f, 0xb3, 0x4e, 0x03, 0x08, 0xa4,
        0x28, 0x85, 0x5c, 0xb2, 0x97, 0x5f, 0x6a, 0xb8, 0x04, 0xfa, 0x34, 0x07, 0xc6, 0x21, 0x96, 0x7d,
        0xe3, 0x7e, 0x1e, 0xab, 0x14, 0x53, 0x5d, 0xb1, 0x62, 0x18, 0x91, 0xa1, 0x60, 0x6b, 0x59, 0xc7,
        0xae, 0xd5, 0xf9, 0xe2, 0x0b, 0xce, 0x86, 0xe9, 0x48, 0x17, 0xbf, 0xf7, 0x9b, 0x6c, 0xb4, 0x5a,
        0xc1, 0xec, 0xe1, 0xe6, 0x23, 0xda, 0xe0, 0xb6, 0xed, 0xc5, 0xf0, 0x1f, 0xff, 0x82, 0xdc, 0x5b,
        0x35, 0xd7, 0x4f, 0xa6, 0x2b, 0x37, 0xd2, 0x57, 0x98, 0xeb, 0x43, 0x67, 0x0d, 0xfd, 0x0e, 0xd3,
        0x36, 0x25, 0xfe, 0xd6, 0xfb, 0x84, 0x8c, 0x63, 0xef, 0x24, 0x9f, 0x39, 0x56, 0xc4, 0x0c, 0x00,
        0x33, 0xea, 0x55, 0x89, 0x45, 0xf5, 0x8a, 0x61, 0x10, 0xc2, 0x31, 0x90, 0xf6, 0xd9, 0x27, 0x64,
        0x9d, 0x94, 0x47, 0x05, 0x29, 0x77, 0x7b, 0x72, 0xde, 0x93, 0xbc, 0x69, 0x40, 0xa8, 0x1c, 0xe4,
        0xba, 0xee, 0x4d, 0xa0, 0xd1, 0xf8, 0xf1, 0xe8, 0xa5, 0xe5, 0x44, 0x8f, 0x74, 0x76, 0x3c, 0x26,
        0xf2, 0xc8, 0x51, 0xcf, 0x87, 0x22, 0x2d, 0xf4, 0x54, 0x2f, 0x99, 0x75, 0xdd, 0x79, 0x7f, 0x83,
        0x6e, 0x95, 0x4b, 0xaa, 0x15, 0x92, 0xcd, 0x2e, 0xca, 0xfc, 0x81, 0x71, 0x0a, 0xdf, 0x4a, 0x46,
        0x1d, 0x0f, 0xc0, 0x8e, 0xac, 0x12, 0x9a, 0x30, 0x70, 0x42, 0x3e, 0xd0, 0x09, 0x32, 0xc3, 0x38,
        0x1b, 0x6d, 0x7a, 0x49, 0xb9, 0xf3, 0xad, 0xd8, 0x02, 0xdb, 0x11, 0xe7, 0x5e, 0xbd, 0x52, 0x13,
        0x80, 0xc9, 0x73, 0x01, 0xbb, 0x3a, 0x7c, 0x6f, 0x65, 0x88, 0xaf, 0x58, 0x9c, 0xa2, 0x1a, 0xa9
    },
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x3c, 0xe4, 0x5c, 0x82, 0x75, 0xa7, 0x4f, 0x3e, 0x89, 0x8f, 0x60, 0xa5, 0x58, 0xf7, 0x33, 0x43,
        0xfd, 0x9a, 0x86, 0x0f, 0x0e, 0xf3, 0xc3, 0x80, 0xda, 0xba, 0x9c, 0xa9, 0x64, 0x32, 0x7a, 0xd1,
        0x94, 0xf9, 0x68, 0x91, 0x0a, 0x63, 0x5a, 0x49, 0xf8, 0x55, 0x73, 0x69, 0x54, 0x20, 0x87, 0x71,
        0x72, 0xab, 0x4c, 0x04, 0x53, 0x47, 0x3b, 0x48, 0xf0, 0x99, 0x29, 0xa6, 0x3d, 0xac, 0x03, 0x1a,
        0xec, 0xbe, 0x2d, 0xa0, 0xb5, 0x7b, 0xe5, 0x6d, 0x23, 0xb8, 0xdb, 0x15, 0xb3, 0x97, 0xc5, 0x95,
        0xf2, 0x4a, 0xb0, 0xa8, 0x39, 0x52, 0xe1, 0xe2, 0x10, 0x09, 0xc8, 0xf4, 0x44, 0x6f, 0x8d, 0xd9,
        0x07, 0xd5, 0x70, 0x13, 0x90, 0xc7, 0x85, 0x78, 0x35, 0x22, 0x51, 0xdd, 0xf6, 0x6a, 0xd4, 0x40,
        0xce, 0x62, 0xc2, 0xb4, 0x57, 0xfa, 0x9b, 0x45, 0x37, 0x4e, 0x96, 0x05, 0x25, 0x34, 0xcd, 0x1b,
        0xb6, 0x7e, 0xd6, 0x6e, 0x38, 0xbc, 0xad, 0x36, 0x61, 0x9d, 0x59, 0x12, 0xd0, 0xb2, 0x1e, 0x26,
        0xfe, 0x84, 0x1d, 0x30, 0xff, 0x8b, 0xde, 0xb9, 0x83, 0xbb, 0xc0, 0xe6, 0xee, 0x2f, 0x5d, 0x0d,
        0x3a, 0xaf, 0x5f, 0xf1, 0xd3, 0xdf, 0xae, 0xf5, 0xe0, 0xc6, 0xcf, 0xc1, 0x2e, 0xbd, 0x1f, 0x16,
        0x06, 0xd7, 0x8e, 0xa1, 0xa2, 0x92, 0x3f, 0x7d, 0x7c, 0x28, 0xfc, 0x88, 0xcc, 0xeb, 0x66, 0x02,
        0x6c, 0xaa, 0x4b, 0x65, 0x08, 0x56, 0x0b, 0x4d, 0xa4, 0x8c, 0x9f, 0x21, 0xe3, 0xd8, 0x31, 0x9e,
        0x6b, 0x81, 0x93, 0x5b, 0xa3, 0x2a, 0xdc, 0xb7, 0x76, 0x2b, 0x19, 0xc4, 0xbf, 0x24, 0x77, 0x0c,
        0x14, 0x46, 0xe7, 0xca, 0xc9, 0xef, 0x17, 0x01, 0x00, 0xd2, 0x2c, 0xe8, 0x5e, 0x42, 0xb1, 0x79,
        0x41, 0x50, 0x18, 0xcb, 0x8a, 0x98, 0x67, 0x74, 0xed, 0x7f, 0xea, 0x1c, 0x11, 0xe9, 0x27, 0xfb
    },
    {// 0     1     2     3     4     5     6     7     8     9     a     n     c     d     e     f
        0x82, 0x29, 0xa3, 0xc5, 0x1d, 0xb3, 0x4a, 0x17, 0x36, 0xb8, 0xf5, 0xc0, 0xdb, 0xfe, 0x4e, 0xca,
        0x66, 0xf3, 0xb0, 0x3b, 0xee, 0x9e, 0x00, 0x94, 0x5e, 0x9b, 0x77, 0xdc, 0xb1, 0x5c, 0xe6, 0xab,
        0x47, 0x93, 0xeb, 0xbb, 0x78, 0x0d, 0x56, 0xd4, 0xc8, 0xd8, 0x65, 0xe5, 0xfd, 0x42, 0x40, 0xd1,
        0x14, 0xbd, 0x21, 0x98, 0x7c, 0x50, 0xea, 0x97, 0x60, 0x45, 0x8b, 0x03, 0xe4, 0x69, 0x2e, 0x6f,
        0x70, 0x3e, 0xff, 0xe1, 0xa9, 0xb6, 0x28, 0xed, 0xad, 0x11, 0xe2, 0x86, 0x19, 0xc9, 0xcc, 0x07,
        0x4d, 0x7f, 0x76, 0x4f, 0x57, 0x5d, 0xe3, 0x6c, 0xda, 0xbc, 0x88, 0x02, 0x41, 0x8e, 0x0b, 0x08,
        0xb5, 0x89, 0x7a, 0xf4, 0xaf, 0xac, 0xbf, 0xbe, 0x1e, 0x58, 0xf7, 0xb7, 0x99, 0x8a, 0xd0, 0x1b,
        0xa5, 0x4b, 0x61, 0x43, 0x68, 0x18, 0xe8, 0x7d, 0xa7, 0x2a, 0xc6, 0x0c, 0xb4, 0x67, 0xa2, 0x34,
        0x87, 0xd2, 0xe9, 0x31, 0xfa, 0x25, 0x80, 0x59, 0xc1, 0xa6, 0x09, 0xcf, 0x83, 0x79, 0xfc, 0x3f,
        0x9c, 0xa4, 0xf9, 0x96, 0xf6, 0xde, 0x37, 0x3a, 0x1a, 0xf2, 0x3d, 0x63, 0xe0, 0x23, 0xcd, 0x04,
        0xb9, 0x74, 0x90, 0x5f, 0x7b, 0xdf, 0x1c, 0xcb, 0x27, 0x2c, 0xc7, 0xd3, 0xf1, 0xd5, 0x30, 0xce,
        0x6d, 0x5a, 0x9d, 0x26, 0xa0, 0x15, 0xfb, 0x0a, 0x55, 0x44, 0x91, 0x38, 0x84, 0x6e, 0x35, 0xf8,
        0x4c, 0x22, 0x39, 0x2d, 0x95, 0xae, 0x20, 0x16, 0x48, 0xd7, 0xba, 0xaa, 0x10, 0x05, 0xd9, 0x01,
        0x49, 0x8f, 0xa8, 0x33, 0xa1, 0x2b, 0x32, 0x12, 0x81, 0x8c, 0xdd, 0x9a, 0xc4, 0x3c, 0x7e, 0xc3,
        0x71, 0x85, 0xe7, 0x92, 0x1f, 0x2f, 0x6a, 0xef, 0x62, 0x72, 0x0e, 0x6b, 0x9f, 0xb2, 0x53, 0xf0,
        0x8d, 0x51, 0x24, 0x73, 0x13, 0x0f, 0xd6, 0x5b, 0xc2, 0x64, 0x52, 0x46, 0xec, 0x54, 0x06, 0x75
    }
};

wmkcByte wmkcCrypto::wmkcFEA::subBytes(wmkcByte x)
{
    return sbox[3][sbox[2][sbox[1][sbox[0][x]]]];
}

wmkcByte wmkcCrypto::wmkcFEA::invSubBytes(wmkcByte x)
{
    return rsbox[0][rsbox[1][rsbox[2][rsbox[3][x]]]];
}

/////////////////////////////////////////////////////////////////////
wmkcCrypto::wmkcFEA::wmkcFEA()
{

}

wmkcCrypto::wmkcFEA::~wmkcFEA()
{

}

void wmkcCrypto::wmkcFEA::encrypt(wmkcByte *content, wmkcSize size)
{

}

void wmkcCrypto::wmkcFEA::decrypt(wmkcByte *content, wmkcSize size)
{

}


