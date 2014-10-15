EXEC = CrossValidation
CFLAGS = -O3 -I../../BaseLibrary  -I../../PicesLibrary  -I../../SipperInstruments
LIBS = -lPicesLibrary   -lSipperInstruments    -lBaseLibrary   -lfftw3 -lrt 
LIBDIR = -L../../BaseLibrary  -L../../SipperInstruments  -L../../PicesLibrary  -L../../OutsidePackages/fftw-3.1.2/.libs

CC = g++

OBJECTS	 = CrossValidation.o

$(EXEC): $(OBJECTS)  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) $(LIBDIR)

CrossValidation.o :    CrossValidation.cpp
	$(CC) $(CFLAGS)   -c ./CrossValidation.cpp


