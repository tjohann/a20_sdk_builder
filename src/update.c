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
	(void) payload; // not used

	fprintf(stdout, "Remote: %.*s \n", len, str);

	return 0;
}


static int
update_tips(const char *refname,
	    const git_oid *first,
	    const git_oid *second,
	    void *payload)
{
	char first_oid_s[GIT_OID_HEXSZ+1];
	char second_oid_s[GIT_OID_HEXSZ+1];

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
		fprintf(stdout, "[new] %.20s %s\n", second_oid_s, refname);
	} else {
		git_oid_fmt(first_oid_s, first);
		first_oid_s[GIT_OID_HEXSZ] = '\0';
		fprintf(stdout,
			"[updated] %.10s..%.10s %s\n",
			first_oid_s, second_oid_s, refname);
	}

	return 0;
}


static int
transfer_progress(const git_transfer_progress *stats, void *payload)
{
	if (stats->received_objects == stats->total_objects) {
		fprintf(stdout,
			"Resolving deltas %d/%d\r",
			stats->indexed_deltas, stats->total_deltas);
	} else if (stats->total_objects > 0) {
		fprintf(stdout,
			"Fetched: %d/%d objects (%d) with %zu bytes\r",
			stats->received_objects, stats->total_objects,
			stats->indexed_objects,
			stats->received_bytes);
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

	(void) args; // not used

	int error = git_repository_open(&repo, path);
	if (error != 0) {
		GIT_ERROR_HANDLING();
	}

	if (git_remote_lookup(&remote, repo, url) != 0) {
		error = git_remote_create_anonymous(&remote, repo, url);
		if (error != 0) {
			GIT_ERROR_HANDLING();
		}
	}

	fetch_opts.callbacks.update_tips = &update_tips;
	fetch_opts.callbacks.sideband_progress = sideband_progress;
	fetch_opts.callbacks.transfer_progress = transfer_progress;

	error = git_remote_fetch(remote, NULL, &fetch_opts, "fetch");
	if (error != 0) {
		GIT_ERROR_HANDLING();
	}

	stats = git_remote_stats(remote);
	if (stats->local_objects > 0) {
		fprintf(stdout,
			"Fetched: %d/%d objects with %zu bytes (used %d local objects)\n",
			stats->indexed_objects, stats->total_objects,
			stats->received_bytes,
			stats->local_objects);
	} else{
		fprintf(stdout,
			"Fetched: %d/%d objects with %zu bytes\n",
			stats->indexed_objects, stats->total_objects,
			stats->received_bytes);
	}

out:
	if (remote)
		git_remote_free(remote);

	if (repo)
		git_repository_free(repo);

	return NULL;
}
