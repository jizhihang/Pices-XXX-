#pragma once

#include  "DateTime.h"
#include  "Str.h"
#include  "Matrix.h"
using namespace  KKU;


#include  "PicesRunLog.h"
#include  "PicesClassList.h"


namespace PicesInterface 
{
  /**
   *@class  ClassificationBiasMatrix
   *@brief Assists in adjusting a Classifiers output for bias of a classifier.
   *@details
   * See the paper: 
   *@code
   *     "Estimating the Taxonomic composition of a sample when individules are classified with error"
   *     by Andrew Solow, Cabll Davis, Qiao Hu
   *     Woods Hole Ocanographic Institution, Woods Hole Massachusetts
   *     Marine Ecology Progresss Series
   *     published 2006-july-06;  vol 216:309-311
   *@endcode
   * This class uses the results of a previous 10 fold cross validation confuision matrix to adjust the results of a 
   * classification run.  The application CrossValidation will create a parameter file in the directory 
   * "${PicesHomeDir}\\DataFiles\\TrainingModels" that contains the confusion matrix.  The option "-BiasMatrix" 
   * tells the CrossValidation program to generate this parameter file.  It will have the same name as the Configuration
   * file with the extension ".BiasMatrix.txt" appeneded to the end.  There is a Unmanged version of this class in
   * PicesLibrary called MLL::ClassificationBiasMatrix
   */

  public  ref class ClassificationBiasMatrix
  {
  public:
    ClassificationBiasMatrix (PicesRunLog^  _runLog);   // Will create matrix as defined in the Paper.


    /**
     *@brief  Will constuct a BisMatrix instance from from the parametyer file for '_configFileName'.
     *@param[in] _configFileName The name of the BiasMatrix file will be derived from the configuration 
     *                           file name.  the term '_BiasMatrix.txt' will be apended onto the end.
     *@param[in] _classes        List of classes that you are expecting to be in the 'BiasMatrix' file.
	   *@param[in,out] _runLog     Logging file to use.
     */
    ClassificationBiasMatrix (String^          _configFileName,
                              PicesClassList^  _classes,
                              PicesRunLog^     _runLog
                             );

    !ClassificationBiasMatrix ();

    ~ClassificationBiasMatrix ();

    property  System::DateTime DateTimeFileWritten  {System::DateTime  get ()  {return dateTimeFileWritten;}  void  set (System::DateTime _dateTimeFileWritten) {dateTimeFileWritten = _dateTimeFileWritten;}}


    void   PerformAdjustmnts (array<double>^  classifiedCounts,
                              array<double>^ &adjCounts,
                              array<double>^ &stdErrors
                             );

    void   PrintBiasMatrix (StreamWriter^  sw);

    void   PrintAdjustedResults (StreamWriter^   sw,
                                 array<double>^  classifiedCounts
                                );

    void  TestPaperResults (StreamWriter^   sw);

    bool  Valid ()  {return  valid;}

  private:
    void  BuildTestMatrix ();
    void  DeclareMatrix ();
    void  Read (StreamReader^  r);
    void  ReadSimpleConfusionMatrix (StreamReader^  sr);
   

    String^                 biasFileName;
    PicesClassList^         classes;
    MatrixPtr               counts;
    String^                 configFileName;
    String^                 configFileNameFromMatrixBiasFile;
    System::DateTime        configDateTime;       /*!< TimeStamp of Config file when this BiasMatrix was written. */
    System::DateTime        dateTimeFileWritten;
    MatrixPtr               probabilities;
    int                     numClasses;
    PicesRunLog^            runLog;
    bool                    valid;
  };

}  /* namespace PicesInterface */
