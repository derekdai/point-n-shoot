#include "platform.h"

static void platform_base_init(Base *base);

static void platform_base_dispose(Base *base);

PlatformClass platform_class = {
		.parent = {
			.name			= "Platform",
			.parent			= &base_class,
			.size			= sizeof(Platform),
			.init			= platform_base_init,
			.dispose		= platform_base_dispose,
		},
};

static void platform_base_init(Base *base)
{
	Platform *self = PLATFORM(base);
	self->joysticks = NULL;
	self->n_joysticks = 0;
	self->initialized = FALSE;
}

static void platform_base_dispose(Base *base)
{
	Platform *self = PLATFORM(base);
	if(self->joysticks) {
		g_slice_free1(sizeof(self->joysticks[0]) * self->n_joysticks,
					  self->joysticks);
	}
}

static void platform_init_joysticks(Platform *self, PlatformClass *clazz)
{
	g_return_if_fail(clazz->get_n_joysticks);
	g_return_if_fail(clazz->get_joystick);

	GError *error = NULL;
	self->n_joysticks = clazz->get_n_joysticks(self, &error);
	if(error) {
		goto dump_error;
	}

	if(! self->n_joysticks) {
		return;
	}

	self->joysticks = g_slice_alloc(sizeof(self->joysticks[0]) *
									self->n_joysticks);

	gsize i;
	for(i = 0; i < self->n_joysticks; i ++) {
		self->joysticks[i] = clazz->get_joystick(self, i, &error);
		if(error) {
			goto free_joysticks;
		}
	}

	return;

free_joysticks:
	for(i = 0; i < self->n_joysticks; i ++) {
		if(self->joysticks[i]) {
			base_unref(self->joysticks[i]);
		}
	}
	g_slice_free1(sizeof(self->joysticks[0]) * self->n_joysticks,
				  self->joysticks);
	self->n_joysticks = 0;
dump_error:
	if(error) {
		g_warning("Error occured while initializing joysticks: %s",
				  error->message);
		g_error_free(error);
	}
}

gboolean platform_init(Platform *self, GError **error)
{
	g_return_val_if_fail(self, FALSE);
	g_return_val_if_fail(! error || ! *error, FALSE);

	if(self->initialized) {
		return TRUE;
	}

	PlatformClass *clazz = PLATFORM_GET_CLASS(self);
	g_return_val_if_fail(clazz->init, FALSE);

	self->initialized = clazz->init(self, error);
	if(! self->initialized) {
		return FALSE;
	}

	platform_init_joysticks(self, clazz);

	return self->initialized = TRUE;
}

gint platform_get_n_joysticks(Platform *self)
{
	g_return_val_if_fail(self && self->initialized, 0);

	return self->n_joysticks;
}

Joystick * platform_get_joystick(Platform *self, gint n)
{
	g_return_val_if_fail(self && self->initialized, NULL);
	g_return_val_if_fail(0 <= n && n < platform_get_n_joysticks(self), NULL);

	return self->joysticks[n];
}
