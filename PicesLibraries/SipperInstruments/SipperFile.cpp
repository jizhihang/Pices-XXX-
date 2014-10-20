#include "FirstIncludes.h"
#include <stdlib.h>
#include <string.h>
#include <memory>

#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "MemoryDebug.h"
using namespace std;


#include "BasicTypes.h"
#include "GlobalGoalKeeper.h"
#include "KKStrParser.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;


#include "SipperFile.h"
#include "InstrumentDataFileManager.h"
using  namespace  SipperHardware;


SipperFile::SipperFile (const KKStr&  _sipperFileName):
   sipperFileId            (-1),
   sipperFileName          (),
   cruiseName              (),
   stationName             (),
   deploymentNum           (),
   description             (),
   latitude                (),
   longitude               (),
   dateTimeStart           (osGetLocalDateTime ()),
   sp0                     (NULL),
   sp1                     (NULL),
   sp2                     (NULL),
   sp3                     (NULL),
   ctdExt0                 (),
   ctdExt1                 (),
   ctdExt2                 (),
   ctdExt3                 (),
   sizeInBytes             (0),
   numScanLines            (0),
   scanRate                (0.0f),
   depth                   (0.0),
   extractionStatus        ('0'),
   extractionScanLineStart (0),
   extractionScanLineEnd   (0)
{
  sipperFileName = osGetRootName (_sipperFileName);
}



SipperFile::SipperFile (KKStrParser&  str):
   sipperFileId            (-1),
   sipperFileName          (),
   cruiseName              (),
   stationName             (),
   deploymentNum           (),
   description             (),
   latitude                (),
   longitude                (),
   dateTimeStart           (),
   sp0                     (NULL),
   sp1                     (NULL),
   sp2                     (NULL),
   sp3                     (NULL),
   ctdExt0                 (),
   ctdExt1                 (),
   ctdExt2                 (),
   ctdExt3                 (),
   sizeInBytes             (0),
   numScanLines            (0),
   scanRate                (0.0f),
   depth                   (0.0),
   extractionStatus        ('0'),
   extractionScanLineStart (0),
   extractionScanLineEnd   (0)
{
  ParseTabDelStr (str);
}




SipperFile::~SipperFile ()
{
}



KKStr  SipperFile::Sp0ShortName  () const
{
  if  (sp0 == NULL)
    return "";
  else
    return sp0->ShortName ();
}

KKStr  SipperFile::Sp1ShortName  () const
{
  if  (sp1 == NULL)
    return "";
  else
    return sp1->ShortName ();
}


KKStr  SipperFile::Sp2ShortName  () const
{
  if  (sp2 == NULL)
    return "";
  else
    return sp2->ShortName ();
}


KKStr  SipperFile::Sp3ShortName  () const
{
  if  (sp3 == NULL)
    return "";
  else
    return sp3->ShortName ();
}



KKStr  SipperFile::ToTabDelStr ()  const
{
  KKStr  r (1024);
  r         << "SipperFileId"    << "\t" << sipperFileId
    << "\t" << "SipperFileName"  << "\t" << sipperFileName
    << "\t" << "CruiseName"      << "\t" << cruiseName
    << "\t" << "StationName"     << "\t" << stationName
    << "\t" << "DeploymentNum"   << "\t" << deploymentNum;

  if  (!description.Empty ())  
    r << "\t" << "Description"  << "\t" << description;

  if  (latitude  != 0.0)   r << "\t" << "Latitude"  << "\t" << latitude;
  if  (longitude != 0.0)   r << "\t" << "Longitude" << "\t" << longitude;

  if  (dateTimeStart.Year () >= 1990)
    r << "\t" << "DateTimeStart"  << "\t" << dateTimeStart;

  if  (sp0)  r << "\t" << "SP0" << "\t" << sp0->ShortName ();
  if  (sp1)  r << "\t" << "SP1" << "\t" << sp1->ShortName ();
  if  (sp2)  r << "\t" << "SP2" << "\t" << sp2->ShortName ();
  if  (sp3)  r << "\t" << "SP3" << "\t" << sp3->ShortName ();

  if  (!ctdExt0.Empty ())  r << "\t" << "CtdExt0" << "\t" << ctdExt0;
  if  (!ctdExt1.Empty ())  r << "\t" << "CtdExt1" << "\t" << ctdExt0;
  if  (!ctdExt2.Empty ())  r << "\t" << "CtdExt2" << "\t" << ctdExt2;
  if  (!ctdExt3.Empty ())  r << "\t" << "CtdExt3" << "\t" << ctdExt3;

  if  (sizeInBytes != 0)
    r << "\t" << "sizeInBytes" << "\t" << sizeInBytes;

  if  (numScanLines != 0)
    r << "\t" << "NumScanLines" << "\t" << numScanLines;

  if  (depth != 0.0f)
    r << "\t" << "Depth" << "\t" << depth;

  if  (scanRate != 0.0f)
    r << "\t" << "ScanRate" << "\t" << scanRate;

  r << "\t" << "ExtractionStatus"        << "\t" << extractionStatus
    << "\t" << "ExtractionScanLineStart" << "\t" << extractionScanLineStart
    << "\t" << "ExtractionScanLineEnd"   << "\t" << extractionScanLineEnd;

  return r;
}  /* ToTabDelStr */



