#include "graph.h"

// Window processing function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
    if (uMessage == WM_DESTROY) PostQuitMessage(0);
    else if (uMessage == WM_KEYDOWN)
    {
        if (wParam == 27) PostQuitMessage(0);
    }
    return DefWindowProc(hwnd, uMessage, wParam, lParam);
}

int main()
{
    /* Register the window class */ WNDCLASS wc = {}; HINSTANCE hInstance = GetModuleHandle(nullptr); const wchar_t CLASS_NAME[] = L"WindowClass"; wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance; wc.lpszClassName = CLASS_NAME; RegisterClass(&wc);/* Create the window */ const short windowWidth = 400; const short windowHeight = 400; HWND hwnd = CreateWindow(CLASS_NAME, L"Cube 3d", WS_OVERLAPPEDWINDOW, 0, 0, windowWidth + 16, windowHeight + 39, nullptr, nullptr, hInstance, nullptr); ShowWindow(hwnd, SW_SHOWNORMAL); /* ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL); *//* Window message */ MSG msg = {};
    ShowWindow(GetConsoleWindow(), SW_SHOWNORMAL);

    // Frame
    FRAME frame(windowWidth, windowHeight, hwnd);
    FIGURE_SURFACE figure("figure_surface/cube.dat");

    frame.pen_color = { 0, 255, 0 };
    
    while (true)
    {
        // Processing window messages
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT) break;
        }

        frame.clear({});

        figure.rotate(0, 1, 0, 200, 200, 100);

        frame.set_figure_surface(figure);
        frame.print();

        Sleep(2);
    }

    /* Clear */ DestroyWindow(hwnd); UnregisterClass(CLASS_NAME, hInstance);

    return 0;
}