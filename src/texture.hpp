// ====================
// Texture.hpp
// Header for Defining a Texture Struct
// A Texture holds a pointer to a color
// array as well as a width and a height.
// Used for holding color info for the GPU
// ====================

#pragma once

#include "szark_core.hpp"

struct Texture
{
    // Holds the Array of Colors in Memory
    std::unique_ptr<Color[]> pixels = nullptr;

    // Width in Pixels
    uint width;

    // Height in Pixels
    uint height;

    // Empty Constructor
    Texture() {}

    // Constructor for Creating a Blank Texture
    Texture(uint t_width, uint t_height) : width(t_width), 
        height(t_height) {
        pixels = std::make_unique<Color[]>(width * height);
    }
};