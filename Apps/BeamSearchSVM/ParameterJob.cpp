#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  <math.h>

#include  "ParameterJob.h"

#include  "KKQueue.h"
#include  "Str.h"
#include  "OSservices.h"

#include  "ClassAssignments.h"
#include  "FeatureNumList.h"
#include  "SVMparam.h"
#include  "SVMModel.h"




ParameterJob::ParameterJob (RunLog&    _log,
                            int        _processNum,
                            int        _jobId,
                            int        _cParm,
                            double     _gammaParm,
                            float      _aParm
                           ):
  cancelFlag    (false),
  log           (_log),
  processNum    (_processNum),
  jobId         (_jobId),
  cParm         (_cParm),
  gammaParm     (_gammaParm),
  aParm         (_aParm),
  trainTime     (0.0f),
  classTime     (0.0)
{
  accuracy     = 0;
  startTime    = 0;
  curStatus    = '0';
}




ParameterJob::ParameterJob (RunLog&    _log,
                            KKStr     _desc
                           ):
  cancelFlag (false),
  log        (_log)
{
  accuracy     = 0;
  startTime    = 0;

  _desc.TrimLeft ();

  curStatus = _desc.ExtractChar ();
 
  if  ((curStatus == '0')  ||  
       (curStatus == '1')  ||  
       (curStatus == '2')  ||  
       (curStatus == '3')
      )
  {
    // We Are Ok
  }
  else
  {
    log.Level (-1) << endl 
                   << "ParameterJob:  We have an invalid Cur Status."
                   << endl;
    osWaitForEnter ();
  }


  {
    processNum = _desc.ExtractTokenInt    (" ,\t");
    jobId      = _desc.ExtractTokenInt    (" ,\t");
    cParm      = _desc.ExtractTokenInt    (" ,\t");
    gammaParm  = _desc.ExtractTokenDouble (" ,\t");
    aParm      = _desc.ExtractTokenDouble (" ,\t");
    accuracy   = _desc.ExtractTokenDouble (" ,\t");
    trainTime  = (float)_desc.ExtractTokenDouble (" ,\t");
    classTime  = (float)_desc.ExtractTokenDouble (" ,\t");
    numSVs     = (float)_desc.ExtractTokenDouble (" ,\t");
  }
}




ParameterJob::ParameterJob (ParameterJob&  parameterJob):
  cancelFlag    (parameterJob.cancelFlag),
  log           (parameterJob.log),
  processNum    (parameterJob.processNum),
  jobId         (parameterJob.jobId),
  accuracy      (parameterJob.accuracy),
  curStatus     (parameterJob.curStatus),
  startTime     (parameterJob.startTime),
  cParm         (parameterJob.cParm),
  gammaParm     (parameterJob.gammaParm),
  aParm         (parameterJob.aParm),
  trainTime     (parameterJob.trainTime),
  classTime     (parameterJob.classTime),
  numSVs        (parameterJob.numSVs)
{
}




ParameterJob::~ParameterJob ()
{
}




void  ParameterJob::CrossValidation (ImageFeaturesList&    images,
                                     ImageClassList&       imageClasses,
                                     FeatureNumList&       selectedFeatures,
                                     KKStr                 svmParamStr,
                                     int                   numOfFolds,
                                     ConfussionMatrix2&    results,
                                     double&               numOfSupportVectors,
                                     float&                avgProbability,
                                     float&                avgKnowClassProb,
                                     double&               avgCompact,
                                     double&               compactDev
                                   )

