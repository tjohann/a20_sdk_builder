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

#include "common.h"
#include "global.h"

/*
 * main window
 */
GtkWidget *window;
GtkWidget *vbox;
GtkWidget *hbox;
GtkWidget *menubar;
GtkWidget *statusbar;
GtkWidget *filemenu;

/*
 * buttonbox on the left of main window
 */
GtkWidget *buttonbox;
GtkWidget *open_b;
GtkWidget *test_b;
GtkWidget *exit_b;
GtkWidget *clone_b;
GtkWidget *help_b;
GtkWidget *update_b;
GtkWidget *download_b;

/*
 * filemenu
 */
GtkWidget *file;
GtkWidget *sep;
GtkWidget *quit_m;
GtkWidget *sep_m;
GtkWidget *new_m;
GtkWidget *open_m;
GtkWidget *save_m;
GtkWidget *save_as_m;

GtkWidget *sdk_menu;
GtkWidget *sdk;
GtkWidget *init_m;
GtkWidget *sep2_m;
GtkWidget *clone_m;
GtkWidget *update_m;
GtkWidget *download_m;

GtkAccelGroup *accel_group;

/*
 * clone/download progressbar
 */
// GtkWidget *progressbar;        <- common.h
// GtkWidget *progressbar_button; <- common.h
GtkWidget *progressbar_window;
GtkWidget *progressbar_vbox;
GtkWidget *progressbar_sep;
GtkWidget *progressbar_frame;
GtkWidget *progressbar_frame2;
GtkAdjustment *progressbar_adj;

/*
 * textfield
 */
GtkWidget *textfield;
GtkWidget *textfield_scrolled;
GtkWidget *textfield_entry;
GtkWidget *textfield_table;
GtkTextBuffer *textfield_buffer;

/*
 * the "rest"
 */
GdkPixbuf *icon;
GtkTooltips *tooltips;


/*
 * gui specific defines
 * --------------------
 */
#define LOCK_ELEMENT 0x00
#define UNLOCK_ELEMENT 0x01



void
show_gtk_version_info()
{
	fprintf(stdout, _("Show verion of used gui libs: \n"));
	fprintf(stdout, _("----------------------------- \n"));
	g_print("Glib version: %d.%d.%d\n",
		glib_major_version,
		glib_minor_version,
		glib_micro_version);

	g_print("GTK+ version: %d.%d.%d\n",
		gtk_major_version,
		gtk_minor_version,
		gtk_micro_version);
	fprintf(stdout, _("---------------------------- \n"));
}


/*
  usage:
  icon = create_pixbuf("sdk_icon.svg");
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


void
set_progressbar_value(int statusbar_percent, char *statusbar_percent_string)
{
	/*
	 * add for pulsing changes instead of continous growing
	 * -> 	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar));
	 */

	gdk_threads_enter();

	gtk_progress_set_value(GTK_PROGRESS(progressbar), statusbar_percent);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar),
				  statusbar_percent_string);

	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar));

	if (statusbar_percent == 100)
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar),
					      1.0);

	if (statusbar_percent == 0)
		gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progressbar),
					      0.0);
	gdk_threads_leave();
}


