#ifndef __BASE_H_
#define __BASE_H_

#include <glib.h>

G_BEGIN_DECLS

#define BASE_CLASS(c)			((BaseClass *)(c))

#define BASE(o)					((Base *)(o))

#define BASE_GET_CLASS(o)		(BASE(o)->clazz)

typedef struct _BaseClass BaseClass;

typedef struct _Base Base;

struct _BaseClass
{
	const gchar *name;

	BaseClass *parent;

	gsize size;

	void (*init)(Base *base);

	void (*dispose)(Base *base);
};

struct _Base
{
	BaseClass *clazz;

	gint ref_count;
};

/**
 * @clazz: point to BaseClass or its derivation
 */
gpointer base_new(gpointer clazz);

gpointer base_ref(gpointer base);

void base_unref(gpointer base);

extern BaseClass base_class;

G_END_DECLS

#endif /* __POINT_N_SHOOT_H_ */
