# Makefile.am

bin_PROGRAMS = sdk_confd

sdk_confd_SOURCES =             \
	main.c    	        \
	common.h        	\
	gettext.h


sdk_confd_CFLAGS =             	\
	$(LIBCONFIG_CFLAGS)     \
	-DLOCALEDIR=\"$(localedir)\" -I$(srcdir)/../../lib


sdk_confd_LDADD =               \
	$(LIBCONFIG_LIBS)   	\
	$(LIBINTL) ../../lib/libservice.a
