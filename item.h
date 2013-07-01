#ifndef __ITEM_H_
#define __ITEM_H_

#include <glib.h>
#include <cairo.h>
#include "base.h"

G_BEGIN_DECLS

#define ITEM_CLASS(c)			((ItemClass *)(c))

#define ITEM(o)					((Item *)(o))

struct _Scene;

typedef struct _ItemClass ItemClass;

typedef struct _Item Item;

struct _ItemClass
{
	BaseClass parent;

	void (* draw)(Item *item, cairo_t *cr);

	/* not a good design */
	void (* refresh)(Item *item, struct _Scene *scene);
};

struct _Item
{
	Base parent;

	Item *prev;

	Item *next;
};

void item_draw(Item *self, cairo_t *cr);

void item_refresh(Item *self, struct _Scene *scene);

Item * item_get_prev(Item *self);

Item * item_get_next(Item *self);

/**
 * won't base_ref()
 */
Item * item_append(Item *self, Item *items);

/**
 * won't base_ref()
 */
Item * item_prepend(Item *self, Item *items);

/**
 * won't base_unref()
 */
Item * item_remove(Item *self, Item * item);

void item_foreach(Item *self, GFunc func, gpointer user_data);

extern ItemClass item_class;

G_END_DECLS

#endif /* __ITEM_H_ */
