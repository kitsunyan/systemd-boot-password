SUBDIRS = \
	efi

bin_SCRIPTS = \
	sbpctl

sbpctl:
	$(AM_V_GEN) sed \
	-e 's,[@]BOOT_LIB_DIR[@],$(bootlibdir),g' \
	-e 's,[@]EFI_MACHINE_TYPE_NAME[@],$(EFI_MACHINE_TYPE_NAME),g' \
	< sbpctl.in > sbpctl
	@chmod a+x sbpctl

CLEANFILES = \
	$(bin_SCRIPTS)
