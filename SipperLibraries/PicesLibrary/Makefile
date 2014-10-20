EXEC = libPICL
LIBS = -lfftw3 -lrt
VPATH = ../BaseLibrary
LIBDIR = ../BaseLibrary  ../SipperInstruments
#FFTWAPI = ../OutsidePackages/fftw-3.1.2/api
#FFTWAPI = ../OutsidePackages/fftw-3.3.2/

CFLAGS = -O3 -I ../BaseLibrary  -I ../SipperInstruments  
#CFLAGS = -g  -I $(LIBDIR)


CC = g++


IMAGEFEATURESDATAINDEXED = ImageFeaturesDataIndexed.h   
IMAGEFEATURESNAMEINDEXED = ImageFeaturesNameIndexed.h   

ATTRIBUTE          = Attribute.h           $(FIRSTINCLUDES)

FEATURENUMLIST     = FeatureNumList.h      $(ATTRIBUTE)    

MLCLASS            = MLClass.h

MLCLASSCONSTLIST   = MLClassConstList.h

FILEDESC           = FileDesc.h            $(ATTRIBUTE)          $(MLCLASS)

ORDERINGS          = Orderings.h           $(FILEDESC)

CLASSSTATISTIC     = ClassStatistic.h      $(MLCLASS)

FEATUREVECTOR      = FeatureVector.h       $(ATTRIBUTE)          $(CLASSSTATISTIC)     FeatureFileIO.h   \
                     $(FEATURENUMLIST)     $(FILEDESC)           $(MLCLASS)

IMAGEFEATURES      = ImageFeatures.h       $(ATTRIBUTE)          $(CLASSSTATISTIC)     $(FEATURENUMLIST) \
                     $(FEATUREVECTOR)      $(FILEDESC)

FEATUREFILEIO      = FeatureFileIO.h       $(FEATURENUMLIST)     $(FEATUREVECTOR)      $(MLCLASS)

FEATUREFILEIOARFF    = FeatureFileIOArff.h     $(FEATUREFILEIO) 
FEATUREFILEIOC45     = FeatureFileIOC45.h      $(FEATUREFILEIO) 
FEATUREFILEIOCOLUMN  = FeatureFileIOColumn.h   $(FEATUREFILEIO)
FEATUREFILEIODSTWEB  = FeatureFileIODstWeb.h   $(FEATUREFILEIO)
FEATUREFILEIOPICES   = FeatureFileIOPices.h    $(FEATUREFILEIO)   $(IMAGEFEATURES)
FEATUREFILEIOROBERTS = FeatureFileIORoberts.h  $(FEATUREFILEIO)  
FEATUREFILEIOSPARSE  = FeatureFileIOSparse.h   $(FEATUREFILEIO)
FEATUREFILEIOUCI     = FeatureFileIOUCI.h      $(FEATUREFILEIO)

NORMALIZATIONPARMS = NormalizationParms.h  $(FEATURENUMLIST)     $(ATTRIBUTE)

DUPLICATEIMAGES    = DuplicateImages.h     $(IMAGEFEATURES)

CLASSASSIGNMENTS   = ClassAssignments.h

CHAINHASH          = ChainHash.h     $(FEATURENUMLIST)  $(ATTRIBUTE)

BITREDUCTION       = BitReduction.h  $(CHAINHASH)  $(IMAGEFEATURES)  svm.h

CONFUSSIONMATRIX2  = ConfusionMatrix2.h   $(MLCLASS)

SVM                = svm.h

SVM2               = svm2.h                $(FEATURENUMLIST)     $(FEATUREVECTOR)

BINARYCLASSPARMS   = BinaryClassParms.h    $(SVM)                $(FEATURENUMLIST)


SVMPARAM           = SVMparam.h


SVMMODEL           = SVMModel.h            $(CLASSASSIGNMENTS)   $(FEATUREVECTOR)      $(FILEDESC)  \
                     $(MLCLASS)         $(SVM)                $(BITREDUCTION)       $(SVMPARAM)


