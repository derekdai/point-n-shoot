#include "base.h"

static void base_init(Base *self, BaseClass *clazz)
{
	if(clazz->parent) {
		base_init(self, clazz->parent);
	}

	if(clazz->init) {
		clazz->init(self);
	}
}

gpointer base_new(gpointer clazz)
{
	BaseClass *base_class = clazz;
	g_return_val_if_fail(clazz && sizeof(Base) <= base_class->size, NULL);

	Base *self = g_slice_alloc(base_class->size);
	self->clazz = base_class;
	self->ref_count = 1;
	base_init(self, base_class);

	return self;
}

gpointer base_ref(gpointer base)
{
	g_return_val_if_fail(base && 0 < BASE(base)->ref_count, NULL);

	g_atomic_int_inc(&BASE(base)->ref_count);

	return base;
}

void base_unref(gpointer base)
{
	g_return_if_fail(base);

	Base *self = BASE(base);
	if(! g_atomic_int_dec_and_test(&self->ref_count)) {
		return;
	}

	BaseClass *clazz = self->clazz;
	while(clazz) {
		if(clazz->dispose) {
			clazz->dispose(base);
		}

		clazz = clazz->parent;
	}

	g_slice_free1(clazz->size, base);
}

BaseClass base_class = {
		.name			= "Base",
		.size			= sizeof(Base),
};
