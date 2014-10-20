#include "StdAfx.h"
#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

#include "MemoryDebug.h"
#include "BasicTypes.h"
#include "..\BaseLibrary\GoalKeeper.h"
using namespace KKU;

#include "PicesKKStr.h"
#include "PicesMethods.h"
#include "PicesSipperFile.h"
#include "PicesDataBaseServer.h"
#include "PicesDataBaseServerPrompter.h"

#include "PicesDataBase.h"

using namespace  PicesInterface;

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Windows::Forms;


namespace  PicesInterface
{
  void  PicesDataBase::Initialization ()
  {
    DataBase::Initialization ();
  }


  void  PicesDataBase::FinalCleanUp ()
  {
    DataBase::FinalCleanUp ();
  }


  bool  PicesDataBase::EmbeddedServerRunning ()
  {
    return  DataBase::EmbeddedServerRunning ();
  }



  PicesDataBase::PicesDataBase (PicesRunLog^  _runLog)
  {
    runLog = _runLog;

    dbConn = new DataBase (runLog->Log ());
    cancelFlag = new bool;
    *cancelFlag = false;
  };



  PicesDataBase::PicesDataBase (PicesDataBaseServer^  _server,
                                PicesRunLog^          _log
                               )
  {
    if  (_server == nullptr)
    {
      String^ startTrace = Environment::StackTrace;
      DataBase::ErrorLogMsg ("PicesDataBase::PicesDataBase    _server == nullptr\n\n" + PicesKKStr::SystemStringToKKStr (startTrace));
      KKU::RunLog  runLog;
      _server = gcnew PicesDataBaseServer (DataBase::GetDefaultServer (runLog));
    }

    if  (_log == nullptr)
    {
      String^ startTrace = Environment::StackTrace;
      DataBase::ErrorLogMsg ("PicesDataBase::PicesDataBase   _log == nullptr\n\n" + PicesKKStr::SystemStringToKKStr (startTrace));
      throw gcnew Exception ("PicesDataBase::PicesDataBase   _log == nullptr");
    }

    runLog = _log;
    dbConn = new DataBase (_server->Server (), runLog->Log ());
    cancelFlag = new bool;
    *cancelFlag = false;
  }



  PicesDataBase::!PicesDataBase ()
  {
    delete  dbConn;
    dbConn = NULL;

    delete  cancelFlag;
    cancelFlag = NULL;
  }



  PicesDataBase::~PicesDataBase ()
  {
    this->!PicesDataBase ();
  }


  void  PicesDataBase::Close ()
  {
    if  (dbConn)
    {
      dbConn->Close ();
      delete dbConn;
      dbConn = NULL;
    }
  }


  bool  PicesDataBase::CancelFlag::get ()
  {
    if  (cancelFlag)
      return *cancelFlag;
    else
      return false;
  }


  void  PicesDataBase::CancelFlag::set (bool _cancelFlag)
  {
    if  (!cancelFlag)
      cancelFlag = new bool;
    *cancelFlag = _cancelFlag;
  }
   


  PicesDataBaseServer^  PicesDataBase::Server::get ()
  {
    if  (dbConn == NULL)
      return nullptr;

    DataBaseServerPtr  server = dbConn->Server ();
    if  (server == NULL)
      return gcnew PicesDataBaseServer ();

    return  gcnew PicesDataBaseServer (server);
  }


  void  PicesDataBase::CloseGlobalDbConn ()
  {
    if  (globaDbConn != nullptr)
    {
      globaDbConn->Close ();
      globaDbConn = nullptr;
      GC::Collect ();
    }
  }



  PicesDataBase^  PicesDataBase::GetGlobalDatabaseManagerNewInstance (PicesRunLog^ _runLog)
  {
    if  (defaultGlobalServer == nullptr)
    {
      RunLog  tempLog;
      DataBaseServerPtr  dbServer = DataBase::GetDefaultServer (tempLog);
      if  (dbServer == NULL)
      {
        dbServer = new DataBaseServer ();
        dbServer->HostName ("LocalHost");
        dbServer->Description ("No Default Server Defined.");
        dbServer->UserName ("Root");
      }

      defaultGlobalServer = gcnew PicesDataBaseServer (dbServer);
      delete  dbServer;
      dbServer = NULL;
    }

    return  gcnew PicesDataBase (defaultGlobalServer, _runLog);
  }  /* GetGlobalDatabaseManagerNewInstance */




  PicesDataBase^  PicesDataBase::SelectNewDataBaseServer (PicesRunLog^  _log)
  {
    while  (true)
    {
      PicesDataBaseServerPrompter^ dataBasePrompter = gcnew PicesDataBaseServerPrompter (_log);
      dataBasePrompter->ShowDialog ();
      PicesDataBaseServer^ newServer = dataBasePrompter->SelectedServer;

      if  (newServer == nullptr)
        return nullptr;

      if  (globaDbConn != nullptr)
        globaDbConn->Close ();
      globaDbConn = nullptr;
      globaDbConn = gcnew PicesDataBase (newServer, _log);
      if  (globaDbConn->Valid ())
        break;
    }
    
    return  globaDbConn;
  }  /* SelectNewDataBaseServer */




  String^  PicesDataBase::ServerDescription ()
  {
    if  (dbConn)
      return  PicesKKStr::KKStrToSystenStr (dbConn->ServerDescription ());
    else
      return  "***  not connected  ***";
  }  /* ServerDescription */




  String^  PicesDataBase::LastErrorDesc ()
  {
    return  PicesKKStr::KKStrToSystenStr (dbConn->LastErrorDesc ());
  }


  bool  PicesDataBase::Valid ()
  {
    if  (!dbConn)
      return  false;
    return  dbConn->Valid ();
  }



  bool  PicesDataBase::ThreadInit ()
  {
    return DataBase::ThreadInit ();
  }



  void  PicesDataBase::ThreadEnd ()
  {
    DataBase::ThreadEnd ();
  }

   
  array<array<String^>^ >^  PicesDataBase::QueryStatement (String^          statement,
                                                           array<String^>^  colsToReturn
                                                          )
  {
    char**  colsToReturnCS = NULL;
    uint    col = 0;
    uint    numCols = 0;

    if  (colsToReturn != nullptr)
    {
      numCols = colsToReturn->Length;
      colsToReturnCS = new char*[numCols + 1];

      for (col = 0;  col < numCols;  col++)
        colsToReturnCS[col] = PicesKKStr::StrToCharStar (colsToReturn[col]);
      colsToReturnCS[numCols] = NULL;
    }

    KKStr  kkStrStatement = PicesKKStr::SystemStringToKKStr (statement);
    KKStrMatrixPtr resultsKKStr = dbConn->QueryStatementKKStr (kkStrStatement, colsToReturnCS);

    if  (colsToReturnCS)
    {
      for  (col = 0;  col < numCols;  col++)
        delete  colsToReturnCS[col];
      delete  colsToReturnCS;
      colsToReturnCS = NULL;
    }

    if  (resultsKKStr == NULL)
      return nullptr;

    int  numRows = resultsKKStr->NumRows ();
    array<array<String^>^ >^ results = gcnew array<array<String^>^ >(numRows);

    for  (int row = 0;  row < numRows;  row++)
    {
      KKStrList&  rowDataKKStr = (*resultsKKStr)[row];
      uint  numCols = rowDataKKStr.QueueSize ();

      array<String^>^  rowData = gcnew array<String^> (numCols);

      for  (col = 0;  col < numCols;  col++)
      {
        rowData[col] = PicesKKStr::KKStrToSystenStr (rowDataKKStr[col]);
      }

      results[row] = rowData;
    }

    delete  resultsKKStr;
    resultsKKStr = NULL;

    return  results;
  }  /* QueryStatement*/




  //***************************************************************************************
  //*                               Feature Data Routines                                 *
  //***************************************************************************************
  void  PicesDataBase::FeatureDataInsertRow (String^              _sipperFileName,
                                             PicesFeatureVector^  _example
                                            )
  {
    dbConn->FeatureDataInsertRow (PicesKKStr::SystemStringToKKStr (_sipperFileName), *(_example->Features ()));
    lastOpSuccessful = dbConn->Valid ();
  }  /* FeatureDataInsertRow*/




