ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
bootlib_DATA =
bin_SCRIPTS =
EXTRA_DIST =
CLEANFILES =

bootlibdir = $(prefix)/lib/systemd/boot/efi-sbp
installerdir = $(prefix)/bin
bashcompletiondir = @bashcompletiondir@

# ------------------------------------------------------------------------------

efi_cppflags = \
	$(EFI_CPPFLAGS) \
	-I$(top_builddir) -include config.h \
	-I$(EFI_INC_DIR)/efi \
	-I$(EFI_INC_DIR)/efi/$(EFI_ARCH) \
	-DEFI_MACHINE_TYPE_NAME=\"$(EFI_MACHINE_TYPE_NAME)\"

efi_cflags = \
	$(EFI_CFLAGS) \
	-Wall \
	-Wextra \
	-std=gnu90 \
	-nostdinc \
	-ggdb -O0 \
	-fpic \
	-fshort-wchar \
	-nostdinc \
	-ffreestanding \
	-fno-strict-aliasing \
	-fno-stack-protector \
	-Wsign-compare \
	-Wno-missing-field-initializers

if ARCH_X86_64
efi_cflags += \
	-mno-red-zone \
	-mno-sse \
	-mno-mmx \
	-DEFI_FUNCTION_WRAPPER \
	-DGNU_EFI_USE_MS_ABI
endif

if ARCH_IA32
efi_cflags += \
	-mno-sse \
	-mno-mmx
endif

efi_ldflags = \
	$(EFI_LDFLAGS) \
	-T $(EFI_LDS_DIR)/elf_$(EFI_ARCH)_efi.lds \
	-shared \
	-Bsymbolic \
	-nostdlib \
	-znocombreloc \
	-L $(EFI_LIB_DIR) \
	$(EFI_LDS_DIR)/crt0-efi-$(EFI_ARCH).o

# Aarch64 and ARM32 don't have an EFI capable objcopy. Use 'binary' instead,
# and add required symbols manually.
if ARCH_AARCH64
efi_ldflags += --defsym=EFI_SUBSYSTEM=0xa
EFI_FORMAT = -O binary
else
EFI_FORMAT = --target=efi-app-$(EFI_ARCH)
endif

systemd_boot_headers = \
	src/efi/util.h \
	src/efi/console.h \
	src/efi/graphics.h \
	src/efi/pefile.h \
	src/efi/measure.h \
	src/efi/disk.h \
	src/efi/sha512.h

systemd_boot_sources = \
	src/efi/util.c \
	src/efi/console.c \
	src/efi/graphics.c \
	src/efi/pefile.c \
	src/efi/disk.c \
	src/efi/measure.c \
	src/efi/boot.c \
	src/efi/sha512.c

systemd_boot_objects = $(addprefix $(top_builddir)/,$(systemd_boot_sources:.c=.o))
systemd_boot_solib = $(top_builddir)/src/efi/systemd_boot.so
systemd_boot = systemd-boot$(EFI_MACHINE_TYPE_NAME).efi

$(systemd_boot): $(systemd_boot_solib)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	  -j .dynsym -j .rel -j .rela -j .reloc $(EFI_FORMAT) $< $@

$(systemd_boot_solib): $(systemd_boot_objects)
	$(LD) $(efi_ldflags) $(systemd_boot_objects) \
		-o $@ -lefi -lgnuefi $(shell $(CC) -print-libgcc-file-name);
	nm -D -u $@ | grep ' U ' && exit 1 || :

$(top_builddir)/src/efi/%.o: $(top_srcdir)/src/efi/%.c $(addprefix $(top_srcdir)/,$(systemd_boot_headers))
	@$(MKDIR_P) $(top_builddir)/src/efi/
	$(CC) $(efi_cppflags) $(efi_cflags) -c $< -o $@

bootlib_DATA += $(systemd_boot)
CLEANFILES += $(systemd_boot_objects) $(systemd_boot_solib) $(systemd_boot)

# ------------------------------------------------------------------------------

systemd_boot_stub_headers = \
	src/efi/util.h \
	src/efi/pefile.h \
	src/efi/disk.h \
	src/efi/graphics.h \
	src/efi/splash.h \
	src/efi/measure.h \
	src/efi/linux.h

systemd_boot_stub_sources = \
	src/efi/util.c \
	src/efi/pefile.c \
	src/efi/disk.c \
	src/efi/graphics.c \
	src/efi/splash.c \
	src/efi/linux.c \
	src/efi/measure.c \
	src/efi/stub.c

systemd_boot_stub_objects = $(addprefix $(top_builddir)/,$(systemd_boot_stub_sources:.c=.o))
systemd_boot_stub_solib = $(top_builddir)/src/efi/systemd_boot_stub.so
systemd_boot_stub = systemd-boot$(EFI_MACHINE_TYPE_NAME).efi.stub

$(systemd_boot_stub): $(systemd_boot_stub_solib)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	  -j .dynsym -j .rel -j .rela -j .reloc $(EFI_FORMAT) $< $@

$(systemd_boot_stub_solib): $(systemd_boot_stub_objects)
	$(LD) $(efi_ldflags) $(systemd_boot_stub_objects) \
		-o $@ -lefi -lgnuefi $(shell $(CC) -print-libgcc-file-name);
	nm -D -u $@ | grep ' U ' && exit 1 || :

$(top_builddir)/src/efi/%.o: $(top_srcdir)/src/efi/%.c $(addprefix $(top_srcdir)/,$(systemd_boot_stub_headers))
	@$(MKDIR_P) $(top_builddir)/src/efi/
	$(EFI_CC) $(efi_cppflags) $(efi_cflags) -c $< -o $@

bootlib_DATA += $(systemd_boot_stub)
CLEANFILES += $(systemd_boot_stub_objects) $(systemd_boot_stub_solib) $(systemd_boot_stub)

# ------------------------------------------------------------------------------

ctl = sbpctl

$(ctl):
	sed -e 's,[@]BOOT_LIB_DIR[@],$(bootlibdir),g' \
		-e 's,[@]EFI_MACHINE_TYPE_NAME[@],$(EFI_MACHINE_TYPE_NAME),g' \
		< $(top_srcdir)/src/sbpctl.in > $(ctl)
	chmod a+x $(ctl)

bin_SCRIPTS += $(ctl)
CLEANFILES += $(ctl)

# ------------------------------------------------------------------------------

man_MANS = \
	man/sbpctl.1

EXTRA_DIST += \
	man/sbpctl.xml

XSLTPROC_FLAGS = \
	--nonet \
	--stringparam man.authors.section.enabled 0 \
	--stringparam man.copyright.section.enabled 0

man/%.1: man/%.xml
	$(XSLTPROC) -o $@ $(XSLTPROC_FLAGS) \
		http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl $<

CLEANFILES += $(man_MANS)

# ------------------------------------------------------------------------------

bashcompletion_data = \
	completion/bash/sbpctl

if ENABLE_BASH_COMPLETION
bashcompletion_DATA = $(bashcompletion_data)
endif
