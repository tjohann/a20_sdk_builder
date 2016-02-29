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


ssize_t
read_line(int fd, void *buf, size_t n_bytes)
{
	ssize_t n = 0;
	size_t count = 0;
	char *tmp_buf = buf;
	char ch = 0;

read_cmd:
	n = read(fd, &ch, 1);
	if (n == -1) {
		if (errno == EINTR)
			goto read_cmd;
		else {
			error_msg_return("read_line() -> read");
		}
	} else {
		if (n == 0 && count == 0)
			return FILE_EMPTY;

		if (count < n_bytes - 1) {
			count++;
			*tmp_buf++ = ch;
		}

		if ((ch != '\n') && (n != 0))        // NEWLINE or EOF
			goto read_cmd;
	}
	*tmp_buf = '\0';

	return count;
}


size_t
read_words(char *line, char *word_array[], size_t n_words)
{
	char *c = line;
	size_t n = 0;

read_cmd:
	while (isspace(*c))
		c++;

	if (*c == '\0')
		return n;

	word_array[n++] = c;

	while (!isspace(*c) && *c != '\0')
		c++;

	if (*c == '\0')
		return n;

	*c++ = '\0';

	if (n > n_words)
		info_msg("Line has more than %d words", n_words);
	else
		goto read_cmd;

	return n;
}


int
calc_checksum(download_tupel_t *t)
{
	char *checksum_s = NULL;
	unsigned char *hash = NULL;
	FILE *f;
	SHA256_CTX ctx;
	size_t len;

	hash = malloc(SHA256_DIGEST_LENGTH);
	if (hash == NULL)
		error_msg_return("Possible ERROR: hash == NULL");

	// hash * 2 -> hash as string -> + 1 for \0
	checksum_s = malloc((SHA256_DIGEST_LENGTH * 2) + 1);
	if (checksum_s == NULL)
		error_msg_return("Possible ERROR: checksum_s == NULL");

	memset(checksum_s, 0, SHA256_DIGEST_LENGTH + 1);
	memset(hash, 0, SHA256_DIGEST_LENGTH);

	f = fopen(t->path, "r");
	if (f == NULL)
		error_msg_return("Possible ERROR: couldn't open %s\n", t->path);

	SHA256_Init(&ctx);

	do {
		len = fread(hash, 1, SHA256_DIGEST_LENGTH, f);
		SHA256_Update(&ctx, hash, len);
	} while (len == SHA256_DIGEST_LENGTH);

	SHA256_Final(hash, &ctx);
	fclose(f);

	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		sprintf(checksum_s + (i * 2), "%02x", hash[i]);

	t->hash = hash;
	t->checksum_s = checksum_s;

#ifndef DEBUG
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		fprintf(stdout, "%02x", hash[i]);
	putchar('\n');
	info_msg(t->checksum_s);
#endif

	return 0;
}


void
print_checksum_tupel(checksum_tupel_t *c)
{
	if (c == NULL) {
		debug_msg("c == NULL in %s", __FUNCTION__);
	} else {
		if (c->name == NULL)
			debug_msg("c->name == NULL in %s", __FUNCTION__);
		else
			debug_msg("c->name = %s", c->name);

		if (c->checksum_s == NULL)
			debug_msg("c->checksum_s == NULL in %s", __FUNCTION__);
		else
			debug_msg("c->checksum_s = %s", c->checksum_s);
	}
}
