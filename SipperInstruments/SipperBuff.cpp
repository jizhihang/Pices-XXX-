#include  "FirstIncludes.h"
#include  <stdio.h>
#include  <errno.h>
#include  <string.h>
#include  <ctype.h>

#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"

using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace KKU;



#include  "SipperBuff.h"

#include  "InstrumentDataManager.h"
#include  "SipperBuff3Bit.h"
#include  "SipperBuff4Bit.h"
#include  "SipperBuffBinary.h"
#include  "Sipper3Buff.h"        // Came out with SIPPER III
#include  "Sipper3RevBuff.h"     // Same as Sipper III, but accounts for some wires 
                                 // being swapped, as per Bill Flanery.
#include  "SipperBuff4Bit.h"
#include  "SipperHeaderRec.h"
#include  "SipperVariables.h"
using namespace SipperHardware;


SipperBuff::SipperBuff (InstrumentDataManagerPtr  _instrumentDataManager,
                        RunLog&                   _log
                       ):

  bytesDropped              (0),
  byteOffset                (0),
  curRow                    (0),
  eof                       (false),
  fileName                  (),
  iOwnInstrumentDataManager (false),
  inFile                    (NULL),
  instrumentDataManager     (_instrumentDataManager),
  invalidLine               (false),
  log                       (_log),
  opened                    (false),
  recCount                  (0)

{
}



SipperBuff::SipperBuff (const KKStr&             _fileName,
                        InstrumentDataManagerPtr  _instrumentDataManager,
                        RunLog&                   _log
                       ):

  bytesDropped              (0),
  byteOffset                (0),
  curRow                    (0),
  eof                       (false),
  fileName                  (_fileName),
  iOwnInstrumentDataManager (false),
  inFile                    (NULL),
  instrumentDataManager     (_instrumentDataManager),
  invalidLine               (false),
  log                       (_log),
  opened                    (false),
  recCount                  (0)

{
  Open (fileName); 
}



SipperBuff::~SipperBuff ()
{
  if  (opened)
  {
    fclose (inFile);
    opened = false;
  }

  if  (iOwnInstrumentDataManager)
  {
    delete  instrumentDataManager;
    instrumentDataManager = NULL;
  }
}



void  SipperBuff::Reset ()
{
  if  (!opened)
    return;

  curRow       = 0;
  eof          = false;  
  recCount     = 0;
  bytesDropped = 0;
  rewind (inFile);
}  /* Reset */



void  SipperBuff::AssignSerialPorts (const SipperHeaderRecPtr  sipperHeaderRec)
{
  if  (instrumentDataManager)
    instrumentDataManager->AssignSerialPorts (sipperHeaderRec);
}   /* AssignSerialPorts */




void  SipperBuff::Open (const KKStr&  _fileName)
{
  eof = false;
  fileName = _fileName;

  inFile = osFOPEN (fileName.Str (), "rb");

  if  (inFile)
  {
    opened = true;
  }
  else
  {
    opened = false;
    cerr << "***ERROR***, SipperBuff Opening File[" << fileName << "]" << endl;
    //exit (-1);
  }
}  /* Open */



void  SipperBuff::ResetByteOffset (uint32  _byteOffset)
{
  if  (!inFile)
  {
    cerr << endl << endl
         << "SipperBuff::ResetByteOffset    SIPPER file is not opened." << endl
         << endl;
    return;
  }

#if defined(_MSC_VER)
  _fseeki64 (inFile, _byteOffset, SEEK_SET);
#else
  fseek (inFile, (int32)_byteOffset, SEEK_SET);
#endif

  byteOffset = _byteOffset;
}  /* ResetByteOffset */



void  SipperBuff::SkipOneByte ()
{
  if  (!inFile)
  {
    cerr << endl << endl
         << "SipperBuff::ResetByteOffset    SIPPER file is not opened." << endl
         << endl;
    return;
  }

  char  oneByte;
  fread (&oneByte, 1, 1, inFile);
  byteOffset++;
}  /* SkipOneByte */




