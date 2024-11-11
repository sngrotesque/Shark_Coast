#include <WukWin32Api.hh>

#include <WukWin32Api.cc>

#include <iostream>
using namespace std;

int main()
{
    wuk::WinApi api;

    api.set_wallpaper(wstring{L"F:/Pitchers/Backgrounds/壁纸-16d9/background_00.png"});

    return 0;
}
