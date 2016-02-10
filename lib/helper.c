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
	putchar('\n');
	fprintf(stdout, _("Show content of version realted info: \n"));
	fprintf(stdout, _("------------------------------------- \n"));
	fprintf(stdout, _("Package version: %s\n"), PACKAGE_VERSION);
	fprintf(stdout, _("------------------------------------- \n"));
	putchar('\n');
}


void
show_package_name()
{	putchar('\n');
	fprintf(stdout, _("Show content of package realted info: \n"));
	fprintf(stdout, _("------------------------------------- \n"));
	fprintf(stdout, _("Package name is %s\n"), PACKAGE_STRING);
	fprintf(stdout, _("------------------------------------- \n"));
	putchar('\n');
}


void
show_program_name(char *program_name)
{
	putchar('\n');
	fprintf(stdout, _("Show content of program realted info: \n"));
	fprintf(stdout, _("------------------------------------- \n"));
	fprintf(stdout, _("Program name is %s\n"), program_name);
	fprintf(stdout, _("------------------------------------- \n"));
	putchar('\n');
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
	tmp_str = malloc(len);

	memset(tmp_str, 0, len);
	strncat(tmp_str, whoami, len);

	*program_name = tmp_str;
}


bool
is_this_a_dir(const char *dir_name)
{
	struct stat sb;
	memset(&sb, 0, sizeof(struct stat));

	if (dir_name == NULL)
		return false;
	
	if (stat(dir_name, &sb) == 0 && S_ISDIR(sb.st_mode))
		return true;
	else
		return false;

	// should never reached
	return false;
}


char *
alloc_string(const char *tmp_str)
{
	char *str = NULL;
	size_t len = 0;

	if (tmp_str == NULL)
		return NULL;

	// conf dir handling
	len = strlen(tmp_str) + 1;
	
	str = malloc(len);
	if (str == NULL)
		return NULL;
	
	memset(str, 0, len);
	strncat(str, tmp_str, len);
	
	return str;
}