void  SipperBuff::SkipToScanLine (uint32  scanLine)
{
  uchar*  lineBuff = new uchar[5000];
  uint32  lineSize;
  uint32  colCount[5000];
  uint32  pixelsInRow;
  bool    flow;
  
  lineSize = 1;

  while  ((curRow < scanLine) &&  (lineSize > 0))
  {
    GetNextLine (lineBuff, 5000, lineSize, colCount, pixelsInRow, flow);
  }

  delete  lineBuff;
  lineBuff = NULL;
}  /* SkipToScanLine */





void  SipperBuff::SkipToScanLine (uint32  _scanLine,
                                  uint64  _byteOffset
                                 )
{
#if defined(_MSC_VER)
  int32  returnCd = _fseeki64 (inFile, _byteOffset, SEEK_SET);
#else
  int32  returnCd = fseek (inFile, _byteOffset, SEEK_SET);
#endif


//int32 _fseeki64( 
//   FILE *stream,
//   __int64 offset,
//   int32 origin 
//);

  if  (returnCd != 0)
  {
    eof = true;
    return;
  }

  curRow = _scanLine - 1;
  byteOffset = _byteOffset;
};


#ifdef  WIN32
struct  SipperBuff::SipperRec
{
    uchar pix3      : 1;
    uchar pix2      : 1;
    uchar pix1      : 1;
    uchar pix0      : 1;

    uchar flow      : 1;  // 0 = Flow Meter in Position 0
                          // 1 = Flow Meter in Position 1

    uchar raw       : 1;  // 1 = Data is Raw 
                          // 0 = Compressed Run-Length

    uchar eol       : 1;  // 1 = End of Line Encountered, 
                          // 0 = End of Line not Encountered

    uchar cameraNum : 1;

    uchar pix11     : 1;
    uchar pix10     : 1;
    uchar pix9      : 1;
    uchar pix8      : 1;
    uchar pix7      : 1;
    uchar pix6      : 1;
    uchar pix5      : 1;
    uchar pix4      : 1;
};


#else


struct SipperBuff::SipperRec
{
    uchar cameraNum : 1;

    uchar eol       : 1;  // 1 = End of Line Encountered, 
                          // 0 = End of Line not Encountered

    uchar raw       : 1;  // 1 = Data is Raw 
                          // 0 = Compressed Run-Length

    uchar flow      : 1;  // 0 = Flow Meter in Position 0
                          // 1 = Flow Meter in Position 1

    uchar pix0      : 1;
    uchar pix1      : 1;
    uchar pix2      : 1;
    uchar pix3      : 1;

    uchar pix4      : 1;
    uchar pix5      : 1;
    uchar pix6      : 1;
    uchar pix7      : 1;
    uchar pix8      : 1;
    uchar pix9      : 1;
    uchar pix10     : 1;
    uchar pix11     : 1;
};
#endif



void  SipperBuff::DisplaySipperRec (SipperBuff::SipperRec&  sr)
{
  char  bs[30];

  int32  numOfBlanks;

  bs[7] = sr.cameraNum ? 'c' : '-';
  bs[6] = sr.eol       ? 'e' : '-';
  bs[5] = sr.raw       ? 'r' : '-';
  bs[4] = sr.flow      ? 'f' : '-';
  bs[3] = sr.pix0   + '0';
  bs[2] = sr.pix1   + '0';
  bs[1] = sr.pix2   + '0';
  bs[0] = sr.pix3   + '0';

  bs[15] = sr.pix4  + '0';
  bs[14] = sr.pix5  + '0';
  bs[13] = sr.pix6  + '0';
  bs[12] = sr.pix7  + '0';
  bs[11] = sr.pix8  + '0';
  bs[10] = sr.pix9  + '0';
  bs[9]  = sr.pix10 + '0';
  bs[8]  = sr.pix11 + '0';

  bs[16] = 0;

  numOfBlanks = sr.pix0 * 2048 + sr.pix1  * 1024 + sr.pix2 * 512 +
                sr.pix3 * 256  + sr.pix4  * 128  + sr.pix5 * 64  +
                sr.pix6 * 32   + sr.pix7  * 16   + sr.pix8 * 8   +
                sr.pix9 * 4    + sr.pix10 * 2    + sr.pix11;

  cout <<  bs << "  " << numOfBlanks; 
  cout << endl; 
}