int
create_progress_bar_window(unsigned char progressbar_type)
{
	PRINT_LOCATION();

	/*
	   Layout:
	      + -------- vbox --------+   <- progressbar_vbox
	      |       progresbar      |   <- progressbar
	      |   ..... button .....  |   <- progressbar_button
	      +-----------------------+
	*/


	if (progressbar != NULL) {
		fprintf(stderr, _("ERROR: Progressbar != NULL\n"));
		write_to_textfield(_("Progressbar != NULL\n"), ERROR_MSG);
		return -1;
	}

	progressbar_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(progressbar_window,
			 "destroy",
			 G_CALLBACK(destroy_progressbar_window),
			 NULL);
	gtk_window_set_resizable(GTK_WINDOW(progressbar_window), FALSE);

	switch (progressbar_type) {
	case CLONE_BAR:
		gtk_window_set_title(GTK_WINDOW(progressbar_window), _("Clone progress"));
		g_print(_("Progressbar_type == CLONE_BAR\n"));
		break;
	case DOWNLOAD_BAR:
		gtk_window_set_title(GTK_WINDOW(progressbar_window), _("Download progress"));
		g_print(_("Progressbar_type == DOWNLOAD_BAR\n"));
		break;
	case UPDATE_BAR:
		gtk_window_set_title(GTK_WINDOW(progressbar_window), _("Update/Fetch progress"));
		g_print(_("Progressbar_type == UPDATE_BAR\n"));
		break;
	default:
		fprintf(stderr, _("ERROR: unknown progressbar_type\n"));
		write_to_textfield(_("Unknown progressbar_type != NULL\n"), ERROR_MSG);
		return -1;
	}

	progressbar_vbox = gtk_vbox_new(FALSE, 5);
	gtk_container_set_border_width(GTK_CONTAINER(progressbar_vbox), 5);
	gtk_container_add(GTK_CONTAINER(progressbar_window), progressbar_vbox);

	progressbar_adj = (GtkAdjustment*)gtk_adjustment_new(0, 0, 100, 0, 0, 0);
	progressbar = gtk_progress_bar_new_with_adjustment(progressbar_adj);
	gtk_box_pack_start(GTK_BOX(progressbar_vbox), progressbar, FALSE, FALSE, 0);
	// set size of child -> and there'fore for progressbar_window
	gtk_widget_set_size_request(progressbar, 500, 20);

	progressbar_sep = gtk_hseparator_new ();
	gtk_box_pack_start(GTK_BOX(progressbar_vbox), progressbar_sep, FALSE, FALSE, 0);

	progressbar_button = gtk_button_new_with_label(_("Close"));
	g_signal_connect(progressbar_button,
			 "clicked",
			 G_CALLBACK(destroy_progressbar_window),
			 NULL);
	gtk_widget_set_sensitive(progressbar_button, FALSE);
	gtk_box_pack_start(GTK_BOX(progressbar_vbox), progressbar_button, FALSE, FALSE, 0);

	gtk_widget_show_all(progressbar_window);

	gtk_progress_set_value(GTK_PROGRESS(progressbar), 0);
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), "0%");

	return 0;
}


void
enter_sdk_thread()
{
	gdk_threads_enter();
	lock_button(INIT_M);
	lock_button(CLONE_B);
	lock_button(CLONE_M);
	lock_button(UPDATE_B);
	lock_button(UPDATE_M);
	lock_button(DOWNLOAD_B);
	lock_button(DOWNLOAD_M);
	lock_button(TEST_B);
	gdk_threads_leave();
}


void
leave_sdk_thread()
{
	gdk_threads_enter();
	check_sdk_git_path();
	check_sdk_workdir();
	check_toolchain();
	check_test_env();
	gdk_threads_leave();
}


static void
init_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
clone_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&clone_sdk_repo, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}


static void
update_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&update_sdk_repo, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}


static void
download_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&download_toolchain, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}


static void
test_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&test_sdk, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}


/*
 * button and menu
 */
static void
show_help(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&help, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}


/*
 * button and menu
 */
static void
new_config(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&config_sdk, NULL, FALSE, NULL) != 0)
		fprintf(stderr, _("Can't create the thread"));
}


