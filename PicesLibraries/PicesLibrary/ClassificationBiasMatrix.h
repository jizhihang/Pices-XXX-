#pragma once

#include "DateTime.h"
#include "Str.h"
#include "Matrix.h"
#include "RunLog.h"

#include "ConfusionMatrix2.h"
#include "MLClass.h"


//  Job in Life:   This module is the Managed version of "DataBaseServer".
//
// See the paper: 
//    "Estimating the Taxonomic composition of a sample when individuals are classified with error"
//     by Andrew Solow, Cabll Davis, Qiao Hu
//     Woods Hole Ocanographic Institution, Woods Hole Massachusetts
//     Marine Ecology Progresss Series
//     published 2006-july-06;  vol 216:309-311


namespace MLL 
{
  /**
   *@class  ClassificationBiasMatrix
   *@brief Assists in adjusting a Classifiers output for the bias of a classifier.
   *@details
   *See the paper: 
   *@code
   *    "Estimating the Taxonomic composition of a sample when individuals are classified with error"
   *    by Andrew Solow, Cabll Davis, Qiao Hu
   *    Woods Hole Oceanographic Institution, Woods Hole Massachusetts
   *    Marine Ecology Progress Series
   *    published 2006-july-06;  vol 216:309-311
   *@endcode
   *This class uses the results of a previous 10 fold cross validation confusion matrix to adjust the results of a
   *classification run.  The application CrossValidation will create a parameter file in the directory 
   *"${PicesHomeDir}\\DataFiles\\TrainingModels" that contains the confusion matrix.  The option "-BiasMatrix" 
   *tells the CrossValidation program to generate this parameter file.  It will have the same name as the Configuration
   *file with the extension ".BiasMatrix.txt" appended to the end.  There is a managed version of this class in
   *PicesInterface called PicesInterface::ClassificationBiasMatrix
  */
  class  ClassificationBiasMatrix
  {
  public:
    typedef  ClassificationBiasMatrix*  ClassificationBiasMatrixPtr;

    typedef  KKU::RunLog      RunLog;
    typedef  KKU::KKStr       KKStr;
    typedef  KKU::DateTime    DateTime;
    typedef  KKU::Matrix      Matrix;
    typedef  KKU::MatrixPtr   MatrixPtr;


    /**
     *@brief  Will construct a ClassificationBiasMatrix instance from a input stream.
     *@param[in]      f        Input Stream.
     *@param[in]      classes  List of classes we expect to find in the input stream f.
     *@param[in,out]  log      Logging File.
     */
    static
    ClassificationBiasMatrixPtr  BuildFromIstreamXML (istream&              f,
                                                      MLClassConstList&  classes,
                                                      RunLog&               log
                                                     );


    /**
     *@brief Will construct an instance as defined in the Paper. 
     */
    ClassificationBiasMatrix (RunLog&  _runLog);   


    ClassificationBiasMatrix (const ClassificationBiasMatrix &  cbm);

    ClassificationBiasMatrix (MLClassConstList&  classes,
                              RunLog&               _runLog
                             );


    /**
     *@brief  Construct a ClassificationBiasMatrix instance from from the parameter file for '_configFileName'.
     *@param[in]     _configFileName The name of the BiasMatrix file will be derived from the configuration 
     *                               file name.  the term '_BiasMatrix.txt' will be appended onto the end.
     *@param[in]     _classes        List of classes that you are expecting to be in the 'BiasMatrix' file.
     *@param[in,out] _runLog         Logging file to use.
     */
    ClassificationBiasMatrix (const KKStr&          _configFileName,
                              MLClassConstList&  _classes,          // List of classes you expect to find in BiasMatrix file.
                              RunLog&               _runLog
                             );

    /**
     *@brief  Construct a ClassificationBiasMatrix instance from a ConfusionMatrix object.
     *@param[in]      cm        Confusion Matrix to get parameters from. 
     *@param[in,out]  _runLog   Logging file to use.
     */
    ClassificationBiasMatrix (const ConfusionMatrix2&  cm,
                              RunLog&                  _runLog
                             );

    ~ClassificationBiasMatrix ();

    DateTime  DateTimeFileWritten ()  const  {return  dateTimeFileWritten;}


    void   PerformAdjustmnts (const VectorDouble&  classifiedCounts,
                              VectorDouble&        adjCounts,
                              VectorDouble&        stdErrors
                             );

    void   PrintBiasMatrix (ostream&  sw);

    void   PrintAdjustedResults (ostream&              sw,
                                 const VectorDouble&   classifiedCounts
                                );

    void   TestPaperResults (ostream&    sw);

    void   WriteXML (ostream&  o);


  private:
    void  BuildFromConfusionMatrix (const ConfusionMatrix2&  cm);
    void  BuildTestMatrix ();
    void  DeclareMatrix ();
    void  ReadXML (istream&   r);
    void  ReadSimpleConfusionMatrix (istream&           sr,
                                     MLClassListPtr  fileClasses
                                    );
   

    KKStr                   biasFileName;
    MLClassConstListPtr  classes;
    MatrixPtr               counts;
    KKStr                   configFileName;
    KKStr                   configFileNameFromMatrixBiasFile;
    DateTime                configDateTime;       /**< TimeStamp of Config file when this BiasMatrix was written */
    DateTime                dateTimeFileWritten;
    VectorKKStr             errMsgs;
    int32                   numClasses;
    MatrixPtr               probabilities;
    RunLog&                 runLog;
    bool                    valid;
  };

  typedef  ClassificationBiasMatrix::ClassificationBiasMatrixPtr  ClassificationBiasMatrixPtr;
}  /*  namespace MLL  */
