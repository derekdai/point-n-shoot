#ifndef __POINT_N_SHOOT_H_
#define __POINT_N_SHOOT_H_

#include <glib.h>

G_BEGIN_DECLS

typedef struct _PointNShoot PointNShoot;

PointNShoot * pns_get_default();

void pns_run(PointNShoot *self);

void pns_quit(PointNShoot *self);

void pns_destroy();

G_END_DECLS

#endif /* __POINT_N_SHOOT_H_ */
