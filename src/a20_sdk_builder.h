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

#include "config.h"
#include "libservice.h"



// all widgets
GtkWidget *window;
GtkWidget *statusbar;
GtkWidget *textfield;


/*
 * gui.c
 * =========
 */

void
build_main_window();

GdkPixbuf *
create_pixbuf(const gchar *filename);

void
exit_button(GtkWidget *widget, gpointer data);

void
test_button(GtkWidget *widget, gpointer data);

void
init_button(GtkWidget *widget, gpointer data);

void
download_button(GtkWidget *widget, gpointer data);

void
clone_button(GtkWidget *widget, gpointer data);

void
new_menu(GtkWidget *widget, gpointer data);

void
open_menu(GtkWidget *widget, gpointer data);

void
save_menu(GtkWidget *widget, gpointer data);

void
save_as_menu(GtkWidget *widget, gpointer data);


#endif
