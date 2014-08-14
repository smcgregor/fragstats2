#
# This makefile has two main targets.  The first is the default
# unix compilation of fragstats.  The second is the target needed
# to compile fragstats under DOS with the DJGPP compiler.  This
# also requires DMAKE in order to build a response file, otherwise
# the link command would exceed the DOS command line limit of
# 128 characters.
#

#
# Choose a level of diagnostic information
#
#CFLAGS = -v -c -O -g -ansi -W -Wall -Wshadow -Wconversion -Wredundant-decls -finline-functions
CFLAGS = -c -O

CC = gcc

HEADERS = stats.h routines.h erdas.h

OBJECTS1 = fragstats.o setup.o prompt.o getsizes.o space.o \
	patchsiz.o allocmem.o freemem.o\
	readimg.o readasci.o readsvf.o readbin.o readerd.o readhd.o \
	rdweight.o rdclass.o \
	writeimg.o writeasc.o writebin.o writesvf.o writeerd.o \
	wrtpatch.o hdpatch.o hdclass.o hdland.o \
	patch.o class.o land.o getpatch.o \
	nndist.o bndedge.o ckedge.o landedge.o corearea.o \
	regress.o rascont.o readidri.o ckback.o

OBJECTS2 = setshort.o getshort.o setfloat.o getfloat.o setint.o \
	getint.o getchar.o setchar.o usage.o

fragstats: $(OBJECTS1) $(OBJECTS2)
	$(CC) -o fragstats $(OBJECTS1) $(OBJECTS2) -lm 

#
# DJGPP version
#
djgpp: $(OBJECTS1) $(OBJECTS2)  
	$(CC) -g -o fragstat $(OBJECTS1) $(OBJECTS2) -lm
	coff2exe -s n:/marks/djgpp/bin/go32.exe fragstat 
