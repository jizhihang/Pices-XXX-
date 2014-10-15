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


#include  "SipperBuffBinary.h"


using namespace SipperHardware;


SipperBuffBinary::SipperBuffBinary (InstrumentDataManagerPtr  _instrumentDataManager,
                                    RunLog&                   _log
                                   ):
  SipperBuff (_instrumentDataManager, _log)
{
  byteOffset = 0;
}



SipperBuffBinary::SipperBuffBinary (KKStr                    _fileName,
                                    uchar                     _selCameraNum,
                                    InstrumentDataManagerPtr  _instrumentDataManager,
                                    RunLog&                   _log
                                   ):
  SipperBuff    (_fileName, _instrumentDataManager, _log),
  selCameraNum  (_selCameraNum)
{
  byteOffset = 0;
}



SipperBuffBinary::~SipperBuffBinary ()
{
  this->SipperBuff::~SipperBuff ();
}



typedef  struct
{
  KKU::uchar  b1;
  KKU::uchar  b2;
}  SplitRec;

typedef  SplitRec*  SplitRecPtr;



void  SipperBuffBinary::GetNextSipperRec (int32&   spaceLeft,
                                          uchar&   cameraNum,
                                          bool&    raw,
                                          bool&    eol,
                                          bool&    flow,
                                          uchar&   pix0,
                                          uchar&   pix1,
                                          uchar&   pix2,
                                          uchar&   pix3,
                                          uchar&   pix4,
                                          uchar&   pix5,
                                          uchar&   pix6,
                                          uchar&   pix7,
                                          uchar&   pix8,
                                          uchar&   pix9,
                                          uchar&   pix10,
                                          uchar&   pix11,
                                          int32&   numOfBlanks,
                                          bool&    moreRecs)
{
  SipperBinaryRec  sipperRec;

  int32 recsRead;

  curRowByteOffset = byteOffset;

  do  {
    recsRead =  (int32)fread (&sipperRec, sizeof (sipperRec), 1, inFile);
    if  (recsRead < 1)
    {
      eof = true;
      moreRecs = false;
      return;
    }
  }  while  (sipperRec.cameraNum != selCameraNum);

  bool  exitLoop = false;

  while  (!exitLoop)
  {
    byteOffset = byteOffset + sizeof (sipperRec);

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


    numOfBlanks = pix0 * 2048 + pix1  * 1024 + pix2 * 512 +
                  pix3 * 256  + pix4  * 128  + pix5 * 64  +
                  pix6 * 32   + pix7  * 16   + pix8 * 8   +
                  pix9 * 4    + pix10 * 2    + pix11;


    if  (numOfBlanks == 0)
      numOfBlanks = 4096;

    if  (false)
    {
      exitLoop = false;

      SplitRecPtr  tsr  = (SplitRecPtr)&sipperRec;

      tsr->b1 = tsr->b2;

      recsRead = (int32)fread (&(tsr->b2), 1, 1, inFile);
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





int32  linesInRowThatExceededBuffLen = 0;



void  SipperBuffBinary::GetNextLine (uchar*  lineBuff,
                                     uint32  lineBuffSize,
                                     uint32& lineSize,
                                     uint32  colCount[],
                                     uint32& pixelsInRow,
                                     bool&   flow
                                    )
{
  uchar    cameraNum;

  bool     eol;

  bool     exceededBuffLen = false;

  bool     moreRecs;

  int32    numOfBlanks;

  uchar    pix1,  pix2,  pix3,  pix4,
           pix5,  pix6,  pix7,  pix8,
           pix9,  pix10, pix11, pix0;

  bool     raw;

  int32    spaceLeft = lineBuffSize;


  memset (lineBuff, 0, lineBuffSize);

  GetNextSipperRec (spaceLeft, 
                    cameraNum,  raw, eol, flow,
                    pix0, pix1, pix2, pix3, pix4,  pix5,
                    pix6, pix7, pix8, pix9, pix10, pix11,
                    numOfBlanks,
                    moreRecs);
 
  bool  notFinished = moreRecs;

  lineSize = 0;

  pixelsInRow = 0;

  bool pauseAtEol = false;

  while  (notFinished)
  {
    if  (raw)
    {
      if  (spaceLeft <= 0)
      {
        cerr << "*** Warning  ***" << endl;
        if  (spaceLeft < 0)
          exceededBuffLen = true;
      }
      else
      {
        if  (pix0 > 0)  
        {
          lineBuff[lineSize] = 255;
          colCount[lineSize]++;
          pixelsInRow++;
        }
        lineSize++;
        spaceLeft--;


        if  (spaceLeft > 0)
        {
          if  (pix1 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix2 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix3 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix4 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix5 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix6 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix7 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix8 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix9 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix10 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }

        if  (spaceLeft > 0)
        {
          if  (pix11 > 0)
          {
            lineBuff[lineSize] = 255;
            colCount[lineSize]++;
            pixelsInRow++;
          }

          lineSize++;
          spaceLeft--;
        }
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
    } 
  
    if  (eol)
    {
      notFinished = false;
    }
    else
    {
      GetNextSipperRec (spaceLeft, 
                        cameraNum,  raw, eol, flow,
                        pix0, pix1, pix2, pix3, pix4,  pix5,
                        pix6, pix7, pix8, pix9, pix10, pix11,
                        numOfBlanks,
                        moreRecs);
      notFinished = moreRecs;
    }
  }


  // if  (pauseAtEol)
  //  WaitForEnter ();

  if  (exceededBuffLen)
    linesInRowThatExceededBuffLen++;
  else
    linesInRowThatExceededBuffLen = 0;

  if  (linesInRowThatExceededBuffLen > 1)
  {
    linesInRowThatExceededBuffLen = 0;
    cout << "Resinking Buffer" << endl;
    // WaitForEnter ();
  }

  if  (lineSize > 4096)
  {
    cerr << "SipperBuffBinary::GetNextLine  LineSize["  << lineSize << "] exceeded 4096 ."  << endl;
  }

  else if  ((lineSize == 0)  &&  (!eof))
  {
    lineSize = 1;
  }


  curRow++;
}  /* GetNextLine */


