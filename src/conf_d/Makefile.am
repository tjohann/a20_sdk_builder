# Makefile.am

bin_PROGRAMS = sdk_confd

sdk_confd_SOURCES =             \
	main.c    	        \
	common.h        	\
	threads.c		\
	gettext.h


sdk_confd_CFLAGS =             	\
	$(LIBCONFIG_CFLAGS)     \
	$(LIBSSL_CFLAGS)        \
	$(LIBCRYPTO_CFLAGS)     \
	-DLOCALEDIR=\"$(localedir)\" -I$(srcdir)/../../lib


sdk_confd_LDADD =               \
	$(LIBCONFIG_LIBS)   	\
	$(LIBSSL_LIBS)         	\
	$(LIBCRYPTO_LIBS)      	\
	$(LIBINTL) ../../lib/libservice.a
