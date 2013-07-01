#ifndef __DOT_H_
#define __DOT_H_

#include <glib.h>

G_BEGIN_DECLS

#define DOT(o)					((Dot *)(o))

typedef struct _Dot Dot;

Dot * dot_new(gfloat x, gfloat y, gfloat speed, gfloat radius, guint32 argb);

G_END_DECLS

#endif /* __DOT_H_ */
