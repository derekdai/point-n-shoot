#include <gtk/gtk.h>
#include "point-n-shoot.h"

gint main(gint argc, gchar *args[])
{
	gtk_init(&argc, &args);

	PointNShoot *pns = pns_get_default();
	pns_run(pns);
	pns_destroy();

	return 0;
}
