#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <math.h>

#include  <map>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;

#include  "Compressor.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "InstrumentDataFileManager.h"
using namespace SipperHardware;

#include  "DataBase.h"
#include  "DataBaseImage.h"
#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
using namespace MLL;


#include "MergeFeatureFiles.h"


// -format c45  -src CovType-TrainTest.data  -src CovType-Validation.data  -dest CovType-All.data  -randomize


// C:\Users\kkramer\GradSchool\Papers\BinaryFeatureSelection\Experiments\ETP_TrainTest_Acc
// -format Pices -src ETP-Train.data  -src ETP-Test.data  -dest ETP-TrainTest.data  -Stratify 10

// C:\Users\kkramer\GradSchool\Papers\BinaryFeatureSelection\Experiments\PenDigits
// -format  UCI  -src pendigits.tes.txt -format c45 -dest pendigits_tes_Stratified.data  -stratify 10


// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\WFS2
// -format  Pices  -src Wfs_Test.data   -src Wfs_Train.data  -src Wfs_Validation.data    -dest Wfs_All.data  -stratify 10

// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\NineClasses_TrainTest_Acc\
// -format C45  -src NineClasses_TrainTest_c45.data  -src NineClasses_ValidationData_c45.data  -dest C:\Users\kkramer\Dissertation\BiasAdjustmentExperiment\NineClassPlankton\NineClassPlankton.data  -Stratify 20


MergeFeatureFiles::MergeFeatureFiles (int     argc, 
                                      char**  argv
                                      ):

   Application          (argc, argv),
   currentDefaultFormat (FeatureFileIOPices::Driver ()),
   numOfFolds           (10),
   randomize            (false),
   srcData              (NULL),
   stratify             (false)
{
  if  (argc < 2)
  {
    log.Level (-1) << endl << endl 
                   << "No Command Line Parameters were provided." << endl
                   << endl;

    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  InstrumentDataFileManager::InitializePush ();
  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }


  if  (srcFileNames.size () < 1)
  {
    log.Level (-1) << endl << endl 
                   << "MergeFeatureFiles    ***ERROR***   you must provide at least one '-src' parameter." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }

  if  (destFileName.Empty ())
  {
    log.Level (-1) << endl << endl 
                   << "MergeFeatureFiles    ***ERROR***   No '-dest' parameter was specified." << endl
                   << endl;
    DisplayCommandLineParameters ();
    Abort (true);
    return;
  }
}





MergeFeatureFiles::~MergeFeatureFiles ()
{
  delete  srcData;
  InstrumentDataFileManager::InitializePop ();
}




bool  MergeFeatureFiles::ProcessCmdLineParameter (
                                                  char    parmSwitchCode, 
                                                  KKStr   parmSwitch, 
                                                  KKStr   parmValue
                                                 )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();

  parmSwitch.Upper ();

  if  ((parmSwitch == "-S")  ||  (parmSwitch == "-SRC")||  (parmSwitch == "-SOURCE"))
  {
    KKStr  srcFileName = parmValue;
    if  (!osFileExists (srcFileName))
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Invalid '-src' [" << srcFileName << "] file." << endl;
      Abort (true);
    }

    srcFileNames.push_back (srcFileName);
    srcFormats.push_back   (currentDefaultFormat);
    if  (!currentDefaultFormat->CanRead ())
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Format[" << currentDefaultFormat->DriverName () << "] does not support read." << endl;
      Abort (true);
    }
  }

  else if  ((parmSwitch == "-DEST")  ||  (parmSwitch == "-D")  ||  (parmSwitch == "-DESTINATION"))
  {
    if  (!destFileName.Empty ())
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    More than one destination file was specified." << endl;
      Abort (true);
    }
    
    destFileName = parmValue;
    if  (osFileExists (destFileName))
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Destination File[" << destFileName << "] already exists." << endl;
      Abort (true);
    }

    if  (!currentDefaultFormat->CanWrite ())
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    Format[" << currentDefaultFormat->DriverName () << "] does not support write." << endl;
      Abort (true);
    }

    destFormat = currentDefaultFormat;
  }

  else if  ((parmSwitch == "-F")  ||  (parmSwitch == "-FORMAT"))
  {
    FeatureFileIOPtr  newFormat = FeatureFileIO::FileFormatFromStr (parmValue);
    if  (!newFormat)
    {
      log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    No such format as[" << parmValue<< "]." << endl;
      Abort (true);
    }
    else
    {
      currentDefaultFormat = newFormat;
    }
  }

  else if  ((parmSwitch == "-STRAT")  ||  (parmSwitch == "-STRATIFY"))
  {
    stratify = true;
    if  (!parmValue.Empty ())
    {
      numOfFolds = parmValue.ToInt ();
      if  ((numOfFolds < 1)  ||  (numOfFolds > 1000))
      {
        log.Level (-1) << "ProcessCmdLineParameter   ***ERROR***    -Folds[" << numOfFolds << "]  must be 1 or greater and less than 10000." << endl;
        Abort (true);
      }
    }
  }


  else if  ((parmSwitch == "-RAND")  ||  (parmSwitch == "-RANDOM")  ||  (parmSwitch == "-RANDOMIZE"))
  {
    randomize = true;
  }

  else
  {
    log.Level (-1) << endl << endl
                   << "MergeFeatureFiles::ProcessCmdLineParameter    ***ERROR***" << endl
                   << endl
                   << "             Invalid Parameter[" << parmSwitch << "]" << endl
                   << endl;
    Abort (true);
  }


	return  !Abort ();
}  /* ProcessCmdLineParameter */




