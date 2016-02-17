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

/*
 * main window
 */
static GtkWidget *window;
static GtkWidget *vbox;
static GtkWidget *hbox;
static GtkWidget *menubar;
static GtkWidget *statusbar;
static GtkWidget *filemenu;

/*
 * buttonbox on the left of main window
 */
static GtkWidget *buttonbox;
static GtkWidget *open_b;
static GtkWidget *test_b;
static GtkWidget *exit_b;
static GtkWidget *clone_b;
static GtkWidget *help_b;
static GtkWidget *update_b;
static GtkWidget *download_b;

/*
 * filemenu
 */
static GtkWidget *file;
static GtkWidget *quit_m;
static GtkWidget *sep_m;
static GtkWidget *new_m;
static GtkWidget *open_m;
static GtkWidget *save_m;
static GtkWidget *save_as_m;

static GtkWidget *sdk_menu;
static GtkWidget *sdk;
static GtkWidget *init_m;
static GtkWidget *sep2_m;
static GtkWidget *clone_m;
static GtkWidget *update_m;
static GtkWidget *download_m;

static GtkWidget *sd_card_menu;
static GtkWidget *sdcard;
static GtkWidget *sep3_m;
static GtkWidget *partition_m;
static GtkWidget *kernel_m;
static GtkWidget *home_m;
static GtkWidget *root_m;
static GtkWidget *sep4_m;
static GtkWidget *uboot_m;
static GtkWidget *sep5_m;
static GtkWidget *do_all_m;

static GtkWidget *external_menu;
static GtkWidget *external;
static GtkWidget *clone_all_m;
static GtkWidget *sep6_m;
static GtkWidget *external1_m;
static GtkWidget *external2_m;
static GtkWidget *external3_m;
static GtkWidget *external4_m;
static GtkWidget *external5_m;
static GtkWidget *external6_m;
static GtkWidget *external7_m;
static GtkWidget *external8_m;
static GtkWidget *external9_m;
static GtkWidget *external10_m;
static GtkWidget *sep7_m;
static GtkWidget *clone_selected_m;

static GtkAccelGroup *accel_group;

/*
 * clone/download progressbar
 */
static GtkWidget *progressbar;
static GtkWidget *progressbar_button;
static GtkWidget *progressbar_window;
static GtkWidget *progressbar_vbox;
static GtkWidget *progressbar_sep;
static GtkAdjustment *progressbar_adj;

/*
 * textfield
 */
static GtkWidget *textfield;
static GtkWidget *textfield_scrolled;
static GtkWidget *textfield_entry;
static GtkWidget *textfield_table;
static GtkTextBuffer *textfield_buffer;

/*
 * the "rest"
 */
static GtkTooltips *tooltips;


/*
 * gui specific defines
 * --------------------
 */
#define LOCK_ELEMENT 0x00
#define UNLOCK_ELEMENT 0x01



void
show_gtk_version_info()
{
	info_msg(_("Show verion of used gui libs: "));
	info_msg(_("----------------------------- "));
	info_msg("Glib version: %d.%d.%d",
		glib_major_version,
		glib_minor_version,
		glib_micro_version);

	info_msg("GTK+ version: %d.%d.%d",
		gtk_major_version,
		gtk_minor_version,
		gtk_micro_version);
	info_msg(_("----------------------------- "));
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
		error_msg("%s", error->message);
		g_error_free(error);
	}

	return pixbuf;
}


static void
destroy_progressbar_window()
{
	if (progressbar_window != NULL)
		gtk_widget_destroy(GTK_WIDGET(progressbar_window));

	progressbar = NULL;
	progressbar_button = NULL;
}


void
set_progressbar_value(int statusbar_percent, char *statusbar_percent_string)
{
	/*
	 * add for pulsing changes instead of continous growing
	 * -> 	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar));
	 */

	if (progressbar != NULL) {
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
	} else {
		write_warning_msg(_("ERROR: progressbar == NULL -> progressbar_window destroyed?"));
	}
}


