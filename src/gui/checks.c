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
check_sdk_git_path()
{
	PRINT_LOCATION();

	git_repository *repo = NULL;

	const char *path = get_sdk_repo_path();
	if (path != NULL) {
		int error = git_repository_open_ext(&repo, path, 0, NULL);
		if (error == 0) {
			lock_button(CLONE_B);
			lock_button(CLONE_M);
			unlock_button(UPDATE_B);
			unlock_button(UPDATE_M);
		} else {
			lock_button(UPDATE_B);
			lock_button(UPDATE_M);
			unlock_button(CLONE_B);
			unlock_button(CLONE_M);
		}
	} else {
		error_msg("get_sdk_repo_path() == NULL");
	}

	if (repo)
		git_repository_free(repo);
}


void
check_toolchain()
{
	PRINT_LOCATION();

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


int
check_sdk_workdir()
{

/*
 * check if /opt/..._sdk is available
 */


	// check for workdir
	return 0;
}


int
check_sdk_runtimedir()
{

/*
 * check if /var/lib..._sdk is available
 * Note: to check if it's a git repo use check_sdk_git_path()
 */


	// check runtimedir!
	return 0;
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
		write_error_msg(_("Something went wrong ret == -1"));
	} else {
		if (ret == 0)
			unlock_button(TEST_B);
		else
			lock_button(TEST_B);

	}
}
