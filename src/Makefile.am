# Makefile.am

bin_PROGRAMS = a20_sdk_builder

a20_sdk_builder_SOURCES = main.c gui.c common_git.c config.c clone.c update.c download.c test.c common.h help.c gettext.h

a20_sdk_builder_CFLAGS = $(LIBGTK_CFLAGS) $(LIBCONFIG_CFLAGS) $(LIBGIT2_CFLAGS) -DLOCALEDIR=\"$(localedir)\" -I$(srcdir)/../lib
a20_sdk_builder_LDADD = $(LIBGTK_LIBS) $(LIBCONFIG_LIBS) $(LIBGIT2_LIBS) $(LIBINTL) ../lib/libservice.a


#
# specials to handle /opt/a20_sdk(toolchain) and /var/lib/a20_sdk(git repo)
#
install-data-hook: handle_opt handle_var
uninstall-hook: remove_opt remove_var

handle_opt:
	@-if test -d /opt/a20_sdk; then \
		echo "a20_sdk@opt already available -> handle access rights"; \
		chgrp -R users /opt/a20_sdk; \
		chmod 775 /opt/a20_sdk; \
	else \
		echo "a20_sdk@opt NOT available -> create it";\
		mkdir /opt/a20_sdk; \
		chgrp -R users /opt/a20_sdk; \
		chmod 775 /opt/a20_sdk; \
	fi

handle_var:
	@-if test -d /var/lib/a20_sdk; then \
		echo "a20_sdk@var already available -> handle access rights"; \
		chgrp -R users /var/lib/a20_sdk; \
		chmod 775 /var/lib/a20_sdk; \
	else \
		echo "a20_sdk@var NOT available -> create it";\
		mkdir /var/lib/a20_sdk; \
		chgrp -R users /var/lib/a20_sdk; \
		chmod 775 /var/lib/a20_sdk; \
	fi

remove_opt:
	@-if test -d /opt/a20_sdk; then \
		echo "a20_sdk@opt already available -> remove it"; \
		rm -rf /opt/a20_sdk;\
	else \
		echo "a20_sdk@opt NOT available";\
	fi

remove_var:
	@-if test -d /var/lib/a20_sdk; then \
		echo "a20_sdk@var already available -> remove it"; \
		rm -rf /var/lib/a20_sdk;\
	else \
		echo "a20_sdk@var NOT available";\
	fi
