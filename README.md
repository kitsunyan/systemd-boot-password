# systemd-boot-password

systemd-boot-password is systemd-boot boot manager with password-protected editor.

## Building and Installing

The `gnu-efi` library, `docbook-xsl` and GNU autotools should be installed.

Run `./autogen.sh && make && sudo make install` to build and install systemd-boot-password to your system.

Arch Linux users can install [`systemd-boot-password`](https://aur.archlinux.org/packages/systemd-boot-password) package from AUR.

## Boot Manager Installing and Configuration

You can install boot manager using `sudo sbpctl install $esp`.

In `$esp/loader/loader.conf` you can add `editor 1` to enable kernel parameters editor (enabled by default).

For password protection, run `sbpctl generate` and enter desired password. This tool will generate SHA-512 hash sum which you can add to `loader.conf` file: `password $sha512sum`. Boot manager will prompt password when you try to open editor with `e` key.

You can create a standalone EFI application with Linux EFI and initramfs: `sudo sbpctl standalone --initrd /boot/initramfs-linux.img /boot/vmlinuz-linux $esp/linux.efi`.

You can create a single `loader.conf` file with all entries, divided by empty line:

```
default entry0
timeout 0
editor 1
password 61cf00560dff557e0cd498fe

title Linux
efi /linux.efi
options	root=/dev/sda2 rw
```

You can also include your configuration to EFI binary using `--include` command option for `install`. In this case `loader.conf` should be placed at `/etc/sbp`.

You can automatically sign your EFI applications for Secure Boot using `--sign` option for `install` and `standalone`. To do this you should install `sbsigntools` and place your `db.crt` and `db.key` to `/etc/sbp` directory.

Consider changing permissions for `/etc/sbp` to 700 and fmask for `$esp` to 0077 or 0177 in `/etc/fstab`.

## License

This program is licensed with LGPLv2.1+.