void
set_progressbar_window_title(char *title)
{
	if (progressbar_window != NULL)
		gtk_window_set_title(GTK_WINDOW(progressbar_window), title);
	else
		write_warning_msg(_("Possible ERROR: progressbar_window != NULL"));
}


int
create_progressbar_window(char *name)
{
	char *title = NULL;

	if (name == NULL)
		title = "Progress";
	else
		title = name;

	/*
	   Layout:
	      + -------- vbox --------+   <- progressbar_vbox
	      |       progresbar      |   <- progressbar
	      |   ..... button .....  |   <- progressbar_button
	      +-----------------------+
	*/


	if (progressbar != NULL)
		write_error_msg_return(_("Progressbar != NULL"));

	progressbar_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(progressbar_window,
			 "destroy",
			 G_CALLBACK(destroy_progressbar_window),
			 NULL);
	gtk_window_set_resizable(GTK_WINDOW(progressbar_window), FALSE);
	gtk_window_set_title(GTK_WINDOW(progressbar_window), title);

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
		write_error_msg(_("Can't create the thread"));
}


static void
update_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&update_sdk_repo, NULL, FALSE, NULL) != 0)
		write_error_msg(_("Can't create the thread"));
}


static void
download_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&download_toolchain, NULL, FALSE, NULL) != 0)
		write_error_msg(_("Can't create the thread"));
}


static void
test_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;

	if (!g_thread_create(&test_sdk, NULL, FALSE, NULL) != 0)
		write_error_msg(_("Can't create the thread"));
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
		write_error_msg(_("Can't create the thread"));
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
		write_error_msg(_("Can't create the thread"));
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


static void
partition_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
kernel_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
root_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
home_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
uboot_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
do_all_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
clone_all_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


