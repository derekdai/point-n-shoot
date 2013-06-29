#ifndef __ARROW_H_
#define __ARROW_H_

#include <glib.h>
#include "event.h"

G_BEGIN_DECLS

#define ARROW(o)			((Arrow *)(o))

typedef struct _Arrow Arrow;

Arrow * arrow_new(gfloat x,
				  gfloat y,
				  gfloat width,
				  gfloat height,
				  gfloat degree,
				  gfloat speed,
				  guint32 argb);

void arrow_set_x(Arrow *self, gfloat x);

gfloat arrow_get_x(Arrow *self);

void arrow_set_y(Arrow *self, gfloat x);

gfloat arrow_get_y(Arrow *self);

void arrow_set_degree(Arrow *self, gfloat x);

void arrow_forward(Arrow *self, gfloat distance);

void arrow_rotate(Arrow *self, gfloat degree);

G_END_DECLS

#endif /* __ARROW_H_ */
