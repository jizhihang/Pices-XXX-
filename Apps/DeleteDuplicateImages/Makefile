EXEC   = DeleteDuplicateImages
CFLAGS = -O3 -I../../Library
LIBS   = -lPICL -lfftw 
VPATH  = ../../Library
LIBDIR = ../../Library
NAME   = $(EXEC)

CC = g++


NEIGHBOR = Neighbor.h

OBJECTS	 = $(NAME).o

$(EXEC): $(OBJECTS)  libPICL.a   $(LIBDIR)/*.h  $(LIBDIR)/*.cpp  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR)


$(NAME).o :    $(NAME).cpp   $(LIBDIR)/*.h
	$(CC) $(CFLAGS)   -c ./$(NAME).cpp


libPICL.a: $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)
