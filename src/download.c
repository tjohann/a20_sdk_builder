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

#define MAX_SIZE_DOWNLOAD_ARRAY 15


static size_t
write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}


static size_t
read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fread(ptr, size, nmemb, stream);
}


static int
checkout_progress(void *p,
		  curl_off_t tot,   // download
		  curl_off_t cur,   // download
		  curl_off_t u_tot, // upload
		  curl_off_t u_cur) // upload
{
	unsigned int statusbar_percent = (100.0 * cur) / tot;
	char statusbar_percent_string[5];

	(void) p;

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

#ifdef DEBUG
	info_msg(_("Download: %3d%% (%llu/%llu)"), statusbar_percent,
		cur, tot);
#endif
	set_progressbar_value(statusbar_percent, statusbar_percent_string);

	return 0;
}


static
int do_download(download_tupel_t *download)
{
	CURL *curl;
	CURLcode res;
	FILE *fd;

	char *url = download->url;
	char *path = download->path;

	if ((url == NULL) || (path == NULL)) {
		write_error_msg(_("(url == NULL) || (path == NULL)"));
		goto error;
	}

	curl = curl_easy_init();
	if (curl)
	{
		fd = fopen(path, "wb");
		if (fd == NULL) {
			write_error_msg(_("fd == NULL"));
			goto error;
		}

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_func);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,
				 checkout_progress);

		// TODO: check value
		res = curl_easy_perform(curl);

		fclose(fd);
		curl_easy_cleanup(curl);
	} else {
		write_error_msg(_("curl_easy_init == NULL"));
		goto error;
	}

	return 0;

error:
	if (curl)
		curl_easy_cleanup(curl);

	return -1;
}


void *
download_toolchain(void *args)
{
	download_tupel_t *download_array[MAX_SIZE_DOWNLOAD_ARRAY];
	memset(download_array, 0, sizeof(download_array));

	(void) args;

	enter_sdk_thread();

	/*
	 * toolchain -> cross-compiler + sysroot
	 * host -> host related tools like mkimage (source of sysroot)
	 */
	download_array[0] = get_toolchain();
	download_array[1] = get_host();

	if (create_progressbar_window(_("Download toolchain/host")) != 0)
		write_error_msg(_("ERROR: create_progress_bar_window != 0"));

	int i = 0;
	for (;;) {
		if (download_array[i] == NULL)
			break;

		set_progressbar_window_title(get_download_tupel_path(download_array[i]));
		set_progressbar_value(0, "0%");
		if (do_download(download_array[i]) == -1)
			break;
		set_progressbar_value(100, "!! extracting !!");

		// TODO: add filename
		write_info_msg("Extrating file: %s", download_array[i]->path);
		if (extract_toolchain(download_array[i]->path) == -1)
			break;
		i++;
	}

out:
	leave_sdk_thread();

	set_progressbar_value(100, "100%");
	set_progressbar_window_title(_("Download toolchain/host"));
	unlock_button(PROGRESSBAR_B);

	return NULL;
}


void
check_toolchain()
{
	PRINT_LOCATION();

	/*
	 * check checksum.sha256 and tgz are in sync
	 */
	int state = 1;
	if (state == 0) {
		lock_button(DOWNLOAD_B);
		lock_button(DOWNLOAD_M);
	} else {
		unlock_button(DOWNLOAD_B);
		unlock_button(DOWNLOAD_M);
	}
}
