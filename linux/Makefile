#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

.PHONY: doc

all: doc

doc kernel:
	make -C $@

runqemu: kernel/$(KBUILD_OUTPUT)/arch/x86/boot/bzImage
	qemu-system-i386 -kernel $< \
			 -append "console=ttyS0" \
			 -chardev stdio,id=tty -device isa-serial,chardev=tty

rootfs/init:
	make -C src
	cp src/helloworld $@

rootfs/dev/console:
	fakeroot -- mknod -m 622 $@ c 5 1

rootfs/dev/ttyS0:
	fakeroot -- mknod -m 622 $@ c 4 64

%.cpio:
	cd $(<D) && find . | cpio -H newc -o -R root:root >$(CURDIR)/$@

initramfs.cpio: rootfs/init rootfs/dev/console rootfs/dev/ttyS0

clean mrproper:
	for subdir in doc kernel; do make -C $$subdir $@; done

