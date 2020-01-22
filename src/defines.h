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
#include <windows.h>

/* Windows Export Keyword */

#define EXPORT __declspec(dllexport)

/* Standard Headers */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Primitive Definitions */

typedef const wchar_t* wstr;

/* Struct Definitions */

typedef struct Dimension
{
    int x, y;
    int width, height;
} Dimension;

typedef struct Window 
{
    wstr title;
    int width, height;
    bool shouldClose;
    bool fullscreen;

    HGLRC glContext;
    void(*eventCallback)(int);
    HWND hWnd;
} Window;

typedef enum Event {
    Opened, Closed, Resized
} Event;

/* Window Styles */

const long STYLE_WINDOWED = 
    WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX;
const long STYLE_FULLSCREEN = 
    WS_POPUP | WS_VISIBLE;

// Creates a window
EXPORT Window* createWindow(wstr title, int width, int height);

// Shows a window on screen
EXPORT void showWindow(const Window* window);

// Cleans up a window pointer
EXPORT void cleanupWindow(Window* window);

// Sets the title of a window
EXPORT bool setTitle(const Window* self, wstr newTitle);

// Set the size of a window
EXPORT void setSize(Window* self, int width, int height);

// Set whether or not the window should be fullscreen
EXPORT void setFullscreen(Window* self, bool isFullscreen);

// Polls events from the window
EXPORT void pollEvents(const Window* self);