/******************************************************************************
 * DisplayCommandLineParameters()
 * DESC: Displays a help message to the user
 ******************************************************************************/
void   MergeFeatureFiles::DisplayCommandLineParameters ()
{
  log.Level (0) << "MergeFeatureFiles"                                                               << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "    -src   <Source File>  Can specify multiple -src parameters.  You specify"    << endl;
  log.Level (0) << "           one of these parametrs for eachfeature file to include in"            << endl;
  log.Level (0) << "           destination feature file."                                            << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "    -dest  <destination File>  Mandatory parameter and can only specify one."    << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "    -format  " << FeatureFileIO::FileFormatsReadAndWriteOptionsStr ()            << endl;
  log.Level (0) << "           all feature Files that follow this parameter will use this format"    << endl;
  log.Level (0) << "           until another '-format' parameter is specified."                      << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "           Read  formats Supported: " << FeatureFileIO::FileFormatsReadOptionsStr    () << endl;
  log.Level (0) << "           Write formats Supported: " << FeatureFileIO::FileFormatsWrittenOptionsStr () << endl;
  log.Level (0) << "           if no '-format' paramter provided defaults to 'Pices'."               << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "    -Stratify <NumOfFolds>     Will stratify the output by 'Class'."             << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "    -Randomize   Will sort th edestination file into random order."              << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "Examples:"                                                                       << endl;
  log.Level (0) << "  MergeFeatureFiles  -format c45  -src Src1.data  -src Src2.data  -format arff   -dest combined.data"  << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "           Will read in the files 'Src1.data' and 'Src2.data' which should be"   << endl;
  log.Level (0) << "           in 'c45' format and write the output to Combined.data in 'arff'"      << endl;
  log.Level (0) << "           format."                                                              << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "  MergeFeatureFiles  -format c45  -src Src1.data  -src Src2.data  -dest combined.data  -Stratify 10"  << endl;
  log.Level (0)                                                                                      << endl;
  log.Level (0) << "           Will read in the files 'Src1.data' and 'Src2.data' which should be"   << endl;
  log.Level (0) << "           in 'c45' format and write the output to Combined.data also in 'c45'"  << endl;
  log.Level (0) << "           format with the data stratified by class for 10 folds."               << endl;
}  /* DisplayCommandLineParameters */



