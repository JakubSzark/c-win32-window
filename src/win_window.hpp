// ====================
// Win_Windows.hpp
// Header for Creating a Window
// on the Windows Platform
// ====================

#pragma once

#include "szark_core.hpp"

#ifdef __MINGW32__
    /* Variables */

    static HDC hdc;
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

                    RECT rect;
                    GetClientRect(hWnd, &rect);
                    gOptions.windowSize = rect.right - rect.left;

                    glEnable(GL_TEXTURE_2D);
                    initDrawTarget();

                    if (gOptions.onOpened != nullptr)
                        gOptions.onOpened();
                }
                break;
            case WM_PAINT: 
                {
                    SwapBuffers(hdc);
                    mainRender();

                    if (gOptions.onLoop != nullptr)
                        gOptions.onLoop();
                }
                break;
            case WM_DESTROY: 
                {
                    wglDeleteContext(wgl_context);
                    PostQuitMessage(0);

                    if (gOptions.onClosed != nullptr)
                        gOptions.onClosed();
                }
                break;
            default:
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
                break;
        }

        return 0;
    }

    bool createWindow(WindowOptions& t_options) 
    {
        gOptions = t_options;
        const wchar_t CLASS_NAME[] = L"DesktopApp";
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASS wc = { };

        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        HWND hWnd = CreateWindowEx(
            0, CLASS_NAME, gOptions.title,
            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
            0, 0, gOptions.windowSize, gOptions.windowSize,
            NULL, NULL, hInstance, NULL 
        );

        if (!hWnd)
        {
            printf("Could not create Window!");
            return false;
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

        return true;
    }
#endif