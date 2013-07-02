#include "joystick.h"

static void joystick_init(Base *base)
{
	Joystick *self = JOYSTICK(base);
	self->handler = NULL;

	JoystickClass *clazz = JOYSTICK_GET_CLASS(base);
	self->opened = clazz->open(self);
	if(! self->opened) {
		return;
	}

	clazz->get_value_range(self, &self->value_min, &self->value_max);
}

static void joystick_dispose(Base *base)
{
	Joystick *self = JOYSTICK(base);
	if(! self->opened) {
		return;
	}

	JoystickClass *clazz = JOYSTICK_GET_CLASS(base);
	clazz->close(self);
}

void joystick_dispatch_event(Joystick *self, JoystickEvent *event)
{
	g_return_if_fail(self);
	g_return_if_fail(event);

	if(self->handler) {
		self->handler(self, event);
	}
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
