#ifndef __SCENE_H_
#define __SCENE_H_

#include <gtk/gtk.h>
#include "item.h"
#include "event.h"

G_BEGIN_DECLS

#define SCENE(o)			((Scene *)(o))

typedef struct _Scene Scene;

typedef enum _SceneState SceneState;

enum _SceneState
{
	SCENE_STATE_NULL,
	SCENE_STATE_PLAYING,
	SCENE_STATE_PAUSE,
	SCENE_STATE_END,
};

Scene * scene_new();

GtkWidget * scene_get_widget(Scene *self);

void scene_set_width(Scene *self, gfloat width);

void scene_set_height(Scene *self, gfloat height);

void scene_start(Scene *self);

SceneState scene_get_state(Scene *self);

void scene_pause(Scene *self);

void scene_restart(Scene *self);

void scene_add_item(Scene *self, Item *item);

void scene_remove_item(Scene *self, Item *item);

void scene_set_keys(Scene *self, GameKeys keys);

GameKeys scene_get_keys(Scene *self);

G_END_DECLS

#endif /* __SCENE_H_ */
