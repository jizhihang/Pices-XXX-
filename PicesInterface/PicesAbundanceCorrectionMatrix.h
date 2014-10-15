#pragma once

#include "DateTime.h"
#include "Str.h"
#include "Matrix.h"
using namespace  KKU;

#include "MLClass.h"
using namespace  MLL;

#include "PicesRunLog.h"
#include "PicesClassList.h"
#include "PicesClassStatistic.h"


namespace PicesInterface 
{
  /**
   *@class  PicesAbundanceCorrectionMatrix
   *@brief Assists in adjusting a Classifiers output for bias of a classifier.
   *@details
   * See the paper: 
   *@code
   *    "Accurate automatic quantification of taxa-specific plankton abundance using dual classification with correction"
   *    by: Qiao Hu, Cabell Davis
   *    Date: 2006-01-11
   *    http://darchive.mblwhoilibrary.org:8080/handle/1912/4501
   *@endcode
   *This class uses the results of a n-fold cross validation to compute parameters that are used to adjust the 
   * results of a classification run.  The application "AbundanceCorrectionStatsBuilder" will create a parameter 
   * file in the directory "${PicesHomeDir}\\DataFiles\\TrainingModels\\SaveFiles" that contains the parameters.  
  */

  public  ref class PicesAbundanceCorrectionMatrix
  {
  public:
    /**
     *@brief  Will constuct a instance from from the parameter file for '_configFileName'.
     *@param[in] _configFileName The name of the BiasMatrix file will be derived from the configuration 
     *                           file name.  Se the code for 'the term 'PicesAbundanceCorrectionMatrix' in PicesLibrary'.
	   *@param[in,out] _runLog     Logging file to use.
     */
    PicesAbundanceCorrectionMatrix (PicesClassList^  _classes,
                                    PicesClass^      _otherClass,
                                    PicesRunLog^     _runLog
                                   );
                             
    PicesAbundanceCorrectionMatrix (String^       _configFileName,
                                    PicesRunLog^  _runLog
                                   );

    !PicesAbundanceCorrectionMatrix ();

    ~PicesAbundanceCorrectionMatrix ();


    System::DateTime   DateTimeComputed ();
    array<float>^      ProbOfDetection  ();
    array<float>^      ProbOfFalseAlarm ();
    bool               Valid            ();


    void  AddIn (PicesAbundanceCorrectionMatrix^  matrix,
                 PicesRunLog^                     runLog
                );


    PicesClassStatisticList^   AdjustClassificationCounts (PicesClassStatisticList^  inputCounts,
                                                           PicesRunLog^              log
                                                          );

    void  ComputeStatistics (); /**< computing  probOfDetection  and  probOfFalseAlarm(Specifiity) */

    PicesClassStatisticList^   LumpCounts (PicesClassStatisticList^  inputCounts);

    void  Prediction (PicesClass^   knownClass,
                      PicesClass^   predClass,
                      PicesRunLog^  log
                     );

    void  PrintConfusionMatrixTabDelimited (StreamWriter^  outFile);

    void  PrintAdjustedResults (StreamWriter^             sw,
                                PicesClassStatisticList^  classifiedCounts,
                                PicesRunLog^              runLog
                               );

    void  SaveForConfigFileName (String^  configFileName);


  private:
    MLL::AbundanceCorrectionMatrixPtr  abundanceMatrix;
  };
}  /* namespace PicesInterface */
