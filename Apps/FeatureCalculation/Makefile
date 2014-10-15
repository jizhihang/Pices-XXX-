EXEC = ExtractFeatures
CFLAGS = -O3 -I../../Library
LIBS = -lPICL -lfftw -lrt
VPATH = ../../Library
LIBDIR = ../../Library
NAME   = $(EXEC)



CC = g++

OBJECTS	 = $(NAME).o  ExtractFeaturesApp.o

$(EXEC): $(OBJECTS)  libPICL.a   $(LIBDIR)/*.h  $(LIBDIR)/*.cpp  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR)

$(NAME).o :    $(NAME).cpp  $(NAME).h
	$(CC) $(CFLAGS)   -c ./$(NAME).cpp

ExtractFeaturesApp.o : ExtractFeaturesApp.cpp  ExtractFeatures.h   $(LIBDIR)/*.cpp  
	$(CC) $(CFLAGS)   -c ExtractFeaturesApp.cpp



libPICL.a: $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)