static void
external_button(GtkWidget *widget, gpointer data)
{
	PRINT_LOCATION();

	(void) data;

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external1_m)))
		info_msg("external 1 selected");
	else
		info_msg("external 1 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external2_m)))
		info_msg("external 2 selected");
	else
		info_msg("external 2 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external3_m)))
		info_msg("external 3 selected");
	else
		info_msg("external 3 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external4_m)))
		info_msg("external 4 selected");
	else
		info_msg("external 4 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external5_m)))
		info_msg("external 5 selected");
	else
		info_msg("external 5 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external6_m)))
		info_msg("external 6 selected");
	else
		info_msg("external 6 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external7_m)))
		info_msg("external 7 selected");
	else
		info_msg("external 7 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external8_m)))
		info_msg("external 8 selected");
	else
		info_msg("external 8 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external9_m)))
		info_msg("external 9 selected");
	else
		info_msg("external 9 DE-selected");

	if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(external10_m)))
		info_msg("external 10 selected");
	else
		info_msg("external 10 DE-selected");
}


static void
clone_selected_button(GtkWidget *widget, gpointer data)
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
			gtk_widget_set_sensitive(open_m, TRUE);
		} else {
			gtk_widget_set_sensitive(open_b, FALSE);
			gtk_widget_set_sensitive(open_m, FALSE);
		}
		break;
	case INIT_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(init_m, TRUE);
		else
			gtk_widget_set_sensitive(init_m, FALSE);
		break;
	case NEW_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(new_m, TRUE);
		else
			gtk_widget_set_sensitive(new_m, FALSE);
		break;
	case SAVE_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(save_m, TRUE);
		else
			gtk_widget_set_sensitive(save_m, FALSE);
		break;
	case SAVE_AS_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(save_as_m, TRUE);
		else
			gtk_widget_set_sensitive(save_as_m, FALSE);
		break;
	case PARTITION_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(partition_m, TRUE);
		else
			gtk_widget_set_sensitive(partition_m, FALSE);
		break;
	case HOME_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(home_m, TRUE);
		else
			gtk_widget_set_sensitive(home_m, FALSE);
		break;
	case ROOT_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(root_m, TRUE);
		else
			gtk_widget_set_sensitive(root_m, FALSE);
		break;
	case KERNEL_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(kernel_m, TRUE);
		else
			gtk_widget_set_sensitive(kernel_m, FALSE);
		break;
	case UBOOT_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(uboot_m, TRUE);
		else
			gtk_widget_set_sensitive(uboot_m, FALSE);
		break;
	case DO_ALL_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(do_all_m, TRUE);
		else
			gtk_widget_set_sensitive(do_all_m, FALSE);
		break;
	case CLONE_ALL_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(clone_all_m, TRUE);
		else
			gtk_widget_set_sensitive(clone_all_m, FALSE);
		break;
	case PROGRESSBAR_B:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(progressbar_button, TRUE);
		else
			gtk_widget_set_sensitive(progressbar_button, FALSE);
		break;
	case EXTERNAL1_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external1_m, TRUE);
		else
			gtk_widget_set_sensitive(external1_m, FALSE);
		break;
	case EXTERNAL2_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external2_m, TRUE);
		else
			gtk_widget_set_sensitive(external2_m, FALSE);
		break;
	case EXTERNAL3_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external3_m, TRUE);
		else
			gtk_widget_set_sensitive(external3_m, FALSE);
		break;
	case EXTERNAL4_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external4_m, TRUE);
		else
			gtk_widget_set_sensitive(external4_m, FALSE);
		break;
	case EXTERNAL5_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external5_m, TRUE);
		else
			gtk_widget_set_sensitive(external5_m, FALSE);
		break;
	case EXTERNAL6_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external6_m, TRUE);
		else
			gtk_widget_set_sensitive(external6_m, FALSE);
		break;
	case EXTERNAL7_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external7_m, TRUE);
		else
			gtk_widget_set_sensitive(external7_m, FALSE);
		break;
	case EXTERNAL8_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external8_m, TRUE);
		else
			gtk_widget_set_sensitive(external8_m, FALSE);
		break;
	case EXTERNAL9_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external9_m, TRUE);
		else
			gtk_widget_set_sensitive(external9_m, FALSE);
		break;
	case EXTERNAL10_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(external10_m, TRUE);
		else
			gtk_widget_set_sensitive(external10_m, FALSE);
		break;
	case CLONE_SELECTED_M:
		if (what_to_do == UNLOCK_ELEMENT)
			gtk_widget_set_sensitive(clone_selected_m, TRUE);
		else
			gtk_widget_set_sensitive(clone_selected_m, FALSE);
		break;
	default:
		write_error_msg(_("Unknown GUI element"));
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
	case OPEN_M:
		return gtk_widget_get_sensitive(open_m);
		break;
	case INIT_M:
		return gtk_widget_get_sensitive(init_m);
		break;
	case NEW_M:
		return gtk_widget_get_sensitive(new_m);
		break;
	case SAVE_M:
		return gtk_widget_get_sensitive(save_m);
		break;
	case SAVE_AS_M:
		return gtk_widget_get_sensitive(save_as_m);
		break;
	case PARTITION_M:
		return gtk_widget_get_sensitive(partition_m);
		break;
	case HOME_M:
		return gtk_widget_get_sensitive(home_m);
		break;
	case KERNEL_M:
		return gtk_widget_get_sensitive(kernel_m);
		break;
	case ROOT_M:
		return gtk_widget_get_sensitive(root_m);
		break;
	case UBOOT_M:
		return gtk_widget_get_sensitive(uboot_m);
		break;
	case DO_ALL_M:
		return gtk_widget_get_sensitive(do_all_m);
		break;
	case CLONE_ALL_M:
		return gtk_widget_get_sensitive(clone_all_m);
		break;
	case PROGRESSBAR_B:
		if (progressbar_button != NULL)
			return gtk_widget_get_sensitive(progressbar_button);
		else
			return false;
		break;
	case EXTERNAL1_M:
		return gtk_widget_get_sensitive(external1_m);
		break;
	case EXTERNAL2_M:
		return gtk_widget_get_sensitive(external2_m);
		break;
	case EXTERNAL3_M:
		return gtk_widget_get_sensitive(external3_m);
		break;
	case EXTERNAL4_M:
		return gtk_widget_get_sensitive(external4_m);
		break;
	case EXTERNAL5_M:
		return gtk_widget_get_sensitive(external5_m);
		break;
	case EXTERNAL6_M:
		return gtk_widget_get_sensitive(external6_m);
		break;
	case EXTERNAL7_M:
		return gtk_widget_get_sensitive(external7_m);
		break;
	case EXTERNAL8_M:
		return gtk_widget_get_sensitive(external8_m);
		break;
	case EXTERNAL9_M:
		return gtk_widget_get_sensitive(external9_m);
		break;
	case EXTERNAL10_M:
		return gtk_widget_get_sensitive(external10_m);
		break;
	case CLONE_SELECTED_M:
		return gtk_widget_get_sensitive(clone_selected_m);
		break;
	default:
		write_error_msg(_("Unknown GUI element"));
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


