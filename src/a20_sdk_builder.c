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


/*
 * fix paths -> see a20_sdk.git
 */
const char *url = "https://github.com/tjohann/a20_sdk.git";
const char *path = "/var/lib/a20_sdk";


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

static int
sideband_progress(const char *str, int len, void *payload)
{
	(void) payload; // // not used

	fprintf(stdout, "Remote: %s  len: %d\n", str, len);
	return 0;
}

static int
fetch_progress(const git_transfer_progress *stats, void *payload)
{
	int fetch_percent = (100 * stats->received_objects)/stats->total_objects;
	//int index_percent = (100 * stats->indexed_objects) /stats->total_objects;
	int receive_kbyte = stats->received_bytes / 1024;

	int statusbar_percent = (fetch_percent * 2) / 3;
	char statusbar_percent_string[5];

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

	(void) payload; // not used

	fprintf(stdout,	"Fetched: %3d%% (%d/%d) %d kB \n",
		fetch_percent,
		stats->received_objects, stats->total_objects,
		receive_kbyte);

	/*
	 * I suppose that if the window is destroyed for whatever reason,
	 * the clone proccess should continue. Otherwise we leave a broken
	 * git repo.
	 */
	if (progressbar != NULL) {
		SET_PROGRESSBAR_VALUE();
	} else
		fprintf(stderr, _("progressbar == NULL -> progressbar_window destroyed?\n"));

	return 0;
}


static void
checkout_progress(const char *path, size_t cur, size_t tot, void *payload)
{
	int checkout_percent = (100 * cur) / tot;

	int statusbar_percent = (checkout_percent / 3 ) + 67;
	char statusbar_percent_string[5];

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

	(void) payload; // not used

	fprintf(stdout, "Checkout: %3d%% (%d/%d) %s \n",
		checkout_percent,
		(int) tot, (int) cur,
		path);

	/*
	 * I suppose that if the window is destroyed for whatever reason,
	 * the clone proccess should continue. Otherwise we leave a broken
	 * git repo.
	 */
	if (progressbar != NULL) {
		SET_PROGRESSBAR_VALUE();
	} else
		fprintf(stderr, _("progressbar == NULL -> progressbar_window destroyed?\n"));
}


void *
clone_sdk_repo(void *args)
{
	git_repository *repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

	(void) args; // not used

	checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE;
	checkout_opts.progress_cb = checkout_progress;
	clone_opts.checkout_opts = checkout_opts;
	clone_opts.fetch_opts.callbacks.sideband_progress = sideband_progress;
	clone_opts.fetch_opts.callbacks.transfer_progress = &fetch_progress;

	/*
	 * I suppose that the clone proccess should finish; so i wont kill
	 * the thread if the progessbar_window is destroyed.
	 */
	int error = git_clone(&repo, url, path, &clone_opts);
	if (error != 0) {
		const git_error *err = giterr_last();

		if (err)
			fprintf(stderr,
				"ERROR %d: %s\n",
				err->klass,
				err->message);
		else
			fprintf(stderr,
				"ERROR %d: no detailed info\n",
				error);

		SET_PROGRESSBAR_0();
		goto out;
	}

	UNLOCK_PROGRESS_CLONE_BUTTON();
out:
	if (repo)
		git_repository_free(repo);

	return NULL;
}


void
update_sdk_repo()
{
	PRINT_LOCATION();
}


void
download_toolchain()
{
	PRINT_LOCATION();
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
	
	if (init_network() != -1)
		g_print(_("Init network code: done\n"));



	/*
	 * init gtk stuff
	 */
	show_gtk_version_info();
	
	if (!g_thread_supported())
		g_thread_init(NULL);

	gdk_threads_init();
	gtk_init(&argc, &argv);

	build_main_window();
	gtk_widget_show_all(window);  

	gdk_threads_enter();
	gtk_main();
	gdk_threads_leave();

	git_libgit2_shutdown();

	return status;
}
