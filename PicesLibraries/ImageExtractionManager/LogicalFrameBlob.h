#if  !defined(_IEM_LOGICALFRAMEBLOB_)
#define  _IEM_LOGICALFRAMEBLOB_
/**
 @class MLL::LogicalFrameBlob
 @brief  Used by the ImageExtractionManager to manage a single blob.
 @author  Kurt Kramer
 @details
 @code
 ****************************************************************************
 **                           LogicalFrameBlob                              *
 **                                                                         *
 **  Used by ImageExtractionManager to keep track of an individule conected *
 **  componets in a sipper frame.  We call these connected componets blobs. *
 **  The fields rowTop, rowBot, colLeft, colRight indicate a tight bounding *
 **  box arround the blob.                                                  *
 ****************************************************************************
 @endcode
*/

#include  "BasicTypes.h"
#include  "KKQueue.h"
#include  "RasterSipper.h"
#include  "Str.h"


namespace  ImageExtractionManager  
{

  class  LogicalFrameBlob
  {

  public:
    typedef  LogicalFrameBlob*               LogicalFrameBlobPtr;
    typedef  vector<LogicalFrameBlobPtr>     VectorLogicalFrameBlob;

    typedef  map<int32, LogicalFrameBlobPtr>  NeighborsList;


    LogicalFrameBlob (uint32  _maxFrameHeight,
                      uint32  _maxFrameWidth
                     );

    ~LogicalFrameBlob ();

    int32  Id ()  {return  id;}

    uint32  ColLeft ()     {return colLeft;}
    uint32  ColRight ()    {return colRight;}


    void  AddNeighbor (LogicalFrameBlobPtr  _neighbor);

    void  DialateBlob (uchar**  frame,
                       int32**  blobIds,
                       int32    size
                      );

    bool  Explored ()  const  {return explored;}

    void  Explored (bool _explored)  {explored = _explored;}

    uint32  Height ()      {return  abs ((1 + (int32)rowBot - (int32)rowTop));}

    NeighborsList&  Neighbors ()  {return  neighbors;}

    uint32  RowBot ()      {return rowBot;}
    uint32  RowTop ()      {return rowTop;}

    uint32  Width ()       {return  abs ((1 + (int32)colLeft - (int32)colRight));}

    uint32  PixelCount ()  {return pixelCount;}

    int32   id;

    bool  explored;

    uint32  rowBot;
    uint32  rowTop;
    uint32  colLeft;
    uint32  colRight;

    int32          neighborIdLastAdded;
    NeighborsList  neighbors;

    uint32  maxFrameHeight;
    uint32  maxFrameWidth;

    uint32  pixelCount;

  };  /* LogicalFrameBlob */


  typedef  LogicalFrameBlob::LogicalFrameBlobPtr     LogicalFrameBlobPtr;
  typedef  LogicalFrameBlob::VectorLogicalFrameBlob  VectorLogicalFrameBlob;

}  /* namespace  ImageExtractionManager  */


#endif