/*
 * button and menu
 */
void
lock_unused_buttons()
{
	lock_button(OPEN_M);
	lock_button(OPEN_B);
	lock_button(NEW_M);
	lock_button(SAVE_M);
	lock_button(SAVE_AS_M);

	lock_button(PARTITION_M);
	lock_button(ROOT_M);
	lock_button(HOME_M);
	lock_button(KERNEL_M);
	lock_button(UBOOT_M);
	lock_button(DO_ALL_M);

	lock_button(CLONE_ALL_M);
	lock_button(EXTERNAL1_M);
	lock_button(EXTERNAL2_M);
	lock_button(EXTERNAL3_M);
	lock_button(EXTERNAL4_M);
	lock_button(EXTERNAL5_M);
	lock_button(EXTERNAL6_M);
	lock_button(EXTERNAL7_M);
	lock_button(EXTERNAL8_M);
	lock_button(EXTERNAL9_M);
	lock_button(EXTERNAL10_M);
	lock_button(CLONE_SELECTED_M);
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
build_sdcard_menubar_entry()
{
        // Partition menuentry
	partition_m = gtk_menu_item_new_with_label(_("Partition SD-Card"));
	gtk_tooltips_set_tip(tooltips,
			     partition_m,
			     _("Partition a new SD-Card for target devices"),
			     NULL);
	g_signal_connect(partition_m, "activate", G_CALLBACK(partition_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), partition_m);

	// SEPERATOR
	sep3_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), sep3_m);

	// Kernel menuentry
	kernel_m = gtk_menu_item_new_with_label(_("Install Kernel-FS"));
	gtk_tooltips_set_tip(tooltips,
			     kernel_m,
			     _("Install kernel image to SD-Card"),
			     NULL);
	g_signal_connect(kernel_m, "activate", G_CALLBACK(kernel_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), kernel_m);

	// Root menuentry
	root_m = gtk_menu_item_new_with_label(_("Install Root-FS"));
	gtk_tooltips_set_tip(tooltips,
			     root_m,
			     _("Install rootfs image to SD-Card"),
			     NULL);
	g_signal_connect(root_m, "activate", G_CALLBACK(root_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), root_m);

	// home menuentry
	home_m = gtk_menu_item_new_with_label(_("Install Home-FS"));
	gtk_tooltips_set_tip(tooltips,
			     home_m,
			     _("Install home image to SD-Card"),
			     NULL);
	g_signal_connect(home_m, "activate", G_CALLBACK(home_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), home_m);

	// SEPERATOR
	sep4_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), sep4_m);

	// U-Boot menuentry
	uboot_m = gtk_menu_item_new_with_label(_("Install U-Boot"));
	gtk_tooltips_set_tip(tooltips,
			     uboot_m,
			     _("Install U-Boot to SD-Card"),
			     NULL);
	g_signal_connect(uboot_m, "activate", G_CALLBACK(uboot_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), uboot_m);

	// SEPERATOR
	sep5_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), sep5_m);

        // Do-All menuentry
	do_all_m = gtk_menu_item_new_with_label(_("Do ALL"));
	gtk_tooltips_set_tip(tooltips,
			     do_all_m,
			     _("Do all in one step"),
			     NULL);
	g_signal_connect(do_all_m, "activate", G_CALLBACK(do_all_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(sd_card_menu), do_all_m);
}


