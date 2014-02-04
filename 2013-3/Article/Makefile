NAME=Monocular\ Estimation\ of\ Spatial\ Displacement
BBL=$(NAME).bbl
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
PDF=$(NAME).pdf
TEX=$(NAME).tex
BIB=references.bib

GS_PDF=-sDEVICE=pdfwrite
GS_GRY=-sColorConversionStrategy=Gray -dProcessColorModel=/DeviceGray
GS_OPT=-dAutoRotatePages=/None -dCompatibilityLevel=1.4 -dNOPAUSE -dBATCH

PLT=platex --kanji=utf-8 -shell-escape

$(BBL): $(BIB)
	bibtex $(NAME)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

$(DVI): $(TEX) $(CLS)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

$(PDF): $(DVI) $(BBL)
	dvipdfm -p a4 $(DVI)

$(BLW): $(PDF)
	gs $(GS_PDF) $(GS_GRY) $(GS_OPT) -sOutputFile=$(BLW) $(PDF)

color: $(PDF)

bw: $(BLW)

clean:
	rm -f *.xbb *.aux *.dvi *.log *.bbl *.blg

