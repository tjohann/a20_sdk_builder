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
 * fix paths -> see a20_sdk.git
 */
//const char *url = "https://github.com/tjohann/a20_sdk.git";
//const char *path = "/var/lib/a20_sdk";


static void
show_gtk_version_info()
{
	g_print("Glib version: %d.%d.%d\n",
		glib_major_version,
		glib_minor_version,
		glib_micro_version);

	g_print("GTK+ version: %d.%d.%d\n",
		gtk_major_version,
		gtk_minor_version,
		gtk_micro_version);
}


void
exit_function(GtkWidget *widget, gpointer data)
{
	/*
	  For autosave ...

	  Quit-Button and Quit-Function do autosave
	  "X" of the wm-window will send "delete-event" which will be handeld via
	  dialog box (see on_delete_event@gui.c)
	 */

	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


int
main(int argc, char **argv)
{
	int status = EXIT_SUCCESS;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	/*
	 * init non-gtk stuff
	 */
	g_print(_("Package name is %s\n"), PACKAGE_STRING);
	show_version_info();

	// for all git handling
	git_libgit2_init();

	// for all download handling
	curl_global_init(CURL_GLOBAL_ALL);

	// init main config struct
	init_main_config();

	if (init_network() != -1)
		g_print(_("Init network code: done\n"));

	/*
	 * init gtk stuff
	 */
	show_gtk_version_info();

	if (!g_thread_supported())
		g_thread_init(NULL);

	gdk_threads_init();
	gdk_threads_enter();

	gtk_init(&argc, &argv);
	build_main_window();

	/*
	 * check for some defaults to control the gui
	 */
	check_sdk_git_path();
	check_sdk_workdir();
	check_toolchain();
	check_test_env();

	gtk_main();
	gdk_threads_leave();

	git_libgit2_shutdown();

	return status;
}
