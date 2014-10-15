EXEC = RandomSplits

CFLAGS = -O3 -I../../BaseLibrary  -I../../PicesLibrary    -I../../JobManager -I../../SipperInstruments  
LIBS = -lJobManager -lPicesLibrary   -lSipperInstruments    -lBaseLibrary   -lfftw3  
LIBDIR = -L../../BaseLibrary  -L../../SipperInstruments  -L../../PicesLibrary -L../../JobManager  -L../../OutsidePackages/fftw-3.1.2/.libs

#MYSQL_include = ../OutsidePackages/MySQL_Linux/include
#MYSQL_lib     = -L../../OutsidePackages/MySQL_Linux/lib -lmysql -lpthread -lz

CC = g++


RANDOMSPLITJOB = RandomSplitJob.h

RANDOMSPLITJOBMANAGER = RandomSplitJobManager.h 

RANDOMSPLITS   = RandomSplits.h 

OBJECTS	 = RandomSplits.o   RandomSplitJobManager.o    RandomSplitJob.o


#$(EXEC): $(OBJECTS) 
#	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) $(LIBDIR)  $(MYSQL_lib)

$(EXEC): $(OBJECTS) 
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) $(LIBDIR)


RandomSplits.o : RandomSplits.cpp  $(RANDOMSPLITS)  $(RANDOMSPLITJOBMANAGER)  $(RANDOMSPLITJOB)
	$(CC) $(CFLAGS) -c RandomSplits.cpp


RandomSplitJobManager.o : RandomSplitJobManager.cpp    $(RANDOMSPLITJOBMANAGER)  $(RANDOMSPLITJOB)
	$(CC) $(CFLAGS) -c RandomSplitJobManager.cpp



RandomSplitJob.o : RandomSplitJob.cpp  $(RANDOMSPLITJOB)  $(RANDOMSPLITJOBMANAGER)  $(RANDOMSPLITJOB)
	$(CC) $(CFLAGS) -c RandomSplitJob.cpp




	
























