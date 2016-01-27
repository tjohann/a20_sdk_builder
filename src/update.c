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

#include "common.h"



void
update_sdk_repo()
{
	PRINT_LOCATION();

	write_to_textfield(_("--NORMAL_MSG--: in update_sdk_repo\n"), NORMAL_MSG);
	write_to_textfield(_("--WARNING_MSG--: in update_sdk_repo\n"), WARNING_MSG);
	write_to_textfield(_("--ERROR_MSG--: in update_sdk_repo\n"), ERROR_MSG);
	write_to_textfield(_("--INFO_MSG--: in update_sdk_repo\n"), INFO_MSG);
}
