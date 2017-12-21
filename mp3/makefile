# makefile: makefile for assembly-language examples
# 
# Assumes a matched pair of sources, e.g., myprog.s and myprogc.c,
# an assembler source for a function and its C driver.

# Usage: For an assembler source, say myprog.s, with a corresponding 
#  C driver myprogc.c, set A to the basename in the make command line: 
#  "make A=myprog myprog.lnx" or just "make A=myprog"
#  (since $(A).lnx is the first make target)
#  Then myprog.lnx is ready for downloading via "mtip -f myprog.lnx"
#  (followed by ~r and ~d inside the mtip program)

# also "make clean" to clean up non-source files in a directory

# Here the assembler program has the primary source name,
# the C program is just a driver of almost the same basename:
C = $(A)c

# system directories needed for compilers, libraries, header files--
# assumes the environment variables SAPC_TOOLS, SAPC_GNUBIN, and SAPC_SRC
# are set up, usually by the ulab module

PC_LIB = $(SAPC_TOOLS)/lib
PC_INC = $(SAPC_TOOLS)/include

CC   = $(SAPC_GNUBIN)/i386-gcc
CFLAGS = -g -Wall -Wno-implicit -Wshadow -I$(PC_INC)
AS   = $(SAPC_GNUBIN)/i386-as
LD   = $(SAPC_GNUBIN)/i386-ld
NM   = $(SAPC_GNUBIN)/i386-nm

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

$(A).lnx: $(A).opc $(C).opc 
	$(LD) -N -Ttext 1000e0 -o $(A).lnx \
		$(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
		$(C).opc $(A).opc $(PC_LIB)/libc.a
	rm -f syms;$(NM) -n $(A).lnx>$(A).syms;ln -s $(A).syms syms

$(A).opc: $(A).s
	$(AS) -o $(A).opc $(A).s

# tell gcc to use $(PC_INC) for #include <...> headers--
$(C).opc: $(C).c
	$(CC) $(CFLAGS) -c -o $(C).opc $(C).c
	
clean:
	rm -f *.o *.opc *.syms *.lnx core syms
