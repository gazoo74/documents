#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

.PHONY: doc

all: doc

doc:
	make -C $@

KBUILD_OUTPUT = $(CURDIR)/kernel/linux-x86

kernel: initramfs.cpio
	make -C $@ CONFIG_INITRAMFS_SOURCE=$(CURDIR)/$<

$(KBUILD_OUTPUT)/arch/x86/boot/bzImage: initramfs.cpio
	make -C kernel CONFIG_INITRAMFS_SOURCE=$(CURDIR)/$<

runqemu: $(KBUILD_OUTPUT)/arch/x86/boot/bzImage
	qemu-system-$(shell uname -m) -kernel $< \
			 -initrd initramfs.cpio \
			 -append "console=ttyS0 rdinit=/bin/sh" \
			 -serial stdio
#			 -chardev stdio,id=tty -device isa-serial,chardev=tty
#			 -append "console=ttyS0 rdinit=/sbin/init -s CONSOLE=/dev/ttyS0" \

kernel_menuconfig:

kernel_%:
	make -C kernel $*

busybox_menuconfig:

busybox_%:
	make -C busybox $*

rootfs/bin/busybox:
#	make -C busybox install CONFIG_PREFIX=$(CURDIR)/rootfs CONFIG_STATIC=y CONFIG_EXTRA_CFLAGS="-m32" CONFIG_EXTRA_LDFLAGS="-m32"
	make -C busybox install CONFIG_PREFIX=$(CURDIR)/rootfs


rootfs/etc/inittab: busybox/busybox/examples/inittab
		cp $< $@

rootfs/init:
	make -C src
	cp src/helloworld $@

rootfs/dev/console:
	fakeroot -- mknod -m 622 $@ c 5 1

rootfs/dev/ttyS0:
	fakeroot -- mknod -m 622 $@ c 4 64

%.cpio:
	cd $(<D) && find . | cpio -H newc -o -R root:root >$(CURDIR)/$@

initramfs.cpio: rootfs/init rootfs/dev/console rootfs/dev/ttyS0 rootfs/etc/inittab

clean mrproper:
	for subdir in doc kernel; do make -C $$subdir $@; done


busybox-i686:
	wget --url https://busybox.net/downloads/binaries/$@
	chmod a+x $@

rootfs/bin/sh: rootfs/bin/busybox
	ln -sf $(<F) $@

#rootfs/bin/busybox: busybox-i686
#	install -d $(@D)/
#	cp $< $@



busybox-1.24.1.tar.bz2:
	wget --url https://busybox.net/downloads/$@

