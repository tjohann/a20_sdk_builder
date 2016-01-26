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
GtkWidget *window;
GtkWidget *statusbar;
GtkWidget *textfield;

GtkWidget *clone_b;
GtkWidget *update_b;
GtkWidget *download_b;

GtkWidget *save_m;
GtkWidget *save_as_m;

GtkWidget *progressbar;
GtkWidget *progressbar_button;



/*
 * common defines
 * -------------
 */
#define CLONE_BAR 1
#define DOWNLOAD_BAR 2


/*
 * common types
 * -------------
 */



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
		gdk_threads_enter();					\
		gtk_progress_set_value(GTK_PROGRESS(progressbar),	\
				       statusbar_percent);		\
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), \
					  statusbar_percent_string);	\
		gdk_threads_leave();					\
	}


#define SET_PROGRESSBAR_0() {						\
		gdk_threads_enter();					\
		gtk_progress_set_value(GTK_PROGRESS(progressbar), 0);	\
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), \
					  "0%");			\
		gdk_threads_leave();					\
	}


#define SET_PROGRESSBAR_100() {						\
		gdk_threads_enter();					\
		gtk_progress_set_value(GTK_PROGRESS(progressbar), 100);	\
		gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progressbar), \
					  "100%");			\
		gdk_threads_leave();					\
	}



/*
 * gui.c
 * =========
 */

void
build_main_window();

GdkPixbuf *
create_pixbuf(const gchar *filename);

void
download_button(GtkWidget *widget, gpointer data);

void
test_button(GtkWidget *widget, gpointer data);

/*
 * For button and menu entry
 */
void
new_config(GtkWidget *widget, gpointer data);

void
open_menu(GtkWidget *widget, gpointer data);

void
save_menu(GtkWidget *widget, gpointer data);

void
save_as_menu(GtkWidget *widget, gpointer data);


/*
 * a20_sdk_builder.c
 * =========
 */

// clone a20_sdk.git
void *
clone_sdk_repo(void *args);

// update (git pull) a20_sdk.git
void
update_sdk_repo(void);

void
download_toolchain();

void
exit_function(GtkWidget *widget, gpointer data);


#endif
