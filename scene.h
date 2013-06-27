#ifndef __ARROW_H_
#define __ARROW_H_

#include <glib.h>
#include "item.h"

G_BEGIN_DECLS

#define SCENE(o)			((Scene *)(o))

typedef struct _Scene Scene;

Scene * scene_new();

void scene_add_item(Scene *self, Item *item);

void scene_remove_item(Scene *self, Item *item);

G_END_DECLS

#endif /* __POINT_N_SHOOT_H_ */