  char  ClassKeyToUse (System::Char  classKeyToUse)
  {
    if  ((classKeyToUse == 'V')  ||  (classKeyToUse == 'v'))
      return 'V';
    else
      return 'P';
  }




  PicesFeatureVector^  PicesDataBase::FeatureDataRecLoad (String^  imageFileName)
  {
    KKStr fn = PicesKKStr::SystemStringToKKStr (imageFileName);
    ImageFeaturesPtr  fv = dbConn->FeatureDataRecLoad (fn);
    lastOpSuccessful = dbConn->Valid ();
    if  ((!lastOpSuccessful)  ||  (fv == NULL))
      return nullptr;
    else
      return  gcnew PicesFeatureVector (fv);
  }  /* FeatureDataRecLoad */




  //***************************************************************************************
  //*   Will use ImageFilenName, SipperFileName, and TopLeftRow to load in ImageFeatures  *
  //*  data from 'FeatureData'  and  'InstrumentData' tables.                             *
  //***************************************************************************************
  PicesFeatureVector^  PicesDataBase::FeatureDataRecLoad (PicesDataBaseImage^  image)
  {
    ImageFeaturesPtr  fv = dbConn->FeatureDataRecLoad (image->UnManagedDataBaseImage ());
    lastOpSuccessful = dbConn->Valid ();
    if  ((!lastOpSuccessful)  ||  (fv == NULL))
      return nullptr;
    else
      return  gcnew PicesFeatureVector (fv);
  }



  PicesFeatureVectorList^  PicesDataBase::FeatureDataGetOneSipperFile (String^       sipperFileName,
                                                                       PicesClass^   mlClass,
                                                                       System::Char  classKeyToUse,
                                                                       bool          reExtractInstrumentData
                                                                      )
  {
    MLClassPtr  ic = NULL;
    if  (mlClass != nullptr)
      ic = mlClass->UnmanagedMLClass ();

    ImageFeaturesListPtr  examples = dbConn->FeatureDataGetOneSipperFile (PicesKKStr::SystemStringToKKStr (sipperFileName), 
                                                                          ic, 
                                                                          ClassKeyToUse (classKeyToUse),
                                                                          reExtractInstrumentData,
                                                                          *cancelFlag
                                                                         );
    if  (examples == NULL)
      return nullptr;

    PicesFeatureVectorList^  results = gcnew PicesFeatureVectorList (*examples);
    // "results"  will own the individual instances in "examples" so we need
    // to set the "Owner" property of "examples" to false so taht when it gets
    // deleted it does not delete them.
    examples->Owner (false);  
    delete  examples;  examples = NULL;

    return  results;
  }  /* FeatureDataGetOneSipperFile */




  PicesFeatureVectorList^  PicesDataBase::FeatureDataForImageGroup (PicesDataBaseImageGroup^  imageGroup,
                                                                    PicesClass^               mlClass,
                                                                    System::Char              classKeyToUse
                                                                   )
  {
    if  (imageGroup == nullptr)
      return nullptr;

    MLClassPtr  ic = NULL;
    if  (mlClass != nullptr)
      ic = mlClass->UnmanagedMLClass ();

    ImageFeaturesListPtr  examples = dbConn->FeatureDataForImageGroup (imageGroup->UnManagedClass (), 
                                                                       ic,  
                                                                       ClassKeyToUse (classKeyToUse), 
                                                                       *cancelFlag
                                                                      );
    if  (examples == NULL)
      return nullptr;

    PicesFeatureVectorList^  results = gcnew PicesFeatureVectorList (*examples);
    // "results"  will own the individual instances in "examples" so we need
    // to set the "Owner" property of "examples" to false so taht when it gets
    // deleted it does not delete them.
    examples->Owner (false);  
    delete  examples;  examples = NULL;

    return  results;
  }  /* FeatureDataForImageGroup */




  void   PicesDataBase::FeatureDataUpdate (PicesDataBaseImage^  databaseImage,
                                           PicesFeatureVector^  example
                                          )
  {
    DataBaseImagePtr dbi = NULL;
    if  (databaseImage != nullptr)
      dbi = databaseImage->UnManagedDataBaseImage ();
    dbConn->FeatureDataUpdate (dbi, example->UnManagedClass ());
    lastOpSuccessful = dbConn->Valid ();
  }



  //**********************************************************************************************
  //****************************     PicesDataBaseImage  routines     ****************************
  //**********************************************************************************************

  PicesDataBaseImage^  PicesDataBase::ImageLoad (uint  imageId)
  {
    DataBaseImagePtr image = dbConn->ImageLoad (imageId);
    lastOpSuccessful = (image != NULL);
    if  (image == NULL)
      return nullptr;

    return gcnew PicesDataBaseImage (image);
  }  /* ImageLoad*/



  PicesDataBaseImage^  PicesDataBase::ImageLoad (String^   imageFileName)
  {
    if  (dbConn == NULL)
    {
      
      System::Windows::Forms::MessageBox::Show ("dbConn == NULL", "dbConn == NULL", MessageBoxButtons::OK, MessageBoxIcon::Exclamation); 
      return nullptr;
                                                   
    }

    DataBaseImagePtr image = dbConn->ImageLoad (PicesKKStr::SystemStringToKKStr (imageFileName));
    lastOpSuccessful = (image != NULL);
    if  (image == NULL)
      return nullptr;

    return gcnew PicesDataBaseImage (image);
  }  /* ImageLoad*/



  PicesDataBaseImageList^  PicesDataBase::ImagesQuery (PicesDataBaseImageGroup^  group,
                                                       bool                      includeThumbnail
                                                      )
  {
    DataBaseImageListPtr images = dbConn->ImagesQuery (group->UnManagedClass (), includeThumbnail, *cancelFlag);
    lastOpSuccessful = dbConn->Valid ();
    if  (!images)
      return nullptr;

    PicesDataBaseImageList^  managedImages = gcnew PicesDataBaseImageList (images);
    images->Owner (false);
    delete  images;
    return  managedImages;
  }  /* ImagesQuery */





  PicesDataBaseImageList^  PicesDataBase::ImagesQuery (PicesDataBaseImageGroup^  imageGroup,
                                                       String^                   sipperFileName,
                                                       PicesClass^               mlClass,
                                                       System::Char              classKeyToUse,
                                                       float                     probMin,
                                                       float                     probMax,
                                                       int                       sizeMin,
                                                       int                       sizeMax,
                                                       float                     depthMin,
                                                       float                     depthMax,
                                                       uint                      restartImageId,
                                                       int                       limit,             // Max # of rows to return.  -1 indicates no limit.
                                                       bool                      includeThumbnail
                                                      )
  {
    *cancelFlag = false;

    DataBaseImageGroupPtr  ig = NULL;
    if  (imageGroup != nullptr)
      ig = imageGroup->UnManagedClass ();

    MLClassPtr  unmanagedClass = NULL;
    if  (mlClass != nullptr)
      unmanagedClass = mlClass->UnmanagedMLClass ();

    
    DataBaseImageListPtr  
              images = dbConn->ImagesQuery (ig,
                                            PicesKKStr::SystemStringToKKStr (sipperFileName),
                                            unmanagedClass,  ClassKeyToUse (classKeyToUse),
                                            probMin,   probMax,
                                            sizeMin,   sizeMax,
                                            depthMin,  depthMax,
                                            restartImageId,
                                            limit,
                                            includeThumbnail,
                                            *cancelFlag
                                           );

    lastOpSuccessful = dbConn->Valid ();

    if  (images == NULL)
      return  nullptr;

    if  (*cancelFlag)
    {
      delete  images;
      return  nullptr;
    }

    PicesDataBaseImageList^  picesImages = gcnew PicesDataBaseImageList (images);
    images->Owner (false);
    delete  images;
    images = NULL;
    return  picesImages;
  }  /* ImagesQuery */





