#pragma once
#include <config/WukConfig.hh>

#if WUK_SUPPORT
#include <config/WukException.hh>
#include <WukMemory.hh>
#include <sys/timeb.h>
#include <time.h>

#ifdef WUK_PLATFORM_WINOS
#include <Windows.h>
#elif defined(WUK_PLATFORM_LINUX)
#include <sys/time.h>
#include <unistd.h>
#endif

namespace wuk {
    class LIBWUK_API Time {
    public:
        Time() {}
        std::string get_now_time(std::string timeFormat = "%a, %m/%d %Y %H:%M:%S");
        void sleep(double _t);
        double time();
    };
}

#endif /* WUK_SUPPORT */
