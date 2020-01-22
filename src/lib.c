/*
    This source file is for compiling the 
    library into either a .dll or a .so
    Please put all includes here.
    Library made by: Jakub P. Szarkowicz
*/

#include "defines.h"

PIXELFORMATDESCRIPTOR GeneratePFD()
{
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nVersion = 1;
    
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.iPixelType = PFD_TYPE_RGBA;

    pfd.cColorBits = 32;
    pfd.cStencilBits = 8;
    pfd.cDepthBits = 24;

    return pfd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam) 
{
    HDC hdc = GetDC(hWnd);
    Window* window = (Window*)lParam;

    switch (uMsg)
    {
        case WM_CREATE: 
            {
                // Set Context Pixel Format Descriptor
                PIXELFORMATDESCRIPTOR pfd = GeneratePFD();
                SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

                // Make OpenGL work on current Context
                window->glContext = wglCreateContext(hdc);
                wglMakeCurrent(hdc, window->glContext);
                window->eventCallback(Opened);
            }
            break;
        case WM_PAINT: 
            SwapBuffers(hdc);
            break;
        case WM_DESTROY: 
            PostQuitMessage(0);
            wglDeleteContext(window->glContext);
            window->eventCallback(Closed);                
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
            break;
    }

    return 0;
}

Window* createWindow(wstr title, int width, int height)
{
    // Creating a Window Struct
    Window* window = (Window*)calloc(1, sizeof(Window));

    window->glContext = NULL;
    window->eventCallback = NULL;

    window->title = title;
    window->height = height;
    window->width = width;

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpszClassName = L"DesktopApp";

    RegisterClass(&wc);

    window->hWnd = CreateWindowEx(
        0, wc.lpszClassName, title, 
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        0, 0, width, height, NULL, NULL, 
        wc.hInstance, NULL 
    );

    // Check for Window Creation Failure
    if (!window->hWnd) return NULL;
    return window;    
}

void showWindow(const Window* window)
{
    ShowWindow(window->hWnd, SW_SHOW);
    UpdateWindow(window->hWnd);
}

void pollEvents(const Window* self)
{
    MSG msg;
    while (GetMessage(&msg, NULL, 0, (LPARAM)self))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void cleanupWindow(Window* window) 
{
    if (window != NULL) 
    {
        free(window);
        window = NULL;
    }
}

bool setTitle(const Window* self, wstr title) {
    return SetWindowText(self->hWnd, title);
}

void setSize(Window* self, int width, int height)
{
    self->width = width;
    self->height = height;

    if (!self->fullscreen) 
    {
        SetWindowPos(self->hWnd, NULL, 0, 0, 
            width, height, SWP_SHOWWINDOW);
        UpdateWindow(self->hWnd);
    }

    self->eventCallback(Resized);
}

void setFullscreen(Window* self, bool isFullscreen)
{
    self->fullscreen = isFullscreen;

    Dimension dimension = { 
        0, 0, self->width, self->height 
    };

    if (self->fullscreen)
    {
        MONITORINFO monitor_info;
        monitor_info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(MonitorFromWindow(NULL, 
            MONITOR_DEFAULTTONEAREST), &monitor_info);
        RECT rect = monitor_info.rcMonitor;

        dimension.x = rect.left;
        dimension.y = rect.top;

        dimension.width = rect.right - rect.left;
        dimension.height = rect.bottom - rect.top;
    }
    
    SetWindowLong(self->hWnd, GWL_STYLE, self->fullscreen ? 
        STYLE_FULLSCREEN : STYLE_WINDOWED);

    SetWindowPos(self->hWnd, NULL, dimension.x, dimension.y, 
        dimension.width, dimension.height, 0);
    UpdateWindow(self->hWnd);
}

int main() {
    return 0; 
}