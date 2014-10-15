#ifndef  _SIPPERBLOB_
#define  _SIPPERBLOB_
/**
 @class MLL::SipperBlob
 @brief  Used by SipperImageExtrtaction to manage a single blob.
 @author  Kurt Kramer
 @details
 @code
 ***************************************************************************
 **                              SipperBlob                                *
 **                                                                        *
 **  Used by SipperImageExtraction to keep track of an individule conected *
 **  componets in a sipper frame.  We call these connected componets blobs.*
 **  The fuields rowTop, rowBot, colLeft, colRight indicate a tight bound- *
 **  box arround the blob.  SipperImageExtraction will use                 *
 **                                                                        *
 ***************************************************************************
 @endcode
 @see SipperImageExtraction
*/

#include  "BasicTypes.h"
#include  "KKQueue.h"
#include  "Raster.h"
#include  "Str.h"


namespace  MLL  {

#ifndef  _IMAGEFEATURES_
class  ImageFeatures;
class  ImageFeaturesList;

typedef  ImageFeatures*      ImageFeaturesPtr;
typedef  ImageFeaturesList*  ImageFeaturesListPtr;
#endif



#ifndef  _MLCLASS_
class  MLClass;
class  MLClassList;

typedef  MLClass*      MLClassPtr;
typedef  MLClassList*  MLClassListPtr;
#endif



class  SipperBlob
{

public:
  typedef  SipperBlob*            SipperBlobPtr;
  typedef  vector<SipperBlobPtr>  VectorSipperBlob;

  typedef  map<int32, SipperBlobPtr>  NeighborsList;


  SipperBlob ();

  ~SipperBlob ();

  int32  Id ()  {return  id;}

  uint32  ColLeft ()     {return colLeft;}
  uint32  ColRight ()    {return colRight;}


  void  AddNeighbor (SipperBlobPtr  _neighbor);

  void  DialateBlob (uchar** frame,
                     int32**   blobIds,
                     int32     size
                    );

  RasterPtr  GetRaster (uchar** frame,
                        int32**   blobIds,     
                        uint32    firstFrameRowScanLine
                       )  const;

  bool  Explored ()  const  {return explored;}

  void  Explored (bool _explored)  {explored = _explored;}

  uint32  Height ()      {return  abs ((1 + (int32)rowBot - (int32)rowTop));}

  NeighborsList&  Neighbors ()  {return  neighbors;}

  uint32  RowBot ()      {return rowBot;}
  uint32  RowTop ()      {return rowTop;}

  uint32  Width ()       {return  abs ((1 + (int32)colLeft - (int32)colRight));}

  uint32  PixelCount ()  {return pixelCount;}

  ImageFeaturesPtr  Save (uchar**        frame,
                          int32**        blobIds,     
                          KKStr          fileName,
                          bool           createImageFeaturesObject,
                          MLClassPtr  mlClass,
                          uint32         firstFrameRowScanLine,
                          bool           save,        /**< Set to true to save file false if only calculating features.      */
                          bool           colorize     /**< Set to true if you wish to save the 7 grayscale values as colors. */
                         );

  static void   MaxFrameHeight (uint32  _maxFrameHeight)  {maxFrameHeight = _maxFrameHeight;}
  static void   MaxFrameWidth  (uint32  _maxFrameWidth)   {maxFrameWidth  = _maxFrameWidth;}



  int32   id;

  bool  explored;

  uint32  rowBot;
  uint32  rowTop;
  uint32  colLeft;
  uint32  colRight;

  int32          neighborIdLastAdded;
  NeighborsList  neighbors;

  static  
    uint32  maxFrameHeight;

  static  
    uint32  maxFrameWidth;

    uint32  pixelCount;

};  /* SipperBlob */


typedef  SipperBlob::SipperBlobPtr     SipperBlobPtr;
typedef  SipperBlob::VectorSipperBlob  VectorSipperBlob;

}  /* namespace  MLL  */


#endif
