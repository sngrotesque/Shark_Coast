#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <assert.h>

namespace wuk {
    namespace crypto {
        class LIBWUK_API ChaCha20 {
        private:
            typedef wByte _key256[32];
            typedef wByte _nonce96[12];

            _key256 key;
            _nonce96 nonce;

            wU32 state[16];
            wU32 counter;
        public:
            ChaCha20(wByte *key, wByte *nonce, wU32 counter = 0);
            void init();
            void xcrypt(wByte *buffer, wSize length);
        };
    }
}

#endif
