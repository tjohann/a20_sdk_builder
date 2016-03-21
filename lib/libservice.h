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

#ifndef __USE_GNU
#define __USE_GNU
#endif
#define _GNU_SOURCE

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
#include <sys/utsname.h>
#include <sched.h>
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

// autotools generated config
#include <config.h>

// getopt and locale realted inc
#include <getopt.h>
#include <libintl.h>
#include <locale.h>
#include "gettext.h"

// libgit2
#include <git2.h>
#include <git2/clone.h>

// libcurl
#include <curl/curl.h>

// libarchive
#include <archive.h>
#include <archive_entry.h>

// libressl
#include <openssl/sha.h>



/*
 * common defines
 * -------------
 */
#define MAXLINE 254
// toolchain_x86_64.tgz + host_86_64.tgz
#define MAX_COUNT_TOOLCHAIN 2
// kernel, root, home
#define MAX_COUNT_DEVICES 3
#define MAX_SUPPORTED_DEVICES 4
#define MAX_SUPPORTED_EXTERNAL_REPOS 10
#define MAX_LEN_CHECKSUM_ARRAY (MAX_SUPPORTED_DEVICES * MAX_COUNT_DEVICES + MAX_COUNT_TOOLCHAIN + 1)

#define NAME_CHECKSUM_FILE "checksum.sha256"
#define DUMMY_STRING "dummy"
#define FILE_EMPTY -2

#define VAR_RUN_DIR "/var/run"
#define TMP_DIR "/tmp"
#define TMP_FILE "/tmp/sdk_builder.trash"

// running modes -> normale application in foreground or as daemon in background
#define RUN_AS_APPLICATION 0
#define RUN_AS_DAEMON      1


/*
 * common types
 * -------------
 */
// shortcut for old signal api (signal_old())
typedef	void sigfunc(int);

// dowload/clone url to path
typedef struct download_tupel {
	char *url;
	char *path;
	char *checksum_s;     // sha256sum as string
	unsigned char *hash;  // sha256sum
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


// represent content of one line of checksum.sha256
typedef struct checksum_tupel {
	char *name;
	char *checksum_s;     // sha256sum as string
} checksum_tupel_t;


/*
 * common macros
 * -------------
 */
#define _(string) gettext(string)

#define PRINT_LOCATION() do {			      \
		info_msg(_("Your're in %s of %s"),    \
			 __FUNCTION__, __FILE__);     \
	} while (0)


#define git_error_handling() do {					\
		const git_error *err = giterr_last();			\
									\
		if (err) {						\
			write_error_msg(_("GIT: %d: %s"), err->klass,	\
					err->message);			\
		} else {						\
			write_error_msg(_("GIT: %d: no detailed info"), \
					error);				\
		}							\
		goto out;						\
	} while (0)


// TODO: remove EXTERN -> use callbacks and/or sockets ...

/*
 * EXTERN functions
 * ================
 */
extern void
set_progressbar_value(int statusbar_percent, char *statusbar_percent_string);

extern void
set_progressbar_window_title(char *title);

// print info message
extern void
write_info_msg(const char *fmt, ...);

// print error message
void
write_error_msg(const char *fmt, ...);


/*
 * helper.c
 * ========
 */

void
show_version_info(void);

// based on config.h
void
show_package_name(void);

void
show_program_name(char *program_name);

void
set_program_name(char **program_name, char *kdo_arg);

bool
is_this_a_dir(const char *dir_name);

// returns a heap object with content of tmp_str
char *
alloc_string(const char *tmp_str);

// read a line of fd into buf for max n_bytes
ssize_t
read_line(int fd, void *buf, size_t n_bytes);

// link words of line into word_array[x] for max n_words
size_t
read_words(char *line, char *word_array[], size_t n_words);

// calc hash and checksum string
int
calc_checksum(download_tupel_t *t);

// print content of a checksum tupel
void
print_checksum_tupel(checksum_tupel_t *c);

