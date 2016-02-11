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

/*
 * config specific
 */
// to check conf_file against (really simple check)
char *sdk_config_name;
// location of config dir/file
conf_path_t *conf_location;
// runtime*/workdir
char *workdir;
char *runtimedir;

/*
 * gui
 */
// example (a20_sdk) -> a20_sdk_builder
char *gui_name;

/*
 * *_sdk.git
 */
download_tupel_t *sdk_repo;

/*
 * toolchain -> cross-compiler and libs
 * host -> sysroot and host tools like mkimage
 */
download_tupel_t *toolchain;
download_tupel_t *host;

/*
 * devices (4 devices max)
 */
// example (a20_sdk) -> bananapi
char *device1_name;
download_tupel_t *device1_kernel;
download_tupel_t *device1_root;
download_tupel_t *device1_home;
device_tupel_t *device1;

// example (a20_sdk) -> bananapi_pro
char *device2_name;
download_tupel_t *device2_kernel;
download_tupel_t *device2_root;
download_tupel_t *device2_home;
device_tupel_t *device2;

// example (a20_sdk) -> cubietruck
char *device3_name;
download_tupel_t *device3_kernel;
download_tupel_t *device3_root;
download_tupel_t *device3_home;
device_tupel_t *device3;

// example (a20_sdk) -> olimex
char *device4_name;
download_tupel_t *device4_kernel;
download_tupel_t *device4_root;
download_tupel_t *device4_home;
device_tupel_t *device4;

/*
 * external repos (10 max)
 */
char *repo1_name;
download_tupel_t *repo1;
//repo_tupel_t *repo1;

char *repo2_name;
download_tupel_t *repo2;
//repo_tupel_t *repo2;

char *repo3_name;
download_tupel_t *repo3;
//repo_tupel_t *repo3;

char *repo4_name;
download_tupel_t *repo4;
//repo_tupel_t *repo4;

char *repo5_name;
download_tupel_t *repo5;
//repo_tupel_t *repo5;

char *repo6_name;
download_tupel_t *repo6;
//repo_tupel_t *repo6;

char *repo7_name;
download_tupel_t *repo7;
//repo_tupel_t *repo7;

char *repo8_name;
download_tupel_t *repo8;
//repo_tupel_t *repo8;

char *repo9_name;
download_tupel_t *repo9;
//repo_tupel_t *repo9;

char *repo10_name;
download_tupel_t *repo10;
//repo_tupel_t *repo10;


/*
 * kernel -> taken from /var/lib/..._sdk/..._env
 */
kernel_tupel_t *kernel;


#endif