SipperFileFormat  SipperBuff::GuessFormatOfNextLine (FILE*    in,
                                                     int32    _cameraNum,
                                                     RunLog&  _log
                                                    )
{
  SipperFileFormat  fileFormat;

  size_t  recsRead;

  SipperRec   sipperRec;

  int32  oneBitLen   = 0;   // Count for Binary Sipper
  int32  fourBitLen  = 0;   // Count for GrayScale sipper  or Sipper II
  int32  numOfBlanks = 0;

  int32  sipperRecsRead = 0;

  do  {
    do {
      //recsRead = fread (&sipperRec, sizeof (sipperRec), 1, in);  
      // sizeof can not work with bit fields
      recsRead = fread (&sipperRec, SIPPERRECSIZE, 1, in);
      // DisplaySipperRec  (sipperRec);

      sipperRecsRead++;
      if  ((sipperRecsRead > 5000)  ||  (recsRead == 0))
      {
        return  sfUnKnown;
      }
    }  while  ((recsRead > 0)  &&  (sipperRec.cameraNum != _cameraNum));

    if  (sipperRec.raw == 0)
    {
      numOfBlanks = sipperRec.pix0 * 2048 + sipperRec.pix1  * 1024 + sipperRec.pix2 * 512 +
                    sipperRec.pix3 * 256  + sipperRec.pix4  * 128  + sipperRec.pix5 * 64  +
                    sipperRec.pix6 * 32   + sipperRec.pix7  * 16   + sipperRec.pix8 * 8   +
                    sipperRec.pix9 * 4    + sipperRec.pix10 * 2    + sipperRec.pix11;
      
      if  (numOfBlanks == 0)
        oneBitLen = oneBitLen + 4096;
      else
        oneBitLen = oneBitLen + numOfBlanks;

      fourBitLen = fourBitLen + numOfBlanks * 4;
    }
    else
    {
      fourBitLen = fourBitLen + 4;
      
      if  ((oneBitLen >= 4084)  &&  (oneBitLen < 4096))
        oneBitLen = 4096;
      else
        oneBitLen = oneBitLen + 12;
    }
  }  while  ((recsRead > 0)  &&  (sipperRec.eol == 0));
  
  if  (fourBitLen == 4096)
  {
    fileFormat = sf3Bit;
  }

  else if  ((oneBitLen == 2048)  ||  (oneBitLen == 4096))
  {
    fileFormat = sfBinary;
  }

  else
  {
    fileFormat = sfUnKnown;
  }

  return  fileFormat;
}  /*  GuessFormatOfFile */





SipperFileFormat  SipperBuff::GuessFormatOfFile (const KKStr&  _fileName,
                                                 int32         _cameraNum,
                                                 RunLog&       _log
                                                )
{
  // First we will see if file is the new SIPPER 3 format,  if it is not
  // then we will try to see in one of the other two formats.


  SipperFileFormat  fileFormat = sfUnKnown;
  SipperFileFormat  lastFormat;
  {
    Sipper3BuffPtr  s3 = new Sipper3Buff (_fileName, NULL, _log);
    if  (s3->Opened ())
    {
      if  (s3->FileFormatGood ())
        fileFormat = sfSipper3;
    }

    delete s3;
  }

  if  (fileFormat == sfSipper3)
  {
    // Since we know it is a SIPPER 3  file,  we are done and can return 
    return  sfSipper3;
  }

  {
    SipperBuff4BitPtr  sf = new SipperBuff4Bit (_fileName, NULL, _log);
    if  (sf->Opened ())
    {
      if  (sf->FileFormatGood ())
        fileFormat = sfSipper4Bit;
    }

    delete sf;
  }

  if  (fileFormat == sfSipper4Bit)
  {
    // Since we know it is a SIPPER 4bit  file,  we are done and can return 
    return  sfSipper4Bit;
  }



  FILE*  in = osFOPEN (_fileName.Str (), "rb");

  if  (!in)
  {
    int32  errorCode = errno;

    cerr << "***ERROR[" << errorCode << "]***, SipperBuff::GuessFormatOfFile  Can not open file[" << _fileName << "]." 
         << endl;

    return  sfUnKnown;
  }

  int32  numInARow = 0;

  fileFormat = sfUnKnown;

  int32  outerTries = 0;

  while  ((numInARow < 3)  &&  (outerTries < 2))
  {
    int32  numOfTries = 0;
    rewind (in);
    if  (outerTries > 0)
    {
      char  buff[20];
      fread (buff, 1, 1, in);
    }

    while  ((numInARow < 3)  &&  (numOfTries < 2000))
    {
      lastFormat = fileFormat;
      fileFormat = GuessFormatOfNextLine (in, _cameraNum, _log);
      numOfTries++;
      // WaitForEnter ();

      if  ((fileFormat == sfUnKnown)  ||  (lastFormat != fileFormat))
      {
        numInARow = 0;
      }
      else
      {
        numInARow++;
      }
    }
    outerTries++;
  }

  fclose (in);

  return  fileFormat;
}  /*  GuessFormatOfFile */




