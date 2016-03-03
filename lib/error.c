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


int use_syslog;


/*
 * Common error function which is called in all exported error handling
 * functions
 * Hint: see man 3 syslog for log_level
 *
 * +---------------------+------------+------------+--------------------------+
 * |     function        | use errno? | terminate? | log_level (man 3 syslog) |
 * +---------------------+------------+------------+--------------------------+
 * | error_exit          |     yes    |   exit()   |         LOG_ERR          |
 * | dump_exit           |     yes    |  abort()   |         LOG_ERR          |
 * | error_msg           |     yes    |    no      |         LOG_ERR          |
 * | error_msg_return    |     yes    |   return   |         LOG_ERR          |
 * | info_msg            |     no     |    no      |         LOG_INFO         |
 * | info_msg_return     |     no     |   return   |         LOG_INFO         |
 * | debug_msg           |     no     |    no      |         LOG_DEBUG        |
 * | debug_msg_return    |     no     |    no      |         LOG_DEBUG        |
 * +---------------------+------------+------------+--------------------------+
 * | th_error_msg        | errno_val  |    no      |         LOG_ERR          |
 * | th_error_msg_return | errno_val  |   return   |         LOG_ERR          |
 * | th_error_exit       | errno_val  |   exit()   |         LOG_ERR          |
 * | th_dump_exit        | errno_val  |  abourt()  |         LOG_ERR          |
 * +---------------------+------------+------------+--------------------------+
 */
static void
error_common(int errno_flag, int errno_val, int log_level, const char *fmt, va_list va)
{
	char buf[MAXLINE + 1];
	int errno_save = 0;

        // errno_flag (errno) should override errno_val
	if (errno_val != 0)
		errno_save = errno_val;

	if (errno_flag)
		errno_save = errno;

	vsnprintf(buf, MAXLINE, fmt, va);

	size_t n = strlen(buf);
	if (errno_save)
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));

	strcat(buf, "\n");

	if (use_syslog)
		syslog(log_level, buf);

	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
}


/*
 * print error message and exit
 */
void
__attribute__((noreturn)) error_exit(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	error_common(1, 0, LOG_ERR, fmt, va);
	va_end(va);

	exit(EXIT_FAILURE);
}


/*
 * print error message and dump/exit
 */
void
__attribute__((noreturn)) dump_exit(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	error_common(1, 0, LOG_ERR, fmt, va);
	va_end(va);

	abort();
	exit(EXIT_FAILURE);  // shouldn't get here
}


/*
 * print error message
 */
void
error_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(1, 0, LOG_ERR, fmt, va);
	va_end(va);
}


/*
 * print error message and return
 */
void
error_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(1, 0, LOG_ERR, fmt, va);
	va_end(va);

	return;
}


/*
 * print info message
 */
void
info_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, 0, LOG_INFO, fmt, va);
	va_end(va);
}


/*
 * print info message and return
 */
void
info_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, 0, LOG_INFO, fmt, va);
	va_end(va);

	return;
}


/*
 * print debug message
 */
void
debug_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, 0, LOG_DEBUG, fmt, va);
	va_end(va);
}

/*
 * print debug message and return
 */
void
debug_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, 0, LOG_DEBUG, fmt, va);
	va_end(va);

	return;
}


/*
 * print error message with errno = errno_val
 */
void
th_error_msg(int errno_val, const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, errno_val, LOG_ERR, fmt, va);
	va_end(va);
}

/*
 * print error message with errno = errno_val and return
 */
void
th_error_msg_return(int errno_val, const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, errno_val, LOG_ERR, fmt, va);
	va_end(va);

	return;
}


/*
 * print error message with errno = errno_val and dump/exit
 */
void
__attribute__((noreturn)) th_dump_exit(int errno_val, const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, errno_val, LOG_ERR, fmt, va);
	va_end(va);

	abort();
	exit(EXIT_FAILURE);  // shouldn't get here
}


/*
 * print error message with errno = errno_val and exit
 */
void
__attribute__((noreturn)) th_error_exit(int errno_val, const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	error_common(0, errno_val, LOG_ERR, fmt, va);
	va_end(va);

	exit(EXIT_FAILURE);
}
