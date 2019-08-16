#include "szark_core.hpp"

void opened()
{
    printf("Opened");
}

void loop()
{
    putchar('.');
}

void closed()
{
    printf("Closed");
    getchar();
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