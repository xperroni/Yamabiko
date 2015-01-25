NAME=Image\ Stream\ Synchronization\ for\ Visual\ Navigation
BBL=$(NAME).bbl
BIB=references.bib
BLW=$(NAME)\ BW.pdf
CLS=jsproceedings.cls
DVI=$(NAME).dvi
IMG=teach_replay.pdf pipeline.pdf indoors_teach.eps indoors_replay.eps outdoors_teach.eps outdoors_replay.eps  indoors_similarity_map.pdf indoors_pairing_teach.eps indoors_pairing_replay.eps outdoors_similarity_map.pdf outdoors_pairing_teach.eps outdoors_pairing_replay.eps indoors_shift_map.pdf outdoors_shift_map.pdf outdoors_similarity_map_30.pdf outdoors_shift_map_30.pdf outdoors_similarity_map_45.pdf outdoors_shift_map_45.pdf outdoors_similarity_map_60.pdf outdoors_shift_map_60.pdf
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
