#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

O ?= linux-x86
KBUILD_OUTPUT ?= $(CURDIR)/$(O)
export KBUILD_OUTPUT

.PHONY: all
all: kernel

.PHONY: kernel
kernel: $(KBUILD_OUTPUT)/arch/x86/boot/bzImage

$(KBUILD_OUTPUT)/arch/x86/boot/bzImage: $(KBUILD_OUTPUT)/.config
	make -C linux $(@F)

$(KBUILD_OUTPUT)/.config: linux/Makefile $(CFG)
	install -d $(@D)/
	make -C linux tinyconfig

linux/Makefile:
	@echo "You need to provide your own kernel sources into the $(CURDIR)/$(@D) directory!" >&2
	@echo "Have a look at https://www.kernel.org! or run one of the commands below:" >&2
	@echo "$$ git clone git@github.com:torvalds/linux.git $(CURDIR)/$(@D)" >&2
	@echo "or" >&2
	@echo "$$ make $(@D)_download" >&2
	@exit 1

.PHONY: kernel_download linux_download
kernel_download linux_download:
	wget -qO- https://www.kernel.org/index.html \
		| sed -n '/<td id="latest_link"/,/<\/td>/s,.*<a.*href="\(.*\)">\(.*\)</a>.*,wget -qO- \1 | tar xvJ \&\& ln -sf linux-\2 linux,p' \
		| sh

kernel_menuconfig linux_menuconfig:

kernel_tinyconfig linux_tinyconfig:

kernel_% linux_%:
	make -C linux $*

.PHONY: clean
clean:
	rm -f $(KBUILD_OUTPUT)/arch/x86/boot/bzImage

.PHONY: mrproper
mrproper:
	rm -rf $(KBUILD_OUTPUT)/

