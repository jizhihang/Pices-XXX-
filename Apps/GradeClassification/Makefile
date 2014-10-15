EXEC = GradeClassification
CFLAGS = -O3 -I../../Library
LIBS = -lPICL -lfftw -lrt
VPATH = ../../Library
LIBDIR = ../../Library
NAME   = $(EXEC)



CC = g++

OBJECTS	 = $(NAME).o  main.o

$(EXEC): $(OBJECTS)  libPICL.a   $(LIBDIR)/*.h  $(LIBDIR)/*.cpp  
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) -L$(LIBDIR)

$(NAME).o :    $(NAME).cpp  $(NAME).h
	$(CC) $(CFLAGS)   -c ./$(NAME).cpp

main.o : main.cpp  GradeClassification.h
	$(CC) $(CFLAGS)   -c main.cpp



libPICL.a: $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)
