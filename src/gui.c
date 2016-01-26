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

#include "a20_sdk_builder.h"

GtkWidget *menubar;
GtkWidget *vbox;
GtkWidget *hbox;
GtkWidget *buttonbox;
GtkWidget *open_b;
GtkWidget *update_b;
GtkWidget *test_b;
GtkWidget *exit_b;

GdkPixbuf *icon;
GtkTooltips *tooltips;

GtkWidget *filemenu;
GtkWidget *file;
GtkWidget *sep;
GtkWidget *quit_m;
GtkWidget *sep_m;
GtkWidget *new_m;
GtkWidget *open_m;

GtkWidget *progressbar_window;
GtkWidget *progressbar_vbox;
GtkWidget *progressbar_sep;
GtkWidget *progressbar_frame;
GtkWidget *progressbar_frame2;
GtkAdjustment *progressbar_adj;

GtkAccelGroup *accel_group = NULL;


/*
  usage: 
  icon = create_pixbuf("a20_sdk_icon.svg");  
  gtk_window_set_icon(GTK_WINDOW(window), icon);

  update icon cache:
  gtk-update-icon-cache -f -t  ~/share/icons/hicolor/scalable/ 
*/
GdkPixbuf *
create_pixbuf(const gchar *filename)
{
    
	GdkPixbuf *pixbuf;
	GError *error = NULL;
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
	
	if (!pixbuf) {
		
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}
	
	return pixbuf;
}


static void
destroy_progressbar_window()
{
	gtk_widget_destroy(GTK_WIDGET(progressbar_window));
	progressbar = NULL;
}


static int
create_progress_bar_window(unsigned char progressbar_type)
{
	PRINT_LOCATION();

	if (progressbar != NULL) {
		fprintf(stderr, _("ERROR: Progressbar != NULL\n"));
		destroy_progressbar_window();
	}

	progressbar_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(progressbar_window), 500, 1);
	g_signal_connect(progressbar_window,
			 "destroy",
			 G_CALLBACK(destroy_progressbar_window),
			 NULL);

	switch (progressbar_type) {
	case CLONE_BAR:
		gtk_window_set_title(GTK_WINDOW(progressbar_window), _("Clone progress"));
		g_print(_("Progressbar_type == CLONE_BAR\n"));
		break;
	case DOWNLOAD_BAR:
		gtk_window_set_title(GTK_WINDOW(progressbar_window), _("Download progress"));
		g_print(_("Progressbar_type == DOWNLOAD_BAR\n"));
		break;
	default:
		return -1;
	}

	progressbar_vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(progressbar_vbox), 5);
	gtk_container_add(GTK_CONTAINER(progressbar_window), progressbar_vbox);

	progressbar_adj = (GtkAdjustment*)gtk_adjustment_new(0, 0, 100, 0, 0, 0);
	progressbar = gtk_progress_bar_new_with_adjustment(progressbar_adj);
	gtk_box_pack_start(GTK_BOX(progressbar_vbox), progressbar, FALSE, FALSE, 0);

	progressbar_sep = gtk_hseparator_new ();
	gtk_box_pack_start(GTK_BOX(progressbar_vbox), progressbar_sep, FALSE, FALSE, 0);

	progressbar_button = gtk_button_new_with_label(_("Close"));
	g_signal_connect(progressbar_button,
			 "clicked",
			 G_CALLBACK(destroy_progressbar_window),
			 NULL);
	gtk_tooltips_set_tip(tooltips,
			     progressbar_button,
			     _("Close window -> thread still active!"),
			     NULL);
	LOCK_PROGRESS_CLONE_BUTTON();
	gtk_box_pack_start(GTK_BOX(progressbar_vbox), progressbar_button, FALSE, FALSE, 0);

	gtk_widget_show_all(progressbar_window);

	gtk_progress_set_value(GTK_PROGRESS(progressbar), 0);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), "0%");

	return 0;
}


static void
clone_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (create_progress_bar_window(CLONE_BAR) != 0)
		fprintf(stderr, _("ERROR: create_progress_bar_window != 0\n"));

	if (!g_thread_create(&clone_sdk_repo, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}

static void
update_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	update_sdk_repo();
}


void
download_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	download_toolchain();
}


void
test_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


/*
 * button and menu
 */
