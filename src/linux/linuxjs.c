#include <glib-unix.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
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
	struct js_event native_event;
	read(self->fd, &native_event, sizeof(native_event));

	if(native_event.type & JS_EVENT_INIT) {
		return TRUE;
	}

	JoystickEvent event = {
			.type		= native_event.type & JS_EVENT_BUTTON
								? JOYSTICK_EVENT_BUTTON
								: JOYSTICK_EVENT_AXES,
			.time		= native_event.time,
			.number		= native_event.number,
			.value		= native_event.value,
	};
	joystick_dispatch_event(JOYSTICK(self), &event);

	return TRUE;
}

static gboolean linux_js_open(Joystick *joystick,
							  const gchar *uri,
							  GError **error)
{
	LinuxJs *self = LINUX_JS(joystick);
	self->fd = open(uri, O_RDONLY | O_NONBLOCK);
	if(-1 == self->fd) {
		g_set_error(error,
					G_FILE_ERROR,
					g_file_error_from_errno(errno),
					"failed to open joystick: %s",
					uri);
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

static gint linux_js_get_n_buttons(Joystick *joystick, GError **error)
{
	LinuxJs *self = LINUX_JS(joystick);
	__u8 n_buttons;
	if(-1 == ioctl(self->fd, JSIOCGBUTTONS, &n_buttons)) {
		g_set_error(error,
					G_FILE_ERROR,
					g_file_error_from_errno(errno),
					"failed to get number of buttons");
		return 0;
	}

	return n_buttons;
}

static gint linux_js_get_n_axes(Joystick *joystick, GError **error)
{
	LinuxJs *self = LINUX_JS(joystick);
	__u8 n_axes;
	if(-1 == ioctl(self->fd, JSIOCGAXES, &n_axes)) {
		g_set_error(error,
					G_FILE_ERROR,
					g_file_error_from_errno(errno),
					"failed to get number of axes");
		return 0;
	}

	return n_axes;
}

static gchar * linux_js_get_name(Joystick *joystick, GError **error)
{
	LinuxJs *self = LINUX_JS(joystick);
	gchar buf[128];
	if(-1 == ioctl(self->fd, JSIOCGNAME(sizeof(buf)), buf)) {
		g_set_error(error,
					G_FILE_ERROR,
					g_file_error_from_errno(errno),
					"failed to get name of joystick");
		return NULL;
	}

	return g_strdup(buf);
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
		.get_n_buttons		= linux_js_get_n_buttons,
		.get_n_axes			= linux_js_get_n_axes,
		.get_name			= linux_js_get_name,
};
