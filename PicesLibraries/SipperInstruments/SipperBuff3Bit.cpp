#include  "FirstIncludes.h"
#include  <stdio.h>
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


#include  "SipperBuff3Bit.h"
using namespace SipperHardware;



uchar  ShadesFor3Bit[] = {0, 36, 73, 109, 146, 182, 219, 255};



SipperBuff3Bit::SipperBuff3Bit (InstrumentDataManagerPtr  _instrumentDataManager,
                                RunLog&                   _log
                               ):
  SipperBuff (_instrumentDataManager, _log)
{
  byteOffset = 0;
}



SipperBuff3Bit::SipperBuff3Bit (KKStr                    _fileName,
                                InstrumentDataManagerPtr  _instrumentDataManager,
                                RunLog&                   _log
                               ):
  SipperBuff (_fileName, _instrumentDataManager, _log)
{
  byteOffset = 0;
}



SipperBuff3Bit::~SipperBuff3Bit ()
{
  this->SipperBuff::~SipperBuff ();
}


typedef  struct
{
  uchar  b1;
  uchar  b2;
}  SplitRec;

typedef  SplitRec*  SplitRecPtr;



void  SipperBuff3Bit::GetNextSipperRec (uint32&  spaceLeft,
                                        uchar&   cameraNum,
                                        bool&    raw,
                                        bool&    eol,
                                        bool&    flow,
                                        uchar&   pixel0,
                                        uchar&   pixel1,
                                        uchar&   pixel2,
                                        uchar&   pixel3,
                                        uint32&  numOfBlanks,
                                        bool&    moreRecs
                                       )
{
  uchar  pix1,  pix2,  pix3,  pix4,
         pix5,  pix6,  pix7,  pix8,
         pix9,  pix10, pix11, pix0;


  Sipper3BitRec  sipperRec;

  size_t  recsRead;
  curRowByteOffset = byteOffset;

  // sizeof can not work with bit fields
  //recsRead =  fread (&sipperRec, sizeof (sipperRec), 1, inFile);  // kk 2005-02-11
  recsRead =  fread (&sipperRec, SIPPERRECSIZE, 1, inFile);
  if  (recsRead < 1)
  {
    eof = true;
    moreRecs = false;
    return;
  }

  bool  exitLoop = false;

  while  (!exitLoop)
  {
    //byteOffset = byteOffset + sizeof (sipperRec);   // kk 2005-02-11
    byteOffset = byteOffset + SIPPERRECSIZE;

    cameraNum = sipperRec.cameraNum;
    raw       = sipperRec.raw;
    eol       = sipperRec.eol;
    flow      = sipperRec.flow;

    pix0      = sipperRec.pix0;
    pix1      = sipperRec.pix1;
    pix2      = sipperRec.pix2;
    pix3      = sipperRec.pix3;
    pix4      = sipperRec.pix4;
    pix5      = sipperRec.pix5;
    pix6      = sipperRec.pix6;
    pix7      = sipperRec.pix7;
    pix8      = sipperRec.pix8;
    pix9      = sipperRec.pix9;
    pix10     = sipperRec.pix10;
    pix11     = sipperRec.pix11;

    pixel0 = pix11 * 4 + pix10 * 2 + pix9;
    pixel1 = pix8  * 4 + pix7  * 2 + pix6;
    pixel2 = pix5  * 4 + pix4  * 2 + pix3;
    pixel3 = pix2  * 4 + pix1  * 2 + pix0;

    numOfBlanks = pixel0 * 2048 + pixel1 * 256 + pixel2 * 32  + pixel3 * 4;

    if  (((!raw) && (numOfBlanks > (spaceLeft + 100)))  ||  (cameraNum == 1))
    {
      // Looks like we are a byte out of sequence,  Maybe Byte was dropped 
      // by sipper.  We will read one byte only to seee if we can get back 
      // into sync,

      exitLoop = false;

      SplitRecPtr  tsr  = (SplitRecPtr)&sipperRec;

      tsr->b1 = tsr->b2;  // Moving 2nd Byte to 1st Byte position

      recsRead = fread (&(tsr->b2), 1, 1, inFile);  // Read in 2nd byte from sipper.
      if  (recsRead == 0)
      {
        moreRecs = false;
        return;
      }
      else
      {
        bytesDropped++;
      }
    }
    else
    {
      exitLoop = true;
    }
  }

  moreRecs = true;

  return;
}  /* GetNextSipperRec */