  PicesDataBaseImageList^  PicesDataBase::ImagesQuery (PicesDataBaseImageGroup^  imageGroup,
                                                       String^                   cruiseName,
                                                       String^                   stationName,
                                                       String^                   deploymentNum,
                                                       PicesClass^               mlClass,
                                                       System::Char              classKeyToUse,
                                                       float                     probMin,
                                                       float                     probMax,
                                                       int                       sizeMin,
                                                       int                       sizeMax,
                                                       float                     depthMin,
                                                       float                     depthMax,
                                                       uint                      restartImageId,
                                                       int                       limit,             // Max # of rows to return.  -1 indicates no limit.
                                                       bool                      includeThumbnail
                                                      )
  {
    *cancelFlag = false;
    MLClassPtr  unmanagedClass = NULL;
    if  (mlClass != nullptr)
      unmanagedClass = mlClass->UnmanagedMLClass ();

    DataBaseImageGroupPtr  ig = NULL;
    if  (imageGroup != nullptr)
      ig = imageGroup->UnManagedClass ();

    DataBaseImageListPtr  
              images = dbConn->ImagesQuery (ig,
                                            PicesKKStr::SystemStringToKKStr (cruiseName),
                                            PicesKKStr::SystemStringToKKStr (stationName),
                                            PicesKKStr::SystemStringToKKStr (deploymentNum),
                                            unmanagedClass,  ClassKeyToUse (classKeyToUse), 
                                            probMin,   probMax,
                                            sizeMin,   sizeMax,
                                            depthMin,  depthMax,
                                            restartImageId,
                                            limit,
                                            includeThumbnail,
                                            *cancelFlag
                                           );

    lastOpSuccessful = dbConn->Valid ();

    if  (images == NULL)
      return  nullptr;

    if  (*cancelFlag)
    {
      delete  images;
      return  nullptr;
    }

    PicesDataBaseImageList^  picesImages = gcnew PicesDataBaseImageList (images);
    images->Owner (false);
    delete  images;
    images = NULL;
    return  picesImages;
  }  /* ImagesQuery */







  PicesDataBaseImageList^  PicesDataBase::ImagesQueryByGrouop 
                                    (PicesDataBaseImageGroup^   imageGroup,
                                     String^                    cruiseName,
                                     String^                    stationName,
                                     String^                    deploymentNum,
                                     String^                    sipperFileName,
                                     PicesClass^                mlClass,
                                     System::Char               classKeyToUse,
                                     float                      probMin,
                                     float                      probMax,
                                     int                        sizeMin,
                                     int                        sizeMax,
                                     float                      depthMin,
                                     float                      depthMax,
                                     uint                       restartImageId,
                                     int                        limit,            // Max # of rows to return.  -1 indicates no limit.,
                                     bool                       includeThumbnail
                                    )
  {
    if  (imageGroup == nullptr)
      return  nullptr;

    DataBaseImageListPtr  images = dbConn->ImagesQueryByGrouop 
                                (imageGroup->UnManagedClass (),
                                 PicesKKStr::SystemStringToKKStr (cruiseName), 
                                 PicesKKStr::SystemStringToKKStr (stationName),
                                 PicesKKStr::SystemStringToKKStr (deploymentNum),
                                 PicesKKStr::SystemStringToKKStr (sipperFileName),
                                 ((mlClass == nullptr) ? NULL : mlClass->UnmanagedMLClass ()),
                                 ClassKeyToUse (classKeyToUse), 
                                 probMin,  probMax, 
                                 sizeMin,  sizeMax,
                                 depthMin, depthMax,
                                 restartImageId,
                                 limit,
                                 includeThumbnail
                                );
    lastOpSuccessful = dbConn->Valid ();

    if  (images == NULL)
      return  nullptr;

    PicesDataBaseImageList^  picesImages = gcnew PicesDataBaseImageList (images);
    images->Owner (false);
    delete  images;
    images = NULL;
    return  picesImages;
  }  /* ImagesQueryByGrouop */






  PicesClassStatisticList^  PicesDataBase::ImageGetClassStatistics 
                                  (PicesDataBaseImageGroup^  imageGroup,
                                   String^                   sipperFileName,
                                   PicesClass^               mlClass,
                                   System::Char              classKeyToUse,
                                   float                     minProb,
                                   float                     maxProb,
                                   int                       minSize,
                                   int                       maxSize,
                                   float                     minDepth,
                                   float                     maxDepth
                                  )
  {
    DataBaseImageGroupPtr  ig = NULL;
    if  (imageGroup != nullptr)
      ig = imageGroup->UnManagedClass ();

    MLClassPtr  ic = NULL;
    if  (mlClass != nullptr)
      ic = mlClass->UnmanagedMLClass ();

    ClassStatisticListPtr  stats 
         = dbConn->ImageGetClassStatistics (ig,
                                            PicesKKStr::SystemStringToKKStr (sipperFileName),
                                            ic,
                                            ClassKeyToUse (classKeyToUse),
                                            minProb,  maxProb,
                                            minSize,  maxSize,
                                            minDepth, maxDepth
                                           );
    if   (stats == NULL)
      return nullptr;

    PicesClassStatisticList^ results = gcnew PicesClassStatisticList (*stats);
    delete  stats;

    return  results;
  }  /* ImageGetClassStatistics */





  PicesClassStatisticList^  PicesDataBase::ImageGetClassStatistics 
                                 (PicesDataBaseImageGroup^  imageGroup,
                                  String^                   cruiseName,
                                  String^                   stationName,
                                  String^                   deploymentNum,
                                  PicesClass^               mlClass,
                                  System::Char              classKeyToUse,
                                  float                     minProb,
                                  float                     maxProb,
                                  int                       minSize,
                                  int                       maxSize,
                                  float                     minDepth,
                                  float                     maxDepth
                                 )
  {
    DataBaseImageGroupPtr  ig = NULL;
    if  (imageGroup != nullptr)
      ig = imageGroup->UnManagedClass ();

    MLClassPtr  ic = NULL;
    if  (mlClass != nullptr)
      ic = mlClass->UnmanagedMLClass ();

    ClassStatisticListPtr  stats 
         = dbConn->ImageGetClassStatistics (ig,
                                            PicesKKStr::SystemStringToKKStr (cruiseName),
                                            PicesKKStr::SystemStringToKKStr (stationName),
                                            PicesKKStr::SystemStringToKKStr (deploymentNum),
                                            ic,
                                            ClassKeyToUse (classKeyToUse),
                                            minProb,  maxProb,
                                            minSize,  maxSize,
                                            minDepth, maxDepth
                                           );
    if   (stats == NULL)
      return nullptr;

    PicesClassStatisticList^  results = gcnew PicesClassStatisticList (*stats);
    delete  stats;

    return  results;
  }  /* ImageGetClassStatistics */





  array<uint>^  PicesDataBase::ImageGetDepthStatistics (PicesDataBaseImageGroup^  imageGroup,
                                                        String^                   sipperFileName,
                                                        float                     depthIncrements,
                                                        PicesClass^               mlClass,
                                                        System::Char              classKeyToUse,
                                                        float                     probMin,
                                                        float                     probMax,
                                                        int                       sizeMin,
                                                        int                       sizeMax
                                                       )
  {
    DataBaseImageGroupPtr  ig = NULL;
    if  (imageGroup != nullptr)
      ig = imageGroup->UnManagedClass ();

    MLClassPtr  ic = NULL;
    if  (mlClass != nullptr)
      ic = mlClass->UnmanagedMLClass ();

    VectorUint*  depthStats 
      = dbConn->ImageGetDepthStatistics (ig,
                                         PicesKKStr::SystemStringToKKStr (sipperFileName),
                                         depthIncrements,
                                         ic,
                                         ClassKeyToUse (classKeyToUse),
                                         probMin,
                                         probMax,
                                         sizeMin,
                                         sizeMax
                                        );

    if  ((depthStats == NULL)  ||  (depthStats->size () < 1))
    {
      delete  depthStats;  depthStats = NULL;
      return  nullptr;
    }
    
    array<uint>^ stats = gcnew array<uint> (depthStats->size ());

    for  (int x = 0;  x < (int)depthStats->size ();  x++)
      stats->SetValue ((*depthStats)[x], x);

    delete  depthStats;  depthStats = NULL;
    return  stats;
  }  /* ImageGetDepthStatistics */



