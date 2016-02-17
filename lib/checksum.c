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


int
calc_checksum(char *filename)
{
	unsigned char buffer[BUFSIZ];
	FILE *f;
	SHA256_CTX ctx;
	size_t len;

	/*if (argc < 2) {
		fprintf(stderr, "usage: %s <file>\n", argv[0]);
		return 1;
	}
	f = fopen(argv[1], "r");
	if (!f) {
		fprintf(stderr, "couldn't open %s\n", argv[1]);
		return 1;
	}
	*/
	SHA256_Init(&ctx);
	do {
		len = fread(buffer, 1, BUFSIZ, f);
		SHA256_Update(&ctx, buffer, len);
	} while (len == BUFSIZ);
	SHA256_Final(buffer, &ctx);
	fclose(f);
	for (len = 0; len < SHA256_DIGEST_LENGTH; ++len)
		printf("%02x", buffer[len]);
	putchar('\n');

	return 0;
}