static void
build_external_menubar_entry()
{
	// clone_all menuentry
	clone_all_m = gtk_menu_item_new_with_label(_("Clone ALL"));
	gtk_tooltips_set_tip(tooltips,
			     clone_all_m,
			     _("Clone all external repositorys"),
			     NULL);
	g_signal_connect(clone_all_m, "activate", G_CALLBACK(clone_all_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), clone_all_m);

	// SEPERATOR
	sep6_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), sep6_m);

	// external 1 menuentry
	external1_m = gtk_check_menu_item_new_with_label("External 1");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external1_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external1_m,
			     _("Clone externel 1 repository"),
			     NULL);
	g_signal_connect(external1_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external1_m);

	// external 2 menuentry
	external2_m = gtk_check_menu_item_new_with_label("External 2");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external2_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external2_m,
			     _("Clone externel 2 repository"),
			     NULL);
	g_signal_connect(external2_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external2_m);

	// external 3 menuentry
	external3_m = gtk_check_menu_item_new_with_label("External 3");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external3_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external3_m,
			     _("Clone externel 3 repository"),
			     NULL);
	g_signal_connect(external3_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external3_m);

	// external 4 menuentry
	external4_m = gtk_check_menu_item_new_with_label("External 4");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external4_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external4_m,
			     _("Clone externel 4 repository"),
			     NULL);
	g_signal_connect(external4_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external4_m);

	// external 5 menuentry
	external5_m = gtk_check_menu_item_new_with_label("External 5");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external5_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external5_m,
			     _("Clone externel 5 repository"),
			     NULL);
	g_signal_connect(external5_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external5_m);

	// external 6 menuentry
	external6_m = gtk_check_menu_item_new_with_label("External 6");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external6_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external6_m,
			     _("Clone externel 6 repository"),
			     NULL);
	g_signal_connect(external6_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external6_m);

	// external 7 menuentry
	external7_m = gtk_check_menu_item_new_with_label("External 7");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external7_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external7_m,
			     _("Clone externel 7 repository"),
			     NULL);
	g_signal_connect(external7_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external7_m);

	// external 8 menuentry
	external8_m = gtk_check_menu_item_new_with_label("External 8");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external8_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external8_m,
			     _("Clone externel 8 repository"),
			     NULL);
	g_signal_connect(external8_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external8_m);

	// external 9 menuentry
	external9_m = gtk_check_menu_item_new_with_label("External 9");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external9_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external9_m,
			     _("Clone externel 9 repository"),
			     NULL);
	g_signal_connect(external9_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external9_m);

	// external 10 menuentry
	external10_m = gtk_check_menu_item_new_with_label("External 10");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(external10_m), TRUE);
	gtk_tooltips_set_tip(tooltips,
			     external1_m,
			     _("Clone externel 1o repository"),
			     NULL);
	g_signal_connect(external10_m, "activate", G_CALLBACK(external_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), external10_m);

	// SEPERATOR
	sep7_m = gtk_separator_menu_item_new();
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), sep7_m);

	// clone_selected menuentry
	clone_selected_m = gtk_menu_item_new_with_label(_("Clone selected"));
	gtk_tooltips_set_tip(tooltips,
			     clone_selected_m,
			     _("Clone selected external repositorys"),
			     NULL);
	g_signal_connect(clone_selected_m, "activate", G_CALLBACK(clone_selected_button), NULL);
	gtk_menu_shell_append(GTK_MENU_SHELL(external_menu), clone_selected_m);
}


