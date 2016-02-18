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

extern int use_syslog;

static void
message_common(int errno_flag, message_types_t type, const char *fmt, va_list va)
{
	char buf[MAXLINE + 1];
	int errno_save = errno;
	int log_level = LOG_ERR;

	vsnprintf(buf, MAXLINE, fmt, va);

	size_t n = strlen(buf);
	if (errno_flag)
		snprintf(buf + n, MAXLINE - n, ": %s", strerror(errno_save));

	strcat(buf, "\n");

	switch(type) {
		case NORMAL_MSG:
			write_to_textfield(buf, NORMAL_MSG);
			fprintf(stdout, buf);
			log_level = LOG_INFO;
			break;
		case WARNING_MSG:
			write_to_textfield(buf, WARNING_MSG);
			fprintf(stdout, buf);
			log_level = LOG_WARNING;
			break;
		case ERROR_MSG:
			write_to_textfield(buf, ERROR_MSG);
			fprintf(stderr, buf);
			log_level = LOG_ERR;
			break;
		case INFO_MSG:
			write_to_textfield(buf, INFO_MSG);
			fprintf(stdout, buf);
			log_level = LOG_INFO;
			break;
		case NONE:
			write_to_textfield(buf, NONE);
			fprintf(stdout, buf);
			log_level = LOG_DEBUG;
			break;
		default:
			fprintf(stderr,
				_("ERROR: Message type not supported\n"));
		}

	if (use_syslog)
		syslog(log_level, buf);
}


/*
 * print error message and exit
 */
void
__attribute__((noreturn)) write_error_msg_exit(const char *fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	message_common(1, ERROR_MSG, fmt, va);
	va_end(va);

	exit(EXIT_FAILURE);
}


/*
 * print error message
 */
void
write_error_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(1, ERROR_MSG, fmt, va);
	va_end(va);
}

/*
 * print error message and return
 */
void
write_error_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(1, ERROR_MSG, fmt, va);
	va_end(va);

	return;
}


/*
 * print info message
 */
void
write_info_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(0, INFO_MSG, fmt, va);
	va_end(va);
}


/*
 * print info message and return
 */
void
write_info_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(0, INFO_MSG, fmt, va);
	va_end(va);

	return;
}


/*
 * print debug message
 */
void
write_debug_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(0, NONE, fmt, va);
	va_end(va);
}


/*
 * print debug message and return
 */
void
write_debug_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(0, NONE, fmt, va);
	va_end(va);

	return;
}


/*
 * print warning message
 */
void
write_warning_msg(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(1, WARNING_MSG, fmt, va);
	va_end(va);
}


/*
 * print warning message and return
 */
void
write_warning_msg_return(const char *fmt, ...)
{
	va_list	va;

	va_start(va, fmt);
	message_common(1, WARNING_MSG, fmt, va);
	va_end(va);

	return;
}
