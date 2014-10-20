#ifndef  _SIPPERBUFF_
#define  _SIPPERBUFF_
//**************************************************************************
//*                                SipperBuff                              *
//*                                                                        *
//*                                                                        *
//* To be sub-classed for implementation of different specific Sipper file *
//* layouts.                                                               *
//*                                                                        *
//*                                                                        *
//* <p>Copyright: Copyright (c) 2003</p>                                   *
//* <p>author     Kurt Kramer                                              * 
//*                                                                        *
//*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
//*                                                                        *
//*  January 2003   Communicate to Sipper File through this object.        *   
//*  Kurt Kramer                                                           *
//*                                                                        *
//*  Kurt     2005-Feb-03  Added support for Sipper 3.                     *
//*  kurt     2005-Aug-12  Added support for SIPPER instrument data.       *
//*                        Now create an instance of InstrumentDataManager *
//*                        which all SIPPER 3 instrument data is sent to.  *
//*                                                                        *
//**************************************************************************

#include  "RunLog.h"
#include  "Str.h"
using namespace KKU;

#include  "SipperHeaderRec.h"


namespace SipperHardware
{
#define  MAXLINELEN  4096
#define  SipperBuffSize 10240

#ifndef  _INSTRUMENTDATAMANAGER_
  class InstrumentDataManager;
  typedef  InstrumentDataManager*  InstrumentDataManagerPtr;
#endif


  // We need to use a define for SippetrRec size because recs that
  // contain bit fields do not report correct size when using
  // sizeof 
  #define  SIPPERRECSIZE 2


  typedef  enum  {sfBinary,
                  sf3Bit,
                  sfSipper3,
                  sfSipper3Rev,
                  sfSipper4Bit,
                  sfUnKnown
                 }  SipperFileFormat;





  /**
   *@class SipperBuff  Base class to be used for all the different Sipper File Formats.
   *@see  InstrumentDataManager
   */
  class  SipperBuff
  {
  public:
    typedef  SipperBuff*  SipperBuffPtr;
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    SipperBuff (InstrumentDataManagerPtr  _instrumentDataManager,
                RunLog&                   _log);
  
    SipperBuff (const KKStr&              _fileName,
                InstrumentDataManagerPtr  _instrumentDataManager,
                RunLog&                   _log
               );
    
    virtual  ~SipperBuff ();
  
    const KKStr&  FileName () const {return  fileName;}
  
  
  
    void    InvalidLine  (bool _invalidLine)  {invalidLine = _invalidLine;}
    void    Open (const KKStr&  _fileName);
  

    uint32  BytesDropped     ()  const {return  bytesDropped;}
    uint64  ByteOffset       ()  const {return  byteOffset;}
  
    uint32  CurRow           ()  const {return  curRow;} 
    uint64  CurRowByteOffset ()  const {return  curRowByteOffset;}
    bool    Eof              ()  const {return  eof;}
    bool    InvalidLine      ()  const {return  invalidLine;}
    bool    Opened           ()  const {return  opened;}
    uint32  RecCount         ()  const {return  recCount;}
    int32   ScanRate         ()  const {return  25000;}
  
    virtual
    int32   LineWidth        ()  const {return (int32)MAXLINELEN;}  /**< Returns the width in pixles of the associated scanner file.  */

    virtual 
    void  GetNextLine (uchar*   lineBuff,
                       uint32   lineBuffSize,
                       uint32&  lineSize,
                       uint32   colCount[],
                       uint32&  pixelsInRow,
                       bool&    flow
                      ) = 0;
  
    void  SkipToScanLine (uint32  scanLine);
  
    void  SkipToScanLine (uint32  _scanLine,
                          uint64  _byteOffset
                         );
  
  
    static
    SipperFileFormat  GuessFormatOfFile (const KKStr&  _fileName,
                                         int32         _cameraNum,
                                         RunLog&       _log
                                        );
  
    static
    SipperBuffPtr  CreateSipperBuff (SipperFileFormat          _format,
                                     KKStr                     _fileName,
                                     int32                     _cameraNum,
                                     InstrumentDataManagerPtr  _instrumentDataManager,
                                     RunLog&                   _log
                                    );
  
    static
    SipperBuffPtr  CreateSipperBuff (KKStr                     _fileName,
                                     int32                     _cameraNum,
                                     InstrumentDataManagerPtr  _instrumentDataManager,
                                     RunLog&                   _log
                                    );
  
    void  DetermineCropSettings  (uint32&  cropLeft,
                                  uint32&  cropRght
                                 );


    void  Reset ();
  
    void  ResetByteOffset (uint32  _byteOffset);
  
    void  ReportInstrumentData (uchar  sensorId,
                                bool   text,
                                uchar  sensorData
                               );
  
  
    void  SkipOneByte ();
  
    
  protected:
  
    void  AssignSerialPorts (const SipperHeaderRecPtr  sipperHeaderRec);
  
    static
    SipperFileFormat  GuessFormatOfNextLine (FILE*    in,
                                             int32    _cameraNum,
                                             RunLog&  _log
                                            );
  
    uint32                    bytesDropped;
    uint64                    byteOffset;
    uint32                    curRow;
    uint64                    curRowByteOffset;
    bool                      eof;
    KKStr                     fileName;
    bool                      iOwnInstrumentDataManager;
    FILE*                     inFile;
    InstrumentDataManagerPtr  instrumentDataManager;
    bool                      invalidLine;   // Set to true if invalid data is detected on 
                                             // a scan line.  Such as exceeding line length.
    RunLog&                   log;
    bool                      opened;
    uint32                    recCount;

    struct  SipperRec;
    static  void  DisplaySipperRec (SipperBuff::SipperRec&  sr);
  };  /* SipperBuff */

  typedef  SipperBuff::SipperBuffPtr  SipperBuffPtr;

  KKStr  SipperFileFormatToStr (SipperFileFormat  fileFormat);
  
  SipperFileFormat  SipperFileFormatFromStr (KKStr  fileFormatStr);
  
  KKStr  SipperFileAvailableOptions ();
}  /* SipperHardware */


#endif
