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
 * checks.c
 * ========
 */

void
check_sdk_git_path(void);

void
check_toolchain(void);

void
check_test_env(void);

int
check_sdk_workdir(void);

int
check_sdk_runtimedir(void);


/*
 * threads.c
 * =========
 */

// update (fetch) git repo
void *
update_sdk_repo(void *args);

// clone git repo
void *
clone_sdk_repo(void *args);

// dowload tarballs
void *
download_toolchain(void *args);

// start toolchain checks
void *
test_sdk(void *args);

// config
void *
config_sdk(void *args);

// help
void *
help(void *args);

// init sdk workdir (/opt)
void *
init_sdk_workdir(void *args);


#endif
