#    Copyright (C) 1989-1998 Aladdin Enterprises.  All rights reserved.
# 
# This file is part of Aladdin Ghostscript.
# 
# Aladdin Ghostscript is distributed with NO WARRANTY OF ANY KIND.  No author
# or distributor accepts any responsibility for the consequences of using it,
# or for whether it serves any particular purpose or works at all, unless he
# or she says so in writing.  Refer to the Aladdin Ghostscript Free Public
# License (the "License") for full details.
# 
# Every copy of Aladdin Ghostscript must include a copy of the License,
# normally in a plain ASCII text file named PUBLIC.  The License grants you
# the right to copy, modify and redistribute Aladdin Ghostscript, but only
# under certain conditions described in the License.  Among other things, the
# License requires that the copyright notice and this notice be preserved on
# all copies.


# makefile for (MS-Windows 3.1/Win32s / Windows 95 / Windows NT) +
#   Borland C++ 4.5 platform.

# ------------------------------- Options ------------------------------- #

###### This section is the only part of the file you should need to edit.

# ------ Generic options ------ #

# Define the directory that will hold documentation at runtime.

GS_DOCDIR=c:/gs

# Define the default directory/ies for the runtime
# initialization and font files.  Separate multiple directories with \;.
# Use / to indicate directories, not a single \.

GS_LIB_DEFAULT=.;c:/gs\;c:/gs/fonts

# Define whether or not searching for initialization files should always
# look in the current directory first.  This leads to well-known security
# and confusion problems, but users insist on it.
# NOTE: this also affects searching for files named on the command line:
# see the "File searching" section of Use.htm for full details.
# Because of this, setting SEARCH_HERE_FIRST to 0 is not recommended.

SEARCH_HERE_FIRST=1

# Define the name of the interpreter initialization file.
# (There is no reason to change this.)

GS_INIT=gs_init.ps

# Choose generic configuration options.

# Setting DEBUG=1 includes debugging features (-Z switch) in the code.  The
# compiled code is substantially slower and larger.

DEBUG=0

# Setting TDEBUG=1 includes symbol table information for the debugger, and
# also enables stack checking.  The compiled code is substantially slower
# and larger.

TDEBUG=0

# Setting NOPRIVATE=1 makes private (static) procedures and variables
# public, so they are visible to the debugger and profiler.  There is no
# execution time or space penalty, just larger .OBJ and .EXE files.

NOPRIVATE=0

# Define the names of the executable files.

GS=gswin32
GSCONSOLE=gswin32c
GSDLL=gsdll32

# To build two small executables and a large DLL, use MAKEDLL=1.
# To build two large executables, use MAKEDLL=0.

MAKEDLL=1

# If you want multi-thread-safe compilation, set MULTITHREAD=1; if not, set
# MULTITHREAD=0.  MULTITHREAD=0 produces slightly smaller and faster code,
# but MULTITHREAD=1 is required if you use any "asynchronous" output
# drivers.

MULTITHREAD=1

# Define the source, generated intermediate file, and object directories
# for the graphics library (GL) and the PostScript/PDF interpreter (PS).

GLSRCDIR=.
GLGENDIR=obj
GLOBJDIR=obj
PSSRCDIR=.
PSGENDIR=obj
PSOBJDIR=obj

# Define the directory where the IJG JPEG library sources are stored,
# and the major version of the library that is stored there.
# You may need to change this if the IJG library version changes.
# See jpeg.mak for more information.

JSRCDIR=jpeg
JVERSION=6

# Define the directory where the PNG library sources are stored,
# and the version of the library that is stored there.
# You may need to change this if the libpng version changes.
# See libpng.mak for more information.

PSRCDIR=libpng
PVERSION=96

# Define the directory where the zlib sources are stored.
# See zlib.mak for more information.

ZSRCDIR=zlib

# Define the configuration ID.  Read gs.mak carefully before changing this.

CONFIG=

# Define any other compilation flags.

CFLAGS=