SipperBuffPtr  SipperBuff::CreateSipperBuff (KKStr                     _fileName,
                                             int32                     _cameraNum,
                                             InstrumentDataManagerPtr  _instrumentDataManager,
                                             RunLog&                   _log
                                            )
{
  SipperFileFormat  format = GuessFormatOfFile (_fileName, _cameraNum, _log);

  return  CreateSipperBuff (format, _fileName, _cameraNum, _instrumentDataManager, _log);
}  /* CreateSipperBuff */




SipperBuffPtr  SipperBuff::CreateSipperBuff (SipperFileFormat          _format,
                                             KKStr                     _fileName,
                                             int32                     _cameraNum,
                                             InstrumentDataManagerPtr  _instrumentDataManager,
                                             RunLog&                   _log
                                            )
{
  SipperBuffPtr  sipperBuff = NULL;

  if  (_format == sfUnKnown)
  {
    // We are going to have to figre out the format 
    _format = GuessFormatOfFile (_fileName, _cameraNum, _log);
  }

  if  (_format == sfBinary)
    sipperBuff = new SipperBuffBinary (_fileName, _cameraNum, _instrumentDataManager, _log);

  else if  (_format == sf3Bit)
    sipperBuff = new SipperBuff3Bit   (_fileName, _instrumentDataManager, _log);

  else if  (_format == sfSipper3)
    sipperBuff = new Sipper3Buff      (_fileName, _instrumentDataManager, _log);

  else if  (_format == sfSipper4Bit)
    sipperBuff = new SipperBuff4Bit   (_fileName, _instrumentDataManager, _log);

  else if  (_format == sfSipper3)
    sipperBuff = new Sipper3Buff      (_fileName, _instrumentDataManager, _log);

  else if  (_format == sfSipper3Rev)
    sipperBuff = new Sipper3RevBuff   (_fileName, _instrumentDataManager, _log);

  if  ((sipperBuff != NULL)  &&  (!sipperBuff->Opened ()))
  {
    delete  sipperBuff;
    sipperBuff = NULL;
  }

  return  sipperBuff;
}  /* CreateSipperBuff */




