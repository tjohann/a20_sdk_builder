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
#include "global.h"


static int
read_conf_string(config_t *cfg, char *conf_name, char **write_to)
{
	const char *str;

	int error = config_lookup_string(cfg, conf_name, &str);
	if (error == CONFIG_FALSE) {
                error_msg(_("No '%s' setting in config file!"),	conf_name);
		return -1;
	}

	*write_to = alloc_string(str);
	if (*write_to == NULL)
		return -1;

	return 0;
}


static int
read_conf_download_tupel(config_t *cfg,
			 char *url,
			 char *path,
			 download_tupel_t  **write_to)
{
	const char *str;
	char *tmp_url = NULL;
	char *tmp_path = NULL;
	download_tupel_t *repo = NULL;

	repo = malloc(sizeof(download_tupel_t));
	if (repo == NULL)
		goto error;

	int error = config_lookup_string(cfg, url, &str);
	if (error == CONFIG_FALSE) {
                error_msg(_("No '%s' setting in config file!"), url);
		goto error;;
	}

	tmp_url = alloc_string(str);;
	if (tmp_url == NULL)
		goto error;


	error = config_lookup_string(cfg, path, &str);
	if (error == CONFIG_FALSE) {
                error_msg(_("No '%s' setting in config file!"), path);
		goto error;;
	}

	tmp_path = alloc_string(str);;
	if (tmp_path == NULL)
		goto error;

	repo->url = tmp_url;
	repo->path = tmp_path;
	*write_to = repo;

	return 0;

error:
	if (tmp_url != NULL)
		free(tmp_url);
	if (tmp_path != NULL)
		free(tmp_path);
	if (repo != NULL)
		free(repo);

	return -1;
}


int
set_conf_location(char *conf_file, char *conf_dir)
{
	conf_path_t *tmp = NULL;
	char *conf_f = NULL;
	char *conf_d = NULL;

	// the default sysconfdir (see FHS)
	char *usr_local_etc = "/usr/local/etc/sdk_builder";
	char *etc = "/etc/sdk_builder";

	tmp = malloc(sizeof(conf_path_t));
	if (tmp == NULL)
		return -1;

	memset(tmp, 0, sizeof(conf_path_t));

	if (conf_dir == NULL) {
		info_msg(_("Argument conf_dir == NULL in %s"), __FUNCTION__);

		if (!is_this_a_dir(usr_local_etc) && !(is_this_a_dir(etc))) {
			error_msg(
				_("No valid config dir in /etc or /usr/local/etc"));
			goto error;
		}

		if (is_this_a_dir(etc))
			conf_dir = etc;
		else
			conf_dir = usr_local_etc;
	}

	conf_d = alloc_string(conf_dir);
	if (conf_d == NULL)
		goto error;

	conf_f = alloc_string(conf_file);
	if (conf_f == NULL)
		goto error;

	tmp->conf_file = conf_f;
	tmp->conf_dir = conf_d;
	conf_location = tmp;

	return 0;

error:
	if (tmp != NULL)
		free(tmp);
	if (conf_f != NULL)
		free(conf_f);
	if (conf_d != NULL)
		free(conf_d);

	return -1;
}


static int
get_config_object(config_t **cfg)
{
	char *conf_f = get_conf_location_file();
	char *conf_d = get_conf_location_dir();

	// read config
	if (chdir(conf_d) == -1)
		error_exit(_("can't change to dir %s"), conf_d);

        config_init(*cfg);
	if (config_read_file(*cfg, conf_f) != CONFIG_TRUE)
	{
                error_msg("%s:%d - %s", config_error_file(*cfg),
			  config_error_line(*cfg), config_error_text(*cfg));
		config_destroy(*cfg);
		return -1;
        }

	return 0;
}


char *
get_conf_location_dir()
{
	if (conf_location->conf_dir == NULL)
		error_msg("conf_location->conf_dir == NULL");
	else
		return conf_location->conf_dir;

	return NULL;
}


char *
get_conf_location_file()
{
	if (conf_location->conf_file == NULL)
		error_msg("conf_location->conf_file == NULL");
	else
		return conf_location->conf_file;

	return NULL;
}


char *
get_common_gui_name()
{
	if (gui_name == NULL)
		error_msg("gui_name == NULL");
	else
		return gui_name;

	return NULL;
}


