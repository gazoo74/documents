#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

.PHONY: doc

all: doc

doc kernel:
	make -C $@

clean mrproper:
	for subdir in doc kernel; do make -C $$subdir $@; done

