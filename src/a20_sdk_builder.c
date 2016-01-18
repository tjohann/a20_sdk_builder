/*
  GPL                                                                        
  (c) 2016, thorsten.johannvorderbrueggen@t-online.de
  
  This program is free software; you can redistribute it and/or modify       
  it under the terms of the GNU General Public License as published by       
  the Free Software Foundation; either version 2 of the License, or          
  (at your option) any later version.                                        
  
  This program is distributed in the hope that it will be useful,            
  but WITHOUT ANY WARRANTY; without even the implied warranty of             
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/


#include <gtk/gtk.h>

#include "libservice.h"


static void
clone_button_clicked(GtkWidget *widget,
	       gpointer data)
{
	g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);

}

static void
download_button_clicked(GtkWidget *widget,
	       gpointer data)
{
	g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);

}

static void
exit_button_clicked(GtkWidget *widget,
	       gpointer data)
{
	g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);

}


static void
activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *exit_button;
	GtkWidget *clone_button;
	GtkWidget *download_button;
	GtkWidget *button_box;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "A20-SDK-Builder");
	//gtk_window_set_default_size(GTK_WINDOW(window), 600, 300);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);


	/*
	 * the button box@button
	 */
	button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_container_add(GTK_CONTAINER(window), button_box);

	/*
	 * clone a20_sdk git repo button
	 */
	clone_button = gtk_button_new_with_label(_("Clone A20_SDK"));
	g_signal_connect(clone_button, "clicked", G_CALLBACK(clone_button_clicked), NULL);
	// function pointer to func in libservice.a
	//g_signal_connect_swapped(clone_button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_container_add(GTK_CONTAINER(button_box), clone_button);

	/*
	 * download toolchain button
	 */
	download_button = gtk_button_new_with_label(_("Clone A20_SDK"));
	g_signal_connect(download_button, "clicked", G_CALLBACK(download_button_clicked), NULL);
	// function pointer to func in libservice.a
	//g_signal_connect_swapped(download_button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_container_add(GTK_CONTAINER(button_box), download_button);
	
	/*
	 * exit button
	 */
	exit_button = gtk_button_new_with_label(_("Leave (only for testing)"));
	g_signal_connect(exit_button, "clicked", G_CALLBACK(exit_button_clicked), NULL);
	g_signal_connect_swapped(exit_button, "clicked", G_CALLBACK(gtk_widget_destroy), window);
	gtk_container_add(GTK_CONTAINER(button_box), exit_button);

	gtk_widget_show_all(window);
}


int
main(int argc, char **argv)
{
	GtkApplication *app;
	int status;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	
	/*
	 * init non-gtk stuff
	 */
	g_print(_("Package name is %s\n"), PACKAGE_STRING);
	
	if (init_network() != -1)
		g_print(_("Init network code: done\n"));


	/*
	 * init gtk stuff
	 */
	app = gtk_application_new("none.none.a20_sdk_builder", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref (app);
	
	return status;
}
