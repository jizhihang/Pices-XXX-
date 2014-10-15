#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;

//using namespace PicesCSharpClasses;


#include  "..\\BaseLibrary\\OSservices.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{

  public ref class PicesSipperVariables
  {
  public:
    PicesSipperVariables ();

    static  bool     AllowUpdates                   ();  /**< 'True' indicates that user is allowed to peofrom updates.  */
    static  void     UpdatesNotAllowed (String^  caption);

    static  String^  InstrumentDataFilesDir         ();
    static  String^  PicesConfigurationDirectory    ();  // Where application configuration files go;  NOT training models.
    static  String^  PicesCmdLineProgDir            ();  // Directory where you can find Command Line programs such as "CrossValidation.exe"
    static  String^  PicesDataFilesDirectory        ();
    static  String^  PicesHomeDir                   ();
    static  String^  PicesTempDirectory             ();
    static  String^  PicesDataFileDirectory         ();
    static  String^  PicesReportDir                 ();
    static  String^  SipperConfigrationFileName     ();
    static  String^  SipperDirectoryListFileName    ();
    static  String^  SipperFilesDefaultDir          ();
    static  String^  SipperFilesDirectory           ();   // Same as 'SipperFilesDefaultDir'
    static  String^  SipperInterfaceDir             ();
    static  String^  SipperInterfaceLogDir          ();
    static  String^  TrainingModelsConfigurationDir ();
  };
}