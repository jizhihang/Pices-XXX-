#pragma once


using namespace System;
using namespace System::Collections::Generic;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


#include "MLClass.h"
#include "MLClassConstList.h"
#include "ImageFeatures.h"
#include "PicesRunLog.h"

#include "PicesFeatureVector.h"

namespace PicesInterface {

  public ref class PicesFeatureVectorList: List<PicesFeatureVector^>
  {
  public:
    PicesFeatureVectorList ();

    PicesFeatureVectorList (ImageFeaturesList&  examples);  // Will take ownership of "examples"  contents.

    PicesFeatureVectorList (FeatureVectorList&  examples);  // Will take ownership of "examples"  contents  and reduce it to an empty list.
                                                            // will convert constents into instances of 'ImageFeatures'.  If current contents 
                                                            // are instances of 'FeatureVector' will delete them; so make sure that they 
                                                            // arte not being used anywhere else.


  private:
    ~PicesFeatureVectorList ();

  protected:
    !PicesFeatureVectorList ();

  public:


	  property  uint  NumExamplesWritten   {uint get (); }

    property  bool  Owner                {bool get () {return owner;}  void  set (bool _owner)  {owner = _owner;} }



    void  AddQueue (PicesFeatureVectorList^ subQueue);


    void  CancelLoad ();


    /**
     *@brief  Creates a list of FeatureVectors where the class assignment will refect the
     *  specified Hierarhy level specified by 'level'.  
     *@details
     *  Will create a list of FeatureVectors where the class assignment will refect the
     *  specified Hierarhy level specified by 'level'.  The hierarchy of a iven class will
     *  be indicated by underscore characters in the class name.
     *@code
     *  ex:   Level 1:  gelatinous
     *        Level 2:  gelatinous_hydromedusae
     *        Level 3:  gelatinous_hydromedusae_solmundella
     *@endcode
     */
    PicesFeatureVectorList^   CreateListForAGivenLevel (uint          level,
                                                        PicesRunLog^  runLog
                                                       );

    PicesFeatureVectorList^  ExtractImagesForAGivenClass (PicesClass^  mlClass);

    PicesClassList^  ExtractListOfClasses ();


    void  LoadInSubDirectoryTree (String^       rootDir,
                                  bool          useDirectoryNameForClassName,
                                  PicesRunLog^  log,
                                  bool          rewiteRootFeatureFile
                                 );
    
    PicesFeatureVector^  LookUpByImageFileName (String^  imageFileName);

    void  RandomizeOrder ();

    void  Save (String^       fileName,
                String^       fileFormat,
                PicesRunLog^  runLog
               );

    void  SortByImageFileName ();

    PicesFeatureVectorList^   StratifyAmoungstClasses (int  numOfFolds);

    // Will create a "FeatureVectorList" object that will contain the unmanaged instances that are in this list.
    // it will not own thses instances because the Managed objects that currently own them.  And if you delete them it will cause 
    // memory problems.
    FeatureVectorListPtr      ToFeatureVectorList (PicesRunLog^  runLog);  // Creates an unmanaged list of feature vectors.

    /**
     *@brief Will return a list of examples that are dupliactes, that is they have the same root file name.  
     *@details If no duplicates are found then 'nullptr' will be returned.
     */
    PicesFeatureVectorList^    ExtractDulicateExamples (PicesRunLog^  runLog);

  private:
    void  CleanUpUnmanagedResources ();


    bool*                   cancelFlag;   // the "CancelLoad" methid will monitor this flag;  if set to true will terminate.
    MLClassConstListPtr  classes;
    uint*                   numExamplesWritten;  // used by SaveFeatureFile 
    bool                    owner;
    bool                    sortedByImageFileName;

    ref  class  FeatureVectorComparitorByImageFileName;
  };  /* PicesFeatureVectorList */

}  /* PicesInterface */
