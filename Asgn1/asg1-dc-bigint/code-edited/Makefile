# $Id: Makefile,v 1.7 2022-03-31 16:25:24-07 - - $

MKFILE      = Makefile
DEPSFILE    = ${MKFILE}.deps
NOINCL      = ci clean spotless tar
NEEDINCL    = ${filter ${NOINCL}, ${MAKECMDGOALS}}
GMAKE       = ${MAKE} --no-print-directory
GPPOPTS     = -std=gnu++20 -fdiagnostics-color=never
GPPWARN     = -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast
GPP         = g++ ${GPPOPTS} ${GPPWARN}
COMPILECPP  = ${GPP} -g -O0 ${GPPOPTS}
MAKEDEPSCPP = ${GPP} -MM ${GPPOPTS}

MODULES     = ubigint bigint libfns scanner debug util
CPPHEADER   = ${MODULES:=.h} iterstack.h relops.h
CPPSOURCE   = ${MODULES:=.cpp} main.cpp
EXECBIN     = ydc
OBJECTS     = ${CPPSOURCE:.cpp=.o}
MODULESRC   = ${foreach MOD, ${MODULES}, ${MOD}.h ${MOD}.cpp}
OTHERSRC    = ${filter-out ${MODULESRC}, ${CPPHEADER} ${CPPSOURCE}}
ALLSOURCES  = ${MODULESRC} ${OTHERSRC} ${MKFILE}
LISTING     = Listing.ps
TARFILE     = ydc-bigint.tar.gz

export PATH := ${PATH}:/afs/cats.ucsc.edu/courses/cse110a-wm/bin

all : ${EXECBIN}

${EXECBIN} : ${OBJECTS}
	${COMPILECPP} -o $@ ${OBJECTS}

%.o : %.cpp
	- checksource $<
	- cpplint.py.perl $<
	${COMPILECPP} -c $<

ci : check
	- cid -is ${ALLSOURCES}

check : ${ALLSOURCES}
	- checksource ${ALLSOURCES}
	- cpplint.py.perl ${CPPSOURCE}

tar : ${ALLSOURCES}
	tar cvfz ${TARFILE} $^

lis : ${ALLSOURCES}
	mkpspdf ${LISTING} ${ALLSOURCES} ${DEPSFILE}

clean :
	- rm ${OBJECTS} ${DEPSFILE} core ${EXECBIN}.errs

spotless : clean
	- rm ${EXECBIN} ${LISTING} ${LISTING:.ps=.pdf}


deps : ${CPPSOURCE} ${CPPHEADER}
	@ echo "# ${DEPSFILE} created $$(LC_TIME=C date)" >${DEPSFILE}
	${MAKEDEPSCPP} ${CPPSOURCE} >>${DEPSFILE}

${DEPSFILE} :
	@ touch ${DEPSFILE}
	${GMAKE} deps

again :
	${GMAKE} spotless deps ci all

submit : 
	submit cse111-wm.s22 asg1 ubigint.cpp \
	ubigint.h bigint.cpp bigint.h \
	debug.cpp debug.h iterstack.h \
	libfns.cpp libfns.h main.cpp relops.h \
	scanner.h scanner.cpp util.h \
	util.cpp Makefile README

ifeq (${NEEDINCL}, )
include ${DEPSFILE}
endif

