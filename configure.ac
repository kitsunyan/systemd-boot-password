AC_INIT([systemd-boot-password], [0.9.4.232])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
	Makefile
	completion/Makefile
	completion/bash/Makefile
	man/Makefile
	src/Makefile
	src/efi/Makefile
])

AM_INIT_AUTOMAKE([foreign 1.11 -Wall -Wno-portability tar-pax no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])

AC_CANONICAL_HOST

AC_PROG_CC_C99
AC_CHECK_TOOL(OBJCOPY, objcopy)
PKG_PROG_PKG_CONFIG

SET_ARCH(X86_64, x86_64*)
SET_ARCH(IA32, i*86*)
SET_ARCH(MIPS, mips*)
SET_ARCH(AARCH64, aarch64*)

EFI_ARCH=`echo $host | sed "s/\(-\).*$//"`
AM_COND_IF(ARCH_IA32, [EFI_ARCH=ia32 EFI_MACHINE_TYPE_NAME=ia32])
AM_COND_IF(ARCH_X86_64, [EFI_MACHINE_TYPE_NAME=x64])
AM_COND_IF(ARCH_AARCH64, [EFI_MACHINE_TYPE_NAME=aa64])

AC_SUBST([EFI_ARCH])
AC_SUBST([EFI_MACHINE_TYPE_NAME])

AC_CHECK_HEADERS(efi/${EFI_ARCH}/efibind.h, [AC_DEFINE(HAVE_GNUEFI, 1, [Define if gnuefi is available]) have_gnuefi=yes],
	[AC_MSG_ERROR([*** gnuefi is required])])
efiroot=$(echo $(cd /usr/lib/$(${CC} -print-multi-os-directory); pwd))
EFI_LIB_DIR="$efiroot"
AC_ARG_WITH(efi-libdir, AS_HELP_STRING([--with-efi-libdir=PATH], [Path to EFI lib directory]),
	[EFI_LIB_DIR="$withval"], [EFI_LIB_DIR="$efiroot"])
AC_SUBST([EFI_LIB_DIR])
have_efi_lds=no
AC_ARG_WITH(efi-ldsdir, AS_HELP_STRING([--with-efi-ldsdir=PATH], [Path to EFI lds directory]),
	[EFI_LDS_DIR="$withval" && AS_IF([test -f "${EFI_LDS_DIR}/elf_${EFI_ARCH}_efi.lds"], [have_efi_lds=yes])],
	[AS_FOR([DIR], [EFI_LDS_DIR], ["${EFI_LIB_DIR}/gnuefi" "${EFI_LIB_DIR}"],
	[AS_IF([test -f "${EFI_LDS_DIR}/elf_${EFI_ARCH}_efi.lds"], [have_efi_lds=yes && break])])])
AS_IF([test "x$have_efi_lds" = xyes], [AC_SUBST([EFI_LDS_DIR])],
	[AC_MSG_ERROR([*** gnuefi is required])])
AC_ARG_WITH(efi-includedir, AS_HELP_STRING([--with-efi-includedir=PATH], [Path to EFI include directory]),
	[EFI_INC_DIR="$withval"], [EFI_INC_DIR="/usr/include"])
AC_SUBST([EFI_INC_DIR])

AC_PATH_PROG([XSLTPROC], [xsltproc])
AS_IF([test -z "$XSLTPROC"], AC_MSG_ERROR([*** xsltproc is required]))

AC_ARG_ENABLE([secure-boot-password], AS_HELP_STRING([--disable-secure-boot-password],
	[disallow password prompt with Secure Boot enabled]),
	[case "${enableval}" in
		yes) secure_boot_password=true ;;
		no) secure_boot_password=false ;;
		*) AC_MSG_ERROR([bad value ${enableval} for --disable-secure-boot-password]) ;;
	esac], [secure_boot_password=true])
AC_DEFINE_UNQUOTED([SECURE_BOOT_PASSWORD], `test ! "x$secure_boot_password" != "xfalse"; echo $?`,
	[Define if Secure Boot password is allowed])

AC_ARG_WITH([bashcompletiondir], AS_HELP_STRING([--with-bashcompletiondir=DIR],
	[Bash completions directory]), [], [AS_IF([$($PKG_CONFIG --exists bash-completion)],
	[with_bashcompletiondir=$($PKG_CONFIG --variable=completionsdir bash-completion)],
	[with_bashcompletiondir=${datadir}/bash-completion/completions])])
AM_CONDITIONAL(ENABLE_BASH_COMPLETION, [test "$with_bashcompletiondir" != "no"])
AX_NORMALIZE_PATH([with_bashcompletiondir])
AC_SUBST([bashcompletiondir], [$with_bashcompletiondir])

AC_SUBST([bootlibdir], ["${prefix}/lib/systemd/boot/efi-sbp"])

AC_OUTPUT
AC_MSG_RESULT([
$PACKAGE_NAME $PACKAGE_VERSION
prefix: ${prefix}
datarootdir: ${datarootdir}
arch: $EFI_ARCH
EFI machine type: $MACHINE_TYPE_NAME
EFI libdir: ${EFI_LIB_DIR}
EFI ldsdir: ${EFI_LDS_DIR}
EFI includedir: ${EFI_INC_DIR}
Allow Secure Boot password: ${secure_boot_password}
Bash completions dir: ${with_bashcompletiondir}
Boot lib dir: ${bootlibdir}
])
