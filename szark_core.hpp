#define WINDOWS
#define UNICODE

/* Standard Libaries */

#include <stdio.h>
#include <functional>
#include <stdlib.h>

/* Structs */

struct WindowOptions
{
    const wchar_t* title;
    unsigned int width, height;

    std::function<void()> onOpened = nullptr;
    std::function<void()> onLoop = nullptr;
    std::function<void()> onClosed = nullptr;
};

/* Function Definitions */

int createWindow(WindowOptions& options);

/* Platform Specifics */

#ifdef WINDOWS

    #pragma comment(lib, "opengl32.lib")

    #include "GL/gl.h"

    #include <windows.h>
    #include <tchar.h>

    /* Variables */

    static HDC hdc;
    static WindowOptions options;
    static HGLRC wgl_context;

    /* Required Windows Desktop Functions */

    PIXELFORMATDESCRIPTOR createPFD()
    {
        return PIXELFORMATDESCRIPTOR
        {
            .nSize = sizeof(PIXELFORMATDESCRIPTOR),
            .nVersion = 1,
            .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            .iPixelType = PFD_TYPE_RGBA,
            .cColorBits = 32,
            .cDepthBits = 24,
            .cStencilBits = 8,
            .iLayerType = PFD_MAIN_PLANE
        };
    }

    LRESULT CALLBACK WndProc(
        _In_ HWND   hWnd,
        _In_ UINT   uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam
    ) {
        switch (uMsg)
        {
            case WM_CREATE: 
                {
                    auto pfd = createPFD();
                    hdc = GetDC(hWnd);
                    SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
                    wgl_context = wglCreateContext(hdc);
                    wglMakeCurrent(hdc, wgl_context);

                    if (options.onOpened != nullptr)
                        options.onOpened();
                }
                break;
            case WM_PAINT: 
                {
                    glViewport(0, 0, 800, 600);
                    glClearColor(1, 0, 0, 1);
                    glClear(GL_COLOR_BUFFER_BIT);
                    SwapBuffers(hdc);

                    if (options.onLoop != nullptr)
                        options.onLoop();
                }
                break;
            case WM_DESTROY: 
                {
                    wglDeleteContext(wgl_context);
                    PostQuitMessage(0);

                    if (options.onClosed != nullptr)
                        options.onClosed();
                }
                break;
            default:
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
                break;
        }

        return 0;
    }

    int createWindow(WindowOptions& t_options) 
    {
        options = t_options;
        const wchar_t CLASS_NAME[] = L"DesktopApp";
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASS wc = { };

        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        HWND hWnd = CreateWindowEx(
            0, CLASS_NAME, options.title,
            WS_OVERLAPPEDWINDOW,
            0, 0, options.width, options.height,
            NULL, NULL, hInstance, NULL 
        );

        if (!hWnd)
        {
            MessageBox(
                NULL, 
                L"Could not create Window!", 
                L"ERROR", 0
            );
            return 1;
        }

        /* Shows the Window */
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return (int)msg.wParam;
    }

#endif