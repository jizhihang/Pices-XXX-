EXEC = FeatureSelection


#FFTWAPI = /usr/lib64
CFLAGS = -O3  -I../../BaseLibrary  -I../../PicesLibrary   -I../../SipperInstruments  
#LIBS = -lPicesLibrary   -lSipperInstruments    -lBaseLibrary   -lfftw3f -lrt
#LIBS = -lPicesLibrary   -lSipperInstruments    -lBaseLibrary   -lfftw3-3 -lrt
LIBS = -lPicesLibrary   -lSipperInstruments    -lBaseLibrary   -lfftw3f -lrt  -lpthread 

#FFTWAPI = ../../OutsidePackages/fftw-3.1.2/.libs
#FFTWAPI = /usr/lib64
#FFTWAPI = ../OutsidePackages/fftw-3.3.2/
LIBDIR = -L../../BaseLibrary  -L../../SipperInstruments  -L../../PicesLibrary  
#LIBDIR = -L../../BaseLibrary  -L../../SipperInstruments  -L../../PicesLibrary 

#MYSQL_include = ../OutsidePackages/MySQL_Linux/include
#MYSQL_lib     = -L../../OutsidePackages/MySQL_Linux/lib -lmysql -lpthread -lz

CC = g++




FEATURESELECTIONTYPES = FeatureSelectionTypes.h

ACCBYFEATURESEL = AccByFeatureSel.h

BINARYCLASS = BinaryClass.h 

BINARYJOB   = BinaryJob.h  $(FEATURESELECTIONTYPES)

BINARYJOBLIST = BinaryJobList.h   $(BINARYJOB)

FEATUREIMPACT = FeatureImpact.h   $(BINARYJOBLIST)

FEATURESELECTION = FeatureSelection.h  $(BINARYCLASS)  $(FEATURESELECTIONTYPES)

JOBRANDOMSPLIT = JobRandomSplit.h  $(BINARYJOB)

RANDOMSPLITRESULTS =   RandomSplitResults.h   $(FEATURESELECTIONTYPES)  $(JOBRANDOMSPLIT)

RESULTS = Results.h  $(FEATURESELECTIONTYPES)

PROCESSOR = Processor.h  $(FEATURESELECTIONTYPES)

FINALRESULTS = FinalResults.h  $(RANDOMSPLITRESULTS)  $(FEATURESELECTIONTYPES)

JOBVALIDATION = JobValidation.h

VALIDATIONRESULTS =   ValidationResults.h   $(FEATURESELECTIONTYPES)  $(JOBVALIDATION)



OBJECTS	 = AccByFeatureSel.o                                                      \
           FeatureSelection.o     BinaryClass.o          BinaryJob.o              \
           BinaryJobList.o        FeatureImpact.o        FeatureSelectionTypes.o  \
           FinalResults.o         JobRandomSplit.o       JobValidation.o          \
           Processor.o            RandomSplitResults.o   Results.o                \
           ValidationResults.o


all:	$(EXEC)

clean:
	rm *.o;  rm *.a


#$(EXEC): $(OBJECTS) 
#	$(CC) -o $(EXEC) $(OBJECTS) $(LIBS) $(LIBDIR)  $(MYSQL_lib)

$(EXEC): $(OBJECTS) 
	$(CC) -o $(EXEC) $(CFLAGS) $(OBJECTS) $(LIBS) $(LIBDIR)


FeatureSelection.o : FeatureSelection.cpp  $(FEATURESELECTION)    $(BINARYCLASS) $(BINARYJOB) $(PROCESSOR)   $(FINALRESULTS)
	$(CC) $(CFLAGS) -c FeatureSelection.cpp


AccByFeatureSel.o : AccByFeatureSel.cpp  $(ACCBYFEATURESEL)
	$(CC) $(CFLAGS) -c AccByFeatureSel.cpp

BinaryClass.o : BinaryClass.cpp  $(BINARYCLASS)  
	$(CC) $(CFLAGS) -c BinaryClass.cpp


BinaryJob.o : BinaryJob.cpp  $(BINARYJOB)  $(PROCESSOR) $(FEATURESELECTIONTYPES)
	$(CC) $(CFLAGS) -c BinaryJob.cpp


BinaryJobList.o : BinaryJobList.cpp  $(BINARYJOBLIST)  $(BINARYJOB)  $(PROCESSOR)
	$(CC) $(CFLAGS) -c BinaryJobList.cpp


FeatureImpact.o : FeatureImpact.cpp  $(FEATUREIMPACT)  $(BINARYJOB)  $(PROCESSOR) $(BINARYJOBLIST)
	$(CC) $(CFLAGS) -c FeatureImpact.cpp


FeatureSelectionTypes.o : FeatureSelectionTypes.cpp  $(FEATURESELECTIONTYPES)
	$(CC) $(CFLAGS) -c FeatureSelectionTypes.cpp


FinalResults.o : FinalResults.cpp  $(FINALRESULTS)  $(FEATURESELECTIONTYPES)  $(RANDOMSPLITRESULTS)  $(FINALRESULTS)
	$(CC)  $(CFLAGS) -c FinalResults.cpp 

	
JobRandomSplit.o : JobRandomSplit.cpp  $(JOBRANDOMSPLIT)  $(PROCESSOR)  $(BINARYJOB) $(BINARYJOBLIST)  $(FEATURESELECTIONTYPES)  $(RANDOMSPLITRESULTS)
	$(CC)  $(CFLAGS) -c JobRandomSplit.cpp 


JobValidation.o : JobValidation.cpp $(JOBVALIDATION)  $(PROCESSOR)  $(BINARYJOB) $(BINARYJOBLIST)  $(FEATURESELECTIONTYPES)  $(VALIDATIONRESULTS)
	$(CC)  $(CFLAGS) -c JobValidation.cpp 


Processor.o : Processor.cpp   $(PROCESSOR)  $(BINARYJOB)  $(FEATUREIMPACT)  $(FEATURESELECTION) 
	$(CC) $(CFLAGS) -c Processor.cpp 


RandomSplitResults.o : RandomSplitResults.cpp  $(RANDOMSPLITRESULTS)  $(PROCESSOR)  $(BINARYJOB)  $(FEATURESELECTIONTYPES)  $(JOBRANDOMSPLIT)
	$(CC)  $(CFLAGS) -c RandomSplitResults.cpp 


Results.o : Results.cpp  $(RESULTS)  $(PROCESSOR)  $(BINARYJOB)  $(FEATURESELECTIONTYPES)
	$(CC)  $(CFLAGS) -c Results.cpp



ValidationResults.o : ValidationResults.cpp  $(VALIDATIONRESULTS)  $(PROCESSOR)  $(BINARYJOB)  $(FEATURESELECTIONTYPES)  $(JOBVALIDATION)
	$(CC)  $(CFLAGS) -c ValidationResults.cpp 