void  SipperBuff::DetermineCropSettings  (uint32&  cropLeft,
                                          uint32&  cropRight
                                         )
{
  int  maxLineSize = 5000;
  uchar*  lineBuff         = new uchar[maxLineSize];
  uchar*  lineBuffSmoothed = new uchar[maxLineSize];
  uint32  lineSize;
  uint32  colCount[5000];
  uint32  pixelsInRow;
  bool    flow;
  
  lineSize = 1;

  int32  x = 0;

  uchar  maxVals[4096];
  uint32 totVals[4096];

  for  (x = 0;  x < 4096;  ++x)
  {
    maxVals[x] = 0;
    totVals[x] = 0;
  }

  vector<uchar>  smoothWindow(3, 0);

  // Skip fisr 50K lines
  for  (x = 0;  (x < 50000)  &&  (!eof);  ++x)
  {
    pixelsInRow = 0;
    GetNextLine (lineBuff, maxLineSize, lineSize, colCount, pixelsInRow, flow);
  }

  int  count = 0;
  pixelsInRow = 0;
  GetNextLine (lineBuff, maxLineSize, lineSize, colCount, pixelsInRow, flow);
  for  (x = 0;  (x < 300000)  &&  (!eof);  ++x)
  {
    ++count;

    uint32  y = 0;

    // Medium Smooth Line
    for  (y = 1;  y <  (4096 - 1);  ++y)
    {
      uchar sw0 = lineBuff[y - 1];
      uchar sw1 = lineBuff[y];
      uchar sw2 = lineBuff[y + 1];

      if  ((sw0 == sw1)  &&  (sw0 == sw2))
      {
        lineBuffSmoothed[y] = sw0;
      }
      else
      {
        smoothWindow[0] = sw0;
        smoothWindow[1] = sw1;
        smoothWindow[2] = sw2;
        sort (smoothWindow.begin (), smoothWindow.end ());
        lineBuffSmoothed[y] = smoothWindow[1];
      }
    }

    for  (y = 0;  y < 4096;  ++y)
    {
      maxVals[y] = Max (maxVals[y], lineBuffSmoothed[y]);
      //totVals[y] += lineBuffSmoothed[y];
    }

    GetNextLine (lineBuff, maxLineSize, lineSize, colCount, pixelsInRow, flow);
  }

  //vector<uint32>  avgVals (4096, 0);
  //for  (x = 0;  x < 4096;  ++x)
  //  avgVals[x] = totVals[x] / count;

  cropLeft = 1;
  while  ((cropLeft < 2500)  &&  (maxVals[cropLeft] < 50))
    ++cropLeft;

  cropRight = 4094;
  while  ((cropRight > 2000)  &&  (maxVals[cropRight] < 50))
    --cropRight;

  delete  lineBuff;          lineBuff         = NULL;
  delete  lineBuffSmoothed;  lineBuffSmoothed = NULL;
}  /* DetermineCropSettings */





void  SipperBuff::ReportInstrumentData (uchar  instrumentId,
                                        bool   text,
                                        uchar  data
                                       )
{
  if  (!instrumentDataManager)
  {
    // Will try to put Report directory in same directory with SIPPER file.

    KKStr  sipperFileDir = osGetPathPartOfFile (fileName);
    if  (sipperFileDir.Empty ())
      sipperFileDir = osGetCurrentDirectory ();
    KKStr  reportDir = osAddSlash (SipperVariables::PicesReportDir ()) + "InstrumentReports";

    instrumentDataManager = new InstrumentDataManager (fileName, NULL, reportDir, log);
    iOwnInstrumentDataManager = true;
  }

  instrumentDataManager->ReportInstrumentData (curRow, instrumentId, text, data);
}  /* ReportInstrumentData */



KKStr  SipperHardware::SipperFileFormatToStr (SipperFileFormat  fileFormat)
{
  if  (fileFormat == sfBinary)
    return  "Binary";

  if  (fileFormat == sf3Bit)
    return  "3Bit";

  if  (fileFormat == sfSipper3)
    return  "Sipper3";

  if  (fileFormat == sfSipper4Bit)
    return  "Sipper4Bit";

  if  (fileFormat == sfSipper3Rev)
    return  "Sipper3Rev";

  return  "";
}  /* SipperFileFormatToStr */



SipperFileFormat  SipperHardware::SipperFileFormatFromStr (KKStr  fileFormatStr)
{
  fileFormatStr.Upper ();

  if  ((fileFormatStr == "BINARY")      ||  (fileFormatStr == "1BIT"))
    return  sfBinary;

  if  ((fileFormatStr == "3BIT")        ||  (fileFormatStr == "SIPPER2"))
    return  sf3Bit;

  if  ((fileFormatStr == "SIPPER3")     ||  (fileFormatStr == "S3")  ||  (fileFormatStr == "3"))
    return  sfSipper3;

  if  (fileFormatStr.EqualIgnoreCase ("Sipper4Bit")  ||  (fileFormatStr == "S4")  ||  (fileFormatStr == "4"))
    return  sfSipper4Bit;

  if  ((fileFormatStr == "SIPPER3REV")  ||  (fileFormatStr == "SIPPER3REV")  ||  (fileFormatStr == "S3R"))
    return  sfSipper3Rev;

  return  sfUnKnown;
}  /* SipperFileFormatFromStr */



KKStr  SipperHardware::SipperFileAvailableOptions ()
{
  return  "Binary, 1Bit, 3Bit, Sipper2, Sipper3, S3, Sipper3Rev, S3R";
}