int32  SearchFieldIdx (const KKStr  searchField, 
                       const char*  fieldNames[]
                      )
{
  int32 idx = 0;

  for  (idx = 0;  fieldNames[idx] != NULL;  idx++)
  {
    if  (searchField.EqualIgnoreCase (fieldNames[idx]))
      return idx;
  }
  return -1;
}  /* SearchFieldIdx */




void  SipperFile::ParseTabDelStr (KKStrParser&  parser)
{
  static  const char* fieldNames[] 
                     = {"SipperFileId", "SipperFileName", "CruiseName",        //  0 -  2
                        "stationName",  "deploymentNum",  "description",       //  3 -  5
                        "latitude",     "longitude",      "DateTimeStart",     //  6 -  8
                        "SP0",          "SP1",            "SP2",               //  9 - 11
                        "SP3",          "CtdExt0",        "CtdExt1",           // 12 - 14
                        "CtdExt2",      "CtdExt3",        "SizeInBytes",       // 15 - 17
                        "NumScanLines", "Depth",          "ScanRate",          // 18 - 20
                        "ExtractionStatus",
                        "ExtractionScanLineStart",
                        "ExtractionScanLineEnd",
                        NULL
                       };


  while  (parser.MoreTokens ())
  {
    KKStr  fieldName = parser.GetNextToken ("\t");
    int32  fieldIdx = SearchFieldIdx (fieldName, fieldNames);
    if  (fieldName.EqualIgnoreCase ("SipperFileId"))
      sipperFileId = parser.GetNextTokenInt ();

    else if  (fieldName.EqualIgnoreCase ("SipperFileName"))
      sipperFileName = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("CruiseName"))
      cruiseName = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("stationName"))
      stationName = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("deploymentNum"))
      deploymentNum = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("Description"))
      description = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("Latitude"))
      latitude = parser.GetNextTokenDouble ();

    else if  (fieldName.EqualIgnoreCase ("Longitude"))
      longitude = parser.GetNextTokenDouble ();

    else if  (fieldName.EqualIgnoreCase ("DateTimeStart"))
      dateTimeStart = parser.GetNextTokenDateTime ();

    else if  (fieldName.EqualIgnoreCase ("SP0"))
      sp0 = Instrument::LookUpByShortName (parser.GetNextToken ());

    else if  (fieldName.EqualIgnoreCase ("SP1"))
      sp1 = Instrument::LookUpByShortName (parser.GetNextToken ());

    else if  (fieldName.EqualIgnoreCase ("SP2"))
      sp2 = Instrument::LookUpByShortName (parser.GetNextToken ());

    else if  (fieldName.EqualIgnoreCase ("SP3"))
      sp3 = Instrument::LookUpByShortName (parser.GetNextToken ());

    else if  (fieldName.EqualIgnoreCase ("CtdExt0"))
      ctdExt0 = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("CtdExt1"))
      ctdExt1 = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("CtdExt2"))
      ctdExt2 = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("CtdExt3"))
      ctdExt3 = parser.GetNextToken ();

    else if  (fieldName.EqualIgnoreCase ("SizeInBytes"))
      sizeInBytes = parser.GetNextTokenLong ();

    else if  (fieldName.EqualIgnoreCase ("NumScanLines"))
      numScanLines = parser.GetNextTokenInt ();

    else if  (fieldName.EqualIgnoreCase ("Depth"))
      depth = parser.GetNextTokenFloat ();

    else if  (fieldName.EqualIgnoreCase ("ScanRate"))
      scanRate = parser.GetNextTokenFloat ();

    else if  (fieldName.EqualIgnoreCase ("ExtractionStatus"))
      extractionStatus = parser.GetNextTokenChar ();

    else if  (fieldName.EqualIgnoreCase ("ExtractionScanLineStart"))
      extractionScanLineStart = parser.GetNextTokenUint ();

    else if  (fieldName.EqualIgnoreCase ("ExtractionScanLineEnd"))
      extractionScanLineEnd = parser.GetNextTokenUint ();
  }

  return;
}  /* ParseTabDelStr */



