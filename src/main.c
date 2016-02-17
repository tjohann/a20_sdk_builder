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

static char *program_name;


static void
__attribute__((noreturn)) usage(int status)
{
	putchar('\n');
	fprintf(stdout, _("Usage: %s [options]              \n"), program_name);
	fprintf(stdout, _("Options:                                       \n"));

	fprintf(stdout, _("        -f filename.conf   \
give a name for a configuration file [mandatory]      \n"));
	fprintf(stdout, _("        -d directory       \
give a directory name to search for configuration file\n"));
	fprintf(stdout, _("        -h                 \
show help                                             \n"));
	putchar('\n');
	fprintf(stdout, _("Examples:                                      \n"));
	fprintf(stdout, _("Normal usage (via desktop.entry):                   \
a20_sdk_builder \n"));
	fprintf(stdout, _("Give configuration as argument:                     \
sdk_builder -f a20_sdk_builder.conf                   \n"));
	fprintf(stdout, _("-> check path /etc/sdk_builder                 \n"));
	fprintf(stdout, _("-> check path /usr/local/etc/sdk_builder       \n"));
	fprintf(stdout, _("Give both as argument:                              \
sdk_builder -f a20_sdk_builder.conf -d $HOME/etc/sdk-builder/ \n"));
	putchar('\n');

	exit(status);
}


static void
cleanup(void)
{
	fprintf(stdout, _("Finalize cleanup -> cheers %s\n"), getenv("USER"));
}


static void
show_all_infos()
{
	/*
	 * show some useful info
	 */
	show_program_name(program_name);
	show_package_name();
	show_version_info();
	show_gtk_version_info();
	show_config();
}


static void
check_all_states()
{
	/*
	 * check for some defaults to control the gui
	 */
	if (check_sdk_runtimedir() != 0)
		info_msg("%s isn't available", get_common_runtimedir());

	if (check_sdk_workdir() != 0)
		info_msg("%s isn't available", get_common_workdir());

	check_sdk_git_path();
	check_toolchain();
	check_test_env();
}


static int
read_complete_config(char *conf_file, char *conf_dir)
{
	if (init_main_config(conf_file, conf_dir) != 0) {
		error_msg(_("ERROR main sdk_config"));
		return -1;
	}

	if (init_common_config(conf_file, conf_dir) != 0)
		error_msg(_("ERROR common sdk_config done"));


	if (init_repo_config(conf_file, conf_dir) != 0)
		error_msg(_("ERROR repo sdk_config"));


	if (init_toolchain_config(conf_file, conf_dir) != 0)
		error_msg(_("ERROR toolchain sdk_config"));


	if (init_device_config(conf_file, conf_dir) != 0)
		error_msg(_("ERROR device sdk_config"));


	if (init_external_config(conf_file, conf_dir) != 0)
		error_msg(_("ERROR external sdk_config"));


	if (init_kernel_config(conf_file, conf_dir) != 0)
		error_msg(_("ERROR kernel sdk_config"));

	return 0;
}


void
exit_function(GtkWidget *widget, gpointer data)
{
	/*
	  For autosave ...

	  Quit-Button and Quit-Function do autosave
	  via "X" of the wm-window will send "delete-event" which will be handeld
	  dialog box (see on_delete_event@gui.c)
	 */

	PRINT_LOCATION();

	(void) widget;
	(void) data;
}


int
main(int argc, char *argv[])
{
	char *conf_file = NULL;
	char *conf_dir = NULL;
	int c;

	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	set_program_name(&program_name, argv[0]);

	while ((c = getopt(argc, argv, "hf:d:")) != -1) {
		switch (c) {
		case 'f':
			conf_file = optarg;
			break;
		case 'd':
			conf_dir = optarg;
			break;
		case 'h':
			usage(EXIT_SUCCESS);
			break;
		default:
			fprintf(stderr, _("ERROR: no valid argument\n"));
			usage(EXIT_FAILURE);
		}
	}

	if (atexit(cleanup) != 0)
		exit(EXIT_FAILURE);

	if (conf_file == NULL)
		usage(EXIT_FAILURE);

        /*
	 * init non-gtk stuff
	 */
	git_libgit2_init();
	curl_global_init(CURL_GLOBAL_ALL);

	if (read_complete_config(conf_file, conf_dir) == 0) {
		info_msg(_("Init main sdk_config done"));
	} else {
		error_msg(_("Possible init problem: read_complete_config != 0"));
		usage(EXIT_FAILURE);
	}

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

	// lock unused buttons/menus -> not yet implemented
	lock_unused_buttons();

	/*
	 * check for some defaults to control the gui
	 */
	check_all_states();

	/*
	 * show some useful info
	 */
	show_all_infos();

	gtk_main();
	gdk_threads_leave();

	git_libgit2_shutdown();

	return EXIT_SUCCESS;
}