void   MergeFeatureFiles::Main ()
{
  if  (Abort ())
    return;

  {
    // Will first load source files.
    uint  srcIdx = 0;
    for  (srcIdx = 0;  srcIdx < srcFileNames.size ();  srcIdx++)
    {
      bool  cancelFlag  = false;
      bool  successful  = false;
      bool  changesMade = false;

      KKStr             srcFileName = srcFileNames[srcIdx];
      FeatureFileIOPtr  srcFormat   = srcFormats  [srcIdx];
      
      MLClassConstList   classes;
      
      FeatureVectorListPtr  s = NULL;

      s = srcFormat->LoadFeatureFile (srcFileName, classes, -1, cancelFlag, successful, changesMade, log);

      if  ((s == NULL)  ||  (!successful))
      {
        log.Level (-1) << endl << endl
                       << "MergeFeatureFiles::Main   ***ERROR***    Could not load file[" << srcFileName << "]" << endl
                       << endl;
        delete  s;  s = NULL;
        Abort (true);
        return;
      }

      if  (!srcData)
        srcData = new FeatureVectorList (s->FileDesc (), true, log);

      if  ((*(s->FileDesc ())) != (*(srcData->FileDesc ())))
      {
        // The last source file read does not have the same attribute sets as previous feature files already read.
        log.Level (-1) << endl << endl
                       << "MergeFeatureFiles::Main   ***ERROR***    Feature File[" << srcFileName << "] does not have the same Attributes as previous feature files already read in." << endl
                       << endl;
        Abort (true);
        delete  s;  s = NULL;
        return;
      }
      
      srcData->AddQueue (*s);
      s->Owner (false);
      delete  s;
    }
  }


  if  (!Abort ())
  {
    if  (randomize)
    {
      DateTime  dt = osGetLocalDateTime ();
      srcData->RandomizeOrder (dt.Seconds ());
      srcData->RandomizeOrder ();
    }

    if  (stratify)
    {
      FeatureVectorListPtr  stratifiedSrc = srcData->StratifyAmoungstClasses (numOfFolds);
      srcData->Owner (false);
      stratifiedSrc->Owner (true);
      delete  srcData;
      srcData = stratifiedSrc;
      stratifiedSrc = NULL;
    }

    bool  cancelFlag  = false;
    bool  successful  = false;

    uint  numExamplesWritten = 0;
    destFormat->SaveFeatureFile (destFileName, srcData->AllFeatures (), *srcData, numExamplesWritten, cancelFlag, successful, log);
    if  (!successful)
    {
      log.Level (-1) << endl << endl
                     << "MergeFeatureFiles::Main   ***ERROR***    Could not save to file[" << destFileName << "]" << endl
                     << endl;
      Abort (true);
    }
  }
}  /* Main */






// I added this code as a one time fix to update all the Instrument Data tables after I made sure that the
// Sipper file definitions were correct.
void  ReFreshInstrumentData (DataBasePtr  dbConn,
                             RunLog&      log
                            )
{
  InstrumentDataFileManager::InitializePush ();
  SipperFileListPtr  sipperFiles = dbConn->SipperFileLoad ("", "", "");

  SipperFileList::iterator  idx;

  bool   cancelFlag = false;
  char   msgBuff[1024];

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;

    cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;

    dbConn->InstrumentDataReFreshSipperFile (sf->SipperFileName (), cancelFlag, msgBuff, sizeof (msgBuff));


    //InstrumentDataListPtr  id = InstrumentDataFileManager::ReExtractInstrumentDataForSipperFile 
    //      (sf->SipperFileName (), sf, log);;

    //if  (id != NULL)
    //  dbConn->InstrumentDataSaveListForOneSipperFile (sf->SipperFileName (), *id, cancelFlag);
  }

  InstrumentDataFileManager::InitializePop ();

}  /* UpdateInstrumentData */




void  TestCompressClass ()
{
  char* inBuff = "This is a test of the Compressing rountine to see how well it will work";
  int   inBuffLen = strlen (inBuff) + 1;

  KKU::uint  compressedLen = 0;
  void*  compressedData = Compressor::CreateCompressedBuffer ((KKU::uchar*)inBuff, inBuffLen, compressedLen);

  KKU::uint  unCompressedLen = 0;
  char*  decompressedStr = (char*)Compressor::Decompress (compressedData, compressedLen, unCompressedLen);

  cout << decompressedStr << endl;

}  /* TestCompressClass */




