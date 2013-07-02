#ifndef __EVENT_H_
#define __EVENT_H_

#include <glib.h>

G_BEGIN_DECLS

typedef enum _GameKeys GameKeys;

enum _GameKeys
{
	GAME_KEYS_NONE		= 0,
	GAME_KEYS_UP		= 1 << 0,
	GAME_KEYS_DOWN		= 1 << 1,
	GAME_KEYS_LEFT		= 1 << 2,
	GAME_KEYS_RIGHT		= 1 << 3,
};

G_END_DECLS

#endif /* __EVENT_H_ */
