EXEC = RandomizeData
CFLAGS = -O3 -I../../BaseLibrary  -I../../PicesLibrary  -I../../SipperInstruments
LIBS = -lPicesLibrary   -lSipperInstruments    -lBaseLibrary   -lfftw3  
LIBDIR = -L../../BaseLibrary  -L../../SipperInstruments  -L../../PicesLibrary  -L../../OutsidePackages/fftw-3.1.2/.libs

CC = g++

OBJECTS	 = RandomizeData.o

$(EXEC): $(OBJECTS)  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) $(LIBDIR)

RandomizeData.o :    RandomizeData.cpp
	$(CC) $(CFLAGS)   -c ./RandomizeData.cpp

