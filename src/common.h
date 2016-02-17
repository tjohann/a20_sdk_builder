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


// attribute for write_to_textfield
typedef enum message_types {
		NORMAL_MSG = 0x00,
		WARNING_MSG,
		ERROR_MSG,
		INFO_MSG,
		NONE
} message_types_t;


/*
 * at least for a common naming scheme
 * CLONE_B -> clone button
 * CLONE_M -> clone menutentry
 */
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
		OPEN_M,
		NEW_M,
		SAVE_M,
		SAVE_AS_M,
		PARTITION_M,
		HOME_M,
		ROOT_M,
		KERNEL_M,
		UBOOT_M,
		DO_ALL_M,
		CLONE_ALL_M,
	        EXTERNAL1_M,
		EXTERNAL2_M,
		EXTERNAL3_M,
		EXTERNAL4_M,
		EXTERNAL5_M,
		EXTERNAL6_M,
		EXTERNAL7_M,
		EXTERNAL8_M,
		EXTERNAL9_M,
		EXTERNAL10_M,
		CLONE_SELECTED_M,
		PROGRESSBAR_B
} gui_element_t;


/*
 * common.h
 * ========
 */
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
create_progressbar_window(char *name);

void
set_progressbar_window_title(char *title);

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

// lock unused buttons/menus -> not yet implemented
void
lock_unused_buttons();


/*
 * main.c
 * ======
 */

void
exit_function(GtkWidget *widget, gpointer data);


/*
 * message_handler.c
 * =================
 */
// print error message and exit
void
__attribute__((noreturn)) write_error_msg_exit(const char *fmt, ...);

// print error message
void
write_error_msg(const char *fmt, ...);

// print error message and return
void
write_error_msg_return(const char *fmt, ...);

// print info message
void
write_info_msg(const char *fmt, ...);

// print info message and return
void
write_info_msg_return(const char *fmt, ...);

// print debug message
void
write_debug_msg(const char *fmt, ...);

// print debug message and return
void
write_debug_msg_return(const char *fmt, ...);

// print warning message
void
write_warning_msg(const char *fmt, ...);

// print warning message and return
void
write_warning_msg_return(const char *fmt, ...);


/*
 * init.c
 * ======
 */

void *
init_sdk_workdir(void *args);

int
check_sdk_workdir(void);

int
check_sdk_runtimedir(void);


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

int
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
