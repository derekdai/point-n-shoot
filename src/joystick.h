#ifndef __JOYSTICK_H_
#define __JOYSTICK_H_

#include "base.h"

G_BEGIN_DECLS

#define JOYSTICK_CLASS(c)			((JoystickClass *)(c))

#define JOYSTICK(o)					((Joystick *)(o))

#define JOYSTICK_GET_CLASS(o)		(JOYSTICK_CLASS(BASE_GET_CLASS(o)))

typedef enum _JoystickEventType JoystickEventType;

typedef struct _JoystickClass JoystickClass;

typedef struct _Joystick Joystick;

typedef struct _JoystickEvent JoystickEvent;

typedef void (*JoystickEventHandler)(Joystick *self,
									 JoystickEvent *event,
									 gpointer user_data);

enum _JoystickEventType
{
	JOYSTICK_EVENT_BUTTON		= 1 << 0,
	JOYSTICK_EVENT_AXES			= 1 << 1,
};

struct _JoystickEvent
{
	JoystickEventType type;

	guint time;

	/* button or axis number */
	gint number;

	/*for buttons, 1 (press) or 0 (release). for axes -1 ~ 0 ~ 1*/
	gfloat value;
};

struct _JoystickClass
{
	BaseClass parent;

	gboolean (*open)(Joystick *joystick, const gchar *uri, GError **error);

	void (*close)(Joystick *joystick);

	gint (*get_n_buttons)(Joystick *joystick, GError **error);

	gint (*get_n_axes)(Joystick *joystick, GError **error);

	gchar * (*get_name)(Joystick *joystick, GError **error);
};

struct _Joystick
{
	Base parent;

	gchar *name;

	gint n_buttons;

	gint n_axes;

	gboolean opened;

	JoystickEventHandler handler;

	gpointer handler_data;
};

gboolean joystick_open(Joystick *self, const gchar *uri, GError **error);

void joystick_close(Joystick *self);

gint joystick_get_n_buttons(Joystick *self);

gint joystick_get_n_axes(Joystick *self);

const gchar * joystick_get_name(Joystick *self);

void joystick_set_handler(Joystick *self,
						  JoystickEventHandler handler,
						  gpointer user_data);

void joystick_dispatch_event(Joystick *self, JoystickEvent *event);

extern JoystickClass joystick_class;

G_END_DECLS

#endif /* __JOYSTICK_H_ */
