#include "Core.h"

#ifdef OS_WINDOWS
    static HDC gHDC;
    static HGLRC gContext;

    /*
        Generates a description of how a
        context should render elements.
    */
    PIXELFORMATDESCRIPTOR GeneratePFD()
    {
        PIXELFORMATDESCRIPTOR pfd = { };
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

    /*
        Windows Procedure Function,
        Main Window Loop, Startup,
        Events, Cleanup.
    */
    LRESULT CALLBACK WndProc(
        _In_ HWND   hWnd, _In_ UINT   uMsg,
        _In_ WPARAM wParam, _In_ LPARAM lParam) 
    {
        switch (uMsg)
        {
            case WM_CREATE: 
                {
                    // Set Context Pixel Format Descriptor
                    gHDC = GetDC(hWnd);
                    PIXELFORMATDESCRIPTOR pfd = GeneratePFD();
                    SetPixelFormat(gHDC, ChoosePixelFormat(gHDC, &pfd), &pfd);

                    // Make OpenGL work on current Context
                    gContext = wglCreateContext(gHDC);
                    wglMakeCurrent(gHDC, gContext);
                    glEnable(GL_TEXTURE_2D);

                    Setup();

                    // Call User Open
                    if (gConfig.onOpen != NULL)
                        gConfig.onOpen();
                }
                break;
            case WM_PAINT: 
                {
                    // Update and Swap Buffer
                    SwapBuffers(gHDC);
                    Render();
                }
                break;
            case WM_DESTROY: 
                {
                    // Cleanup
                    wglDeleteContext(gContext);
                    PostQuitMessage(0);

                    Cleanup();

                    // Call User Cleanup
                    if (gConfig.onClose != NULL)
                        gConfig.onClose();
                }
                break;
            default:
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
                break;
        }

        return 0;
    }

    /*
        Creates a context / window use Win32 api.
        WndProc is then used for the main loop.
    */
    bool CreateContext(const Config* cfg)
    {
        gConfig = *cfg;
        const wchar_t* CLASS = L"DesktopApp";
        HINSTANCE hInstance = GetModuleHandle(NULL);
        WNDCLASS wc = { };

        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpszClassName = CLASS;

        RegisterClass(&wc);

        HWND hWnd = CreateWindowEx(
            0, CLASS, gConfig.title,
            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
            0, 0, gConfig.width, gConfig.height,
            NULL, NULL, hInstance, NULL 
        );

        // Check for Window Creation Failure
        if (!hWnd) return false;

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