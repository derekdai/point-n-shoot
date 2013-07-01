#include <gtk/gtk.h>
#include <gst/gst.h>
#include "point-n-shoot.h"
#include "config.h"

gint main(gint argc, gchar *args[])
{
	gtk_init(&argc, &args);
	gst_init(&argc, &args);

	GstElement * playbin = gst_element_factory_make("playbin", "playbin");
	g_object_set(playbin, "uri", "file://" MUSIC_DIR "DST-1990.mp3", NULL);
	gst_element_set_state(playbin, GST_STATE_PLAYING);

	PointNShoot *pns = pns_get_default();
	pns_run(pns);
	pns_destroy();

	return 0;
}
