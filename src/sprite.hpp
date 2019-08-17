// ====================
// Sprite.hpp
// Header for Defining a Sprite Struct
// A sprite converts a Texture struct
// into a uint ID that OpenGL creates
// for use in rendering.
// ====================

#pragma once

#include "szark_core.hpp"

struct Sprite
{
    // ID created by OpenGL
    uint id;
    
    // Width in Pixels
    uint width;
    
    // Height in Pixels
    uint height;

    // Empty Constructor
    Sprite() {}

    // Main Sprite Constructor
    // Converts Texture into OpenGL ID
    Sprite(Texture& texture)
    {
        width = texture.width;
        height = texture.height;

        // Tell OpenGL to Generate Texture ID
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
            width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.pixels.get());

        // Texture Parameters for Pixel-Art
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
};