void
new_config(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


void
open_menu(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


void
save_menu(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


void
save_as_menu(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static gboolean
on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	/* 
	   - return FALSE -> "destroy" signal will be sent
	   - return TRUE  -> NO "destroy" signal will be sent 
	*/

	GtkWidget *dialog;

	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
					GTK_DIALOG_DESTROY_WITH_PARENT,
					GTK_MESSAGE_QUESTION,
					GTK_BUTTONS_YES_NO,
					_("Are you sure to quit (without saving)?"));
	gtk_window_set_title(GTK_WINDOW(dialog), "Question");

	gint result = gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);

	if (result == GTK_RESPONSE_YES) 
		return FALSE;
	else
		return TRUE;
}



static void
build_button_box()
{
	open_b = gtk_button_new_with_label(_("Open Default"));
	g_signal_connect(open_b, "clicked", G_CALLBACK(new_config), NULL);
	gtk_tooltips_set_tip(tooltips,
			     open_b,
			     _("Load default configuration for this tool (a20_sdk_build.conf)"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), open_b);

	clone_b = gtk_button_new_with_label(_("Clone SDK"));
	g_signal_connect(clone_b, "clicked", G_CALLBACK(clone_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     clone_b,
			     _("Clone a20_sdk.git (https://github.com/tjohann/a20_sdk_builder.git)"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), clone_b);

	update_b = gtk_button_new_with_label(_("Update Repo"));
	g_signal_connect(update_b, "clicked", G_CALLBACK(update_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     update_b,
			     _("Update a20_sdk.git (https://github.com/tjohann/a20_sdk_builder.git)"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), update_b);
	
	download_b = gtk_button_new_with_label(_("Download SDK"));
	g_signal_connect(download_b, "clicked", G_CALLBACK(download_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     download_b,
			     _("Download toolchain binarys (http://sourceforge.net/projects/baalue-sdk/)"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), download_b);
	
	test_b = gtk_button_new_with_label(_("Test SDK"));
	g_signal_connect(test_b, "clicked", G_CALLBACK(test_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     test_b,
			     _("Run basic test/check on the sdk/toolchain"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), test_b);
	
	exit_b = gtk_button_new_with_label(_("Quit"));
	g_signal_connect(exit_b, "clicked", G_CALLBACK(exit_function), NULL);
	g_signal_connect_swapped(exit_b,
				 "clicked",
				 G_CALLBACK(gtk_widget_destroy),
				 window);
	gtk_tooltips_set_tip(tooltips,
			     exit_b,
			     _("Quit/Exit with autosave of toolconfig (a20_sdk_build.conf)"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), exit_b);
}


static void
build_menu_bar()
{
	filemenu = gtk_menu_new();

	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);


	// ---------------------- build FILE-MENU ------------------------------
	file = gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);

	// NEW menuentry
	new_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
	gtk_tooltips_set_tip(tooltips,
			     new_m,
			     _("Create default toolconfig (a20_sdk_build.conf)"),
			     NULL);
	g_signal_connect(new_m, "activate", G_CALLBACK(new_config), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new_m);

	// OPEN menuentry
	open_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
	gtk_tooltips_set_tip(tooltips,
			     open_m,
			     _("Open new toolconfig (a20_sdk_build.conf)"),
			     NULL);
	g_signal_connect(open_m, "activate", G_CALLBACK(open_menu), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open_m);


	// SAVE menuentry
	save_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
	gtk_tooltips_set_tip(tooltips,
			     save_m,
			     _("Save toolconfig (a20_sdk_build.conf)"),
			     NULL);
	g_signal_connect(save_m, "activate", G_CALLBACK(save_menu), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_m);

	// SAVE_AS menuentry
	save_as_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
	gtk_tooltips_set_tip(tooltips,
			     save_as_m,
			     _("Save toolconfig under a new name (a20_sdk_build.conf)"),
			     NULL);
	g_signal_connect(save_as_m, "activate", G_CALLBACK(save_as_menu), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_as_m);

	// SEPERATOR
	sep_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep_m);

	// QUIT menuentry
	quit_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);
	gtk_tooltips_set_tip(tooltips,
			     quit_m,
			     _("Quit/Exit with autosave of toolconfig (a20_sdk_build.conf)"),
			     NULL);
	g_signal_connect(quit_m, "activate", G_CALLBACK(exit_function), NULL);
	g_signal_connect_swapped(quit_m,
				 "activate",
				 G_CALLBACK(gtk_widget_destroy),
				 window);
	gtk_widget_add_accelerator(quit_m,
				   "activate",
				   accel_group,
				   GDK_q,
				   GDK_CONTROL_MASK,
				   GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit_m);

	// finish FILE-MENU
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

	// ---------------------------------------------------------------------
	
}


void
build_main_window()
{
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	gtk_window_set_title(GTK_WINDOW(window), "A20-SDK-Builder");
	gtk_window_set_default_size(GTK_WINDOW(window), 700, 1);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_name(GTK_WINDOW (window), "a20_sdk_builder");
	//GTK_WINDOW(window)->allow_shrink = TRUE;
	
	/*
	 * delete-event -> event coming from the window manager
	 */
	g_signal_connect(window,
			 "delete-event",
			 G_CALLBACK(on_delete_event),
			 NULL);
	g_signal_connect(window,
			 "destroy",
			 G_CALLBACK(gtk_main_quit),
			 NULL);

        /*   
	   Layout: 
	      +--------- menu --------+   <- menu 
	      + -------- vbox --------+
	      | buttonbox | texfield  |   <- hbox for buttonbox + texfield
	      |  ..... statusbar .... |   <- statusbar
	      +-----------------------+
	*/

        // ------------------ tooltips --------------------
	tooltips = gtk_tooltips_new();

	
        // ------------------ vbox ------------------------
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);


        // ------------------ menubar --------------------
	menubar = gtk_menu_bar_new();
	build_menu_bar();
	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 1);

       
	// ------------------ hbox ------------------------
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), hbox);


	// ------------------ buttonbox -------------------
	buttonbox = gtk_vbutton_box_new();
	gtk_container_set_border_width(GTK_CONTAINER(buttonbox), 5);
	build_button_box();
	gtk_box_pack_start(GTK_BOX(hbox), buttonbox, FALSE, FALSE, 1);


	// ------------------ textfield -------------------
	textfield = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(textfield), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textfield), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textfield), GTK_WRAP_WORD);
	gtk_box_pack_end(GTK_BOX(hbox), textfield, TRUE, TRUE, 1);

	
	// ------------------ statusbar -------------------
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);
}