# ------ Platform-specific options ------ #

# Define the drive, directory, and compiler name for the Borland C files.
# COMPDIR contains the compiler and linker (normally \bc\bin).
# INCDIR contains the include files (normally \bc\include).
# LIBDIR contains the library files (normally \bc\lib).
# COMP is the full C compiler name (bcc32 for Borland C++).
# COMPCPP is the full C++ compiler path name (bcc32 for Borland C++).
# COMPAUX is the compiler name for DOS utilities (bcc for Borland C++).
# RCOMP is the resource compiler name (brcc32 for Borland C++).
# LINK is the full linker path name (normally \bc\bin\tlink32).
# Note that these prefixes are always followed by a \,
#   so if you want to use the current directory, use an explicit '.'.

COMPBASE=c:\bc
COMPDIR=$(COMPBASE)\bin
INCDIR=$(COMPBASE)\include
LIBDIR=$(COMPBASE)\lib
COMP=$(COMPDIR)\bcc32
COMPCPP=$(COMP)
COMPAUX=$(COMPDIR)\bcc
RCOMP=$(COMPDIR)\brcc32
LINK=$(COMPDIR)\tlink32

# If you don't have an assembler, set USE_ASM=0.  Otherwise, set USE_ASM=1,
# and set ASM to the name of the assembler you are using.  This can be
# a full path name if you want.  Normally it will be masm or tasm.

USE_ASM=0
ASM=tasm

# Define the processor architecture. (always i386)

CPU_FAMILY=i386

# Define the processor (CPU) type.  (386, 486 or 586)

CPU_TYPE=586

# Define the math coprocessor (FPU) type.
# Options are -1 (optimize for no FPU), 0 (optimize for FPU present,
# but do not require a FPU), 87, 287, or 387.
# If you have a 486 or Pentium CPU, you should normally set FPU_TYPE to 387,
# since most of these CPUs include the equivalent of an 80387 on-chip;
# however, the 486SX and the Cyrix 486SLC do not have an on-chip FPU, so if
# you have one of these CPUs and no external FPU, set FPU_TYPE to -1 or 0.
# An xx87 option means that the executable will run only if a FPU
# of that type (or higher) is available: this is NOT currently checked
# at runtime.

FPU_TYPE=387

# ------ Devices and features ------ #

# Choose the language feature(s) to include.  See gs.mak for details.

FEATURE_DEVS=psl3.dev pdf.dev ttfont.dev

# Choose whether to compile the .ps initialization files into the executable.
# See gs.mak for details.

COMPILE_INITS=0

# Choose whether to store band lists on files or in memory.
# The choices are 'file' or 'memory'.

BAND_LIST_STORAGE=file

# Choose which compression method to use when storing band lists in memory.
# The choices are 'lzw' or 'zlib'.  lzw is not recommended, because the
# LZW-compatible code in Ghostscript doesn't actually compress its input.

BAND_LIST_COMPRESSOR=zlib

# Choose the implementation of file I/O: 'stdio', 'fd', or 'both'.
# See gs.mak and sfxfd.c for more details.

FILE_IMPLEMENTATION=stdio

# Choose the device(s) to include.  See devs.mak for details,
# devs.mak and contrib.mak for the list of available devices.

