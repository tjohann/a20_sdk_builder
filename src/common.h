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

#ifndef _A20SDKBUILDER_H_
#define _A20SDKBUILDER_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <git2.h>
#include <git2/clone.h>

#include "config.h"
#include "libservice.h"


// all global widgets
GtkWidget *progressbar;
GtkWidget *progressbar_button;


/*
 * common locations
 * ----------------
 */
#define REPO_NAME "https://github.com/tjohann/a20_sdk.git"
#define SDK_GIT_PATH "/var/lib/a20_sdk"


/*
 * common defines
 * -------------
 */


/*
 * common types
 * -------------
 */
typedef enum message_types {
		NORMAL_MSG  = 0x00,
		WARNING_MSG = 0x01,
		ERROR_MSG   = 0x02,
		INFO_MSG    = 0x03
} message_types_t;


typedef enum progressbar_types {
		CLONE_BAR    = 0x00,
		DOWNLOAD_BAR = 0x01
} progressbar_types_t;


/*
 * common macros
 * -------------
 */
#define PRINT_LOCATION() {						\
		g_print(_("Your're in %s of %s\n"),			\
			__FUNCTION__,					\
			__FILE__);					\
	}


#define GIT_ERROR_HANDLING() {					\
		const git_error *err = giterr_last();		\
								\
		if (err) {						\
			fprintf(stderr,					\
				"ERROR %d: %s\n",			\
				err->klass,				\
				err->message);				\
			write_to_textfield(err->message, ERROR_MSG);	\
			write_to_textfield("\n", NORMAL_MSG);		\
		} else {						\
			fprintf(stderr,					\
				"ERROR %d: no detailed info\n",		\
				error);					\
			write_to_textfield("Unclassified error occured\n", \
					   ERROR_MSG);			\
		}							\
		goto out;						\
	}


/*
 * gui.c
 * =====
 */

void
build_main_window();

void
write_to_textfield(char *message, message_types_t type);

void
set_progressbar_value(int statusbar_percent, char *statusbar_percent_string);

GdkPixbuf *
create_pixbuf(const gchar *filename);



/*
 * main.c
 * ======
 */

void
exit_function(GtkWidget *widget, gpointer data);


/*
 * clone.c
 * =======
 */

// clone a20_sdk.git
void *
clone_sdk_repo(void *args);


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


/*
 * test.c
 * ==========
 */

void *
test_sdk(void *args);


/*
 * config.c
 * ==========
 */

void *
config_sdk(void *args);


/*
 * help.c
 * ==========
 */

void *
help(void *args);

#endif
