#include "Renderer.h"
#include "Motherboard.h"


//? remove later
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <wchar.h>







HWND windowHandle;
WNDCLASSW windowClass;
HINSTANCE hInstance;


struct WindowFlags {
    bool running;
};
struct WindowFlags windowMain;
// window stuff


// event handler
// window handle, msg, and weird shit
LRESULT windProc(HWND windHandle, UINT msg, WPARAM wp, LPARAM lp) {
    LRESULT rez = 0;
    switch (msg) {
    case WM_CLOSE:
        if (MessageBox(windHandle, L"Want a break from the a*s?", L"", MB_YESNO) == IDYES) {
            windowMain.running = false;
        }
        break;
    default:
        rez = DefWindowProc(windHandle, msg, wp, lp);
    }

    return rez;
}


void testWindows() {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    hInstance = GetModuleHandle(0);



    windowClass.hCursor = LoadCursor(0, IDC_ARROW);

    windowClass.hInstance = hInstance;
    windowClass.lpszClassName = L"Main class";

    windowClass.style = CS_HREDRAW | CS_VREDRAW; // refresh window on resizing
    windowClass.lpfnWndProc = windProc;

    if (!RegisterClass(&windowClass)) {
        exit(-1);
    }

    windowHandle = CreateWindow(
        windowClass.lpszClassName,           
        L"SHE LIVES BITCHES",               // title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,   // default window
        CW_USEDEFAULT,                      // position x
        CW_USEDEFAULT,                      // position y
        CW_USEDEFAULT,                      // size x
        CW_USEDEFAULT,                      // size x
        0, 0, hInstance, 0
    );
    windowMain.running = true;

    while (windowMain.running) {
        if (GetAsyncKeyState('A') & 1) {
            printf("pressed a\n");
            ShowWindow(windowHandle, SW_SHOW);
        }

        MSG msg;
        while (PeekMessage(&msg, windowHandle, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    

} 



