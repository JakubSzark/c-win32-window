#include "../src/szark_core.hpp"

Texture hiTexture;

void drawTexture(Texture& tex, uint x, uint y)
{
    for (int i = 0; i < tex.width; i++)
    {
        for (int j = 0; j < tex.height; j++)
        {
            Color color = tex.pixels[j * tex.width + i];
            if (color.alpha == 255)
                gDrawTarget.setPixel(x + i, y + j, color);
        }
    }
}

void drawBackground()
{
    for (int i = 0; i < gDrawTarget.width; i++)
    {
        for (int j = 0; j < gDrawTarget.height; j++)
        {
            gDrawTarget.setPixel(i, j, Color(255, 0, 0, 255));
        }
    }    
}

void opened() 
{
    drawBackground();
    hiTexture = readImage("./TestImage.tga");
    drawTexture(hiTexture, 0, 0);
    hiTexture.free();
}

void loop() 
{
    refreshDrawTarget(gDrawTarget.pixels, 
        gDrawTarget.width, gDrawTarget.height);
}

void closed() {

}

int main()
{
    WindowOptions options;
    
    options.title = L"Example";
    options.windowSize = 800;
    options.pixelSize = 8;

    options.onOpened = opened;
    options.onLoop = loop;
    options.onClosed = closed;

    createWindow(options);
}