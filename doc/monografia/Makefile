all: main.tex chapters/*.tex references.bib
	pdflatex main.tex
	bibtex main
	pdflatex main.tex

clean:
	rm *.aux *.toc *.bbl *.bcf *.blg *.log *.pdf