static void
build_sdk_menubar_entry()
{
	// CLONE menuentry
	init_m = gtk_menu_item_new_with_label(_("Init /opt/*_sdk"));
	gtk_tooltips_set_tip(tooltips,
			     init_m,
			     _("Init workdir -> /opt/*_sdk"),
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
}


static void
build_file_menubar_entry()
{
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
}


static void
build_menu_bar()
{
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	// ---------------------- build FILE-MENU ------------------------------
	filemenu = gtk_menu_new();
	file = gtk_menu_item_new_with_mnemonic("_File");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
	build_file_menubar_entry();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

	// ---------------------- build SDK-MENU ------------------------------
	sdk_menu = gtk_menu_new();
	sdk = gtk_menu_item_new_with_mnemonic("_SDK");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(sdk), sdk_menu);
	build_sdk_menubar_entry();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), sdk);

	// ---------------------- build SD-Card-MENU ---------------------------
	sd_card_menu = gtk_menu_new();
	sdcard = gtk_menu_item_new_with_mnemonic("SD-_Card");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(sdcard), sd_card_menu);
	build_sdcard_menubar_entry();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), sdcard);

	// ---------------------- build external-MENU ---------------------------
	external_menu = gtk_menu_new();
	external = gtk_menu_item_new_with_mnemonic("_External");
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(external), external_menu);
	build_external_menubar_entry();
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), external);

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

	gtk_text_buffer_create_tag(textfield_buffer, "bold", "weight",
				   PANGO_WEIGHT_BOLD, NULL);
	gtk_text_buffer_create_tag(textfield_buffer, "italic", "style",
				   PANGO_STYLE_ITALIC, NULL);
	gtk_text_buffer_create_tag(textfield_buffer, "red", "foreground",
				   "red",  NULL);
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

	if (textfield_buffer != NULL) {
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
								 "--- WARNING ---\n",
								 -1, "bold",
								 NULL);
			gtk_text_buffer_insert(textfield_buffer, &iter,
					       message, -1);
			gdk_threads_leave();
			break;
		case ERROR_MSG:
			gdk_threads_enter();
			gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
			gtk_text_buffer_insert_with_tags_by_name(textfield_buffer,
								 &iter,
								 "--- ERROR ---\n",
								 -1, "red", "bold",
								 NULL);
			gtk_text_buffer_insert(textfield_buffer, &iter, message, -1);
			gdk_threads_leave();
			break;
		case INFO_MSG:
			gdk_threads_enter();
			gtk_text_buffer_get_end_iter(textfield_buffer, &iter);
			gtk_text_buffer_insert_with_tags_by_name(textfield_buffer,
								 &iter,
								 "--- INFO ---\n",
								 -1, "italic",
								 NULL);
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
			error_msg(_("ERROR: Message type not supported"));
		}
	} else {
		error_msg(_("ERROR: textfield_buffer == NULL!"));
	}
}

void
build_main_window()
{
	PRINT_LOCATION();

	char *gui_name = get_common_gui_name();

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
	g_signal_connect(window, "delete-event",
			 G_CALLBACK(on_delete_event), NULL);
	g_signal_connect(window, "destroy",
			 G_CALLBACK(gtk_main_quit), NULL);

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
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(textfield_scrolled),
		GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

	textfield = gtk_text_view_new();
	setup_textfield();

	// entry line below the textfield
	textfield_entry = gtk_entry_new();
	setup_textfield_entry();

	gtk_container_add(GTK_CONTAINER(textfield_scrolled), textfield);
	gtk_table_attach_defaults(GTK_TABLE(textfield_table),
				  textfield_scrolled, 0, 1, 0, 1);
	gtk_table_attach_defaults(GTK_TABLE(textfield_table),
				  textfield_entry, 0, 1, 1, 2);

	gtk_box_pack_end(GTK_BOX(hbox), textfield_table, TRUE, TRUE, 1);

	// ------------------ statusbar -------------------
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(vbox), statusbar, FALSE, TRUE, 1);

	gtk_widget_show_all(window);
}
