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


// all used widgets
GtkWidget *window;
GtkWidget *vbox;
GtkWidget *statusbar;
GtkWidget *textfield;



static void
clone_button_clicked(GtkWidget *widget, gpointer data)
{
	g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);

}

static void
download_button_clicked(GtkWidget *widget, gpointer data)
{
	g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);

}

static void
exit_button_clicked(GtkWidget *widget, gpointer data)
{
	g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);

}


/*
 * build the gui ...
 */
static void
build_main_window()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title(GTK_WINDOW(window), "A20-SDK-Builder");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 300);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	//gtk_window_set_icon(GTK_WINDOW(window),create_pixbuf("a20_sdk_icon.png"));
	GTK_WINDOW(window)->allow_shrink = TRUE;

        // create vbox and add it to main window
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);


	// ------------------ hbox ------------------------
	

	// ------------------ textfield -------------------
	textfield = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textfield), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textfield), FALSE);

	gtk_box_pack_start(GTK_BOX(vbox), textfield, TRUE, TRUE, 3);
	
	
	// ------------------ statusbar -------------------
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

	
	// -------------- connect signals -----------------
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
				 G_CALLBACK(gtk_main_quit),
				 G_OBJECT(window));
}


int
main(int argc, char **argv)
{
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
	if (!g_thread_supported())
		g_thread_init(NULL);

	gdk_threads_init();
	gdk_threads_enter();

	gtk_init(&argc, &argv);
	
	build_main_window();
	
	gtk_widget_show_all(window);  
	
	gtk_main();
	gdk_threads_leave();
	
	return status;
}