FEATUREENCODER     = FeatureEncoder.h      $(ATTRIBUTE)          $(BITREDUCTION)       $(FEATUREVECTOR)   \
                     $(FILEDESC)           $(MLCLASS)         $(SVM)                $(SVMPARAM)


MODELPARAM         = ModelParam.h          $(FEATURENUMLIST)     $(FILEDESC)
  

MODELPARAMDUAL     = ModelParamDual.h      $(MODELPARAM)


MODELPARAMKNN      = ModelParamKnn.h       $(MODELPARAM)


MODELPARAMOLDSVM   = ModelParamOldSVM.h    $(BINARYCLASSPARMS)   $(FEATURENUMLIST)     $(MLCLASS)  \
                     $(FILEDESC)           $(MODELPARAM)         $(SVM)                $(SVMPARAM)


MODELPARAMSVMBASE  = ModelParamSvmBase.h   $(MODELPARAM)         $(SVM2)


MODELPARAMUSFCASCOR = ModelUsfCasCor.h     $(MODELPARAM)  


FEATUREENCODER2    = FeatureEncoder2.h     $(ATTRIBUTE)          $(BITREDUCTION)       $(FEATURENUMLIST)  \
                     $(FEATUREVECTOR)      $(FILEDESC)           $(MLCLASS)         $(MODELPARAM)  


CLASSPROB          = ClassProb.h


MODEL              = Model.h               $(CLASSASSIGNMENTS)   $(FEATUREENCODER2)    $(FEATUREVECTOR)      \
                     $(FILEDESC)           $(MLCLASS)         $(MODELPARAM)         $(NORMALIZATIONPARMS) \
                     $(SVM)                $(SVMPARAM)     


MODELDUAL          = ModelDual.h           $(MODEL)


MODELKNN           = ModelKnn.h            $(FEATUREVECTOR)      $(MODEL)              $(MODELPARAMKNN)


MODELOLDSVM        = $(BINARYCLASSPARMS)   $(FILEDESC)           $(MLCLASS)         $(FEATUREENCODER2)    \
                     $(FEATURENUMLIST)     $(FEATUREVECTOR)      $(SVM)                $(MODEL)              \
                     $(MODELPARAMOLDSVM)   $(SVMMODEL)


MODELSVMBASE       = ModelSvmBase.h        $(FEATURENUMLIST)     $(FEATUREVECTOR)      $(FILEDESC)           \
                     $(MLCLASS)         $(MODEL)              $(MODELPARAM)         $(MODELPARAMSVMBASE)  \
                     $(SVM2)
                     
                     
USFCASCOR          = UsfCasCor.h  

  
MODELUSFCASCOR     = ModelUsfCasCor.h      $(MODEL)              $(MODELPARAM)         $(MODELPARAMUSFCASCOR)  $(USFCASCOR)
 
  

SIZEDISTRIBUTION   = SizeDistribution.h    $(MLCLASS)         $(IMAGEFEATURES)


SIPPEREXTRACTIONIMAGEMANAGER = SipperExtractionImageManager.h \
                     $(MLCLASS)         $(IMAGEFEATURES)      $(SIZEDISTRIBUTION) 


SVMWRAPPER         = SvmWrapper.h          $(SVMPARAM)


TRAININGCLASS      = TrainingClass.h


TRAININGCONFIGURATION2 = TrainingConfiguration2.h  \
                     $(FILEDESC)           $(FEATUREVECTOR)      $(MODEL)          $(MODELPARAM)        \
                     $(MODELPARAMOLDSVM)   $(SVMPARAM)           $(TRAININGCLASS)


TRAININGPROCESS2   = TrainingProcess2.h    $(FEATUREVECTOR)      $(IMAGEFEATURESDATAINDEXED)   \
                     $(IMAGEFEATURESNAMEINDEXED)                 $(NORMALIZATIONPARMS)         \
                     $(MODEL)              $(MODELOLDSVM)        $(SVMMODEL)                   \
                     $(TRAININGCONFIGURATION2)


