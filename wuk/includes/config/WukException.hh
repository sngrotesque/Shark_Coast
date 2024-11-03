#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_EXCEPTION
#define WUK_CPP_EXCEPTION
#include <stdexcept>
#include <cerrno>

#ifdef WUK_STD_CPP_20
#   include <format>
#else
#   if defined(WUK_PLATFORM_WINOS) && defined(_MSC_VER)
#       pragma comment(lib, "msvcprt")
#   endif
#endif

namespace wuk {
    typedef enum : wI32 {
        OK     = 0, // 一切正常，无异常
        ERR    = 1, // 一切暂时无法定义的异常
        NPTR   = 2, // 调用空指针异常
        MEMORY = 3, // 内存管理异常
        FNOTF  = 4, // 文件未找到（File not found）
    } Error;

    class LIBWUK_API Exception : public std::exception {
        public:
            std::string output_message;
            Exception(wuk::Error code, const char *function, const char *message);
            const char *what() const noexcept override;
    };
}

#endif /* WUK_CPP_EXCEPTION */
#endif /* WUK_SUPPORT */
