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

#include "libservice.h"


void
show_version_info()
{
	fprintf(stdout, "Package version: %s\n", PACKAGE_VERSION);

}

void
show_package_name()
{
	fprintf(stdout, _("Package name is %s\n"), PACKAGE_STRING);
}

void
show_program_name(char *program_name)
{
	fprintf(stdout, _("Program name is %s\n"), program_name);
}

void
set_program_name(char **program_name, char *kdo_arg)
{
	char *whoami = NULL;
	char *tmp_str = NULL;
	size_t len = 0;

	if (kdo_arg == NULL)
		kdo_arg = "dummy";

	if ((whoami = strrchr(kdo_arg, '/')) == NULL)
		whoami = kdo_arg;
	else
		whoami++;

	len = strlen(whoami) + 1;
	printf("strlen whoami %d\n", len);
	tmp_str = malloc(len);

	memset(tmp_str, 0, len);
	strncat(tmp_str, whoami, len);

	*program_name = tmp_str;
}
