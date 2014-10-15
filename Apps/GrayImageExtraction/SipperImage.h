#ifndef  _SIPPERIMAGE_
#define  _SIPPERIMAGE_

#include "RasterSipper.h"
#include "SipperBlob.h"
using namespace  KKU;
using namespace  MLL;


class  SipperImage
{
public:
  typedef  SipperImage*  SipperImagePtr;

  SipperImage ();
  SipperImage (SipperBlobPtr  blob);

  ~SipperImage ();

  void  AddBlob (SipperBlobPtr  _blob);
  void  Explore (SipperBlobPtr  _blob);

  uint64 ByteOffset () const  {return byteOffset;}
  uint   ColLeft    () const  {return colLeft;}
  uint   ColRight   () const  {return colRight;}
  uint   PixelCount () const  {return pixelCount;}
  uint   RowBot     () const  {return rowBot;}
  uint   RowTop     () const  {return rowTop;}

  uint   Width      () const  {return (1 + colRight - colLeft);}
  uint   Height     () const  {return (1 + rowBot   - rowTop);}


  void  Dialate (uchar** frame,
                 int**   blobIds,
                 int     size
                );


  RasterSipperPtr  GetRaster (uchar** frame,
                              int**   blobIds,     
                              uint    firstFrameRowScanLine,
                              uint64*  frameRowByteOffsets
                             );



private:
  uint64  byteOffset;  // Sipper File ByteOffset of scanline that contains the 1st row in this image.
  uint    colLeft;
  uint    colRight;
  uint    pixelCount;
  uint    rowBot;
  uint    rowTop;

  vector<SipperBlobPtr>   blobList;
};

typedef  SipperImage::SipperImagePtr  SipperImagePtr;
typedef  vector<SipperImage*>         VectorSipperImages;

#endif
