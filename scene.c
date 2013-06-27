#include "scene.h"
#include "utils.h"

struct _Scene
{
	Item base;

	GList *items;
};

static ItemClass scene_class;

Scene * scene_new()
{
	return base_new(&scene_class);
}

void scene_add_item(Scene *self, Item *item)
{
	self->items = g_list_prepend(self->items, base_ref(item));
}

void scene_remove_item(Scene *self, Item *item)
{
	GList *node = g_list_find(self->items, item);
	if(! node) {
		return;
	}

	self->items = g_list_remove_link(self->items, node);
	base_unref(node->data);
}

static void scene_init(Base *base)
{
	SCENE(base)->items = NULL;
}

static void scene_draw(Item *item, cairo_t *cr)
{
	g_list_foreach(SCENE(item)->items, (GFunc) item_draw, cr);
}

static void scene_dispose(Base *base)
{
	g_list_free_full(SCENE(base)->items, base_unref);
}

static ItemClass scene_class = {
		.parent = {
			.name			= "Scene",
			.parent			= BASE_CLASS(&item_class),
			.size			= sizeof(Scene),
			.init			= scene_init,
			.dispose		= scene_dispose,
		},
		.draw			= scene_draw,
};
