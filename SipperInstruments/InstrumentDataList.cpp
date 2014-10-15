#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <math.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
using namespace std;


#include  "BasicTypes.h"
#include  "KKStrParser.h"


#include  "OSservices.h"
using namespace KKU;



#include  "InstrumentDataList.h"

#include  "InstrumentDataManager.h"
#include  "SipperBuff.h"
#include  "SipperFile.h"
using  namespace  SipperHardware;



InstrumentDataList::InstrumentDataList (bool  _owner):
   KKQueue<InstrumentData> (_owner),
   fileVersion           (0),
   scanRate              (24950.0f),
   sipperFileName        (),
   thereIsDepthData      (false),
   thereIsFlowRateData   (false)

{
}


InstrumentDataList::InstrumentDataList (const KKStr&  _fileName,
                                        bool&         _loadWasSucessfull,
                                        RunLog&       _log
                                       ):
   KKQueue<InstrumentData> (true),
   fileVersion           (0),
   scanRate              (24950.0f),
   sipperFileName        (),
   thereIsDepthData      (false),
   thereIsFlowRateData   (false)

{
  Load (_fileName, _loadWasSucessfull, _log);
}




InstrumentDataList::~InstrumentDataList ()
{
}




InstrumentDataListPtr  InstrumentDataList::CreateFromSipperFile (const KKStr&          fullSipperFileName,
                                                                 SipperFilePtr         sipperFile,
                                                                 volatile const bool&  cancelFlag,
                                                                 RunLog&               log
                                                                )
{
  log.Level (10) << "InstrumentDataList::CreateFromSipperFile    FullSipperFileName[" << fullSipperFileName << "]" << endl;

  KKStr  sipperFileName = osGetRootName (fullSipperFileName);


  InstrumentDataManagerPtr  instrumentDataManager 
    = new InstrumentDataManager (fullSipperFileName, sipperFile, log);

  SipperBuffPtr  sipperBuff = SipperBuff::CreateSipperBuff (sfUnKnown, 
                                                            fullSipperFileName, 
                                                            0,                    // Camera Number 0
                                                            instrumentDataManager,
                                                            log
                                                           );

  if  (!sipperBuff)
  {
    log.Level (-1) << endl << endl << "InstrumentDataList::CreateFromSipperFile     *** ERROR ***" << endl
                                   << "                 InstrumentDataList::CreateFromSipperFile  Opening File[" << fullSipperFileName << "]" << endl
                                   << endl;
    return  NULL;
  }

  uchar*  lineBuff = new uchar[4096];
  uint32  lineSize = 0;
  uint32*   colCount = new uint32[4096];
  uint32  pixelsInRow;
  bool    flow;

  int32 RecordingRateNumSamplingFrames = 2;

  InstrumentDataListPtr  data = new InstrumentDataList (true);
  data->SipperFileName (sipperFileName);

  float  scanRate = 24950.0f;
  if  (sipperFile)
  {
    // Unfortunatly;  do to poor designing on my part;  we have instances when 'sipperFile' is not defined.
    // in that case we will assume a ScanRate of 24950.0f.
    scanRate = sipperFile->ScanRate ();
    if  (scanRate < 1000)
      scanRate = 24950.0f;

    data->ScanRate (scanRate);
  }


  sipperBuff->GetNextLine (lineBuff,
                           4096,      // Size of 'LineBuff'
                           lineSize,
                           colCount,
                           pixelsInRow,
                           flow
                          );

  while  ((!sipperBuff->Eof ())  &&  (!cancelFlag))
  {
    if  ((sipperBuff->CurRow () % 4096) == 0)
    {
      InstrumentDataPtr  id = instrumentDataManager->CreateInstrumentData ();
      id->ByteOffset (sipperBuff->CurRowByteOffset ());
      id->ScanLine   (sipperBuff->CurRow ());

      if  (data->QueueSize () > 0)
      {
        int32  frameNumPrev  = data->QueueSize () - 1 - RecordingRateNumSamplingFrames;
        if  (frameNumPrev < 0)
          frameNumPrev = 0;

        InstrumentDataPtr  prevID = data->IdxToPtr (frameNumPrev);

        uint32 scanLineRow     = sipperBuff->CurRow ();
        uint32 scanLineRowPrev = prevID->ScanLine ();
        uint32 scanLinesDelta  = scanLineRow - scanLineRowPrev;

        uint64  byteOffset      = sipperBuff->ByteOffset ();
        uint64  byteOffsetPrev  = prevID->ByteOffset ();
        uint64  byteOffsetDelta = byteOffset - byteOffsetPrev;

        double  secsDelta = (double)scanLinesDelta / (double)scanRate;

        double  bytesDeltaMega = (double)byteOffsetDelta / 1000000.0;
              
        float  recordRate = (float)(bytesDeltaMega / secsDelta);
        id->RecordRate (recordRate);
      }

      if  (id->Depth () != 0.0)
          data->ThereIsDepthData (true);

      if  (id->FlowRate1 () > 0.0)
        data->ThereIsFlowRateData (true);

      data->PushOnBack (id);
    }

    if  ((sipperBuff->CurRow () % 300000) == 0)
      cout << sipperFileName << "\t"  << StrFormatInt (sipperBuff->CurRow (), "ZZZ,ZZZ,ZZ0") << endl;

    sipperBuff->GetNextLine (lineBuff,
                             4096,      // Size of 'LineBuff'
                             lineSize,
                             colCount,
                             pixelsInRow,
                             flow
                            );
  }

  if  (!cancelFlag)
  {
    InstrumentDataList::iterator idx;
    InstrumentDataPtr  firstValidRow = NULL;
    InstrumentDataPtr  id = NULL;
    
    for  (idx = data->begin ();  (idx != data->end ()) &&  (!firstValidRow);  idx++)
    {
      id = *idx;
      if  ((id->CtdDate ().Year () > 2005)  &&
           (id->Temperature () > 0.0f)      && 
           (id->Temperature () < 40.0f)     &&
           (id->Depth       () > 0.0f)      &&
           (id->Depth       () < 1000.0f)   &&
           (id->Salinity    () >= -2.0f)    &&
           (id->Salinity    () <= 40.0f)
          )
         firstValidRow = id;
    }

    if  (firstValidRow)
    {
      for  (idx = data->begin ();  idx != data->end ();  idx++)
      {
        id = *idx;
        if  (id->ScanLine () >= firstValidRow->ScanLine ())
          break;
        id->RefreshDataFields (*firstValidRow);
      }

      if  (data->FrontOfQueue ()->ScanLine () > 0)
      {
        InstrumentDataPtr  begRow = new InstrumentData (*firstValidRow);
        begRow->ScanLine (0);
        begRow->ByteOffset (0);
        data->PushOnFront (begRow);
      }
    }
  }

  delete  colCount;               colCount              = NULL;
  delete  lineBuff;               lineBuff              = NULL;
  delete  sipperBuff;             sipperBuff            = NULL;
  delete  instrumentDataManager;  instrumentDataManager = NULL;

  bool  changesMade = false;

  if  (cancelFlag)
  {
    delete  data;
    data = NULL;
  }
  else
  {
    data->MakeSureFlowRateAndDepthDataIsThere (changesMade);
  }

  //if  (sipperFile)
  //  data->MakeSureDepthDataIsThere (sipperFile,  changesMade);

  return  data;
}  /* CreateFromSipperFile */





