NAME=Image\ Stream\ Synchronization\ for\ Visual\ Navigation
BBL=$(NAME).bbl
BIB=references.bib
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
IMG=
PDF=$(NAME).pdf
TEX=$(NAME).tex

GS_PDF=-sDEVICE=pdfwrite
GS_GRY=-sColorConversionStrategy=Gray -dProcessColorModel=/DeviceGray
GS_OPT=-dAutoRotatePages=/None -dCompatibilityLevel=1.4 -dNOPAUSE -dBATCH

PLT=platex --kanji=utf-8 -shell-escape

color: $(PDF)

bw: $(BLW)

# Convert PNG files to Encapsulated PS
%.eps: %.png
	convert $< $@

%.pdf: %.odg
	unoconv -f pdf $<

%.pdf: %.odt
	unoconv -f pdf $<

%.pdf: %.svg
	rsvg-convert -f pdf -o $@ $<

$(BBL): $(BIB)
	bibtex $(NAME)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

$(DVI): $(TEX) $(CLS)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

$(PDF): $(IMG) $(DVI) $(BBL)
#	dvipdfm $(DVI)
	dvipdfm -p a4 $(DVI)

$(BLW): $(PDF)
	gs $(GS_PDF) $(GS_GRY) $(GS_OPT) -sOutputFile=$(BLW) $(PDF)

clean:
	rm -f *.xbb *.aux *.dvi *.log *.bbl *.blg

wipe: clean
	rm -f $(IMG)
