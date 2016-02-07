# Makefile.am

appicondir = @datadir@/icons/hicolor/scalable/apps
appicon_DATA = sdk_builder.svg


#
# specials to handle the update of the icon cache
#
gtk_update_icon_cache = gtk-update-icon-cache -f -t $(appicondir)

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache

update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating GTK icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated. After (un)install, run this:";\
		echo "*** $(gtk_update_icon_cache)"; \
	fi