void  InstrumentDataList::Load (const KKStr&  fileName,
                                bool&         _loadWasSucessfull,
                                RunLog&       log
                               )

{
  log.Level (10) << "InstrumentDataList::Load  [" << fileName << "]" << endl;

  _loadWasSucessfull = false;
  FILE*  f = osFOPEN (fileName.Str (), "r");
  if  (f == NULL)
  {
    log.Level (-1) << endl << endl << endl 
                   << "InstrumentDataList::Load      *** Error Opening File[" << fileName << "]" << endl
                   << endl;
    return;
  }

  int32  expectedCount = -1;

  VectorIntPtr  fieldIndirections = InstrumentData::CreateDefaultFieldIndirectionList ();

  char  buff[20480];

  while  (fgets (buff, sizeof (buff), f))
  {
    if  ((buff[0] == '/')  &&  (buff[1] == '/'))
      continue;

    KKStrParser  line (buff);

    KKStr  rowType = line.GetNextToken ("\t").ToLower ();
    rowType.TrimRight ();
    if  (rowType == "fieldlist")
    {
      delete  fieldIndirections;  fieldIndirections = NULL;

      // We have a line that defines the order of fields n the following lines
      fieldIndirections = InstrumentData::CreateFieldIndirectionVectorFromTabDelStr (line);
    }

    else if  (rowType == "sipperfilename")
    {
      sipperFileName = line.GetNextToken ("\t");
      sipperFileName.TrimRight ();
    }

    else if  (rowType == "version")
    {
      fileVersion = line.GetNextTokenInt ("\t");
      if  (fileVersion < InstrumentData::CurrentFileVersion ())
      {
        // Flag load as bad.
        _loadWasSucessfull = false;
        break;
      }
    }

    else if  (rowType == "count")
    {
      expectedCount = line.GetNextTokenInt ("\t");
    }

    else if  (rowType == "endoffile")
    {
      break;
    }

    else if  (rowType == "scanrate")
    {
      scanRate = (float)line.GetNextTokenDouble ("\t\n\r");
    }

    else if  (rowType == "data")
    {
      if  (fileVersion < InstrumentData::CurrentFileVersion ())
      {
        // Flag load as bad.
        _loadWasSucessfull = false;
        break;
      }

      InstrumentDataPtr  i = new InstrumentData (line, *fieldIndirections);
      if  (i->Depth () != 0.0)
        thereIsDepthData = true;

      if  (i->FlowRate1 () >= 0.0f)
        thereIsFlowRateData = true;
      PushOnBack (i);
    }
  }

  fclose (f);

  delete  fieldIndirections;  fieldIndirections = NULL;


  if  (fileVersion < InstrumentData::CurrentFileVersion ())
  {
    log.Level (-1) << endl << endl << "InstrumentDataList::Load      *** ERROR ***    File Version[" << fileVersion << "] out of date;  Expected Version[" << InstrumentData::CurrentFileVersion () << "]." << endl << endl;
    return;
  }

  if  (expectedCount <= 0)
  {
    log.Level (-1) << endl << endl << "InstrumentDataList::Load      *** ERROR ***     File Format is bad,  can not use.  'Count' was not specified." << endl << endl;
    return;
  }

  if  (QueueSize () != expectedCount)
  {
    log.Level (-1) << endl << endl 
                   << "InstrumentDataList::Load    *** ERROR ***     Incorrect number of items in data file"     << endl
                   << "                                              Expected[" << expectedCount << "]   found[" << QueueSize () << "]"  << endl
                   << endl;
    return;
  }
  
  {
    bool  changesMade = false;
    MakeSureFlowRateAndDepthDataIsThere (changesMade);
    if  (changesMade)
    {
      Save (fileName, log);
    }
  }

  _loadWasSucessfull = true;

}  /* Load */