static void
open_menu(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
save_menu(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
save_as_menu(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


/*
 * button and menu
 */
static void
handle_gui_element(gui_element_t button, unsigned char what_to_do)
{
	PRINT_LOCATION();

	switch(button)
	{
	case CLONE_B:
	case CLONE_M:
		if (what_to_do == UNLOCK_ELEMENT) {
			gtk_widget_set_sensitive(clone_b, TRUE);
			gtk_widget_set_sensitive(clone_m, TRUE);
		} else {
			gtk_widget_set_sensitive(clone_b, FALSE);
			gtk_widget_set_sensitive(clone_m, FALSE);
		}
		break;
	case DOWNLOAD_B:
	case DOWNLOAD_M:
		if (what_to_do == UNLOCK_ELEMENT) {
			gtk_widget_set_sensitive(download_b, TRUE);
			gtk_widget_set_sensitive(download_m, TRUE);
		} else {
			gtk_widget_set_sensitive(download_b, FALSE);
			gtk_widget_set_sensitive(download_m, FALSE);
		}
		break;
	case UPDATE_B:
	case UPDATE_M:
		if (what_to_do == UNLOCK_ELEMENT) {
			gtk_widget_set_sensitive(update_b, TRUE);
			gtk_widget_set_sensitive(update_m, TRUE);
		} else {
			gtk_widget_set_sensitive(update_b, FALSE);
			gtk_widget_set_sensitive(update_m, FALSE);
		}
		break;
	case TEST_B:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(test_b, TRUE);
		else
			gtk_widget_set_sensitive(test_b, FALSE);
		break;
	case HELP_B:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(help_b, TRUE);
		else
			gtk_widget_set_sensitive(help_b, FALSE);
		break;
	case OPEN_B:
	case OPEN_M:
		if (what_to_do == UNLOCK_ELEMENT) {
			gtk_widget_set_sensitive(open_b, TRUE);
			gtk_widget_set_sensitive(new_m, TRUE);
		} else {
			gtk_widget_set_sensitive(open_b, FALSE);
			gtk_widget_set_sensitive(new_m, FALSE);
		}
		break;
	case INIT_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(init_m, TRUE);
		else
			gtk_widget_set_sensitive(init_m, FALSE);
		break;

	default:
		write_to_textfield(_("Unknown GUI element\n"), ERROR_MSG);
	}
}


/*
 * button and menu
 */
int
get_state_of_gui_element(gui_element_t button)
{
	PRINT_LOCATION();

	switch(button)
	{
	case CLONE_B:
		return gtk_widget_get_sensitive(clone_b);
		break;
	case CLONE_M:
		return gtk_widget_get_sensitive(clone_m);
		break;
	case DOWNLOAD_B:
		return gtk_widget_get_sensitive(download_b);
		break;
	case DOWNLOAD_M:
		return gtk_widget_get_sensitive(download_m);
		break;
	case UPDATE_B:
		return gtk_widget_get_sensitive(update_b);
		break;
	case UPDATE_M:
		return gtk_widget_get_sensitive(update_m);
		break;
	case TEST_B:
		return gtk_widget_get_sensitive(test_b);
		break;
	case HELP_B:
		return gtk_widget_get_sensitive(help_b);
		break;
	case OPEN_B:
		return gtk_widget_get_sensitive(open_b);
		break;
	default:
		write_to_textfield(_("Unknown GUI element\n"), ERROR_MSG);
	}

	return -1;
}


/*
 * button and menu
 */
void
lock_button(gui_element_t button)
{
	PRINT_LOCATION();

	handle_gui_element(button, LOCK_ELEMENT);
}


/*
 * button and menu
 */
void
unlock_button(gui_element_t button)
{
	PRINT_LOCATION();

	handle_gui_element(button, UNLOCK_ELEMENT);
}


static gboolean
on_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
        (void) event;
	(void) data;

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

	gint result = gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);

	if (result == GTK_RESPONSE_YES)
		return FALSE;
	else
		return TRUE;
}


static void
build_button_box()
{
	PRINT_LOCATION();

	gtk_container_set_border_width(GTK_CONTAINER(buttonbox), 5);

	open_b = gtk_button_new_with_label(_("Open Default"));
	g_signal_connect(open_b, "clicked", G_CALLBACK(new_config), NULL);
	gtk_tooltips_set_tip(tooltips,
			     open_b,
			     _("Load default configuration for this tool (*sdk_build.conf)"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), open_b);

	clone_b = gtk_button_new_with_label(_("Clone SDK"));
	g_signal_connect(clone_b, "clicked", G_CALLBACK(clone_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     clone_b,
			     _("Clone SDK repository"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), clone_b);

	update_b = gtk_button_new_with_label(_("Update Repo"));
	g_signal_connect(update_b, "clicked", G_CALLBACK(update_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     update_b,
			     _("Update SDK repository"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), update_b);

	download_b = gtk_button_new_with_label(_("Download toolchain"));
	g_signal_connect(download_b, "clicked", G_CALLBACK(download_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     download_b,
			     _("Download toolchain binarys"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), download_b);

	test_b = gtk_button_new_with_label(_("Test SDK"));
	g_signal_connect(test_b, "clicked", G_CALLBACK(test_button), NULL);
	gtk_tooltips_set_tip(tooltips,
			     test_b,
			     _("Run basic test/check on the sdk/toolchain"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), test_b);

	help_b = gtk_button_new_with_label(_("Help"));
	g_signal_connect(help_b, "clicked", G_CALLBACK(show_help), NULL);
	gtk_tooltips_set_tip(tooltips,
			     help_b,
			     _("Open help window"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), help_b);

	exit_b = gtk_button_new_with_label(_("Quit"));
	g_signal_connect(exit_b, "clicked", G_CALLBACK(exit_function), NULL);
	g_signal_connect_swapped(exit_b,
				 "clicked",
				 G_CALLBACK(gtk_widget_destroy),
				 window);
	gtk_tooltips_set_tip(tooltips,
			     exit_b,
			     _("Quit/Exit"),
			     NULL);
	gtk_container_add(GTK_CONTAINER(buttonbox), exit_b);
}


static void
build_menu_bar()
{
	PRINT_LOCATION();

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
			     _("Create default toolconfig"),
			     NULL);
	g_signal_connect(new_m, "activate", G_CALLBACK(new_config), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new_m);

	// OPEN menuentry
	open_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
	gtk_tooltips_set_tip(tooltips,
			     open_m,
			     _("Open new toolconfig "),
			     NULL);
	g_signal_connect(open_m, "activate", G_CALLBACK(open_menu), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open_m);


	// SAVE menuentry
	save_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
	gtk_tooltips_set_tip(tooltips,
			     save_m,
			     _("Save toolconfig "),
			     NULL);
	g_signal_connect(save_m, "activate", G_CALLBACK(save_menu), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), save_m);

	// SAVE_AS menuentry
	save_as_m = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, NULL);
	gtk_tooltips_set_tip(tooltips,
			     save_as_m,
			     _("Save toolconfig under a new name"),
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
			     _("Quit/Exit with autosave of toolconfig"),
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


	// ---------------------- build SDK-MENU ------------------------------
	sdk_menu = gtk_menu_new();

	sdk = gtk_menu_item_new_with_mnemonic("_SDK");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(sdk), sdk_menu);


	// CLONE menuentry
	init_m = gtk_menu_item_new_with_label(_("Init /opt/*_sdk"));
	gtk_tooltips_set_tip(tooltips,
			     init_m,
			     _("Init workdir of *_sdk -> /opt/*_sdk"),
			     NULL);
	g_signal_connect(init_m, "activate", G_CALLBACK(init_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sdk_menu), init_m);


	// SEPERATOR
	sep2_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(sdk_menu), sep2_m);

	// CLONE menuentry
	clone_m = gtk_menu_item_new_with_label(_("Clone SDK"));
	gtk_tooltips_set_tip(tooltips,
			     clone_m,
			     _("Clone SDK repository"),
			     NULL);
	g_signal_connect(clone_m, "activate", G_CALLBACK(clone_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sdk_menu), clone_m);

	// UPDATE menuentry
	update_m = gtk_menu_item_new_with_label(_("Update Repo"));
	gtk_tooltips_set_tip(tooltips,
			     update_m,
			     _("Update SDK repositor"),
			     NULL);
	g_signal_connect(update_m, "activate", G_CALLBACK(update_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sdk_menu), update_m);

	// DOWNLOAD menuentry
	download_m = gtk_menu_item_new_with_label(_("Download toolchain"));
	gtk_tooltips_set_tip(tooltips,
			     download_m,
			     _("Download toolchain binarys"),
			     NULL);
	g_signal_connect(download_m, "activate", G_CALLBACK(download_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sdk_menu), download_m);


	// finish SDK-MENU
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), sdk);
}


