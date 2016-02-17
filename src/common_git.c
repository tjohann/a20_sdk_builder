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

int
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


int
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


void
check_sdk_git_path()
{
	PRINT_LOCATION();

	git_repository *repo = NULL;

	const char *path = get_sdk_repo_path();
	if (path != NULL) {
		int error = git_repository_open_ext(&repo, path, 0, NULL);
		if (error == 0) {
			lock_button(CLONE_B);
			lock_button(CLONE_M);
			unlock_button(UPDATE_B);
			unlock_button(UPDATE_M);
		} else {
			lock_button(UPDATE_B);
			lock_button(UPDATE_M);
			unlock_button(CLONE_B);
			unlock_button(CLONE_M);
		}
	} else {
		error_msg("get_sdk_repo_path() == NULL");
	}

	if (repo)
		git_repository_free(repo);
}
