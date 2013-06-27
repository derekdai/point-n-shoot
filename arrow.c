#include "item.h"
#include "arrow.h"

struct _Arrow
{
	Item base;
};

static ItemClass arrow_class;

Arrow * arrow_new(gfloat x, gfloat y, guint32 argb)
{
	return ARROW(item_new_full(&arrow_class, x, y, 0, argb));
}

static void arrow_draw(Item *item, cairo_t *cr)
{
	Arrow *self = ARROW(item);
}

static ItemClass arrow_class = {
		.name			= "Arrow",
		.parent			= &item_class,
		.size			= sizeof(Arrow),
		.draw			= arrow_draw,
		.rotate			= NULL,
		.forward		= NULL,
		.dispose		= NULL,
};