  array<uint>^  PicesDataBase::ImageGetDepthStatistics (PicesDataBaseImageGroup^  imageGroup,
                                                        String^                   cruiseName,
                                                        String^                   stationName,
                                                        String^                   deploymentNum,
                                                        float                     depthIncrements,
                                                        PicesClass^               mlClass,
                                                        System::Char              classKeyToUse,
                                                        float                     probMin,
                                                        float                     probMax,
                                                        int                       sizeMin,
                                                        int                       sizeMax
                                                       )
  {
    DataBaseImageGroupPtr  ig = NULL;
    if  (imageGroup != nullptr)
      ig = imageGroup->UnManagedClass ();

    MLClassPtr  ic = NULL;
    if  (mlClass != nullptr)
      ic = mlClass->UnmanagedMLClass ();

    VectorUint*  depthStats 
      = dbConn->ImageGetDepthStatistics (ig,
                                         PicesKKStr::SystemStringToKKStr (cruiseName),
                                         PicesKKStr::SystemStringToKKStr (stationName),
                                         PicesKKStr::SystemStringToKKStr (deploymentNum),
                                         depthIncrements,
                                         ic,
                                         ClassKeyToUse (classKeyToUse),
                                         probMin,
                                         probMax,
                                         sizeMin,
                                         sizeMax,
                                         *cancelFlag
                                        );

    if  ((depthStats == NULL)  ||  (depthStats->size () < 1))
    {
      delete  depthStats;  depthStats = NULL;
      return  nullptr;
    }
    
    array<uint>^ stats = gcnew array<uint> (depthStats->size ());

    for  (int x = 0;  x < (int)depthStats->size ();  x++)
      stats->SetValue ((*depthStats)[x], x);

    delete  depthStats;  depthStats = NULL;
    return  stats;
  }   /* ImageGetDepthStatistics */





  void  PicesDataBase::ImageUpdateInstrumentDataFields (PicesInstrumentData^  instumentData,
                                                        String^               sipperFileName,
                                                        uint                  scanLineStart,
                                                        uint                  scanLineEnd
                                                       )
  {
    dbConn->ImageUpdateInstrumentDataFields (instumentData->UnmanagedClass (),
                                             PicesKKStr::SystemStringToKKStr (sipperFileName),
                                             scanLineStart,
                                             scanLineEnd
                                            );
  }  /* ImageUpdateInstrumentDataFields */








  void  PicesDataBase::ImagesUpdatePredictions (String^      imageFileName,
                                                PicesClass^  class1Pred,
                                                float        class1Prob,
                                                PicesClass^  class2Pred,
                                                float        class2Prob,
                                                uint         logEntryId
                                               )
  {
    if  (class1Pred == nullptr)
      class1Pred = PicesClassList::GetUnKnownClassStatic ();

    if  (class2Pred == nullptr)
      class2Pred = PicesClassList::GetUnKnownClassStatic ();

    dbConn->ImagesUpdatePredictions (PicesKKStr::SystemStringToKKStr (imageFileName),
                                     class1Pred->UnmanagedMLClass (),
                                     class1Prob,
                                     class2Pred->UnmanagedMLClass (),
                                     class2Prob,
                                     logEntryId
                                    );
    lastOpSuccessful = dbConn->Valid ();
  }  /* ImagesUpdatePredictions*/



  void  PicesDataBase::ImagesUpdatePredictionsList (uint     _logEntryId,
                                                    String^  _predictionList
                                                   )
  {
    dbConn->ImagesUpdatePredictionsList (_logEntryId, PicesKKStr::SystemStringToKKStr (_predictionList));
    lastOpSuccessful = dbConn->Valid ();
  }



  void  PicesDataBase::ImagesUpdateValidatedClass (String^      imageFileName, 
                                                   PicesClass^  mlClass
                                                  )
  {
    dbConn->ImagesUpdateValidatedClass (PicesKKStr::SystemStringToKKStr (imageFileName), 
                                        mlClass->UnmanagedMLClass ()
                                       );
    lastOpSuccessful = dbConn->Valid ();
  }



  void  PicesDataBase::ImagesUpdateImageSize (String^          imageFileName,
                                              PicesPointList^  sizeCoordinates, 
                                              float            imageSize
                                             )
  {
    KKU::PointListPtr  unmangedPoints = sizeCoordinates->ToUnmanaged ();
    dbConn->ImagesUpdateImageSize (PicesKKStr::SystemStringToKKStr (imageFileName), unmangedPoints, imageSize);
    lastOpSuccessful = dbConn->Valid ();
    delete  unmangedPoints;
    unmangedPoints = NULL;
  }



  void  PicesDataBase::ImageRemoveValidation (String^   imageFileName)
  {
    dbConn->ImageRemoveValidation (PicesKKStr::SystemStringToKKStr (imageFileName));
    lastOpSuccessful = dbConn->Valid ();
  }


  void  PicesDataBase::ImagesDeleteOneImage (String^   imageFileName)
  {
    dbConn->ImagesDeleteOneImage (PicesKKStr::SystemStringToKKStr (imageFileName));
    lastOpSuccessful = dbConn->Valid ();
  }



  void  PicesDataBase::ImagesUpdateValidatedAndPredictClass (String^     imageFileName, 
                                                             PicesClass^ mlClass, 
                                                             float       class1Prob
                                                            )
  {
    if  (mlClass == nullptr)
      mlClass = PicesClassList::GetUnKnownClassStatic ();

    dbConn->ImagesUpdateValidatedAndPredictClass (PicesKKStr::SystemStringToKKStr (imageFileName),
                                                  mlClass->UnmanagedMLClass (),
                                                  class1Prob
                                                 );
    lastOpSuccessful = dbConn->Valid ();
  }  /* ImagesUpdateValidatedAndPredictClass */




  //***************************************************************************************
  //*                           ImagesFullSize  Routines                                  *
  //***************************************************************************************
  PicesRaster^  PicesDataBase::ImageFullSizeLoad (String^  imageFileName)
  {
    RasterSipperPtr  r = dbConn->ImageFullSizeLoad (PicesKKStr::SystemStringToKKStr (imageFileName));
    if  (!r)
      return  nullptr;
    else
      return  gcnew PicesRaster (r);
  }




