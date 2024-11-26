#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_PNG
#define WUK_CPP_PNG
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <WukBuffer.hh>

namespace wuk {
    class LIBWUK_API Png {
    public:
        typedef struct {
            wU32 width;
            wU32 height;
        } png_ihdr;
    };
}

#endif
#endif