static void
setup_textfield_entry()
{
	PRINT_LOCATION();

	/*
	 * TODO:
	 * Read value of textfield_entry which should represent some command
	 * to control the sdk and more
	 *
	 * See https://developer.gnome.org/gtk2/stable/GtkEntry.html
	 */

	gtk_entry_set_editable(GTK_ENTRY(textfield_entry), FALSE);
}


static void
setup_textfield()
{
	PRINT_LOCATION();

	gtk_widget_set_size_request(textfield, 600, 240);

	gtk_text_view_set_editable(GTK_TEXT_VIEW(textfield), FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(textfield), FALSE);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textfield), GTK_WRAP_WORD);

	textfield_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textfield));

	gtk_text_buffer_create_tag(textfield_buffer,
				   "bold",
				   "weight",
				   PANGO_WEIGHT_BOLD,
				   NULL);
	gtk_text_buffer_create_tag(textfield_buffer,
				   "italic",
				   "style",
				   PANGO_STYLE_ITALIC,
				   NULL);
	gtk_text_buffer_create_tag(textfield_buffer,
				   "red",
				   "foreground",
				   "red",
				   NULL);
}


void
write_to_textfield(const char *message, message_types_t type)
{
	PRINT_LOCATION();

	GtkTextIter iter;

	/*
	  Some example on howto use the textfield:

	  gtk_text_buffer_insert_with_tags_by_name(textfield_buffer, &iter,
	                                           "Bold text\n", -1,
						   "bold", NULL);

          gtk_text_buffer_insert_with_tags_by_name(textfield_buffer, &iter,
	                                           "Bold italic text\n", -1,
						   "bold", "italic", NULL);

	  gtk_text_buffer_insert_with_tags_by_name(textfield_buffer, &iter,
                                                   "Red bold italic text\n", -1,
						   "bold", "italic" ,"red", NULL);
	*/

	switch(type) {
	case NORMAL_MSG:
		gdk_threads_enter();
		gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
		gtk_text_buffer_insert(textfield_buffer, &iter, message, -1);
		gdk_threads_leave();
		break;
	case WARNING_MSG:
		gdk_threads_enter();
		gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
		gtk_text_buffer_insert_with_tags_by_name(textfield_buffer,
							 &iter,
							 "--- WARNING ---\n", -1,
							 "bold", NULL);
		gtk_text_buffer_insert(textfield_buffer, &iter, message, -1);
		gdk_threads_leave();
		break;
	case ERROR_MSG:
		gdk_threads_enter();
		gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
		gtk_text_buffer_insert_with_tags_by_name(textfield_buffer,
							 &iter,
							 "--- ERROR ---\n", -1,
							 "red", "bold", NULL);
		gtk_text_buffer_insert(textfield_buffer, &iter, message, -1);
		gdk_threads_leave();
		break;
	case INFO_MSG:
		gdk_threads_enter();
		gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
		gtk_text_buffer_insert_with_tags_by_name(textfield_buffer,
							 &iter,
							 "--- INFO ---\n", -1,
							 "italic", NULL);
		gtk_text_buffer_insert(textfield_buffer, &iter, message, -1);
		gdk_threads_leave();
		break;
	case NONE:
		gdk_threads_enter();
		gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
		gtk_text_buffer_insert(textfield_buffer, &iter, message, -1);
		gdk_threads_leave();
		break;
	default:
		fprintf(stderr,_("ERROR: Message type not supported\n"));
	}
}

