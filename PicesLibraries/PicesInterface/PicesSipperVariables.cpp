#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

#include "SipperVariables.h"
using namespace SipperHardware;

#include "OSservices.h"
#include "PicesKKStr.h"
#include "PicesSipperVariables.h"

using namespace  PicesInterface;

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace KKU;
using namespace System::Windows::Forms;





PicesSipperVariables::PicesSipperVariables (void)
{
}



bool PicesSipperVariables::AllowUpdates ()
{
  return SipperVariables::AllowUpdates ();
}


void  PicesSipperVariables::UpdatesNotAllowed (String^  caption)
{
  System::Windows::Forms::MessageBox::Show ("Updates are not allowed.", caption);
  return;
}


String^  PicesSipperVariables::InstrumentDataFilesDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::InstrumentDataFilesDir ());
}



String^  PicesSipperVariables::PicesHomeDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesHomeDir ());
}


String^  PicesSipperVariables::PicesConfigurationDirectory ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesConfigurationDirectory ());
}



String^  PicesSipperVariables::PicesCmdLineProgDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesCmdLineProgDir ());
}




String^  PicesSipperVariables::PicesDataFilesDirectory ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesDataFilesDirectory ());
}



String^  PicesSipperVariables::SipperConfigrationFileName ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::SipperConfigrationFileName ());
}



String^  PicesSipperVariables::PicesDataFileDirectory ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesDataFileDirectory ());
}



String^  PicesSipperVariables::PicesReportDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesReportDir ());
}



String^  PicesSipperVariables::PicesTempDirectory ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::PicesTempDirectory ());
}



String^  PicesSipperVariables::SipperDirectoryListFileName ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::SipperDirectoryListFileName ());
}


String^  PicesSipperVariables::SipperFilesDefaultDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::SipperFilesDefaultDir ());
}


String^  PicesSipperVariables::SipperFilesDirectory ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::SipperFilesDefaultDir ());
}


String^  PicesSipperVariables::SipperInterfaceDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::SipperInterfaceDir ());
}


String^  PicesSipperVariables::SipperInterfaceLogDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::SipperInterfaceLogDir ());
}  /* SipperInterfaceLogDir */


    

String^  PicesSipperVariables::TrainingModelsConfigurationDir ()
{
  return  PicesKKStr::KKStrToSystenStr (SipperVariables::TrainingModelsConfigurationDir ());
}
