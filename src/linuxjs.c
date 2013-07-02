#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <glib-unix.h>
#include "linuxjs.h"

#define DEVICE_PATH			"/dev/input/js0"

struct _LinuxJs
{
	Joystick parent;

	gint fd;

	guint source_id;
};

static void linux_js_init(Base *base)
{
	LinuxJs *self = LINUX_JS(base);
	self->fd = -1;
	self->source_id = 0;
}

static gboolean linux_js_event_ready(gint fd, GIOCondition cond, LinuxJs *self)
{
	struct js_event event;
	read(self->fd, &event, sizeof(event));



	return TRUE;
}

static gboolean linux_js_open(Joystick *joystick)
{
	LinuxJs *self = LINUX_JS(joystick);
	self->fd = open(DEVICE_PATH, O_RDONLY | O_NONBLOCK);
	if(-1 == self->fd) {
		g_warning("Failed to open joystick: %s", DEVICE_PATH);
		return FALSE;
	}

	self->source_id = g_unix_fd_add(self->fd,
									G_IO_IN,
									(GUnixFDSourceFunc) linux_js_event_ready,
									self);

	return TRUE;
}

static void linux_js_close(Joystick *joystick)
{
	LinuxJs *self = LINUX_JS(joystick);
	if(self->source_id) {
		g_source_remove(self->source_id);
		self->source_id = 0;
	}

	if(-1 != self->fd) {
		close(self->fd);
		self->fd = -1;
	}
}

JoystickClass linux_js_class = {
		.parent = {
			.name			= "LinuxJs",
			.parent			= BASE_CLASS(&joystick_class),
			.size			= sizeof(LinuxJs),
			.init			= linux_js_init,
		},
		.open				= linux_js_open,
		.close				= linux_js_close,
};
