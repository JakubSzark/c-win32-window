#include "Core.h"

/*
    Turns normal ASCII char to WCHAR*
    Please Free after use.
*/
wchar_t* ConvertToWChar(cstr title)
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
    Window* window = (Window*)lParam;

    switch (uMsg)
    {
        case WM_CREATE: 
            {
                // Set Context Pixel Format Descriptor
                HDC hdc = GetDC(hWnd);
                PIXELFORMATDESCRIPTOR pfd = GeneratePFD();
                SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

                // Make OpenGL work on current Context
                window->glContext = wglCreateContext(hdc);
                wglMakeCurrent(GetDC(hWnd), window->glContext);
                window->eventCallback(Opened);
            }
            break;
        case WM_PAINT: 
            {
                // Update and Swap Buffer
                SwapBuffers(GetDC(hWnd));
                window->eventCallback(Render);
            }
            break;
        case WM_DESTROY: 
            {
                // Cleanup
                wglDeleteContext(window->glContext);
                PostQuitMessage(0);
                window->eventCallback(Closed);                
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
Window* CreateContext(cstr title, uint width, uint height)
{
    // Creating a Window Struct
    Window* window = (Window*)calloc(1, sizeof(Window));

    window->glContext = NULL;
    window->eventCallback = NULL;

    window->title = title;
    window->height = height;
    window->width = width;

    const wchar_t* CLASS = L"DesktopApp";
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS wc = { };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpszClassName = CLASS;

    RegisterClass(&wc);

    wchar_t* newTitle = ConvertToWChar(window->title);

    window->hWnd = CreateWindowEx(
        0, CLASS, newTitle, 
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        0, 0, width, height,
        NULL, NULL, hInstance, NULL 
    );

    // Check for Window Creation Failure
    if (!window->hWnd) return NULL;
    free(newTitle);
    return window;    
}

/*
    Shows the Window Context on Screen
*/
void ShowContext(const Window* window)
{
    ShowWindow(window->hWnd, SW_SHOW);
    UpdateWindow(window->hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, (LPARAM)window))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

/*
    Frees the Window Context Ptr
*/
void CleanupContext(Window* window) 
{
    if (window != NULL) 
    {
        free(window);
        window = NULL;
    }
}

/*
    Set the Title of the Window. Duh.
    Returns whether or not successful.
*/
bool SetTitle(const Window* self, const char* title) 
{
    wchar_t* newTitle = ConvertToWChar(title);
    bool success = SetWindowText(self->hWnd, newTitle);
    free(newTitle);
    return success;
}