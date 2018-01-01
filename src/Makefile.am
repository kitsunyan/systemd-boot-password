SUBDIRS = \
	efi

bin_SCRIPTS = \
	sbpctl

sbpctl: sbpctl.in
	$(AM_V_GEN) sed \
	-e 's,[@]BOOT_LIB_DIR[@],$(bootlibdir),g' \
	-e 's,[@]EFI_MACHINE_TYPE_NAME[@],$(EFI_MACHINE_TYPE_NAME),g' \
	< $< > $@
	@chmod a+x $@

EXTRA_DIST = \
	sbpctl.in

CLEANFILES = \
	$(bin_SCRIPTS)
