#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>

namespace wuk {
    namespace crypto {
        class LIBWUK_API Counter {
        private:
            wByte counter[16];

        public:
            Counter();
            Counter(const wByte *nonce, wU32 size, wSize begin = 1);
            Counter(const char *nonce, wU32 size, wSize begin = 1);
            Counter(std::string nonce, wSize begin = 1);

            Counter(const Counter &other) noexcept;
            Counter(Counter &&other) noexcept;

            Counter &operator=(const Counter &other);
            Counter &operator=(Counter &&other);

            wByte *get() noexcept;
            void clean() noexcept;
        };
    }
}

#endif
