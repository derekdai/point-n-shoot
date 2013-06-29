#include <math.h>
#include "item.h"
#include "arrow.h"
#include "scene.h"
#include "utils.h"
#include "event.h"

#define ANGLE_PER_ROTATE		(10.0)

#define NORMALIZE_DEGREE(a)		( \
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

	gfloat degree;

	gfloat speed;

	guint32 argb;
};

static void arrow_draw(Item *item, cairo_t *cr);

static void arrow_refresh(Item *item, Scene *scene);

static ItemClass arrow_class = {
		.parent = {
			.name			= "Arrow",
			.parent			= BASE_CLASS(&item_class),
			.size			= sizeof(Arrow),
		},
		.draw			= arrow_draw,
		.refresh		= arrow_refresh,
};

Arrow * arrow_new(gfloat x,
				  gfloat y,
				  gfloat width,
				  gfloat height,
				  gfloat degree,
				  gfloat speed,
				  guint32 argb)
{
	Arrow *self = base_new(&arrow_class);
	self->x = x;
	self->y	= y;
	self->width = width;
	self->height = height;
	self->degree	= degree;
	self->speed = speed;
	self->argb	= argb;

	return self;
}

static void arrow_draw(Item *item, cairo_t *cr)
{
	Arrow *self = ARROW(item);

	gfloat half_width = self->width / 2, half_height = self->height / 2;
	cairo_move_to(cr, self->x + half_width, self->y + half_height);
	cairo_rotate(cr, TO_RADIAN(self->degree));
	cairo_rel_move_to(cr, half_width, 0);

	guint32 argb = self->argb;
	cairo_set_source_rgb(cr, RED(argb), GREEN(argb), BLUE(argb));
	cairo_rel_line_to(cr, -self->width, half_height);
	cairo_rel_line_to(cr, half_width / 2, -half_height);
	cairo_rel_line_to(cr, -(half_width / 2), -half_height);
	cairo_close_path(cr);
	cairo_fill(cr);
	cairo_stroke(cr);
}

void arrow_forward(Arrow *self, gfloat distance)
{
	gfloat radian = TO_RADIAN(self->degree);
	gfloat h = cos(radian);
	gfloat v = sin(radian);
	self->x += distance * h;
	self->y += distance * v;
}

void arrow_rotate(Arrow *self, gfloat degree)
{
	g_return_if_fail(self);

	self->degree += degree;
	self->degree = NORMALIZE_DEGREE(self->degree);
}

void arrow_set_x(Arrow *self, gfloat x)
{
	g_return_if_fail(self);

	self->x = x;
}

gfloat arrow_get_x(Arrow *self)
{
	g_return_val_if_fail(self, 0.0);

	return self->x;
}

void arrow_set_y(Arrow *self, gfloat y)
{
	g_return_if_fail(self);

	self->y = y;
}

gfloat arrow_get_y(Arrow *self)
{
	g_return_val_if_fail(self, 0.0);

	return self->y;
}

void arrow_set_degree(Arrow *self, gfloat degree)
{
	g_return_if_fail(self);

	self->degree = degree;
}

static void arrow_refresh(Item *item, Scene *scene)
{
	Arrow *self = ARROW(item);
	GameKeys keys = scene_get_keys(scene);
	if(GAME_KEYS_LEFT & keys) {
		arrow_rotate(self, ANGLE_PER_ROTATE);
	}
	else if(GAME_KEYS_RIGHT & keys) {
		arrow_rotate(self, -ANGLE_PER_ROTATE);
	}
	if(GAME_KEYS_UP & keys) {
		arrow_forward(self, 10.0);
	}
}
