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

// toolchain adds
#define TOOLCHAIN_NAME_ADD       "toolchain_x86_64.tgz/download"
#define TOOLCHAIN_PATH_ADD       "/toolchain_x86_64.tgz"
#define TOOLCHAIN_NAME_HOST_ADD  "host_x86_64.tgz/download"
#define TOOLCHAIN_PATH_HOST_ADD  "/host_x86_64.tgz"

// bananapi adds

// bananapi-pro adds

// cubietruck adds

// olimey adds


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


static
int do_download(download_tupel_t *download)
{
	CURL *curl;
	CURLcode res;
	FILE *fd;

	char *url = download->url;
	char *path = download->path;

	printf("in do_download \n\n");

	printf("URL: %s \n", url);
	printf("PATH: %s \n", path);

	if ((url == NULL) || (path == NULL)) {
		fprintf(stderr, "ERROR: (url == NULL) || (path == NULL) -> %s", strerror(errno));
		write_to_textfield(_("(url == NULL) || (path == NULL) \n"), ERROR_MSG);
		goto error;
	}

	curl = curl_easy_init();
	if (curl)
	{
		// TODO: error handling
		fd = fopen(path, "wb");
		if (fd == NULL) {
			fprintf(stderr, "ERROR: fd == NULL -> %s", strerror(errno));
			write_to_textfield(_("fd == NULL \n"), ERROR_MSG);
			goto error;
		}

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
		goto error;
	}

	return 0;

error:
	if (curl)
		curl_easy_cleanup(curl);

	return -1;
}

static int
build_download_array(download_types_t d_type, download_tupel_t *download_array[])
{
	PRINT_LOCATION();

	char *url = NULL;
	char *path = NULL;

	size_t len_url = 0;
	size_t len_path = 0;

	size_t len_add_url = 0;
	size_t len_add_path = 0;

	size_t len_base_url = strlen(TOOLCHAIN_NAME);
	size_t len_base_path = strlen(TOOLCHAIN_PATH);

	download_tupel_t *element = NULL;

	switch (d_type)
	{
	case DOWNLOAD_TOOLCHAIN:

		/*
		 * toolchain_x86_64.tgz
		 */
		// build url string
		len_add_url = strlen(TOOLCHAIN_NAME_ADD);
		len_url = len_base_url + len_add_url;

		url = malloc(len_url + 1);
		if (url == NULL)
			goto error;
		memset(url, 0, len_url + 1);

		strncpy(url, TOOLCHAIN_NAME, len_base_url);
		strncat(url, TOOLCHAIN_NAME_ADD, len_add_url);

		printf("in build array with url: %s\n", url);

		// build path string
		len_add_path = strlen(TOOLCHAIN_PATH_ADD);
		len_path = len_base_path + len_add_path;

		path = malloc(len_path + 1);
		if (path == NULL)
			goto error;
		memset(path, 0, len_path + 1);

		strncpy(path, TOOLCHAIN_PATH, len_base_path);
		strncat(path, TOOLCHAIN_PATH_ADD, len_add_path);

		printf("in build array with url: %s\n", path);

		// build the first element of dowload_array
		element = malloc(sizeof(download_tupel_t));
		if (element == NULL)
			goto error;

		memset(element, 0, sizeof(download_tupel_t));
		element->path = path;
		element->url = url;

		// add to download array -> toolchain_x86_64.tgz
		download_array[0] = element;

		/*
		 * host_x86_64.tgz
		 */
		// build url string
		len_add_url = strlen(TOOLCHAIN_NAME_HOST_ADD);
		len_url = len_base_url + len_add_url;

		url = malloc(len_url + 1);
		if (url == NULL)
			goto error;
		memset(url, 0, len_url + 1);

		strncpy(url, TOOLCHAIN_NAME, len_base_url);
		strncat(url, TOOLCHAIN_NAME_HOST_ADD, len_add_url);

		printf("in build array with url: %s\n", url);

		// build path string
		len_add_path = strlen(TOOLCHAIN_PATH_HOST_ADD);
		len_path = len_base_path + len_add_path;

		path = malloc(len_path + 1);
		if (path == NULL)
			goto error;
		memset(path, 0, len_path + 1);

		strncpy(path, TOOLCHAIN_PATH, len_base_path);
		strncat(path, TOOLCHAIN_PATH_HOST_ADD, len_add_path);

		printf("in build array with url: %s\n", path);

		// build the first element of dowload_array
		element = malloc(sizeof(download_tupel_t));
		if (element == NULL)
			goto error;

		memset(element, 0, sizeof(download_tupel_t));
		element->path = path;
		element->url = url;

                // add to download array -> host_x86_64.tgz
		download_array[1] = element;

		break;
	case DOWNLOAD_BANANAPI:
		// not yet
		break;
	case DOWNLOAD_BANANAPI_PRO:
		// not yet
		break;
	case DOWNLOAD_CUBIETRUCK:
		// not yet
		break;
	case DOWNLOAD_OLIMEX:
		// not yet
		break;
	case DOWNLOAD_ALL_IMAGES:
		// not yet
		break;
	case DOWNLOAD_ALL:
		// not yet
		break;
	default:
		fprintf(stderr, "ERROR: unknown download_type\n");
		return -1;
	}

	printf("URL: %s \n", url);
	printf("PATH: %s \n", path);

	return 0;

error:
	if (url)
		free(url);

	if (path)
		free(path);

	if (element)
		free(element);

	fprintf(stderr, "ERROR: something went wrong in %s\n", __FUNCTION__);

	return -1;
}


void *
download_toolchain(void *args)
{
	download_tupel_t *download_array[MAX_SIZE_DOWNLOAD_ARRAY];
	memset(download_array, 0, sizeof(download_array));

	printf("in download toolchain\n");

	(void) args;

        /*
	 * only one thread could be active
	 */
	enter_sdk_thread();

	if (create_progress_bar_window(DOWNLOAD_BAR) != 0)
		fprintf(stderr, _("ERROR: create_progress_bar_window != 0\n"));

	if (build_download_array(DOWNLOAD_TOOLCHAIN, download_array) != 0)
		goto out;

	printf("vor for schleife\n");
	int i = 0;
	for (;;) {
		printf("in for schleife mit i == %d\n", i);
		if (download_array[i] == NULL)
			break;

		printf("in for schleife download_array != NULL\n");

		set_progressbar_value(0, "0%");
		if (do_download(download_array[i]) == -1)
			break;

		set_progressbar_value(100, "100%");

		extract_toolchain(download_array[i]->path);
		i++;
	}

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

	//const char *toolchain_path = TOOLCHAIN_PATH;
        //const char *repo_path = SDK_GIT_PATH;

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
