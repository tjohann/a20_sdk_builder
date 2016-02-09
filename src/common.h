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

#ifndef _BUILDER_COMMON_H_
#define _BUILDER_COMMON_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <git2.h>
#include <git2/clone.h>

#include <curl/curl.h>

#include <archive.h>
#include <archive_entry.h>

#include <openssl/sha.h>

#include "libservice.h"

/*
 * global objects
 * --------------
 */

// --> see global.h


typedef enum message_types {
		NORMAL_MSG = 0x00,
		WARNING_MSG,
		ERROR_MSG,
		INFO_MSG,
		NONE
} message_types_t;


typedef enum progressbar_types {
		CLONE_BAR = 0x01,
		DOWNLOAD_BAR,
		UPDATE_BAR
} progressbar_types_t;


typedef enum gui_element {
		CLONE_B = 0x01,
		INIT_M,
		CLONE_M,
		DOWNLOAD_B,
		DOWNLOAD_M,
		UPDATE_B,
		UPDATE_M,
		TEST_B,
		HELP_B,
		OPEN_B,
		OPEN_M
} gui_element_t;


typedef enum download_types {
		DOWNLOAD_TOOLCHAIN = 0x00,
		DOWNLOAD_BANANAPI,
		DOWNLOAD_BANANAPI_PRO,
		DOWNLOAD_CUBIETRUCK,
		DOWNLOAD_OLIMEX,
		DOWNLOAD_ALL_IMAGES,
		DOWNLOAD_ALL,
} download_types_t;


typedef struct download_tupel {
	char *url;
	char *path;
} download_tupel_t;


typedef struct conf_path {
	char *conf_file;
	char *conf_dir;
} conf_path_t;


/*
 * global objects
 * --------------
 */

// --> see global.h


/*
 * common.h
 * ========
 */
#define PRINT_LOCATION() do {						\
		g_print(_("Your're in %s of %s\n"),			\
			__FUNCTION__,					\
			__FILE__);					\
	} while (0)


#define git_error_handling() do {					\
		const git_error *err = giterr_last();			\
									\
		if (err) {						\
			fprintf(stderr,					\
				_("ERROR %d: %s\n"),			\
				err->klass,				\
				err->message);				\
			write_to_textfield(err->message, ERROR_MSG);	\
			write_to_textfield("\n", NORMAL_MSG);		\
		} else {						\
			fprintf(stderr,					\
				_("ERROR %d: no detailed info\n"),	\
				error);					\
			write_to_textfield(_("Unclassified error occured\n"), \
					   ERROR_MSG);			\
		}							\
		goto out;						\
	} while (0)


#define ERROR_MSG_AND_RETURN(name) do {		\
		error_msg(name);		\
		return -1;			\
	} while (0)


/*
 * gui.c
 * =====
 */

void
show_gtk_version_info(void);

void
build_main_window();

void
write_to_textfield(const char *message, message_types_t type);

int
create_progress_bar_window(unsigned char progressbar_type);

void
set_progressbar_value(int statusbar_percent, char *statusbar_percent_string);

GdkPixbuf *
create_pixbuf(const gchar *filename);

void
lock_button(gui_element_t button);

void
unlock_button(gui_element_t button);

int
get_state_of_gui_element(gui_element_t button);

void
enter_sdk_thread(void);

void
leave_sdk_thread(void);


/*
 * main.c
 * ======
 */

void
exit_function(GtkWidget *widget, gpointer data);


/*
 * error.c
 * =======
 */

// print error message and exit
void
__attribute__((noreturn)) error_exit(const char *fmt, ...);

void
error_msg(const char *fmt, ...);


/*
 * init.c
 * ======
 */

int
init_main_config(char *conf_file, char *conf_dir);

void *
init_sdk_workdir(void *args);

void
check_sdk_workdir(void);


/*
 * clone.c
 * =======
 */

// clone a20_sdk.git
void *
clone_sdk_repo(void *args);


/*
 * common_git.c
 * ==========
 */

int
fetch_progress(const git_transfer_progress *stats, void *payload);

int
sideband_progress(const char *str, int len, void *payload);

void
check_sdk_git_path(void);


/*
 * update.c
 * ========
 */

// update (git pull) a20_sdk.git
void *
update_sdk_repo(void *args);


/*
 * download.c
 * ==========
 */

void *
download_toolchain(void *args);

void
check_toolchain(void);


/*
 * extract.c
 * ==========
 */

void
extract_toolchain(char *filename);


/*
 * test.c
 * ======
 */

void *
test_sdk(void *args);

void
check_test_env(void);


/*
 * config.c
 * ========
 */

void *
config_sdk(void *args);


/*
 * help.c
 * ======
 */

void *
help(void *args);

#endif
