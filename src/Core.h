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

typedef const char* cstr;
typedef unsigned char byte;
typedef unsigned int uint;

/* Struct Definitions */

typedef struct Color {
    byte red, green, blue, alpha;
} Color;

typedef struct Window 
{
    cstr title;
    uint height;
    uint width;

    HGLRC glContext;
    void(*eventCallback)(int);
    HWND hWnd;
} Window;

// Sets Callback for a Window Context
EXPORT bool SetCallback(const Window* self, void(*callback)());

// Creates a Window Context
EXPORT Window* CreateContext(cstr title, uint width, uint height);

// Shows the Window Context on Screen
EXPORT void ShowContext(const Window* window);

// Cleans up the Window Context Pointer
EXPORT void CleanupContext(Window* window);

// Sets a Title for a Window Context
EXPORT bool SetTitle(const Window* self, cstr newTitle);