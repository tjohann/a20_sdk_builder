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

	fprintf(stdout, "Download: %3d%% (%llu/%llu)\n",
		statusbar_percent,
		cur, tot);

	if (progressbar != NULL)
		set_progressbar_value(statusbar_percent, statusbar_percent_string);
	else {
		fprintf(stderr,
			_("ERROR: progressbar == NULL -> progressbar_window destroyed?\n"));
		write_to_textfield(
			_("progressbar == NULL -> progressbar_window destroyed?\n"),
			ERROR_MSG);
	}

	return 0;
}


void *
download_toolchain(void *args)
{
	CURL *curl;
	CURLcode res;
	FILE *fd;

	//const char *url = TOOLCHAIN_NAME;
	//const char *path = TOOLCHAIN_PATH;

	const char *url = "http://sourceforge.net/projects/a20devices/files/toolchain_x86_64.tgz/download";
	const char *path = "/opt/a20_sdk/toolchain_x86_64.tgz";

        /*
	 * only one thread could be active
	 */
	enter_sdk_thread();

	if (create_progress_bar_window(DOWNLOAD_BAR) != 0)
		fprintf(stderr, _("ERROR: create_progress_bar_window != 0\n"));

	curl = curl_easy_init();
	if (curl)
	{
		fd = fopen(path, "wb");

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_func);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_func);

		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, checkout_progress);

		// TODO: check value
		res = curl_easy_perform(curl);

		fclose(fd);
		curl_easy_cleanup(curl);
	} else {
		fprintf(stderr, _("ERROR: curl_easy_init == NULL \n"));
		write_to_textfield(_("curl_easy_init == NULL \n"), ERROR_MSG);
		goto out;
	}

	set_progressbar_value(100, "100%");


	extract_toolchain();

out:
	/*
	 * check for correct state
	 */
	leave_sdk_thread();

	gtk_widget_set_sensitive(progressbar_button, TRUE);

	return NULL;
}

void
check_toolchain()
{
	PRINT_LOCATION();

	const char *toolchain_path = TOOLCHAIN_PATH;
        const char *repo_path = SDK_GIT_PATH;

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