CLASSIFIER2        = Classifier2.h         $(MODEL)          $(MODELPARAMOLDSVM)    $(MODELOLDSVM)  \
                     $(MODELPARAMOLDSVM)   $(SVMMODEL)    


CROSSVALIDATIONTHREEWAY = CrossValidationThreeWay.h

CROSSVALIDATION        = CrossValidation.h         $(BITREDUCTION)
CROSSVALIDATIONVOTING  = CrossValidationVoting.h
CROSSVALIDATIONMXN     = CrossValidationMxN.h

CLASSIFICATIONBIASMATRIX = ClassificationBiasMatrix.h  ConfusionMatrix2.h   MLClass.h


CMDLINEEXPANDER = CmdLineExpander.h


OBJECTS  = Attribute.o                  BinaryClassParms.o           BitReduction.o               \
           ChainHash.o                  ClassAssignments.o           ClassProb.o                  \
           Classifier2.o                ClassStatistic.o             ClassificationBiasMatrix.o   \
           ConfusionMatrix2.o           CrossValidation2.o           CrossValidationMxN.o         \
           CrossValidationThreeWay.o    CrossValidationVoting.o      DuplicateImages.o            \
           FeatureEncoder.o             FeatureEncoder2.o            FeatureFileIO.o              \
           FeatureFileIOArff.o          FeatureFileIOC45.o           FeatureFileIOColumn.o        \
           FeatureFileIODstWeb.o        FeatureFileIOPices.o         FeatureFileIORoberts.o       \
           FeatureFileIOSparse.o        FeatureFileIOUCI.o           FeatureNumList.o             \
           FeatureVector.o              FileDesc.o                   ImageFeatures.o              \
           ImageFeaturesDataIndexed.o   ImageFeaturesNameIndexed.o   MLClass.o                    \
           MLClassConstList.o           Model.o                      ModelDual.o                  \
           ModelSvmBase.o               \
           ModelKnn.o                   ModelOldSVM.o                ModelUsfCasCor.o             \
           ModelParam.o                 ModelParam.o                 ModelParamDual.o             \
           ModelParamSvmBase.o          ModelParamKnn.o              ModelParamOldSVM.o           \
           ModelParamUsfCasCor.o        NormalizationParms.o         \
           Orderings.o                  SipperExtractionImageManager.o                            \
           SizeDistribution.o           svm.o                        svm2.o                       \
           SVMModel.o                   SVMparam.o                   SvmWrapper.o                 \
           TrainingClass.o              TrainingConfiguration2.o     \
           TrainingProcess2.o           UsfCasCor.o
           
           
all:	$(EXEC)

clean:
	rm *.o;  rm *.a

$(EXEC): $(OBJECTS) 
	rm *.a; ar cq libPicesLibrary.a  $(OBJECTS)

Attribute.o : Attribute.cpp   $(ATTRIBUTE)   $(FEATURENUMLIST)  $(MLCLASS)  $(IMAGEFEATURES)  
	$(CC) $(CFLAGS) -c Attribute.cpp


BinaryClassParms.o :   BinaryClassParms.cpp  $(BINARYCLASSPARMS) $(MLCLASS) svm.h
	$(CC) $(CFLAGS) -c BinaryClassParms.cpp


BitReduction.o :       BitReduction.cpp         $(BITREDUCTION)    $(FEATUREVECTOR)     $(FILEDESC)
	$(CC) $(CFLAGS) -c BitReduction.cpp


ChainHash.o :          ChainHash.cpp            $(CHAINHASH)       $(FEATURENUMLIST)
	$(CC) $(CFLAGS) -c ChainHash.cpp


ClassAssignments.o :   ClassAssignments.cpp     $(CLASSASSIGNMENTS)     $(MLCLASS)
	$(CC) $(CFLAGS) -c ClassAssignments.cpp


ClassProb.o :   ClassProb.cpp     $(CLASSPROB)     $(MLCLASS)
	$(CC) $(CFLAGS) -c ClassProb.cpp
	

