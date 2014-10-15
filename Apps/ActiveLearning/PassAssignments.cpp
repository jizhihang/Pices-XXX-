#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <stdio.h>
#include  <memory>
#include  <math.h>
#include  <limits.h>
#include  <float.h>

#include  <iostream>
#include  <fstream>
#include  <string>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;



#include  "OSservices.h"


#include  "PassAssignments.h"




PassAssignments::PassAssignments (FileDescPtr         _fileDesc,
                                  KKStr              _fileName,
                                  FeatureVectorList&  _images,
                                  RunLog&             _log,
                                  bool&               _successfull
                                 ):

  assignments                     (NULL),
  fileDesc                        (_fileDesc),
  mlClasses                    (),
  imagesPerClass                  (NULL),
  initialTrainingImagesPerClass   (-1),
  log                             (_log),
  numOfClasses                    (-1),
  numOfRandomPasses               (-1),
  totalNumOfInitialTrainingImages (-1)
{
  if  (!fileDesc)
  {
    log.Level (-1) << endl
                   << "PassAssignments::PassAssignments    *** ERROR ***" << endl
                   << "                                    FileDesc == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  Read (_fileName, _images, _successfull);
}




PassAssignments::PassAssignments (FileDescPtr         _fileDesc,
                                  FeatureVectorList&  _images,
                                  MLClassList&     _mlClasses,
                                  int                 _numOfRandomPasses,
                                  int                 _initialTrainingImagesPerClass,
                                  bool                _usePriorForIIPC,
                                  RunLog&             _log
                                 ):

  assignments                     (NULL),
  fileDesc                        (_fileDesc),
  mlClasses                    (_mlClasses),
  initialTrainingImagesPerClass   (_initialTrainingImagesPerClass),
  log                             (_log),
  numOfClasses                    (-1),
  imagesPerClass                  (NULL),
  numOfRandomPasses               (_numOfRandomPasses),
  usePriorForIIPC                 (_usePriorForIIPC),
  totalNumOfInitialTrainingImages (-1)

{
  if  (!fileDesc)
  {
    log.Level (-1) << endl
                   << "PassAssignments::PassAssignments    *** ERROR ***" << endl
                   << "                                    FileDesc == NULL" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  numOfClasses = _mlClasses.QueueSize ();
  PopulateImagesPerClass (_images);

  totalNumOfInitialTrainingImages = _initialTrainingImagesPerClass * numOfClasses;

  AllocateAssignments ();

  for  (int pass = 0; pass < numOfRandomPasses;  pass++)
  {
    _images.RandomizeOrder ();
    _images.RandomizeOrder ();
    _images.RandomizeOrder ();

    MLClassPtr  mlClass = NULL;

    int  numInRandomPass = 0;
    int  classIDX        = 0;

    for  (classIDX = 0;  classIDX < numOfClasses;  ++classIDX)
    {
      mlClass = mlClasses.IdxToPtr (classIDX);
      int  numImagesToExtract = imagesPerClass[classIDX];
      FeatureVectorListPtr  imagesInClass = _images.ExtractImagesForAGivenClass (mlClass, numImagesToExtract);

      if  (imagesInClass->QueueSize () < numImagesToExtract)
      {
         numImagesToExtract = imagesInClass->QueueSize ();

         cerr << endl
              << "PassAssignments::PassAssignments     *** WARNING ***"  << endl
              << "                                     Not Enough Images in Class[" << mlClass->Name () << "]" << endl
              << endl;
      }


      for  (int idx = 0;  ((idx < numImagesToExtract)  &&  (numInRandomPass < totalNumOfInitialTrainingImages))  ;  idx++)
      {
        assignments[pass][numInRandomPass] = new KKStr (imagesInClass->IdxToPtr (idx)->ImageFileName ());
        numInRandomPass++;
      }

      delete  imagesInClass;
    }
  }
}





PassAssignments::~PassAssignments ()
{
  FreeUpAssignments ();
  delete  imagesPerClass;
  imagesPerClass = NULL;
}



void  PassAssignments::PopulateImagesPerClass (FeatureVectorList&  images)
{
  int            classIDX = 0;
  MLClassPtr  mlClass = NULL;
  delete  imagesPerClass;
  imagesPerClass = new int[numOfClasses];

  if  (!usePriorForIIPC)
  {
    for  (classIDX = 0;  classIDX < mlClasses.QueueSize ();  classIDX++)
       imagesPerClass[classIDX] = initialTrainingImagesPerClass;
  }
  else
  {
    int  totalNumImageszInInitialTrainLibrary = mlClasses.QueueSize () * initialTrainingImagesPerClass;

    int  imageCount = 0;

    for  (classIDX = 0;  classIDX < mlClasses.QueueSize ();  classIDX++)
    {
      mlClass = mlClasses.IdxToPtr (classIDX);
      int  imagesToExtractThisClass = totalNumImageszInInitialTrainLibrary - imageCount;
      if  (classIDX < (mlClasses.QueueSize () - 1))
      {
        FeatureVectorListPtr  imagesInThisClass = images.ExtractImagesForAGivenClass (mlClass);
        float  fracThisClass = (float)(imagesInThisClass->QueueSize ()) / (float)(images.QueueSize ());
        imagesToExtractThisClass = (int)(fracThisClass * (float)totalNumImageszInInitialTrainLibrary + 0.5f);
        delete  imagesInThisClass;
      }
      imagesPerClass[classIDX] = imagesToExtractThisClass;
      imageCount += imagesToExtractThisClass;
    }
  }
}  /* PopulateImagesPerClass */








void  PassAssignments::FreeUpAssignments ()
{
  if  (!assignments)
    return;

  for  (int pass = 0; pass < numOfRandomPasses;  pass++)
  {
    if  (assignments[pass])
    {
      for  (int idx = 0;  idx < totalNumOfInitialTrainingImages;  idx++)
      {
        if  (assignments[pass][idx])
          delete  assignments[pass][idx];
      }
    }

    delete  assignments[pass];
  }
  
  delete  assignments; assignments = NULL;
}  /* FreeUpAssignments */






void  PassAssignments::AllocateAssignments ()
{
  if  (assignments)
    FreeUpAssignments ();

  assignments = new KKStrPtr*[numOfRandomPasses];

  for  (int pass = 0; pass < numOfRandomPasses;  pass++)
  {
    assignments[pass] = new KKStrPtr[totalNumOfInitialTrainingImages];

    for  (int idx = 0;  idx < totalNumOfInitialTrainingImages;  idx++)
    {
      assignments[pass][idx] = NULL;
    }
  }
}  /* AllocateAssignments */





void  PassAssignments::Save (KKStr  fileName)
{
  ofstream   out (fileName.Str ());

  if  (!out.is_open ())
  {
    log.Level (-1)  << endl
                    << "PassAssignments::Save   *** ERROR ***  Open File[" << fileName << "]" << endl
                    << endl;

    exit (-1);
    osWaitForEnter ();
  }

  int classIDX = 0;

  out << "NumOfRandomPasses"              << "\t" << numOfRandomPasses             << endl;
  out << "InitialTrainingImagesPerClass"  << "\t" << initialTrainingImagesPerClass << endl;
  out << "NUMOFCLASSES"                   << "\t" << numOfClasses                  << endl;

  out << "ClassList";
  for  (int classIDX = 0;  classIDX < mlClasses.QueueSize ();  classIDX++)
    out << "\t" << mlClasses[classIDX].Name () << "\t" << imagesPerClass[classIDX];
  out << endl;
   
  
  for  (int pass = 0; pass < numOfRandomPasses;  pass++)
  {
    for  (int idx = 0;  idx < totalNumOfInitialTrainingImages;  idx++)
    {
       out << pass << "\t" << idx << "\t" << (*assignments[pass][idx]) << endl;
    }
  }

  out.close ();

}  /* Save */







void  PassAssignments::Read (KKStr              fileName,
                             FeatureVectorList&  images,
                             bool&               successfull
                            )
{
  successfull = false;

  FILE*  in = osFOPEN (fileName.Str (), "r");
  if  (!in)
  {
    log.Level (-1) << endl
                   << "*** ERROR ***   PassAssignments::PassAssignments  Opening FileName[" << fileName << "]." << endl
                   << endl;
    return;
  }


  char  buff[2048];
  int   classIDX;


  // First Two Lines Must be Dimensions
  bool  nonPaprameterFound      = false;
  bool  numOfPassesFound        = false;
  bool  numOfInitialImagesFound = false;
  bool  numOfClassesFound       = false;
  bool  classListFound          = false;

  while  (!nonPaprameterFound)
  {
    if  (!fgets (buff, sizeof (buff), in))
    {
      log.Level (-1) << endl
                     << "*** ERROR ***   PassAssignments::PassAssignments  FileName[" << fileName << "].   INVALID FORMAT" << endl
                     << endl;
      fclose (in);
      return;
    }

    KKStr  ln (buff);

    KKStr  fieldName = ln.ExtractToken (" \n\t\r");
    fieldName.Upper ();

    if  (fieldName == "NUMOFRANDOMPASSES")
    {
      numOfRandomPasses = ln.ExtractTokenInt (" \n\t\r");
      numOfPassesFound  = true;
    }

    else if  (fieldName == "INITIALTRAININGIMAGESPERCLASS")
    {
      initialTrainingImagesPerClass = ln.ExtractTokenInt (" \n\t\r");
      numOfInitialImagesFound = true;
    }

    else if  (fieldName == "NUMOFCLASSES")
    {
      numOfClasses = ln.ExtractTokenInt (" \n\t\r");
      numOfClassesFound = true;
    }

    else if  (fieldName == "CLASSLIST")
    {
      // This is a New parameter that will not be in older files.
      classListFound = true;
    }

    else
    {
      nonPaprameterFound = true;
    }
  }

  if  ((!numOfPassesFound)  &&  (!numOfInitialImagesFound)  &&  (!numOfClassesFound))
  {
    log.Level (-1) << endl
                   << "*** ERROR ***   PassAssignments::PassAssignments  FileName[" << fileName << "].   INVALID FORMAT" << endl
                   << endl;
    fclose (in);
    return;
  }


  totalNumOfInitialTrainingImages = initialTrainingImagesPerClass * numOfClasses;
  AllocateAssignments ();

  int  count = 0;
  int  totalExpected = numOfRandomPasses * totalNumOfInitialTrainingImages;


  delete  imagesPerClass;
  imagesPerClass = new int[numOfClasses];
  for  (classIDX = 0;  classIDX < numOfClasses;  classIDX++)
    imagesPerClass[classIDX] = 0;

  bool  moreData = true;

  while  (moreData)
  {
    KKStr  ln (buff);
    KKStr  origLine = ln;

    int     pass          = ln.ExtractTokenInt (" \n\t\r");
    int     idx           = ln.ExtractTokenInt (" \n\t\r");
    KKStr  imageFileName = ln.ExtractToken (" \n\t\r");


    if  ((pass < 0) ||  (pass >= numOfRandomPasses)             ||
         (idx  < 0) ||  (idx  >= totalNumOfInitialTrainingImages)
        )
    {
      log.Level (-1) << endl
                     << "*** ERROR ***   PassAssignments    Invalid Input Line"         << endl
                     << "               [" << origLine << "]"                           << endl
                     << endl;
      fclose (in);
      return;
    }

    if  (assignments[pass][idx] != NULL)
    {
      // Assignment Already in File
      log.Level (-1) << endl
                     << "*** ERROR ***   PassAssignments    Invalid Input Line"         << endl
                     << "               [" << origLine << "]"                           << endl
                     << "               Slot used more than once"                       << endl
                     << endl;
      fclose (in);
      return;
    }
    
    FeatureVectorPtr  image = images.LookUpByImageFileName (imageFileName);

    if  (!image)
    {
      log.Level (-1) << endl
                     << "*** ERROR ***   PassAssignments    Invalid Input Line"                << endl
                     << "               [" << origLine << "]"                                  << endl
                     << "               Image[" << imageFileName << "] is not a Known Image."  << endl
                     << endl;
      fclose (in);
      return;
    }

    if  (pass == 0)
    {
      // Use the first pass to populate imagesPerClass
      classIDX = mlClasses.PtrToIdx (image->MLClass ());
      if  (classIDX < 0)
      {
        if  (mlClasses.QueueSize () >= numOfClasses)
        {
          log.Level (-1) << endl
                         << "*** ERROR ***   PassAssignments    Invalid Number of MLClasses found."  << endl
                         << endl;
          return;
        }

        classIDX = mlClasses.QueueSize ();
        mlClasses.AddMLClass (image->MLClass ());
      }
      imagesPerClass[classIDX]++;
    }

    assignments[pass][idx] = new KKStr (imageFileName);
    count++;

    moreData = (fgets (buff, sizeof (buff), in) != NULL);
  }

  fclose (in);

  if  (count != totalExpected)
  {
    log.Level (-1) << endl
                   << "*** ERROR ***   PassAssignments    Invalid Number Entries In Line"  << endl
                   << "                NumRead[" << count << "]"                           << endl
                   << "                TotalExpeceted[" << totalExpected << "]"            << endl
                   << endl;
     return;
  }

  successfull = true;
}  /* Read */
  




bool  PassAssignments::ImageInPass (const KKStr&  imageFileName, 
                                    int            pass
                                   )
{
  for  (int idx = 0;  idx < totalNumOfInitialTrainingImages;  idx++)
  {
    if  (assignments[pass][idx])
    {
      if  (imageFileName == assignments[pass][idx])
        return true;
    }
  }

  return  false;
}  /* ImageInPass */





void  PassAssignments::ExtractInitialTrainingAndTestImages (int                    pass,
                                                            FeatureVectorList&     masterList,
                                                            MLClassList&        mlClasses,
                                                            FeatureVectorListPtr&  initialTraingImages,
                                                            FeatureVectorListPtr&  testImages
                                                           )
{
  if  ((pass < 0)  ||  (pass > numOfRandomPasses))
  {
    log.Level (-1) << endl
                   << "PassAssignments::ExtractInitialTrainingAndTestImages    *** ERROR ***"  << endl
                   << endl
                   << "                 Pass[" << pass << "]  is greater than numOfRandomPasses[" << numOfRandomPasses << "]." << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  int  numOfTrainigImages = initialTrainingImagesPerClass * mlClasses.QueueSize ();
  int  estNumOfTestImages    = 10 + masterList.QueueSize () * mlClasses.QueueSize ()  / numOfClasses - numOfTrainigImages;
  
  
  initialTraingImages = new FeatureVectorList (fileDesc, false, log, numOfTrainigImages);
  testImages          = new FeatureVectorList (fileDesc, false, log, estNumOfTestImages);
  
  MLClassPtr  mlClass = NULL;

  MLClassListIterator  icIDX (mlClasses);

  for  (icIDX.Reset ();  mlClass = icIDX.CurPtr ();  ++icIDX)
  {
    FeatureVectorListPtr  imagesInClass = masterList.ExtractImagesForAGivenClass (mlClass);

    FeatureVectorPtr  image = NULL;

    ImageFeaturesListIterator imagesIDX (*imagesInClass);

    for  (imagesIDX.Reset ();  image = imagesIDX.CurPtr ();  ++imagesIDX)
    {
      if  (ImageInPass (image->ImageFileName (), pass))
      {
        initialTraingImages->PushOnBack (image);
      }
      else
      {
        testImages->PushOnBack (image);
      }
    }

    delete  imagesInClass;
  }

  testImages->Compress ();
}  /* ExtractInitialTrainingAndTestImages */


