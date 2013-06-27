#ifndef __SCENE_H_
#define __SCENE_H_

#include <glib.h>
#include "item.h"

G_BEGIN_DECLS

#define SCENE(o)			((Scene *)(o))

typedef struct _Scene Scene;

Scene * scene_new();

void scene_add_item(Scene *self, Item *item);

void scene_remove_item(Scene *self, Item *item);

G_END_DECLS

#endif /* __SCENE_H_ */
