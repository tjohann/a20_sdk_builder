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



/*
 * common macros
 * -------------
 */
#define PRINT_LOCATION() {						\
		g_print(_("Your're in %s of %s\n"),			\
			__FUNCTION__,					\
			__FILE__);					\
	}

#define UNLOCK_UPDATE_BUTTON() { gtk_widget_set_sensitive(update_b, TRUE); }
#define LOCK_UPDATE_BUTTON() { gtk_widget_set_sensitive(update_b, FALSE); }

#define UNLOCK_CLONE_BUTTON() { gtk_widget_set_sensitive(clone_b, TRUE); }
#define LOCK_CLONE_BUTTON() { gtk_widget_set_sensitive(clone_b, FALSE); }

#define UNLOCK_DOWNLOAD_BUTTON() { gtk_widget_set_sensitive(download_b, TRUE); }
#define LOCK_DOWNLOAD_BUTTON() { gtk_widget_set_sensitive(download_b, FALSE); }


/*
 * not only for buttons, also for menu entrys
 */
#define UNLOCK_BUTTONS() {						\
		gtk_widget_set_sensitive(update_b, TRUE);		\
		gtk_widget_set_sensitive(download_b, TRUE);		\
		gtk_widget_set_sensitive(clone_b, TRUE);		\
		gtk_widget_set_sensitive(save_m, TRUE);			\
		gtk_widget_set_sensitive(save_as_m, TRUE);		\
	}


/*
 * not only for buttons, also for menu entrys
 */
#define LOCK_BUTTONS() {						\
		gtk_widget_set_sensitive(download_b, FALSE);		\
		gtk_widget_set_sensitive(clone_b, FALSE);		\
		gtk_widget_set_sensitive(save_m, FALSE);		\
		gtk_widget_set_sensitive(save_as_m, FALSE);		\
	}


#define UNLOCK_PROGRESS_CLONE_BUTTON() {				\
		gtk_widget_set_sensitive(progressbar_button, TRUE);	\
	}


#define LOCK_PROGRESS_CLONE_BUTTON() {					\
		gtk_widget_set_sensitive(progressbar_button, FALSE);	\
	}


/*
 * add for pulsing changes instead of continous growing
 * -> 	gtk_progress_bar_pulse(GTK_PROGRESS_BAR(progressbar));
 */
#define SET_PROGRESSBAR_VALUE() {					\
		gtk_progress_set_value(GTK_PROGRESS(progressbar),	\
				       statusbar_percent);		\
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), \
					  statusbar_percent_string);	\
	}


#define SET_PROGRESSBAR_0() {						\
		gtk_progress_set_value(GTK_PROGRESS(progressbar), 0);	\
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), \
					  "0%");			\
	}


#define SET_PROGRESSBAR_100() {						\
		gtk_progress_set_value(GTK_PROGRESS(progressbar), 100);	\
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), \
					  "100%");			\
	}


/*
  gdk_threads_enter();
  gdk_threads_leave();
 */


/*
 * gui.c
 * =====
 */

void
build_main_window();

void
write_to_textfield(char *message, message_types_t type);

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
void
update_sdk_repo(void);


/*
 * download.c
 * ==========
 */

void
download_toolchain();


#endif
