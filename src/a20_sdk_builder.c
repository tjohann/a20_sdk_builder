/*
 * 
 */

#include <gtk/gtk.h>

#include "libservice.h"


static void
button_clicked(GtkWidget *widget,
	    gpointer   data)
{
	g_print(_("your're in %s of %s\n"), __FUNCTION__, __FILE__);
	printf(_("package name is %s\n"), PACKAGE_STRING);
}


static void
activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *button;
	GtkWidget *button_box;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "A20-SDK-Builder");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 300);
	
	button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add (GTK_CONTAINER(window), button_box);
	
	button = gtk_button_new_with_label("For testing");
	g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), NULL);
	g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_container_add(GTK_CONTAINER(button_box), button);
	
	gtk_widget_show_all(window);
}


int
main(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALDIR);
	textdomain(PACKAGE);
	
	/*
	 * init stuff
	 */
	if (init_network() != -1)
		fprintf(stdout, "successefully init the network\n");

	app = gtk_application_new("none.none.a20_sdk_builder", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);



	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref (app);
	
	return status;
}