Classifier2.o :        Classifier2.cpp          $(CLASSIFIER2)            $(MLCLASS)   \
                       $(NORMALIZATIONPARMS)    $(TRAININGPROCESS2)
	$(CC) $(CFLAGS) -c Classifier2.cpp


ClassStatistic.o : ClassStatistic.cpp  $(CLASSSTATISTIC)   $(MLCLASS)   
	$(CC) $(CFLAGS) -c ClassStatistic.cpp


ClassificationBiasMatrix.o : ClassificationBiasMatrix.cpp  $(CLASSIFICATIONBIASMATRIX)    $(MLCLASS)   
	$(CC) $(CFLAGS) -c ClassificationBiasMatrix.cpp


ConfusionMatrix2.o :  ConfusionMatrix2.cpp    $(CONFUSSIONMATRIX2)
	$(CC) $(CFLAGS) -c ConfusionMatrix2.cpp


CrossValidation2.o :  ./CrossValidation.cpp          $(CROSSVALIDATION)     $(CLASSIFIER2)            \
                      $(CONFUSSIONMATRIX2)           $(FEATUREVECTOR)       $(TRAININGCONFIGURATION2) \
                      $(TRAININGPROCESS2)
	$(CC) $(CFLAGS) -c ./CrossValidation.cpp  -o CrossValidation2.o



CrossValidationMxN.o :  ./CrossValidationMxN.cpp  \
                        $(CONFUSSIONMATRIX2)   $(CROSSVALIDATION)         $(FEATUREVECTOR)    \
                        $(ORDERINGS)           $(TRAININGCONFIGURATION2)   $(TRAININGPROCESS2)
	$(CC) $(CFLAGS) -c CrossValidationMxN.cpp  -o CrossValidationMxN.o


CrossValidationThreeWay.o : CrossValidationThreeWay.cpp  $(CROSSVALIDATIONTHREEWAY)     $(CLASSIFIER2)     \
                            $(CONFUSSIONMATRIX2)         $(FEATUREVECTOR)               $(MLCLASS)     \
                            $(TRAININGCLASS)             $(TRAININGCONFIGURATION2)      $(TRAININGPROCESS2)
	$(CC) $(CFLAGS) -c CrossValidationThreeWay.cpp


CrossValidationVoting.o : CrossValidationVoting.cpp  $(CROSSVALIDATIONVOTING)                             \
                          $(CLASSIFIER2)             $(CONFUSSIONMATRIX2)        $(FEATUREVECTOR)         \
                          $(FILEDESC)                $(MLCLASS)               $(TRAININGCONFIGURATION2) \
                          $(TRAININGPROCESS2)
	$(CC) $(CFLAGS) -c CrossValidationVoting.cpp



DuplicateImages.o :  DuplicateImages.cpp         $(DUPLICATEIMAGES)           $(FEATUREVECTOR) \
                     $(IMAGEFEATURESNAMEINDEXED) $(IMAGEFEATURESDATAINDEXED)
	$(CC) $(CFLAGS) -c DuplicateImages.cpp



FeatureEncoder.o :   FeatureEncoder.cpp     $(FEATUREENCODER)   $(BITREDUCTION)  $(BINARYCLASSPARMS) \
                    $(FEATURENUMLIST)       $(FEATUREVECTOR)    $(SVMWRAPPER)
	$(CC)  $(CFLAGS) -c FeatureEncoder.cpp



FeatureEncoder2.o :  FeatureEncoder2.cpp    $(FEATUREENCODER2)  $(BINARYCLASSPARMS)    $(FEATURENUMLIST) \
                     $(FEATUREVECTOR)        $(SVMWRAPPER)
	$(CC)  $(CFLAGS) -c FeatureEncoder2.cpp


FeatureFileIO.o :   FeatureFileIO.cpp  $(FEATUREFILEIO)  $(FILEDESC)  $(MLCLASS)
	$(CC) $(CFLAGS) -c FeatureFileIO.cpp


