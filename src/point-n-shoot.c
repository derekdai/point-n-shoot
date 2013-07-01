#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "point-n-shoot.h"
#include "scene.h"
#include "event.h"

#define APPLICATION_ID		"idv.dabod.PointNShoot"

#define DEFAULT_WIDTH		(1280)

#define DEFAULT_HEIGHT		(720)

#define KEY_NOTIFY_INTERVAL	(100)

struct _PointNShoot
{
	GtkApplication *app;

	GtkWidget *main_win;

	Scene *scene;

	GameKeys keys;
};

static void pns_activate(PointNShoot *self);

static void pns_request_quit(PointNShoot *self);

static void pns_key_press(PointNShoot *self, GdkEventKey *event);

static void pns_key_release(PointNShoot *self, GdkEventKey *event);

static PointNShoot * pns = NULL;;

PointNShoot * pns_new()
{
	PointNShoot *self = g_slice_new(PointNShoot);
	self->app = gtk_application_new(APPLICATION_ID,
									G_APPLICATION_FLAGS_NONE);
	g_signal_connect_swapped(self->app,
							 "activate",
							 G_CALLBACK(pns_activate),
							 self);
	self->scene = scene_new();
	self->main_win = NULL;
	self->keys = GAME_KEYS_NONE;

	return self;
}

PointNShoot * pns_get_default()
{
	if(! pns) {
		pns = pns_new();
	}

	return pns;
}

static gboolean pns_window_size_allocate(PointNShoot *self,
										 GdkRectangle *rect,
										 GtkWidget *win)
{
	if(SCENE_STATE_NULL == scene_get_state(self->scene)) {
		scene_set_width(self->scene, rect->width);
		scene_set_height(self->scene, rect->height);
		scene_start(self->scene);
	}

	return TRUE;
}

static void pns_activate(PointNShoot *self)
{
	self->main_win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	gtk_window_set_resizable(GTK_WINDOW(self->main_win), FALSE);
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
	g_signal_connect_swapped(self->main_win,
							 "size-allocate",
							 G_CALLBACK(pns_window_size_allocate),
							 self);

	gtk_container_add(GTK_CONTAINER(self->main_win),
					  scene_get_widget(self->scene));

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

static void pns_free(PointNShoot *self)
{
	g_return_if_fail(self);

	base_unref(self->scene);

	g_object_unref(self->app);

	g_slice_free(PointNShoot, self);
}

void pns_destroy()
{
	if(pns) {
		pns_free(pns);
		pns = NULL;
	}
}

static void pns_notify_key_state(PointNShoot *self)
{
	scene_set_keys(self->scene, self->keys);
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

	pns_notify_key_state(self);
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

	pns_notify_key_state(self);
}