void   SipperFile::ReFresh (const SipperFile&  sf)
{
  sipperFileId    =  sf.sipperFileId;
  sipperFileName  =  sf.sipperFileName;
  cruiseName      =  sf.cruiseName;
  stationName     =  sf.stationName;
  deploymentNum   =  sf.deploymentNum;
  description     =  sf.description;
  latitude        =  sf.latitude;
  longitude       =  sf.longitude;
  dateTimeStart   =  sf.dateTimeStart;
   
  sp0             =  sf.sp0;
  sp1             =  sf.sp1;
  sp2             =  sf.sp2;
  sp3             =  sf.sp3;

  ctdExt0         =  sf.ctdExt0;
  ctdExt1         =  sf.ctdExt1;
  ctdExt2         =  sf.ctdExt2;
  ctdExt3         =  sf.ctdExt3;
  
  sizeInBytes     =  sf.sizeInBytes;
  numScanLines    =  sf.numScanLines;
  scanRate        =  sf.scanRate;
  depth           =  sf.depth;

  extractionStatus        = sf.extractionStatus;
  extractionScanLineStart = sf.extractionScanLineStart;
  extractionScanLineEnd   = sf.extractionScanLineEnd;

}  /* ReFresh */



void   SipperFile::AssignCtdExternalInstruments (const KKStr&   rootName)
{
  // Ideally I should be getting these assignments from the SipperFiles table in the database.
  // but if not available so we have to use prior knowledge to make best guess assignments.
  KKStr  fullFileName = InstrumentDataFileManager::GetFullSipperFileName (rootName);

  DateTime  sipperFileTimeStamp  = osGetFileDateTime (fullFileName);
  long long  fileSizeInBytes = osGetFileSize (fullFileName);
  SizeInBytes (fileSizeInBytes);
  DateTimeStart (sipperFileTimeStamp);

  if  (rootName.SubStrPart (0, 3) == "hrs02")
  {
    // These are SIPER2 files that do not contain instrument data.
    CruiseName ("HRS02");
    StationName ("1");
    DeploymentNum (rootName.SubStrPart (5, 6));
    Sp0 (NULL);
    Sp1 (NULL);
    Sp2 (NULL);
    return;
  }

  Sp0 (Instrument::LookUpByShortName ("CTD"));
  Sp1 (Instrument::LookUpByShortName ("P-R"));
  Sp2 (Instrument::LookUpByShortName ("BAT"));

  if  (sipperFileName.SubStrPart (0, 2) == "Sta")
  {
    AssignCtdExternalInstrumentsETP2007 (rootName);
    return;
  }

  if  ((sipperFileName.SubStrPart (0,6) == "ETP2008")  ||  (sipperFileName.SubStrPart (0,4) == "ETP08"))
  {
    AssignCtdExternalInstrumentsETP2008 (rootName);
    return;
  }

  if  (rootName.SubStrPart (0, 4) == "B0710")
  {
    CruiseName ("B0710");
    if  (rootName.SubStrPart (0, 10) == "B0710ST004A")
    {
      StationName ("ST004A");
      DeploymentNum ("");
    }
    else if  (rootName.SubStrPart (0, 10) == "B0710ST004B")
    {
      StationName ("ST004B");
      DeploymentNum ("");
    }
    else if  (rootName.SubStrPart (0, 10) == "B0710ST017A")
    {
      StationName ("ST017");
      DeploymentNum ("");
    }
    else
    {
      StationName (rootName.SubStrPart (5, 9) == "B0710ST017A");
      DeploymentNum ("");
    }
    CtdExt0 ("TRN");
    CtdExt1 ("OXG");
    CtdExt2 ("FLO");
    CtdExt3 ("TUR");
    return;
  }

  else
  {
    CruiseName (rootName.SubStrPart (0, 3));
    StationName ("1");
    DeploymentNum ("A");
  }

  // All other files
  CtdExt0 ("OXG");
  CtdExt1 ("");
  CtdExt2 ("TRN");
  CtdExt3 ("FLO");

  return;
}  /* AssignCtdExternalInstruments */



