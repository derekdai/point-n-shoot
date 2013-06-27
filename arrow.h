#ifndef __ARROW_H_
#define __ARROW_H_

#include <glib.h>

G_BEGIN_DECLS

#define ARROW(o)			((Arrow *)(o))

typedef struct _Arrow Arrow;

Arrow * arrow_new(gfloat x,
				  gfloat y,
				  gfloat width,
				  gfloat height,
				  gfloat angle,
				  gfloat speed,
				  guint32 argb);

void arrow_forward(Arrow *self, gfloat distance);

void arrow_rotate(Arrow *self, gfloat angle);

G_END_DECLS

#endif /* __ARROW_H_ */
