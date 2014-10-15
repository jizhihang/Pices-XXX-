EXEC = libJobManager
LIBS = -lfftw -lrt
VPATH = ../BaseLibrary  ../PicesLibrary
LIBDIR = ../BaseLibrary 
FFTWAPI = ../OutsidePackages/fftw-3.1.2/api
CFLAGS = -O3 -I$(LIBDIR)
#CFLAGS = -g   -I$(LIBDIR)

CC = g++

JOB              = Job.h
JOBMAMAGER       = JobManager.h   Job.h

OBJECTS	= Job.o  JobManager.o
          

$(EXEC): $(OBJECTS)
	ar cq libJobManager.a  $(OBJECTS)


Job.o: Job.cpp  $(JOB)  ../PicesLibrary/*.h
	$(CC) $(CFLAGS) -c $*.cpp

JobManager.o: JobManager.cpp    $(JOBMAMAGER)  $(JOB)  ../PicesLibrary/*.h
	$(CC) $(CFLAGS) -c $*.cpp


libPicesLibrary.a : $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd ../PicesLibrary; /bin/rm -f libPicesLibrary.a; make)


libBaseLibrary.a : $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); /bin/rm -f libBaseLibrary.a; make)































