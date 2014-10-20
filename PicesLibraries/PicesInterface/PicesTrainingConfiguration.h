#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace System::IO;

#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "TrainingConfiguration2.h"

#include  "PicesFeatureVectorList.h"
#include  "PicesTrainingClass.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public ref class PicesTrainingConfiguration
  {
  public:
    PicesTrainingConfiguration (PicesRunLog^  _log);

    PicesTrainingConfiguration (String^       _configFileName,
                                PicesRunLog^  _log 
                               );

    PicesTrainingConfiguration (DirectoryInfo^  di,
                                PicesRunLog^    log
                               );
  private:
    ~PicesTrainingConfiguration  ();

  protected:
    !PicesTrainingConfiguration  ();

  public:
    void                      CancelLoad ();  // Call this methid to set the 'canclFlag' to true.

    TrainingConfiguration2Ptr Config ()  {return config;};

    PicesClassList^           ExtractClassList ();

    static
    String^                   GetEffectiveConfigFileName (String^  configFileName);

    String^                   LoadLogStream ()  {return loadLogStream;}

    PicesFeatureVectorList^   LoadFeatureDataFromTrainingLibraries (bool^ changesMadeToTrainingLibraries);

    String^                   ModelName ();

    uint                      NumHierarchialLevels ();  // returns back the number of hierarchail levels there

    PicesClass^               OtherClass ();

    String^                   RootDir ();

    void                      Save (String^  configFileName);

    PicesTrainingClassList^   TrainingClasses ();

    Boolean                   Valid ();

  private:
    void  AdjustCurMemoryPressure ();

    bool*                      cancelFlag;
    TrainingConfiguration2Ptr  config;
    MLClassConstListPtr     classes;
    int                        curMemoryPressure;
    String^                    loadLogStream;
    PicesRunLog^               log;
    Boolean^                   valid;
  };  /* PicesTrainingConfiguration */
}
