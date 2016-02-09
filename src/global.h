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

#include "common.h"


/*
 * global objects
 * --------------
 */

// all global widgets
GtkWidget *progressbar;
GtkWidget *progressbar_button;


#define COMMON_WORKDIR "/opt"

// git repo related
#define REPO_NAME "https://github.com/tjohann/a20_sdk.git"
#define SDK_GIT_PATH "/var/lib/a20_sdk"
// toolchain realted
#define TOOLCHAIN_NAME "http://sourceforge.net/projects/a20devices/files/"
#define TOOLCHAIN_PATH "/opt/a20_sdk"


/*
 * global configurations
 */

/*
 * config specific
 */
// to check conf_file against (really simple check)
char *name;
// location of config dir/file
conf_path_t *conf_location;

/*
 * gui
 */
// example (a20_sdk) -> a20_sdk_builder
char *gui_name;

/*
 * *_sdk.git
 */
download_tupel_t *repo;

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
char *name_device1;
download_tupel_t *device1_kernel;
download_tupel_t *device1_rootfs;
download_tupel_t *device1_home;
download_tupel_t *device1_image;

// example (a20_sdk) -> bananapi_pro
char *name_device2;
download_tupel_t *device2_kernel;
download_tupel_t *device2_rootfs;
download_tupel_t *device2_home;
download_tupel_t *device2_image;

// example (a20_sdk) -> cubietruck
char *name_device3;
download_tupel_t *device3_kernel;
download_tupel_t *device3_rootfs;
download_tupel_t *device3_home;
download_tupel_t *device3_image;

// example (a20_sdk) -> olimex
char *name_device4;
download_tupel_t *device4_kernel;
download_tupel_t *device4_rootfs;
download_tupel_t *device4_home;
download_tupel_t *device4_image;


#endif
