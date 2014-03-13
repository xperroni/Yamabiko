# LaTex file name (without extension, escape spaces)
NAME=Weightless\ Neural\ Networks\ for\ Depth\ Estimation

# Bibliography file name (with extension)
BIB=references.bib

# Document class file name (with extension)
CLS=jsproceedings.cls

# Space-separated list of image files to be converted to PDF or EPS
# (extension should be ".pdf" or ".eps", depending on the case)
IMG=architecture_color.pdf architecture.pdf bands.pdf case.pdf dispatcher.pdf fourier.pdf gaussian.pdf hypercube.pdf maps_color.pdf maps.pdf perceptron.pdf sampler.pdf trie.pdf vg-ram.pdf weightless.pdf zeta.eps

# Intermediate file names used throughout the compilation process
BBL=$(NAME).bbl
BLW=$(NAME)\ BW.pdf
DVI=$(NAME).dvi
PDF=$(NAME).pdf
TEX=$(NAME).tex

# Options for converting PDF files to B&W
GS_PDF=-sDEVICE=pdfwrite
GS_GRY=-sColorConversionStrategy=Gray -dProcessColorModel=/DeviceGray
GS_OPT=-dAutoRotatePages=/None -dCompatibilityLevel=1.4 -dNOPAUSE -dBATCH

# LaTeX compiler command call
PLT=platex --kanji=utf-8 -shell-escape

# Compile a colored PDF
color: $(PDF)

# Compile a Black & White PDF
bw: $(BLW)

# Convert LibreOffice Draw files to PDF
%.pdf: %.odg
	unoconv -f pdf $<

# Convert JPG files to Encapsulated PS
%.eps: %.jpg
	convert $< $@

# Convert PNG files to Encapsulated PS
%.eps: %.png
	convert $< $@

# Merge bibliographic references into article
$(BBL): $(BIB)
	bibtex $(NAME)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

# Generate Device-Independent document from LaTeX file
$(DVI): $(TEX) $(CLS)
	$(PLT) $(TEX)
	$(PLT) $(TEX)

# Compile LaTeX document as PDF file
$(PDF): $(IMG) $(DVI) $(BBL)
	dvipdfm -p a4 $(DVI)

# Compile LaTeX document as B&W PDF file
$(BLW): $(PDF)
	gs $(GS_PDF) $(GS_GRY) $(GS_OPT) -sOutputFile=$(BLW) $(PDF)

# Remove all intermediate files, except image conversions
clean:
	rm -f *.xbb *.aux *.dvi *.log *.bbl *.blg

# Remove all intermediate files, including image conversions
wipe: clean
	rm -f $(IMG)
