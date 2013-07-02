#ifndef __LINUX_JS_H_
#define __LINUX_JS_H_

#include "joystick.h"

G_BEGIN_DECLS

#define LINUX_JS(o)					((LinuxJs *)(o))

typedef struct _LinuxJs LinuxJs;

extern JoystickClass linux_js_class;

G_END_DECLS

#endif /* __LINUX_JS_H_ */
