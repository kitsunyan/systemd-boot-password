# systemd-boot-password

systemd-boot-password is systemd-boot boot manager with password-protected editor.

## Building and Installing

The `gnu-efi` library and GNU autotools should be installed.

Run `./autogen.sh && ./configure && make && sudo make install` to build and install systemd-boot-password to your system.

Arch Linux users can install [`systemd-boot-password`](https://aur.archlinux.org/packages/systemd-boot-password) package from AUR.

## Boot Manager Installing and Configuration

You can install boot manager using `sudo sbp-install $esp`.

In `$esp/loader/loader.conf` you can add `editor 1` to enable kernel parameters editor (enabled by default).

For password protection, run `sbp-generate` and enter desired password. This tool will generate SHA-512 hash sum which you can add to `loader.conf` file: `password $sha512sum`. Boot manager will prompt password when you try to open editor with `e` key.

## License

This program is licensed with LGPLv2.1+.