void  InstrumentDataList::Save (const KKStr&  fileName,
                                RunLog&       log
                               )  const
{
  log.Level (10) << "InstrumentDataList::Save  [" << fileName << "]" << endl;
  int32  x;
  ofstream  o (fileName.Str ());
  if  (!o.is_open ())
  {
    log.Level (-1) << "InstrumentDataList::Save     *** Could not open file["  << fileName << "]";
    return;
  }
  
  o << "//  InstrumentData"    << endl
    << "//  SipperFileName: "  << sipperFileName        << endl
    << "//  SaveFileName  : "  << fileName              << endl
    << "//  DateSaved     : "  << osGetLocalDateTime () << endl
    << "//  ScanRate      : "  << scanRate              << endl
    << "//  Num of Entries: "  << QueueSize ()          << endl
    << "//"                                             << endl;

  o << "SipperFileName" << "\t" << sipperFileName << endl;
  o << "version"        << "\t" << InstrumentData::CurrentFileVersion () << endl;

  o << "FieldList";
  for  (x = 0;  x < InstrumentData::NumFields ();  x++)
    o << "\t" << InstrumentData::FieldName (x);
  o << endl;

  o << "Count" << "\t" << QueueSize () << endl;

  const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    InstrumentDataPtr id = *idx;
    o << "Data" << "\t" << id->ToTabDelStr () << endl;
  }
 
  o << "EndOfFile"  << endl;
  o.close ();
}  /* Save */





