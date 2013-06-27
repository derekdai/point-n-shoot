#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "point-n-shoot.h"
#include "scene.h"
#include "arrow.h"

#define APPLICATION_ID		"idv.dabod.PointNShoot"

#define DEFAULT_WIDTH		(1280)

#define DEFAULT_HEIGHT		(720)

struct _PointNShoot
{
	GtkApplication *app;

	GtkWidget *main_win;

	GtkWidget *canvas;

	Scene *scene;

	Arrow *arrow;
};

static void pns_activate(PointNShoot *self);

static void pns_request_quit(PointNShoot *self);

static void pns_draw(PointNShoot *self, cairo_t *cr, GtkWidget *widget);

static void pns_key_press(PointNShoot *self, GdkEventKey *event);

PointNShoot * pns_new()
{
	PointNShoot *self = g_slice_new(PointNShoot);
	self->app = gtk_application_new(APPLICATION_ID,
									G_APPLICATION_FLAGS_NONE);
	g_signal_connect_swapped(self->app,
							 "activate",
							 G_CALLBACK(pns_activate),
							 self);
	self->arrow = arrow_new(DEFAULT_WIDTH / 2,
							DEFAULT_HEIGHT / 2,
							10.0,
							20.0,
							90.0,
							5.0,
							0xffff0000);
	self->scene = scene_new();
	scene_add_item(self->scene, ITEM(self->arrow));
	self->main_win = NULL;

	return self;
}

static void pns_activate(PointNShoot *self)
{
	self->main_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(self->main_win),
								DEFAULT_WIDTH,
								DEFAULT_HEIGHT);
	gtk_window_set_position(GTK_WINDOW(self->main_win), GTK_WIN_POS_CENTER);
	g_signal_connect_swapped(self->main_win,
							 "delete-event",
							 G_CALLBACK(pns_request_quit),
							 self);
	g_signal_connect_swapped(self->main_win,
							 "key-press-event",
							 G_CALLBACK(pns_key_press),
							 self);

	self->canvas = gtk_drawing_area_new();
	gtk_widget_add_events(self->canvas,
						  GDK_KEY_PRESS_MASK |
						  GDK_KEY_RELEASE_MASK);
	g_signal_connect_swapped(self->canvas,
							 "draw",
							 G_CALLBACK(pns_draw),
							 self);
	gtk_container_add(GTK_CONTAINER(self->main_win), self->canvas);

	gtk_widget_show_all(self->main_win);

	gtk_application_add_window(self->app, GTK_WINDOW(self->main_win));
}

void pns_run(PointNShoot *self)
{
	g_application_run(G_APPLICATION(self->app), 0, NULL);
}

static void pns_request_quit(PointNShoot *self)
{
//	GtkWidget * dialog = gtk_message_dialog_new(self->main_win,
//												GTK_DIALOG_MODAL,
//												GTK_MESSAGE_QUESTION,
//												GTK_BUTTONS_YES_NO,
//												"Quit Game?");
//	if(GTK_RESPONSE_YES == gtk_dialog_run(GTK_DIALOG(dialog))) {
		pns_quit(self);
//	}
}

void pns_quit(PointNShoot *self)
{
	g_return_if_fail(self);

	gtk_widget_destroy(self->main_win);
	self->main_win = NULL;

	g_application_quit(G_APPLICATION(self->app));
}

void pns_free(PointNShoot *self)
{
	g_return_if_fail(self);

	g_object_unref(self->app);

	g_slice_free(PointNShoot, self);
}

static void pns_draw(PointNShoot *self, cairo_t *cr, GtkWidget *widget)
{
	item_draw(ITEM(self->scene), cr);
}

static void pns_key_press(PointNShoot *self, GdkEventKey *event)
{
	switch(event->keyval) {
	case GDK_KEY_A:
	case GDK_KEY_a:
		arrow_rotate(self->arrow, -5.0);
		break;
	case GDK_KEY_D:
	case GDK_KEY_d:
		arrow_rotate(self->arrow, 5.0);
		break;
	case GDK_KEY_W:
	case GDK_KEY_w:
		break;
	case GDK_KEY_S:
	case GDK_KEY_s:
		break;
	}

	gtk_widget_queue_draw(self->canvas);
}
