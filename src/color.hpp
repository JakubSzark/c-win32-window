// ====================
// Color.hpp
// Header for Defining a Color Struct
// Uses RGBA format with unsigned bytes
// ====================

#pragma once

#include "szark_core.hpp"

struct Color
{
    // Color Components
    ubyte red, green, blue, alpha;

    // Blank Color
    Color() : red(0), green(0), blue(0), alpha(0) {}

    // Common Constructor for Creating a Color
    Color(ubyte red, ubyte green, ubyte blue, ubyte alpha = 255) :
        red(red), green(green), blue(blue), alpha(alpha) {}
};