#include <config/WukConfig.hh>

#if WUK_SUPPORT
#ifndef WUK_CPP_WINAPI
#define WUK_CPP_WINAPI
#include <config/WukException.hh>
#include <WukMemory.hh>

#include <Windows.h>

// namespace wuk {
//     namespace winapi {
//         struct widthHeight {
//             wU32 width;
//             wU32 height;
//         };
//         struct widthHeight get_screen_resolution();
//         struct widthHeight get_cursor_pos();
//         std::string get_username(char format = 'A');
//         void set_cursor_pos(wU32 x, wU32 y);
//         void set_desktop_wallpaper(std::wstring &path);
//         void opacity_startmenu(BYTE alpha);
//     };
// }

namespace wuk {
    class WinApi {
    private:
        char err_msg[256];
        wuk::Error err_code;
    
    private:
        void get_error() noexcept;

    public:
        WinApi() = default;

        POINT get_cursor_pos();
        void set_cursor_pos(wI32 x, wI32 y) const noexcept;
        void set_cursor_pos(POINT pos) const noexcept;
        void set_cursor_pos(MOUSEINPUT m) const noexcept;

        template <typename T>
        T get_username();

        template <typename T>
        void set_wallpaper(T path) const noexcept;
    };
}

#endif
#endif