FeatureFileIOArff.o : FeatureFileIOArff.cpp  $(FEATUREFILEIOARFF)  $(FILEDESC)  $(MLCLASS)
	$(CC) $(CFLAGS) -c FeatureFileIOArff.cpp 


FeatureFileIOC45.o : FeatureFileIOC45.cpp  $(FEATUREFILEIOC45)   $(FILEDESC)  $(MLCLASS)
	$(CC) $(CFLAGS) -c FeatureFileIOC45.cpp


FeatureFileIOColumn.o : FeatureFileIOColumn.cpp  $(FEATUREFILEIOCOLUMN)   $(FILEDESC)  $(MLCLASS)
	$(CC) $(CFLAGS) -c FeatureFileIOColumn.cpp


FeatureFileIODstWeb.o : FeatureFileIODstWeb.cpp  $(FEATUREFILEIODSTWEB)    $(FILEDESC)  $(MLCLASS)
	$(CC) $(CFLAGS) -c FeatureFileIODstWeb.cpp


FeatureFileIOPices.o : FeatureFileIOPices.cpp  $(FEATUREFILEIOPICES)   $(FILEDESC)  $(MLCLASS)  $(IMAGEFEATURES)
	$(CC) $(CFLAGS) -c FeatureFileIOPices.cpp


FeatureFileIORoberts.o : FeatureFileIORoberts.cpp  $(FEATUREFILEIOROBERTS)
	$(CC) $(CFLAGS) -c FeatureFileIORoberts.cpp


FeatureFileIOSparse.o : FeatureFileIOSparse.cpp  $(FEATUREFILEIOSPARSE)
	$(CC) $(CFLAGS) -c FeatureFileIOSparse.cpp


FeatureFileIOUCI.o : FeatureFileIOUCI.cpp  $(FEATUREFILEIOUCI)
	$(CC) $(CFLAGS) -c FeatureFileIOUCI.cpp


FeatureNumList.o :   FeatureNumList.cpp  $(FEATURENUMLIST)  $(FILEDESC)
	$(CC) $(CFLAGS) -c FeatureNumList.cpp

FeatureVector.o :   FeatureVector.cpp  $(FEATUREVECTOR) $(FEATURENUMLIST)  $(FEATUREFILEIO)  $(MLCLASS) 
	$(CC) $(CFLAGS) -c FeatureVector.cpp


FileDesc.o :        FileDesc.cpp  $(FILEDESC)  $(FEATURENUMLIST)  $(MLCLASS)  $(IMAGEFEATURES)
	$(CC) $(CFLAGS) -c FileDesc.cpp



ImageFeatures.o :      ImageFeatures.cpp  $(IMAGEFEATURES)  $(FEATURENUMLIST)  $(FEATUREFILEIO)  $(MLCLASS)
	$(CC) $(CFLAGS) -c ImageFeatures.cpp 


ImageFeaturesDataIndexed.o : ImageFeaturesDataIndexed.cpp   $(IMAGEFEATURESDATAINDEXED)  $(IMAGEFEATURES)
	$(CC) $(CFLAGS) -c ImageFeaturesDataIndexed.cpp


ImageFeaturesNameIndexed.o : ImageFeaturesNameIndexed.cpp  $(IMAGEFEATURESNAMEINDEXED)   $(IMAGEFEATURES)
	$(CC) $(CFLAGS) -c ImageFeaturesNameIndexed.cpp


MLClass.o :         MLClass.cpp   $(MLCLASS)   $(FILEDESG)  $(MLCLASSCONSTLIST)
	$(CC) $(CFLAGS) -c MLClass.cpp


MLClassConstList.o : MLClassConstList.cpp   $(MLCLASSCONSTLIST)   $(FILEDESC)  $(MLCLASS)  
	$(CC) $(CFLAGS) -c MLClassConstList.cpp


