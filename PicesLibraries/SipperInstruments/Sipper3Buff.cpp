#include  "FirstIncludes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
using namespace std;


#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;



#include  "Sipper3Buff.h"
#include  "InstrumentDataManager.h"
#include  "SipperHeaderRec.h"
using namespace SipperHardware;



uchar  Sipper3Buff::ShadesFor3Bit[] = {0, 36, 73, 109, 146, 182, 219, 255};




Sipper3Buff::Sipper3Buff (InstrumentDataManagerPtr  _instrumentDataManager,
                          RunLog&                   _log
                         ):

  SipperBuff (_instrumentDataManager, _log),

  overflowPixels  (0),
  sipperHeaderRec ()

{
  byteOffset = 0;
  if  (Opened ())
    ExtractSipperHeaderInfo ();
}




Sipper3Buff::Sipper3Buff (const KKStr&              _fileName,
                          InstrumentDataManagerPtr  _instrumentDataManager,
                          RunLog&                   _log
                         ):

  SipperBuff      (_fileName, _instrumentDataManager, _log),
  overflowPixels  (0),
  sipperHeaderRec ()

{
  if  (Opened ())
    ExtractSipperHeaderInfo ();
}





Sipper3Buff::~Sipper3Buff ()
{
  //SipperBuff::~SipperBuff ();
}





void  Sipper3Buff::GetNextSipperRec (uint32&  spaceLeft,
                                     uchar&   cameraNum,
                                     bool&    imageData,
                                     bool&    raw,
                                     bool&    eol,
                                     bool&    grayScale,
                                     uchar*   pixels,       // Array of size 12
                                     uchar&   numPixels,    // num of pixels in pixels 
                                     uint32&  numOfBlanks,
                                     bool&    moreRecs
                                    )
{
  Sipper3Rec  sipperRec;
  size_t  recsRead;

  do {
    //recsRead =  fread (&sipperRec, sizeof (sipperRec), 1, inFile);
    recsRead =  fread (&sipperRec, SIPPERRECSIZE, 1, inFile);
    if  (recsRead < 1)
    {
      eof      = true;
      moreRecs = false;
      return;
    }

    //byteOffset = byteOffset + sizeof (sipperRec);
    byteOffset = byteOffset + SIPPERRECSIZE;
    cameraNum = 0;
    imageData   = sipperRec.imageData;
    raw         = sipperRec.raw;
    eol         = sipperRec.eol;
    grayScale   = sipperRec.grayScale;

    if  (!imageData)
    {
      uchar  sensorId   = sipperRec.raw       * 32 + 
                          sipperRec.grayScale * 16 + 
                          sipperRec.pix11     * 8  +
                          sipperRec.pix10     * 4  +
                          sipperRec.pix9      * 2  +
                          sipperRec.pix8;

      uchar  sensorData = sipperRec.pix7  * 128 +
                          sipperRec.pix6  * 64  +
                          sipperRec.pix5  * 32  +
                          sipperRec.pix4  * 16  +
                          sipperRec.pix3  * 8   +
                          sipperRec.pix2  * 4   +
                          sipperRec.pix1  * 2   +
                          sipperRec.pix0;

      ReportInstrumentData (sensorId, 
                            eol == 1,
                            sensorData
                           );
    }

  }  while  (!imageData);


  if  (!raw) 
  {
    numOfBlanks = 0;
    numOfBlanks = sipperRec.pix11 * 8192 + sipperRec.pix10 * 4096 + sipperRec.pix9 * 2048 +
                  sipperRec.pix8  * 1024 + sipperRec.pix7  *  512 + sipperRec.pix6 *  256 +
                  sipperRec.pix5  *  128 + sipperRec.pix4  *   64 + sipperRec.pix3 *   32 +
                  sipperRec.pix2  *   16 + sipperRec.pix1  *    8 + sipperRec.pix0 *    4;
    numPixels = 0;
  }
  else
  {
    if  (grayScale)
    {
      numPixels = 4;
      pixels[0] = sipperRec.pix11 * 4 + sipperRec.pix10 * 2 + sipperRec.pix9;
      pixels[1] = sipperRec.pix8  * 4 + sipperRec.pix7  * 2 + sipperRec.pix6;
      pixels[2] = sipperRec.pix5  * 4 + sipperRec.pix4  * 2 + sipperRec.pix3;
      pixels[3] = sipperRec.pix2  * 4 + sipperRec.pix1  * 2 + sipperRec.pix0;
    }
    else
    {
      numPixels = 12;
      pixels[0]  = sipperRec.pix11;
      pixels[1]  = sipperRec.pix10;
      pixels[2]  = sipperRec.pix9;
      pixels[3]  = sipperRec.pix9;
      pixels[4]  = sipperRec.pix7;
      pixels[5]  = sipperRec.pix6;
      pixels[6]  = sipperRec.pix5;
      pixels[7]  = sipperRec.pix4;
      pixels[8]  = sipperRec.pix3;
      pixels[9]  = sipperRec.pix2;
      pixels[10] = sipperRec.pix1;
      pixels[11] = sipperRec.pix0;

      // Since this is B/W data,
      // lets convert to   "0 for white"  and  "7 fo black"
      int32  x;
      for  (x = 0;  x < 12;  x++)
      {
        if  (pixels[x] > 0)
          pixels[x] = 7;
      }
    }
  }

  moreRecs = true;

  return;
}  /* GetNextSipperRec */





