#include "item.h"
#include "arrow.h"
#include "utils.h"

struct _Arrow
{
	Item parent;

	gfloat x;

	gfloat y;

	gfloat width;

	gfloat height;

	gfloat angle;

	gfloat speed;

	guint32 argb;
};

static ItemClass arrow_class;

Arrow * arrow_new(gfloat x,
				  gfloat y,
				  gfloat width,
				  gfloat height,
				  gfloat angle,
				  gfloat speed,
				  guint32 argb)
{
	Arrow *self = base_new(&arrow_class);
	self->x = x;
	self->y	= y;
	self->width = width;
	self->height = height;
	self->angle	= angle;
	self->speed = speed;
	self->argb	= argb;

	return self;
}

static void arrow_draw(Item *item, cairo_t *cr)
{
	Arrow *self = ARROW(item);

	guint32 argb = self->argb;
	cairo_set_source_rgb(cr, RED(argb), GREEN(argb), BLUE(argb));
	cairo_move_to(cr, self->x + self->width / 2, self->y + self->height / 2);
	cairo_rotate(cr, self->angle / 180 * 3.141592654);
	cairo_rel_line_to(cr, self->width / 2, self->height);
	cairo_rel_line_to(cr, -(self->width / 2), -(self->height / 4));
	cairo_rel_line_to(cr, -(self->width / 2), self->height / 4);
	cairo_close_path(cr);
	cairo_fill(cr);
}

void arrow_rotate(Arrow *self, gfloat angle)
{
	g_return_if_fail(self);

	self->angle += angle;
	if(self->angle > 360.0) {
		self->angle -= 360.0;
	}
	else if(self->angle < 0.0){
		self->angle = 360.0 - self->angle;
	}
}

static ItemClass arrow_class = {
		.parent = {
			.name			= "Arrow",
			.parent			= BASE_CLASS(&item_class),
			.size			= sizeof(Arrow),
		},
		.draw			= arrow_draw,
};
