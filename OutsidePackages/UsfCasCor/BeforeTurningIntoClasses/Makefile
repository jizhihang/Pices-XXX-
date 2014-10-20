SH=bash
SHELL=bash
CC=gcc
MAKE=gmake

ARCH=$(shell uname -s)
SRCDIR=$(shell pwd)
VPATH=$(SRCDIR)

ifeq ($(ARCH),SunOS)
   BIN_DIR=$(HOME)/bin
else
   BIN_DIR=$(HOME)/bin/linux
endif

LIBS=-lm -lz

CFLAGS+=-Wall -g -O3 -D$(ARCH)

ifeq ($(USE_MPI),1)
   CC=mpicc
   CFLAGS=-DUSE_MPI -D$(ARCH)
endif

SRCS=usfcascor.c \
	parms.c \
	globals.c\
	load_data.c\
	load_namesfile.c\
	normalize.c \
	netio.c \
	learn.c \
	util.c \
	getopt.c \
	getopt1.c \
	utils.c

OBJS=usfcascor.o \
	parms.o\
	globals.o\
	load_data.o\
	load_namesfile.o\
	normalize.o \
	netio.o \
	learn.o \
	util.o \
	getopt.o \
	getopt1.o \
	utils.o

all: all-$(ARCH)
all-$(ARCH): usfcascor-$(ARCH)

mpi:
	$(MAKE) USE_MPI=1 all
arch_dir:
	-@mkdir $(ARCH)

install: $(BIN_DIR)/usfcascor
$(BIN_DIR)/usfcascor: $(ARCH)/usfcascor
	cp $< $@


usfcascor-$(ARCH): depend arch_dir
	cd $(ARCH) && $(MAKE) -f ../Makefile VPATH=$(VPATH) usfcascor

usfcascor: $(OBJS)
	$(CC) $(CFLAGS) -o usfcascor $^ $(LIBS)

usfcascor.o: usfcascor.c usfcascor.h
parms.o: parms.c usfcascor.h

clean:
	-rm -r $(ARCH) .depend 

# Automatically generate dependencies
depend: .depend

.depend: $(SRCS)
	$(CC) $(CFLAGS) -MM $(SRCS) > .depend

-include $(VPATH)/.depend

