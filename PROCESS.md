# The Way I Research Alone

*Already five months have passed since I arrived at the University of Tsukuba for my Ph.D. course. In our [Intelligent Robot Laboratory](http://roboken.esys.tsukuba.ac.jp/) new students usually enroll in April, spend the first semester learning the basics of robot programming and working out their research subjects, and then in the second semester take part in the three* Yamabico symposiums *organized every year sometime around late Summer, middle Autumn and early Spring. Not me, though: having enrolled in October, I was required to take part in the Autumn and Spring symposiums while still very much figuring what to do and how to go about it.*

*It's alright, though: I've always preferred to learn by doing. Besides, I already had experience on writing technical reports and giving presentations from my M.Sc. years and work at LG, so I mostly applied what I already knew. Now, as the semester comes to an end, I decided to take the time to write down a record of the tools I used, the little tweaks and references I resorted to, and the general shape of my work process. I do this largely for my own good, though I'd be pleased if it proved useful to others as well.*

*I'll try to keep this document updated as I further develop my work practices. Judging from the pace of my first semester at Tsukuba, I doubt I'll have the time to note these down immediately, but I'll be sure to come back and do it when I get the chance.*

– Helio Perroni Filho, 2014-03-05

## Overview

This is a short summary of the tools I use for research and article writing. The next sections elaborate on each one, providing references and usage tips.

* [Linux](http://www.linux.org/) as basic development platform;
* [Code::Blocks](http://www.codeblocks.org/) for C++ development;
* [Python](http://www.python.org/) with [numpy](http://www.numpy.org/) / [scipy](http://scipy.org/) and [matplotlib](http://matplotlib.org/) for ad-hoc number-crunching and plot drawing;
* [LaTeX](http://en.wikibooks.org/wiki/LaTeX) for writing articles;
* [LibreOffice Draw](http://www.libreoffice.org/features/draw/) for diagrams and vectorial drawing in general;
* [LibreOffice Impress](http://www.libreoffice.org/features/impress/) for presentations;
* [git](http://git-scm.com/) for version control;
* [github](https://github.com/) as publishing platform.

Usually when performing research, I write most of my code in C++ or Python. I also use a lot of Python scripting to automate simple tasks (e.g. batch file renaming), reformat data files and to plot results. Articles and reports are written in LaTeX, with diagrams drawn on LibreOffice Draw. If I have to make a presentation, I create it on LibreOffice Impress. I often write down my intended speech in the [Notes view](http://presentationsoft.about.com/od/openofficeimpress/ig/OpenOffice-Slide-Views/notes_pages.htm) of the slideshow, both as a way to practice it and to work as cues during the actual presentation.

All my files, both source code and article contents, are version-controlled using `git`. I do a lot of small commits containing two or three files, concerning small changes; I find this works better than doing large commits that cannot be attributed to any single requirement, improvement or bug. Using `git` makes it simple to publish code, data or other contents to the web using `github`.

I like to treat every article as a separate project, with files sorted in subfolders like this:

* `Article`: LaTeX sources, including any images used;
* `Code`: source code used in the research;
* `Data`: data files, scripts for drawing plots and the like;
* `Presentation`: slides for the article's presentation;
* `Presentation/Drafts`: earlier presentation drafts.

The reason I keep my presentation drafts as separate files in a subfolder is because perfectly fine presentation sections often have to be changed or removed due to time restrictions. I find it's a shame to throw away good work that could still get salvaged someday; this makes them easier to recover, if I ever get the opportunity to reuse them.

When I'm stuck with some problem my first step is always to do a Google search. Usually that's enough to find an answer, but if nothing turns up then I ask a question on one of the [Stack Exchange](http://stackexchange.com/) sites, usually [Stack Overflow](http://stackoverflow.com/).

## Linux

This will go without saying for most computer science researchers, but [Linux](http://www.linux.org/) is the most convenient platform for academic work. This is largely the result of a positive feedback loop: Linux (and the UNIX that preceded it) has long been popular in academia, so academic software developers tend to target it, so researchers tend to use it. The widespread use of package managers across [distributions](http://distrowatch.com/) also makes downloading and installing new tools very simple, without the need to deal with compilation issues, dependencies and so on.

Over ten years as a user I have never been able to settle with any single distribution. Most people gravitate towards the [major ones](http://distrowatch.com/dwres.php?resource=major); as for myself, for the last few years I have been using [Linux Mint](http://www.linuxmint.com/) and its original Cinnamon desktop environment, though I am currently experimenting with the Mint KDE variant. Contrary to conventional wisdom, in my experience the so-called "fragmentation" of Linux has never been a problem: I have always been able to use the tools I needed in whatever distribution I used at the time. That said I try to stick to the bigger, most popular ones or their immediate derivatives, where packages tend to be more actively maintained.

## Code::Blocks

C++ can be bothersome at times (to say nothing of plain C), but as far as native software development is concerned it remains our best option. The wealth of libraries available for the language (such as [boost](http://boost.org/) and [OpenCV](http://opencv.org/)) makes a good case for its adoption too, and helps to mitigate some of its worse shortcomings. Adopting a good IDE will also take you a long way towards making development more comfortable.

[Code::Blocks](http://www.codeblocks.org/) is a free, multi-platform C++ IDE that's relatively lightweight and simple to use. It provides good support to all basic development chores, such as project building, debugging, search-and-replace across files, etc. It also includes plug-ins for somewhat more advanced tasks, such as source version control and dynamic analysis with [Valgrind](http://valgrind.org/).

Over the six-odd years since I discovered Code::Blocks, every now and again I try out a different C++ IDE (e.g. [Eclipse CDT](https://www.eclipse.org/cdt/) or [CodeLite](http://codelite.org/)) and so far I have always come back. That said avoid version 12.11 if you can: due to then-ongoing implementation changes it lacks some [features](http://forums.codeblocks.org/index.php?topic=18208.0) that have since been reimplemented in version 13.12.

## Python

[Python](http://www.python.org/) is a script language noted for its intuitiveness, large [standard library](http://docs.python.org/2.7/library/index.html) and even larger collection of [external packages](https://pypi.python.org/pypi), covering pretty much anything one might want to do with it. Its performance is somewhat lacking compared to native languages such as C++ (though there have been efforts to [remedy](http://cython.org/) that), but when ease of development is preferred over computing speed – as in [exploratory programming](http://en.wikipedia.org/wiki/Exploratory_programming) or for quick-and-dirty task automation – it is by far one of the best alternatives.

Two of the Python packages most useful for researchers are [numpy](http://www.numpy.org/) and its companion [scipy](http://scipy.org/). Both implement many classes and algorithms useful for numerical and scientific computing, sidestepping Python's performance limitations by implementing critical code in C / C++. Together they turn Python into a very convenient tool for prototyping and data analysis. Visualization of results is possible with the help of [matplotlib](http://matplotlib.org/), a powerful (if sometimes counter-intuitive) plotting library which can draw to the screen as well as several file formats. It all comes in very handy when I need to generate plots to use as figures in my articles: use `numpy` / `scipy` to preprocess and format the data, and `matplotlib` to render it as a plot in a SVG file or equivalent.

Currently Python is in the middle of a transition from the 2.x series (which is still required by older code bases) and the 3.x series (which implemented several important revisions to the language, but broke compatibility to existing code as a result). Personally I recommend sticking to 2.x for the time being, as that's what all major Linux distributions are still using for their own scripts, and only make the jump to 3.x when it starts being offered by default.

Like any useful skill, Python scripting grows on you over time, but the basics of the language aren't hard to get. [Dive Into Python](http://www.diveintopython.net/) was my first textbook, and while it's hopelessly outdated by now it remains a good introduction to most fundamental concepts. For a more up-to-date resource, I recommend [The Python Tutorial](http://docs.python.org/2.7/tutorial/index.html). Python is today a very popular language, so when you're stuck you can simply do a search on Google, or maybe [Stack Overflow](http://stackoverflow.com/).

## LaTeX

[LaTeX](http://en.wikibooks.org/wiki/LaTeX) is a markup language and associated set of packages and command line tools widely used for article writing in academic circles, particularly mathematics. It is highly regarded for its ability to provide powerful formatting features while letting authors concentrate on writing content. Its ability to automatically manage labeling and formatting of references is also very praised. Finally, since LaTeX documents are plain-text files, writers are free to select whatever text editor they like best.

Compiling a LaTeX file along with associated images and other resources into a final document (e.g. a PDF file) can quickly become a project in itself. After contending with the tools for a long time, I finally decided to take the time and write a template Makefile to manage the compilation of my articles:

```Makefile
# LaTex file name (without extension, escaped spaces)
NAME=

# Bibliography file name (with extension)
BIB=

# Document class file name (with extension)
CLS=

# Space-separated list of image files to be converted to PDF or EPS
# (extension should be ".pdf" or ".eps", depending on the case)
IMG=

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

# Convert LibreOffice Text files to PDF
%.pdf: %.odt
    unoconv -f pdf $<

# Convert Scalable Vector Graphics files to PDF
%.pdf: %.svg
    rsvg-convert -f pdf -o $@ $<

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

# Remove all intermediary files, including image conversions
wipe: clean
    rm -f $(IMG)
```

Apart from the usual LaTeX toolchain (i.e. `platex`, `bibtex` and `dvipdfm`), this template also expects [unoconv](http://dag.wiee.rs/home-made/unoconv/), [ImageMagick](http://www.imagemagick.org/) and [Ghostscript](http://www.ghostscript.com/) to be installed on your environment. Most Linux distributions will have these packages available on their package managers. Besides managing compilation of LaTeX documents, the Makefile will also convert vector drawings (expected to be LibreOffice Draw files, extension ".odg") and raster images (JPEG and PNG) to PDF and EPS respectively, formats compatible to the LaTeX tools.

The template also assumes that bibliography entries are kept in a separate [BibTeX](http://en.wikibooks.org/wiki/LaTeX/Bibliography_Management#BibTeX) file. Many [bibliography sites](http://tex.stackexchange.com/questions/143/what-are-good-sites-to-find-citations-in-bibtex-format) produce references in the BibTeX format, so an article's bibliography can be gathered mostly by copy-and-pasting. If however you prefer to work with embedded references, removing all mentions to the `$(BBL)` rule should be enough to adapt it to your uses.

When starting a new project, create a copy of the Makefile above on the same folder containing the LaTeX, BibTeX and image file, then fill the first four variables as required. For example, in the Makefile of [my first Yamabico submission](https://github.com/xperroni/Yamabiko/tree/master/2013-2), the following values can be found:

```Makefile
NAME=Weightless\ Neural\ Networks\ for\ Depth\ Estimation

BIB=references.bib

CLS=jsproceedings.cls

IMG=architecture_color.pdf architecture.pdf bands.pdf case.pdf dispatcher.pdf fourier.pdf gaussian.pdf hypercube.pdf maps_color.pdf maps.pdf perceptron.pdf sampler.pdf trie.pdf vg-ram.pdf weightless.pdf zeta.eps
```

Calling `make` or `make color` on a shell environment will compile a color PDF file, while `make bw` will additionally call `gs` to build a black & white version.

After changes to the document class or bibliography it's usually recommended to remove all intermediate files before starting a new compilation. Converted image files however can often be kept, and they take comparatively long to generate, so `make clean` will not remove these; call `make wipe` if you want to remove *all* intermediate files including image conversions. Notice that either way, the final PDF output file is never removed.

For articles that don't require support for East Asian languages, a variant of the template above can be used, substituting `pdflatex` for `platex`. The advantage is that PNG files can be used directly on articles, instead of being first converted to EPS. Therefore only vectorial drawings have to be converted.

```Makefile
# LaTex file name (without extension, escape spaces)
NAME=

# Bibliography file name (with extension)
BIB=

# Document class file name (with extension)
CLS=

# Space-separated list of image files to be converted to PDF
IMG=

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
PLT=pdflatex

# Compile a colored PDF
color: $(PDF)

# Compile a Black & White PDF
bw: $(BLW)

# Convert LibreOffice Draw files to PDF
%.pdf: %.odg
    unoconv -f pdf $<

# Merge bibliographic references into article
$(BBL): $(BIB)
    bibtex $(NAME)

# Generate Device-Independent document from LaTeX file
$(DVI): $(TEX) $(CLS)
    $(PLT) $(TEX)

# Compile LaTeX document as PDF file
$(PDF): $(IMG) $(DVI) $(BBL)
    $(PLT) $(TEX)
    $(PLT) $(TEX)

# Compile LaTeX document as B&W PDF file
$(BLW): $(PDF)
    gs $(GS_PDF) $(GS_GRY) $(GS_OPT) -sOutputFile=$(BLW) $(PDF)

# Remove all intermediate files, except image conversions
clean:
    rm -f *.xbb *.aux *.dvi *.log *.bbl *.blg

# Remove all intermediate files, including image conversions
wipe: clean
    rm -f $(IMG)
```

The LaTeX markup language is relatively simple and quick to learn, but some of its more advanced features (particularly the ones related to images and mathematical formula formating) can be challenging. The [WikiBooks reference on LaTeX](http://en.wikibooks.org/wiki/LaTeX/) and the [Tex](http://tex.stackexchange.com/) site on Stack Exchange are often good references for when you're stuck. If that fails, Google is your friend.

## LibreOffice

Some things are best done in plain text, others can be more conveniently achieved with a GUI. Personally I feel that vector diagrams and presentations fall in the later category. Since I am using Linux, it only makes sense that I use [Draw](http://www.libreoffice.org/features/draw/) and [Impress](http://www.libreoffice.org/features/impress/) from [LibreOffice](http://www.libreoffice.org/) to make these.

The conventional wisdom is that GUI applications are simpler to use than command-line equivalents; even if that is true, "simpler" does not necessarily mean "easy", and that's certainly the case for LibreOffice. Therefore I recommend any new users to take their time to become familiar with the suite, especially its context menus (a lot of drawing properties can be manipulated through context menus, with much more precision and speed than by drag-and-dropping). Fortunately there is a lot of [references](https://www.libreoffice.org/get-help/documentation/) and [forums](http://ask.libreoffice.org/en/questions/) around to study.

LibreOffice is sometimes accused of being buggy, and while it's certainly not above the odd glitch it's generally stable enough for my needs. There is this one problem with the Notes view on Impress that left me pulling hairs once, but eventually I [found a way around it](http://ask.libreoffice.org/en/question/18353/impress-slide-content-not-shown-on-notes-page/).

## git & github

Version control is an important aspect of any relatively large software project, but I usually extend this to other contents such as data files and documents as well. This is both a prevention against accidental changes, and a convenience for when I want to publish those files on the web. Using `git` as my version control tool, I can easily push everything to a web-accessible public repository at [github](https://github.com/). `git`'s interface is notoriously awkward sometimes, but with practice and the help of [extensive references](http://git-scm.com/doc) it's not so hard to get by.
