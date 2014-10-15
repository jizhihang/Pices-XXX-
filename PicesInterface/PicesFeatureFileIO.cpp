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
#include "FeatureVector.h"
#include "FeatureFileIOPices.h"
#include "FileDesc.h"
#include "MLClass.h"
#include "NormalizationParms.h"

#include "PicesKKStr.h"
#include "PicesRaster.h"
#include "PicesRunLog.h"
#include "PicesFeatureVector.h"
#include "PicesFeatureVectorList.h"


#include "PicesFeatureFileIO.h"


using namespace  PicesInterface;


namespace  PicesInterface
{

  PicesFeatureFileIO::PicesFeatureFileIO (String^  driverName):
      cancelLoad          (NULL),
      changesMade         (NULL),
      driver              (NULL),
      lastError           (nullptr),
      numExamplesWritten  (NULL),
      successful          (NULL)
  {
    if  (String::IsNullOrEmpty (driverName))
      driverName = "Pices";

    driver = FeatureFileIO::FileFormatFromStr (PicesKKStr::SystemStringToKKStr (driverName));
    cancelLoad          = new bool;
    changesMade         = new bool;
    numExamplesWritten  = new uint;
    successful          = new bool;

    *cancelLoad         = false;
    *changesMade        = false;
    *numExamplesWritten = 0;
    *successful = (driver != NULL);
  }



  PicesFeatureFileIO::~PicesFeatureFileIO ()
  {
    this->!PicesFeatureFileIO ();
  }



  PicesFeatureFileIO::!PicesFeatureFileIO ()
  {
    delete  cancelLoad;          cancelLoad         = NULL;
    delete  changesMade;         changesMade        = NULL;
    delete  numExamplesWritten;  numExamplesWritten = NULL;
    delete  successful;          successful         = NULL;
  }



  array<String^>^  PicesFeatureFileIO::RegisteredDriverNames (bool  canRead,
                                                              bool  canWrite
                                                             )
  {
    VectorKKStr  names = FeatureFileIO::RegisteredDriverNames (canRead, canWrite);

    array<String^>^  results = gcnew array<String^> (names.size ());
    
    uint  idx;
    for  (idx = 0;  idx < names.size ();  idx++)
    {
      results->SetValue (PicesKKStr::KKStrToSystenStr (names[idx]), (int)idx);
    }

    return  results;
  }  /* RegisteredDriverNames */ 





  bool  PicesFeatureFileIO::ValidDriverName (String^ _driverName)
  {
    KKStr driverName = PicesKKStr::SystemStringToKKStr (_driverName);

    return  (FeatureFileIO::FileFormatFromStr (driverName) != NULL);
  }




  bool    PicesFeatureFileIO::CancelLoad::get ()  
  {
    if  (cancelLoad)
      return  *cancelLoad;
    else
      return  false;
  }




  void PicesFeatureFileIO::CancelLoad::set (bool _cancelLoad)
  {
    if  (!cancelLoad)
      cancelLoad = new bool;
    *cancelLoad = _cancelLoad;
  }




  bool  PicesFeatureFileIO::CanRead::get ()
  {
    if  (!driver)
      return false;
    return driver->CanRead ();
  }



  bool  PicesFeatureFileIO::CanWrite::get ()
  {
    if  (!driver)
      return false;
    return driver->CanWrite ();
  }



  bool    PicesFeatureFileIO::ChangesMade::get ()
  {
    if  (changesMade)
      return *changesMade;
    else
      return  false;
  }



  String^  PicesFeatureFileIO::DriverName::get ()
  {
    if  (driver)
      return  PicesKKStr::KKStrToSystenStr (driver->DriverName ());
    else
      return String::Empty;
  }


  uint  PicesFeatureFileIO::NumExamplesWritten::get ()
  {
    if  (numExamplesWritten)
      return  *numExamplesWritten;
    else
      return 0;
  }



  bool  PicesFeatureFileIO::Successful::get ()
  {
    if  (successful)
      return  *successful;
    else
      return  false;
  }




  bool  PicesFeatureFileIO::Valid::get ()
  {
    return  (driver != NULL);
  }




