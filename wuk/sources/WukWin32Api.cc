#include <WukWin32Api.hh>

struct wuk::winapi::widthHeight wuk::winapi::get_screen_resolution()
{
    wuk::winapi::widthHeight size;
    size.width = GetSystemMetrics(SM_CXSCREEN);
    size.height = GetSystemMetrics(SM_CYSCREEN);
    if(!size.width || !size.height) {
        throw wuk::Exception(wuk::Error::ERR, "wuk::winapi::get_screen_resolution",
            "GetSystemMetrics function returned an error code when called.");
    }
}

struct wuk::winapi::widthHeight wuk::winapi::get_cursor_pos()
{
    wuk::winapi::widthHeight point;
    POINT _point_tmp = {0};
    if(!GetCursorPos(&_point_tmp)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::winapi::get_cursor_pos",
            "GetCursorPos function returned an error code when called.");
    }
    point.width = _point_tmp.x;
    point.height = _point_tmp.y;
}

std::string wuk::winapi::get_username(char format = 'A')
{
    if(format == 'A') {
        char tmp_buffer[512]{0};
        DWORD tmp_buffer_size = sizeof(tmp_buffer);
        GetUserNameA(tmp_buffer, &tmp_buffer_size);
        return std::string(tmp_buffer);
    } else if(format == 'W') {
        WCHAR tmp_buffer[512]{0};
        DWORD tmp_buffer_size = sizeof(tmp_buffer) >> 1;
        GetUserNameW(tmp_buffer, &tmp_buffer_size);
        return std::string((char *)tmp_buffer, tmp_buffer_size);
    }
}

void wuk::winapi::set_cursor_pos(wU32 x, wU32 y)
{
    if(!SetCursorPos(x, y)) {
        throw wuk::Exception(static_cast<wuk::Error>(GetLastError()), "wuk::winapi::set_cursor_pos",
            "SetCursorPos function returned an error code when called.");
    }
}

// #include <windows.h>
// #include <Wininet.h>
// #include <shlobj.h>
// bool SetWallpaper(const std::wstring& file, DWORD style) {
//     // 初始化COM组件
//     CoInitialize(NULL);
//     // 创建IActiveDesktop接口的实例
//     IActiveDesktop* pIAD = NULL;
//     HRESULT hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void**)&pIAD);
//     if (FAILED(hr)) return false;
//     // 设置壁纸的文件路径
//     hr = pIAD->SetWallpaper(file.c_str(), 0);
//     if (FAILED(hr)) return false;
//     // 设置壁纸的样式
//     WALLPAPEROPT wpo;
//     wpo.dwSize = sizeof(wpo);
//     wpo.dwStyle = style;
//     hr = pIAD->SetWallpaperOptions(&wpo, 0);
//     if (FAILED(hr)) return false;
//     // 应用壁纸的设置
//     hr = pIAD->ApplyChanges(AD_APPLY_ALL);
//     if (FAILED(hr)) return false;
//     // 释放IActiveDesktop接口的实例
//     pIAD->Release();
//     // 反初始化COM组件
//     CoUninitialize();
//     return true;
// }

void wuk::winapi::set_desktop_wallpaper(std::string path)
{
    
}

void wuk::winapi::opacity_startmenu(BYTE alpha)
{
    
}
