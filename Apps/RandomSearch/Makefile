EXEC = RandomSearch

CFLAGS = -O3 -I../../Library
#CFLAGS = -g -I../../Library

LIBS = -lPICL -lfftw3
VPATH = ../../Library
LIBDIR = ../../Library

CC = g++


RESULTLINE    = ResultLine.h 

FEATURESTATS  = FeatureStats.h  $(RESULTLINE)

RANDOMSEARCH  = RandomSearch.h  $(RESULTLINE)


OBJECTS	 = RandomSearch.o ResultLine.o FeatureStats.o

$(EXEC): $(OBJECTS)  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR)  -L../../OutsidePackages/fftw-3.0.1/.libs


FeatureStats.o : FeatureStats.cpp  ResultLine.h  $(FEATURESTATS)
	$(CC) $(CFLAGS) -c $*.cpp


RandomSearch.o : RandomSearch.cpp  FeatureStats.h  $(RANDOMSEARCH)  
	$(CC) $(CFLAGS) -c $*.cpp


ResultLine.o : ResultLine.cpp  $(RESULTLINE)  $(LIBDIR)
	$(CC) $(CFLAGS) -c $*.cpp


libPICL.a: $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)