// i like the old signal api
sigfunc *
signal_old(int signo, sigfunc *func);

// become a daemon
int
become_daemon(void);

// create pid file for a dameon
char *
create_daemon_pidfile();

// set close-on-exec for fd
int
set_cloexec(int fd);

// lock a region in fd
int
lock_region(int fd);



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
 * +---------------------+------------+------------+--------------------------+
 * |     function        | use errno? | terminate? | log_level (man 3 syslog) |
 * +---------------------+------------+------------+--------------------------+
 * | error_exit          |     yes    |   exit()   |         LOG_ERR          |
 * | dump_exit           |     yes    |  abort()   |         LOG_ERR          |
 * | error_msg           |     yes    |    no      |         LOG_ERR          |
 * | info_msg            |     no     |    no      |         LOG_INFO         |
 * | debug_msg           |     no     |    no      |         LOG_DEBUG        |
 * +---------------------+------------+------------+--------------------------+
 * | th_error_msg        | errno_val  |    no      |         LOG_ERR          |
 * | th_error_exit       | errno_val  |   exit()   |         LOG_ERR          |
 * | th_dump_exit        | errno_val  |  abourt()  |         LOG_ERR          |
 * +---------------------+------------+------------+--------------------------+
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

// print info message
void
info_msg(const char *fmt, ...);

// print debug message
void
debug_msg(const char *fmt, ...);

// print error message with errno = errno_val
void
th_error_msg(int errno_val, const char *fmt, ...);

// print error message with errno = errno_val and dump/exit
void
__attribute__((noreturn)) th_dump_exit(int errno_val, const char *fmt, ...);

// print error message with errno = errno_val and exit
void
__attribute__((noreturn)) th_error_exit(int errno_val, const char *fmt, ...);

// enable logging via syslog
void
enable_syslog(bool use_it);



/*
 * config.c
 * ========
 */

// Note: first set conf file name and directory before init_*
int
set_conf_location(char *conf_file, char *conf_dir);

char *
get_conf_location_dir();

char *
get_conf_location_file();

// fill checksum_tupel with content of $RUNTIMEDIR/$NAME_CHECKSUM_FILE
int
read_checksum_file(void);

// check if at least checksum_array[0] != NULL
bool
is_checksum_array_valid();


// Note: must be called before the other init_*_config functions
int
init_main_config(char *conf_file, char *conf_dir);

// init globals based on common: { ... } entry of *.conf
int
init_common_config();

// init globals based on repo: { ... } entry of *.conf
int
init_repo_config();

// init globals based on toolchain: { ... } entry of *.conf
int
init_toolchain_config();

// init globals based on device: { ... } entry of *.conf
int
init_device_config();

// init globals based on external: { ... } entry of *.conf
int
init_external_config();

// init globals based on runtimedir/armhf.env entry
int
init_kernel_config();

// print all globals to stdout
void
show_config();

/*
 * Scheme of config file handling
 *
 * get/set_STRUCTURE_VARIABLE  -> sdk_repo.url -> get_sdk_repo_url()
 */
char *
get_common_gui_name();

char *
get_common_workdir();

char *
get_common_runtimedir();

char *
get_sdk_repo_url();

char *
get_sdk_repo_path();

char *
get_toolchain_url();

char *
get_toolchain_path();

download_tupel_t *
get_toolchain_tupel();

download_tupel_t *
get_host_tupel();

/*
 * get type specific content
 */
char *
get_download_tupel_url(download_tupel_t *t);

char *
get_download_tupel_path(download_tupel_t *t);

checksum_tupel_t *
get_checksum_tupel(char *name);



/*
 * git.c
 * =====
 */

void
do_clone_repo(char *url, char *path);

void
do_update_repo(char *url, char *path);


/*
 * download.c
 * ==========
 */
int
do_download(download_tupel_t *download_array[]);


/*
 * extract.c
 * =========
 */
int
extract_toolchain(char *filename);

#endif