VectorDouble  InstrumentDataList::FrameOffsetsInMeters (uint32 scanLinesPerFrame,
                                                        float  scanRate,             // Scan lines per second.
                                                        float  defaultFlowRate
                                                       )
{
  const_iterator  idx;

  VectorDouble  frameOffsets;
  uint32  lastFrameScanLine = 0;
  uint32  nextFrameScanLine = lastFrameScanLine + scanLinesPerFrame;
  double  distTransversedCurFrame = 0.0;
  double  totalDist               = 0.0;
  frameOffsets.push_back (0.0);
  uint32  lastScanLineCalced = 0;

  idx = begin ();
  while  (idx != end ())
  {
    const InstrumentDataPtr  id = *idx;
    while  (id->ScanLine () >= nextFrameScanLine)
    {
      // This Istrumentdata record 'id' is beyond the end of th ecurrent frame.  In this case
      // we want to calc the distance traveled in this frame and add to 'frameOffsets'.
      uint32  scanLines = nextFrameScanLine - lastScanLineCalced;
      double  deltaTime = scanLines / scanRate;   // (ScanLines / ScanLines per Sec)

      float  flowRate = id->FlowRate1 ();
      if  (flowRate <= 0.0f)
        flowRate = defaultFlowRate;

      double  deltaDist = deltaTime * flowRate;
      distTransversedCurFrame += deltaDist;
      
      totalDist += distTransversedCurFrame;
      frameOffsets.push_back (totalDist);
      distTransversedCurFrame = 0.0;
      lastFrameScanLine  = nextFrameScanLine;
      nextFrameScanLine  = lastFrameScanLine + scanLinesPerFrame;
      lastScanLineCalced = lastFrameScanLine;
    }

    {
      // We now want to add scan lines that were not just added to frames in previous loop to 
      // 'distTransversedCurFrame'.
      uint32  scanLines = id->ScanLine () - lastScanLineCalced;
      double  deltaTime = scanLines / scanRate;   // (ScanLines / ScanLines per Sec)

      float  flowRate = id->FlowRate1 ();
      if  (flowRate <= 0.0f)
        flowRate = defaultFlowRate;

      double  deltaDist = deltaTime * flowRate;
      distTransversedCurFrame += deltaDist;
      lastScanLineCalced = id->ScanLine ();
    }
    
    idx++;
  }

  return  frameOffsets;
}  /* FramOffsetsInMeters */




InstrumentDataPtr  InstrumentDataList::SearchForNearestScanLine (uint32  scanLine)
{
  if  (QueueSize () < 1)
    return  NULL;

  InstrumentDataPtr  id = NULL;
  uint32  lowerBound = 0;
  uint32  upperBound = (uint32)QueueSize () - 1;
  uint32  mid  = 0;

  uint32  lowerScanLine = 0;
  uint32  upperScanLine = 999999999;
  uint32  midScanLine   = 0;

  id = IdxToPtr (0);
  lowerScanLine = id->ScanLine ();
  if  (scanLine < lowerScanLine)
    return  id;

  id = IdxToPtr (QueueSize () - 1);
  upperScanLine = id->ScanLine ();
  if  (scanLine > upperScanLine)
     return  id;
  

  while  (lowerBound < upperBound)
  {
    mid = (lowerBound + upperBound) / 2;
    id = IdxToPtr (mid);

    midScanLine = id->ScanLine ();

    if  (scanLine > midScanLine)
    {
      lowerBound = mid + 1;
    }
    else if  (scanLine < midScanLine)
    {
      upperBound = mid - 1;
    }
    else
    {
      return id;
    }
  }

  if  (scanLine > midScanLine)
  {
    InstrumentDataPtr  midPlusOne = IdxToPtr (mid + 1);
    uint32  deltaMid        = (scanLine - midScanLine);
    uint32  deltaMidPlusOne = (midPlusOne->ScanLine () - scanLine);

    if  (deltaMid < deltaMidPlusOne)
      return  id;
    else
      return  midPlusOne;
  }

  else if  (scanLine < midScanLine)
  {
    InstrumentDataPtr  midLessOne = IdxToPtr (mid - 1);
    uint32  deltaMid        = (midScanLine - scanLine);
    uint32  deltaMidLessOne = (scanLine - midLessOne->ScanLine ());

    if  (deltaMid < deltaMidLessOne)
      return  id;
    else
      return  midLessOne;
  }

  return  id;
}  /* GetNearestScanLine */




VectorFloat  InstrumentDataList::GetVolumePerMeterProfile (SipperFilePtr   sipperFile,
                                                           volatile bool&  cancelFlag,
                                                           RunLog&         log
                                                          )  const
{
  VectorFloat  volumePerMeter;

  if  (!sipperFile)
  {
    log.Level (-1) << endl << endl << endl
                   << "InstrumentDataList::GetVolumePerMeterProfile      ***ERROR***    ('sipperFile' = NULL)" << endl
                   << endl;
    return  volumePerMeter;
  }

  float  lastDepth = 0.0f;
  uint32  lastScanLine = 0;

  const_iterator  idx;

  float  scanRateToUse = sipperFile->ScanRate ();
  if  (scanRateToUse < 1.0f)
  {
    scanRateToUse = scanRate;
    if  (scanRateToUse < 1.0f)
      scanRateToUse = 24950;
  }


  for  (idx = begin ();  (idx != end ())  &&  (!cancelFlag);  idx++)
  {
    const InstrumentDataPtr  id = *idx;
    float  depth = id->Depth ();
    uint32  scanLine = id->ScanLine ();

    uint32  deltaScanLines = scanLine - lastScanLine;
    float  flowRate = id->FlowRate1 ();

    uint32 depthIndex = (int32)lastDepth;

    while  (volumePerMeter.size () <= depthIndex)
      volumePerMeter.push_back (0.0f);

    double  distTraveled = (double)(deltaScanLines * flowRate) / (double)scanRateToUse;
    double  volume = (0.098) * (0.098) * distTraveled;   // SIPPER tube in 9.8 x 9.8 cm's square

    volumePerMeter[depthIndex] += (float)volume;

    lastScanLine = scanLine;
    lastDepth    = depth;
  }

  return  volumePerMeter;
}  /* GetVolumePerMeterProfile */