void  Sipper3Buff::GetNextLine (uchar*  lineBuff,
                                uint32  lineBuffSize,
                                uint32& lineSize,
                                uint32  colCount[],
                                uint32& pixelsInRow,
                                bool&   flow
                               )
{
  bool    imageData;
  uchar   cameraNum;
  bool    eol;
  bool    moreRecs;
  uint32  numOfBlanks;
  bool    exceededBuffLen = false;
  uint32  spaceLeft = lineBuffSize;   //  Number of pixels left on the scan line.
  bool    grayScale;
  bool    raw;

  uchar  pixels[12];
  uchar  numPixels;

  bool  exceededLineMsgPrinted = false;

  InvalidLine (false);

  curRowByteOffset = byteOffset;

  memset (lineBuff, 0, lineBuffSize);

  if  (overflowPixels > 0)
  {
    // Last time we were called a line overflow occurred.  In this case
    // we want to return back a line with a special color indicating
    // the size of the overflow.  As per Bill Flanery
    uint32  ofb = overflowPixels;
    if  (ofb > 4096)
      ofb = 4096;
      
    for  (uint32 x = 0;  x < ofb; x++)
      lineBuff[x] = 253;  // 253 is the color that indicates a line that the scan line length was exceeded.

    lineSize = overflowPixels;
    overflowPixels = 0;
    curRow++;
    return;
  }


  GetNextSipperRec (spaceLeft,
                    cameraNum, 
                    imageData,      raw,     eol,  grayScale,
                    pixels,         numPixels,
                    numOfBlanks,
                    moreRecs
                   );

  bool  notFinished = moreRecs;

  lineSize = 0;

  pixelsInRow = 0;

  bool pauseAtEol = false;

  while  (notFinished)
  {
    // Will keep on reading SIPPER records until we reach an end-of-line or end-of-file.
    if  (raw)
    {
      int32  x;
      for  (x = 0;  x < numPixels;  x++)
      {
        if  (spaceLeft > 0)
        {
          if  (pixels[x] > 0)  
          {
            lineBuff[lineSize] = ShadesFor3Bit[pixels[x]];
            colCount[lineSize]++;
            pixelsInRow++;
          }
          lineSize++;
          spaceLeft--;
        }
        else
        {
          overflowPixels++;
        }
      }
    }

    else
    {
      // We have a Run Length for Blanks
      if  (numOfBlanks > spaceLeft)
      {
        overflowPixels += (numOfBlanks - spaceLeft);

        int32 z = numOfBlanks - spaceLeft;
        if  ( z > 12)
        {
          // Something is very very wrong.
          if  (!exceededLineMsgPrinted)
          {
            cerr << "GetNextLine  ***ERROR***  "  << osGetRootName (FileName ()) << ":" << CurRow () << "  Length by more than 12 bytes." << endl;
            exceededLineMsgPrinted = true;
          }
        }

        InvalidLine (true);
        numOfBlanks = spaceLeft;
        exceededBuffLen = true;
      }

      lineSize = lineSize + numOfBlanks;
      spaceLeft = spaceLeft - numOfBlanks;
    }

    if  (exceededBuffLen)
    {
      exceededBuffLen = false;
      pauseAtEol = true;
    } 
  
    if  (eol)
    {
      notFinished = false;
    }
    else
    {
      GetNextSipperRec (spaceLeft, 
                        cameraNum, 
                        imageData, raw, eol, grayScale,
                        pixels,
                        numPixels,
                        numOfBlanks,
                        moreRecs
                       );
      notFinished = moreRecs;
    }
  }

  if  (lineSize < 4096)
  {
    // We have an underflow,  as per Bill Flanery, we want to fill
    // special color to act as an UnderFlow alarm.

    int32 z = lineSize;
    while  (z < 4096)
    {
      lineBuff[z] = 254;  // This will be flag to set pixel to red, indicating a line underflow.
      z++;
    }
    InvalidLine (true);
  }

  if  (lineSize > 4096)
  {
    cerr << endl
         << "Sipper3Buff::GetNextLine  LineSize["  << lineSize << "] exceeded 4096 ."  << endl
         << endl;
  }

  else if  ((lineSize == 0)  &&  (!eof))
  {
    lineSize = 1;
  }


  curRow++;
}  /* GetNextLine */




