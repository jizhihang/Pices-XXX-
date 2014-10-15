// #include  <stdio.h>
#include  <string>
#include  <iomanip>
#include  <iostream>
#include  <fstream>
#include  <vector>

#ifdef  WIN32
#include  <ostream>
#endif

using namespace std;



#include  <math.h>

#include  "ParameterJob.h"

#include  "Queue.h"
#include  "Str.h"
#include  "OSservices.h"

#include  "FeatureNumList.h"
#include  "SVMparam.h"
#include  "SVMModel.h"




ParameterJob::ParameterJob (RunLog&    _log,
                            int        _processNum,
                            int        _jobId,
                            int        _cParm,
                            double     _gammaParm,
                            int        _aParm
                           ):
  log           (_log),
  processNum    (_processNum),
  jobId         (_jobId),
  cParm         (_cParm),
  gammaParm     (_gammaParm),
  aParm         (_aParm)
{
  accuracy     = 0;
  startTime    = 0;
  curStatus    = '0';
}




ParameterJob::ParameterJob (RunLog&    _log,
                            String     _desc
                           ):
  log (_log)
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
    String  processsNumField = _desc.ExtractToken (" ,\t");
    processNum = atoi (processsNumField.Str ());
  }


  {
    String  jobIdField = _desc.ExtractToken (" ,\t");
    jobId = atoi (jobIdField.Str ());
  }


  {
    String  cParmStr = _desc.ExtractToken (" ,\t");
    cParm = atoi (cParmStr.Str ());
  }


  {
    String  gammaParmStr = _desc.ExtractToken (" ,\t");
    gammaParm = atof (gammaParmStr.Str ());
  }


  {
    String  aParmStr = _desc.ExtractToken (" ,\t");
    aParm = atoi (aParmStr.Str ());
  }


  {
    String  accuracyField = _desc.ExtractToken (" ,\t");
    accuracy = atof (accuracyField.Str ());
  }
}




ParameterJob::ParameterJob (ParameterJob&  parameterJob):
  log           (parameterJob.log),
  processNum    (parameterJob.processNum),
  jobId         (parameterJob.jobId),
  accuracy      (parameterJob.accuracy),
  curStatus     (parameterJob.curStatus),
  startTime     (parameterJob.startTime),
  cParm         (parameterJob.cParm),
  gammaParm     (parameterJob.gammaParm),
  aParm         (parameterJob.aParm)

{
}




ParameterJob::~ParameterJob ()
{
}







void  ParameterJob::CrossValidation (ImageFeaturesList&    images,
                                     ImageClassList&       imageClasses,
                                     String                svmParamStr,
                                     int                   numOfFolds,
                                     ConfussionMatrix2&    results,
                                     double&               numOfSupportVectors,
                                     double&               avgProbability,
                                     double&               avgKnowClassProb,
                                     double&               avgCompact,
                                     double&               compactDev
                                   )

{   
  log.Level (10) << "ParameterJob::CrossValidation" << endl;

  int  correct  = 0;
  int  imageCount = images.QueueSize ();
  int  classNum = 0;
  int  total    = 0;

  numOfSupportVectors = 0.0;
  avgProbability      = 0.0;
  avgKnowClassProb    = 0.0;
  avgCompact          = 0.0;

  double  predClassProb;
  double  knownClassProb;
  double  smallestDistance;

  double  compact;
  double* compactTable = new double[imageCount];
  int     compactIDX = 0;


  ImageClassListIterator icIDX (imageClasses);
  for  (icIDX.Reset (); icIDX.CurPtr (); ++icIDX)
  {
    icIDX->Num (classNum);
    classNum++;
  }


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


    ImageFeaturesListPtr  trainingImages = new ImageFeaturesList (false, log);

    ImageFeaturesListPtr  testImages     = new ImageFeaturesList (false, log);


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

    String  featureNumStr ("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72");

    FeatureNumList  selectedFeatures (IncludeFeatureNums, featureNumStr);

    bool  validParms;

    SVMparam  svmParam (svmParamStr, selectedFeatures, log, validParms);

    svmParam.A_Param     (aParm);
    svmParam.C_Param     (cParm);
    svmParam.Gamma_Param (gammaParm);
   
    SVMModelPtr  model = new SVMModel (svmParam, *trainingImages, imageClasses, log);

    numOfSupportVectors += model->NumOfSupportVectors ();

    ImageFeaturesListIterator  tiIDX (*testImages);

    for  (tiIDX.Reset (); tiIDX.CurPtr (); ++tiIDX)
    {
      ImageClassPtr  knownClass      = tiIDX->ImageClass ();
      //ImageClassPtr  predictedClass  = model->Predict (tiIDX.CurPtr ());

      compact = 0;


      double  compact;
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
                         predClassProb,
                         smallestDistance
                        );

      avgProbability   += predClassProb;
      avgKnowClassProb += knownClassProb;
      avgCompact       += compact;

      compactTable[compactIDX] = compact;
      compactIDX++;
    }

    delete  model;

    delete  trainingImages;
    delete  testImages;

    firstInGroup = firstInGroup + numImagesPerFold;
  }  // foldNum

  numOfSupportVectors = numOfSupportVectors / numOfFolds;
  avgProbability      = avgProbability      / imageCount;
  avgKnowClassProb    = avgKnowClassProb    / imageCount;
  avgCompact          = avgCompact          / imageCount; 
  
  accuracy = 100.0 * (double)correct / (double)total;


  // Lets Calc compat Stats
  double  compactVar = 0.0;
  double  delta;

  for  (compactIDX = 0;  compactIDX < imageCount;  compactIDX++)
  {
    delta = (avgCompact - compactTable[compactIDX]);
    compactVar += (delta * delta);
  }

  compactVar = sqrt (compactVar);
  compactDev = compactVar / imageCount;

  delete  compactTable;


  cout << "Job ID[" << jobId << "]  Accuracy[" << StrFormatDouble (accuracy, "zz0.000") << "%]." << endl;
}  /* CrossValidation */





String  ParameterJob::ToString ()
{
  String  statusStr ("");

  statusStr.Append (curStatus);


  statusStr << "\t" << processNum
            << "\t" << jobId 
            << "\t" << cParm
            << "\t" << gammaParm
            << "\t" << aParm
            << "\t" << accuracy;
            
  
  return  statusStr;
} /* ToString */





ParameterJobList::ParameterJobList (bool  _owner)  :  
           Queue<ParameterJob> (_owner, 100)
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


