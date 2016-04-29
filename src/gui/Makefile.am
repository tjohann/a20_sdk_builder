# Makefile.am

bin_PROGRAMS = sdk_builder

sdk_builder_SOURCES =             \
	main.c                    \
	gui.c                     \
	message_handler.c         \
	threads.c                 \
	common.h  		  \
	checks.c                  \
	gettext.h

sdk_builder_CFLAGS =              \
	$(LIBGTK_CFLAGS)          \
	$(LIBCONFIG_CFLAGS)       \
	$(LIBGIT2_CFLAGS)         \
	$(LIBCURL_CFLAGS)         \
	$(LIBARCHIVE_CFLAGS)      \
	$(LIBSSL_CFLAGS)          \
	$(LIBCRYPTO_CFLAGS)       \
	$(LIBBSD_CFLAGS)      	  \
	$(LIBBAALUE_CFLAGS)    	  \
	-DLOCALEDIR=\"$(localedir)\" -I$(srcdir)/../../lib

sdk_builder_LDADD =               \
	$(LIBGTK_LIBS)            \
	$(LIBCONFIG_LIBS)         \
	$(LIBGIT2_LIBS)           \
	$(LIBCURL_LIBS)           \
	$(LIBARCHIVE_LIBS)        \
	$(LIBSSL_LIBS)         	  \
	$(LIBCRYPTO_LIBS)      	  \
	$(LIBBSD_LIBS)      	  \
	$(LIBBAALUE_LIBS)      	  \
	$(LIBINTL) ../../lib/libservice.a


#
# specials to handle /opt/a20_sdk(toolchain) and /var/lib/a20_sdk(git repo)
#
# TODO: remove -> build small scripts for it
# NOTE: will removed
#

# remove for the moment -> should be done by tool itself or script
#install-data-hook: handle_opt handle_var
#uninstall-hook: remove_opt remove_var

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
