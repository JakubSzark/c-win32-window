#pragma once

#include "szark_core.hpp"

struct TgaHeader
{
    char id_length, colormap_type, datatype_code;
    short int colormap_origin, colormap_length;
    char colormap_depth;
    short int x_origin, y_origin;
    short width, height;
    char bpp, image_descriptor;
};

void mergeBytes(Color* pixel, ubyte* p)
{
    pixel->alpha = p[3];
    pixel->red = p[2];
    pixel->green = p[1];
    pixel->blue = p[0];
}

Texture readImage(const char* imagePath)
{
    int bytes2read = 0, skipover = 0;
    ubyte p[5];

    TgaHeader header;
    Texture result;

    std::fstream file(imagePath, std::ios::in);

    // Read header of TGA file

    header.id_length = file.get();
    header.colormap_type = file.get();
    header.datatype_code = file.get();

    file.read((char*)&header.colormap_origin, 2);
    file.read((char*)&header.colormap_length, 2);

    header.colormap_depth = file.get();

    file.read((char*)&header.x_origin, 2);
    file.read((char*)&header.y_origin, 2);

    file.read((char*)&header.width, 2);
    file.read((char*)&header.height, 2);

    header.bpp = file.get();
    header.image_descriptor = file.get();

    result = Texture(header.width, header.height);
    
    skipover += header.id_length;
    skipover += header.colormap_type * header.colormap_length;
    file.seekp((long)file.tellp() + skipover);

    // Reading the Image

    bytes2read = header.bpp / 8;
    
    // Reads Uncompressed TGA
    if (header.datatype_code == 2)
    {
        int index = 0;
        while (!file.eof())
        {
            file.read((char*)p, bytes2read);
            mergeBytes(&result.pixels[index], p);
            index++;
        }
    }

    file.close();
    return result;
}