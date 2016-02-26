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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "libservice.h"


/*
 * global configurations
 */

/* location of config dir/file -> not part of conf file! */
conf_path_t *conf_location;

/* to check conf_file against (really simple check) */
char *sdk_config_name;

char *gui_name;
char *workdir;
char *runtimedir;

download_tupel_t *sdk_repo;
download_tupel_t *toolchain;
download_tupel_t *host;

/* deviceX */
device_tupel_t *device_array[MAX_SUPPORTED_DEVICES + 1];

/* external */
repo_tupel_t *repo_array[MAX_SUPPORTED_EXTERNAL_REPOS + 1];

/* kernel -> taken from /var/lib/..._sdk/..._env  */
kernel_tupel_t *kernel;

/* content of checksum.sha256 */
checksum_tupel_t *checksum_array[MAX_LEN_CHECKSUM_ARRAY];

#endif