void   SipperFile::AssignCtdExternalInstrumentsETP2007 (const KKStr&  rootName)
{
  CruiseName ("ETP2007");
  StationName ("1");
  DeploymentNum ("A");
  CtdExt0 ("CDM");
  CtdExt1 ("OXG");
  CtdExt2 ("FLO");
  CtdExt2 ("TUR");
}  /* AssignCtdExternalInstrumentsETP2007 */


void   SipperFile::AssignCtdExternalInstrumentsETP2008 (const KKStr&   rootName)
{
  CruiseName ("ETP2008");
  StationName ("1");
  DeploymentNum ("A");
  CtdExt0 ("OXG");
  CtdExt1 ("TRN");
  CtdExt2 ("FLO");
  if  (rootName.Len () > 9)
  {
    if  (rootName.SubStrPart (0, 7) == "ETP08_1A_01")
    {
      StationName ("1");
      DeploymentNum ("A");
    }

    else if  (rootName.SubStrPart (0, 8) == "ETP2008_1")
    {
      KKStr  deploymentNum = rootName.SubStrPart (9, 9);
      StationName ("1");
      DeploymentNum (deploymentNum);
    }

    else if  (rootName.SubStrPart (0, 8) == "ETP2008_8")
    {
      KKStr  deploymentNum = rootName.SubStrPart (9);
      deploymentNum = deploymentNum.ExtractToken ("_");
      StationName ("8");
      DeploymentNum (deploymentNum);
    }

    else if  (rootName.SubStrPart (0, 9) == "ETP2008_4a")
      StationName ("4a");

    else if  (rootName.SubStrPart (0, 9) == "ETP2008_4b")
      StationName ("4b");
  }

  if  ((rootName == "ETP08_1A_01")     ||  
       (rootName == "ETP08_1A_02")     ||  
       (rootName == "ETP2008_1A_01")   ||  
       (rootName == "ETP2008_1A_02")   ||  
       (rootName == "ETP2008_1AA_01")  ||
       (rootName == "ETP2008_1AA_02")  ||
       (rootName == "ETP2008_1AA_03")  ||
       (rootName == "ETP2008_1AA_04")  ||
       (rootName == "OnDeckTest_02")
      )
  {
    CtdExt0 ("TRN");
    CtdExt1 ("OXG");
    CtdExt2 ("FLO");
    CtdExt3 ("TUR");
    return;
  }

  if  (rootName == "ETP2008_4a_01")
  {
    CtdExt0 ("TRN");
    CtdExt1 ("");
    CtdExt2 ("FLO");
    CtdExt3 ("TUR");
    return;
  }

  if  (rootName.Len () > 10)
  {
    if  ((rootName.SubStrPart (0, 9)  == "ETP2008_1B")    ||  
         (rootName.SubStrPart (0, 9)  == "ETP2008_1C")    ||
         (rootName.SubStrPart (0, 9)  == "ETP2008_1D")    ||
         (rootName.SubStrPart (0, 9)  == "ETP2008_1E")    ||
         (rootName                    == "ETP2008_1E_01")
        )
    {
      CtdExt0 ("OXG");
      CtdExt1 ("");
      CtdExt2 ("FLO");
      CtdExt3 ("TUR");
      return;
    }

    if  (rootName.SubStrPart (0, 6) == "ETP2008")
    {
      CtdExt0 ("TRN");
      CtdExt1 ("OXG");
      CtdExt2 ("FLO");
      CtdExt3 ("TUR");
      return;
    }
  }
}  /* AssignCtdExternalInstrumentsETP2008 */






SipperFileList::SipperFileList (bool  _owner):
    KKQueue<SipperFile> (_owner)
{
}


SipperFileList::~SipperFileList ()
{
}




