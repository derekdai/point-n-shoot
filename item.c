#include "item.h"

void item_draw(Item *self, cairo_t *cr)
{
	g_return_if_fail(self && ITEM_CLASS(BASE_GET_CLASS(self))->draw);
	g_return_if_fail(cr);

	ITEM_CLASS(BASE_GET_CLASS(self))->draw(self, cr);
}

void item_refresh(Item *self, struct _Scene *scene)
{
	g_return_if_fail(self && ITEM_CLASS(BASE_GET_CLASS(self))->refresh);
	g_return_if_fail(scene);

	ITEM_CLASS(BASE_GET_CLASS(self))->refresh(self, scene);
}

ItemClass item_class = {
		.parent = {
			.name			= "Item",
			.parent			= &base_class,
			.size			= sizeof(Item),
		},
		.draw			= NULL,
};