void  Test ()
{
  RunLog  log;
  DataBasePtr  dbConn = new DataBase (log);

  //  InstrumentDataPtr  id = dbConn->InstrumentDataGetByScanLine ("TestCruise_01", 4022);

  //{
  //  ImageFeaturesPtr  fv  = NULL;
  //  KKStr imageFileName = "TestCruise_01_00006156_3701";
  //  DataBaseImagePtr dbi = dbConn->ImageLoad (imageFileName);
  //  if  (dbi)
  //    fv  = dbConn->FeatureDataRecLoad (dbi);
  //  delete  fv;
  //  delete  dbi;
  //}

  {
    SipperCruiseListPtr  cruises = dbConn->SipperCruiseLoadAllCruises ();
    delete  cruises;
  }



  bool  cancelFlag = false;

  {
    DataBaseImageGroupPtr  existingGroup = dbConn->ImageGroupLoad ("TestGroup2");

    if  (existingGroup)
    {

      VectorUint*  depthStats = dbConn->ImageGetDepthStatistics 
             (existingGroup, 
              "",         // sipperFileName
              10.0f,      // depthIncrements,
              NULL,       // mlClass,
              'P',        // 'p' = Use Predicted Class
              0.0f, 1.0f, // minProb, maxProb,
              0, 0        // minSize, maxSize
             );

      delete  depthStats;
      depthStats = NULL;



      ClassStatisticListPtr  stats = dbConn->ImageGetClassStatistics 
           (existingGroup,
            "ETP2008_8A_06",
            NULL,
            'P',         // 'P' = Use Predicted Class
            0.0f, 1.0f,  // MinProb,  MaxProb
            0, 0,        // MinSize,  MaxSize
            0.0f, 0.0f   // MinDepth, MaxDepth
           );



      delete  stats;
      stats = NULL;
    }

    DataBaseImageListPtr  images = dbConn->ImagesQuery (existingGroup, true, cancelFlag);
  }

  DataBaseImageGroupPtr g = new  DataBaseImageGroup (-1, "TestGroup2", "Description of group", 0);
  dbConn->ImageGroupInsert (*g);
  if  (dbConn->DuplicateKey ())
  {
    DataBaseImageGroupPtr  existingGroup = dbConn->ImageGroupLoad (g->Name ());
    if  (existingGroup)
    {
      g->ImageGroupId (existingGroup->ImageGroupId ());
      dbConn->ImageGroupDelete (existingGroup->ImageGroupId ());
      dbConn->ImageGroupInsert (*g);
      delete  existingGroup;
      existingGroup = NULL;
    }
  }


  DataBaseImageListPtr  images = dbConn->ImagesQuery (NULL,
                                                      "ETP2008", "8", "A", NULL,
                                                      'P',             // 'P' = Use Predicted Class
                                                      0.0f, 1.0f,      // MinProb,  MaxProb
                                                      0, 0,            // MinSize,  MaxSize
                                                      290.0f, 293.0f,  // MinDepth, MaxDepth
                                                      0,               // Restart Image
                                                      -1,              // unlimited Limit
                                                      true,            // true=Include ThumbNail
                                                      cancelFlag
                                                     );


  VectorKKStr fileNames;
  {
    DataBaseImageList::iterator  idx;
    for  (idx = images->begin ();  idx != images->end ();  idx++)
      fileNames.push_back ((*idx)->ImageFileName ());
  }

  dbConn->ImageGroupEntriesInsert (g->ImageGroupId (), fileNames);

  
  delete  dbConn;
}  /* Test */



map<KKStr, KKStr>*  GetImagesAlreadyInImagesFullSize (const KKStr&  sipperFileName,
                                                      DataBasePtr   dbConn
                                                     )
{
  KKStr queryStr = "call  ImagesFullSizeFileNamesForSipperFile(" + sipperFileName.QuotedStr () + ")";
  char*  fieldNanes[] = {"ImageId", "ImageFileName", "FullSizeImageLength", NULL};
  map<KKStr, KKStr>*  alreadyInFullSize = new map<KKStr, KKStr> ();

  KKStrMatrixPtr  results = dbConn->QueryStatementKKStr (queryStr, fieldNanes);
  if  (!results)
    return  alreadyInFullSize;

  for  (KKU::uint row = 0;  row < results->NumRows ();  row++)
  {
    KKStr  imageFileName = (*results)[row][1];
    alreadyInFullSize->insert (pair<KKStr, KKStr> (imageFileName, imageFileName));
  }

  delete  results;
  results = NULL;

  return  alreadyInFullSize;
}