  void  PicesDataBase::ImageFullSizeSave (String^       imageFileName,
                                          PicesRaster^  raster
                                         )
  {
    dbConn->ImageFullSizeSave (PicesKKStr::SystemStringToKKStr (imageFileName), *(raster->UnmanagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }  /* ImageFullSizeSave */





  PicesRaster^  PicesDataBase::ImageFullSizeFind (String^  imageFileName)
  {
    RasterSipperPtr  r = NULL;

    try  {r = dbConn->ImageFullSizeFind (PicesKKStr::SystemStringToKKStr (imageFileName));}
    catch  (KKStrException&  e1)  {throw gcnew Exception ("PicesDataBase::ImageFullSizeFind KKStrException\n" +  PicesKKStr::KKStrToSystenStr (e1.ToString ()));}
    catch  (exception&       e2)  {throw gcnew Exception ("PicesDataBase::ImageFullSizeFind std::exception\n" +  PicesKKStr::KKStrToSystenStr (e2.what ()));}
    catch  (...)                  {throw gcnew Exception ("PicesDataBase::ImageFullSizeFind Exception ocured");}
    if  (!r)
      return nullptr;
    return  gcnew PicesRaster (r);
  }



  //***************************************************************************************
  //*                             MLClass  Routines                                    *
  //***************************************************************************************

  PicesClass^  PicesDataBase::MLClassLoad (String^  className)  /**< Load instance on PicesClass with name = "ClassName"  
                                                                    * if not in database will return nullptr
                                                                    */
  {
    MLClassConstPtr  mlClass = dbConn->MLClassLoad (PicesKKStr::SystemStringToKKStr (className));
    if  (mlClass == NULL)
      return  nullptr;
    else
    {
      PicesClass^ result = PicesClassList::GetUniqueClass (mlClass);
      result->Description = PicesKKStr::KKStrToSystenStr (mlClass->Description ());

      if  (mlClass->Parent () == NULL)
      {
        if  (mlClass->Name ().EqualIgnoreCase ("ALLCLASSES"))
        {
          // This class is the root node;  its parent should be pointing to 'null'.
          int  zed = 7777;
        }
        else
        {
          result->Parent = PicesClassList::GetUniqueClass ("AllClasses", "");
        }
      }
      else
      {
        result->Parent = PicesClassList::GetUniqueClass (mlClass->Parent ());
      }

      return  result;
    }
  }  /* MLClassLoad */





  PicesClassList^  PicesDataBase::MLClassLoadList ()
  {
    MLClassConstListPtr  classes = dbConn->MLClassLoadList ();
    if  (!classes)
      return  nullptr;

    PicesClass^ rootPicesClass = nullptr;

    PicesClassList^  picesClasses = gcnew PicesClassList ();

    MLClassConstList::iterator  idx;
    for  (idx = classes->begin ();  idx != classes->end ();  idx++)
    {
      MLClassConstPtr  ic = *idx;

      PicesClass^ c = PicesClassList::GetUniqueClass (PicesKKStr::KKStrToSystenStr (ic->Name ()), nullptr);
      picesClasses->Add (c);
      if  (ic->Name ().EqualIgnoreCase ("ALLCLASSES"))
      {
        rootPicesClass = c;
        picesClasses->RootNode = c;
      }
    }

    if  (rootPicesClass == nullptr)
    {
      // We never loaded the 'ALLCLASSES' root class.
      rootPicesClass = PicesClassList::GetUniqueClass ("AllClasses", nullptr);
      picesClasses->Add (rootPicesClass);
    }

    for  (idx = classes->begin ();  idx != classes->end ();  idx++)
    {
      MLClassConstPtr  ic = *idx;

      PicesClass^ pc = picesClasses->LookUpByUnmanagedClass (ic);

      if  (pc == nullptr)
      {
        // This should not have been able to have happened;  but it did;  so lets have a breal point here
        // there must be something wrong with my program logic.
        runLog->Log ().Level (-1) << endl << endl 
                                  << "PicesDataBase::MLClassLoadList   ***ERROR***" << endl
                                  << "                                    The Parent Child Relationship is Broken" << endl
                                  << "                                    ClassName[" << ic->Name () << "]" << endl
                                  << endl;
      }
      else if  (ic->Parent () == NULL)
      {
        if  (pc != rootPicesClass)
        {
          pc->Parent = rootPicesClass;
          rootPicesClass->AddAChild (pc);
        }
      }
      else
      {
        PicesClass^  parent = picesClasses->LookUpByUnmanagedClass (ic->Parent ());
        if  (parent == nullptr)
        {
          pc->Parent = rootPicesClass;
          rootPicesClass->AddAChild (pc);
        }
        else
        {
          pc->Parent = parent;
          parent->AddAChild (pc);
        }
      }
    }

    picesClasses->RootNode = rootPicesClass;

    delete  classes;
    classes = NULL;

    return  picesClasses;
  }  /* MLClassLoadList */




  void  PicesDataBase::MLClassDelete (String^  className)
  {
    dbConn->MLClassDelete (PicesKKStr::SystemStringToKKStr (className));
  }  /* MLClassDelete */




  void  PicesDataBase::MLClassUpdate (String^      oldClassName,
                                         PicesClass^  picesClass
                                        )

  {
    bool b = false; 

    KKStr  oldClassNameKKStr;

    if  (String::IsNullOrEmpty (oldClassName))
      oldClassNameKKStr = PicesKKStr::SystemStringToKKStr (picesClass->Name);
    else
      oldClassNameKKStr = PicesKKStr::SystemStringToKKStr (oldClassName);


    dbConn->MLClassUpdate (oldClassNameKKStr, *(picesClass->UnmanagedMLClass ()), b);
    lastOpSuccessful = b;
    return;
  }



  void  PicesDataBase::MLClassInsert (PicesClass^  picesClass)
  {
    bool b = false; 
    dbConn->MLClassInsert (*(picesClass->UnmanagedMLClass ()), b);
    lastOpSuccessful = b;
  }



  void  PicesDataBase::MLClassInsertReturn (PicesClass^  mlClass,
                                               bool%        successful
                                              )
  {
    MLClassPtr  ic = mlClass->UnmanagedMLClass ();

    bool  mlClassUpdated = false;
    dbConn->MLClassInsertReturn (*ic, mlClassUpdated);
    successful = mlClassUpdated;
  }



  //*************************************************************************************************
  //*                                ImageGroup Routines.                                           *
  //*************************************************************************************************
  PicesDataBaseImageGroup^  PicesDataBase::ImageGroupLoad (String^  name)
  {
    DataBaseImageGroupPtr group = dbConn->ImageGroupLoad (PicesKKStr::SystemStringToKKStr (name));
    if  (!group)
      return nullptr;
    return  gcnew PicesDataBaseImageGroup (group);
  }  /* ImageGroupQueryByName */


  void  PicesDataBase::ImageGroupInsert (PicesDataBaseImageGroup^  imageGroup)
  {
    dbConn->ImageGroupInsert (*(imageGroup->UnManagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }



  PicesDataBaseImageGroupList^  PicesDataBase::ImageGroupLoad ()
  {
    DataBaseImageGroupListPtr  imageGroups = dbConn->ImageGroupLoad ();
    if  (!imageGroups)
    {
      lastOpSuccessful = false;
      return  nullptr;
    }

    PicesDataBaseImageGroupList^ managedGroupsList = gcnew PicesDataBaseImageGroupList (imageGroups);
    imageGroups->Owner (false);  // 'managedGroupsList' now owns the contents of 'imageGroups'.
    delete  imageGroups;
    imageGroups = NULL;
    return  managedGroupsList;
  }



  void  PicesDataBase::ImageGroupDelete (int imageGroupId)
  {
    dbConn->ImageGroupDelete (imageGroupId);
    lastOpSuccessful = dbConn->Valid ();
  }





  //*************************************************************************************************
  //*                             ImageGroupEntries Routines.                                       *
  //*************************************************************************************************
  void  PicesDataBase::ImageGroupEntriesInsert (PicesDataBaseImageGroupEntry^  imageGroupEntry)
  {
    dbConn->ImageGroupEntriesInsert (*(imageGroupEntry->UnManagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }



  void  PicesDataBase::ImageGroupEntriesInsert (PicesDataBaseImageGroupEntryList^  imageGroupList)
  {
    DataBaseImageGroupEntryListPtr  unmagedList = new DataBaseImageGroupEntryList (false); // will not own contents
    for each (PicesDataBaseImageGroupEntry^ entry in imageGroupList)
    {
      unmagedList->PushOnBack (entry->UnManagedClass ());
    }
    dbConn->ImageGroupEntriesInsert (*unmagedList);
    lastOpSuccessful = dbConn->Valid ();

    delete  unmagedList;
    unmagedList = NULL;
  }  /* ImageGroupEntriesInsert */



  array<array<String^>^ >^  PicesDataBase::ImageGroupEntriesInsert (int              groupId,
                                                                    List<String^>^   imageFileNames
                                                                   )
  {
    VectorKKStr  names;
    for each  (String^ name  in imageFileNames)
      names.push_back (PicesKKStr::SystemStringToKKStr (name));

    KKStrMatrixPtr  resultsKKStr = dbConn->ImageGroupEntriesInsert (groupId, names);
    if  (resultsKKStr == NULL)
      return  nullptr;

    int  numRows = resultsKKStr->NumRows ();
    array<array<String^>^ >^ results = gcnew array<array<String^>^ >(numRows);

    for  (int row = 0;  row < numRows;  row++)
    {
      KKStrList&  rowDataKKStr = (*resultsKKStr)[row];
      uint  numCols = rowDataKKStr.QueueSize ();

      array<String^>^  rowData = gcnew array<String^> (numCols);

      for  (uint col = 0;  col < numCols;  col++)
      {
        rowData[col] = PicesKKStr::KKStrToSystenStr (rowDataKKStr[col]);
      }

      results[row] = rowData;
    }

    lastOpSuccessful = dbConn->Valid ();

    delete  resultsKKStr;  resultsKKStr = NULL;

    return  results;
  }  /* ImageGroupEntriesInsert */




  array<array<String^>^ >^  PicesDataBase::ImageGroupEntriesInsert (int                      groupId, 
                                                                    PicesDataBaseImageList^  images
                                                                   )
  {
    List<String^>^   imageFileNames = gcnew List<String^> ();
    for each  (PicesDataBaseImage^ image  in images)
      imageFileNames->Add (image->ImageFileName);
    return ImageGroupEntriesInsert (groupId, imageFileNames);
  }  /* ImageGroupEntriesInsert */




  PicesDataBaseImageGroupEntryList^  PicesDataBase::ImageGroupEntriesLoad (int groupId)
  {
    DataBaseImageGroupEntryListPtr  entries = dbConn->ImageGroupEntriesLoad (groupId);
    if  (!entries)
    {
      lastOpSuccessful = false;
      return  nullptr;
    }

    PicesDataBaseImageGroupEntryList^  managedEntries = gcnew PicesDataBaseImageGroupEntryList (entries);
    entries->Owner (false);  // 'managedEntries' now own the contents of 'entries'
    delete  entries;
    entries = NULL;

    return  managedEntries;
  }  /* ImageGroupEntriesLoad */








  //*****************************************************************************************************
  //*                                  InstrumentData Routines.                                         *
  //*****************************************************************************************************
  List<String^>^  PicesDataBase::InstrumentDataGetFieldNames ()  // Get the names of individule field names;  used by 
  {                                                              // applications that will query InstrumentData table 
    List<String^>^ results = gcnew List<String^> ();             // allowing users to select fields to query and display.

    VectorKKStr names = dbConn->InstrumentDataGetFieldNames ();
    VectorKKStr::const_iterator  idx;
    for  (idx = names.begin ();  idx != names.end ();  idx++)
      results->Add (PicesKKStr::KKStrToSystenStr (*idx));

    return  results;
  }  /* InstrumentDataGetFieldNames */




  PicesInstrumentData^  PicesDataBase::InstrumentDataGetByScanLine (String^  sipperFileName,   // Will retrieve the Instrument data that is 
                                                                    uint     scanLine          // closest to 'ScanLine'
                                                                   )
  {
    InstrumentDataPtr  id = dbConn->InstrumentDataGetByScanLine (PicesKKStr::SystemStringToKKStr (sipperFileName), scanLine);
    if  (id == NULL)
      return  nullptr;

    return   gcnew PicesInstrumentData (id);
  }  /* InstrumentDataGetByScanLine */





  PicesInstrumentDataList^  PicesDataBase::InstrumentDataLoad (String^ sipperFileName)
  {
    *cancelFlag = false;

    InstrumentDataListPtr  data = dbConn->InstrumentDataLoad (PicesKKStr::SystemStringToKKStr (sipperFileName), *cancelFlag);
    if  (!data)
      return  nullptr;

    PicesInstrumentDataList^ picesInstrumentData = gcnew PicesInstrumentDataList (data);
    data->Owner (false);
    delete  data;
    data = NULL;

    return  picesInstrumentData;
  }  /* InstrumentDataLoad */






  PicesInstrumentDataList^  PicesDataBase::InstrumentDataLoad (String^  cruiseName,
                                                               String^  stationName,
                                                               String^  deploymentNum
                                                              )
  {
    *cancelFlag = false;

    InstrumentDataListPtr  data = dbConn->InstrumentDataLoad (PicesKKStr::SystemStringToKKStr (cruiseName),
                                                              PicesKKStr::SystemStringToKKStr (stationName),
                                                              PicesKKStr::SystemStringToKKStr (deploymentNum),
                                                              *cancelFlag
                                                             );
    if  (!data)
      return  nullptr;

    PicesInstrumentDataList^  picesInstrumentData = gcnew PicesInstrumentDataList (data);
    data->Owner (false);
    delete  data;
    data = NULL;

    return  picesInstrumentData;
  }  /* InstrumentDataLoad */



  PicesVolumeSampledStatList^  PicesDataBase::InstrumentDataGetVolumePerMeterDepth (String^  siperFileName,
                                                                                    float    depthBinSize
                                                                                   )
  {
    PicesVolumeSampledStatList^  results = nullptr;

    VolumeSampledStatListPtr  
      stats = dbConn->InstrumentDataGetVolumePerMeterDepth (PicesKKStr::SystemStringToKKStr (siperFileName), depthBinSize);
    if  (stats)
    {
      results = gcnew PicesVolumeSampledStatList (*stats);
      delete  stats;
      stats = NULL;
    }
    return  results;
  }


  PicesVolumeSampledStatList^  PicesDataBase::InstrumentDataGetVolumePerMeterDepth (String^  cruiseName,
                                                                                    String^  stationName,
                                                                                    String^  deploymentNum,
                                                                                    float    depthBinSize
                                                                                   )
  {
    PicesVolumeSampledStatList^  results = nullptr;

    VolumeSampledStatListPtr  
      stats = dbConn->InstrumentDataGetVolumePerMeterDepth (PicesKKStr::SystemStringToKKStr (cruiseName), 
                                                            PicesKKStr::SystemStringToKKStr (stationName), 
                                                            PicesKKStr::SystemStringToKKStr (deploymentNum), 
                                                            depthBinSize,
                                                            *cancelFlag
                                                           );
    if  (stats)
    {
      results = gcnew PicesVolumeSampledStatList (*stats);
      delete  stats;
      stats = NULL;
    }
    return  results;
  }



  void  PicesDataBase::InstrumentDataSaveListForOneSipperFile (String^                   sipperFileName, 
                                                               PicesInstrumentDataList^  data
                                                              )
  {
    InstrumentDataListPtr  instData = new InstrumentDataList (false);

    for each (PicesInstrumentData^  id in data)
      instData->PushOnBack (id->UnmanagedClass ());

    (*cancelFlag) = false;

    KKStr  sfn = PicesKKStr::SystemStringToKKStr (sipperFileName);
    dbConn->InstrumentDataSaveListForOneSipperFile (sfn, *instData, *cancelFlag);

    delete  instData;
    instData = NULL;
  }  /* InstrumentDataSaveListForOneSipperFile */





  List<ulong>^  PicesDataBase::GetScanLinesPerMeterProfile (String^           sipperFileName,
                                                            PicesSipperFile^  sipperFile
                                                           )
  {
    VectorUlong  scanLinesPerMeter =  InstrumentDataFileManager::GetScanLinesPerMeterProfile 
                              (PicesKKStr::SystemStringToKKStr (sipperFileName), 
                               sipperFile->UnmanagedClass (),
                               *cancelFlag,
                               runLog->Log ()
                              );

    List<ulong>^ results = gcnew List<ulong> ();

    VectorUlong::iterator  idx;
    for  (idx = scanLinesPerMeter.begin ();  idx !=  scanLinesPerMeter.end ();  idx++)
      results->Add (*idx);

    return  results;
  }  /* GetScanLinesPerMeterProfile */



  List<float>^  PicesDataBase::GetVolumePerMeterProfile (String^           sipperFileName,
                                                         PicesSipperFile^  sipperFile
                                                        )
  {
    *cancelFlag = false;
    VectorFloat  volumePerMeter =  InstrumentDataFileManager::GetVolumePerMeterProfile 
                              (PicesKKStr::SystemStringToKKStr (sipperFileName), 
                               sipperFile->UnmanagedClass (),
                               *cancelFlag,
                               runLog->Log ()
                              );

    List<float>^ results = gcnew List<float> ();

    VectorFloat::iterator  idx;
    for  (idx = volumePerMeter.begin ();  idx !=  volumePerMeter.end ();  idx++)
      results->Add (*idx);

    return  results;
  }  /* GetVolumePerMeterProfile */



  array<float, 2>^  PicesDataBase::InstrumentDataGetSpecificFields (String^           sipperFileName,
                                                                    List<String^>^    fieldNames,
                                                                    System::DateTime  dateTimeStart,
                                                                    System::DateTime  dateTimeEnd
                                                                   )
  {
    KKStrListPtr  fieldNamesList = PicesKKStr::SystemStringListToKKStrListPtr (fieldNames);

    vector<vector<float> >*  unManagedResults
      = dbConn->InstrumentDataGetSpecificFields (PicesKKStr::SystemStringToKKStr (sipperFileName), 
                                                 fieldNamesList, 
                                                 PicesMethods::DateTimeSystemToKKU (dateTimeStart), 
                                                 PicesMethods::DateTimeSystemToKKU (dateTimeEnd)
                                                );
    delete  fieldNamesList;
    fieldNamesList = NULL;

    if  (unManagedResults == NULL)
      return nullptr;

    int numRows = unManagedResults->size ();
    int numCols = fieldNames->Count;

    array<float, 2>^  results = gcnew array<float, 2> (numRows, numCols);
    for  (int row = 0;  row < numRows;  ++row)
    {
      for  (int col = 0;  col < numCols;  ++col)
        results[row, col] =  (*unManagedResults)[row][col];
    }

    delete  unManagedResults;
    unManagedResults = NULL;

    return  results;
  }



  array<float, 2>^    PicesDataBase::InstrumentDataGetSpecificFields (String^           cruiseName,
                                                                      String^           stationName,
                                                                      String^           deploymentNum,
                                                                      List<String^>^    fieldNames,
                                                                      System::DateTime  dateTimeStart,
                                                                      System::DateTime  dateTimeEnd
                                                                     )
  {
    KKStrListPtr  fieldNamesList = PicesKKStr::SystemStringListToKKStrListPtr (fieldNames);

    vector<vector<float> >*  unManagedResults
      = dbConn->InstrumentDataGetSpecificFields (PicesKKStr::SystemStringToKKStr (cruiseName), 
                                                 PicesKKStr::SystemStringToKKStr (stationName), 
                                                 PicesKKStr::SystemStringToKKStr (deploymentNum), 
                                                 fieldNamesList, 
                                                 PicesMethods::DateTimeSystemToKKU (dateTimeStart), 
                                                 PicesMethods::DateTimeSystemToKKU (dateTimeEnd)
                                                );
    delete  fieldNamesList;
    fieldNamesList = NULL;
    if  (unManagedResults == NULL)
      return nullptr;

    int numRows = unManagedResults->size ();
    int numCols = fieldNames->Count;

    array<float, 2>^  results = gcnew array<float, 2> (numRows, numCols);
    for  (int row = 0;  row < numRows;  ++row)
    {
      for  (int col = 0;  col < numCols;  ++col)
        results[row, col] =  (*unManagedResults)[row][col];
    }

    delete  unManagedResults;
    unManagedResults = NULL;

    return  results;
  }




  PicesGPSDataPointList^  PicesDataBase::InstrumentDataRetrieveGPSInfo (String^  cruiseName,
                                                                        String^  stationName,
                                                                        String^  deploymentNum,
                                                                        int32    timeInterval   /**< Interval in seconds */
                                                                       )
  {
    PicesGPSDataPointList^  results = nullptr;

    GPSDataPointListPtr   
      stats = dbConn->InstrumentDataRetrieveGPSInfo (PicesKKStr::SystemStringToKKStr (cruiseName),
                                                     PicesKKStr::SystemStringToKKStr (stationName),
                                                     PicesKKStr::SystemStringToKKStr (deploymentNum),
                                                     timeInterval
                                                    );
    if  (!stats)
    {
      results =  nullptr;
    }
    else
    {
      results = gcnew PicesGPSDataPointList (*stats);
      delete stats;
      stats = NULL;
    }

    return results;
  }




  //***********************************************************************************
  PicesDataBaseLogEntry^  PicesDataBase::LogEntriesProcessStart (String^           progCode,
                                                                 String^           progName,
                                                                 System::DateTime  dateTimeCompiled,
                                                                 String^           cmdLine,
                                                                 String^           sipperFileName
                                                                )
  {
    DataBaseLogEntryPtr  dataBaseLogEntry 
      = dbConn->LogEntriesProcessStart (PicesKKStr::SystemStringToKKStr   (progCode),
                                        PicesKKStr::SystemStringToKKStr   (progName),
                                        PicesMethods::DateTimeSystemToKKU (dateTimeCompiled),
                                        PicesKKStr::SystemStringToKKStr   (cmdLine),
                                        PicesKKStr::SystemStringToKKStr   (sipperFileName)
                                       );

    return  gcnew PicesDataBaseLogEntry (dataBaseLogEntry);
  }



  void  PicesDataBase::LogEntriesProcessEnd (PicesDataBaseLogEntry^  logEntry,
                                             float                   cpuTimeUsed,
                                             String^                 completionResult
                                            )
  {
    dbConn->LogEntriesProcessEnd (logEntry->UnmanagedClass (), cpuTimeUsed, PicesKKStr::SystemStringToKKStr (completionResult));
  }



  PicesDataBaseLogEntry^  PicesDataBase::LogEntriesSelect (uint _logEntryId)
  {
    DataBaseLogEntryPtr  logEntry = dbConn->LogEntriesSelect (_logEntryId);
    if  (logEntry == NULL)
      return nullptr;
    else
      return gcnew PicesDataBaseLogEntry (logEntry);
  }


  //****************************************************************************************************
  //*                                  SipperCruise  routines.                                         *
  //****************************************************************************************************

  void  PicesDataBase::SipperCruiseDelete (String^  cruiseName)
  {
    dbConn->SipperCruiseDelete (PicesKKStr::SystemStringToKKStr (cruiseName));
    lastOpSuccessful = dbConn->Valid ();
  }



  void  PicesDataBase::SipperCruiseInsert (PicesSipperCruise^  cruise)
  {
    bool  s;
    dbConn->SipperCruiseInsert (cruise->UnmanagedSipperCruise (), s);
    lastOpSuccessful = s;
    return;
  }  /* SipperCruiseInsert */




  PicesSipperCruise^  PicesDataBase::SipperCruiseLoad (String^  cruiseName)
  {
    SipperCruisePtr  cruise = dbConn->SipperCruiseLoad (PicesKKStr::SystemStringToKKStr (cruiseName));
    if  (!cruise)
      return  nullptr;

    return  gcnew PicesSipperCruise (cruise);
  }




  PicesSipperCruiseList^  PicesDataBase::SipperCruiseLoadAllCruises ()
  {
    SipperCruiseListPtr  sipperCruises = dbConn->SipperCruiseLoadAllCruises ();

    if  (sipperCruises == NULL)
      return  nullptr;

    PicesSipperCruiseList^  results = gcnew PicesSipperCruiseList (sipperCruises);
    sipperCruises->Owner (false);  // 'result' now own the contents of 'sipperCruises'.
    delete  sipperCruises;
    sipperCruises = NULL;

    return  results;
  }



  void  PicesDataBase::SipperCruiseUpdate (PicesSipperCruise^  cruise)
  {
    bool s = false;
    dbConn->SipperCruiseUpdate (cruise->UnmanagedSipperCruise (), s);
    lastOpSuccessful = s;
    return;
  }  /* SipperCruiseUpdate */





  //****************************************************************************************************
  //*                                SipperDeployment  routines.                                       *
  //****************************************************************************************************
  PicesSipperDeploymentList^  PicesDataBase::SipperDeploymentLoad (String^  cruiseName,
                                                                   String^  stationName
                                                                  )
  {
    SipperDeploymentListPtr  deployments = dbConn->SipperDeploymentLoad (PicesKKStr::SystemStringToKKStr (cruiseName),
                                                                         PicesKKStr::SystemStringToKKStr (stationName) 
                                                                        );
    if  (deployments == NULL)
      return  nullptr;
    
    PicesSipperDeploymentList^  results = gcnew PicesSipperDeploymentList (deployments);
    deployments->Owner (false);
    delete  deployments;
    deployments = NULL;
    return  results;
  }  /* SipperDeploymentLoad */



  PicesSipperDeployment^  PicesDataBase::SipperDeploymentLoad (String^  cruiseName,
                                                               String^  stationName,
                                                               String^  deploymentNum
                                                              )
  {
    SipperDeploymentPtr  d 
      = dbConn->SipperDeploymentLoad 
                            (PicesKKStr::SystemStringToKKStr (cruiseName), 
                             PicesKKStr::SystemStringToKKStr (stationName), 
                             PicesKKStr::SystemStringToKKStr (deploymentNum)
                            );
    if  (!d)
      return nullptr;

    return gcnew PicesSipperDeployment (d);
  }

    


  void   PicesDataBase::SipperDeploymentInsert (PicesSipperDeployment^  deployment)
  {
    dbConn->SipperDeploymentInsert (*(deployment->UnManagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }
   


  void   PicesDataBase::SipperDeploymentUpdate (PicesSipperDeployment^  deployment)
  {
    dbConn->SipperDeploymentUpdate (*(deployment->UnManagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }



   
  void   PicesDataBase::SipperDeploymentDelete (String^  cruiseName,
                                                String^  stationName,
                                                String^  deploymentNum
                                               )
  {
    dbConn->SipperDeploymentDelete (PicesKKStr::SystemStringToKKStr (cruiseName), 
                                    PicesKKStr::SystemStringToKKStr (stationName), 
                                    PicesKKStr::SystemStringToKKStr (deploymentNum)
                                   );
    lastOpSuccessful = dbConn->Valid ();
  }




  //****************************************************************************************************
  //*                                   SipperFile  routines.                                          *
  //****************************************************************************************************


  array<String^>^  PicesDataBase::SipperFileGetList (String^ cruiseName,
                                                     String^ stationName,
                                                     String^ deploymentNum
                                                    )
  {
    VectorKKStr*  sipperFileNames = dbConn->SipperFileGetList 
         (PicesKKStr::SystemStringToKKStr (cruiseName),
          PicesKKStr::SystemStringToKKStr (stationName),
          PicesKKStr::SystemStringToKKStr (deploymentNum)
         );
       
    if  (!sipperFileNames)
      return nullptr;

    array<String^>^ sipperFileNamesManaged = gcnew array<String^>(sipperFileNames->size ());

    VectorKKStr::iterator  idx;
    int                    idxMan = 0;
    for  (idx = sipperFileNames->begin ();  idx != sipperFileNames->end ();  idx++)
    {
      sipperFileNamesManaged->SetValue (PicesKKStr::KKStrToSystenStr (*idx), idxMan);
      idxMan++;
    }

    delete  sipperFileNames;  sipperFileNames = NULL;
    return  sipperFileNamesManaged;
  }  /* SipperFileGetList */





  PicesSipperFileList^  PicesDataBase::SipperFileLoad (String^  cruiseName,
                                                       String^  stationName,
                                                       String^  deploymentNum
                                                      )
  {
    SipperFileListPtr sipperFiles = dbConn->SipperFileLoad (PicesKKStr::SystemStringToKKStr (cruiseName),
                                                            PicesKKStr::SystemStringToKKStr (stationName),
                                                            PicesKKStr::SystemStringToKKStr (deploymentNum)
                                                           );
    if  (sipperFiles == NULL)
      return  nullptr;

    PicesSipperFileList^  picesSipperFiles = gcnew PicesSipperFileList (sipperFiles);

    sipperFiles->Owner (false);
    delete  sipperFiles;
    sipperFiles = NULL;

    return   picesSipperFiles;
  }  /* SipperFileLoad */



  void  PicesDataBase::SipperFileLoadOracle ()
  {
    dbConn->SipperFileLoadOracle ();
  }



  PicesSipperFile^  PicesDataBase::SipperFileRecLoad  (String^ _sipperFileName)
  {
    SipperFilePtr  sipperFile = dbConn->SipperFileRecLoad (PicesKKStr::SystemStringToKKStr (_sipperFileName));
    if  (!sipperFile)
      return  nullptr;

    return  gcnew PicesSipperFile (sipperFile);
  }





  void  PicesDataBase::SipperFileInsert (PicesSipperFile^  sipperFile)
  {
    if  (sipperFile == nullptr)
    {
      lastOpSuccessful  = false;
      throw gcnew Exception ("PicesDataBase::SipperFileInsert   'nullptr' was passed in.");
    }

    if  (!(sipperFile->UnmanagedClass ()))
    {
      lastOpSuccessful  = false;
      throw gcnew Exception ("PicesDataBase::SipperFileInsert   The Un-Managed Class wass 'NULL'.");
    }

    dbConn->SipperFileInsert (*(sipperFile->UnmanagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
    return;
  }  /* SiperFileRecInsert */





  void   PicesDataBase::SipperFileUpdate (PicesSipperFile^  sipperFile)
  {
    if  (sipperFile == nullptr)
    {
      lastOpSuccessful  = false;
      throw gcnew Exception ("PicesDataBase::SipperFileUpdate   'nullptr' was passed in.");
    }

    if  (!(sipperFile->UnmanagedClass ()))
    {
      lastOpSuccessful  = false;
      throw gcnew Exception ("PicesDataBase::SipperFileUpdate   The Unmanaged Class wass 'NULL'.");
    }

    dbConn->SipperFileUpdate (*(sipperFile->UnmanagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }





  String^  PicesDataBase::SipperFileGetFullPath (String^ _sipperFileName)
  {
    String^  rootName = OSservices::GetRootName (_sipperFileName);

    KKStr  fullPathName = InstrumentDataFileManager::GetFullSipperFileName (PicesKKStr::SystemStringToKKStr (rootName));
    if  (fullPathName.Empty ())
      return nullptr;

    return  PicesKKStr::KKStrToSystenStr (fullPathName);
  }  /* SipperFileGetFullPath */





  //****************************************************************************************************
  //*                                 SipperStation  routines.                                         *
  //****************************************************************************************************
  PicesSipperStationList^  PicesDataBase::SipperStationLoad (String^  cruiseName)
  {
    SipperStationListPtr  stations = dbConn->SipperStationLoad (PicesKKStr::SystemStringToKKStr (cruiseName));
    if  (stations == NULL)
      return  nullptr;
    
    PicesSipperStationList^  results = gcnew PicesSipperStationList (stations);
    stations->Owner (false);
    delete  stations;
    stations = NULL;
    return  results;
  }  /* SipperStationLoad */



  PicesSipperStation^   PicesDataBase::SipperStationLoad (String^  cruiseName,
                                                          String^  stationName
                                                         )
  {
    SipperStationPtr  station = dbConn->SipperStationLoad (PicesKKStr::SystemStringToKKStr (cruiseName), 
                                                           PicesKKStr::SystemStringToKKStr (stationName)
                                                          );
    if  (station == NULL)
      return  nullptr;
    
    return gcnew PicesSipperStation (station);
  }  /* SipperStationLoad */




  void  PicesDataBase::SipperStationInsert (PicesSipperStation^  station)
  {
    if  (station == nullptr)
      throw gcnew Exception ("'station' == nullptr");

    dbConn->SipperStationInsert (*(station->UnmanagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }




  void  PicesDataBase::SipperStationUpdate (PicesSipperStation^  station)
  {
    if  (station == nullptr)
      throw gcnew Exception ("'station' == nullptr");

    dbConn->SipperStationUpdate (*(station->UnmanagedClass ()));
    lastOpSuccessful = dbConn->Valid ();
  }



  void  PicesDataBase::SipperStationDelete (String^  cruiseName,
                                            String^  stationName
                                           )
  {
    if  (cruiseName == nullptr)
      throw gcnew Exception ("'cruiseName' == nullptr");

    if  (stationName == nullptr)
      throw gcnew Exception ("'stationName' == nullptr");

    dbConn->SipperStationDelete (PicesKKStr::SystemStringToKKStr (cruiseName), 
                                 PicesKKStr::SystemStringToKKStr (stationName)
                                );
    lastOpSuccessful = dbConn->Valid ();
  }  /* SipperStationDelete */

}  /* PicesInterface */