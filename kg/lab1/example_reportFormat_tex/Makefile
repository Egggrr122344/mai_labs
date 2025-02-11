filename=report

all: pdf text

pdf: ${filename}.tex
	pdflatex ${filename}.tex

text: html
	html2text -width 100 -style pretty ${filename}.html | sed -n '/./,$$p' | head -n-2 >${filename}.txt

html: ${filename}.tex
	htlatex ${filename}

ps:	dvi
	dvips -t letter ${filename}.dvi

dvi: ${filename}.tex
	latex ${filename}
	bibtex ${filename}||true
	latex ${filename}
	latex ${filename}

clean:
	rm -f ${filename}.{ps,pdf,log,aux,out,dvi,bbl,blg}