void  UpdateFullSizeImages ()
{
  RunLog  log;
  DataBasePtr  dbConn = new DataBase (log);


  SipperFileListPtr  sipperFiles = dbConn->SipperFileLoad ("ETP2008", "", "");

  SipperFileList::iterator  idx;

  bool   cancelFlag = false;


  KKU::uint  numImages = 0;
  KKU::uint  numImagesSaved = 0;

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  idx++)
  {
    SipperFilePtr sf = *idx;

    KKStr  fullSipperFileName = InstrumentDataFileManager::GetFullSipperFileName (sf->SipperFileName ());
    if  (fullSipperFileName.Empty ())
    {
      // we do not have access to the sipper file;  no point proessing this file.
      continue;
    }

    int imagesThisSipperFile = 0;

    cout << "Sipper File [" << sf->SipperFileName () << "]" << std::endl;

    bool  cancelFlag = false;
    //DataBaseImageListPtr  images = dbConn->ImagesQuery (NULL, sf->SipperFileName (), NULL, 0.0f, 1.0f, 0, 0, 0.0f, 0.0f, "ETP2008_1AA_01_04560000_0000", 1000, false, cancelFlag);
    DataBaseImageListPtr  images = dbConn->ImagesQuery (NULL, sf->SipperFileName (), NULL, 'P', 0.0f, 1.0f, 0, 0, 0.0f, 0.0f, 0, -1, false, cancelFlag);
    
    map<KKStr, KKStr>*  imagesAlreadtInFullSize = GetImagesAlreadyInImagesFullSize (sf->SipperFileName (), dbConn);
    map<KKStr, KKStr>::iterator  fullSizeIdx;

    DataBaseImageList::iterator  idx;
    for  (idx = images->begin ();  idx != images->end ();  idx++)
    {
      imagesThisSipperFile++;
      numImages++;

      DataBaseImagePtr  i = *idx;
      KKU::uint  maxDim = Max (i->Height (), i->Width ());
      if  (maxDim > 100)
      {
        fullSizeIdx = imagesAlreadtInFullSize->find (i->ImageFileName ());
        if  (fullSizeIdx == imagesAlreadtInFullSize->end ())
        {
          // We need to get the full size version of this image.
          RasterPtr r = i->GetOrigImageFromSipperFile (log);
          if  (r)
          {
            dbConn->ImageFullSizeSave (i->ImageFileName (), *r);
            delete  r;
            r = NULL;
          }
          numImagesSaved++;
        }
      }

      if  ((numImages % 100) == 0)
      {
        cout << sf->SipperFileName () << "\t"
             << StrFormatInt (images->QueueSize (), "ZZ,ZZZ,ZZ0") << "\t"
             << StrFormatInt (imagesThisSipperFile, "ZZ,ZZZ,ZZ0") << "\t"
             << StrFormatInt (numImages,            "ZZ,ZZZ,ZZ0") << "\t"
             << StrFormatInt (numImagesSaved,       "ZZ,ZZZ,ZZ0")
             << endl;
      }

    }

    delete  imagesAlreadtInFullSize;
    imagesAlreadtInFullSize = NULL;

    delete  images;
    images = NULL;
  }
}   /* UpdateFullSizeImages */



/**
 @brief  One off routine to read in csv file with GPS data from GULF 2010-may cruise
         April Gulf Oild Cruize.
 */

