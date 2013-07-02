#include <glib/gstdio.h>
#include "linuxplat.h"
#include "linuxjs.h"

#define PATH_TEMPLATE			"/dev/input/js%d"

#define MAXPATHLEN				(1024)

static gboolean linux_plat_init(Platform *self, GError **error);

static gint linux_plat_get_n_joysticks(Platform *self, GError **error);

static Joystick * linux_plat_get_joystick(Platform *self,
										  gint n,
										  GError **error);

PlatformClass linux_plat_class = {
		.parent = {
			.name			= "LinuxPlat",
			.parent			= &base_class,
			.size			= sizeof(LinuxPlat),
		},
		.init				= linux_plat_init,
		.get_n_joysticks	= linux_plat_get_n_joysticks,
		.get_joystick		= linux_plat_get_joystick,
};

static gboolean linux_plat_init(Platform *self, GError **error)
{
	return TRUE;
}

static gint linux_plat_get_n_joysticks(Platform *self, GError **error)
{
	gint i;
	gchar path[MAXPATHLEN];
	for(i = 0; ; i ++) {
		g_snprintf(path, sizeof(path), PATH_TEMPLATE, i);

		GStatBuf stat_buf;
		if(-1 == g_stat(path, &stat_buf)) {
			break;
		}

		if(! S_ISCHR(stat_buf.st_mode)) {
			break;
		}
	}

	return i;
}

static Joystick * linux_plat_get_joystick(Platform *self,
										  gint n,
										  GError **error)
{
	gchar path[MAXPATHLEN];
	g_snprintf(path, sizeof(path), PATH_TEMPLATE, n);

	Joystick *js = base_new(&linux_js_class);
	if(! joystick_open(js, path, error)) {
		base_unref(js);
		return FALSE;
	}

	return js;
}
