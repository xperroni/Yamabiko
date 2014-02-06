NAME=Weightless\ Neural\ Networks\ for\ Depth\ Estimation
BIB=references.bib
BBL=$(NAME).bbl
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
IMG=architecture_color.pdf architecture.pdf bands.pdf case.pdf dispatcher.pdf fourier.pdf gaussian.pdf hypercube.pdf maps_color.pdf maps.pdf perceptron.pdf sampler.pdf trie.pdf vg-ram.pdf weightless.pdf zeta.pdf
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
