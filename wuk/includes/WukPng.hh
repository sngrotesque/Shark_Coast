#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_PNG
#define WUK_CPP_PNG
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukBuffer.hh>

namespace wuk {
    namespace png {
        class LIBWUK_API ihdr {
        public:
            w_u32 width;
            w_u32 height;
        };
    }
}

#endif
#endif
