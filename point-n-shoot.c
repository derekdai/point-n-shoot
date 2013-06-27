#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "point-n-shoot.h"
#include "scene.h"
#include "arrow.h"
#include "event.h"

#define APPLICATION_ID		"idv.dabod.PointNShoot"

#define DEFAULT_WIDTH		(1280)

#define DEFAULT_HEIGHT		(720)

#define ARROW_WIDTH			(16.0)

#define ARROW_HEIGHT		(20.0)

#define ARROW_ANGLE			(90.0)

#define ARROW_SPEED			(10.0)

#define ARROW_COLOR			(0xffff0000)

#define KEY_NOTIFY_INTERVAL	(100)

struct _PointNShoot
{
	GtkApplication *app;

	GtkWidget *main_win;

	GtkWidget *canvas;

	Scene *scene;

	Arrow *arrow;

	GameKeys keys;

	guint key_notify_handle;
};

static void pns_activate(PointNShoot *self);

static void pns_request_quit(PointNShoot *self);

static void pns_draw(PointNShoot *self, cairo_t *cr, GtkWidget *widget);

static void pns_key_press(PointNShoot *self, GdkEventKey *event);

static void pns_key_release(PointNShoot *self, GdkEventKey *event);

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
							ARROW_WIDTH,
							ARROW_HEIGHT,
							ARROW_ANGLE,
							ARROW_SPEED,
							ARROW_COLOR);
	self->scene = scene_new();
	scene_add_item(self->scene, ITEM(self->arrow));
	self->main_win = NULL;
	self->keys = GAME_KEYS_NONE;
	self->key_notify_handle = 0;

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
	g_signal_connect_swapped(self->main_win,
							 "key-release-event",
							 G_CALLBACK(pns_key_release),
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

static gboolean pns_notify_key_state(PointNShoot *self)
{
	return TRUE;
}

static void pns_check_key_state(PointNShoot *self)
{
	if(self->key_notify_handle) {
		g_source_remove(self->key_notify_handle);
		self->key_notify_handle = 0;
	}

	if(! self->keys) {
		return;
	}

	self->key_notify_handle = g_timeout_add(KEY_NOTIFY_INTERVAL,
											(GSourceFunc) pns_notify_key_state,
											self);
}

static void pns_key_press(PointNShoot *self, GdkEventKey *event)
{
	GameKeys keys;
	switch(event->keyval) {
	case GDK_KEY_A:
	case GDK_KEY_a:
		keys = GAME_KEYS_LEFT;
		break;
	case GDK_KEY_D:
	case GDK_KEY_d:
		keys = GAME_KEYS_RIGHT;
		break;
	case GDK_KEY_W:
	case GDK_KEY_w:
		keys = GAME_KEYS_UP;
		break;
	case GDK_KEY_S:
	case GDK_KEY_s:
		keys = GAME_KEYS_DOWN;
		break;
	default:
		return;
	}

	if(keys & self->keys) {
		return;
	}

	self->keys |= keys;

	pns_check_key_state(self);
}

static void pns_key_release(PointNShoot *self, GdkEventKey *event)
{
	switch(event->keyval) {
	case GDK_KEY_A:
	case GDK_KEY_a:
		self->keys &= ~GAME_KEYS_LEFT;
		break;
	case GDK_KEY_D:
	case GDK_KEY_d:
		self->keys &= ~GAME_KEYS_RIGHT;
		break;
	case GDK_KEY_W:
	case GDK_KEY_w:
		self->keys &= ~GAME_KEYS_UP;
		break;
	case GDK_KEY_S:
	case GDK_KEY_s:
		self->keys &= ~GAME_KEYS_DOWN;
		break;
	default:
		return;
	}

	pns_check_key_state(self);
}