void  ImpotrtGPSData (const KKStr&  fileName)
{
  RunLog  log;

  DataBasePtr  dbConn = new DataBase (log);

  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
  {
    log.Level (-1) << endl << endl 
       << "ImpotrtGPSData  Could not open file[" << fileName << "]" << endl
       << endl;
    return;
  }

  log.Level (10) << endl << endl << endl << endl << endl
    << "ImpotrtGPSData   FileName[" << fileName << "]" << endl << endl
    << endl;

  char  buff[20480];
  bool  firstPass = true;

  int  lastHour = 0;

  DateTime  lastDateTime;
  while  (i.getline (buff, sizeof (buff)))
  {
    KKStr  ln = buff;
    ln.TrimLeft ();
    char  ch = ln[0];
    if  ((ch < '0')  ||  (ch > '9'))
      continue;

    KKStr  dateStr = ln.ExtractToken2 (",");
    KKStr  timeStr = ln.ExtractToken2 (",");

    KKStr  WXTP_TaStr = ln.ExtractToken2 (",");
    KKStr  WXTS_TaStr = ln.ExtractToken2 (",");

    KKStr  WXTS_PaStr = ln.ExtractToken2 (",");
    KKStr  WXTP_PaStr = ln.ExtractToken2 (",");
    KKStr  dec_latStr = ln.ExtractToken2 (",");
    KKStr  dec_lonStr = ln.ExtractToken2 (",");

    KKStr  cogStr = ln.ExtractToken2 (",");
    KKStr  sogStr = ln.ExtractToken2 (",");

    DateType  gmtDate (dateStr);
    TimeType  gmtTime (timeStr);

    DateTime  gmtDateTime (gmtDate, gmtTime);
    DateTime  localTime = gmtDateTime;
    localTime.HoursAdd (-4);

    DateTime  startDT = localTime;
    DateTime  endDT   = localTime;

    if  (firstPass)
    {
      firstPass = false;
      startDT.SecondsAdd (-180);
    }
    else
    {
      DateTime deltaDT = localTime - lastDateTime;
      long  deltaSecs = (long)deltaDT.Seconds ();
      startDT.SecondsAdd (-(deltaSecs / 2));
    }

    endDT.SecondsAdd (120);

    dbConn->InstrumentDataUpdateLatitudeAndLongitude (startDT, endDT, dec_latStr.ToDouble (), dec_lonStr.ToDouble ());

    lastDateTime = localTime;

    if  (gmtTime.Minute () != lastHour)
    {
      lastHour = gmtTime.Hour ();
      log.Level (10) << "File[" << osGetRootName (fileName) << "]  GMT Time[" << gmtDate.MMM_DD_YYYY () << " - " << gmtTime.HH_MM_SS () << "]" << endl;
    }
  }

  i.close ();

  delete  dbConn;
  dbConn = NULL;


}  /* ImpotrtGPSData */




/**
 @brief  One off routine to read in csv file with GPS data from GULF 2010-may cruise
         May - June Gulf Oil Cruize.   GPGGA Strings
 */

