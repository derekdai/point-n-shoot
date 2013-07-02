#include "joystick.h"

static void joystick_init(Base *base)
{
	Joystick *self = JOYSTICK(base);
	self->name = NULL;
	self->handler = NULL;
	self->opened = FALSE;
}

static void joystick_dispose(Base *base)
{
	Joystick *self = JOYSTICK(base);
	if(self->opened) {
		g_warning("Joystick %s not close yet", self->name);

		joystick_close(self);
	}

	if(self->name) {
		g_free(self->name);
	}
}

gboolean joystick_open(Joystick *self, const gchar *uri, GError **error)
{
	g_return_val_if_fail(self, FALSE);
	g_return_val_if_fail(uri, FALSE);
	g_return_val_if_fail(! error || ! *error, FALSE);

	if(self->opened) {
		return TRUE;
	}

	JoystickClass *clazz = JOYSTICK_GET_CLASS(self);
	g_return_val_if_fail(clazz->open, FALSE);
	g_return_val_if_fail(clazz->get_name, FALSE);
	g_return_val_if_fail(clazz->get_n_buttons, FALSE);
	g_return_val_if_fail(clazz->get_n_axes, FALSE);

	GError *inner_error = NULL;
	if(! clazz->open(self, uri, &inner_error)) {
		goto end;
	}

	self->name = clazz->get_name(self, &inner_error);
	if(! self->name) {
		goto close_joystick;
	}

	self->n_buttons = clazz->get_n_buttons(self, &inner_error);
	if(inner_error) {
		goto close_joystick;
	}

	self->n_axes = clazz->get_n_axes(self, &inner_error);
	if(inner_error) {
		goto close_joystick;
	}

	return self->opened = TRUE;

close_joystick:
	clazz->close(self);
end:
	if(inner_error) {
		g_propagate_error(error, inner_error);
	}
	return FALSE;
}

void joystick_close(Joystick *self)
{
	g_return_if_fail(self && JOYSTICK_GET_CLASS(self)->close);

	if(! self->opened) {
		return;
	}

	JOYSTICK_GET_CLASS(self)->close(self);
}

void joystick_dispatch_event(Joystick *self, JoystickEvent *event)
{
	g_return_if_fail(self && self->opened);
	g_return_if_fail(event);

	if(self->handler) {
		self->handler(self, event, self->handler_data);
	}
}

gint joystick_get_n_buttons(Joystick *self)
{
	g_return_val_if_fail(self, 0);

	return self->n_buttons;
}

gint joystick_get_n_axes(Joystick *self)
{
	g_return_val_if_fail(self, 0);

	return self->n_axes;
}

const gchar * joystick_get_name(Joystick *self)
{
	g_return_val_if_fail(self, NULL);

	return self->name;
}

void joystick_set_handler(Joystick *self,
						  JoystickEventHandler handler,
						  gpointer user_data)
{
	g_return_if_fail(self);

	self->handler = handler;
	self->handler_data = user_data;
}

JoystickClass joystick_class = {
		.parent = {
			.name			= "Joystick",
			.parent			= &base_class,
			.size			= sizeof(Joystick),
			.init			= joystick_init,
			.dispose		= joystick_dispose,
		}
};
