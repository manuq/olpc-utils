#include <linux/input.h>

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
set_keycode (int fd, int scancode, int keycode)
{
	int ret;
	int v[2];

	v[0] = scancode;
	v[1] = keycode;

	ret = ioctl (fd, EVIOCSKEYCODE, v);
	if (ret) {
	fprintf (stderr, "Unable to set scancode %Xh to keycode %Xh: %s\n",
		scancode, keycode, strerror(errno));
	}

	return ret;
}

int
main (int argc, char *argv[])
{
	int fd, ret = 0;

	if (argc != 2) {
	fprintf (stderr, "Usage: %s <keyboard device node>\n", argv[0]);
	exit (1);
	}

	fd = open (argv[1], O_RDWR);
	if (fd < 0) {
	fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
	}

	ret -= set_keycode(fd, 0x59, KEY_FN);

	ret -= set_keycode(fd, 0x01 | 0x80, KEY_FN_ESC);

	ret -= set_keycode(fd, 0x79 | 0x80, KEY_CAMERA);
	/* FN-CAMERA is 'Mic', KEY_SOUND is as close as I see. */
	ret -= set_keycode(fd, 0x78 | 0x80, KEY_SOUND);

	ret -= set_keycode(fd, 0x3B | 0x80, KEY_FN_F1);
	ret -= set_keycode(fd, 0x3C | 0x80, KEY_FN_F2);
	ret -= set_keycode(fd, 0x3D | 0x80, KEY_FN_F3);
	ret -= set_keycode(fd, 0x3E | 0x80, KEY_FN_F4);
	ret -= set_keycode(fd, 0x3F | 0x80, KEY_FN_F5);
	ret -= set_keycode(fd, 0x40 | 0x80, KEY_FN_F6);
	ret -= set_keycode(fd, 0x41 | 0x80, KEY_FN_F7);
	ret -= set_keycode(fd, 0x42 | 0x80, KEY_FN_F8);
	ret -= set_keycode(fd, 0x43 | 0x80, KEY_FN_F9);
	ret -= set_keycode(fd, 0x44 | 0x80, KEY_FN_F10);
	ret -= set_keycode(fd, 0x57 | 0x80, KEY_FN_F11);
	ret -= set_keycode(fd, 0x58 | 0x80, KEY_FN_F12);

	/* Using KEY_F13-KEY_F21 for the .5 F keys right now. */
	ret -= set_keycode(fd, 0x77 | 0x80, KEY_F13);
	ret -= set_keycode(fd, 0x76 | 0x80, KEY_F14);
	ret -= set_keycode(fd, 0x75 | 0x80, KEY_F15);
	ret -= set_keycode(fd, 0x74 | 0x80, KEY_F16);
	ret -= set_keycode(fd, 0x73 | 0x80, KEY_F17);
	ret -= set_keycode(fd, 0x72 | 0x80, KEY_F18);
	ret -= set_keycode(fd, 0x71 | 0x80, KEY_F19);
	ret -= set_keycode(fd, 0x70 | 0x80, KEY_F20);
	ret -= set_keycode(fd, 0x6F | 0x80, KEY_F21);

	ret -= set_keycode(fd, 0x6E | 0x80, KEY_CHAT);
	/* FIXME: fn-CHAT, just mapping also to KEY_CHAT right now. */
	ret -= set_keycode(fd, 0x64 | 0x80, KEY_CHAT);

	/* frame and fn-frame, which is listed as 'Win App', try. */
	ret -= set_keycode(fd, 0x5D | 0x80, KEY_MENU);
	ret -= set_keycode(fd, 0x5A | 0x80, KEY_PROG1);

	/* The FN of some keys is other keys. */
	ret -= set_keycode(fd, 0x53 | 0x80, KEY_DELETE);
	ret -= set_keycode(fd, 0x52 | 0x80, KEY_INSERT);
	ret -= set_keycode(fd, 0x49 | 0x80, KEY_PAGEUP);
	ret -= set_keycode(fd, 0x51 | 0x80, KEY_PAGEDOWN);
	ret -= set_keycode(fd, 0x47 | 0x80, KEY_HOME);
	ret -= set_keycode(fd, 0x4F | 0x80, KEY_END);


	/*
	 * FIXME: These should be the language key.
	 * Don't ask what they are doing as KEY_HP.
	 * (I'd have to tell you, and then I'd be making your brain hurt.)
	 */
	ret -= set_keycode(fd, 0x73, KEY_HP);
	ret -= set_keycode(fd, 0x7E, KEY_HP);


	/* L/R GRAB */
	ret -= set_keycode(fd, 0x5B | 0x80, KEY_LEFTMETA);
	ret -= set_keycode(fd, 0x5C | 0x80, KEY_RIGHTMETA);
	/* FIXME: Right grab seems to be releasing on a different scancode. */
	ret -= set_keycode(fd, 0x85, KEY_RIGHTMETA);


	/* FN space, toggles the backlight. */
	ret -= set_keycode(fd, 0x56 | 0x80, KEY_KBDILLUMTOGGLE);


	/* Set up game keys to map to up/down/left/right for now */
	ret -= set_keycode(fd, 0x65, KEY_KP8);
	ret -= set_keycode(fd, 0x66, KEY_KP2);
	ret -= set_keycode(fd, 0x67, KEY_KP4);
	ret -= set_keycode(fd, 0x68, KEY_KP6);

	/* Set the other game keys to map to pgup/pgdn/home/end */
	ret -= set_keycode(fd, 0x65 | 0x80, KEY_KP9);
	ret -= set_keycode(fd, 0x66 | 0x80, KEY_KP3);
	ret -= set_keycode(fd, 0x67 | 0x80, KEY_KP7);
	ret -= set_keycode(fd, 0x68 | 0x80, KEY_KP1);

	/* The brightness key to toggle the backlight on the screen. */
	ret -= set_keycode(fd, 0x69, KEY_SWITCHVIDEOMODE);

	exit(ret);
}
