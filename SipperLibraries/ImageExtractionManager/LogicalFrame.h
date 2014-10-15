#if  !defined(_IEM_LOGICALFRAME_)
#define  _IEM_LOGICALFRAME_

#include <queue>

#include "GoalKeeper.h"

#include "PreDefinedClasses.h"
#include "ExtractionParms.h"


namespace  ImageExtractionManager
{
  /**
   *@brief   The Image extracion procedure will contain several threads perform various functions.
   *@details  This will be the base class for each one of the different types of processing that 
   * Sipper Image processing will be done.  Soem of threads will Extract logical frames from Sipper 
   * files,  
   */
  class  LogicalFrame
  {
  public:
    LogicalFrame (const ExtractionParms&  _parms,
                  uint32                  _frameHeightMax,
                  uint32                  _frameWidth,
                  const bool&             _cancelFlag
                 );

    ~LogicalFrame ();

    int32   MemoryConsumedEstimated ()  const;

    uint32  FrameHeight   ()  const  {return frameHeight;}
    uint32  FrameNum      ()  const  {return frameNum;}
    uint32  ImagesInFrame ()  const  {return imagesInFrame;}

    void  PopulateFrame (uint32   _frameNum,
                         uint32   _frameHeight,
                         uchar*   _frameArea,
                         uint32   _frameSipperRow,
                         uint64*  _frameRowByteOffset,
                         uint32*  _pixelsPerRow
                        );

    ExtractedImageListPtr  ProcessFrame ();


  private:
    void  AllocateFrame ();
    void  DeAllocateFrame ();
    void  PerformDialation ();
    void  PerformErosion ();
    void  PerformOpening ();
    void  PerformClosing ();
    void  PerformMorphOperations ();
    void  InitailzieBlobIds ();
    void  LocateBlobsUsingConnectedDistanceOf3 ();
    void  LocateBlobsUsingConnectedDistanceNotOf3 ();
    
    VectorSipperImagePtr  BuildListOfSipperImages (uint32&  imagesInFrame);
    
    int  GetMaxBlobIdInUpperLeft (uint32 row,
                                  uint32 col
                                 );

    int  GetMaxBlobIdInUpperRight (uint32 row,
                                   uint32 col
                                  );

    LogicalFrameBlobPtr  NewBlob (uint32  rowTop,
                                  uint32  colLeft
                                 );

    bool  ForegroundPixel (uchar  pixValue);

    void  CountAndTotal (KKU::uchar    c,
                         KKU::ushort&  count,
                         KKU::ushort&  total
                        );

    static
    int  CountPixs (int x1,  int x2,  int x3,  
                    int x4,  int x5,  int x6,  
                    int x7,  int x8,  int x9
                   );

    static  inline
    int  Max (int x1,  int x2,  int x3, 
              int x4,  int x5,  int x6
             );

    static  inline  
    int  Max (int x1,  int x2,  int x3,  
              int x4,  int x5,  int x6,  
              int x7,  int x8,  int x9
             );


    uchar**         frame;                  /**< Row pointers into 'frameArea'.                           */

    uint32          frameHeight;            /**< Number of rows in current frame.                         */
    uint32          frameHeightMax;         /**< Number of scan lines that have been actually allocated.  */
    uint32          frameWidth;             /**< Number of pixedls per row in frame.                      */
    uchar*          frameArea;              /**< Pointer to block of memory that will contain frame       */
    uint32          frameSipperRow;         /**< Row within the Sipper File That Frame starts at.         */
    uint64*         frameRowByteOffset;
    uint32          frameTotalPixels;       /**< Total number of pixels that frame could contain.         */
    uint32*         pixelsPerRow;           /**< Number of forground pixels in each row.                  */

    uchar**         origFrame;              /**< 'origFrame' and 'origFrameArea' are only used when       */
    uchar*          origFrameArea;          /**<  morphalogical operations are performed.                 */
   
    uchar**         workFrame;              /**< 'workFrame' & 'workFrameArea' are used to perform        */
    uchar*          workFrameArea;          /**< morphalogical operations with.                           */

    uint32          frameNum;               /**< Sequential number assigned to each frame.                */
    uint32          imagesInFrame;          /**< Number of descrete images located by the connected component analysis. */

    // Sgructures to support Blob Processing.
    LogicalFrameBlobPtr*  blobs;
    int32*          blobIdsArea;            /**< Continuous chunck of memory that was allocated to hold 'blobIds' in.   */
    int32**         blobIds;                /**< Two dimensional array that will work with frame.  For each pixel that is 
                                             *   determined to belong to an image, the ID of that image that it is part of
                                             *   will be kept here.
                                             */
    uint32          connectedPixelDist;     /**< Distance that two forground pixels have to be to be considered part of same connected image. */
    uint32          maxBlobsPerFrame;       /**< Maximum number of blobs that can be tracked;  'blobs' wil be allocated to this size.         */
    uint32          numOfBlobsInFrame;      /**< Number of blobs located in curent frame so far.                                              */

    const
    ExtractionParms&  parms;

    uchar           backgroundPixelTH;
    uint32          cropLeft;
    uint32          cropRight;
                            
    volatile
    const bool&     cancelFlag;
  };  /* LogicalFrame */

  typedef  LogicalFrame*  LogicalFramePtr;

}  /* ImageExtractionManager */


#endif
