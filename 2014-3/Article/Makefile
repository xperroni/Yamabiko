NAME=Visual\ Localization\ for\ Autonomous\ Navigation
BBL=$(NAME).bbl
BIB=references.bib
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
PDF=$(NAME).pdf
TEX=$(NAME).tex

IMG=dis.pdf drift.pdf slide_cc.pdf selection_0.pdf selection_6.pdf indoors_teach.eps indoors_replay.eps outdoors_teach.eps outdoors_replay.eps cupido_01.eps cupido_02.eps cupido_03.eps cupido_04.eps shifts_indoors.pdf shifts_outdoors.pdf shifts_turn_left.pdf shifts_turn_right.pdf

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
