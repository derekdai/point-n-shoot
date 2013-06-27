#include "item.h"

Item * item_new(ItemClass *clazz)
{
	return item_new_full(clazz, 0.0, 0.0, 0.0, 0xff7f7f7f);
}

Item * item_new_full(ItemClass *clazz,
					 gfloat x,
					 gfloat y,
					 gfloat angle,
					 guint32 argb)
{
	g_return_val_if_fail(clazz && sizeof(Item) <= clazz->size, NULL);

	Item *self = g_slice_alloc(clazz->size);
	*self = (Item) {
		.clazz		= clazz,
		.x			= x,
		.y			= y,
		.angle		= angle,
		.argb		= argb,
	};

	return self;
}

void item_draw(Item *self, cairo_t *cr)
{
	g_return_if_fail(self && self->clazz->draw);
	g_return_if_fail(cr);

	self->clazz->draw(self, cr);
}

static void item_default_rotate(Item *self, gfloat angle)
{
	self->angle += angle;
}

void item_rotate(Item *self, gfloat angle)
{
	g_return_if_fail(self && self->clazz->rotate);

	self->clazz->rotate(self, angle);
}

static void item_default_forward(Item *self, gfloat distance)
{

}

void item_forward(Item *self, gfloat distance)
{
	g_return_if_fail(self && self->clazz->forward);

	self->clazz->forward(self, distance);
}

void item_free(gpointer item)
{
	Item *self = ITEM(item);
	ItemClass *clazz = self->clazz;
	if(clazz->dispose) {
		clazz->dispose(item);
	}

	g_slice_free1(clazz->size, item);
}

ItemClass item_class = {
		.name			= "Item",
		.parent			= NULL,
		.size			= sizeof(Item),
		.draw			= NULL,
		.rotate			= item_default_rotate,
		.forward		= item_default_forward,
		.dispose		= NULL,
};
