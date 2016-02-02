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


void *
test_sdk(void *args)
{

	(void) args;

	/*
	 * only one thread could be active
	 */
	enter_sdk_thread();

	write_to_textfield(_("--INFO_MSG--: in test_sdk\n"), INFO_MSG);

	sleep(5);

        /*
	 * check for correct state
	 */
	leave_sdk_thread();

	return NULL;
}

void
check_test_env()
{
	PRINT_LOCATION();

	/*
	 * check state of download-button
	 */
	int ret = get_state_of_gui_element(DOWNLOAD_B);
	if (ret < 0) {
		fprintf(stderr, _("Something went wrong ret == -1"));
	} else {
		if (ret == 0)
			unlock_button(TEST_B);
		else
			lock_button(TEST_B);

	}
}
