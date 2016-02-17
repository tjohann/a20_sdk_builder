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

#ifndef _LIBSERVICE_H_
#define _LIBSERVICE_H_

// more or less common inc
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>
#include <wait.h>
#include <limits.h>
#include <linux/limits.h>
#include <sysexits.h>
#include <ctype.h>
#include <pthread.h>
#include <bsd/stdlib.h>

// time related inc
#include <time.h>
#include <sys/times.h>

// inotify inc
#include <sys/inotify.h>

// more or less network related inc
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netpacket/packet.h>

// libconfig
#include <libconfig.h>

// autotools genrated config
#include <config.h>

// getopt and locale realted inc
#include <getopt.h>
#include <libintl.h>
#include <locale.h>
#include "gettext.h"


/*
 * common defines
 * -------------
 */
#define MAXLINE 254
#define MAX_SUPPORTED_DEVICES 4
#define MAX_SUPPORTED_EXTERNAL_REPOS 10

#define TMP_DIR "/tmp/"
#define TMP_FILE "/tmp/sdk_builder.trash"

#define DUMMY_STRING "dummy"


/*
 * common types
 * -------------
 */

// dowload/clone url to path
typedef struct download_tupel {
	char *url;
	char *path;
} download_tupel_t;


// represent a device like Bananapi or Olimex
typedef struct device_tupel {
	char *name;
	download_tupel_t *kernel;
	download_tupel_t *root;
	download_tupel_t *home;
} device_tupel_t;


// represent a externel repo like u-boot or linux
typedef struct repo_tupel {
	char *name;
	download_tupel_t *repo;
} repo_tupel_t;


// represent the kernel parts
typedef struct kernel_tupel {
	download_tupel_t *non_rt_kernel;
	download_tupel_t *rt_kernel;
	download_tupel_t *rt_patch;
} kernel_tupel_t;


/*
 * configuration dir and configuration file
 * -> conf_dir = /etc/sdk_builder
 * -> conf_file = a20_sdk_builder.conf
 */
typedef struct conf_path {
	char *conf_file;
	char *conf_dir;
} conf_path_t;


/*
 * common macros
 * -------------
 */
#define _(string) gettext(string)

#define PRINT_LOCATION() do {			      \
		info_msg(_("Your're in %s of %s"),    \
			 __FUNCTION__, __FILE__);     \
	} while (0)


/*
 * helper.c
 * ========
 */

void
show_version_info(void);

void
show_package_name(void);

void
show_program_name(char *program_name);

void
set_program_name(char **program_name, char *kdo_arg);

bool
is_this_a_dir(const char *dir_name);

char *
alloc_string(const char *tmp_str);


/*
 * network.c
 * =========
 */

int
init_network(void);


/*
 * error.c
 * =======
 */

/*
 * +------------------+------------+------------+--------------------------+
 * |     function     | use errno? | terminate? | log_level (man 3 syslog) |
 * +------------------+------------+------------+--------------------------+
 * | error_exit       |     yes    |   exit()   |         LOG_ERR          |
 * | dump_exit        |     yes    |  abort()   |         LOG_ERR          |
 * | error_msg        |     yes    |    no      |         LOG_ERR          |
 * | error_msg_return |     no     |   return   |         LOG_ERR          |
 * | info_msg         |     no     |    no      |         LOG_INFO         |
 * | info_msg_return  |     no     |   return   |         LOG_INFO         |
 * | debug_msg        |     no     |    no      |         LOG_DEBUG        |
 * | debug_msg_return |     no     |    no      |         LOG_DEBUG        |
 * +------------------+------------+------------+--------------------------+
 */

// print error message and exit
void
__attribute__((noreturn)) error_exit(const char *fmt, ...);

// print error message and dump/exit
void
__attribute__((noreturn)) dump_exit(const char *fmt, ...);

// print error message
void
error_msg(const char *fmt, ...);

// print error message and return
void
error_msg_return(const char *fmt, ...);

// print info message
void
info_msg(const char *fmt, ...);

// print info message and return
void
info_msg_return(const char *fmt, ...);

// print debug message
void
debug_msg(const char *fmt, ...);

// print debug message and return
void
debug_msg_return(const char *fmt, ...);


/*
 * config.c
 * ========
 */

int
set_conf_location(char *conf_file, char *conf_dir);

char *
get_conf_location_dir();

char *
get_conf_location_file();

int
init_main_config(char *conf_file, char *conf_dir);

int
init_common_config();

int
init_repo_config();

int
init_toolchain_config();

int
init_device_config();

int
init_external_config();

int
init_kernel_config();

void
show_config();

/*
 * Scheme:
 *
 * get/set_STRUCTURE_VARIABLE  -> sdk_repo.url -> get_sdk_repo_url()
 */

/*
 * common
 */
char *
get_common_gui_name();

char *
get_common_workdir();

char *
get_common_runtimedir();

/*
 * sdk_repo
 */
char *
get_sdk_repo_url();

char *
get_sdk_repo_path();


/*
 * toolchain
 */
char *
get_toolchain_url();

char *
get_toolchain_path();

download_tupel_t *
get_toolchain();

download_tupel_t *
get_host();


/*
 * download_tupel_t
 */
char *
get_download_tupel_url(download_tupel_t *t);

char *
get_download_tupel_path(download_tupel_t *t);



#endif
