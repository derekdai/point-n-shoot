#ifndef __ITEM_H_
#define __ITEM_H_

#include <glib.h>
#include <cairo.h>
#include "base.h"

G_BEGIN_DECLS

#define ITEM_CLASS(c)			((ItemClass *)(c))

#define ITEM(o)					((Item *)(o))

typedef struct _ItemClass ItemClass;

typedef struct _Item Item;

struct _ItemClass
{
	BaseClass parent;

	void (* draw)(Item *item, cairo_t *cr);
};

struct _Item
{
	Base parent;
};

void item_draw(Item *self, cairo_t *cr);

extern ItemClass item_class;

G_END_DECLS

#endif /* __ITEM_H_ */