bool  Sipper3Buff::NextScanLineGood ()  // Used by SipperBuff to try and guess if
                                        // this is a Sipper3  file.
{
  uchar   cameraNum;
  bool    eol;
  bool    imageData;
  bool    moreRecs;
  uint32  numOfBlanks;
  bool    grayScale;
  bool    raw;

  uchar  pixels[12];
  uchar  numPixels;

  uint32  scanLineLen = 0;

  uint32  spaceLeft = 4096;

  bool  endOfLineFound = false;

  GetNextSipperRec (spaceLeft,
                    cameraNum, 
                    imageData,
                    raw,     
                    eol,  
                    grayScale,
                    pixels,         
                    numPixels,
                    numOfBlanks,
                    moreRecs
                   );

  while  (moreRecs  &&  (!endOfLineFound)  &&  (scanLineLen < 10000))
  {
    if  (imageData)
    {
      if  (raw)
      {
        if  (grayScale)
        {
          scanLineLen += 4;
          spaceLeft   -= 4;
        }
        else
        {
          scanLineLen += 12;
          spaceLeft   -= 12;
        }
      }
      else
      {
        // White Space Found.
        scanLineLen += numOfBlanks;
        spaceLeft   -= numOfBlanks;
      }
    }

    if  (eol)
    {
      endOfLineFound = true;
      if  (scanLineLen < 4096)
      {
        // The scan Line-Length should not be less than 4096 when eol is detected.
        return false;
      }

      else if  (scanLineLen > 4096)
      {
        return false;
      }
    }

    else if  (scanLineLen >= 4096)
    {
      // The scan line is already at least 4096 pixels int32,  
      // this scan line must be invalid.
      return  false;
    }

    else
    {
      GetNextSipperRec (spaceLeft,
                        cameraNum, 
                        imageData,
                        raw,     
                        eol,  
                        grayScale,
                        pixels,         
                        numPixels,
                        numOfBlanks,
                        moreRecs
                       );
    }
  }

  curRow++;

  return  true;
}  /* NextScanLineGood */





bool  Sipper3Buff::FileFormatGood ()
{
  if  (!Opened ())
    return false;

  bool  validSipper3File = false;

  Reset ();

  int32  numInARowThatAreGood = 0;
  int32  numInARowThatAreBad  = 0;
  int32  numRead             = 0;

  do  {
    bool  isNextLineGood = NextScanLineGood ();

    if  (isNextLineGood)
    {
      numInARowThatAreGood++;
      numInARowThatAreBad = 0;

      if  (numInARowThatAreGood > 20)
      {
        validSipper3File = true;
        break;
      }
    }
    else
    {
      numInARowThatAreGood = 0;
      numInARowThatAreBad++;
      if  (numInARowThatAreBad > 20)
      {
        break;
      }
    }
    numRead++;
  
  }  while ((numRead < 500)  &&  (!Eof ()));

  return  validSipper3File;
}  /* FileFormatGood */




void  Sipper3Buff::ExtractSipperHeaderInfo ()
{
  bool        eol         = false;
  bool        grayScale   = true;
  bool        imageData   = 0;
  bool        raw         = false;
  size_t      recsRead    = 0;
  Sipper3Rec  sipperRec;
  uint32      sipperRecsRead = 0;

  fpos_t  origPos; 

  int32  retCode;

  retCode = fgetpos (inFile, &origPos);
  if  (retCode != 0)
    return;

  rewind (inFile);

  KKStr  headerStr (500);  // Pre Allocated 500 characters.


  // The header info should be the first bytes written to the file.  These bytes are written
  // by the program that extracts them from the SIPPER storage disk.
  while  (sipperRecsRead < 10000)
  {
    recsRead = fread (&sipperRec, SIPPERRECSIZE, 1, inFile);
    if  (recsRead < 1)
      break;

    sipperRecsRead++;

    imageData   = sipperRec.imageData;
    if  (!imageData)
    {
      raw         = sipperRec.raw;
      eol         = sipperRec.eol;
      grayScale   = sipperRec.grayScale;

      uchar  sensorId   = sipperRec.raw       * 32 + 
                          sipperRec.grayScale * 16 + 
                          sipperRec.pix11     * 8  +
                          sipperRec.pix10     * 4  +
                          sipperRec.pix9      * 2  +
                          sipperRec.pix8;

      if  (sensorId == 6)
      {

        uchar  sensorData = sipperRec.pix7  * 128 +
                            sipperRec.pix6  * 64  +
                            sipperRec.pix5  * 32  +
                            sipperRec.pix4  * 16  +
                            sipperRec.pix3  * 8   +
                            sipperRec.pix2  * 4   +
                            sipperRec.pix1  * 2   +
                            sipperRec.pix0;

        headerStr.Append (sensorData);
      }
    }
  }


  retCode = fsetpos (inFile, &origPos);

  sipperHeaderRec.ParseHeaderStr (headerStr);
  AssignSerialPorts (&sipperHeaderRec);

  return;
}  /* ExtractSipperHeaderInfo */
