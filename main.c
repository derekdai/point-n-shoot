#include <gtk/gtk.h>
#include "point-n-shoot.h"

gint main(gint argc, gchar *args[])
{
	gtk_init(&argc, &args);

	PointNShoot *pns = pns_new();
	pns_run(pns);

	return 0;
}
