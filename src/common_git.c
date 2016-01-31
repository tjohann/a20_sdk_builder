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
	(void) payload; // not used

	fprintf(stdout, "Remote: %.*s \n", len, str);

	return 0;
}


int
fetch_progress(const git_transfer_progress *stats, void *payload)
{
	int fetch_percent = (100 * stats->received_objects)/stats->total_objects;
	//int index_percent = (100 * stats->indexed_objects) /stats->total_objects;
	int receive_kbyte = stats->received_bytes / 1024;

	int statusbar_percent = (fetch_percent * 5) / 6;
	char statusbar_percent_string[5];

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

	(void) payload; // not used

	if (stats->received_objects == stats->total_objects) {
		fprintf(stdout,
			"Resolving deltas %d/%d\r",
			stats->indexed_deltas, stats->total_deltas);
	} else if (stats->total_objects > 0) {
		fprintf(stdout,	"Fetched: %3d%% (%d/%d) %d kB \n",
			fetch_percent,
			stats->received_objects, stats->total_objects,
			receive_kbyte);
	}

	/*
	 * I suppose that if the window is destroyed for whatever reason,
	 * the clone proccess should continue. Otherwise we leave a broken
	 * git repo.
	 */
	if (progressbar != NULL)
		set_progressbar_value(statusbar_percent, statusbar_percent_string);
	else
		fprintf(stderr,
			_("progressbar == NULL -> progressbar_window destroyed?\n"));

	return 0;
}
