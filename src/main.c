#include <gtk/gtk.h>
#include <gst/gst.h>
#include "point-n-shoot.h"
#include "config.h"

static gboolean play_bg(GstElement *playbin)
{
	gst_element_set_state(playbin, GST_STATE_PLAYING);

	return FALSE;
}

void handle_bus_message(GstBus *bus, GstMessage *message, GstElement *playbin)
{
	switch(message->type) {
	case GST_MESSAGE_EOS:
		gst_element_set_state(playbin, GST_STATE_NULL);
		g_timeout_add_seconds(5, (GSourceFunc) play_bg, playbin);
		break;
	default:
		break;
	}
}

gint main(gint argc, gchar *args[])
{
	gtk_init(&argc, &args);
	gst_init(&argc, &args);

	GstElement * playbin = gst_element_factory_make("playbin", "playbin");
	g_object_set(playbin, "uri", "file://" MUSIC_DIR MUSIC_FILE, NULL);
	gst_element_set_state(playbin, GST_STATE_PLAYING);

	GstBus *bus = gst_element_get_bus(playbin);
	gst_bus_add_signal_watch(bus);
	g_signal_connect(bus, "message", G_CALLBACK(handle_bus_message), playbin);

	PointNShoot *pns = pns_get_default();
	pns_run(pns);

	gst_bus_remove_signal_watch(bus);
	g_object_unref(bus);
	gst_element_set_state(playbin, GST_STATE_NULL);
	g_object_ref(playbin);

	pns_destroy();

	return 0;
}
