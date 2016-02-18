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


void *
init_sdk_workdir(void *args)
{
	PRINT_LOCATION();

	(void) args;

	write_info_msg(_("--INFO_MSG--: in init_workdir"));

	if (check_sdk_workdir() == 0)
		write_error_msg("%s is available", get_common_workdir());
	else {
		write_error_msg("%s isn't available, create it",
				get_common_workdir());
		// TODO: create workdir
	}

	if (check_sdk_runtimedir() == 0)
		write_error_msg("%s is available", get_common_runtimedir());
	else {
		write_error_msg("%s isn't available, create it",
				get_common_runtimedir());
		// TODO: create runtimedir
	}

	return NULL;
}


int
check_sdk_workdir()
{

/*
 * check if /opt/..._sdk is available
 */


	// check for workdir
	return 0;
}


int
check_sdk_runtimedir()
{

/*
 * check if /var/lib..._sdk is available
 * Note: to check if it's a git repo use check_sdk_git_path()
 */


	// check runtimedir!
	return 0;
}
