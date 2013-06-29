#include "scene.h"
#include "arrow.h"
#include "dot.h"
#include "utils.h"

#define ARROW_WIDTH			(20.0)

#define ARROW_HEIGHT		(16.0)

#define ARROW_ANGLE			(90.0)

#define ARROW_SPEED			(10.0)

#define ARROW_COLOR			(0xff5f5f5f)

#define DOT_RADIUS			(5.0)

#define DOT_SPEED			(0.005)

#define DOT_COLOR			(0xffff6f6f)

#define MS_PER_SECOND		(1000)

#define FRAME_PER_SECOND	(30)

#define SCENE_REFRESH_INTERVAL	(MS_PER_SECOND / FRAME_PER_SECOND)

struct _Scene
{
	Item base;

	GtkWidget *canvas;

	gfloat width;

	gfloat height;

	Arrow *arrow;

	GList *items;

	guint timer_handle;

	GameKeys keys;

	SceneState state;
};

static void scene_init(Base *base);

static void scene_dispose(Base *base);

static void scene_start_refresh_timer(Scene *self);

static void scene_remove_refresh_timer(Scene *self);

static void scene_remove_all_items(Scene *self);

static void scene_draw(Scene *self, cairo_t *cr);

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

Scene * scene_new()
{
	return base_new(&scene_class);
}

void scene_set_width(Scene *self, gfloat width)
{
	g_return_if_fail(self);
	g_return_if_fail(0 <= width);

	self->width = width;
}

void scene_set_height(Scene *self, gfloat height)
{
	g_return_if_fail(self);
	g_return_if_fail(0 <= height);

	self->height = height;
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
	Scene *self = SCENE(base);
	self->width = 0;
	self->height = 0;
	self->items = NULL;
	self->timer_handle = 0;
	self->state = SCENE_STATE_NULL;
	self->keys = GAME_KEYS_NONE;

	self->canvas = gtk_drawing_area_new();
	g_signal_connect_swapped(self->canvas,
							 "draw",
							 G_CALLBACK(scene_draw),
							 self);
	g_signal_connect_swapped(self->canvas,
							 "destroy",
							 G_CALLBACK(scene_pause),
							 self);

	self->arrow = arrow_new(0.0,
							0.0,
							ARROW_WIDTH,
							ARROW_HEIGHT,
							ARROW_ANGLE,
							ARROW_SPEED,
							ARROW_COLOR);
	scene_add_item(self, ITEM(self->arrow));
}

GtkWidget * scene_get_widget(Scene *self)
{
	g_return_val_if_fail(self, NULL);

	return self->canvas;
}

static void scene_draw(Scene *self, cairo_t *cr)
{
	g_list_foreach(self->items, (GFunc) item_draw, cr);
}

static void scene_dispose(Base *base)
{
	Scene *self = SCENE(base);
	scene_remove_refresh_timer(self);
	scene_remove_all_items(self);
}

static gboolean scene_refresh(Scene *self)
{
	item_refresh(ITEM(self->arrow), self);
	g_list_foreach(self->items, (GFunc) item_refresh, self);
	gtk_widget_queue_draw(self->canvas);

	return TRUE;
}

static void scene_start_refresh_timer(Scene *self)
{
	if(self->timer_handle) {
		return;
	}

	self->timer_handle = g_timeout_add(SCENE_REFRESH_INTERVAL,
									   (GSourceFunc) scene_refresh,
									   self);
}

static void scene_remove_refresh_timer(Scene *self)
{
	if(! self->timer_handle) {
		return;
	}

	g_source_remove(self->timer_handle);
	self->timer_handle = 0;
}

static void scene_reset_arrow(Scene *self)
{
	arrow_set_x(self->arrow, self->width / 2);
	arrow_set_y(self->arrow, self->height / 2);
	arrow_set_degree(self->arrow, ARROW_ANGLE);
}

static void scene_remove_all_items(Scene *self)
{
	if(self->items) {
		g_list_free_full(self->items, base_unref);
		self->items = NULL;
	}
}

static void scene_reset(Scene *self)
{
	if(SCENE_STATE_NULL == self->state) {
		return;
	}

	scene_reset_arrow(self);
	scene_remove_all_items(self);
	scene_remove_refresh_timer(self);

	self->state = SCENE_STATE_NULL;
}

void scene_start(Scene *self)
{
	g_return_if_fail(self);
	g_return_if_fail(SCENE_STATE_END != self->state);

	if(SCENE_STATE_NULL == self->state) {
		scene_reset_arrow(self);

		gint i;
		GRand *rand = g_rand_new();
		gdouble x_range_end = self->width - DOT_RADIUS;
		gdouble y_range_end = self->height - DOT_RADIUS;
		for(i = 0; i < 50; i ++) {
			Dot *dot = dot_new(g_rand_double_range(rand, DOT_RADIUS, x_range_end),
							   g_rand_double_range(rand, DOT_RADIUS, y_range_end),
							   DOT_SPEED,
							   DOT_RADIUS,
							   DOT_COLOR);
			scene_add_item(self, ITEM(dot));
			base_unref(dot);
		}
		g_rand_free(rand);
	}

	scene_start_refresh_timer(self);

	self->state = SCENE_STATE_PLAYING;
}

SceneState scene_get_state(Scene *self)
{
	g_return_val_if_fail(self, SCENE_STATE_NULL);

	return self->state;
}

void scene_pause(Scene *self)
{
	if(SCENE_STATE_PLAYING != self->timer_handle) {
		return;
	}

	scene_remove_refresh_timer(self);
	self->timer_handle = SCENE_STATE_PAUSE;
}

void scene_restart(Scene *self)
{
	scene_reset(self);
	scene_start(self);
}

void scene_set_keys(Scene *self, GameKeys keys)
{
	g_return_if_fail(self);

	self->keys = keys;

	scene_remove_refresh_timer(self);
	scene_start_refresh_timer(self);
	scene_refresh(self);
}

GameKeys scene_get_keys(Scene *self)
{
	g_return_val_if_fail(self, GAME_KEYS_NONE);

	return self->keys;
}

void scene_get_arrow_postion(Scene *self, gfloat *x, gfloat *y)
{
	g_return_if_fail(self && x && y);

	*x = arrow_get_x(self->arrow);
	*y = arrow_get_y(self->arrow);
}
