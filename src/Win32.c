#include "Core.h"

#ifdef OS_WINDOWS
    static HGLRC gContext;
    static HWND gHWND;

    /*
        Turns normal ASCII char to WCHAR*
        Please Free after use.
    */
    wchar_t* ConvertToWChar(const char* title)
    {
        size_t newSize = strlen(title) + 1;
        wchar_t* newStr = (wchar_t*)calloc(newSize, 
            sizeof(unsigned short));

        size_t convertedChars = 0;
        mbstowcs_s(&convertedChars, newStr, newSize, 
            title, _TRUNCATE);

        return newStr;        
    }

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
                    HDC hdc = GetDC(hWnd);
                    gHWND = hWnd;

                    PIXELFORMATDESCRIPTOR pfd = GeneratePFD();
                    SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

                    // Make OpenGL work on current Context
                    gContext = wglCreateContext(hdc);
                    wglMakeCurrent(GetDC(hWnd), gContext);
                    glEnable(GL_TEXTURE_2D);

                    Setup();
                }
                break;
            case WM_PAINT: 
                {
                    // Update and Swap Buffer
                    SwapBuffers(GetDC(hWnd));
                    Render();
                }
                break;
            case WM_DESTROY: 
                {
                    // Cleanup
                    wglDeleteContext(gContext);
                    PostQuitMessage(0);

                    Cleanup();
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

        wchar_t* newTitle = ConvertToWChar(gConfig.title);

        HWND hWnd = CreateWindowEx(
            0, CLASS, newTitle, 
            WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
            0, 0, cfg->width, cfg->height,
            NULL, NULL, hInstance, NULL 
        );

        // Check for Window Creation Failure
        if (!hWnd) return false;

        // TODO: Add Window Struct Creation

        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        free(newTitle);
        return true;        
    }

    /*
        Set the Title of the Window. Duh.
        Returns whether or not successful.
    */
    bool SetTitle(const char* title) 
    {
        wchar_t* newTitle = ConvertToWChar(title);
        bool success = SetWindowText(gHWND, newTitle);
        free(newTitle);

        return success;
    }
#endif