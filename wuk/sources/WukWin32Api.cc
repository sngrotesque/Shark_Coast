#include <WukWin32Api.hh>

/*
#include <windows.h>
#include <Wininet.h>
#include <shlobj.h>
bool SetWallpaper(const std::wstring& file, DWORD style) {
    // 初始化COM组件
    CoInitialize(NULL);
    // 创建IActiveDesktop接口的实例
    IActiveDesktop* pIAD = NULL;
    HRESULT hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&pIAD);
    if (FAILED(hr)) return false;
    // 设置壁纸的文件路径
    hr = pIAD->SetWallpaper(file.c_str(), 0);
    if (FAILED(hr)) return false;
    // 设置壁纸的样式
    WALLPAPEROPT wpo;
    wpo.dwSize = sizeof(wpo);
    wpo.dwStyle = style;
    hr = pIAD->SetWallpaperOptions(&wpo, 0);
    if (FAILED(hr)) return false;
    // 应用壁纸的设置
    hr = pIAD->ApplyChanges(AD_APPLY_ALL);
    if (FAILED(hr)) return false;
    // 释放IActiveDesktop接口的实例
    pIAD->Release();
    // 反初始化COM组件
    CoUninitialize();
    return true;
}
*/

// void wuk::winapi::set_desktop_wallpaper(const std::wstring &path)
// {
//     bool succ = SystemParametersInfoW(
//         SPI_SETDESKWALLPAPER,
//         0,
//         nullptr,
//         SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE
//     );

//     if (!succ) {
//         throw wuk::Exception(static_cast<wuk::Error>(GetLastError()),
//             "wuk::winapi::set_desktop_wallpaper",
//             "SetCursorPos function returned an error code when called.");
//     }
// }

void wuk::WinApi::get_error() noexcept
{
    // return static_cast<wuk::Error>(GetLastError());
    wU32 last_error_code = GetLastError();

    this->err_code = static_cast<wuk::Error>(last_error_code);
    wuk::memory_zero(this->err_msg, sizeof(this->err_msg));

    FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        last_error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        this->err_msg,
        sizeof(this->err_msg),
        nullptr
    );
}

POINT wuk::WinApi::get_cursor_pos()
{
    POINT m{};
    if (!GetCursorPos(&m)) {
        this->get_error();
        throw wuk::Exception(this->err_code, "wuk::WinApi::get_cursor_pos",
            this->err_msg);
    }
    return m;
}

void wuk::WinApi::set_cursor_pos(wI32 x, wI32 y) const noexcept
{
    SetCursorPos(x, y);
}

void wuk::WinApi::set_cursor_pos(POINT pos) const noexcept
{
    SetCursorPos(pos.x, pos.y);
}

void wuk::WinApi::set_cursor_pos(MOUSEINPUT m) const noexcept
{
    SetCursorPos(m.dx, m.dy);
}

template <typename T>
T wuk::WinApi::get_username()
{
    // 关于`if constexpr`详见[C++ 17](https://zh.cppreference.com/w/cpp/17)
    using BUFFER_TYPE = std::conditional_t<std::is_same_v<T, std::string>, TCHAR, WCHAR>;
    WINBOOL (*GetUserNameProc)(BUFFER_TYPE *, DWORD *) = nullptr;

    // 不需要判断是否是其他类型，如果是那么编译阶段就无法通过
    if constexpr (std::is_same_v<T, std::string>) {
        GetUserNameProc = GetUserNameA;
    } else if constexpr (std::is_same_v<T, std::wstring>) {
        GetUserNameProc = GetUserNameW;
    }

    BUFFER_TYPE buffer[128]{};
    DWORD buffer_len = ARRAYSIZE(buffer);

    if (!GetUserNameProc(buffer, &buffer_len)) {
        this->get_error();
        throw wuk::Exception(this->err_code, "wuk::WinApi::get_username",
            this->err_msg);
    }

    return T{buffer, buffer_len};
}

template <typename T>
void wuk::WinApi::set_wallpaper(T path) const noexcept
{
    using TYPE = std::conditional_t<std::is_same_v<T, std::string>, TCHAR, WCHAR>;
    WINBOOL (*SystemParametersInfoProc)(UINT, UINT, PVOID, UINT) = nullptr;

    if constexpr (std::is_same_v<T, std::string>) {
        SystemParametersInfoProc = SystemParametersInfoA;
    } else if constexpr (std::is_same_v<T, std::wstring>) {
        SystemParametersInfoProc = SystemParametersInfoW;
    }

    SystemParametersInfoProc(
        SPI_SETDESKWALLPAPER,
        0,
        const_cast<TYPE *>(reinterpret_cast<const TYPE *>(path.c_str())),
        SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE
    );
}
