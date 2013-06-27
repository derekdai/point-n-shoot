#include <gtk/gtk.h>
#include "point-n-shoot.h"

#define APPLICATION_ID		"idv.dabod.PointAndShoot"

#define DEFAULT_WIDTH		(1280)

#define DEFAULT_HEIGHT		(720)

struct _PointNShoot
{
	GtkApplication *app;

	GtkWidget *main_win;
};

static void pns_activate(PointNShoot *self);

static void pns_request_quit(PointNShoot *self);

PointNShoot * pns_new()
{
	PointNShoot *self = g_slice_new(PointNShoot);
	self->app = gtk_application_new(APPLICATION_ID,
									G_APPLICATION_FLAGS_NONE);
	g_signal_connect_swapped(self->app,
							 "activate",
							 G_CALLBACK(pns_activate),
							 self);
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
	gtk_widget_show_all(self->main_win);

	gtk_application_add_window(self->app, GTK_WINDOW(self->main_win));
}

void pns_run(PointNShoot *self)
{
	g_application_run(G_APPLICATION(self->app), 0, NULL);
}

static void pns_request_quit(PointNShoot *self)
{
	GtkWidget * dialog = gtk_message_dialog_new(self->main_win,
												GTK_DIALOG_MODAL,
												GTK_MESSAGE_QUESTION,
												GTK_BUTTONS_YES_NO,
												"Quit Game?");
	if(GTK_RESPONSE_YES == gtk_dialog_run(GTK_DIALOG(dialog))) {
		pns_quit(self);
	}
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
