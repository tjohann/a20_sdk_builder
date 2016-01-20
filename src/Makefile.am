# Makefile.am

bin_PROGRAMS = a20_sdk_builder

a20_sdk_builder_SOURCES = a20_sdk_builder.c gui.c a20_sdk_builder.h gettext.h

a20_sdk_builder_CFLAGS = $(LIBGTK_CFLAGS) $(LIBCONFIG_CFLAGS) $(LIBGIT2_CFLAGS) -DLOCALEDIR=\"$(localedir)\" -I$(srcdir)/../lib
a20_sdk_builder_LDADD = $(LIBGTK_LIBS) $(LIBCONFIG_LIBS) $(LIBGIT2_LIBS) $(LIBINTL) ../lib/libservice.a
