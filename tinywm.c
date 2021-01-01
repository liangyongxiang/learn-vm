/* TinyWM is written by Nick Welch <mack@incise.org>, 2005.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <X11/Xlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main()
{
    Display * dpy;
    Window root;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;

    if(!(dpy = XOpenDisplay(0x0))) return 1;

    root = DefaultRootWindow(dpy);

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root, True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);

    for(;;)
    {
        XNextEvent(dpy, &ev);
        if(ev.type == KeyPress) {
        	if (ev.xkey.subwindow == None) {
				printf("key press, root: %ld, subwindow: None\n", root);
			} else {
            	XRaiseWindow(dpy, ev.xkey.subwindow);
				printf("key press, root: %ld, subwindow: %ld, raise to top\n", root, ev.xkey.subwindow);
			}
		} else if(ev.type == ButtonPress) {
        	if (ev.xkey.subwindow == None) {
				printf("key press, root: %ld, subwindow: None\n", root);
			} else {
				printf("key press, root: %ld, subwindow: %ld\n", root, ev.xkey.subwindow);
            	XGrabPointer(dpy, ev.xbutton.subwindow, True,
            	        PointerMotionMask|ButtonReleaseMask, GrabModeAsync,
            	        GrabModeAsync, None, None, CurrentTime);
            	XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            	start = ev.xbutton;
			}
        } else if(ev.type == MotionNotify) {
            int xdiff, ydiff;
			int cnt = 0;

            while(XCheckTypedEvent(dpy, MotionNotify, &ev)) {
				cnt++;
			}
			printf("motion notify, loop: %d\n", cnt);

            xdiff = ev.xbutton.x_root - start.x_root;
            ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, ev.xmotion.window,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        } else if(ev.type == ButtonRelease) {
			printf("key release\n");
            XUngrabPointer(dpy, CurrentTime);
		}
    }
}
