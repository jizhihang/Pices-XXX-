#ifndef  _SIPPERBUFF3BIT_
#define  _SIPPERBUFF3BIT_


#include  "SipperBuff.h"

namespace  SipperHardware
{


#ifdef WIN32


typedef struct 
{
    uchar pix8      : 1;
    uchar pix9      : 1;
    uchar pix10     : 1;
    uchar pix11     : 1;

    uchar flow      : 1;  // 0 = Flow Meter in Position 0
                          // 1 = Flow Meter in Position 1

    uchar raw       : 1;  // 1 = Data is Raw 
                          // 0 = Compressed Run-Length

    uchar eol       : 1;  // 1 = End of Line Encountered, 
                          // 0 = End of Line not Encountered

    uchar cameraNum : 1;

    uchar pix0      : 1;
    uchar pix1      : 1;
    uchar pix2      : 1;
    uchar pix3      : 1;
    uchar pix4      : 1;
    uchar pix5      : 1;
    uchar pix6      : 1;
    uchar pix7      : 1;
}  Sipper3BitRec;

#else


typedef struct 
{
  uchar cameraNum  : 1;
  uchar eol        : 1;  // 1 = End of Line Encountered, 
                         // 0 = End of Line not Encountered

  uchar raw        : 1;  // 1 = Data is Raw 
                         // 0 = Compressed Run-Length

  uchar flow       : 1;  // 0 = Flow Meter in Position 0
                         // 1 = Flow Meter in Position 1

  uchar pix11      : 1;
  uchar pix10      : 1;
  uchar pix9       : 1;

  uchar pix8       : 1;
  uchar pix7       : 1;
  uchar pix6       : 1;

  uchar pix5       : 1;
  uchar pix4       : 1;
  uchar pix3       : 1;

  uchar pix2       : 1;
  uchar pix1       : 1;
  uchar pix0       : 1;
}  Sipper3BitRec;

#endif


typedef  Sipper3BitRec*  Sipper3BitRecPtr;


class  SipperBuff3Bit:  public SipperBuff
{
public:
  SipperBuff3Bit (InstrumentDataManagerPtr  _instrumentDataManager,
                  RunLog&                   _log
                 );

  SipperBuff3Bit (KKStr                    _fileName,
                  InstrumentDataManagerPtr  _instrumentDataManager,
                  RunLog&                   _log
                 );
  
  virtual
  ~SipperBuff3Bit ();

  const KKStr&  FileName ()  {return  fileName;}

  bool  Eof ()      {return  eof;}

  uint32  CurRow ()   {return  curRow;} 

  bool  Opened ()   {return  opened;}

  uint32  RecCount () {return  recCount;}

  void  GetNextLine (uchar* lineBuff,
                     uint32 lineBuffSize,
                     uint32&  lineSize,
                     uint32 colCount[],
                     uint32&  pixelsInRow,
                     bool&  flow);

  void  SkipToScanLine (uint32  scanLine);

private:

  inline
  void  GetNextSipperRec (uint32&  spaceLeft,
                          uchar&   cameraNum,
                          bool&    raw,
                          bool&    eol,
                          bool&    flow,
                          uchar&   pixel0,
                          uchar&   pixel1,
                          uchar&   pixel2,
                          uchar&   pixel3,
                          uint32&  numOfBlanks,
                          bool&    moreRecs);



};

typedef  SipperBuff3Bit*  SipperBuff3BitPtr;

}  /* SipperHardware */

#endif
