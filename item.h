#ifndef __ITEM_H_
#define __ITEM_H_

#include <glib.h>
#include <cairo.h>

G_BEGIN_DECLS

#define ITEM_CLASS(c)			((ItemClass *)(c))

#define ITEM(o)					((Item *)(o))

typedef struct _ItemClass ItemClass;

typedef struct _Item Item;

struct _ItemClass
{
	const gchar *name;

	ItemClass *parent;

	gsize size;

	void (* draw)(Item *base, cairo_t *cr);

	void (* rotate)(Item *base, gfloat angle);

	void (* forward)(Item *base, gfloat distance);

	void (* dispose)(Item *base);
};

struct _Item
{
	ItemClass *clazz;

	gfloat x;

	gfloat y;

	gfloat angle;

	guint32 argb;
};

Item * item_new(ItemClass *clazz);

Item * item_new_full(ItemClass *clazz,
					 gfloat x,
					 gfloat y,
					 gfloat angle,
					 guint32 argb);

void item_draw(Item *self, cairo_t *cr);

void item_rotate(Item *self, gfloat angle);

void item_forward(Item *self, gfloat forward);

void item_free(gpointer item);

extern ItemClass item_class;

G_END_DECLS

#endif /* __POINT_N_SHOOT_H_ */
