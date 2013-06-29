#include "item.h"
#include "dot.h"
#include "scene.h"
#include "utils.h"

struct _Dot
{
	Item parent;

	gfloat x;

	gfloat y;

	gfloat speed;

	gfloat radius;

	guint32 argb;
};

static void dot_init(Dot *self);

static void dot_draw(Item *item, cairo_t *cr);

static void dot_refresh(Item *item, Scene *scene);

static ItemClass dot_class = {
		.parent = {
			.name			= "Dot",
			.parent			= BASE_CLASS(&item_class),
			.size			= sizeof(Dot),
		},
		.draw			= dot_draw,
		.refresh		= dot_refresh,
};

Dot * dot_new(gfloat x, gfloat y, gfloat speed, gfloat radius, guint32 argb)
{
	Dot *self = base_new(&dot_class);
	self->x = x;
	self->y = y;
	self->speed = speed;
	self->radius = radius;
	self->argb = argb;
	return self;
}

static void dot_draw(Item *item, cairo_t *cr)
{
	Dot *self = DOT(item);
	cairo_set_source_rgb(cr,
						 RED(self->argb),
						 GREEN(self->argb),
						 BLUE(self->argb));
	cairo_arc(cr, self->x, self->y, self->radius, 0, 2 * PI);
	cairo_fill(cr);
}

static void dot_refresh(Item *item, Scene *scene)
{
	gfloat arrow_x, arrow_y;
	scene_get_arrow_postion(scene, &arrow_x, &arrow_y);

	Dot *self = DOT(item);
	self->x += (arrow_x - self->x) * self->speed;
	self->y += (arrow_y - self->y) * self->speed;
}
