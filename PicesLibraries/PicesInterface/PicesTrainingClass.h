#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;


#include "TrainingClass.h"
using namespace KKU;
using namespace MLL;

#include "PicesClass.h"
#include "PicesKKStr.h"
#include "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref  class PicesTrainingClass
  {
  public:
    PicesTrainingClass (PicesClass^  _class,
                        String^      _directory
                       );

    PicesTrainingClass (TrainingClassPtr  _trainingClass);

    property  String^      Directory    {String^      get ();    void  set (String^      _directory);}
    property  PicesClass^  MLClass      {PicesClass^  get ();    void  set (PicesClass^  _mlClass);}

    String^   ExpandedDirectory (String^  rootDir);


    TrainingClassPtr  UnManagedClass ();
  private:


    TrainingClassPtr  trainingClass;
  };



  public  ref  class  PicesTrainingClassList:  public  List<PicesTrainingClass^>
  {
  public:
    PicesTrainingClassList ();

    PicesTrainingClassList (TrainingClassListPtr  trainingClasses);

  private:
  };  /* PicesTrainingClassList */
}