ModelParam.o :  ModelParam.cpp       $(MODELPARAM)        $(FILEDESC)            $(MLCLASS)        \
                $(MODELPARAM)        $(MODELPARAMKNN)     $(MODELPARAMOLDSVM)    $(MODELPARAMSVMBASE)
	$(CC) $(CFLAGS) -c ModelParam.cpp


ModelParamDual.o :  ModelParamDual.cpp       $(MODELPARAMDUAL)    $(FILEDESC)   $(MLCLASS)  $(TRAININGCONFIGURATION2)
	$(CC) $(CFLAGS) -c ModelParamDual.cpp


ModelParamKnn.o :  ModelParamKnn.cpp       $(MODELPARAMKNN)    $(FILEDESC)   $(MLCLASS)         
	$(CC) $(CFLAGS) -c ModelParamKnn.cpp


ModelParamOldSVM.o :  ModelParamOldSVM.cpp       $(MODELPARAMOLDSVM)    $(BINARYCLASSPARMS) $(FILEDESC)   $(MLCLASS) 
	$(CC) $(CFLAGS) -c ModelParamOldSVM.cpp


ModelParamSvmBase.o :  ModelParamSvmBase.cpp       $(MODELPARAMSVMBASE)    $(FILEDESC)   $(MLCLASS) 
	$(CC) $(CFLAGS) -c ModelParamSvmBase.cpp


ModelParamUsfCasCor.o :  ModelParamUsfCasCor.cpp   $(MODELPARAMUSFCASCOR)    $(FILEDESC)   $(MLCLASS) 
	$(CC) $(CFLAGS) -c ModelParamUsfCasCor.cpp


Model.o : Model.cpp   $(MODEL)               $(CLASSPROB)        $(FEATUREENCODER2)   $(FEATURENUMLIST)     $(FEATUREVECTOR)       \
                      $(MODELSDUAL)          $(MODELOLDSVM)      $(MODELSVMBASE)      $(MODELKNN)           $(MODELUSFCASCOR)      \
                      $(MODELPARAM)          $(MODELPARAMDUAL)   $(MODELPARAMOLDSVM)  $(MODELPARAMSVMBASE)  $(MODELPARAMUSFCASCOR) \
                      $(NORMALIZATIONPARMS)  $(SVMWRAPPER)
	$(CC) $(CFLAGS) -c  Model.cpp


ModelDual.o :         ModelDual.cpp              $(MODELSDUAL)       $(CLASSPROB)     $(FEATURENUMLIST)     $(FEATUREVECTOR)      \
                      $(TRAININGCONFIGURATION2)  $(TRAININGPROCESS2)
	$(CC) $(CFLAGS) -c ModelDual.cpp


ModelKnn.o :          ModelKnn.h                 $(MODELKNN)         $(CLASSPROB)     $(FEATURENUMLIST)     $(FEATUREVECTOR)  $(SVMWRAPPER)
	$(CC) $(CFLAGS) -c ModelKnn.cpp


ModelOldSVM.o :       ModelOldSVM.cpp          $(MODELOLDSVM)        $(BINARYCLASSPARMS)    $(CLASSPROB)    $(FEATUREENCODER2) \
                      $(FEATURENUMLIST)        $(FEATUREVECTOR)         $(MODELPARAMOLDSVM)      $(SVMPARAM)
	$(CC) $(CFLAGS) -c ModelOldSVM.cpp


ModelSvmBase.o :      ModelSvmBase.cpp         $(MODELSVMBASE)          $(CLASSPROB)          $(FEATURENUMLIST)     $(FEATUREVECTOR)         $(SVM2)
	$(CC) $(CFLAGS) -c ModelSvmBase.cpp


ModelUsfCasCor.o :    ModelUsfCasCor.cpp     $(MODELUSFCASCOR)        $(CLASSPROB)          $(FEATURENUMLIST)     $(FEATUREVECTOR)    $(MLCLASS)
	$(CC) $(CFLAGS) -c ModelUsfCasCor.cpp


