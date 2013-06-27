#include "item.h"
#include "arrow.h"
#include "utils.h"

#define ANGLE_PER_ROTATE		(10.0)

#define ANGLE_NORMALIZE(a)		( \
		(a) > 360.0 \
			? (a) - 360.0f \
			: (a) < 0.0 \
				? 360.0f + (a) \
				: (a))

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
	cairo_rotate(cr, TO_RADIAN(self->angle));
	cairo_rel_move_to(cr, 0, -(self->height / 2));
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
	self->angle = ANGLE_NORMALIZE(self->angle);
}

gboolean arrow_update_key_state(Arrow *self, GameKeys keys)
{
	return FALSE;
}

static ItemClass arrow_class = {
		.parent = {
			.name			= "Arrow",
			.parent			= BASE_CLASS(&item_class),
			.size			= sizeof(Arrow),
		},
		.draw			= arrow_draw,
};
