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
update_tips(const char *refname,
	    const git_oid *first,
	    const git_oid *second,
	    void *payload)
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
			 _("[new] %.20s %s\n"), second_oid_s, refname);

		write_to_textfield(textfield_update_string, INFO_MSG);
		fprintf(stdout, textfield_update_string);
	} else {
		git_oid_fmt(first_oid_s, first);
		first_oid_s[GIT_OID_HEXSZ] = '\0';

		snprintf(textfield_update_string, sizeof(textfield_update_string),
			 _("[updated] %.10s..%.10s %s\n"),
			 first_oid_s, second_oid_s, refname);
		fprintf(stdout, textfield_update_string);
	}

	return 0;
}

void *
update_sdk_repo(void *args)
{
	git_repository *repo;
	git_remote *remote = NULL;
	git_fetch_options fetch_opts = GIT_FETCH_OPTIONS_INIT;

	const git_transfer_progress *stats;

	const char *url = REPO_NAME;
	const char *path = SDK_GIT_PATH;

	char textfield_final_string[MAXLINE];
	memset(textfield_final_string, 0, sizeof(textfield_final_string));

	(void) args; // not used

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

	/*
	 * only one thread could be active
	 */
	enter_sdk_thread();

	if (create_progress_bar_window(UPDATE_BAR) != 0)
		fprintf(stderr, _("ERROR: create_progress_bar_window != 0\n"));

	error = git_remote_fetch(remote, NULL, &fetch_opts, "fetch");
	if (error != 0)
		git_error_handling();

	stats = git_remote_stats(remote);
	int receive_kbyte = stats->received_bytes / 1024;
	if (stats->local_objects > 0) {
		snprintf(textfield_final_string,
			 sizeof(textfield_final_string),
			 _("Fetched: (%d/%d) %d kB (used %d local objects)\n"),
			 stats->indexed_objects, stats->total_objects,
			 receive_kbyte,
			 stats->local_objects);

		write_to_textfield(textfield_final_string, INFO_MSG);
		fprintf(stdout, textfield_final_string);
	} else {
		snprintf(textfield_final_string,
			 sizeof(textfield_final_string),
			 _("Fetched: (%d/%d) %d kB \n"),
			 stats->indexed_objects, stats->total_objects,
			 receive_kbyte);

		write_to_textfield(textfield_final_string, INFO_MSG);
		fprintf(stdout, textfield_final_string);
	}

	set_progressbar_value(100, "100%");

out:
	/*
	 * check for correct state
	 */
	leave_sdk_thread();

	if (remote)
		git_remote_free(remote);
	if (repo)
		git_repository_free(repo);

	gtk_widget_set_sensitive(progressbar_button, TRUE);

	return NULL;
}
