NAME=Monocular\ Estimation\ of\ Spatial\ Displacement
BBL=$(NAME).bbl
BIB=references.bib
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
IMG=distances.pdf mode_sideways.pdf mode_straight.pdf mode_turning.pdf naming.pdf path.pdf properties.pdf sideways_absolute.pdf sideways_relative.pdf straight_absolute.pdf turning_absolute.pdf turning_relative.pdf
PDF=$(NAME).pdf
TEX=$(NAME).tex

GS_PDF=-sDEVICE=pdfwrite
GS_GRY=-sColorConversionStrategy=Gray -dProcessColorModel=/DeviceGray
GS_OPT=-dAutoRotatePages=/None -dCompatibilityLevel=1.4 -dNOPAUSE -dBATCH

PLT=platex --kanji=utf-8 -shell-escape

color: $(PDF)

bw: $(BLW)

%.pdf: %.odg
	unoconv -f pdf $<

$(BBL): $(BIB)
	bibtex $(NAME)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

$(DVI): $(TEX) $(CLS)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

$(PDF): $(IMG) $(DVI) $(BBL)
	dvipdfm -p a4 $(DVI)

$(BLW): $(PDF)
	gs $(GS_PDF) $(GS_GRY) $(GS_OPT) -sOutputFile=$(BLW) $(PDF)

clean:
	rm -f *.xbb *.aux *.dvi *.log *.bbl *.blg $(IMG)

