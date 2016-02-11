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

static void
checkout_progress(const char *path, size_t cur, size_t tot, void *payload)
{
	int checkout_percent = (100 * cur) / tot;

	int statusbar_percent = (checkout_percent / 6 ) + 84;
	char statusbar_percent_string[5];

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

	(void) payload; // not used

	fprintf(stdout, _("Checkout: %3d%% (%d/%d) %s \n"),
		checkout_percent,
		(int) cur, (int) tot,
		path);

	set_progressbar_value(statusbar_percent, statusbar_percent_string);
}


static void
do_clone_repo(char *url, char *path)
{
	git_repository *repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

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
	if (error != 0)
		git_error_handling();

out:
	if (repo)
		git_repository_free(repo);
}



void *
clone_sdk_repo(void *args)
{
	char *url = get_sdk_repo_url();
	char *path = get_sdk_repo_path();

	(void) args; // not used

	enter_sdk_thread();

	if (create_progressbar_window(path) != 0)
		fprintf(stderr, _("ERROR: create_progress_bar_window != 0\n"));

	do_clone_repo(url, path);

	set_progressbar_value(100, "100%");
	unlock_button(PROGRESSBAR_B);

	leave_sdk_thread();

	return NULL;
}
