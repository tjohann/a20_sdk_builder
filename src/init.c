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


void
free_main_config(conf_obj_t *sdk_builder_config)
{
	if (sdk_builder_config != NULL) {

		/*
		 * free content
		 */

		free(sdk_builder_config);
	} else {
		fprintf(stdout, _("Nothing to free %s\n"), __FUNCTION__);
	}
}


conf_obj_t *
create_main_config()
{
	conf_obj_t *sdk_builder_config = NULL;

	sdk_builder_config = malloc(sizeof(conf_obj_t));
	if (sdk_builder_config == NULL)
		return NULL;

	memset(sdk_builder_config, 0, sizeof(conf_obj_t));

	return sdk_builder_config;
}


static conf_path_t *
fill_conf_dir_defaults()
{

        // do something


}


int
init_main_config(char *conf_file, char *conf_dir, conf_obj_t *sdk_builder_config)
{
	const char *str;
	config_t cfg;

	if (conf_file == NULL)
		return -1;

	if (conf_dir == NULL)
		sdk_builder_config->conf_location = fill_conf_dir_defaults;

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


int
check_init_state(conf_obj_t *sdk_builder_config)
{
	if (sdk_builder_config == NULL)
		return -1;
	else
		fprintf(stdout, "sdk_builder_config != NULL\n");

	if (sdk_builder_config->conf_location == NULL)
		return -1;
	else
		fprintf(stdout, "sdk_builder_config->conf_location != NULL\n");

	// TODO: check more of the structure
}
