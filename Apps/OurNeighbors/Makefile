EXEC   = OurNeighbors
CFLAGS = -O3 -I../../Library
LIBS   = -lPICL -lfftw 
VPATH  = ../../Library
LIBDIR = ../../Library
NAME   = $(EXEC)

CC = g++


NEIGHBOR = Neighbor.h

OBJECTS	 = $(NAME).o  Neighbor.o

$(EXEC): $(OBJECTS)  libPICL.a   $(LIBDIR)/*.h  $(LIBDIR)/*.cpp  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR)


$(NAME).o :    $(NAME).cpp   $(LIBDIR)/*.h
	$(CC) $(CFLAGS)   -c ./$(NAME).cpp


Neighbor.o : Neighbor.cpp $(Neighbor)  $(LIBDIR)/*.h
	$(CC)  $(CFLAGS)   -c Neighbor.cpp


libPICL.a: $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)
