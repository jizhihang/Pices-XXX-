#if  !defined(_IEM_SIPPERIMAGE_)
#define  _IEM_SIPPERIMAGE_

#include "RasterSipper.h"
using namespace  KKU;

#include "LogicalFrameBlob.h"


namespace  ImageExtractionManager
{
  class  SipperImage
  {
  public:
    typedef  SipperImage*  SipperImagePtr;

    SipperImage ();
    SipperImage (LogicalFrameBlobPtr  blob);

    ~SipperImage ();

    void  AddBlob (LogicalFrameBlobPtr  _blob);
    void  Explore (LogicalFrameBlobPtr  _blob);

    uint64  ByteOffset () const  {return byteOffset;}
    uint32  ColLeft    () const  {return colLeft;}
    uint32  ColRight   () const  {return colRight;}
    uint32  PixelCount () const  {return pixelCount;}
    uint32  RowBot     () const  {return rowBot;}
    uint32  RowTop     () const  {return rowTop;}

    uint32  Width      () const  {return (1 + colRight - colLeft);}
    uint32  Height     () const  {return (1 + rowBot   - rowTop);}


    void  Dialate (uchar** frame,
                   int**   blobIds,
                   int     size
                  );

    RasterSipperPtr  GetRaster (uchar**  frame,
                                int**    blobIds,     
                                uint32   firstFrameRowScanLine,
                                uint64*  frameRowByteOffsets
                               );



  private:
    uint64   byteOffset;  // Sipper File ByteOffset of scanline that contains the 1st row in this image.
    uint32   colLeft;
    uint32   colRight;
    uint32   pixelCount;
    uint32   rowBot;
    uint32   rowTop;

    vector<LogicalFrameBlobPtr>   blobList;
  };

  typedef  SipperImage::SipperImagePtr  SipperImagePtr;
  typedef  vector<SipperImage*>         VectorSipperImage;
  typedef  VectorSipperImage*           VectorSipperImagePtr;
}  /* ImageExtractionManager */

#endif
