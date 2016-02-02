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


static int
copy_data(struct archive *ar, struct archive *aw)
{
	int r;
	const void *buff;
	size_t size;
	int64_t offset;

	printf("in copy_data ... \n");

	for (;;) {
		r = archive_read_data_block(ar, &buff, &size, &offset);
		if (r == ARCHIVE_EOF)
			return (ARCHIVE_OK);
		if (r != ARCHIVE_OK)
			return (r);
		r = archive_write_data_block(aw, buff, size, offset);
		if (r != ARCHIVE_OK) {
			fprintf(stderr, "archive_write_data_block() %s", archive_error_string(aw));
			return (r);
		}
	}
}


void
extract_toolchain()
{
	struct archive *archive;
	struct archive *ext;
	struct archive_entry *entry;

	char *filename = "/opt/a20_sdk/toolchain_x86_64.tgz";
	chdir("/opt/a20_sdk/");

	int error = 0;

	size_t block_size = 10240;
	int flags = ARCHIVE_EXTRACT_TIME;

	archive = archive_read_new();
	ext = archive_write_disk_new();
	archive_write_disk_set_options(ext, flags);

	archive_read_support_compression_all(archive);
	archive_read_support_format_tar(archive);

	error = archive_read_open_filename(archive, filename, block_size);
	if (error) {
		fprintf(stderr, "archive_read_open_filename() %s \n", archive_error_string(archive));
		write_to_textfield("archive_read_open_filename(): ", ERROR_MSG);
		write_to_textfield(archive_error_string(archive), ERROR_MSG);
		goto out;
	}

	for (;;) {
		error = archive_read_next_header(archive, &entry);

		if (error == ARCHIVE_EOF)
			break;

		if (error != ARCHIVE_OK) {
			fprintf(stderr, "archive_read_next_header() %s \n", archive_error_string(archive));
			write_to_textfield("archive_read_next_header(): ", ERROR_MSG);
			write_to_textfield(archive_error_string(archive), ERROR_MSG);
		}

		error = archive_write_header(ext, entry);
		if (error != ARCHIVE_OK) {
			fprintf(stderr, "archive_write_header() %s \n", archive_error_string(ext));
			write_to_textfield("archive_write_header(): ", ERROR_MSG);
			write_to_textfield(archive_error_string(archive), ERROR_MSG);
		} else {
			copy_data(archive, ext);
			error = archive_write_finish_entry(ext);
			if (error != ARCHIVE_OK) {
				fprintf(stderr, "archive_write_finish_entry() %s \n", archive_error_string(ext));
				write_to_textfield("archive_write_finish_entry(): ", ERROR_MSG);
				write_to_textfield(archive_error_string(archive), ERROR_MSG);
			}
		}
	}

out:

	printf("finished \n");

	archive_read_close(archive);
	archive_read_free(archive);
}