void
build_main_window()
{
	PRINT_LOCATION();

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	if (gui_name == NULL)
		gtk_window_set_title(GTK_WINDOW(window), "SDK-Builder");
	else
		gtk_window_set_title(GTK_WINDOW(window), gui_name);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_icon_name(GTK_WINDOW (window), "sdk_builder");

	/*
	  The size calculation is a little bit tricky

	  - If you set gtk_window_set_resizable(GTK_WINDOW(window), FALSE); then
	    the user cant resize window, but child widgets can!
	    -> gtk_widget_set_size_request(textfield, 600, 240);

	  - With GTK_WINDOW(window)->allow_shrink = TRUE; you can shrink window
	    to smaller size then the widgets include (like buttons)

	  => As a result i do not allow resize by user and set value for the
	     texfield -> the result looks nice
             In this case dont use gtk_window_set_default_size(GTK_WINDOW(window), 700, 1);
	*/
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

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
	      +-------------- menu -----------+   <- menu
	      |                               |
	      + ------------- vbox -----------+
	      |           |                   |
	      |           |                   |
	      | buttonbox |    textarea       |   <- hbox for buttonbox + area
	      |           |   (see below)     |
	      |           |                   |
	      +-----------+-------------------+
	      |      ..... statusbar ....     |   <- statusbar
	      +-------------------------------+
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
	build_button_box();
	gtk_box_pack_start(GTK_BOX(hbox), buttonbox, FALSE, FALSE, 1);


	// ------------------ textarea --------------------

        /*
	   Layout:
	      + -------- table --------+
	      |                      | |
	      |        texfield      | |
	      |                      | |
	      +----------------------+-+
	      |        textentry       |
	      +------------------------+
	*/

	textfield_table = gtk_table_new(2, 1, FALSE);
	textfield_scrolled = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(
		GTK_SCROLLED_WINDOW(textfield_scrolled),
		GTK_POLICY_NEVER,
		GTK_POLICY_AUTOMATIC);

	textfield = gtk_text_view_new();
	setup_textfield();

	// entry line below the textfield
	textfield_entry = gtk_entry_new();
	setup_textfield_entry();

	gtk_container_add(GTK_CONTAINER(textfield_scrolled), textfield);
	gtk_table_attach_defaults(GTK_TABLE(textfield_table), textfield_scrolled, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(textfield_table), textfield_entry, 0, 1, 1, 2);

	gtk_box_pack_end(GTK_BOX(hbox), textfield_table, TRUE, TRUE, 1);


	// ------------------ statusbar -------------------
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

	gtk_widget_show_all(window);
}
