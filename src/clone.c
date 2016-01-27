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


static int
sideband_progress(const char *str, int len, void *payload)
{
	char sideband_info[255];

	memset(sideband_info, 0, sizeof(sideband_info));
	snprintf(sideband_info, 255, "Remote: %s len: %d\n", str, len);

	(void) payload; // not used

	fprintf(stdout, sideband_info);
	fprintf(stdout, "Remote: %s len: %d\n", str, len);
	write_to_textfield(sideband_info, NORMAL_MSG);

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
	gdk_threads_enter();
	PRINT_LOCATION();
	gdk_threads_leave();

	git_repository *repo = NULL;
	git_clone_options clone_opts = GIT_CLONE_OPTIONS_INIT;
	git_checkout_options checkout_opts = GIT_CHECKOUT_OPTIONS_INIT;

	const char *url = REPO_NAME;
	const char *path = SDK_GIT_PATH;

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

out:
	if (repo)
		git_repository_free(repo);

	UNLOCK_PROGRESS_CLONE_BUTTON();

	return NULL;
}
