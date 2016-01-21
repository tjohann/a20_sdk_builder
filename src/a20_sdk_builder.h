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

GtkWidget *progessbar;
GtkWidget *progessbar_window, *progessbar_frame, *progressbar_frame2;
GtkAdjustment *progressbar_adj;

GtkWidget *clone_b;
GtkWidget *update_b;
GtkWidget *download_b;

GtkWidget *save_m;
GtkWidget *save_as_m;


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



/*
 * gui.c
 * =========
 */

void
build_main_window();

GdkPixbuf *
create_pixbuf(const gchar *filename);


/*
 * a20_sdk_builder.c
 * =========
 */

// clone a20_sdk.git
void *
clone_sdk_repo(void);

void
update_sdk_repo(void);

void
download_button(GtkWidget *widget, gpointer data);

void
test_button(GtkWidget *widget, gpointer data);

void
new_config(GtkWidget *widget, gpointer data);

void
exit_function(GtkWidget *widget, gpointer data);

void
open_menu(GtkWidget *widget, gpointer data);

void
save_menu(GtkWidget *widget, gpointer data);

void
save_as_menu(GtkWidget *widget, gpointer data);



#endif
