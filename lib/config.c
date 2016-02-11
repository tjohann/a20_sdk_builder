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


/*
 * common
 */
char *
get_common_gui_name()
{
	return gui_name;
}

char *
get_common_workdir()
{
	return workdir;
}

char *
get_common_runtimedir()
{
	return runtimedir;
}

/*
 * sdk_repo
 */
char *
get_sdk_repo_url()
{
	return sdk_repo->url;
}

char *
get_sdk_repo_path()
{
	return sdk_repo->path;
}

/*
 * toolchain
 */
char *
get_toolchain_url()
{
	return toolchain->url;
}

char *
get_toolchain_path()
{
	return toolchain->path;
}

download_tupel_t *
get_toolchain()
{
	return toolchain;
}


/*
 * host
 */
char *
get_host_url()
{
	return host->url;
}

char *
get_host_path()
{
	return host->path;
}

download_tupel_t *
get_host()
{
	return host;
}




/*
 * download_tupel_t
 */
char *
get_download_tupel_url(download_tupel_t *t)
{
	return t->url;
}

char *
get_download_tupel_path(download_tupel_t *t)
{
	return t->path;
}
