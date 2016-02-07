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
init_main_config(char *conf_file)
{
	const char *str;
	config_t cfg;

	/*
	 * read config
	 */
/*	int cur_dir = open(".", O_RDONLY);
	if (cur_dir == -1)
		error_exit("can't open actual dir -> %s",
			   strerror(errno));

	if (chdir(conf_dir) == -1)
		error_exit("can't change to dir %s -> %s",
			   conf_dir,
			   strerror(errno));

        config_init(&cfg);
        if (config_read_file(&cfg, CONF_FILE) != CONFIG_TRUE)
        {
                fprintf(stderr, "%s:%d - %s\n",
                        config_error_file(&cfg),
                        config_error_line(&cfg),
                        config_error_text(&cfg));

                config_destroy(&cfg);
                exit(EXIT_FAILURE);
        }

        if (config_lookup_string(&cfg, "name", &str))
                fprintf(stdout, "config-file for %s\n", str);
        else
                error_exit("No 'name' setting in config file!");

		putchar('\n');*/


	//config_destroy(&cfg);

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

//do something

}
