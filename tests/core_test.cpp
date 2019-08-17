#include "../src/szark_core.hpp"

void opened() {

}

void loop() {

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