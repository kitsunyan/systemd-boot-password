AM_V_CC = $(am__v_CC_$(V))
am__v_CC_ = $(am__v_CC_$(AM_DEFAULT_VERBOSITY))
am__v_CC_0 = @echo "  CC      " $@;

AM_V_CCLD = $(am__v_CCLD_$(V))
am__v_CCLD_ = $(am__v_CCLD_$(AM_DEFAULT_VERBOSITY))
am__v_CCLD_0 = @echo "  CCLD    " $@;

bootlib_DATA =
EXTRA_DIST =
CLEANFILES =

bootlibdir = \
	@bootlibdir@

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
	console.h \
	disk.h \
	graphics.h \
	linux.h \
	measure.h \
	pe.h \
	sha512.h \
	shim.h \
	util.h

systemd_boot_sources = \
	boot.c \
	console.c \
	disk.c \
	graphics.c \
	measure.c \
	pe.c \
	sha512.c \
	shim.c \
	util.c

systemd_boot_objects = $(systemd_boot_sources:.c=.o)
systemd_boot_solib = systemd_boot.so
systemd_boot = systemd-boot$(EFI_MACHINE_TYPE_NAME).efi

$(systemd_boot): $(systemd_boot_solib)
	$(AM_V_GEN) $(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	-j .dynsym -j .rel -j .rela -j .reloc $(EFI_FORMAT) $< $@

$(systemd_boot_solib): $(systemd_boot_objects)
	$(AM_V_CCLD) $(LD) $(efi_ldflags) $(systemd_boot_objects) \
	-o $@ -lefi -lgnuefi $(shell $(CC) -print-libgcc-file-name);
	@nm -D -u $@ | grep ' U ' && exit 1 || :

%.o: %.c $(systemd_boot_headers)
	$(AM_V_CC) $(CC) $(efi_cppflags) $(efi_cflags) -c $< -o $@

bootlib_DATA += \
	$(systemd_boot)

EXTRA_DIST += \
	$(systemd_boot_headers) \
	$(systemd_boot_sources)

CLEANFILES += \
	$(systemd_boot_objects) \
	$(systemd_boot_solib) \
	$(systemd_boot)

systemd_boot_stub_headers = \
	disk.h \
	graphics.h \
	linux.h \
	measure.h \
	pe.h \
	splash.h \
	util.h

systemd_boot_stub_sources = \
	disk.c \
	graphics.c \
	linux.c \
	measure.c \
	pe.c \
	splash.c \
	stub.c \
	util.c

systemd_boot_stub_objects = $(systemd_boot_stub_sources:.c=.o)
systemd_boot_stub_solib = systemd_boot_stub.so
systemd_boot_stub = linux$(EFI_MACHINE_TYPE_NAME).efi.stub

$(systemd_boot_stub): $(systemd_boot_stub_solib)
	$(AM_V_GEN) $(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	-j .dynsym -j .rel -j .rela -j .reloc $(EFI_FORMAT) $< $@

$(systemd_boot_stub_solib): $(systemd_boot_stub_objects)
	$(AM_V_CCLD) $(LD) $(efi_ldflags) $(systemd_boot_stub_objects) \
	-o $@ -lefi -lgnuefi $(shell $(CC) -print-libgcc-file-name)
	@nm -D -u $@ | grep ' U ' && exit 1 || :

%.o: %.c $(systemd_boot_stub_headers)
	$(AM_V_CC) $(CC) $(efi_cppflags) $(efi_cflags) -c $< -o $@

bootlib_DATA += \
	$(systemd_boot_stub)

EXTRA_DIST += \
	$(systemd_boot_stub_headers) \
	$(systemd_boot_stub_sources)

CLEANFILES += \
	$(systemd_boot_stub_objects) \
	$(systemd_boot_stub_solib) \
	$(systemd_boot_stub)
