// ====================
// Window_Options.hpp
// Header for Window Options Struct
// Mainly used for Creating a Window
// ====================

#pragma once

#include "szark_core.hpp"

struct WindowOptions
{
    const wchar_t* title;

    uint width; 
    uint height;

    uint pixelSize;

    void(*onOpened)();
    void(*onClosed)();
    void(*onLoop)();
};