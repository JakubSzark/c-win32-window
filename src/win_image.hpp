// ====================
// Win_Image.hpp
// Defining Image Processing on Windows
// with Gdiplus Library
// ====================

#pragma once

#include "szark_core.hpp"

#if __MINGW32__

    #pragma comment(lib, "gdiplus.lib")

    #include <gdiplus.h>

    Texture getImage(const wchar_t* imagePath)
    {
        Gdiplus::Bitmap bitmap(imagePath);
        Texture result(bitmap.GetWidth(), bitmap.GetHeight());
        Color* pixels = result.pixels.get();

        for (int i = 0; i < result.width; i++)
        {
            for (int j = 0; j < result.height; j++) 
            {
                Gdiplus::Color color;
                bitmap.GetPixel(i, j, &color);
                pixels[j * result.width + i] = Color(color.GetR(), 
                    color.GetG(), color.GetB(), color.GetA());
            }
        }
    }

#endif