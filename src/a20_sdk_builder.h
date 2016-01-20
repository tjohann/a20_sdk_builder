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
GtkWidget *download_b;

GtkWidget *save_m;
GtkWidget *save_as_m;


/*
 * common types
 * -------------
 */
typedef struct progress_data {
	git_transfer_progress fetch_progress;
	size_t completed_steps;
	size_t total_steps;
	const char *path;
} progress_data;


/*
 * common macros
 * -------------
 */
#define PRINT_LOCATION() { g_print(_("Your're in %s of %s\n"), __FUNCTION__, __FILE__);  }


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


void
clone_button(GtkWidget *widget, gpointer data);

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
