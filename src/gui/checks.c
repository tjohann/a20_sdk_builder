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
	bool state = false;

	if (is_checksum_array_valid())
		state = true;
	else
		state = false;

	/*
	 * TODO: check checksum.sha256 and tgz are in sync
	 */


	if (state) {

		printf("\n\n\t trueeeeee\n");

		unlock_button(DOWNLOAD_B);
		unlock_button(DOWNLOAD_M);
	} else {

		printf("\n\n\t falsssseee\n");

		lock_button(DOWNLOAD_B);
		lock_button(DOWNLOAD_M);
	}
}


int
check_sdk_workdir()
{
	char *workdir = get_common_workdir();

	if (workdir == NULL)
		return -1;

	if (is_this_a_dir(workdir))
		return 0;

	return -1;
}


int
check_sdk_runtimedir()
{
	char *runtimedir = get_common_runtimedir();

	if (runtimedir == NULL)
		return -1;

	if (is_this_a_dir(runtimedir))
		return 0;

	return -1;
}


void
check_test_env()
{
	/*
	 * TODO: compile a hello world and see if it works and runs
	 * -> container
	 * -> check if $HOME/src/a20_sdk/ is valid and available
	 */

	unlock_button(TEST_B);
}


void
check_all_states()
{
	check_all_sdk_states();
	check_all_images_states();
	check_all_external_states();
	check_all_kernel_states();
}


void
check_all_sdk_states()
{
	if (check_sdk_runtimedir() == 0)
		check_sdk_git_path();
	else
		info_msg("%s isn't available", get_common_runtimedir());


	if (check_sdk_workdir() == 0) {
		check_toolchain();
		check_test_env();
	} else {
		info_msg("%s isn't available", get_common_workdir());
	}
}


void
check_all_images_states()
{
	if (check_sdk_runtimedir() == 0)
		PRINT_LOCATION();
	else
		info_msg("%s isn't available", get_common_runtimedir());


	if (check_sdk_workdir() == 0) {
		PRINT_LOCATION();
	} else {
		info_msg("%s isn't available", get_common_workdir());
	}
}


void
check_all_external_states()
{
	if (check_sdk_runtimedir() == 0)
		PRINT_LOCATION();
	else
		info_msg("%s isn't available", get_common_runtimedir());


	if (check_sdk_workdir() == 0) {
		PRINT_LOCATION();
	} else {
		info_msg("%s isn't available", get_common_workdir());
	}
}


void
check_all_kernel_states()
{
	if (check_sdk_runtimedir() == 0)
		PRINT_LOCATION();
	else
		info_msg("%s isn't available", get_common_runtimedir());


	if (check_sdk_workdir() == 0) {
		PRINT_LOCATION();
	} else {
		info_msg("%s isn't available", get_common_workdir());
	}
}
