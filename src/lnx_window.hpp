// ====================
// Lnx_Windows.hpp
// Header for Creating a Window
// on the Linux Platform
// ====================

#pragma once

#include "szark_core.hpp"

#ifdef __linux__
    static GLXContext glc;

    typedef GLXContext (*glXCreateContextAttribsARBProc)
        (Display*, GLXFBConfig, GLXContext, Bool, const int*);

    void mainLoop(Display* display, XID& win)
    {
        bool isRunning = true;
        XEvent event;

        while (isRunning)
        {
            if (XPending(display))
                XNextEvent(display, &event);

            switch (event.type)
            {
                case ClientMessage:
                    isRunning = false;
                    glXDestroyContext(display, glc);
                    XCloseDisplay(display);
                    if (gOptions.onClosed != nullptr)
                        gOptions.onClosed();
                    break;

                case Expose:
                    mainRender();
                    glXSwapBuffers(display, win);
                    break;
            }
        }
    }

    bool createWindow(WindowOptions& t_options)
    {
        gOptions = t_options;

        int att[] = {
            GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None
        };

        auto display = XOpenDisplay(0);
        auto root = DefaultRootWindow(display);
        auto vi = glXChooseVisual(display, 0, att);

        auto cmap = XCreateColormap(display, root, vi->visual, AllocNone);

        XSetWindowAttributes swa;
        swa.colormap = cmap;
        swa.event_mask = ExposureMask | KeyPressMask;

        auto win = XCreateWindow(display, root, 0, 0, 
            gOptions.windowSize, gOptions.windowSize, 0, vi->depth, InputOutput,
                vi->visual, CWColormap | CWEventMask, &swa);

        XSizeHints* hints = XAllocSizeHints();
        hints->flags = PMinSize | PMaxSize;

        hints->min_width = gOptions.windowSize;
        hints->max_width = gOptions.windowSize;
        hints->min_height = gOptions.windowSize;
        hints->max_height = gOptions.windowSize;

        XSetWMNormalHints(display, win, hints);
        XSetWMSizeHints(display, win, hints, PMinSize | PMaxSize);
        XSelectInput(display, win, ExposureMask | KeyPressMask);

        XMapWindow(display, win);
        XFlush(display);

        char cTitle[sizeof(gOptions.title)];
        wcstombs(cTitle, gOptions.title, sizeof(gOptions.title));
        XStoreName(display, win, cTitle);

        Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", True);
        XSetWMProtocols(display, win, &wmDelete, 1);

        glc = glXCreateContext(display, vi, NULL, GL_TRUE);
        glXMakeCurrent(display, win, glc);  

        glEnable(GL_TEXTURE_2D);
        initDrawTarget();

        if (gOptions.onOpened != nullptr)
            gOptions.onOpened();

        mainLoop(display, win);

        return true;
    }
#endif