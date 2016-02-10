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
#include "global.h"


#define INIT_ERROR() do {			\
		if (error != 0)			\
			goto error;		\
	} while(0)


static int
fill_conf_location(char *conf_file, char *conf_dir)
{
	conf_path_t *tmp = NULL;
	char *conf_f = NULL;
	char *conf_d = NULL;
	char *usr_local_etc = "/usr/local/etc/sdb_builder";
	char *etc = "/etc/sdk_builder";

	tmp = malloc(sizeof(conf_path_t));
	if (tmp == NULL)
		return -1;

	memset(tmp, 0, sizeof(conf_path_t));

	if (conf_dir == NULL) {
		fprintf(stdout, _("Argument conf_dir == NULL in %s\n"),
			__FUNCTION__);

		if (!is_this_a_dir(usr_local_etc) && !(is_this_a_dir(etc))) {
			fprintf(stderr,
				_("No valid config dir in /etc or /usr/local/etc\n"));
			goto error;
		}

		if (is_this_a_dir(etc))
			conf_dir = etc;
		else
			conf_dir = usr_local_etc;
	}

	// conf dir handling
	conf_d = alloc_string(conf_dir);
	if (conf_d == NULL)
		goto error;

	// conf file handling
	conf_f = alloc_string(conf_file);
	if (conf_f == NULL)
		goto error;

	// finalize
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
read_conf_string(config_t *cfg, char *conf_name, char **write_to)
{
	const char *str;

	int error = config_lookup_string(cfg, conf_name, &str);
	if (error == CONFIG_FALSE) {
                fprintf(stderr, _("ERROR: No '%s' setting in config file!\n"),
			conf_name);
		return -1;
	}

	*write_to = alloc_string(str);
	if (*write_to == NULL)
		return -1;

	return 0;
}


static int
read_conf_download_tupel(config_t *cfg, char *url, char *path, download_tupel_t  **write_to)
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
                fprintf(stderr, _("ERROR: No '%s' setting in config file!\n"),
			url);
		goto error;;
	}

	tmp_url = alloc_string(str);;
	if (tmp_url == NULL)
		goto error;


	error = config_lookup_string(cfg, path, &str);
	if (error == CONFIG_FALSE) {
                fprintf(stderr, _("ERROR: No '%s' setting in config file!\n"),
			path);
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
init_main_config(char *conf_file, char *conf_dir)
{
	const char *sdk_name = "SDK-Builder";
	config_t cfg;

	if (conf_file == NULL)
		return -1;

	int error = fill_conf_location(conf_file, conf_dir);
	if (error != 0)
		goto error;

	// read config
	if (chdir(conf_location->conf_dir) == -1)
		error_exit(_("can't change to dir %s -> %s"),
			   conf_location->conf_dir,
			   strerror(errno));

        config_init(&cfg);
        if (config_read_file(&cfg, conf_location->conf_file) != CONFIG_TRUE)
        {
                fprintf(stderr, "ERROR: %s:%d - %s\n",
                        config_error_file(&cfg),
                        config_error_line(&cfg),
                        config_error_text(&cfg));

                goto error;
        }

        // name
	error = read_conf_string(&cfg, "name", &name);
	if (error != 0)
		goto error;

	// check for correct config type (a minimal check only)
	if (strncmp(name, sdk_name, strlen(sdk_name)) != 0) {
		fprintf(stderr, _("ERROR: not a valid configuration type!\n"));
		goto error;
	}

	// gui_name
	error = read_conf_string(&cfg, "common.gui_name", &gui_name);
	if (error != 0)
		goto error;

        // repo
	error = read_conf_download_tupel(&cfg, "repo.url", "repo.path", &repo);
	if (error != 0)
		goto error;

	// toolchain
	error = read_conf_download_tupel(&cfg,
					 "toolchain.url", "toolchain.path",
					 &toolchain);
	if (error != 0)
		goto error;

	// host
	error = read_conf_download_tupel(&cfg,
					 "host.url", "host.path",
					 &host);
	if (error != 0)
		goto error;


        // device1
	error = read_conf_string(&cfg, "device1.name", &device1_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "device1.kernel.url", "device1.kernel.path",
						 &device1_kernel);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device1.root.url", "device1.root.path",
						 &device1_root);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device1.home.url", "device1.home.path",
						 &device1_home);
		if (error != 0)
			goto error;
	}


	// device2
	error = read_conf_string(&cfg, "device2.name", &device2_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "device2.kernel.url", "device2.kernel.path",
						 &device2_kernel);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device2.root.url", "device2.root.path",
						 &device2_root);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device2.home.url", "device2.home.path",
						 &device2_home);
		if (error != 0)
			goto error;
	}

        // device3
	error = read_conf_string(&cfg, "device3.name", &device3_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "device3.kernel.url", "device3.kernel.path",
						 &device3_kernel);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device3.root.url", "device3.root.path",
						 &device3_root);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device3.home.url", "device3.home.path",
						 &device3_home);
		if (error != 0)
			goto error;
	}

	// device4
	error = read_conf_string(&cfg, "device4.name", &device4_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "device4.kernel.url", "device4.kernel.path",
						 &device4_kernel);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device4.root.url", "device4.root.path",
						 &device4_root);
		if (error != 0)
			goto error;

		error = read_conf_download_tupel(&cfg,
						 "device4.home.url", "device4.home.path",
						 &device4_home);
		if (error != 0)
			goto error;
	}



	// repo1
	error = read_conf_string(&cfg, "external.repo1.name", &repo1_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo1.url", "external.repo1.path",
						 &repo1);
		if (error != 0)
			goto error;
	}

	// repo2
	error = read_conf_string(&cfg, "external.repo2.name", &repo2_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo2.url", "external.repo2.path",
						 &repo2);
		if (error != 0)
			goto error;
	}

	// repo3
	error = read_conf_string(&cfg, "external.repo3.name", &repo3_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo3.url", "external.repo3.path",
						 &repo3);
		if (error != 0)
			goto error;
	}

	// repo4
	error = read_conf_string(&cfg, "external.repo4.name", &repo4_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo4.url", "external.repo4.path",
						 &repo4);
		if (error != 0)
			goto error;
	}

	// repo5
	error = read_conf_string(&cfg, "external.repo5.name", &repo5_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo5.url", "external.repo5.path",
						 &repo5);
		if (error != 0)
			goto error;
	}

	// repo6
	error = read_conf_string(&cfg, "external.repo6.name", &repo6_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo6.url", "external.repo6.path",
						 &repo6);
		if (error != 0)
			goto error;
	}

	// repo7
	error = read_conf_string(&cfg, "external.repo7.name", &repo7_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo7.url", "external.repo7.path",
						 &repo7);
		if (error != 0)
			goto error;
	}

	// repo8
	error = read_conf_string(&cfg, "external.repo8.name", &repo8_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo8.url", "external.repo8.path",
						 &repo8);
		if (error != 0)
			goto error;
	}

	// repo9
	error = read_conf_string(&cfg, "external.repo9.name", &repo9_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo9.url", "external.repo9.path",
						 &repo9);
		if (error != 0)
			goto error;
	}

	// repo10
	error = read_conf_string(&cfg, "external.repo10.name", &repo10_name);
	if (error == 0) {
		error = read_conf_download_tupel(&cfg,
						 "external.repo10.url", "external.repo10.path",
						 &repo10);
		if (error != 0)
			goto error;
	}

	config_destroy(&cfg);

	return 0;

