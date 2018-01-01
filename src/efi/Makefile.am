CLEANFILES =
bootlib_DATA =

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
	util.h \
	console.h \
	graphics.h \
	pe.h \
	measure.h \
	disk.h \
	shim.h \
	sha512.h

systemd_boot_sources = \
	util.c \
	console.c \
	graphics.c \
	pe.c \
	disk.c \
	measure.c \
	boot.c \
	shim.c \
	sha512.c

systemd_boot_objects = $(systemd_boot_sources:.c=.o)
systemd_boot_solib = systemd_boot.so
systemd_boot = systemd-boot$(EFI_MACHINE_TYPE_NAME).efi

$(systemd_boot): $(systemd_boot_solib)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	  -j .dynsym -j .rel -j .rela -j .reloc $(EFI_FORMAT) $< $@

$(systemd_boot_solib): $(systemd_boot_objects)
	$(LD) $(efi_ldflags) $(systemd_boot_objects) \
		-o $@ -lefi -lgnuefi $(shell $(CC) -print-libgcc-file-name);
	nm -D -u $@ | grep ' U ' && exit 1 || :

%.o: %.c $(systemd_boot_headers)
	$(CC) $(efi_cppflags) $(efi_cflags) -c $< -o $@

bootlib_DATA += \
	$(systemd_boot)

CLEANFILES += \
	$(systemd_boot_objects) \
	$(systemd_boot_solib) \
	$(systemd_boot)

systemd_boot_stub_headers = \
	util.h \
	pe.h \
	disk.h \
	graphics.h \
	splash.h \
	measure.h \
	linux.h

systemd_boot_stub_sources = \
	util.c \
	pe.c \
	disk.c \
	graphics.c \
	splash.c \
	linux.c \
	measure.c \
	stub.c

systemd_boot_stub_objects = $(systemd_boot_stub_sources:.c=.o)
systemd_boot_stub_solib = systemd_boot_stub.so
systemd_boot_stub = systemd-boot$(EFI_MACHINE_TYPE_NAME).efi.stub

$(systemd_boot_stub): $(systemd_boot_stub_solib)
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic \
	  -j .dynsym -j .rel -j .rela -j .reloc $(EFI_FORMAT) $< $@

$(systemd_boot_stub_solib): $(systemd_boot_stub_objects)
	$(LD) $(efi_ldflags) $(systemd_boot_stub_objects) \
		-o $@ -lefi -lgnuefi $(shell $(CC) -print-libgcc-file-name);
	nm -D -u $@ | grep ' U ' && exit 1 || :

%.o: %.c $(systemd_boot_stub_headers)
	$(EFI_CC) $(efi_cppflags) $(efi_cflags) -c $< -o $@

bootlib_DATA += \
	$(systemd_boot_stub)

CLEANFILES += \
	$(systemd_boot_stub_objects) \
	$(systemd_boot_stub_solib) \
	$(systemd_boot_stub)