DEVICE_DEVS=mswindll.dev mswinprn.dev mswinpr2.dev
DEVICE_DEVS2=epson.dev eps9high.dev eps9mid.dev epsonc.dev ibmpro.dev
DEVICE_DEVS3=deskjet.dev djet500.dev laserjet.dev ljetplus.dev ljet2p.dev ljet3.dev ljet4.dev
DEVICE_DEVS4=cdeskjet.dev cdjcolor.dev cdjmono.dev cdj550.dev pj.dev pjxl.dev pjxl300.dev
DEVICE_DEVS5=djet500c.dev declj250.dev lj250.dev jetp3852.dev r4081.dev lbp8.dev uniprint.dev
DEVICE_DEVS6=st800.dev stcolor.dev bj10e.dev bj200.dev m8510.dev necp6.dev bjc600.dev bjc800.dev
DEVICE_DEVS7=t4693d2.dev t4693d4.dev t4693d8.dev tek4696.dev
DEVICE_DEVS8=pcxmono.dev pcxgray.dev pcx16.dev pcx256.dev pcx24b.dev
DEVICE_DEVS9=pbm.dev pbmraw.dev pgm.dev pgmraw.dev pgnm.dev pgnmraw.dev pnm.dev pnmraw.dev ppm.dev ppmraw.dev
DEVICE_DEVS10=tiffcrle.dev tiffg3.dev tiffg32d.dev tiffg4.dev tifflzw.dev tiffpack.dev
DEVICE_DEVS11=bmpmono.dev bmp16.dev bmp256.dev bmp16m.dev tiff12nc.dev tiff24nc.dev
DEVICE_DEVS12=psmono.dev bit.dev bitrgb.dev bitcmyk.dev
DEVICE_DEVS13=pngmono.dev pnggray.dev png16.dev png256.dev png16m.dev
DEVICE_DEVS14=jpeg.dev jpeggray.dev
DEVICE_DEVS15=pdfwrite.dev pswrite.dev epswrite.dev pxlmono.dev pxlcolor.dev

# ---------------------------- End of options ---------------------------- #

# Define the name of the makefile -- used in dependencies.

MAKEFILE=$(GLSRCDIR)\bcwin32.mak

# Define the current directory prefix and shell invocations.

D=\\

EXP=
EXPP=
SH=
SHP=

# Define the arguments for genconf.

CONFILES=-p %s+ -l $(GLGENDIR)\lib.tr
CONFLDTR=-o

# Define the generic compilation flags.

PLATOPT=

INTASM=
PCFBASM=

# Make sure we get the right default target for make.

dosdefault: default $(GLOBJDIR)\gs16spl.exe

# Define the switch for output files.

O_=-o

# Define the compilation flags.

!if $(CPU_TYPE)>500
ASMCPU=/DFOR80386 /DFOR80486
CPFLAGS=-DFOR80486 -DFOR80386
!else if $(CPU_TYPE)>400
ASMCPU=/DFOR80386 /DFOR80486
CPFLAGS=-DFOR80486 -DFOR80386
!else
ASMCPU=/DFOR80386
CPFLAGS=-DFOR80386
!endif

!if $(CPU_TYPE) >= 486 || $(FPU_TYPE) > 0
ASMFPU=/DFORFPU
!else
!if $(FPU_TYPE) < 0
ASMFPU=/DNOFPU
!else
ASMFPU=
!endif
!endif
FPFLAGS=
FPLIB=

!if $(NOPRIVATE)!=0
CP=-DNOPRIVATE
!else
CP=
!endif

!if $(DEBUG)!=0
CD=-DDEBUG
!else
CD=
!endif

!if $(TDEBUG)!=0
CT=-v
LCT=-v -m -s
CO=    # no optimization when debugging
ASMDEBUG=/DDEBUG
!else
CT=
LCT=
CO=-Z -O2
!endif

!if $(DEBUG)!=0 || $(TDEBUG)!=0
CS=-N
!else
CS=
!endif

!if $(MULTITHREAD)!=0
CMT=-tWM
CLIB=cw32mt.lib
!else
CMT=
CLIB=cw32.lib
!endif

# Specify function prolog type
COMPILE_FOR_DLL=-WDE
COMPILE_FOR_EXE=-WE
COMPILE_FOR_CONSOLE_EXE=-WC

# The -tWM is for multi-thread-safe compilation.
GENOPT=$(CP) $(CD) $(CT) $(CS) $(CMT)

