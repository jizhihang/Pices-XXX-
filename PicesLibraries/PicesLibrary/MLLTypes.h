#ifndef  _MLLTYPES_
#define  _MLLTYPES_


/*!
 \namespace  MLL
 \brief  Namespace for all Machine Learning and SIPPER specific code.
 \details 
 A library of routines used for Machine Learning, building Classifiers, etc.  It also includes code that is 
 specific to PICES and SIPPER.  At som point ion the future we need split this library into three libraries.
 MachineLearning, DataBase, and PicesRoutines.

 This library covers three different areas that at some time in the future should be made into 
 three seperate libraries.<br />
 1) Generic Machine Learning code.<br />
 2) Database Related Code.<br />
 3) Analysis.<br />
 4) PICES specific code that would be built onto of 1 and 2.<br />

 \section MachineLearninig "Machine Learning Code"

 \subsection  FeatureData "Feature Data"
 There are several Feature Data formats supported.  Each one has its own class that is derived from 
 'FeatureFileIO'.  The decription of the data is managed by 'FileDesc'.  For each type of dataset there 
 will exist on one instance of a FileDesc class.
 \see Attribute
 \see FeatureNumList
 \see FeatureFileIO
 \see FeatureVector
 \see FeatureVectorList
 \see MLClass
 \see BitReduction
 \see FeatureEncoder
 \see NormalizationParms
 \see Orderings

 \subsection  LearningAlgorithms "Learning Algorithms"
 There are several learning algorithms implemented.  The Learning algorithms are all subclassed from 'Model' and their
 related parameters are all subclassed from 'ModelParam'.
 \see Model
 \see ModelKnn
 \see ModelOldSVM
 \see ModelSvmBase
 \see ModelParam
 \see TrainingClass
 \see TrainingProcess2

 \subsection AnalysisRoutines  "Analysis Routines"
 \see ConfusionMatrix2
 \see CrossValidation
 \see CrossValidationMxN
 \see CrossValidationVoting
 \see SizeDistribution

 \section  DataBase  "Data Base"
 \see  DataBase
 \see  DataBaseServer

 \section  PICESSpecific "PICES Specific"
 This is code that is built on the SipperInstruments (aka SipperHardware Namespace),  Machine Learning Code, Featuire Data Code
 \see ImageFeatures
 \see TrainingConfiguration2
 \see DataBaseImage
 \see DuplicateImages
 \see SipperExtractionImageManager
 */

namespace  MLL  
{
}


#endif

