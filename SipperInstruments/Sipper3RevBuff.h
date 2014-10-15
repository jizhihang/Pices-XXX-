#ifndef  _SIPPER3REVBUFF_
#define  _SIPPER3REVBUFF_

#include  "SipperBuff.h"
#include  "Sipper3Buff.h"

namespace  SipperHardware
{

class  Sipper3RevBuff:  public SipperBuff
{
public:
  Sipper3RevBuff (InstrumentDataManagerPtr  _instrumentDataManager,
                  RunLog&                   _log
                 );

  Sipper3RevBuff (const KKStr&             _fileName,
                  InstrumentDataManagerPtr  _instrumentDataManager,
                  RunLog&                   _log
                 );
  
  ~Sipper3RevBuff ();

  const 
  KKStr&  FileName ()  {return  fileName;}

  uint32   CurRow ()    {return  curRow;} 

  bool     Eof ()       {return  eof;}

  bool     Opened ()    {return  opened;}

  uint32   RecCount ()  {return  recCount;}

  bool     FileFormatGood ();


  void  GetNextLine (uchar*   lineBuff,
                     uint32   lineBuffSize,
                     uint32&  lineSize,
                     uint32   colCount[],
                     uint32&  pixelsInRow,
                     bool&   flow
                    );

  void  SkipToScanLine (uint32  scanLine);


private:

  inline
  void  GetNextSipperRec (uint32&  spaceLeft,
                          uchar&   cameraNum,
                          bool&    imageData,
                          bool&    raw,
                          bool&    eol,
                          bool&    grayScale,
                          uchar*   pixels,      /**< Array of size 12.            */
                          uchar&   numPixels,   /**< Number of pixels in pixels.  */
                          uint32&  numOfBlanks,
                          bool&    moreRecs
                         );

  bool  NextScanLineGood ();   /**< Used by SipperBuff to try and guess if this is a Sipper3  file. */

  uint32  overflowPixels;      /**< if > 0, indicates that the last scan line read had an overflow by that many pixels. */

  static  
    uchar  ShadesFor3Bit[];
};

typedef  Sipper3RevBuff*  Sipper3RevBuffPtr;
}  /* SipperHardware */


#endif
