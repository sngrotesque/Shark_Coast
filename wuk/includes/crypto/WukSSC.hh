#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <WukMemory.hh>
#include <config/WukException.hh>

namespace wuk {
    namespace crypto {
        constexpr wU32 WUK_SSC_KSLEN  = 64; // key stream length
        constexpr wU32 WUK_SSC_KEYLEN = 32; // key length
        constexpr wU32 WUK_SSC_IVLEN  = 16; // IV length

        // SN Stream Cipher
        class LIBWUK_API SSC {
        private:
            wByte counter[WUK_SSC_KSLEN]{};

            void keystream_update();

        public:
            SSC() = default;
            SSC(const wByte *key, const wByte *iv);
        };
    }
}

#endif
