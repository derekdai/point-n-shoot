#ifndef __UTILS_H_
#define __UTILS_H_

#include <glib.h>

G_BEGIN_DECLS

#define ALPHA(argb)		((((argb) >> 24) & 0xff) / 255.0)
#define RED(argb)		((((argb) >> 16) & 0xff) / 255.0)
#define GREEN(argb)		((((argb) >> 8) & 0xff) / 255.0)
#define BLUE(argb)		(((argb) & 0xff) / 255.0)

G_END_DECLS

#endif /* __POINT_N_SHOOT_H_ */