CCFLAGS0=$(GENOPT) $(PLATOPT) $(CPFLAGS) $(FPFLAGS) $(CFLAGS) $(XCFLAGS)
CCFLAGS=$(CCFLAGS0)
CC=$(COMP) @$(GLGENDIR)\ccf32.tr
CPP=$(COMPCPP) @$(GLGENDIR)\ccf32.tr
!if $(MAKEDLL)
WX=$(COMPILE_FOR_DLL)
!else
WX=$(COMPILE_FOR_EXE)
!endif
CC_WX=$(CC) $(WX)
CC_=$(CC_WX) $(CO)
CC_D=$(CC_WX)
CC_INT=$(CC_WX)
CC_LEAF=$(CC_)

# No additional flags are needed for Windows compilation.
CCWINFLAGS=

# Define the files to be removed by `make clean'.
# nmake expands macros when encountered, not when used,
# so this must precede the !include statements.

# ****** HACK ****** *.tr and *.map are still created in the current directory.
BEGINFILES2=$(GLOBJDIR)\gs16spl.exe *.tr *.map

# Include the generic makefiles.

!include $(GLSRCDIR)\winlib.mak
!include $(GLSRCDIR)\winint.mak

# -------------------------- Auxiliary programs --------------------------- #

# Compiler for auxiliary programs

CCAUX=$(COMPAUX) -ml -I$(INCDIR) -L$(LIBDIR) -n$(AUXGENDIR) -O
CCAUX_TAIL=

$(GLGENDIR)\ccf32.tr: $(MAKEFILE) makefile
	echo -a1 -d -r -G -N -X -I$(INCDIR) $(CCFLAGS0) -DCHECK_INTERRUPTS > $(GLGENDIR)\ccf32.tr

$(ECHOGS_XE): $(GLSRC)echogs.c
	$(CCAUX) $(GLSRC)echogs.c $(CCAUX_TAIL)

# Since we are running in a Windows environment with a different compiler
# for the DOS utilities, we have to invoke genarch by hand.
# For unfathomable reasons, the 'win' program requires /, not \,
# in the name of the program to be run, and apparently also in any
# file names passed on the command line (?!).
$(GENARCH_XE): $(GLSRC)genarch.c $(stdpre_h) $(iref_h) $(GLGENDIR)\ccf32.tr
	$(COMP) -I$(INCDIR) -L$(LIBDIR) -n$(AUXGENDIR) -O $(GLSRC)genarch.c
	echo win $(AUXGENDIR)/genarch $(GLGENDIR)/arch.h >_genarch.bat
	echo ***** Run "_genarch.bat", then continue make. *****

$(GENCONF_XE): $(GLSRC)genconf.c $(stdpre_h)
	$(CCAUX) $(GLSRC)genconf.c $(CCAUX_TAIL)

$(GENDEV_XE): $(GLSRC)gendev.c $(stdpre_h)
	$(CCAUX) $(GLSRC)gendev.c $(CCAUX_TAIL)

$(GENINIT_XE): $(PSSRC)geninit.c $(stdio__h) $(string__h)
	$(CCAUX) $(PSSRC)geninit.c $(CCAUX_TAIL)

# -------------------------------- Library -------------------------------- #

# See winlib.mak

# ----------------------------- Main program ------------------------------ #

LIBCTR=libc32.tr
GSCONSOLE_XE=$(GLOBJ)$(GSCONSOLE).exe

$(LIBCTR): $(MAKEFILE) $(ECHOGS_XE)
	echo $(LIBDIR)\import32.lib $(LIBDIR)\$(CLIB) >$(LIBCTR)

!if $(MAKEDLL)
# The graphical small EXE loader
$(GS_XE): $(GSDLL_OBJ).dll  $(DWOBJ) $(GSCONSOLE_XE)\
 $(GS_OBJ).res $(GLSRC)dwmain32.def
	$(LINK) /Tpe $(LCT) @&&!
$(LIBDIR)\c0w32 +
$(DWOBJ) +
,$(GS_XE),$(GS), +
$(LIBDIR)\import32 +
$(LIBDIR)\cw32, +
$(GLSRC)dwmain32.def, +
$(GS_OBJ).res
!

