/*
    This file holds all relevant information,
    libraries, defines, etc.. for the library.
    Each source file includes this header file.
    Made By: Jakub P. Szarkowicz
*/

#pragma once

/* Global Defines */

#define UNICODE

/* OS Specific Libraries */

#include <GL/gl.h>
#pragma comment(lib, "opengl32.lib")
#include <windows.h>

/* Windows Export Keyword */

#define EXPORT __declspec(dllexport)

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
    bool fullscreen;
} Config;

typedef struct Color {
    byte red, green, blue, alpha;
} Color;

typedef struct Window 
{
    Config config;
    HGLRC glContext;
    HWND hwnd;
    void(*eventCallback)(int);
} Window;

/* Exported Functions */

EXPORT bool CreateContext(const Config* cfg);
EXPORT bool SetCallbacks(void(*onOpen)(), 
    void(*onClose)(), void(*onLoop)());
EXPORT void RefreshScreen(const Color* colors, 
    uint width, uint height);
EXPORT bool SetTitle(const char* newTitle);

/* Globals */

static Config gConfig;
static void(*gCallback)();

/* Functions */

void Setup();
void Cleanup();
void Render();