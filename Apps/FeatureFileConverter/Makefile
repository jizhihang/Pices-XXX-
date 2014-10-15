EXEC = FeatureFileConverter
#CFLAGS =  -g -I../../Library
CFLAGS = -O3  -I../../Library
#CFLAGS = -g   -I../../Library
LIBS = -lPICL -lfftw3 -lrt
#LIBS = -lPICL -lfftw -lrt
VPATH = ../../Library
LIBDIR = ../../Library

CC = g++

OBJECTS	 = FeatureFileConverter.o

$(EXEC): $(OBJECTS)     $(LIBDIR)/*.h  $(LIBDIR)/*.cpp  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR) -L../../OutsidePackages/fftw-3.0.1/.libs

FeatureFileConverter.o :    FeatureFileConverter.cpp  FeatureFileConverter.h
	$(CC) $(CFLAGS)   -c ./FeatureFileConverter.cpp


libPICL.a: $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)
