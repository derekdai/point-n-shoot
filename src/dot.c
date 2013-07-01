#include <math.h>
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
	Dot *self = DOT(item);

	gfloat arrow_x, arrow_y;
	scene_get_arrow_postion(scene, &arrow_x, &arrow_y);

	gfloat delta_x = arrow_x - self->x;
	gfloat delta_y = arrow_y - self->y;
	gfloat len = sqrt(delta_x * delta_x + delta_y *delta_y);
	if(0 == len) {
		return;
	}

	/* dot product is a better choice which don't have divided by zero problem */
	gfloat unit_x = delta_x / len;
	gfloat unit_y = delta_y / len;
	gfloat dot_prod = unit_x * 1 + unit_y * 0;
	gfloat radian = acos(dot_prod);
	if(delta_y < 0) {
		radian += PI;
	}
	//g_message("%f\t%f\t%f\t%f\t%f", self->x, self->y, delta_x, delta_y, radian * 180 / PI);

	self->x += self->speed * dot_prod;
	self->y += self->speed * sin(radian);
}