SipperFileListPtr  SipperFileList::GetListOfSipperFilesWeCanFind (RunLog&  runLog)
{
  VectorKKStr sipperFileNames = InstrumentDataFileManager::GetListOfSipperFiles (runLog);

  SipperFileListPtr  sipperFiles = new SipperFileList (true);

  VectorKKStr::iterator  idx;
  for  (idx = sipperFileNames.begin ();  idx != sipperFileNames.end ();  idx++)
  {
    KKStr  rootName = osGetRootName (*idx);
    SipperFilePtr  sf = new SipperFile (rootName);
    sf->Description (*idx);
    sipperFiles->PushOnBack (sf);
  }

  return  sipperFiles;
}  /* GetListOfSipperFilesWeCanFind */




void  SipperFileList::Save (const KKStr&  fileName)
{
  ofstream  o (fileName.Str ());

  o << "// SipperFileList::Save" << endl
    << "// Date  [" << osGetLocalDateTime () << "]" << endl
    << "// Count [" << QueueSize ()          << "]" << endl
    << endl;

  SipperFileList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    o << "SipperFile" << "\t" << (*idx)->ToTabDelStr () << endl;
  }

  o.close ();
}  /* Save */



SipperFilePtr  SipperFileList::LookUpBySipperFileName (const KKStr&  sipperFileName)
{
  KKStr  rootName = osGetRootName (sipperFileName);

  SipperFileList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    SipperFilePtr  sf = *idx;
    if  (sf->SipperFileName ().EqualIgnoreCase (rootName))
      return sf;
  }

  return NULL;
}  /* LookUpBySipperFileName */



SipperFileListPtr  SipperFileList::Load (const KKStr&  fileName)
{
  ifstream  i (fileName.Str ());
  if  (!i.is_open ())
    return  NULL;

  SipperFileListPtr  files = new SipperFileList (true);

  char  buff[40960];

  while  (!i.eof ())
  {
    memset (buff, 0, sizeof (buff));
    i.getline (buff, sizeof (buff));
    KKStrParser parser (buff);

    if  (parser.SubStrPart (0, 1) == "//")
      continue;

    KKStr  lineName = parser.GetNextToken ();
    if  (lineName.EqualIgnoreCase ("SipperFile"))
    {
      SipperFilePtr  sf = new SipperFile (parser);

      SipperFilePtr  existingSipperFile = files->LookUpBySipperFileName (sf->SipperFileName ());
      if  (existingSipperFile)
      {
        existingSipperFile->ReFresh (*sf);
        delete  sf;
        sf = NULL;
      }
      else
      {
        files->PushOnBack (sf);
      }
    }
  }

  i.close ();

  return  files;
}   /* Load */



SipperFilePtr  SipperFileOracle::GetSipperFileEntry (const KKStr& sipperFileRootName)
{
  SipperFilePtr  result = NULL;

  GlobalGoalKeeper::StartBlock ();

  map<KKStr,SipperFilePtr>::iterator  idx;
  idx = allSipperFiles.find (sipperFileRootName);
  if  (idx != allSipperFiles.end ())
    result = idx->second;

  GlobalGoalKeeper::EndBlock ();

  return  result;
}  /* GetSipperFileEntry */



void  SipperFileOracle::AddSipperFileEntry (SipperFilePtr  sipperFile)
{
  GlobalGoalKeeper::StartBlock ();

  map<KKStr,SipperFilePtr>::iterator  idx;
  idx = allSipperFiles.find (sipperFile->SipperFileName ());
  if  (idx == allSipperFiles.end ())
  {
    allSipperFiles.insert (pair<KKStr,SipperFilePtr> (sipperFile->SipperFileName (), sipperFile));
  }
  else
  {
    delete  idx->second;
    idx->second = sipperFile;
  }

  GlobalGoalKeeper::EndBlock ();
}  /* AddSipperFileEntry */




void  SipperFileOracle::AddSipperFileEntries (SipperFileListPtr  sipperFiles)
{
  SipperFileList::iterator  idx;

  for  (idx = sipperFiles->begin ();  idx != sipperFiles->end ();  ++idx)
  {
    SipperFilePtr  sf = *idx;

    if  (sipperFiles->Owner ())
      AddSipperFileEntry (sf);
    else
      AddSipperFileEntry (new SipperFile (*sf));
  }

  sipperFiles->Owner (false);
}  /* AddSipperFileEntries */


map<KKStr,SipperFilePtr>  SipperFileOracle::allSipperFiles;
