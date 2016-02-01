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
download_toolchain(void *args)
{
	PRINT_LOCATION();

	/*
	 * only one thread could be active
	 */
	enter_repo_thread();

	write_to_textfield(_("--INFO_MSG--: in download_toolchain\n"),
			   INFO_MSG);
	sleep(5);

	/*
	 * check for correct state
	 */
	leave_repo_thread();

	return NULL;
}

void
check_toolchain()
{
	PRINT_LOCATION();

	const char *toolchain_path = TOOLCHAIN_PATH;
        const char *repo_path = SDK_GIT_PATH;

	/*
	 * check checksum.sha256 and tgz are in sync
	 */
	int state = 1;
	if (state == 0) {
		lock_button(DOWNLOAD_B);
		lock_button(DOWNLOAD_M);
	} else {
		unlock_button(DOWNLOAD_B);
		unlock_button(DOWNLOAD_M);
	}
}
