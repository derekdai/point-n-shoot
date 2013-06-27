#ifndef __ARROW_H_
#define __ARROW_H_

#include <glib.h>

G_BEGIN_DECLS

#define ARROW(o)			((Arrow *)(o))

typedef struct _Arrow Arrow;

Arrow * arrow_new(gfloat x, gfloat y, guint32 argb);

G_END_DECLS

#endif /* __POINT_N_SHOOT_H_ */