NormalizationParms.o : NormalizationParms.cpp   $(NORMALIZATIONPARMS)    $(FEATURENUMLIST)    \
                       $(MLCLASS)            $(FEATUREVECTOR)         $(TRAININGCONFIGURATION2)
	$(CC) $(CFLAGS) -c NormalizationParms.cpp


Orderings.o :          Orderings.cpp            $(ORDERINGS)             $(FEATUREFILEIO)    $(FILEDESC)   \
                       $(MLCLASS)            $(FEATUREVECTOR)
	$(CC) $(CFLAGS) -c Orderings.cpp


SipperExtractionImageManager.o : SipperExtractionImageManager.cpp  $(SIPPEREXTRACTIONIMAGEMANAGER)  \
                                 $(FEATUREFILEIO)    $(MLCLASS)  $(IMAGEFEATURES)
	$(CC) $(CFLAGS) -c SipperExtractionImageManager.cpp


SizeDistribution.o : SizeDistribution.cpp $(SIZEDISTRIBUTION)  $(MLCLASS) $(FEATUREVECTOR)
	$(CC) $(CFLAGS) -c SizeDistribution.cpp


svm.o :                svm.cpp              $(SVM)
	$(CC) $(CFLAGS) -c svm.cpp
  

svm2.o :               svm2.cpp             $(SVM2)              $(FEATUREENCODER)
	$(CC) $(CFLAGS) -c svm2.cpp


SVMModel.o :           SVMModel.cpp         $(SVMMODEL)          $(BITREDUCTION)   \
                       $(BINARYCLASSPARMS)  $(FEATUREENCODER)    $(FEATURENUMLIST) \
                       $(FEATUREVECTOR)     $(SVMWRAPPER)
	$(CC) $(CFLAGS) -c SVMModel.cpp



SVMparam.o :           SVMparam.cpp            $(SVMPARAM)    $(BINARYCLASSPARMS)  \
                       $(CLASSASSIGNMENTS)     $(FILEDESC)$   $(MLCLASS)   
	$(CC) $(CFLAGS) -c SVMparam.cpp


SvmWrapper.o :         SvmWrapper.cpp           $(SVMWRAPPER)           $(SVM)
	$(CC) $(CFLAGS) -c SvmWrapper.cpp


TrainingClass.o :      TrainingClass.cpp        $(TRAININGCLASS)        $(MLCLASS)
	$(CC) $(CFLAGS) -c TrainingClass.cpp


TrainingConfiguration2.o : TrainingConfiguration2.cpp  $(TRAININGCONFIGURATION2)      $(BINARYCLASSPARMS)    \
                           $(FEATUREFILEIO)            $(FEATUREFILEIOPICES)          $(FEATURENUMLIST)      \
                           $(FEATUREVECTOR)            $(FILEDESC)                    $(MLCLASS)          \
                           $(IMAGEFEATURES)            $(MODEL)                       $(MODELPARAMKNN)       \
                           $(MODELPARAMOLDSVM)         $(MODELPARAMSVMBASE)           $(NORMALIZATIONPARMS)
	$(CC) $(CFLAGS) -c TrainingConfiguration2.cpp


TrainingProcess2.o :   TrainingProcess2.cpp         $(TRAININGPROCESS2)          $(CLASSASSIGNMENTS)      \
                       $(DUPLICATEIMAGES)           $(FEATUREFILEIO)             $(FEATUREFILEIOPICES)    \
                       $(FEATURENUMLIST)            $(FEATUREVECTOR)             $(MLCLASS)            \
                       $(MODEL)                     $(MODELOLDSVM)               $(MODELKNN)              \
                       $(MODELSVMBASE)              $(TRAININGCLASS)             $(TRAININGCONFIGURATION2)
	$(CC) $(CFLAGS)  -c TrainingProcess2.cpp



UsfCasCor.o :          UsfCasCor.cpp                $(USFCASCOR)                 $(CLASSPROB)            \
                       $(FEATUREVECTOR)             $(MLCLASS)
	$(CC) $(CFLAGS)  -c UsfCasCor.cpp