  void  PicesFeatureFileIO::SaveFeatureFile (String^                  _fileName, 
                                             PicesFeatureVectorList^  _examples,
                                             PicesRunLog^             _runLog
                                            )
  {
    if  (!driver)
    {
      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile      Invalid Driver");
      *successful = false;
      return;
    }

    if  (!(driver->CanWrite ()))
    {
      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile      Driver["    + 
                        PicesKKStr::KKStrToSystenStr (driver->DriverName ())  +
                        "] does not support reading");
      *successful = false;
      return;
    }

    FeatureVectorListPtr  data = _examples->ToFeatureVectorList (_runLog);
    driver->SaveFeatureFile (PicesKKStr::SystemStringToKKStr (_fileName), 
                             data->AllFeatures (), 
                             *data, 
                             *numExamplesWritten, 
                             *cancelLoad, 
                             *successful, 
                             _runLog->Log ()
                            );

    delete  data;
    data = NULL;
  }


  void  PicesFeatureFileIO::SaveFeatureFileNormalized (String^                  _fileName, 
                                                       PicesFeatureVectorList^  _examples,
                                                       PicesRunLog^             _runLog
                                                      )
  {
    if  (!driver)
    {
      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile      Invalid Driver");
      *successful = false;
      return;
    }

    if  (!(driver->CanWrite ()))
    {
      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile      Driver["    + 
                        PicesKKStr::KKStrToSystenStr (driver->DriverName ())  +
                        "] does not support reading");
      *successful = false;
      return;
    }

    FeatureVectorListPtr  data = _examples->ToFeatureVectorList (_runLog);
    NormalizationParms normParms (true, *data, _runLog->Log ()); 
    normParms.NormalizeImages (data);

    driver->SaveFeatureFile (PicesKKStr::SystemStringToKKStr (_fileName), 
                             data->AllFeatures (), 
                             *data, 
                             *numExamplesWritten, 
                             *cancelLoad, 
                             *successful, 
                             _runLog->Log ()
                            );

    bool  successful = false;
    KKStr normParameterName = osRemoveExtension (PicesKKStr::SystemStringToKKStr (_fileName)) + ".norm";
    normParms.Save (normParameterName, successful);

    delete  data;
    data = NULL;
  }  /* SaveFeatureFileNormalized */




  PicesFeatureVectorList^  PicesFeatureFileIO::LoadFeatureFile (String^          _fileName, 
                                                                PicesClassList^  _classes, 
                                                                PicesRunLog^     _runLog
                                                               )
  {
    if  (!driver)
    {
      lastError = "Invalid Driver";
      _runLog->Writeln ("PicesFeatureFileIO::LoadFeatureFile  " + lastError);
      *successful = false;
      return  nullptr;
    }



    if  (!(driver->CanWrite ()))
    {
      lastError = "Driver[" + PicesKKStr::KKStrToSystenStr (driver->DriverName ()) + "] does not support writting.";

      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile  " + lastError);
      *successful = false;
      return  nullptr;
    }


    MLClassConstListPtr  classes = new MLClassConstList ();
    FeatureVectorListPtr  data = driver->LoadFeatureFile (PicesKKStr::SystemStringToKKStr (_fileName), 
                                                          *classes,
                                                          -1,
                                                          *cancelLoad,
                                                          *successful,
                                                          *changesMade,
                                                          _runLog->Log ()
                                                         );
    if  (!data)
    {
      lastError = "Driver[" + PicesKKStr::KKStrToSystenStr (driver->DriverName ()) + "] LoadFeatureFile did not return any data.";
      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile  " + lastError);
      *successful = false;
      return nullptr;
    }

    if  (!(*successful))
    {
      lastError = "Driver[" + PicesKKStr::KKStrToSystenStr (driver->DriverName ()) + "] LoadFeatureFile Not Successful.";
      _runLog->Writeln ("PicesFeatureFileIO::SaveFeatureFile  " + lastError);
      delete  data;
      return nullptr;
    }

    PicesFeatureVectorList^  results = gcnew PicesFeatureVectorList (*data);
    data->Owner (false);
    delete  data;  data = NULL;

    return  results;
  }

}  /* PicesInterface */