# The console mode small EXE loader
$(GSCONSOLE_XE): $(OBJC) $(GS_OBJ).res $(GLSRC)dw32c.def
	$(LINK) /Tpe /ap $(LCT) $(DEBUGLINK) @&&!
$(LIBDIR)\c0w32 +
$(OBJC) +
,$(GSCONSOLE_XE),$(GSCONSOLE), +
$(LIBDIR)\import32 +
$(LIBDIR)\cw32, +
$(GLSRC)dw32c.def, +
$(GS_OBJ).res
!

# The big DLL
$(GSDLL_OBJ).dll: $(GS_ALL) $(DEVS_ALL) $(GLOBJ)gsdll.$(OBJ)\
 $(GSDLL_OBJ).res $(GSDLL_SRC).def
	-del gswin32.tr
	copy $(ld_tr) gswin32.tr
	echo $(LIBDIR)\c0d32 $(GLOBJ)gsdll + >> gswin32.tr
	$(LINK) $(LCT) /Tpd @gswin32.tr $(INTASM) ,$(GSDLL_OBJ).dll,$(GSDLL),@$(GLGENDIR)\lib.tr @$(LIBCTR),$(GSDLL_SRC).def,$(GSDLL_OBJ).res

!else
# The big graphical EXE
$(GS_XE):   $(GSCONSOLE_XE) $(GS_ALL) $(DEVS_ALL)\
 $(GLOBJ)gsdll.$(OBJ) $(DWOBJNO) $(GS_OBJ).res $(GLSRC)dwmain32.def
	-del gswin32.tr
	copy $(ld_tr) gswin32.tr
	echo $(LIBDIR)\c0w32 $(GLOBJ)gsdll + >> gswin32.tr
	echo $(DWOBJNO) $(INTASM) >> gswin32.tr
	$(LINK) $(LCT) /Tpe @gswin32.tr ,$(GS_XE),$(GS),@$(GLGENDIR)\lib.tr @$(LIBCTR),$(GLSRC)dwmain32.def,$(GS_OBJ).res
	-del gswin32.tr

# The big console mode EXE
$(GSCONSOLE_XE):  $(GS_ALL) $(DEVS_ALL)\
 $(GLOBJ)gsdll.$(OBJ) $(OBJCNO) $(GS_OBJ).res $(GLSRC)dw32c.def
	-del gswin32.tr
	copy $(ld_tr) gswin32.tr
	echo $(LIBDIR)\c0w32 $(GLOBJ)gsdll + >> gswin32.tr
	echo $(OBJCNO) $(INTASM) >> gswin32.tr
	$(LINK) $(LCT) /Tpe /ap @gswin32.tr ,$(GSCONSOLE_XE),$(GSCONSOLE),@$(GLGENDIR)\lib.tr @$(LIBCTR),$(GLSRC)dw32c.def,$(GS_OBJ).res
	-del gswin32.tr
!endif

# Access to 16 spooler from Win32s

$(GLOBJ)gs16spl.exe: $(GLSRC)gs16spl.c $(GLSRC)gs16spl.rc
	$(CCAUX) -W -ms -v -I$(INCDIR) $(GLO_)gs16spl.obj -c $(GLSRC)gs16spl.c
	$(COMPDIR)\brcc -i$(INCDIR) -r -fo$(GLOBJ)gs16spl.res $(GLSRC)gs16spl.rc
	$(COMPDIR)\tlink /Twe /c /m /s /l @&&!
$(LIBDIR)\c0ws +
$(GLOBJ)gs16spl.obj +
,$(GLOBJ)gs16spl.exe,$(GLOBJ)gs16spl, +
$(LIBDIR)\import +
$(LIBDIR)\mathws +
$(LIBDIR)\cws, +
$(GLSRC)gs16spl.def
!
	$(COMPDIR)\rlink -t $(GLOBJ)gs16spl.res $(GLOBJ)gs16spl.exe

# end of makefile
