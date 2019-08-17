// ====================
// Rendering.hpp
// All Main Rendering goes Here
// ====================

#pragma once

#include "szark_core.hpp"

// Function for Creating a Draw Target
void initDrawTarget()
{
    float scaledSize = (float)gOptions.windowSize / 
        gOptions.pixelSize;
    uint sSize = (uint)ceilf(scaledSize);
    gDrawTarget = Texture(sSize, sSize);
    gDTS = Sprite(gDrawTarget);
}

// Function where the Draw Target is Rendered
void mainRender()
{
    glViewport(0, 0, gOptions.windowSize, gOptions.windowSize);
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1, 1, 1);
    glBindTexture(GL_TEXTURE_2D, gDTS.id);

    glBegin(GL_QUADS);
        glVertex2f(-1, -1); glTexCoord2f(0, 0);
        glVertex2f( 1, -1); glTexCoord2f(1, 0);
        glVertex2f( 1,  1); glTexCoord2f(1, 1);
        glVertex2f(-1,  1); glTexCoord2f(0, 1);
    glEnd();

    if (gOptions.onLoop != nullptr)
        gOptions.onLoop();
}

// Function for Refreshing the Draw Target
void refreshDrawTarget(Color colors[], uint w, uint h)
{
    glBindTexture(GL_TEXTURE_2D, gDTS.id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
        w, h, GL_RGBA, 
            GL_UNSIGNED_BYTE, colors);
}