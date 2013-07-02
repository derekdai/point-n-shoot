#ifndef __PLATFORM_H_
#define __PLATFORM_H_

#include "joystick.h"

G_BEGIN_DECLS

#define PLATFORM_CLASS(c)			((PlatformClass *)(c))

#define PLATFORM(o)					((Platform *)(o))

#define PLATFORM_GET_CLASS(o)		(PLATFORM_CLASS(BASE_GET_CLASS(o)))

typedef struct _PlatformClass PlatformClass;

typedef struct _Platform Platform;

struct _PlatformClass
{
	BaseClass parent;

	gboolean (*init)(Platform *self, GError **error);

	gint (*get_n_joysticks)(Platform *self, GError **error);

	Joystick * (*get_joystick)(Platform *self, gint n, GError **error);
};

struct _Platform
{
	Base parent;

	gboolean initialized;

	Joystick ** joysticks;

	gint n_joysticks;
};

gboolean platform_init(Platform *self, GError **error);

gint platform_get_n_joysticks(Platform *self);

Joystick * platform_get_joystick(Platform *self, gint n);

extern PlatformClass platform_class;

G_END_DECLS

#endif /* __PLATFORM_H_ */
