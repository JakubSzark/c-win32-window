/*
    Stored here is functions that
    utilize OpenGL calls and other setup
    for rendering and cleanup.
*/

#include "Core.h"

static Color* gScreen;
static uint gScreenID;

bool SetCallbacks(void(*onOpen)(), 
    void(*onClose)(), void(*onLoop)())
{
    if (onOpen == NULL || onClose == NULL ||
        onLoop == NULL) return false;

    gOpen = onOpen;
    gClose = onClose;
    gLoop = onLoop;

    return true;
}

/*
    Creates a screen texture made up of colors.
    Then sends that information to the GPU.
*/
void Setup()
{
    // Setup Pixel Width and Height
    float sHeight = (float)gConfig.height / gConfig.pixelSize;
    float sWidth = (float)gConfig.width / gConfig.pixelSize;

    // Create a Screen
    gScreen = (Color*)calloc(sWidth * sHeight, sizeof(Color));

    // Generate and Send Screen to GPU
    glGenTextures(1, &gScreenID);
    glBindTexture(GL_TEXTURE_2D, gScreenID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sWidth, sHeight, 0, 
        GL_RGBA, GL_UNSIGNED_BYTE, gScreen);

    // Texture Parameters for Crisp Textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // Call User Open
    gOpen();
}

/*
    Cleans up allocations
*/
void Cleanup() 
{
    // Call User Cleanup
    gClose();

    if (gScreen)
        free(gScreen);
    gScreen = NULL;
}

/*
    Renders a quad to the context with 
    OpenGL 1.0. Then calls the user defined loop
    function.
*/
void Render()
{
    // Change Rendering Region
    glViewport(0, 0, gConfig.width, gConfig.height);

    // Clear Screen Color
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind Screen Texture
    glBindTexture(GL_TEXTURE_2D, gScreenID);

    // Draw Quad to Screen
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(-1, -1); 
        glTexCoord2f(1, 0); glVertex2f( 1, -1); 
        glTexCoord2f(1, 1); glVertex2f( 1,  1); 
        glTexCoord2f(0, 1); glVertex2f(-1,  1); 
    glEnd();

    gLoop();
}

/*
    Updates the set of pixels for the screen
    and sends it to the the GPU to be rendered.
*/
void RefreshScreen(const Color* colors, 
    uint width, uint height)
{
    glBindTexture(GL_TEXTURE_2D, gScreenID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, gConfig.width,
        gConfig.height, GL_RGBA, GL_UNSIGNED_BYTE, gScreen);
}