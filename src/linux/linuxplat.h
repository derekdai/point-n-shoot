#ifndef __LINUX_PLAT_H_
#define __LINUX_PLAT_H_

#include "platform.h"

G_BEGIN_DECLS

#define LINUX_PLAT(o)				((LinuxPlat *)(o))

typedef struct _LinuxPlat LinuxPlat;

struct _LinuxPlat
{
	Platform parent;
};

extern PlatformClass linux_plat_class;

G_END_DECLS

#endif /* __LINUX_PLAT_H_ */
