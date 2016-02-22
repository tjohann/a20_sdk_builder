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

#include "libservice.h"



static int
sideband_progress(const char *str, int len, void *payload)
{
	char textfield_update_string[MAXLINE];
	memset(textfield_update_string, 0, sizeof(textfield_update_string));

	(void) payload; // not used

	snprintf(textfield_update_string, sizeof(textfield_update_string),
		 _("Remote: %.*s"), len, str);

	write_info_msg(textfield_update_string);

	return 0;
}


static int
fetch_progress(const git_transfer_progress *stats, void *payload)
{
	int fetch_percent = (100 * stats->received_objects) /
		stats->total_objects;
	//int index_percent = (100 * stats->indexed_objects) /
	//      stats->total_objects;

	// simple weight function
	int statusbar_percent = (fetch_percent * 5) / 6;
	char statusbar_percent_string[5];

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

	(void) payload; // not used

#ifdef DEBUG
	int receive_kbyte = stats->received_bytes / 1024;
	if (stats->received_objects == stats->total_objects) {
		info_msg("Resolving deltas %d/%d",
			 stats->indexed_deltas, stats->total_deltas);
	} else if (stats->total_objects > 0) {
		info_msg("Fetched: %3d%% (%d/%d) %d kB",
			 fetch_percent,
			 stats->received_objects, stats->total_objects,
			 receive_kbyte);
	}
#endif

	set_progressbar_value(statusbar_percent, statusbar_percent_string);

	return 0;
}


static void
checkout_progress(const char *path, size_t cur, size_t tot, void *payload)
{
	int checkout_percent = (100 * cur) / tot;

	int statusbar_percent = (checkout_percent / 6 ) + 84;
	char statusbar_percent_string[5];

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

	(void) payload; // not used

#ifdef DEBUG
	info_msg(_("Checkout: %3d%% (%d/%d) %s"),
		checkout_percent,
		(int) cur, (int) tot,
		path);
#else
	(void) path;
#endif

	set_progressbar_value(statusbar_percent, statusbar_percent_string);
}


void
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

	int error = git_clone(&repo, url, path, &clone_opts);
	if (error != 0)
		git_error_handling();

out:
	if (repo)
		git_repository_free(repo);
}


static int
update_tips(const char *refname, const git_oid *first,
	    const git_oid *second, void *payload)
{
	char first_oid_s[GIT_OID_HEXSZ+1];
	char second_oid_s[GIT_OID_HEXSZ+1];

	char textfield_update_string[MAXLINE];
	memset(textfield_update_string, 0, sizeof(textfield_update_string));

	(void) payload; // not used

	memset(first_oid_s, 0, GIT_OID_HEXSZ+1);
	memset(second_oid_s, 0, GIT_OID_HEXSZ+1);

	git_oid_fmt(second_oid_s, second);
	second_oid_s[GIT_OID_HEXSZ] = '\0';

	/*
	  The first oid indicates if it's a new object or not

	  - first oid == 0
	    -> second oid is a new object
	  - first oid != 0
	    -> update of an old object
	 */

	if (git_oid_iszero(first)) {
		snprintf(textfield_update_string, sizeof(textfield_update_string),
			 _("[new] %.20s %s"), second_oid_s, refname);

		write_info_msg(textfield_update_string);
	} else {
		git_oid_fmt(first_oid_s, first);
		first_oid_s[GIT_OID_HEXSZ] = '\0';

		snprintf(textfield_update_string, sizeof(textfield_update_string),
			 _("[updated] %.10s..%.10s %s"),
			 first_oid_s, second_oid_s, refname);
		write_info_msg(textfield_update_string);
	}

	return 0;
}


void
do_update_repo(char *url, char *path)
{
	git_repository *repo;
	git_remote *remote = NULL;
	git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

	const git_transfer_progress *stats;

	char textfield_final_string[MAXLINE];
	memset(textfield_final_string, 0, sizeof(textfield_final_string));

	int error = git_repository_open(&repo, path);
	if (error != 0)
		git_error_handling();

	if (git_remote_lookup(&remote, repo, url) != 0) {
		error = git_remote_create_anonymous(&remote, repo, url);
		if (error != 0)
			git_error_handling();
	}

	fetch_opts.callbacks.update_tips = &update_tips;
	fetch_opts.callbacks.sideband_progress = sideband_progress;
	fetch_opts.callbacks.transfer_progress = fetch_progress;

	error = git_remote_fetch(remote, NULL, &fetch_opts, "fetch");
	if (error != 0)
		git_error_handling();

	stats = git_remote_stats(remote);
	int receive_kbyte = stats->received_bytes / 1024;
	if (stats->local_objects > 0) {
		snprintf(textfield_final_string,
			 sizeof(textfield_final_string),
			 _("Fetched: (%d/%d) %d kB (used %d local objects)"),
			 stats->indexed_objects, stats->total_objects,
			 receive_kbyte,
			 stats->local_objects);

		write_info_msg(textfield_final_string);
	} else {
		snprintf(textfield_final_string,
			 sizeof(textfield_final_string),
			 _("Fetched: (%d/%d) %d kB"),
			 stats->indexed_objects, stats->total_objects,
			 receive_kbyte);

		write_info_msg(textfield_final_string);
	}

out:
	if (remote)
		git_remote_free(remote);
	if (repo)
		git_repository_free(repo);

}
