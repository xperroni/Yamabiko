NAME=Mobile\ Robot\ Path\ Drift\ Estimation\ using\ Visual\ Streams
BBL=$(NAME).bbl
BIB=references.bib
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
IMG=methods.pdf divs.pdf vector.pdf slide_cc.pdf selection.pdf entrance_3l_turn.pdf car_park_3l_turn.pdf corridor_3d_turn.pdf changes.pdf pedestrian.pdf environments.pdf clutter.pdf car_park_3l_steer_left.pdf car_park_3l_steer_right.pdf entrance_3l_steer_right.pdf
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
	dvipdfm -p a4 $(DVI)

$(BLW): $(PDF)
	gs $(GS_PDF) $(GS_GRY) $(GS_OPT) -sOutputFile=$(BLW) $(PDF)

clean:
	rm -f *.xbb *.aux *.dvi *.log *.bbl *.blg

wipe: clean
	rm -f $(IMG)
