# DavydovMA 2013040600 {
# file		: Makefile
# "Copyright (C) 1990-2019 Free Software DavydovMA, Inc
# URL		: http://domain"
# e-mail	: dev@domain

SOFT=viewhp

# ------------- ---- ------
.PHONY: all clean install uninstall debug
#
CC_Lx32=gcc
CC_Lx64=gcc
#CC_Wx32=/bin/i686-w64-mingw32-gcc
#CC_Wx64=/bin/x86_64-w64-mingw32-gcc
#CC_Dx16=
#CC_Dx32=
#CC_Rx16=
#CC_Rx32=
#CC_Rx64=
#
FLAGS_COMPILER_WARN_NO= -w
FLAGS_COMPILER_WARN_ALL= -Wall
FLAGS_COMPILER_NO= -c
FLAGS_COMPILER= $(FLAGS_COMPILER_WARN_NO)
FLAGS_COMPILER_DEBUG= -D DEBUG
FLAGS_LINKER_STRIP_ALL= -s
#FLAGS_LINKER_SDL= -lSDL
#FLAGS_LINKER_SDL_FULL= -I/usr/include/SDL -L/usr/lib -lSDL
FLAGS_LINKER_SDL2= -lSDL2
#FLAGS_LINKER_SDL2_FULL= -I/usr/include/SDL2 -L/usr/lib -lSDL2
FLAGS_LINKER= $(FLAGS_LINKER_STRIP_ALL) $(FLAGS_LINKER_SDL2)
FLAGS_LINKER_DEBUG= $(FLAGS_LINKER_SDL2)
#
#AS_Lx64=nasm
#AS_Lx32=$(AS_Lx64)
#
#LINK=ld
#
#EXT_Rx16=.rx86_16
#EXT_Rx32=.rx86_32
#EXT_Rx64=.rx86_64
#EXT_Dx16=.dx86_16.exe
#EXT_Dx32=.dx86_32.exe
#EXT_Wx32=.wx86_32.exe
#EXT_Wx64=.wx86_64.exe
EXT_Lx32=.lx86_32.elf
EXT_Lx64=.lx86_64.elf
# ------------- ---- ------

ifeq ($(shell arch),x86_64)
	CC  := $(CC_Lx64) -Dx86_64
	EXT := $(EXT_Lx64)
endif
ifeq ($(shell arch),i686)
	CC  := $(CC_Lx32) -Dx86_32
	EXT := $(EXT_Lx32)
endif
ifeq ($(shell arch),i586)
	CC  := $(CC_Lx32) -Dx86_32
	EXT := $(EXT_Lx32)
endif
ifeq ($(shell arch),i486)
	CC  := $(CC_Lx32) -Dx86_32
	EXT := $(EXT_Lx32)
endif
ifeq ($(shell arch),i386)
	CC  := $(CC_Lx32) -Dx86_32
	EXT := $(EXT_Lx32)
endif

all:	all_elf all_man1
#	see top
all_elf:
	$(CC) $(FLAGS_COMPILER) $(FLAGS_LINKER) main.c -o $(SOFT)$(EXT)
all_man1:
	gzip -k -f ./man/man1/$(SOFT).1
	gzip -k -f ./man/ru/man1/$(SOFT).1

clean:	clean_elf clean_man1
#	see top
clean_elf:
	rm -v -f $(SOFT)$(EXT)
#	shred -vun 1 $(SOFT)$(EXT)
clean_man1:
	rm -v -f ./man/man1/$(SOFT).1.gz
	rm -v -f ./man/ru/man1/$(SOFT).1.gz

install:	install_elf install_man1
#	see top
install_elf:
	mkdir -v -p /usr/local/bin/DavydovMA
	cp -v -f $(SOFT)$(EXT) /usr/local/bin/DavydovMA
	ln -s -f /usr/local/bin/DavydovMA/$(SOFT)$(EXT) /usr/bin/$(SOFT)
install_man1:
	cp -v -f ./man/man1/$(SOFT).1.gz    /usr/share/man/man1
	cp -v -f ./man/ru/man1/$(SOFT).1.gz /usr/share/man/ru/man1

uninstall:	uninstall_elf uninstall_man1
#	see top
uninstall_elf:
	rm -v -f /usr/bin/$(SOFT)
	rm -v -f /usr/local/bin/DavydovMA/$(SOFT)$(EXT)
uninstall_man1:
	rm -v -f /usr/share/man/ru/man1/$(SOFT).1.gz
	rm -v -f /usr/share/man/man1/$(SOFT).1.gz

debug:	debug_elf
#	see top
debug_elf:
	$(CC) $(FLAGS_COMPILER_DEBUG) $(FLAGS_LINKER_DEBUG) main.c -o $(SOFT)$(EXT)


# } DavydovMA 2019121800