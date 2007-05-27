#include <gtk/gtk.h>
#include "tomoe-gtk.h"

int
main (int argc, char *argv[])
{
	GtkWidget *win;

	g_setenv ("TOMOE_CONFIG_FILE", "./test-config.xml", FALSE);
	gtk_init (&argc, &argv);
	win = tomoe_window_new ();
	g_signal_connect (G_OBJECT (win), "response",
			  G_CALLBACK (gtk_main_quit), NULL);
	gtk_widget_show (win);
	gtk_main ();
	return 0;
}
