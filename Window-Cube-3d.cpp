#include "graph.h"



// Window processing function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
    /* Register the window class */ WNDCLASS wc = {}; HINSTANCE hInstance = GetModuleHandle(nullptr); const wchar_t CLASS_NAME[] = L"WindowClass"; wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance; wc.lpszClassName = CLASS_NAME; RegisterClass(&wc);/* Create the window */ const short windowWidth = 400; const short windowHeight = 400; HWND hwnd = CreateWindow(CLASS_NAME, L"Window Graphics", WS_OVERLAPPEDWINDOW, 0, 0, windowWidth + 16, windowHeight + 39, nullptr, nullptr, hInstance, nullptr); ShowWindow(hwnd, SW_SHOWNORMAL); /* ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL); *//* Window message */ MSG msg = {};
    /* ShowWindow(GetConsoleWindow(), SW_HIDE); */

    // Frame
    FRAME frame(windowWidth, windowHeight, hwnd);
    frame.clear({});

    fVECT3 VERTEXES[12] =
    {
        { 0, 0, 0 },
        { 400, 0, 0 },
        { 400, 400, 0 },
        { 0, 400, 0 },

        { 0, 0, 400 },
        { 400, 0, 400 },
        { 400, 400, 400 },
        { 0, 400, 400 },

        { 0, 0, -400 },
        { 400, 0, -400 },
        { 400, 400, -400 },
        { 0, 400, -400 }
    };



    
    frame.pen_color = { 0,255,0 };
    iVECT2 vect;

    for (const auto& vert : VERTEXES)
    {
        vect = frame.vect_projection(vert);
        frame.set_circle(vect.x, vect.y, 5);
    }

    frame.print();

    while (GetKeyState(VK_ESCAPE) >= 0)
    {
        // Processing window messages
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) break;
        }

    }

    return 0;
}