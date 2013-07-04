#ifndef __SCENE_H_
#define __SCENE_H_

#include <gtk/gtk.h>
#include "item.h"

G_BEGIN_DECLS

#define SCENE(o)			((Scene *)(o))

typedef struct _Scene Scene;

typedef enum _SceneState SceneState;

typedef enum _SceneLayer SceneLayer;

typedef void (* SceneForeachLayerFunc)(Scene *self,
									   SceneLayer layer,
									   Item *items,
									   gpointer user_data);

enum _SceneState
{
	SCENE_STATE_NULL,
	SCENE_STATE_PLAYING,
	SCENE_STATE_PAUSE,
	SCENE_STATE_END,
};

enum _SceneLayer
{
	SCENE_LAYER_FOREGROUND,
	SCENE_LAYER_BACKGROUND,
	SCENE_LAYER_LAST,
};

Scene * scene_new();

GtkWidget * scene_get_widget(Scene *self);

void scene_set_width(Scene *self, gfloat width);

void scene_set_height(Scene *self, gfloat height);

void scene_start(Scene *self);

SceneState scene_get_state(Scene *self);

void scene_pause(Scene *self);

void scene_restart(Scene *self);

void scene_add_item(Scene *self, SceneLayer layer, Item *item);

void scene_remove_item(Scene *self, SceneLayer layer, Item *item);

void scene_foreach_layer(Scene *self,
						 SceneForeachLayerFunc func,
						 gpointer user_data);

void scene_update_axes(Scene *self, gfloat x_axis, gfloat y_axis);

void scene_get_axes(Scene *self, gfloat *x_axis, gfloat *y_axis);

void scene_get_arrow_postion(Scene *self, gfloat *x, gfloat *y);

G_END_DECLS

#endif /* __SCENE_H_ */
