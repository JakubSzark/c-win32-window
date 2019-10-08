/*
    This file holds all relevant information,
    libraries, defines, etc.. for the library.
    Each source file includes this header file.
    Made By: Jakub P. Szarkowicz
*/

#pragma once

/* Operating System Defines */

#ifdef _WIN32
    #define OS_WINDOWS
#elif defined(__linux__)
    #define OS_LINUX
#endif

/* Global Defines */

#define UNICODE

/* OS Specific Libraries */

#ifdef OS_WINDOWS
    #include <GL/gl.h>
    #pragma comment(lib, "opengl32.lib")
    #include <windows.h>
#endif

#ifdef OS_LINUX
    #include <GL/glx.h>
    #include <X11/Xlib.h>
#endif

/* Windows Export Keyword */

#ifdef OS_WINDOWS
    #define EXPORT __declspec(dllexport)
#elif defined(OS_LINUX)
    #define EXPORT
#endif

/* Standard Headers */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Primitive Definitions */

typedef unsigned char byte;
typedef unsigned int uint;

/* Struct Definitions */

typedef struct Config 
{
    const char* title;
    uint width, height, pixelSize;
    bool fullscreen, decorated, vsync;
    void(*onOpen)(), (*onClose)(), (*onLoop)();
} Config;

typedef struct Color {
    byte red, green, blue, alpha;
} Color;

/* Exported Functions */

EXPORT bool CreateContext(const Config* cfg);
EXPORT void RefreshScreen(const Color* colors, 
    uint width, uint height);

EXPORT void SetSize(uint width, uint height);
EXPORT void SetTitle(const char* newTitle);
EXPORT void SetFullscreen(bool isFullscreen);

/* Globals */

static Config gConfig;

/* Functions */

void Setup();
void Cleanup();
void Render();