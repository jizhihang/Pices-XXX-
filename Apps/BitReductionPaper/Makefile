EXEC  = BitReductionPaper

CFLAGS = -O3 -I../../Library
#CFLAGS = -g -I../../Library
LIBS = -lPICL -lfftw3 -lrt

VPATH  = ../../Library
LIBDIR = ../../Library

CC = g++


BITREDUCTIONPAPER     = BitReductionPaper.h    DataSetParameters.h
RANDOMSAMPLEJOB       = RandomSampleJob.h      DataSetParameters.h
BITREDUCTIONJOB       = BitReductionJob.h      DataSetParameters.h
BITREDUCTIONJOBLIST   = BitReductionJobList.h  BitReductionJob.h    DataSetParameters.h
BITREDUCEDEXAMPLE     = BitReducedExample.h 

OBJECTS	 = BitReductionPaper.o RandomSampleJob.o BitReductionJob.o  BitReductionJobList.o  BitReducedExample.o


$(EXEC): $(OBJECTS) libPICL.a  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC)  $(OBJECTS)  $(LIBS)  -L$(LIBDIR) -L../../OutsidePackages/fftw-3.0.1/.libs

BitReductionPaper.o : BitReductionPaper.cpp  $(BITREDUCTIONPAPER)  $(RANDOMSAMPLEJOB)  $(BITREDUCTIONJOB)  $(BITREDUCTIONJOBLIST)
	$(CC) $(CFLAGS) -c $*.cpp 

RandomSampleJob.o : RandomSampleJob.cpp   RandomSampleJob.h   $(RANDOMSAMPLEJOB) 
	$(CC) $(CFLAGS) -c $*.cpp

BitReductionJob.o : BitReductionJob.cpp  $(BITREDUCTIONJOB)
	$(CC) $(CFLAGS) -c $*.cpp 

BitReductionJobList.o : BitReductionJobList.cpp  $(BITREDUCTIONJOB)  $(BITREDUCTIONJOBLIST)
	$(CC) $(CFLAGS) -c $*.cpp 

BitReducedExample.o : BitReducedExample.cpp $(BITREDUCEDEXAMPLE)
	$(CC) $(CFLAGS) -c $*.cpp 

libPICL.a : $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); /bin/rm *.a; make)