char *
get_common_workdir()
{
	if (workdir == NULL)
		error_msg("workdir == NULL");
	else
		return workdir;

	return NULL;
}


char *
get_common_runtimedir()
{
	if (runtimedir == NULL)
		error_msg("runtimedir == NULL");
	else
		return runtimedir;

	return NULL;
}


char *
get_sdk_repo_url()
{
	if (sdk_repo->url == NULL)
		error_msg("sdk_repo->url == NULL");
	else
		return sdk_repo->url;

	return NULL;
}


char *
get_sdk_repo_path()
{
	if (sdk_repo->path == NULL)
		error_msg("sdk_repo->path == NULL");
	else
		return sdk_repo->path;

	return NULL;
}


char *
get_toolchain_url()
{
	if (toolchain->url == NULL)
		error_msg("toolchain->url == NULL");
	else
		return toolchain->url;

	return NULL;
}


char *
get_toolchain_path()
{
	if (toolchain->path == NULL)
		error_msg("toolchain->path == NULL");
	else
		return toolchain->path;

	return NULL;
}


download_tupel_t *
get_toolchain()
{
	if (toolchain == NULL)
		error_msg("toolchain == NULL");
	else
		return toolchain;

	return NULL;
}


char *
get_host_url()
{
	if (host->url == NULL)
		error_msg("host->url == NULL");
	else
		return host->url;

	return NULL;
}


char *
get_host_path()
{
	if (host->path == NULL)
		error_msg("host->path == NULL");
	else
		return host->path;

	return NULL;
}


download_tupel_t *
get_host()
{
	if (host == NULL)
		error_msg("host == NULL");
	else
		return host;

	return NULL;
}


char *
get_download_tupel_url(download_tupel_t *t)
{
	if (t == NULL)
		return NULL;

	if (t->url == NULL)
		error_msg("t->url == NULL");
	else
		return t->url;

	return NULL;
}


char *
get_download_tupel_path(download_tupel_t *t)
{
	if (t == NULL)
		return NULL;

	if (t->path == NULL)
		error_msg("t->path == NULL");
	else
		return t->path;

	return NULL;
}


static int
read_checksum_from_file(char *filename, checksum_tupel_t *checksum_array[])
{
	int n = 0, m = 0;
	int lines = 0;
	checksum_tupel_t *c = NULL;

	char *elements[2];

	int fd = open(filename, O_RDONLY);
	if (fd < 0)
		error_msg_return(_("Possible ERROR: couldn't open %s"),
				 filename);
	char tmp_str[MAXLINE];
	memset(tmp_str, 0, MAXLINE);

read_cmd:
	n = read_line(fd, tmp_str, MAXLINE);
	if (n > 1) {
		c = malloc(n);
		if (c == NULL)
			error_msg_return(_("c == NULL in %s"), __FUNCTION__);
		memset(c, 0, n);
		lines++;

		printf("tmp_str %s with len %d and count %d\n", tmp_str, n, lines);

		m = read_words(tmp_str, elements, 2);
		if (m == 2) {
			// do something
		} else {
			error_msg(_("read_words() != 2"));
		}

		printf("\n\t%d\n", m);
		printf("\nGEDÖENS !!!%s!!! \n !!!%s!!!\n", elements[0], elements[1]);

		/*
		 * TODO: alloc_string and put it into checksum_config_t
		 */

		memset(tmp_str, 0, MAXLINE);
		if (lines >= LEN_CHECKSUM_ARRAY)
			debug_msg(_("lines >= LEN_CHECKSUM_ARRAY"));
		else
			goto read_cmd;
	}


	close(fd);
	return 0;
}


int
read_checksum_file()
{
	size_t len = strlen(NAME_CHECKSUM_FILE);

	const char *runtime_s = get_common_runtimedir();
	if (runtime_s == NULL)
		error_msg_return("runtime dir == NULL");

	// as always, but with backslash between both
	len += (strlen(runtime_s) + 2);
	char tmp_str[len];
	memset(tmp_str, 0, len);

	strncpy(tmp_str, runtime_s, strlen(runtime_s));
	strncat(tmp_str, "/", 1);
	strncat(tmp_str, NAME_CHECKSUM_FILE, strlen(NAME_CHECKSUM_FILE));

#ifndef DEBUG
	info_msg(_("runtime_s: %s and len %d"), runtime_s, len);
	info_msg(_("tmp_str: %s"), tmp_str);
#endif

	if (read_checksum_from_file(tmp_str, checksum_array) != 0)
		error_msg_return("read_checksum_from_file != 0");

	return 0;
}


