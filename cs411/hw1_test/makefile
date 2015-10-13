TARGET=main
HTML=main_html
SRC=dining_test.cpp

default: all

all: cpp pdf

cpp: matrix_unraveled.cpp matrix_threaded.cpp dining.cpp switch_parser.hpp
	g++ -lboost_thread-mt -Wall -Werror dining.cpp -o out_dining.exe	
	g++ -lboost_thread-mt -Wall -Werror matrix_threaded.cpp -o out_matrix_threaded.exe	
	g++ -Wall -Werror matrix_unraveled.cpp -o out_matrix_unraveled.exe	

both: pdf html

dvi: ${TARGET}.tex 
#	pygmentize the input source file -- THIS NAME SHOULD BE SAFE
#	pygmentize -f latex -o __${SRC}.tex ${SRC}
#	run latex twice to get references correct
	latex ${TARGET}.tex
#	you can also have a bibtex line here
#	bibtex $(TARGET).tex
	latex $(TARGET).tex
#	remove the pygmentized output to avoid cluttering up the directory
#	rm __${SRC}.tex

ps: dvi
	dvips -R -Poutline -t letter ${TARGET}.dvi -o ${TARGET}.ps

pdf: ps
	ps2pdf ${TARGET}.ps


html:
	cp ${TARGET}.tex ${HTML}.tex
	latex ${HTML}.tex
	latex2html -split 0 -noshow_section_numbers -local_icons -no_navigation -noinfo -noaddress ${HTML}

	sed 's/<BR><HR>//g' < ${HTML}/index.html > ${HTML}/index2.html
	mv ${HTML}/index2.html ${TARGET}.html
	rm ${HTML}.*
	rm -rf ${HTML}



