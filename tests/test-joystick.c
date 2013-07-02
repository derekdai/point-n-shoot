#include <glib.h>
#include <glib-unix.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/joystick.h>

static gboolean on_joystick_event(gint fd)
{
	struct js_event event;
	read(fd, &event, sizeof(event));
	switch(event.type) {
	case JS_EVENT_BUTTON:
		g_message("Button %d: %s", event.number, event.value ? "pressed" : "released");
		break;
	case JS_EVENT_AXIS:
		g_message("  Axes %d: %d", event.number, event.value);
		break;
	default:
		break;
	}

	return TRUE;
}

int main(int argc, char *args[])
{
	int fd = open("/dev/input/js0", O_RDONLY);
	if(-1 == fd) {
		g_error("No joystick found: /dev/input/js0");
	}

	__u32 version;
	char js_name[128];
	ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGNAME(sizeof(js_name)), js_name);
	g_message("Joystick %s, IOCTL version: %x", js_name, version);

	__u8 n_axes;
	ioctl(fd, JSIOCGAXES, &n_axes);
	g_message("Number of axes: %d", n_axes);

	__u8 n_buttons;
	ioctl(fd, JSIOCGBUTTONS, &n_buttons);
	g_message("Number of buttons: %d", n_buttons);

	GMainLoop *loop = g_main_loop_new(NULL, FALSE);
	g_unix_fd_add(fd,
				  G_IO_IN,
				  (GUnixFDSourceFunc) on_joystick_event,
				  loop);

	g_main_loop_run(loop);
	g_main_loop_unref(loop);

	return 0;
}
