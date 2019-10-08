#include "Core.h"

#ifdef OS_LINUX
    static GLXContext gContext;

    /*
        Main context loop,
        Processing inputs, rendering, logic
    */
    void MainLoop(Display* display, Window window)
    {
        XEvent event;
        bool isRunning = true;

        while (isRunning)
        {
            if (XPending(display))
                XNextEvent(display, &event);

            switch (event.type)
            {
                case ClientMessage:
                    isRunning = false;
                    glXDestroyContext(display, gContext);
                    XCloseDisplay(display);
                    Cleanup();
                    break;

                case Expose:
                    Render();
                    glXSwapBuffers(display, window);
                    break;
            }
        }
    }

    /*
        Creates a context on linux with X11
    */
    bool CreateContext(const Config* cfg)
    {
        gConfig = *cfg;
        int attributes[] = {
            GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None
        };

        Display* display = XOpenDisplay(0);
        int root = DefaultRootWindow(display);
        XVisualInfo* vi = glXChooseVisual(display, 0, attributes);
        Colormap cmap = XCreateColormap(display, root, vi->visual,
            AllocNone);

        XSetWindowAttributes swa = { };
        swa.event_mask = ExposureMask | KeyPressMask;
        swa.colormap = cmap;

        Window window = XCreateWindow(display, root, 0, 0,
            gConfig.width, gConfig.height, 0, vi->depth, InputOutput,
                vi->visual, CWColormap | CWEventMask, &swa);

        XSizeHints* hints = XAllocSizeHints();
        hints->flags = PMinSize | PMaxSize;

        hints->min_width = gConfig.width;
        hints->max_width = gConfig.width;

        hints->min_height = gConfig.height;
        hints->max_height = gConfig.height;

        XSetWMNormalHints(display, window, hints);
        XSetWMSizeHints(display, window, hints, PMinSize | PMaxSize);
        XSelectInput(display, window, ExposureMask | KeyPressMask);

        XMapWindow(display, window);
        XFlush(display);

        XStoreName(display, window, gConfig.title);

        Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(display, window, &wmDelete, 1);

        gContext = glXCreateContext(display, vi, NULL, GL_TRUE);
        glXMakeCurrent(display, window, gContext);

        glEnable(GL_TEXTURE_2D);        
        Setup();
        MainLoop(display, window);

        return true;
    }
#endif