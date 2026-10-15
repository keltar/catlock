/* Grabs X11 keyboard input until release hotkey is pressed. */
#include <stdio.h>
#include <unistd.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

int main(int argc, char const *const *argv) {
	Display *display = XOpenDisplay(0);	// should use DISPLAY envvar?

	if(!display) {
		perror("open display");
		return 1;
	}

	XSetWindowAttributes attrib = {
		.override_redirect = True,
	};

	Window window = XCreateWindow(display, DefaultRootWindow(display),
				0, 0, 1, 1, 0, CopyFromParent, InputOnly, CopyFromParent,
				CWOverrideRedirect, &attrib);

	XSelectInput(display, window, KeyPressMask | KeyReleaseMask);
	XMapWindow(display, window);

	while((XGrabKeyboard(display, window, False, GrabModeAsync, GrabModeAsync,
				CurrentTime) != GrabSuccess)) {
		// grab failed, wait
		usleep(10000);
	}

	printf("keyboard input grabbbed, press ctrl-esc to cancel\n");

	for (;;) {
		XEvent ev;
		XNextEvent(display, &ev);
		switch(ev.type) {
			case KeyPress: {
				char cbuf[64];
				KeySym ks;
				XLookupString(&ev.xkey, cbuf, sizeof(cbuf), &ks, 0);
				if(ks == XK_Escape && ev.xkey.state & ControlMask) {
					return 0;
				}
			}
		}
	}

	return 0;
}
