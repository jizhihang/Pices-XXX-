CFLAGS = -O3 -I../../Library 
#CFLAGS = -g -I../../Library 

LIBS = 	-lPICL -lfftw


VPATH = ../../Library

CC = g++
#CC = bcc32


LIBDIR = ../../Library


CLASSGROUPHISTORY  = ClassGroupHistory.h   $(SORTORDERTYPE)
CLASSGROUPTOTALS   = ClassGroupTotals.h 
CLASSFOCUS         = ClassFocus.h          $(SORTORDERTYPE)
JOBDESC            = JobDesc.h 
PASSASSIGNMENTS    = PassAssignments.h 

SORTORDERTYPE      = SortOrderType.h  
TRAININGIMAGE      = TrainingImage.h

ACTIVELEARNING     = ActiveLearning.h   $(CLASSGROUPTOTALS)  $(SORTORDERTYPE) $(PASSASSIGNMENTS)



EXEC = ActiveLearning

OBJECTS	= ActiveLearning.o   ActiveLearningApp.o   ClassFocus.o  ClassGroupHistory.o   ClassGroupTotals.o JobDesc.o   PassAssignments.o   SortOrderType.o  TrainingImage.o 
		  

$(EXEC): $(OBJECTS)  libPICL.a  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS)  -L$(LIBDIR)


ActiveLearning.o : ActiveLearning.cpp   $(ACTIVELEARNING)    $(JOBDESC)  libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


ActiveLearningApp.o :  ActiveLearningApp.cpp    $(ACTIVELEARNING) libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


ClassFocus.o :         ClassFocus.cpp           $(CLASSFOCUS)  libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


ClassGroupHistory.o :  ClassGroupHistory.cpp    $(CLASSGROUPHISTORY)   $(CLASSGROUPTOTALS)  libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


ClassGroupTotals.o :   ClassGroupTotals.cpp     $(CLASSFOCUS)    $(CLASSGROUPHISTORY)   $(IMAGECLASS)  libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


JobDesc.o :            JobDesc.cpp              $(JOBDESC)       libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


PassAssignments.o :    PassAssignments.cpp      $(PASSASSIGNMENTS)      libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


SortOrderType.o :      SortOrderType.cpp        $(SORTORDERTYPE)   libPICL.a
	$(CC) $(CFLAGS) -c $*.cpp


TrainingImage.o :      $(TRAININGIMAGE)  
	$(CC) $(CFLAGS) -c $*.cpp


libPICL.a :   $(LIBDIR)/*.cpp  $(LIBDIR)/*.h
	(cd $(LIBDIR); make)
