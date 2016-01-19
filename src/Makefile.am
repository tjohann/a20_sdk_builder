# Makefile.am

bin_PROGRAMS = a20_sdk_builder

a20_sdk_builder_SOURCES = a20_sdk_builder.c gui.c a20_sdk_builder.h gettext.h
a20_sdk_builder_CFLAGS = $(A20_SDK_BUILDER_CFLAGS) -DLOCALEDIR=\"$(localedir)\" -I$(srcdir)/../lib 
a20_sdk_builder_LDADD = $(A20_SDK_BUILDER_LIBS) $(LIBINTL) ../lib/libservice.a

