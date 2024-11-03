#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_MEMORY
#define WUK_CPP_MEMORY

#ifdef WUK_PLATFORM_WINOS
#   include <Windows.h>
#endif

namespace wuk {
    LIBWUK_API void memory_zero(void *p, wSize n);
    LIBWUK_API void memory_secure(void *p, wSize n);

    template <typename T>
    LIBWUK_API T m_alloc(wSize size)
    {
        return static_cast<T>(malloc(size));
    }

    template <typename T>
    LIBWUK_API T m_realloc(T src, wSize size)
    {
        
    }
}

#endif /* WUK_CPP_MEMORY */
#endif /* WUK_SUPPORT */
