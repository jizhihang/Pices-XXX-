EXEC  = BeamSearchSVM
EXEC2 = ParameterSearch
EXEC3 = ParameterSearchBinaryCombo
EXEC4 = ParseBeamSearchResults

CFLAGS = -O3 -I../../Library
#CFLAGS = -g -I../../Library
LIBS = -lPICL -lfftw3 -lrt

VPATH  = ../../Library
LIBDIR = ../../Library

CC = g++


SORTORDERTYPE      = SortOrderType.h    

BINARYCLASSPARMS   = BinaryClassParms.h   
CLASSGROUPHISTORY  = ClassGroupHistory.h   $(SORTORDERTYPE)
CLASSGROUPTOTALS   = ClassGroupTotals.h    $(SORTORDERTYPE)    
CLASSFOCUS         = ClassFocus.h          $(SORTORDERTYPE)
JOBDESC            = JobDesc.h             
PASSASSIGNMENTS    = PassAssignments.h     
PARAMETERJOB       = ParameterJob.h
PARAMETERPROCESS   = ParameterProcess.h    $(PARAMETERJOB)
PARAMETERSEARCH    = ParameterSearch.h     $(PARAMETERJOB)

PARAMETERSEARCHBINARYCOMBO = ParameterSearchBinaryCombo.h   $(PARAMETERPROCESS)  

SINGLERUN          = SingleRun.h
SINGLERUNLIST      = SingleRunList.h  $(SINGLERUN)
BEAMSEARCHSVM      = BeamSearchSvm.h  $(SINGLERUNLIST)
RESULTLINE         = ResultLine.h
SUMMARYLINE        = FirstIncludes.h  SummaryLine.h
TOTALLINE          = TotalLine.h      $(RESULTLINE)



OBJECTS	 = BeamSearchSVM.o SingleRun.o SingleRunList.o 

OBJECTS2 = ParameterSearch.o  ParameterJob.o  ParameterSearchReportLine.o

OBJECTS3 = ParameterSearchBinaryCombo.o ParameterJob.o ParameterProcess.o  

OBJECTS4 = ParseBeamSearchResults.o ResultLine.o TotalLine.o SummaryLine.o  SingleRun.o SingleRunList.o


$(EXEC): $(OBJECTS) libPICL.a  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC)  $(OBJECTS)  $(LIBS)  -L$(LIBDIR) -L../../OutsidePackages/fftw-3.0.1/.libs

$(EXEC2): $(OBJECTS2) libPICL.a   $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC2) $(OBJECTS2) $(LIBS)  -L$(LIBDIR)  -L../../OutsidePackages/fftw-3.0.1/.libs

$(EXEC3): $(OBJECTS3) libPICL.a  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC3) $(OBJECTS3) $(LIBS)  -L$(LIBDIR)  -L../../OutsidePackages/fftw-3.0.1/.libs

$(EXEC4): $(OBJECTS4) libPICL.a  $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	$(CC) -o $(EXEC4) $(OBJECTS4) $(LIBS)  -L$(LIBDIR)  -L../../OutsidePackages/fftw-3.0.1/.libs



SingleRun.o : SingleRun.cpp  $(SINGLERUN)  SingleRunList.h 
	$(CC) $(CFLAGS) -c $*.cpp 

SingleRunList.o : SingleRunList.cpp   SingleRunList.h   $(SINGLERUN) 
	$(CC) $(CFLAGS) -c $*.cpp

BeamSearchSVM.o : BeamSearchSVM.cpp   BeamSearchSVM.h  $(SINGLERUN)  SingleRunList.h  
	$(CC) $(CFLAGS) -c $*.cpp

ParameterJob.o : ParameterJob.cpp  $(PARAMETERJOB)  
	$(CC) $(CFLAGS) -c $*.cpp

ParameterProcess.o : ParameterProcess.cpp  $(PARAMETERPROCESS)  
	$(CC) $(CFLAGS) -c $*.cpp

ParameterSearch.o : ParameterSearch.cpp   $(PARAMETERSEARCH)  
	$(CC) $(CFLAGS) -c $*.cpp

ParameterSearchBinaryCombo.o : ParameterSearchBinaryCombo.cpp   $(PARAMETERSEARCHBINARYCOMBO)  
	$(CC) $(CFLAGS) -c $*.cpp

ParameterSearchReportLine.o : ParameterSearchReportLine.cpp  ParameterSearchReportLine.h  
	$(CC) $(CFLAGS) -c $*.cpp

ParseBeamSearchResults.o : ParseBeamSearchResults.cpp $(RESULTLINE) $(SINGLERUN) $(SUMMARYLINE) $(TOTALLINE)
	$(CC) $(CFLAGS) -c $*.cpp



ResultLine.o : ResultLine.cpp  $(RESULTLINE) 
	$(CC) $(CFLAGS) -c $*.cpp

SummaryLine.o : SummaryLine.cpp  $(SUMMARYLINE)
	$(CC) $(CFLAGS) -c $*.cpp

TotalLine.o : TotalLine.cpp  $(TOTALLINE) $(RESULTLINE) 
	$(CC) $(CFLAGS) -c $*.cpp

libPICL.a : $(LIBDIR)/*.h  $(LIBDIR)/*.cpp
	(cd $(LIBDIR); make)
