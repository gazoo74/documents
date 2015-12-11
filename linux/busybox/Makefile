#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

.PHONY: busybox

all: busybox

busybox: busybox/busybox

busybox/busybox: busybox/.config
	make -C busybox CONFIG_STATIC=y CONFIG_EXTRA_CFLAGS="-m32" CONFIG_EXTRA_LDFLAGS="-m32"

busybox/.config:
	yes "" | make -C busybox oldconfig

busybox/Makefile:
	@echo "You need to provide your own sources into the $(CURDIR)/$(@D) directory!" >&2
	@echo "Have a look at https://www.busybox.net! or run one of the commands below:" >&2
	@echo "$$ git clone git://git.busybox.net/busybox $(CURDIR)/$(@D)" >&2
	@echo "or" >&2
	@echo "$$ make $(@D)_download" >&2
	@exit 1

busybox_download:
	wget -qO- https://busybox.net \
		| sed -n '/<li><b>/,/<\/li>/s,.*<a.*href="\(.*\).tar.bz2">BusyBox \(.*\)</a>.*,wget -qO- \1.tar.bz2 | tar xvj \&\& ln -sf busybox-\2 busybox \&\& exit 0,p' \
		| sh

busybox_menuconfig:

menuconfig: busybox_menuconfig

busybox_%:
	make -C busybox $*

clean:
	-make -C busybox clean

mrproper:
	-make -C busybox mrproper

distclean:
	rm -f busybox busybox*/ 