#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_CRYPTO_UTILS
#define WUK_CPP_CRYPTO_UTILS
#include <config/WukException.hh>

namespace wuk {
    namespace crypto {
        class LIBWUK_API Counter {
        private:
            wByte counter[16];

        public:
            Counter();
            Counter(const wByte *nonce, wU32 size, wSize begin);
            Counter(const char *nonce, wU32 size, wSize begin);
            Counter(std::string nonce, wSize begin);

            wByte *get() noexcept;
        };
    }
}

#endif
#endif