{   
  log.Level (10) << "ParameterJob::CrossValidation" << endl;

  int  correct  = 0;
  int  imageCount = images.QueueSize ();
  int  total    = 0;

  FileDescPtr  fileDesc = images.FileDesc ();

  numOfSupportVectors = 0.0;
  avgProbability      = 0.0;
  avgKnowClassProb    = 0.0;
  avgCompact          = 0.0;
  trainTime           = 0.0f;
  classTime           = 0.0f;
  numSVs              = 0;

  float   predClassProb;
  float   knownClassProb;
  double  smallestDistance;

  double  compact;
  double* compactTable = new double[imageCount];
  int     compactIDX = 0;

  
  // Creae a default assignments based of current odering
  // of imageClasses.
  ClassAssignments  assignments (imageClasses, log);

  int  numImagesPerFold = (imageCount + numOfFolds - 1) / numOfFolds;

  int  firstInGroup = 0;


  for  (int  foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    cout << "Job ID[" << jobId << "]  Fold[" << foldNum << "]." << endl;


    int  lastInGroup;

    // If We are doing the last Fold Make sure that we are including all the images 
    // that have not been tested.
    if  (foldNum == (numOfFolds - 1))
      lastInGroup = imageCount;
    else
      lastInGroup = firstInGroup + numImagesPerFold - 1;

    ImageFeaturesListPtr  trainingImages = new ImageFeaturesList (fileDesc, false, log);

    ImageFeaturesListPtr  testImages     = new ImageFeaturesList (fileDesc, false, log);

    log.Level (20) << "Fold Num["        << foldNum        << "]   "
                   << "FirstTestImage["  << firstInGroup   << "]   "
                   << "LastInGroup["     << lastInGroup    << "]."
                   << endl;

    ImageFeaturesPtr  image = NULL;

    for  (int  x = 0; x < imageCount; x++)
    {
      image = images.IdxToPtr (x);
      if  (image == NULL)
      {
        cout << "*" << endl;
      }

      if  ((x >= firstInGroup)  &&  (x <= lastInGroup))
      {
        testImages->AddSingleImageFeatures (image);
      }
      else
      {
        trainingImages->AddSingleImageFeatures (image);
      }
    }

    bool  validParms;

    SVMparam  svmParam (svmParamStr, selectedFeatures, fileDesc, log, validParms);

    svmParam.A_Param           (aParm);
    svmParam.C_Param           (cParm);
    svmParam.Gamma_Param       (gammaParm);
    svmParam.SelectedFeatures  (selectedFeatures);
   
    SVMModelPtr  model = new SVMModel (svmParam, *trainingImages, assignments, fileDesc, log, cancelFlag);

    trainTime += (float)model->TrainingTime ();
    numOfSupportVectors += model->NumOfSupportVectors ();
    numSVs    += model->NumOfSupportVectors ();

    ImageFeaturesListIterator  tiIDX (*testImages);

    double  classStartTime = osGetSystemTimeUsed ();

    for  (tiIDX.Reset (); tiIDX.CurPtr (); ++tiIDX)
    {
      ImageClassPtr  knownClass      = tiIDX->ImageClass ();
      //ImageClassPtr  predictedClass  = model->Predict (tiIDX.CurPtr ());

      compact = 0;

      int     numOfWinners;
      bool    knownClassOneOfTheWinners;
      float   probability1;             // 5 Other Ways to look at Probability,
      float   probability2;             // 5 Other Ways to look at Probability,
      float   probability3;             // Impolmented for some Active Learning 
      float   probability4;             // expirements.
      float   probability5;
      float   probability6;

      ImageClassPtr  predictedClass  = model->Predict (tiIDX.CurPtr (), 
                                                       knownClass, 
                                                       predClassProb,
                                                       knownClassProb,
                                                       smallestDistance,
                                                       compact,
                                                       numOfWinners,
                                                       knownClassOneOfTheWinners,
                                                       probability1,            // 5 Other Ways to look at Probability,
                                                       probability2,            // 5 Other Ways to look at Probability,
                                                       probability3,            // Impolmented for some Active Learning 
                                                       probability4,             // expirements.
                                                       probability5,
                                                       probability6
                                                      );

      if  (compact > 10000000000.0)
        compact = 10000000000.0;


      predictedClass = imageClasses.LookUpByName (predictedClass->Name ());
      total++;
      if  (knownClass->UpperName () == predictedClass->UpperName ())
      {
        correct++;
      }
               
      results.Increment (knownClass, 
                         predictedClass, 
                         (int)tiIDX->OrigSize (),
                         predClassProb
                        );

      avgProbability   += predClassProb;
      avgKnowClassProb += knownClassProb;
      avgCompact       += compact;

      compactTable[compactIDX] = compact;
      compactIDX++;
    }

    double  classEndTime = osGetSystemTimeUsed ();
    classTime += (float)(classEndTime - classStartTime);

    delete  model;

    delete  trainingImages;
    delete  testImages;

    firstInGroup = firstInGroup + numImagesPerFold;
  }  // foldNum


  numOfSupportVectors = numOfSupportVectors / numOfFolds;
  avgProbability      = avgProbability      / imageCount;
  avgKnowClassProb    = avgKnowClassProb    / imageCount;
  avgCompact          = avgCompact          / (double)imageCount; 
  
  accuracy = 100.0 * (double)correct / (double)total;


  // Lets Calc compat Stats
  double  compactVar = 0.0;
  double  delta;

  for  (compactIDX = 0;  compactIDX < imageCount;  compactIDX++)
  {
    delta = (avgCompact - compactTable[compactIDX]);
    compactVar += (delta * delta);
  }

  compactVar = compactVar / (double)imageCount;
   
  compactDev = sqrt (compactVar);;

  delete  compactTable;


  cout << "Job ID[" << jobId << "]  Accuracy[" << StrFormatDouble (accuracy, "zz0.000") << "%]." << endl;
}  /* CrossValidation */





KKStr  ParameterJob::ToString ()
{
  KKStr  statusStr ("");

  statusStr.Append (curStatus);


  statusStr << "\t" << processNum
            << "\t" << jobId 
            << "\t" << cParm
            << "\t" << gammaParm
            << "\t" << aParm
            << "\t" << accuracy
            << "\t" << trainTime
            << "\t" << classTime
            << "\t" << numSVs;
            
  
  return  statusStr;
} /* ToString */





ParameterJobList::ParameterJobList (bool  _owner)  :  
           KKQueue<ParameterJob> (_owner, 100)
{
}




ParameterJobPtr  ParameterJobList::LocateByProcessNum (int  _processNum)
{
  ParameterJobPtr  j = NULL;

  int  idx = 0;


  while  (idx < QueueSize ())
  {
    j = IdxToPtr (idx);
    if  (j->ProcessNum () == _processNum)
      return  j;

    idx++;
  }

  return  NULL;
}





ParameterJobListIterator::ParameterJobListIterator (ParameterJobList&  runs):
     QueueIterator<ParameterJob> (runs)
{
}