void  SipperBuff3Bit::GetNextLine (uchar*  lineBuff,
                                   uint32  lineBuffSize,
                                   uint32& lineSize,
                                   uint32  colCount[],
                                   uint32& pixelsInRow,
                                   bool&   flow
                                  )
{
  uchar   cameraNum;

  bool    eol;

  bool    moreRecs;

  uint32  numOfBlanks;

  uchar   pixel0, pixel1, pixel2, pixel3;

  bool    exceededBuffLen = false;

  uint32  spaceLeft = lineBuffSize;

  bool    raw;

  memset (lineBuff, 0, lineBuffSize);


  GetNextSipperRec (spaceLeft,
                    cameraNum, raw,     eol,     flow,
                    pixel0,    pixel1,  pixel2,  pixel3,
                    numOfBlanks,
                    moreRecs
                   );

  bool  notFinished = moreRecs;

  lineSize = 0;

  pixelsInRow = 0;

  bool pauseAtEol = false;

  while  (notFinished)
  {
    if  (raw)
    {
      if  (spaceLeft < 4)
      {
        bytesDropped = bytesDropped  + 2;
        cerr << endl
             << "*** Warning ***     Exceeded Line Length." << endl;
        if  (spaceLeft <= 0)
          exceededBuffLen = true;
      }
      else
      {
        if  (pixel0 > 0)  
        {
          lineBuff[lineSize] = ShadesFor3Bit[pixel0];
          colCount[lineSize]++;
          pixelsInRow++;
        }
        lineSize++;

        if  (pixel1 > 0)  
        {
          lineBuff[lineSize] = ShadesFor3Bit[pixel1];
          colCount[lineSize]++;
          pixelsInRow++;
        }
        lineSize++;
      
        if  (pixel2 > 0)  
        {
          lineBuff[lineSize] = ShadesFor3Bit[pixel2];
          colCount[lineSize]++;
          pixelsInRow++;
        }
        lineSize++;
      
        if  (pixel3 > 0)  
        {
          lineBuff[lineSize] = ShadesFor3Bit[pixel3];
          colCount[lineSize]++;
          pixelsInRow++;
        }
        lineSize++;

        spaceLeft = spaceLeft - 4;

        if  (lineSize > lineBuffSize)
          lineSize = lineBuffSize;
      }
    }

    else
    {
      // cout << setw (6) << numOfBlanks;

      if  (numOfBlanks > spaceLeft)
      {
        numOfBlanks = 0;
        exceededBuffLen = true;
      }

      lineSize = lineSize + numOfBlanks;
      spaceLeft = spaceLeft - numOfBlanks;
    }

    // cout << setw (6) << lineSize << endl;
    if  (exceededBuffLen)
    {
      exceededBuffLen = false;

      pauseAtEol = true;

      cerr  << "*";
      //spaceLeft = lineBuffSize;
      //memset (workBuff, 0, workBuffLen);
      //eol = false;
      //lineSize = 0;
      //pixelsInRow = 0;
    } 
  
    if  (eol)
    {
      notFinished = false;
    }
    else
    {
      GetNextSipperRec (spaceLeft,
                        cameraNum, raw,     eol,     flow,
                        pixel0,    pixel1,  pixel2,  pixel3,
                        numOfBlanks,
                        moreRecs);
      notFinished = moreRecs;
    }
  }

  if  (lineSize > 4096)
  {
    cerr << "SipperBuff3Bit::GetNextLine  LineSize["  << lineSize << "] exceeded 4096 ."  << endl;
  }

  else if  ((lineSize == 0)  &&  (!eof))
  {
    lineSize = 1;
  }


  curRow++;
}  /* GetNextLine */