error:
	config_destroy(&cfg);
	return -1;
}


void
show_config()
{
	putchar('\n');
	fprintf(stdout, _("Show content of internal configuration:  \n"));
	fprintf(stdout, _("---------------------------------------  \n"));
	fprintf(stdout, _("Global: conf_location->conf_file: %s     \n"),
		conf_location->conf_file);
	fprintf(stdout, _("Global: conf_location->conf_dir: %s      \n"),
		conf_location->conf_dir);
	fprintf(stdout, _("Global: name: %s                         \n"),
		name);
	fprintf(stdout, _("Global: gui_name: %s                     \n"),
		gui_name);

	// repo
	if (repo != NULL) {
		fprintf(stdout, _("Global: repo->url %s             \n"),
			repo->url);
		fprintf(stdout, _("Global: repo->path %s            \n"),
			repo->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo == NULL     \n"));
	}

	// toolchain
	if (toolchain != NULL) {
		fprintf(stdout, _("Global: toolchain->url %s        \n"),
			toolchain->url);
		fprintf(stdout, _("Global: toolchain->path %s       \n"),
			toolchain->path);
	} else {
		fprintf(stdout, _("Global: ERROR: toolchain == NULL \n"));
	}

	// host
	if (host != NULL) {
		fprintf(stdout, _("Global: host->url %s             \n"),
			host->url);
		fprintf(stdout, _("Global: host->path %s            \n"),
			host->path);
	} else {
		fprintf(stdout, _("Global: ERROR: host == NULL     \n"));
	}


	// device1
	fprintf(stdout, _("Global: device1_name: %s                 \n"),
		device1_name);
	if (device1_kernel != NULL) {
		fprintf(stdout, _("Global: device1_kernel->url %s   \n"),
			device1_kernel->url);
		fprintf(stdout, _("Global: device1_kernel->path %s  \n"),
			device1_kernel->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device1_kernel == NULL\n"));
	}
	if (device1_root != NULL) {
		fprintf(stdout, _("Global: device1_root->url %s     \n"),
			device1_root->url);
		fprintf(stdout, _("Global: device1_root->path %s    \n"),
			device1_root->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device1_root == NULL\n"));
	}
	if (device1_home != NULL) {
		fprintf(stdout, _("Global: device1_home->url %s     \n"),
			device1_home->url);
		fprintf(stdout, _("Global: device1_home->path %s    \n"),
			device1_home->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device1_home == NULL\n"));
	}

	// device2
	fprintf(stdout, _("Global: device2_name: %s                 \n"),
		device2_name);
	if (device2_kernel != NULL) {
		fprintf(stdout, _("Global: device2_kernel->url %s   \n"),
			device2_kernel->url);
		fprintf(stdout, _("Global: device2_kernel->path %s  \n"),
			device2_kernel->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device2_kernel == NULL\n"));
	}
	if (device2_root != NULL) {
		fprintf(stdout, _("Global: device2_root->url %s     \n"),
			device2_root->url);
		fprintf(stdout, _("Global: device2_root->path %s    \n"),
			device2_root->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device2_root == NULL\n"));
	}
	if (device2_home != NULL) {
		fprintf(stdout, _("Global: device2_home->url %s     \n"),
			device2_home->url);
		fprintf(stdout, _("Global: device2_home->path %s    \n"),
			device2_home->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device2_home == NULL\n"));
	}

	// device3
	fprintf(stdout, _("Global: device3_name: %s                 \n"),
		device3_name);
	if (device3_kernel != NULL) {
		fprintf(stdout, _("Global: device3_kernel->url %s   \n"),
			device3_kernel->url);
		fprintf(stdout, _("Global: device3_kernel->path %s  \n"),
			device3_kernel->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device3_kernel == NULL\n"));
	}
	if (device3_root != NULL) {
		fprintf(stdout, _("Global: device3_root->url %s     \n"),
			device3_root->url);
		fprintf(stdout, _("Global: device3_root->path %s    \n"),
			device3_root->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device3_root == NULL\n"));
	}
	if (device3_home != NULL) {
		fprintf(stdout, _("Global: device3_home->url %s     \n"),
			device3_home->url);
		fprintf(stdout, _("Global: device3_home->path %s    \n"),
			device3_home->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device3_home == NULL\n"));
	}

        // device4
	fprintf(stdout, _("Global: device4_name: %s                 \n"),
		device4_name);
	if (device4_kernel != NULL) {
		fprintf(stdout, _("Global: device4_kernel->url %s   \n"),
			device4_kernel->url);
		fprintf(stdout, _("Global: device4_kernel->path %s  \n"),
			device4_kernel->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device4_kernel == NULL\n"));
	}
	if (device4_root != NULL) {
		fprintf(stdout, _("Global: device4_root->url %s     \n"),
			device4_root->url);
		fprintf(stdout, _("Global: device4_root->path %s    \n"),
			device4_root->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device4_root == NULL\n"));
	}
	if (device4_home != NULL) {
		fprintf(stdout, _("Global: device4_home->url %s     \n"),
			device4_home->url);
		fprintf(stdout, _("Global: device4_home->path %s    \n"),
			device4_home->path);
	} else {
		fprintf(stdout, _("Global: ERROR: device4_home == NULL\n"));
	}

	// repo1
	fprintf(stdout, _("Global: repo1_name: %s                   \n"),
		repo1_name);
	if (repo1 != NULL) {
		fprintf(stdout, _("Global: repo1->url %s            \n"),
			repo1->url);
		fprintf(stdout, _("Global: repo1->path %s           \n"),
			repo1->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo1 == NULL     \n"));
	}

        // repo2
	fprintf(stdout, _("Global: repo2_name: %s                   \n"),
		repo2_name);
	if (repo2 != NULL) {
		fprintf(stdout, _("Global: repo2->url %s            \n"),
			repo2->url);
		fprintf(stdout, _("Global: repo2->path %s           \n"),
			repo2->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo2 == NULL     \n"));
	}

	// repo3
	fprintf(stdout, _("Global: repo3_name: %s                   \n"),
		repo3_name);
	if (repo3 != NULL) {
		fprintf(stdout, _("Global: repo3->url %s            \n"),
			repo3->url);
		fprintf(stdout, _("Global: repo3->path %s           \n"),
			repo3->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo3 == NULL     \n"));
	}

        // repo4
	fprintf(stdout, _("Global: repo4_name: %s                   \n"),
		repo4_name);
	if (repo4 != NULL) {
		fprintf(stdout, _("Global: repo4->url %s            \n"),
			repo4->url);
		fprintf(stdout, _("Global: repo4->path %s           \n"),
			repo4->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo4 == NULL     \n"));
	}

	// repo5
	fprintf(stdout, _("Global: repo5_name: %s                   \n"),
		repo5_name);
	if (repo5 != NULL) {
		fprintf(stdout, _("Global: repo5->url %s            \n"),
			repo5->url);
		fprintf(stdout, _("Global: repo5->path %s           \n"),
			repo5->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo5 == NULL     \n"));
	}

        // repo6
	fprintf(stdout, _("Global: repo6_name: %s                   \n"),
		repo6_name);
	if (repo6 != NULL) {
		fprintf(stdout, _("Global: repo6->url %s            \n"),
			repo6->url);
		fprintf(stdout, _("Global: repo6->path %s           \n"),
			repo6->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo6 == NULL     \n"));
	}

	// repo7
	fprintf(stdout, _("Global: repo7_name: %s                   \n"),
		repo7_name);
	if (repo7 != NULL) {
		fprintf(stdout, _("Global: repo7->url %s            \n"),
			repo7->url);
		fprintf(stdout, _("Global: repo7->path %s           \n"),
			repo7->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo7 == NULL     \n"));
	}

        // repo8
	fprintf(stdout, _("Global: repo8_name: %s                   \n"),
		repo8_name);
	if (repo8 != NULL) {
		fprintf(stdout, _("Global: repo8->url %s            \n"),
			repo8->url);
		fprintf(stdout, _("Global: repo8->path %s           \n"),
			repo8->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo8 == NULL     \n"));
	}

        // repo9
	fprintf(stdout, _("Global: repo9_name: %s                   \n"),
		repo9_name);
	if (repo9 != NULL) {
		fprintf(stdout, _("Global: repo9->url %s            \n"),
			repo9->url);
		fprintf(stdout, _("Global: repo9->path %s           \n"),
			repo9->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo9 == NULL     \n"));
	}

	// repo10
	fprintf(stdout, _("Global: repo10_name: %s                   \n"),
		repo10_name);
	if (repo10 != NULL) {
		fprintf(stdout, _("Global: repo10->url %s            \n"),
			repo10->url);
		fprintf(stdout, _("Global: repo10->path %s           \n"),
			repo10->path);
	} else {
		fprintf(stdout, _("Global: ERROR: repo10 == NULL     \n"));
	}


	fprintf(stdout, _("--------------------------------------- \n"));
	putchar('\n');
}


void *
init_sdk_workdir(void *args)
{
	PRINT_LOCATION();

	(void) args;

	write_to_textfield(_("--INFO_MSG--: in init_workdir\n"), INFO_MSG);

	return NULL;
}


void
check_sdk_workdir()
{

/*
 * check if /opt/..._sdk is available
 */
}
