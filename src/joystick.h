#ifndef __JOYSTICK_H_
#define __JOYSTICK_H_

#include "base.h"

G_BEGIN_DECLS

#define JOYSTICK_CLASS(c)			((JoystickClass *)(c))

#define JOYSTICK(o)					((Joystick *)(o))

#define JOYSTICK_GET_CLASS(o)		(JOYSTICK_CLASS(BASE_GET_CLASS(o)))

typedef struct _JoystickClass JoystickClass;

typedef struct _Joystick Joystick;

typedef struct _JoystickEvent JoystickEvent;

typedef void (*JoystickEventHandler)(Joystick *self, JoystickEvent *event);

struct _JoystickEvent
{
	/* button or axis number */
	gint16 number;

	/*for buttons, 1 (press) or 0 (release). for axes, range from -32768 to 31767*/
	gint16 value;
};

struct _JoystickClass
{
	BaseClass parent;

	gboolean (*open)(Joystick *self);

	void (*close)(Joystick *self);

	gint (*get_n_buttons)(Joystick *self);

	gint (*get_n_axes)(Joystick *self);

	void (*get_value_range)(Joystick *self,
							gint *value_min,
							gint *value_max);
};

struct _Joystick
{
	Base parent;

	gint value_min;

	gint value_max;

	gboolean opened;

	JoystickEventHandler handler;
};

gint joystick_get_n_buttons(Joystick *self);

gint joystick_get_n_axes(Joystick *self);

void joystick_set_handler(Joystick *self, JoystickEventHandler handler);

gint joystick_get_n_axes(Joystick *self);

void joystick_dispatch_event(Joystick *self, JoystickEvent *event);

extern JoystickClass joystick_class;

G_END_DECLS

#endif /* __JOYSTICK_H_ */
