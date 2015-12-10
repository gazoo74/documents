#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

.PHONY: all
all: doc

.PHONY: doc kernel
doc kernel:
	make -C $@

.PHONY: linux_download
linux_download:
	make -C kernel $@

.PHONY: clean mrproper
clean mrproper:
	for subdir in doc kernel; do make -C $$subdir $@; done

