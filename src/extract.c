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

/*
 * module specific macros
 * ======================
 */
#define report_error_read_next_header() do {				\
		fprintf(stderr, "archive_read_next_header() %s \n",	\
			archive_error_string(archive));			\
		write_to_textfield("archive_read_next_header():\n",	\
				   ERROR_MSG);				\
		write_to_textfield(archive_error_string(archive),	\
				   NONE);				\
		write_to_textfield("\n", NONE);				\
	} while (0)


#define report_error_archive_write_header() do {			\
		fprintf(stderr, "archive_write_header() %s \n",		\
			archive_error_string(ext));			\
		write_to_textfield("archive_write_header(): \n",	\
				   ERROR_MSG);				\
		write_to_textfield(archive_error_string(archive),	\
				   NONE);				\
		write_to_textfield("\n", NONE);				\
	} while (0)


#define report_error_archive_write_finish_entry() do {			\
		fprintf(stderr, "archive_write_finish_entry() %s \n",	\
			archive_error_string(ext));			\
		write_to_textfield("archive_write_finish_entry():\n",	\
				   ERROR_MSG);				\
		write_to_textfield(archive_error_string(archive),	\
				   NONE);				\
		write_to_textfield("\n", NONE);				\
	} while (0)


#define handle_error_archive_read_open_filename() do {			\
		fprintf(stderr, "archive_read_open_filename() %s \n",	\
			archive_error_string(archive));			\
		write_to_textfield("archive_read_open_filename():\n",	\
				   ERROR_MSG);				\
		write_to_textfield(archive_error_string(archive),	\
				   NONE);				\
		write_to_textfield("\n", NONE);				\
		goto error;						\
	} while (0)


#define report_error_archive_write_data_block() do {			\
		fprintf(stderr, "archive_write_data_block() %s \n",	\
			archive_error_string(a_write));			\
		write_to_textfield("archive_write_data_block(): \n",	\
				   ERROR_MSG);				\
		write_to_textfield(archive_error_string(a_write),	\
				   NONE);				\
		write_to_textfield("\n", NONE);				\
		return (error);						\
	} while (0)


/*
 * content
 * =======
 */
static int
copy_data(struct archive *a_read, struct archive *a_write)
{
	int error;
	const void *buff;
	size_t size;
	int64_t offset;

	for (;;) {
		error = archive_read_data_block(a_read, &buff, &size, &offset);
		if (error == ARCHIVE_EOF)
			return (ARCHIVE_OK);

		if (error != ARCHIVE_OK)
			return (error);

		error = archive_write_data_block(a_write, buff, size, offset);
		if (error != ARCHIVE_OK)
			report_error_archive_write_data_block();
	}
}


int
extract_toolchain(char *filename)
{
	struct archive *archive;
	struct archive *ext;
	struct archive_entry *entry;

	char *workdir = get_common_workdir();

	if (workdir)
		chdir(workdir);
	else
		return -1;

	size_t block_size = 10240;
	int flags = ARCHIVE_EXTRACT_TIME;

	archive = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);

	archive_read_support_compression_all(archive);
	archive_read_support_format_tar(archive);

	int error = archive_read_open_filename(archive, filename, block_size);
	if (error)
		handle_error_archive_read_open_filename();

	for (;;) {
		error = archive_read_next_header(archive, &entry);

		if (error == ARCHIVE_EOF)
			break;

		if (error != ARCHIVE_OK)
			report_error_read_next_header();


		error = archive_write_header(ext, entry);
		if (error != ARCHIVE_OK) {
			report_error_archive_write_header();
		} else {
			error = copy_data(archive, ext);
			if (error)
				fprintf(stderr, "ERROR in copy_data\n");

			error = archive_write_finish_entry(ext);
			if (error != ARCHIVE_OK)
				report_error_archive_write_finish_entry();
		}
	}

	write_to_textfield(_("Uncompress archive finished\n"), INFO_MSG);

	archive_read_close(archive);
	archive_read_free(archive);
	return 0;

error:
	archive_read_close(archive);
	archive_read_free(archive);

	return -1;
}
