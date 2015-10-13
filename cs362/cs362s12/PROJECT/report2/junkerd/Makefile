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
report:
	latex report.tex
	dvips -t letter report.dvi
	touch report.ps

# Report: PS -> PDF
pdf:report
	ps2pdf report.ps
	touch report.pdf

# Report clean
clean:
	$(RM) *.aux
	$(RM) *.dvi
	$(RM) *.log
	$(RM) *.out


# End
