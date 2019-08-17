// ====================
// Window_Options.hpp
// Header for Window Options Struct
// Mainly used for Creating a Window
// ====================

#pragma once

#include "szark_core.hpp"

struct WindowOptions
{
    // Title of the Window
    const wchar_t* title;

    // Square Size of the Window
    uint windowSize;

    // Size of Each Pixel
    uint pixelSize;

    // Called when a Window Opens
    void(*onOpened)();

    // Called every Frame
    void(*onLoop)();

    // Called when a Window Closes
    void(*onClosed)();
};