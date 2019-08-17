// ====================
// Szark_Core.hpp
// File containing definitions and
// other core header files
// ====================

#pragma once

/* Definitions */

#define UNICODE
#define ubyte unsigned char

#if __MINGW32__
    #define uint unsigned int
    #define API __declspec(dllexport)
#else
    #define API
#endif

/* Standard Headers */

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>

/* Windows Libraries */

#ifdef __MINGW32__
    #pragma comment(lib, "opengl32.lib")
#endif

/* Platform Headers */

#ifdef __MINGW32__
    #include <GL/gl.h>
    #include <windows.h>
#elif __linux__
    #include <GL/glx.h>
    #include <X11/Xlib.h>
#endif

/* Structs Headers */

#include "window_options.hpp"
#include "color.hpp"
#include "texture.hpp"
#include "sprite.hpp"

/* API Function Definitions */

extern "C" 
{
    API bool createWindow(WindowOptions& options);
    API void refreshDrawTarget(Color colors[], uint w, uint h);
}

/* Internal Function Definitions */

void mainRender();
void initDrawTarget();

/* Global Variables */

static Texture gDrawTarget;
static WindowOptions gOptions;
static Sprite gDTS;

/* Window Creation */

#ifdef __MINGW32__
    #include "win_window.hpp"
#elif __linux__
    #include "lnx_window.hpp"
#endif

/* Rendering */

#include "rendering.hpp"