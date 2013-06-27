#include "item.h"
#include "arrow.h"
#include "utils.h"

struct _Arrow
{
	Item parent;

	gfloat x;

	gfloat y;

	gfloat angle;

	guint32 argb;
};

static ItemClass arrow_class;

Arrow * arrow_new(gfloat x, gfloat y, gfloat angle, guint32 argb)
{
	Arrow *self = base_new(&arrow_class);
	*self = (Arrow){
		.x		= x,
		.y		= y,
		.angle	= angle,
		.argb	= argb,
	};

	return self;
}

static void arrow_draw(Item *item, cairo_t *cr)
{
	Arrow *self = ARROW(item);

	guint32 argb = self->argb;
	cairo_set_source_rgba(cr, RED(argb), BLUE(argb), BLUE(argb), ALPHA(argb));
	cairo_move_to(cr, self->x, self->y);
	cairo_rel_line_to(cr, 10, 20);
	cairo_rel_line_to(cr, -10, -5);
	cairo_rel_line_to(cr, -10, 5);
	cairo_close_path(cr);
	cairo_paint(cr);
}

static ItemClass arrow_class = {
		.parent = {
			.name			= "Arrow",
			.parent			= BASE_CLASS(&item_class),
			.size			= sizeof(Arrow),
		},
		.draw			= arrow_draw,
};
