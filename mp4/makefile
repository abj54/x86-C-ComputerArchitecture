# makefile for mp3's testpack
# $pcex/makefile with one additional include dir, and timepack_sapc.c
# compiled and loaded.  Also UNIX build for testpack.
# Usage: 
#  for SAPC: make C=testpack   
#  for UNIX: make testpack
#
# makefile for building C or assembly language programs for the
# Standalone 486 or Pentium IBM PC running in 32-bit protected mode, 
# or "SA PC" for short.
# Single-source-file makefile--needs editing for multiple-module programs

# For a C source, say myprog.c, override the "C=ctest" below by
#  setting C in the make command line: "make C=myprog myprog.lnx"
#  or just "make C=myprog", since $(C).lnx is the first make target.
#  Then myprog.lnx is ready for downloading via "mtip -f myprog.lnx"
#  (followed by ~r and ~d inside the mtip program)
C = ctest



# also "make clean" to clean up non-source files in a directory

# system directories needed for compilers, libraries, header files--
# assumes the environment variables SAPC_TOOLS, SAPC_GNUBIN, and SAPC_SRC
# are set up, usually by the ulab module
PC_LIB = $(SAPC_TOOLS)/lib
PC_INC = $(SAPC_TOOLS)/include
TIMING = $(SAPC_TOOLS)/timing

PC_CC   = $(SAPC_GNUBIN)/i386-gcc
# development build: -g provides debugging support, slows down execution
# Use -O2 optimization, no -g on final timing runs!
PC_CFLAGS_DEV = -g  -Wall -Wno-implicit -Wshadow -I$(PC_INC) -I$(TIMING)
PC_CFLAGS_OPT = -O2 -Wall -Wno-implicit -Wshadow -I$(PC_INC) -I$(TIMING)
# make default PC_CFLAGS be PC_CFLAGS_DEV
O = DEV
# allow user to override DEV via O=OPT on the command line
PC_CFLAGS = $(PC_CFLAGS_$(O)) 

PC_AS   = $(SAPC_GNUBIN)/i386-as
PC_LD   = $(SAPC_GNUBIN)/i386-ld
PC_NM   = $(SAPC_GNUBIN)/i386-nm

# File suffixes:
# .c	C source (often useful both for UNIX host and SAPC)
# .s 	assembly language source
# .opc  relocatable machine code, initialized data, etc., for SA PC
# .lnx  executable image (bits as in memory), for SA PC (Linux a.out format)
# .syms text file of .lnx's symbols and their values (the "symbol table")
# Symbol file "syms"--for most recently built executable in directory

# PC executable--tell ld to start code at 0x1000e0, load special startup
# module, special PC C libraries--
# Code has 0x20 byte header, so use "go 100100" (hopefully easier to
# remember than 100020)
# This executable also contains  timepack_sapc.opc
$(C).lnx: $(C).opc timepack_sapc.opc $(PC_LIB)/libc.a \
			$(PC_LIB)/startup.opc $(PC_LIB)/startup0.opc
	$(PC_LD) -N -Ttext 1000e0 -o $(C).lnx \
		$(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
		$(C).opc timepack_sapc.opc $(PC_LIB)/libc.a
	rm -f syms;$(PC_NM) -n $(C).lnx>$(C).syms;ln -s $(C).syms syms

# tell gcc to use $(PC_INC) for #include <...> headers--
$(C).opc: $(C).c
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -I$(TIMING) -c -o $(C).opc $(C).c

timepack_sapc.opc: timepack_sapc.c
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -I$(TIMING) -c \
			-o timepack_sapc.opc timepack_sapc.c

$(A).opc: $(A).s
	$(PC_AS) -o $(A).opc $(A).s

# UNIX version--set include dir via CFLAGS, for makefile's built-in rules
# for C compilation
# The suffixes are used to choose between timepack_unix_opt.o and 
# timepack_unix_dbg.o in the TIMING directory
CFLAGS_DEV = -g -Wall -Wno-implicit -Wshadow -I$(TIMING)
SUFFIX_DEV = _dbg
CFLAGS_OPT = -O2 -Wall -Wno-implicit -Wshadow -I$(TIMING)
SUFFIX_OPT = _opt
# change to following to CFLAGS_OPT and SUFFIX_OPT for optimized versions
# also "make clean" when switching over
CFLAGS = $(CFLAGS_DEV)
SUFFIX = $(SUFFIX_DEV)

testpack: testpack.o $(TIMING)/timepack_unix$(SUFFIX).o 
	gcc -o testpack testpack.o $(TIMING)/timepack_unix$(SUFFIX).o

itimes: itimes.o $(TIMING)/timepack_unix$(SUFFIX).o 
	gcc -o itimes itimes.o $(TIMING)/timepack_unix$(SUFFIX).o 

clean:
	rm -f *.o *.opc *.syms *.lnx core syms