VectorUlong  InstrumentDataList::GetScanLinesPerMeterProfile ()  const
{
  VectorUlong   scanLinesPerMeter;

  float  lastDepth = 0.0f;
  uint32  lastScanLine = 0;

  const_iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    const InstrumentDataPtr  id = *idx;
    float  depth = id->Depth ();
    uint32  scanLine = id->ScanLine ();

    uint32  deltaScanLines = scanLine - lastScanLine;

    uint32 depthIndex = (int32)lastDepth;

    while  (scanLinesPerMeter.size () <= depthIndex)
      scanLinesPerMeter.push_back (0);

    scanLinesPerMeter[depthIndex] += deltaScanLines;

    lastScanLine = scanLine;
    lastDepth    = depth;
  }

  return  scanLinesPerMeter;
}  /* GetScanLinesPerMeterProfile */




void  InstrumentDataList::MakeSureFlowRateAndDepthDataIsThere (bool&  changesMade)
{
  // because isntrument data usually does not show up in a SIPPER file right away some data
  // fields might be falsly populated with '0's.  In that case we would like to use the 
  // first occurance of these fields are used to populate the earlier records.

  if  (QueueSize () < 1)
    return;

  changesMade = false;

  InstrumentDataPtr id = IdxToPtr (0);
  if  (((id->Depth     () == 0.0f)  &&  thereIsDepthData)    ||
       ((id->FlowRate1 () == 0.0f)  &&  thereIsFlowRateData) ||
       ((id->FlowRate2 () == 0.0f)  &&  thereIsFlowRateData)
      )
  {
    float  flowRate1 = 0.0f;
    float  flowRate2 = 0.0f;
    float  depth     = 0.0f;
    InstrumentDataList::iterator  idx;

    {
      // Find first occurence of flowRate and depth
      for  (idx = begin ();  idx != end ();  idx++)
      {
        id = *idx;
        if  ((depth == 0.0f)  &&  (id->Depth () != 0.0f))
          depth = id->Depth ();

        if  ((flowRate1 == 0.0f)  &&  (id->FlowRate1 () != 0.0f))
          flowRate1 = id->FlowRate1 ();

        if  ((flowRate2 == 0.0f)  &&  (id->FlowRate2 () != 0.0f))
          flowRate2 = id->FlowRate2 ();

        if  ((flowRate1 != 0.0f)  &&  (flowRate2 != 0.0f)  &&  (depth != 0.0f))
          break;
      }
    }

    if  ((depth != 0.0f)  ||  (flowRate1 != 0.0f)  ||  (flowRate2 != 0))
    {
      // Update the first Instrument Data objects that do not have
      // updated FlowRate or Depth.
      for  (idx = begin ();  idx != end ();  idx++)
      {
        id = *idx;

        bool  stayInLoop = false;
        if  (id->Depth () == 0.0f)
        {
          id->Depth (depth);
          stayInLoop = true;
          changesMade = true;
        }

        if  (id->FlowRate1 () == 0.0f)
        {
          id->FlowRate1 (flowRate1);
          stayInLoop = true;
          changesMade = true;
        }

        if  (id->FlowRate2 () == 0.0f)
        {
          id->FlowRate2 (flowRate2);
          stayInLoop = true;
          changesMade = true;
        }

        if  (!stayInLoop)
          break;
      }
    }
  }
}  /* MakeSureFlowRateAndDepthDataIsThere */




void  InstrumentDataList::MakeSureDepthDataIsThere (SipperFilePtr  sipperFile,  
                                                    bool&          changesMade
                                                   )
{
  if  (sipperFile  &&  (sipperFile->Depth () == 0.0f))
    return;

  bool  allDepthsAreZero = true;
  InstrumentDataList::iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    InstrumentDataPtr  i = *idx;
    if  (i->Depth () != 0.0f)
    {
      allDepthsAreZero = false;
      break;
    }
  }

  if  (allDepthsAreZero)
  {
    changesMade = true;
    for  (idx = begin ();  idx != end ();  idx++)
    {
      InstrumentDataPtr  i = *idx;
      i->Depth (sipperFile->Depth ());
    }
  }

  return;
}  /* MakeSureDepthDataIsThere */

