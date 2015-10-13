#
#	file:   Makefile
#	auth:   Ian Shearin
#	proj:   CS 362
#	desc:   I'm lazy.
#




# Global Options


# Commands
RM = rm -f




.PHONY:all report pdf clean

all:report




# Report: TEX -> PS
report: snip-testDrawCard.tex snip-checkDrawCard.tex snip-testAll.tex
	latex report.tex
	latex report.tex
	dvips -t letter report.dvi

# Report: PS -> PDF
pdf:report
	ps2pdf report.ps

# Pygmentize anything
%.tex:%.c
	pygmentize -f latex -o $@ -O linenos=1 $<

# Report clean
clean:
	$(RM) *.aux
	$(RM) *.dvi
	$(RM) *.log
	$(RM) *.out
	$(RM) snip-*.tex


# End