void  ImportGPSDataGPGGA (const KKStr&  fileName)
{
  RunLog  log;

  DataBasePtr  dbConn = new DataBase (log);

  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
  {
    log.Level (-1) << endl << endl 
       << "ImpotrtGPSData  Could not open file[" << fileName << "]" << endl
       << endl;
    return;
  }
  log.Level (10) << endl << endl << endl << endl << endl
    << "ImpotrtGPSData   FileName[" << fileName << "]" << endl << endl
    << endl;

  char  buff[20480];
  bool  firstPass = true;

  int  lastMinute = 0;
  int  linesRead = 0;

  KKStr  ln (256);

  DateTime  lastDateTime;
  while  (i.getline (buff, sizeof (buff)))
  {
    linesRead++;
    ln = buff;
    ln.TrimLeft ();
     
    if  (ln.LocateStr ("GPGGA") < 0)
      continue;

    VectorKKStr  fields = ln.Parse (",");
    if  (fields.size () < 8)
      continue;

    if  (!fields[2].EqualIgnoreCase ("$GPGGA"))
      continue;


    /*
    0           1             2        3         4      5       6       7   8  
06/01/2010, 23:59:59.818,  $GPGGA,  235958,  2840.927,  N,  08828.458,  W,  2,  09,22.10,0,M,,,14,0000*12
06/02/2010, 00:00:10.818,  $GPGGA,  000009,  2840.931,  N,  08828.482,  W,  1,  09,0.89,0,M,,,,*2D
06/02/2010, 00:00:21.802,  $GPGGA,  000020,  2840.929,  N,  08828.505,  W,  1,  09,0.89,0,M,,,,*21
06/02/2010, 00:00:31.818,  $GPGGA,  000030,  2840.924,  N,  08828.526,  W,  1,  09,0.89,0,M,,,,*2C
06/02/2010, 00:00:42.818,  $GPGGA,  000041,  2840.917,  N,  08828.547,  W,  1,  09,0.89,0,M,,,,*2D
06/02/2010, 00:00:53.802,  $GPGGA,  000052,  2840.906,  N,  08828.568,  W,  1,  09,1.00,0,M,,,,*22
06/02/2010, 00:01:03.802,  $GPGGA,  000102,  2840.895,  N,  08828.585,  W,  1,  09,0.89,0,M,,,,*2E
06/02/2010, 00:01:13.818,  $GPGGA,  000112,  2840.883,  N,  08828.600,  W,  1,  09,0.89,0,M,,,,*26
   */


    KKStr  dateStr = fields[0];
    KKStr  timeStr = fields[1];

    KKStr  latStr = fields[4];
    KKStr  logStr = fields[6];

    int  x = latStr.LocateCharacter ('.');
    if  (x < 0) 
      continue;

    KKStr latMinStr = latStr.SubStrPart (x - 2);
    KKStr latDegStr = latStr.SubStrPart (0, x - 3);

    double latitude = latDegStr.ToDouble () + latMinStr.ToDouble () / 60.0;
    if  (fields[5].EqualIgnoreCase ("S"))
      latitude = 0.0 - latitude;


    x = logStr.LocateCharacter ('.');
    if  (x < 0) 
      continue;

    KKStr logMinStr = logStr.SubStrPart (x - 2);
    KKStr logDegStr = logStr.SubStrPart (0, x - 3);

    double longitude = logDegStr.ToDouble () + logMinStr.ToDouble () / 60.0;
    if  (fields[7].EqualIgnoreCase ("W"))
      longitude = 0.0 - longitude;


    DateType  gmtDate (dateStr);
    TimeType  gmtTime (timeStr);

    DateTime  gmtDateTime (gmtDate, gmtTime);
    DateTime  localTime = gmtDateTime;
    localTime.HoursAdd (-4);

    DateTime  startDT = localTime;
    DateTime  endDT   = localTime;

    if  (firstPass)
    {
      firstPass = false;
      startDT.SecondsAdd (-180);
    }
    else
    {
      DateTime deltaDT = localTime - lastDateTime;
      long  deltaSecs = (long)deltaDT.Seconds ();
      startDT.SecondsAdd (-(deltaSecs / 2));
    }

    endDT.SecondsAdd (30);

    if  (gmtTime.Minute () != lastMinute)
    {
      lastMinute = gmtTime.Minute ();
      log.Level (10) << "LinesRead[" << linesRead << "]  File[" << osGetRootName (fileName) << "]  GMT Time[" << gmtDate.MMM_DD_YYYY () << " - " << gmtTime.HH_MM_SS () << "]" << endl;
    }

    if  ((endDT.Month () < 6)  &&  (endDT.Day () < 28))
    {
    }
    else
    {
      dbConn->InstrumentDataUpdateLatitudeAndLongitude (startDT, endDT, latitude, longitude);
    }

    lastDateTime = localTime;
  }

  i.close ();

  delete  dbConn;
  dbConn = NULL;


}  /* ImportGPSDataGPGGA */


void  ImportGPSDataGPGGAFiles ()
{
  KKStrListPtr   files = osGetListOfFiles ("952X-GPGGA*.Raw");
  if  ((files == NULL)  ||  (files->QueueSize () < 1))
  {
    cerr << endl << endl
      << "ImportGPSDataGPGGAFiles    ***ERROR***     No Files Found" << endl
      << endl;
    delete  files;
    files = NULL;
    return;
  }

  KKStrList::iterator  idx;
  
  for  (idx = files->begin ();  idx != files->end ();  idx++)
  {
    ImportGPSDataGPGGA (**idx);
  }

  delete  files;
}  /* ImportGPSDataGPGGAFiles */






