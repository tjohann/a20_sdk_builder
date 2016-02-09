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


static int
fill_conf_location(char *conf_file, char *conf_dir)
{
	conf_path_t *tmp = NULL;
	char *conf_f = NULL;
	char *conf_d = NULL;
	size_t len = 0;

	tmp = malloc(sizeof(conf_path_t));
	if (tmp == NULL)
		return -1;

	memset(tmp, 0, sizeof(conf_path_t));

	if (conf_dir == NULL) {
		struct stat sb;
		memset(&sb, 0, sizeof(struct stat));

		if (stat("/etc/sdk_builder", &sb) == 0 && S_ISDIR(sb.st_mode))
			printf("is dir\n");
		else {
			g_print("no dir\n");

			memset(&sb, 0, sizeof(struct stat));

			if (stat("/usr/local/etc/sdk_builder", &sb) == 0 && S_ISDIR(sb.st_mode))
				printf("is dir\n");
			else
				printf("no dir\n");
		}

	} else {
		len = strlen(conf_dir) + 1;
		conf_d = malloc(len);
		if (conf_d == NULL)
			goto error;
	
		memset(conf_d, 0, len);
		strncat(conf_d, conf_dir, len);
	}

	len = strlen(conf_file) + 1;
	conf_f = malloc(len);
	if (conf_f == NULL)
		goto error;
	
	memset(conf_f, 0, len);
	strncat(conf_f, conf_file, len);

	
	tmp->conf_file = conf_f;
	tmp->conf_dir = conf_d;
	conf_location = tmp;

	g_print(_("conf_f %s\n"), tmp->conf_file);
	g_print(_("conf_d %s\n"), tmp->conf_dir);
	
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


int
init_main_config(char *conf_file, char *conf_dir)
{
	const char *str;
	config_t cfg;

	if (conf_file == NULL)
		return -1;

	int cur_dir = open(".", O_RDONLY);
	if (cur_dir == -1)
		error_exit(_("can't open actual dir -> %s\n"), strerror(errno));

	int error = fill_conf_location(conf_file, conf_dir);
	if (error == -1)
		return -1;
	else
		g_print(_("conf_location->conf_dir: %s\n"), conf_location->conf_dir);


	
	//conf_dir = "/etc/sdk_builder/";
	/*
	 * read config
	 */
	//int cur_dir = open(".", O_RDONLY);
	//if (cur_dir == -1)
	//	error_exit("can't open actual dir -> %s", strerror(errno));

	//if (chdir(conf_dir) == -1)
	//	error_exit("can't change to dir %s -> %s",
	//		   conf_dir,
	//		   strerror(errno));

        //config_init(&cfg);
        //if (config_read_file(&cfg, conf_file) != CONFIG_TRUE)
        //{
        //        fprintf(stderr, "%s:%d - %s\n",
        //                config_error_file(&cfg),
        //                config_error_line(&cfg),
        //                config_error_text(&cfg));

        //        config_destroy(&cfg);
        //        exit(EXIT_FAILURE);
        //}

        //if (config_lookup_string(&cfg, "name", &str))
        //        fprintf(stdout, "config-file for %s\n", str);
        //else
        //        error_exit("No 'name' setting in config file!");


	/*
	 * ...
	 */


	//config_destroy(&cfg);

	return 0;
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
 * check if /opt/*_sdk is available
 */
}