int
init_main_config(char *conf_file, char *conf_dir)
{
	config_t cfg;
	config_t *cfg_p = &cfg;
	const char *sdk_name = "SDK-Builder";

	if (conf_file == NULL)
		return -1;

	int error = set_conf_location(conf_file, conf_dir);
	if (error != 0)
		goto error;

	if (get_config_object(&cfg_p) != 0)
		return -1;

	// sdk_config_name
	error = read_conf_string(&cfg, "sdk_config_name", &sdk_config_name);
	if (error != 0)
		goto error;

	// check for correct config type (a minimal check only)
	if (strncmp(sdk_config_name, sdk_name, strlen(sdk_name)) != 0) {
		error_msg(_("Not a valid configuration type!"));
		goto error;
	}

	config_destroy(&cfg);
	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


int
init_common_config()
{
	config_t cfg;
	config_t *cfg_p = &cfg;

	if (get_config_object(&cfg_p) != 0)
		return -1;

	// workdir
	int error = read_conf_string(&cfg, "common.workdir", &workdir);
	if (error != 0)
		goto error;

	// runtimedir
	error = read_conf_string(&cfg, "common.runtimedir", &runtimedir);
	if (error != 0)
		goto error;

	// gui_name
	error = read_conf_string(&cfg, "common.gui_name", &gui_name);
	if (error != 0)
		goto error;

	config_destroy(&cfg);
	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


int
init_repo_config()
{
	config_t cfg;
	config_t *cfg_p = &cfg;

	if (get_config_object(&cfg_p) != 0)
		return -1;

	// repo
	int error = read_conf_download_tupel(&cfg, "sdk_repo.url",
					     "sdk_repo.path", &sdk_repo);
	if (error != 0)
		goto error;

	config_destroy(&cfg);
	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


int
init_toolchain_config()
{
	config_t cfg;
	config_t *cfg_p = &cfg;

	if (get_config_object(&cfg_p) != 0)
		return -1;

	// toolchain
	int error = read_conf_download_tupel(&cfg, "toolchain.url", "toolchain.path",
					 &toolchain);
	if (error != 0)
		goto error;

	// host
	error = read_conf_download_tupel(&cfg, "host.url", "host.path",
					 &host);
	if (error != 0)
		goto error;

	config_destroy(&cfg);
	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


int
init_device_config()
{
	config_t cfg;
	config_t *cfg_p = &cfg;

	if (get_config_object(&cfg_p) != 0)
		return -1;

	PRINT_LOCATION();

	config_destroy(&cfg);
	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


int
init_external_config()
{
	config_t cfg;
	config_t *cfg_p = &cfg;

	if (get_config_object(&cfg_p) != 0)
		return -1;

	PRINT_LOCATION();

	config_destroy(&cfg);
	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


int
init_kernel_config()
{

	PRINT_LOCATION();

        // do something
	return 0;
}


void
show_config()
{
	putchar('\n');
	info_msg(_("Show content of internal configuration: "));
	info_msg("--------------------------------------- ");
	info_msg(_("Global: conf_location->conf_file: %s    "),	conf_location->conf_file);
	info_msg(_("Global: conf_location->conf_dir: %s     "), conf_location->conf_dir);
	info_msg(_("Global: sdk_config_name: %s             "),	sdk_config_name);
	info_msg(_("Global: gui_name: %s                    "), gui_name);

	// repo
	if (sdk_repo != NULL) {
		info_msg(_("Global: sdk_repo->url %s        "), sdk_repo->url);
		info_msg(_("Global: sdk_repo->path %s       "), sdk_repo->path);
	} else {
		info_msg(_("Global: sdk_repo == NULL "));
	}

	// toolchain
	if (toolchain != NULL) {
		info_msg(_("Global: toolchain->url %s       "),	toolchain->url);
		info_msg(_("Global: toolchain->path %s      "), toolchain->path);
	} else {
		info_msg(_("Global: toolchain == NULL"));
	}

	// host
	if (host != NULL) {
		info_msg(_("Global: host->url %s            "),	host->url);
		info_msg(_("Global: host->path %s           "), host->path);
	} else {
		info_msg(_("Global: host == NULL     "));
	}


	fprintf(stdout, _("--------------------------------------- \n"));
	putchar('\n');
}
