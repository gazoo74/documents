#
# Copyright 2015 Gaël PORTAY <gael.portay@gmail.com>
#
# Licensed under the MIT license.
#

PDFLATEX	?= pdflatex
SRC		:= $(wildcard *.tex)
PDF		:= $(SRC:.tex=.pdf)

.PHONY: all
all: $(PDF)

$(PDF):

%.pdf: %.tex
	$(PDFLATEX) $< $@

.PHONY: clean
clean:
	rm -f *.pdf
	rm -f *.aux *.lof *.log *.lot *.fls *.out *.toc *.dvi *-converted-to.*

.PHONY: mrproper
mrproper:

