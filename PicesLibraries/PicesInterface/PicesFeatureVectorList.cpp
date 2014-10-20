#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>

#include  <ctype.h>
#include  <time.h>

#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;


using namespace System;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"
#include  "..\\BaseLibrary\\GoalKeeper.h"
#include  "OSservices.h"

using namespace KKU;


#include "FeatureFileIo.h"
#include "FeatureFileIOPices.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "OSservices.h"
#include "Raster.h"

#include "PicesKKStr.h"
#include "PicesRaster.h"
#include "PicesRunLog.h"
#include "PicesFeatureVector.h"
#include "PicesFeatureVectorList.h"


using namespace  PicesInterface;


namespace  PicesInterface
{
  PicesFeatureVectorList::PicesFeatureVectorList ():
    cancelFlag            (new bool (false)),
    classes               (NULL),
    numExamplesWritten    (NULL),
    sortedByImageFileName (false)
  {
  }


  PicesFeatureVectorList::PicesFeatureVectorList (ImageFeaturesList&  examples): // Will take ownership of "examples"  contents.
    cancelFlag            (new bool (false)),
    classes               (NULL),
    numExamplesWritten    (NULL),
    sortedByImageFileName (false)
  {
    ImageFeaturesList::iterator  idx;
    for  (idx = examples.begin ();  idx != examples.end ();  idx++)
    {
      ImageFeaturesPtr  example = *idx;
      PicesFeatureVector^  pfv = gcnew PicesFeatureVector (example);
      Add (pfv);
    }

    examples.Owner (false);
  }




  PicesFeatureVectorList::PicesFeatureVectorList (FeatureVectorList&  examples)
  {
    PicesFeatureVector^  pfv = nullptr;

    while  (examples.size () > 0)
    {
      FeatureVectorPtr  fv = examples.PopFromFront ();
      if  (strcmp (fv->UnderlyingClass (), "ImageFeatures") == 0)
      {
        pfv = gcnew PicesFeatureVector (dynamic_cast<ImageFeaturesPtr> (fv));
        fv = NULL;
      }
      else
      {
        ImageFeaturesPtr  imageFeatures = new ImageFeatures (*fv);
        delete  fv;  fv = NULL;
        pfv = gcnew PicesFeatureVector (imageFeatures);  // 'pfv' will take ownership of 'imageFeatures'.
      }

      Add (pfv);
    }
  }






  PicesFeatureVectorList::~PicesFeatureVectorList ()
  {
    Clear ();
    this->!PicesFeatureVectorList ();
  }



  PicesFeatureVectorList::!PicesFeatureVectorList ()
  {
    CleanUpUnmanagedResources ();
  }



  void  PicesFeatureVectorList::CleanUpUnmanagedResources ()
  {
    delete  classes;             classes    = NULL;
    delete  cancelFlag;          cancelFlag = NULL;
    delete  numExamplesWritten;  numExamplesWritten = NULL;
  }  /* CleanUpUnmanagedResources */




  void  PicesFeatureVectorList::CancelLoad ()  
  {
    if  (!cancelFlag)
      cancelFlag = new bool (true);
    *cancelFlag = true;
  }



  uint  PicesFeatureVectorList::NumExamplesWritten::get ()
  {
    if  (numExamplesWritten)
      return *numExamplesWritten;
    else
      return 0;
  }




  PicesFeatureVectorList^   PicesFeatureVectorList::CreateListForAGivenLevel (uint          level,
                                                                              PicesRunLog^  runLog
                                                                             )
  {
    PicesFeatureVectorList^  specificLevelList = gcnew PicesFeatureVectorList ();

    for  (int x = 0;  x < this->Count;  x++)
    {
      PicesFeatureVector^  pfv = (*this)[x];

      ImageFeaturesPtr  fvForLevel = new ImageFeatures (*(pfv->Features ()));
      MLClassPtr  classForLevel = fvForLevel->MLClass ()->MLClassForGivenHierarchialLevel (level);
      fvForLevel->MLClass (classForLevel);
      specificLevelList->Add (gcnew PicesFeatureVector (fvForLevel));
    }
    return  specificLevelList;
  }  /* CreateListForAGivenLevel */





  void  PicesFeatureVectorList::LoadInSubDirectoryTree 
                                         (String^       rootDir,
                                          bool          useDirectoryNameForClassName,
                                          PicesRunLog^  log,
                                          bool          rewiteRootFeatureFile
                                         )
  {
    if  (classes)
    {
      delete  classes;
      classes = NULL;
    }

    classes = new MLClassConstList ();

    if  (!cancelFlag)
      cancelFlag = new bool (false);

    ImageFeaturesListPtr  featureData = FeatureFileIOPices::Driver ()->LoadInSubDirectoryTree 
                                       (PicesKKStr::SystemStringToKKStr (rootDir),
                                        *classes,
                                        useDirectoryNameForClassName,
                                        *cancelFlag,
                                        rewiteRootFeatureFile,
                                        log->Log ()
                                       );

    if  (*cancelFlag)
    {
      delete  featureData;
      featureData = NULL;
      return;
    }

    featureData->Owner (false);  // The owership of the individule ImageFeature objects will be taken over by 
                                  // this container object "PicesFeatureVectorList".

    ImageFeaturesList::iterator  idx;
    for  (idx = featureData->begin ();  idx != featureData->end ();  idx++)
    {
      ImageFeaturesPtr fv = *idx;
      PicesFeatureVector^   pfv = gcnew PicesFeatureVector (fv);  // We just turned ownership of 'fv' over to 'PicesFeatureVectorList'
      Add (pfv);
    }

    delete  featureData;
    featureData = NULL;

    return;
  }  /* LoadInSubDirectoryTree */



  PicesClassList^  PicesFeatureVectorList::ExtractListOfClasses ()
  {
    PicesClassList^  classes = gcnew PicesClassList ();
    for each (PicesFeatureVector^ pfv in (*this))
    {
      if  (classes->LookUpIndex (pfv->MLClass) < 0)
        classes->Add (pfv->MLClass);
    }

    return  classes;
  } /* ExtractListOfClasses */



  void  PicesFeatureVectorList::RandomizeOrder ()
  {
    int  numExamples = Count;

    Random^  randGen = gcnew Random ();
    for  (int x = 0;  x < Count;  x++)
    {
      int y = randGen->Next (numExamples);
      PicesFeatureVector^  temp = (*this)[x];
      (*this)[x] = (*this)[y];
      (*this)[y] = temp;
    }
  }  /* RandomizeOrder */



  PicesFeatureVectorList^  PicesFeatureVectorList::ExtractImagesForAGivenClass (PicesClass^  mlClass)
  {
    PicesFeatureVectorList^  extractedExamples = gcnew PicesFeatureVectorList ();

    for each (PicesFeatureVector^  pfv  in (*this))
    {
      if  (pfv->MLClass == mlClass)
        extractedExamples->Add (pfv);
    }

    return  extractedExamples;
  }  /* ExtractImagesForAGivenClass */




  void  PicesFeatureVectorList::AddQueue (PicesFeatureVectorList^ subQueue)
  {
    for each (PicesFeatureVector^  pfv in subQueue)
      Add (pfv);
  }  /* AddQueue */




  PicesFeatureVectorList^  PicesFeatureVectorList::StratifyAmoungstClasses (int numOfFolds)
  {
    int  foldNum = 0;
    PicesFeatureVectorList^  stratifiedExamples = gcnew PicesFeatureVectorList ();

    PicesClassList^  classes = ExtractListOfClasses ();

    array<PicesFeatureVectorList^>^ examplesEachFold = gcnew array<PicesFeatureVectorList^> (numOfFolds);
    for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
     examplesEachFold[foldNum] = gcnew PicesFeatureVectorList ();

    for each (PicesClass^  pc in classes)
    {
      PicesFeatureVectorList^  examplesThisClass = ExtractImagesForAGivenClass (pc);
      examplesThisClass->RandomizeOrder ();
      foldNum = 0;
      for each (PicesFeatureVector^  pfv in examplesThisClass)
      {
        if  (foldNum >= numOfFolds)
          foldNum = 0;
        examplesEachFold[foldNum]->Add (pfv);
        foldNum++;
      }

      examplesThisClass = nullptr;
    }

    for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
    {
      PicesFeatureVectorList^ examplesThisFold = examplesEachFold[foldNum];
      examplesThisFold->RandomizeOrder ();
      stratifiedExamples->AddQueue (examplesThisFold);
      examplesThisFold = nullptr;
      examplesEachFold[foldNum] = nullptr;
    }

    examplesEachFold = nullptr;

    return  stratifiedExamples;
  }  /* StratifyAmoungstClasses */




  ref class  PicesFeatureVectorList::FeatureVectorComparitorByImageFileName : public System::Collections::Generic::IComparer<PicesFeatureVector^>
  {
  public:
    virtual  int  Compare (PicesFeatureVector^ a, 
                           PicesFeatureVector^ b
                          )
    {
      String^ sA = OSservices::GetRootName (a->ImageFileName);
      String^ sB = OSservices::GetRootName (b->ImageFileName);
      return  sA->CompareTo (sB);
    }
  };





  void  PicesFeatureVectorList::SortByImageFileName ()
  {
    FeatureVectorComparitorByImageFileName^ c = gcnew FeatureVectorComparitorByImageFileName ();
    Sort (c);
    sortedByImageFileName = true;
  }




  PicesFeatureVector^  PicesFeatureVectorList::LookUpByImageFileName (String^  imageFileName)
  {
    if  (!sortedByImageFileName)
      SortByImageFileName ();

    String^  imageFileRootName = OSservices::GetRootName (imageFileName);

    int  left = 0;
    int  right = Count - 1;
    int  middle = -1;

    PicesFeatureVector^  fv = nullptr;

    FeatureVectorComparitorByImageFileName^  comparer = gcnew FeatureVectorComparitorByImageFileName ();

    while  (left < right)
    {
      middle = (left + right) / 2;
      String^  middleRootName = OSservices::GetRootName ((this)[middle]->ImageFileName);
      int  x = middleRootName->CompareTo (imageFileRootName);

      if  (x < 0)
        left = middle + 1;

      else if  (x > 0)
        right = middle - 1;

      else
      {
        fv = (this)[middle];
        break;
      }
    }

    return  fv;
  }  /* LookUpByImageFileName */



  // Creates an unmanaged list of feature vectors.
  FeatureVectorListPtr  PicesFeatureVectorList::ToFeatureVectorList (PicesRunLog^  runLog)
  {
    FeatureVectorListPtr  fvl = new FeatureVectorList (FeatureFileIOPices::NewPlanktonFile (runLog->Log ()), false, runLog->Log (), Count);

    for each (PicesFeatureVector^ fv in *this)
      fvl->PushOnBack (fv->UnManagedClass ());

    return  fvl;
  }  /* ToFeatureVectorList */



  PicesFeatureVectorList^  PicesFeatureVectorList::ExtractDulicateExamples (PicesRunLog^  runLog)
  {
    /*
    PicesFeatureVectorList^  duplicateList = gcnew PicesFeatureVectorList ();

    Dictionary<String^,PicesFeatureVector^>^  dictionary = gcnew Dictionary<String^,PicesFeatureVector^> ();

    for each (PicesFeatureVector^ fv in *this)
    {
      String^  rootName = OSservices::GetRootName (fv->ImageFileName);
      if  (dictionary->ContainsKey (rootName))
      {
        duplicateList->Add (fv);

        Dictionary<String^,PicesFeatureVector^>::Enumerator^  idx = dictionary->TryGetValue GetEnumerator ();
        idx->
        

      }

      dictionary->Add (OSservices::GetRootName (fv->ImageFileName), fv);
    }
    */
    return nullptr;
  }




  void  PicesFeatureVectorList::Save (String^       fileName,
                                      String^       fileFormat,
                                      PicesRunLog^  runLog
                                     )
  {
    FeatureVectorListPtr  fvl = ToFeatureVectorList (runLog);
    bool  successful  = false;

    if  (!numExamplesWritten)
      numExamplesWritten = new uint;
    *numExamplesWritten = 0;

    FeatureFileIOPtr driver = 
         FeatureFileIO::FileFormatFromStr (PicesKKStr::SystemStringToKKStr (fileFormat), false, true);
    if  (driver == NULL)
      driver = FeatureFileIOPices::Driver ();

    try
    {
      driver->SaveFeatureFile
                    (PicesKKStr::SystemStringToKKStr (fileName), 
                     fvl->AllFeatures (), 
                     *fvl,
                     *numExamplesWritten,
                     *cancelFlag,
                     successful,
                     runLog->Log ()
                    );
    }
    catch  (Exception^  e)  
    {
      throw  gcnew Exception ("PicesFeatureVectorList::Save", e);
    }

    if (!successful)
      throw  gcnew Exception ("PicesFeatureVectorList::Save");

  }  /* Save */

}  /* PicesInterface */
