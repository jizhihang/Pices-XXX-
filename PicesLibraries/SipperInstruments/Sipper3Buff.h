#ifndef  _SIPPER3BUFF_
#define  _SIPPER3BUFF_


#include  "RunLog.h"

using  namespace  KKU;

#include  "SipperBuff.h"
#include  "SipperHeaderRec.h"

namespace SipperHardware
{

#ifdef WIN32


struct  Sipper3Rec
{
  Sipper3Rec ():
    grayScale (1),
    raw       (1),
    eol       (0),
    imageData (1)
  {
  }

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

  uchar grayScale : 1;  // 0 = B/W
                        // 1 = GrayScale

  uchar raw       : 1;  // 1 = Data is Raw 
                        // 0 = Compressed Run-Length

  uchar eol       : 1;  // 1 = End of Line Encountered, 
                        // 0 = End of Line not Encountered

  uchar imageData : 1;  // 0 = Instrument Data
                        // 1 = Image Data
};


typedef struct 
{
    uchar sensorData  : 8;
    uchar sensorId    : 6;
    uchar typeData    : 1;  // 1 = End of Line Encountered, 
                            // 0 = End of Line not Encountered

    uchar imageData   : 1;  // 0 = Instrument Data
                            // 1 = Image Data

}  Sipper3InstrumentDataRec;




#else


typedef struct 
{
  uchar imageData   : 1;  // 0 = Instrument Data
                          // 1 = Image Data

  uchar eol         : 1;  // 1 = End of Line Encountered, 
                          // 0 = End of Line not Encountered

  uchar raw         : 1;  // 1 = Data is Raw 
                          // 0 = Compressed Run-Length

  uchar grayScale   : 1;  // 0 = B/W bits
                          // 1 = GrayScale

  uchar pix11       : 1;
  uchar pix10       : 1;
  uchar pix9        : 1;

  uchar pix8        : 1;
  uchar pix7        : 1;
  uchar pix6        : 1;

  uchar pix5        : 1;
  uchar pix4        : 1;
  uchar pix3        : 1;

  uchar pix2        : 1;
  uchar pix1        : 1;
  uchar pix0        : 1;
}  Sipper3Rec;
#endif


typedef  Sipper3Rec*  Sipper3RecPtr;


class  Sipper3Buff:  public SipperBuff
{
public:
  typedef  Sipper3Buff*  Sipper3BuffPtr;

  Sipper3Buff (InstrumentDataManagerPtr  _instrumentDataManager,
               RunLog&                   _log
              );

  Sipper3Buff (const KKStr&              _fileName,
               InstrumentDataManagerPtr  _instrumentDataManager,
               RunLog&                   _log
              );
  
  virtual
  ~Sipper3Buff ();

  const 
  KKStr&   FileName ()  {return  fileName;}

  uint32   CurRow   ()  {return  curRow;} 

  bool     Eof ()       {return  eof;}

  bool     Opened ()    {return  opened;}

  uint32   RecCount ()  {return  recCount;}

  bool     FileFormatGood ();

  void     GetNextLine (uchar*   lineBuff,
                        uint32   lineBuffSize,
                        uint32&  lineSize,
                        uint32   colCount[],
                        uint32&  pixelsInRow,
                        bool&  flow
                       );

  void  SkipToScanLine (uint32  scanLine);


private:

  /**
   * @brief This is the text from instrument id 6 "User_Text_Message".  This is needed to
   * properly process instrumentation data.
   * @details The text occurs at the beginning of the SIPPER file and contains meta data that describes the
   * contents of the sipper file as well as the instrument port assignments needed to allow us to decode the
   * instrument data.
   */
  void  ExtractSipperHeaderInfo ();


  inline
  void  GetNextSipperRec (uint32&  spaceLeft,
                          uchar&   cameraNum,
                          bool&    imageData,
                          bool&    raw,
                          bool&    eol,
                          bool&    grayScale,
                          uchar*   pixels,      /**< Array of size 12           */
                          uchar&   numPixels,   /**< Number of pixels in pixels */
                          uint32&  numOfBlanks,
                          bool&    moreRecs
                         );


  bool  NextScanLineGood ();  /**< Used by SipperBuff to try and guess if this is a Sipper3  file. */

  uint32  overflowPixels;     /** if > 0, indicates that the last scan line read had an overflow by that many pixels. */

  SipperHeaderRec  sipperHeaderRec;

  static  
    uchar  ShadesFor3Bit[];
};

typedef  Sipper3Buff::Sipper3BuffPtr Sipper3BuffPtr;

} /* SipperHardware */


#endif
