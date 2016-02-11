# Makefile.am

noinst_LIBRARIES = libservice.a

libservice_a_SOURCES =  \
	libservice.h	\
	global.h 	\
	error.c 	\
	network.c 	\
	helper.c 	\
	config.c 	\
	gettext.h

libservice_a_CFLAGS = -DLOCALDIR=\"$(localedir)\"
libservice_a_LIBADD = $(LIBINTL)