void  UpdateGulf2010MayGPSData ()
{
  /*
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100503_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100504_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100505_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100506_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100507_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100508_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100509_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100510_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100511_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100512_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100513_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100514_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100515_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100516_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\SipperProject\\GulfCruise_2010-05\\GPS_RawDataFromWeatherBird\\WB100517_00.csv");
  */


  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100503_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100504_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100505_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100506_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100507_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100508_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100509_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100510_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100511_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100512_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100513_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100514_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100515_00.csv");
  ImpotrtGPSData ("C:\\Users\\kkramer\\Dropbox\\Sipper\\GPS_Data\\2010-05\\WB100516_00.csv");
 }




void  ImportValidatedClassInfo ()
{
  RunLog  log;

  KKStr  srcFileName = "C:\\Temp\\ValidatedImages.csv";

  ifstream i (srcFileName.Str ());
  if  (!i.is_open ())
    return;

  DataBasePtr  dbConn = new DataBase (log);
  if  (!dbConn)
    return;

  char  buff[20480];

  int  count = 0;

  KKStr  s (1024);
  KKStr  imageFileName;
  int    validatedClassId;
  KKStr  validatedClassName;

  KKStr errMsg = "";


  while  (i.getline (buff, sizeof (buff)))
  {
    count++;
    s = buff;
    imageFileName      = s.ExtractQuotedStr (",\n\t\r", false);
    validatedClassId   = s.ExtractTokenInt  (",\n\t\r");
    validatedClassName = s.ExtractQuotedStr (",\n\t\r", false);

    MLClassConstPtr  c = MLClass::CreateNewMLClass (validatedClassName);

    dbConn->ImagesUpdateValidatedClass (imageFileName, c);
   
    if  (!dbConn->Valid ())
      errMsg = dbConn->LastErrorDesc ();

    cout << count << "\t" << imageFileName << "\t" << validatedClassName << "\t" << errMsg << endl;
    errMsg = "";
  }
}  /* ImportValidatedClassInfo */


#include  "FeatureFileIOC45.h"

void  Strip ()
{
  bool  cancelFlag  = false;
  bool  successful  = false;
  bool  changesMade = false;

  RunLog  log;

  FeatureFileIOPtr driver =  FeatureFileIO::FileFormatFromStr ("C45");

  MLClassConstList  mlClasses;
  FeatureVectorListPtr  data = 
        driver->LoadFeatureFile ("D:\\Pices\\Reports\\FeatureDataFiles\\AllValidatedImages_ForJonathon\\AllValidatedDataNorm.data",
                                 mlClasses,
                                 -1,
                                 cancelFlag,
                                 successful,
                                 changesMade,
                                 log
                               );

  FeatureVectorListPtr  stripped = new FeatureVectorList (data->FileDesc (), false, log, 10000);

  FeatureVectorList::const_iterator  idx;
  for  (idx = data->begin ();  idx != data->end ();  ++idx)
  {
    FeatureVectorPtr  fv = *idx;
    KKStr  fn = fv->ImageFileName ();
    if  (fn.StartsWith ("SML")  ||  (fn.StartsWith ("SMP")))
    {
    }
    else
    {
      stripped->PushOnBack (fv);
    }
  }


  uint32  numExamplesWritten = 90;
  driver->SaveFeatureFile ("D:\\Pices\\Reports\\FeatureDataFiles\\AllValidatedImages_ForJonathon\\AllValidatedData1209.data",
                           data->AllFeatures (), 
                           *stripped, 
                           numExamplesWritten,
                           cancelFlag,
                           successful,
                           log
                          );



}

          


int  main (int     argc,
           char**  argv
          )
{
  //Test ();
  //exit (0);

  //UpdateGulf2010MayGPSData ();
  //exit (0);

  //UpdateFullSizeImages ();
  //exit (0);

  //UpdateGulf2010MayGPSData ();
  //exit (-1);

  //ImportGPSDataGPGGAFiles ();
  //exit (-1);

  //ImportFullSizeImages ();
  //exit (-1);

  //ImportValidatedClassInfo ();
  //exit (-1);

  MergeFeatureFiles  mergeFeatureFiles (argc, argv);
  if  (mergeFeatureFiles.Abort ())
    return 1;



  mergeFeatureFiles.Main ();
  if  (mergeFeatureFiles.Abort ())
    return 1;
  else
    return 0;
}
