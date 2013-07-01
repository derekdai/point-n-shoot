#include "item.h"

static void item_init(Base *base)
{
	Item *self = ITEM(base);

	self->prev = NULL;
	self->next = NULL;
}

void item_draw(Item *self, cairo_t *cr)
{
	g_return_if_fail(self && ITEM_CLASS(BASE_GET_CLASS(self))->draw);
	g_return_if_fail(cr);

	ITEM_CLASS(BASE_GET_CLASS(self))->draw(self, cr);
}

void item_refresh(Item *self, struct _Scene *scene)
{
	g_return_if_fail(self && ITEM_CLASS(BASE_GET_CLASS(self))->refresh);
	g_return_if_fail(scene);

	ITEM_CLASS(BASE_GET_CLASS(self))->refresh(self, scene);
}

Item * item_get_prev(Item *self)
{
	g_return_val_if_fail(self, NULL);

	return self->prev;
}

Item * item_get_next(Item *self)
{
	g_return_val_if_fail(self, NULL);

	return self->next;
}

Item * item_append(Item *self, Item *items)
{
	g_return_val_if_fail(items, NULL);

	if(! self) {
		self = items;
		goto end;
	}

	Item *tail = self;
	while(tail->next) {
		tail = tail->next;
	}

	Item *head = items;
	while(head->prev) {
		head = head->prev;
	}

	tail->next = head;
	head->prev = tail;

end:
	while(self->prev) {
		self = self->prev;
	}

	return self;
}

Item * item_prepend(Item *self, Item *items)
{
	g_return_val_if_fail(items, NULL);

	if(! self) {
		goto end;
	}

	Item *head = self;
	while(head->prev) {
		head = head->prev;
	}

	Item *tail = items;
	while(tail->next) {
		tail = tail->next;
	}

	head->prev = tail;
	tail->next = head;

end:
	while(items->prev) {
		items = items->prev;
	}

	return items;
}

void item_foreach(Item *self, GFunc func, gpointer user_data)
{
	g_return_if_fail(self);
	g_return_if_fail(func);

	Item *node = self;
	while(node->prev) {
		node = node->prev;
	}

	while(node) {
		Item *next = node->next;
		func(node, user_data);
		node = next;
	}
}

Item * item_remove(Item *self, Item * item)
{
	g_return_val_if_fail(item, NULL);

	if(! self) {
		return NULL;
	}

	if(self == item) {
		self = item->next;
		if(! self) {
			self = item->prev;
		}
	}

	if(item->prev) {
		item->prev->next = item->next;
	}

	if(item->next) {
		item->next->prev = item->prev;
	}

	item->prev = NULL;
	item->next = NULL;

	return self;
}

ItemClass item_class = {
		.parent = {
			.name			= "Item",
			.parent			= &base_class,
			.size			= sizeof(Item),
			.init			= item_init,
		},
		.draw				= NULL,
};
