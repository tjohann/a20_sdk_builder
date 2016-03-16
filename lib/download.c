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
download_progress(void *p,
		  curl_off_t tot, curl_off_t cur,     // download
		  curl_off_t u_tot, curl_off_t u_cur) // upload
{
	unsigned int statusbar_percent = (100.0 * cur) / tot;
	char statusbar_percent_string[5];

	(void) p;
	(void) u_tot;
	(void) u_cur;

	memset(statusbar_percent_string, 0, sizeof(statusbar_percent_string));
	snprintf(statusbar_percent_string, 5, "%3d%%", statusbar_percent);

#ifdef __DEBUG__
	info_msg(_("Download: %3d%% (%llu/%llu)"), statusbar_percent,
		cur, tot);
#endif
	set_progressbar_value(statusbar_percent, statusbar_percent_string);

	return 0;
}


static int
do_download_tupel(download_tupel_t *download)
{
	CURL *curl = NULL;
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
				 download_progress);

		res = curl_easy_perform(curl);
		if (res != CURLE_OK)
			write_error_msg(_("curl_easy_perform(curl) != CURLE_OK"));

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


int
do_download(download_tupel_t *download_array[])
{
	checksum_tupel_t *c = NULL;
	char *name = NULL;
	int i = 0;

	for (;;) {
		if (download_array[i] == NULL)
			break;

		set_progressbar_window_title(get_download_tupel_path(download_array[i]));
		set_progressbar_value(0, "0%");

		if (do_download_tupel(download_array[i]) == -1)
			break;

		set_progressbar_value(100, _("!! calc checksum !!"));

		if (calc_checksum(download_array[i]) != 0)
			write_error_msg("Possible ERROR -> calc_checksum != 0");

		name = strrchr(download_array[i]->path, '/');
		if (name == NULL)
			name = download_array[i]->path;
		else
			name++;

#ifdef __DEBUG__
		PRINT_LOCATION();
		info_msg(_("download_tupel->path extracted filename %s"), name);
#endif

		c = get_checksum_tupel(name);
		if (c == NULL) {
			write_error_msg("Possible ERROR -> c == NULL");
			break;
		}

#ifdef __DEBUG__
		PRINT_LOCATION();
		info_msg(_("c->name: %s with c->checksum_s: %s"),
			 c->name, c->checksum_s);
#endif

		if (strncmp(download_array[i]->checksum_s, c->checksum_s,
			    strlen(download_array[i]->checksum_s)) == 0) {
			info_msg(_("checksum is okay"));
		} else {
			write_error_msg(_("checksum is NOT okay"));
			break;
		}

		set_progressbar_value(100, _("!! extracting !!"));

		write_info_msg(_("Extrating file: %s"), download_array[i]->path);
		if (extract_toolchain(download_array[i]->path) == -1)
			break;
		i++;
	}



	return 0;
}
