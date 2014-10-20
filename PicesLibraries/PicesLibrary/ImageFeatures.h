#ifndef  _IMAGEFEATURES_
#define  _IMAGEFEATURES_

/**
 *@class MLL::ImageFeatures
 *@brief  Specialized version of MLL::FeatureVector that will be used
 *to represent the features of a SIPPER Plankton image.
 *@author  Kurt Kramer
 *@details
 * Used for the representation of a Single Plankton Image.  You create an instance of this object for
 * each single image you need to keep track of.  There is a specialized version of MLL::FeatureFileIO
 * caled  MLL::FeatureFileIOPices that is used to write and read feature Data files.  What makes this
 * class of MLL::FeatureVector special are the additional fields that are Plankton specific such as
 * centroidCol, centroidRow, latitude, longitude, numOfEdgePixels, Centroid within SIPPEER file
 * sfCentroidCol, sfCentroidRow and version.
 *
 * The version number field is supposed to indicate which feature calculation routines were used.  This
 * way if there are changes the way features are calculated it can be detected during runtime if the features
 * are up to date they need to be recomputed.
 */

#include <string.h>
#include <string>

#include "BMPImage.h"
#include "KKQueue.h"
#include "RasterSipper.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;

#include "InstrumentData.h"
#include "InstrumentDataList.h"
               

#include "Attribute.h"
#include "ClassStatistic.h"
#include "FeatureNumList.h"
#include "FeatureVector.h"
#include "FileDesc.h"



namespace MLL 
{

  #ifndef  _FEATURENUMLIST_
  class  FeatureNumList;
  typedef  FeatureNumList*  FeatureNumListPtr;
  #endif

  #ifndef  _MLCLASS_
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  typedef  MLClass const  MLClassConst;
  typedef  MLClassConst * MLClassConstPtr;
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
  #endif


  //#define  InfinityValue  (FFLOAT)99999999999999.99
  //#define  NaNvalute      (FFLOAT)99999999999999.99

  //    Date     Prog     Ver#  Description
  // ==========  ======== ====  =================================================
  //
  // 2004-11-27  Kurt      36   Added fields to support Grading Funtionality
  //
  // 2004-12-05  Kurt      37   fixed so that the additional contour features
  //                            created for my thesis are only implemented
  //                            if "IMPLEMENTSAMPLING" is defined in BasicTypes.h
  //
  // 2005-02-03  Kurt      38   Added 2 new Centroid fields that are relative
  //                            to the beginning of the file.
  //                            Added 2nd line to Raw format of Feature file,
  //                            his line will list all the fields in teh file.
  //
  //
  // 2007-11-12  Kurt      44   Added 4 new Instrumentation fieds 'Depth', Oxygen'
  //                            'Florences', and 'Slinity'
  //
  // 2008-08-25  Kurt      45   When the image is greater than a given threshold it will
  //                            be reduced in size by an integer such that the new size
  //                            will be smaller than the threshold.  Calc's should result
  //                            in similar values.
  //
  //
  // 2010-11-17            51   Made a mistake in the way that EigenHead is calculated.  As
  //                            a result it is the inverse of what it was so had to
  //                            force all the Feature Datafiles to recalculate.


  #define   CurrentFeatureFileVersionNum  53


  class  ImageFeatures:  public  FeatureVector 
  {
  public:
    typedef  ImageFeatures*  ImageFeaturesPtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    //ImageFeatures (MLClassPtr  mlClass);

    ImageFeatures (int32  _numOfFeatures);

    ImageFeatures (const ImageFeatures&  _image);


    ImageFeatures (      RasterSipper&  _raster,
                   MLClassConstPtr   _mlClass,
                   RasterSipperListPtr  _saveImages = NULL
                  );

    ImageFeatures (const BmpImage&      _image,
                   MLClassConstPtr   _mlClass,
                   RasterSipperListPtr  _saveImages = NULL
                  );


    ImageFeatures (KKStr                _fileName,
                   MLClassConstPtr   _mlClass,
                   bool&                _successfull,
                   RasterSipperListPtr  _saveImages = NULL
                  );


    /**
     *@brief  Smart copy constructor that will detect the underlying type of the source instance.
     *@details
     *@code
     *************************************************************************************
     ** This constructor will detect what the underlying type of 'featureVector' is.     *
     ** If (underlying type is a 'ImageFeatures' object)  then                           *
     **   | Information that is particular to a 'ImageFeatures' object will be extracted *
     **   | from the 'FeatureVector' object.                                             *
     ** else                                                                             *
     **   | Info that is particular to a 'ImageFeatures' object will be set to default   *
     **   | values.                                                                      *
     * ************************************************************************************
     *@endcode
     */
    ImageFeatures (const FeatureVector&  featureVector);

    virtual  ~ImageFeatures ();

    // Access Methods.
    void  CentroidCol      (FFLOAT               _centroidCol)      {centroidCol      = _centroidCol;}
    void  CentroidRow      (FFLOAT               _centroidRow)      {centroidRow      = _centroidRow;}
    void  CtdDateTime      (const KKU::DateTime& _ctdDateTime)      {ctdDateTime      = _ctdDateTime;}
    void  Depth            (FFLOAT               _depth)            {FeatureData (DepthIndex,        _depth);}
    void  Fluorescence     (FFLOAT               _fluorescence)     {FeatureData (FluorescenceIndex, _fluorescence);}
    void  FlowRate1        (FFLOAT               _flowRate1)        {FeatureData (FlowRate1Index,    _flowRate1);}
    void  FlowRate2        (FFLOAT               _flowRate2)        {FeatureData (FlowRate2Index,    _flowRate2);}
    void  Latitude         (double               _laditude)         {latitude         = _laditude;}
    void  Longitude        (double               _longitude)        {longitude        = _longitude;}
    void  NumOfEdgePixels  (int32                _numOfEdgePixels)  {numOfEdgePixels  = _numOfEdgePixels;}
    void  Oxygen           (FFLOAT               _oxygen)           {FeatureData (OxygenIndex,   _oxygen);}
    void  Salinity         (FFLOAT               _salinity)         {FeatureData (SalinityIndex, _salinity);}

    void  SfCentroidCol    (FFLOAT _SfCentroidCol) {sfCentroidCol    = _SfCentroidCol;}
    void  SfCentroidRow    (double _SfCentroidRow) {sfCentroidRow    = _SfCentroidRow;}

    void  Version          (short  _version)       {version          = _version;}


    float          CentroidCol        () const  {return  centroidCol;}    /**< @brief Centroid column with respect to image.  */
    float          CentroidRow        () const  {return  centroidRow;}    /**< @brief Centroid row with respect to image.     */
    FFLOAT         Depth              () const  {return  FeatureData (DepthIndex);}
    KKU::DateTime  CtdDateTime        () const  {return  ctdDateTime;}
    FFLOAT         Fluorescence       () const  {return  FeatureData (FluorescenceIndex);}
    FFLOAT         FlowRate1          () const  {return  FeatureData (FlowRate1Index);}
    FFLOAT         FlowRate2          () const  {return  FeatureData (FlowRate2Index);}
    double         Latitude           () const  {return  latitude;}
    FFLOAT         Length             () const  {return  FeatureData (LengthIndex);}
    double         Longitude          () const  {return  longitude;}
    int32          NumOfEdgePixels    () const  {return  numOfEdgePixels;}
    FFLOAT         Oxygen             () const  {return  FeatureData (OxygenIndex);}
    FFLOAT         Salinity           () const  {return  FeatureData (SalinityIndex);}
    float          SfCentroidCol      () const  {return  sfCentroidCol;}  /**< @brief Centroid column with respect to whole Sipper File. */
    double         SfCentroidRow      () const  {return  sfCentroidRow;}  /**< @brief Centroid row with respect to whole Sipper File.    */
    short          Version            () const  {return  version;}
    FFLOAT         Width              () const  {return  FeatureData (WidthIndex);}

    virtual
    int32  MemoryConsumedEstimated ()  const;

    void  ResetNumOfFeatures (int32    newNumOfFeatures);  // Used to reallocate memory for feature data.
    void  ResetVersion       (short  newVersion);


    void    CalcFeatures (RasterSipper&        srcRaster,
                          RasterSipperListPtr  saveImages = NULL
                         );

    virtual  const char*  UnderlyingClass () const  {return  "ImageFeatures";}


    static  int32  FirstInstrumentDataField;          /**< @brief The first feature field that we get from a seperate instrument data field.  */

    static  int32  NumSeperateInstrumentDataFields;   /**< @brief The number of features that come from Instrument data,  ex: Oxygen or Depth */



    /**
     *@brief  Will parse the name of a Image that was extracted from a SIPPER file into SipperFileName, ScanLineNum, amd ScanCol
     *@param[in] fullFileName Name of file that was assigned by the SIPPER image extraction program. from this name the three fields 
     *  SipperFileName, ScanLineNum, amd ScanCol can be extractd.
     *@param[in] sipperFileName Name of SIPPERfile that the image was orignally extracted from.
     *@param[in] scanLineNum The scan line with respect to the SIPPER file where the first row in the mage was extracted.
     *@param[in] scanCol The left most column in the original SIPPER file that this image  was extracted.
     */
    static  void  ParseImageFileName (const KKStr&  fullFileName,     /**< File name of image extractd from a Sipper File. */
                                      KKStr&        sipperFileName, 
                                      KKU::uint32&  scanLineNum,
                                      KKU::uint32&  scanCol
                                     );

  private:
    static  bool  atExitDefined;

    static  void  FinalCleanUp ();

    static  void  Save (RasterSipper&        raster, 
                        const KKStr&         desc,
                        RasterSipperListPtr  _saveImages = NULL
                       );

    float          centroidCol;     //   cnetroid with just respect to the image.
    float          centroidRow;     //     ""     ""      ""     ""    ""    ""
    KKU::DateTime  ctdDateTime;
    double         latitude;
    double         longitude;
    int32          numOfEdgePixels;
    float          sfCentroidCol;   //   centroid with respect to whole zipper file,
    double         sfCentroidRow;   //     ""     ""      ""     ""    ""    ""
    short          version;         // This is the same versionNumber as in ImageFeaturesList
                                    // It is related to the Feature calculation routine.  This
                                    // will assist in us changing the feature calcs in the 
                                    // future and  objects and methods having a meens of 
                                    // knowing if the features are similar.
           
           
    static  const  int32    SizeThreshold;

    static  short  SizeIndex;                   // 0;
    static  short  Moment1Index;                // 1;
    static  short  Moment2Index;                // 2;
    static  short  Moment3Index;                // 3;
    static  short  Moment4Index;                // 4;
    static  short  Moment5Index;                // 5;
    static  short  Moment6Index;                // 6;
    static  short  Moment7Index;                // 7;

    static  short  EdgeSizeIndex;               // 8;
    static  short  EdgeMoment1Index;            // 9;
    static  short  EdgeMoment2Index;            // 10;
    static  short  EdgeMoment3Index;            // 11;
    static  short  EdgeMoment4Index;            // 12;
    static  short  EdgeMoment5Index;            // 13;
    static  short  EdgeMoment6Index;            // 14;
    static  short  EdgeMoment7Index;            // 15;

    static  short  TransparancyConvexHullIndex; // 16;
    static  short  TransparancyPixelCountIndex; // 17;
    static  short  TransparancyOpen3Index;      // 18;
    static  short  TransparancyOpen5Index;      // 19;
    static  short  TransparancyOpen7Index;      // 20;
    static  short  TransparancyOpen9Index;      // 21;
    static  short  TransparancyClose3Index;     // 22;
    static  short  TransparancyClose5Index;     // 23;
    static  short  TransparancyClose7Index;     // 24;

    static  short  EigenRatioIndex;             // 25
    static  short  EigenHeadIndex;              // 26
    static  short  ConvexAreaIndex;             // 27
    static  short  TransparancySizeIndex;       // 28
    static  short  TransparancyWtdIndex;        // 29

    static  short  WeighedMoment0Index;         // 30
    static  short  WeighedMoment1Index;         // 31
    static  short  WeighedMoment2Index;         // 32
    static  short  WeighedMoment3Index;         // 33
    static  short  WeighedMoment4Index;         // 34
    static  short  WeighedMoment5Index;         // 35
    static  short  WeighedMoment6Index;         // 36
    static  short  WeighedMoment7Index;         // 37

    static  short  Fourier0Index;               // 38;
    static  short  Fourier1Index;               // 49;
    static  short  Fourier2Index;               // 40;
    static  short  Fourier3Index;               // 41;
    static  short  Fourier4Index;               // 42;

    static  short  FdRight1Index;               // 43;
    static  short  FdLeft2Index;                // 44;
    static  short  FdRight2Index;               // 45;
    static  short  FdLeft3Index;                // 46;
    static  short  FdRight3Index;               // 47;
    static  short  FdLeft4Index;                // 48;
    static  short  FdRight4Index;               // 49;
    static  short  FdLeft5Index;                // 50;
    static  short  FdRight5Index;               // 51;
    static  short  FdLeft6Index;                // 52;
    static  short  FdRight6Index;               // 53;
    static  short  FdLeft7Index;                // 54;
    static  short  FdRight7Index;               // 55;
    static  short  FdLeft8Index;                // 56;
    static  short  FdRight8Index;               // 57;

    static  short  ContourFourierDescriptor0;   // 58
    static  short  ContourFourierDescriptor1;   // 59
    static  short  ContourFourierDescriptor2;   // 60
    static  short  ContourFourierDescriptor3;   // 61
    static  short  ContourFourierDescriptor4;   // 62

    static  short  IntensityHist1Index;         // 63
    static  short  IntensityHist2Index;         // 64
    static  short  IntensityHist3Index;         // 65
    static  short  IntensityHist4Index;         // 66
    static  short  IntensityHist5Index;         // 67
    static  short  IntensityHist6Index;         // 68
    static  short  IntensityHist7Index;         // 69

    static  short  LengthVsWidthIndex;          // 70
    static  short  LengthIndex;                 // 71
    static  short  WidthIndex;                  // 72
    static  short  FilledAreaIndex;             // 73

    static  short  FlowRate1Index;              // 74
    static  short  FlowRate2Index;              // 75

    static  short  IntensityHistWS0Index;       // 76
    static  short  IntensityHistWS1Index;       // 77
    static  short  IntensityHistWS2Index;       // 78
    static  short  IntensityHistWS3Index;       // 79
    static  short  IntensityHistWS4Index;       // 80
    static  short  IntensityHistWS5Index;       // 81
    static  short  IntensityHistWS6Index;       // 82
    static  short  IntensityHistWS7Index;       // 83

    static  short  DepthIndex;                  // 84
    static  short  SalinityIndex;               // 85
    static  short  OxygenIndex;                 // 86
    static  short  FluorescenceIndex;           // 87
  };


  typedef  ImageFeatures::ImageFeaturesPtr  ImageFeaturesPtr;





  class  ImageFeaturesList:  public FeatureVectorList
  {
  public: 
    typedef  ImageFeaturesList*  ImageFeaturesListPtr;

    typedef  SipperHardware::InstrumentDataPtr      InstrumentDataPtr;
    typedef  SipperHardware::InstrumentDataListPtr  InstrumentDataListPtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;


    ImageFeaturesList (FileDescPtr  _fileDesc,
                       bool         _owner,
                       RunLog&      _log,
                       int32        _size = 1000
                      );

  private:
    ImageFeaturesList (const ImageFeaturesList&  images);  /**< Will create a duplicate List of images, 
                                                            * in the same order.  If the source 'images'
                                                            * owns its entries, then new duplicate 
                                                            * entries will be created, and will own them 
                                                            * otherwise will only get pointers to existing 
                                                            * instances in 'images'.
                                                            */

  public:
    /**
     *@brief  Constructs a duplicate list of ImageFeature objects and depending on 
     *        '_owner' flag will either duplicate the ImageFeature instances or 
     *        just point to the same instances.
     *@details
     *  Will create a duplicate List of images, in the same order.  If '_owner' = true
     *  will create new instancs of contents and own them.  If 'owner' = false, will 
     *  copy over pointers of existing instances.
     */
    ImageFeaturesList (const ImageFeaturesList&  images,
                       bool                      _owner
                      );

    /**
     *@details
     *  Will create a duplicate List of images, in the same order.  If '_owner' = true will create
     *  new instancs of contents and own them.  If 'owner' = false, will copy over pointers to
     *  existing instances.  If any of the existing instances do not have an underlying class of
     * ImageFeatures;  the function will throw an EXCEPTIION.
     */
    ImageFeaturesList (const FeatureVectorList&  featureVectorList,
                       bool                      _owner
                      );


    /**
     *@brief  Constructor that will extract a list of feature vectors for all the image files in the 
     *        specified directory.
     *@details
     * Will scan the directory _dirName for any image files.  For each image found a new instance of ImageFeatures
     * will be created whos features will be derived from the image.  These ImageFeatures' objects will be 
     * assigned the class specified by '_mlClass'.  A new data file containg the extracted features will be 
     * saved in fileName.
     *
     *@param[in] _log         Log file to write messages to.
     *@param[in] _mlClass  Class to assign to new 'ImageFeatures' objects.
     *@param[in] _dirName     Directory to scan for images.
     *@param[in] _fileName    Name of file to contain the extracted feature data.  Will be og the Raw format.
     */
    ImageFeaturesList (RunLog&             _log,
                       MLClassConstPtr  _mlClass,
                       const KKStr&        _dirName,
                       const KKStr&        _fileName
                      );



    /**
     @brief  constructor that will create a list of images from _images that are assignd one of the 
             classes listed in _mlClasses.
     @details
        Will Create a list of images that are a subset of the ones in _images.  The subset will
        consist of the images who's mlClass is one of the  ones in mlClasses.  We will not own
        any the contents only point to the ones already in _images.
      @param[in] _mlClasses  List of classes that we are intrested in.
      @param[in] _images        Source examples that we want to scan.
      @param[in] _log           
     */
    ImageFeaturesList (MLClassConstList&  _mlClasses,
                       ImageFeaturesList&    _images,
                       RunLog&               _log
                      );


    /**
     @brief
     @details
        This constructor is meant to create a list of 'ImageFeatures' objects from the FeatureVector
        objects contained in featureVectorList.
     @code
     If  'featureVectorList'  owns its contents (that is 'featureVectorList.Owner () == true'  then
        |  We will create new Instances of 'ImageFeatures' objects that we will own.
        |  The underlying class of the 'FeatureVector' objects will be converted to a
        |  'ImageFeatures'  class.
     else
        |  all the 'FeatureVector' objects in 'featureVectorList' must have an underlying class of
        |  'ImageFeatures'.  If one or more do not then the program will halt with a message to
        |  the log.
    @endcode
    */
    ImageFeaturesList (const FeatureVectorList&  featureVectorList);


    virtual  ~ImageFeaturesList ();




    void                   AddSingleImageFeatures (ImageFeaturesPtr  _imageFeatures);  // Same as PushOnBack

    void                   AddQueue (ImageFeaturesList&  imagesToAdd);

    ImageFeaturesPtr       BackOfQueue ();

    ImageFeaturesPtr       BinarySearchByName (const KKStr&  _imageFileName)  const;

    VectorFloat            CalculateDensitesByQuadrat (float        scanRate,         // Scan Lines per Sec.
                                                       float        quadratSize,      // Meters.
                                                       float        defaultFlowRate,  // Meters per Sec
                                                       const bool&  cancelFlag,
                                                       RunLog&      log
                                                      );

    ImageFeaturesListPtr   DuplicateListAndContents ()  const;


    /**
     *@brief  Returns: a list of 'ImageFeatures' objects that have duplicate root file names.
     *@details
     *@code
     ***************************************************************************************************
     ** Returns: a list of 'ImageFeatures' objects that have duplicate root file names.  The returned  *
     ** list will not own these items.  All instances of the duplicate objects will be returned.       *
     ** Ex:  if three insatnces have the same ImageFileName all three will be returned.                * 
     ***************************************************************************************************
     *@endcode
     */
    ImageFeaturesListPtr   ExtractDuplicatesByRootImageFileName ();


    ImageFeaturesListPtr   ExtractImagesForAGivenClass (MLClassConstPtr  _mlClass,
                                                        int32               _maxToExtract = -1,
                                                        FFLOAT              _minSize      = -1.0f
                                                       )  const;




    /**
     *@brief  Calculates the position in meters of each plankton Feature-Vector with respect to the first plankton particle in the SIPPER file.
     *@details It is assumed that all the items in this list are from the same SIPPER file.  Item(FeatureVector) will be assigend a position 
     *         in meters with respecvt to the first particle in the file.
     *
     *@param[in] instrumentData  Provies the flow rate from the instrument data will be used to calc  
     *                           distance traveled; otherwise 'defaultFlowRate' will be used.
     *
     *@param[in] defaultScanRate Scan lines per second that the camera operates at, if <= 0 will assume 24950.0 scan lines/sec.
     *
     *@param[in] defaultFlowRate Flow rate to use if none provided by 'instrumentData'. if <= 0.0 will default to 1.0 meters/sec.
     *
     *@returns A Vector of doubles where each element in it will represent the distance in meters from the first particle in the SIPPER 
     *         file.  There will be one entry for each "ImageFeatures" objet in the list and it wil be sorted from low to high.
     */
    VectorDouble           ExtractPositionsByMeter (InstrumentDataListPtr  instrumentData,
                                                    float                  defaultScanRate,
                                                    float                  defaultFlowRate
                                                   );

    void                   FixSipperFileScanLineAndColFields ();

    void                   FeatureExtraction (const KKStr&       _dirName, 
                                              const KKStr&       _fileName, 
                                              MLClassConstPtr _mlClass
                                             );

    VectorInt              CreateSpatialDistributionHistogram (InstrumentDataListPtr  instrumentData,
                                                               float                  defaultScanRate,   // Scan rate to use if it can not be located from 
                                                               float                  defaultFlowRate,   // Flow rate to use if no instrumentation data.
                                                               float                  bucketSize         // Size of each histogram bucket.
                                                              );

    ImageFeaturesPtr       IdxToPtr (int32 idx) const;

    ImageFeaturesPtr       LookUpByImageFileName (const KKStr&  _imageFileName)  const;

    ImageFeaturesPtr       LookUpByRootName (const KKStr&  _rootName);

    /**
     *@brief  return a list of examples that consist and are in the same 
     *        order as the list of ImageFileNames in the file 'fileName'.
     *@details
     *  Using list of ImageFileNames in a file('fileName') create a new
     *  ImageFeaturesList instance with images in order based off contents
     *  of file. If error occurs will return NULL.
    */
    ImageFeaturesListPtr   OrderUsingNamesFromAFile (const KKStr&  fileName);

    ImageFeaturesPtr       PopFromBack ();

    void                   PrintSpatialHistogramReport (std::ostream&          r,
                                                        InstrumentDataListPtr  instrumentData,
                                                        float                  defaultScanRate,   // Scan rate to use if it can not be located from 
                                                        float                  defaultFlowRate,   // Flow rate to use if no instrumentation data.
                                                        float                  bucketSize
                                                       );

    void                   RecalcFeatureValuesFromImagesInDirTree (KKStr  rootDir,
                                                                   bool&  successful
                                                                  );

    void                   SortBySpatialDistance ();

    ImageFeaturesListPtr   StratifyAmoungstClasses (int32  numOfFolds);


    ImageFeaturesListPtr   StratifyAmoungstClasses (MLClassConstListPtr  mlClasses,
                                                    int32                   maxImagesPerClass,
                                                    int32                   numOfFolds
                                                   );


    short                  Version () const  {return  version;}
   
    void                   Version (short  _version)  {version = _version;}

    virtual  const char*   UnderlyingClass ()  const  {return  "ImageFeaturesList";}


    class  const_iterator
    {
    private:
      FeatureVectorList::const_iterator  idx;
    
    public:
      const_iterator ():
          idx ()
      {
      }


      const_iterator (const const_iterator&  ivConst_Iterator):
          idx (ivConst_Iterator.idx)
      {
      }


      const_iterator (const FeatureVectorList::const_iterator&  fvConst_Iterator):
          idx (fvConst_Iterator)
      {
      }


      const_iterator (const FeatureVectorList::iterator&  fvIterator):
          idx (fvIterator)
      {
      }


      const ImageFeaturesPtr  operator*()
      {
        //FeatureVectorPtr  fv = FeatureVectorList::iterator::operator*();
        //return  (ImageFeaturesPtr)(fv);
        return  (const ImageFeaturesPtr)*idx;
      }


      const_iterator&   operator= (const const_iterator&  right)
      {
        idx = right.idx;
        return  *this;
      }



      const_iterator&   operator= (const FeatureVectorList::iterator&  right)  
      {
        idx = right;
        return *this;
      }


      const_iterator&   operator= (const FeatureVectorList::const_iterator&  right)  
      {
        idx = right;
        return *this;
      }

      
      bool  operator!= (const const_iterator&  right)  const
      {
        return  idx != right.idx;
      }


      bool  operator!= (const FeatureVectorList::iterator&  right)  const
      {
        return  idx != (FeatureVectorList::const_iterator)right;
      }


      bool  operator!= (const FeatureVectorList::const_iterator&  right)  const
      {
        return  idx != right;
      }


      bool  operator== (const const_iterator&  right)  const
      {
        return  idx == right.idx;
      }


      bool  operator== (const FeatureVectorList::iterator&  right)  const
      {
        return  (idx == right);
      }


      const_iterator&   operator++ (int  x)
      {
        idx++;
        return  *this;
      }


      const_iterator&   operator++ ()
      {
        ++idx;
        return  *this;
      }
    };  /* const_iterator */





    class  iterator
    {
    private:
      FeatureVectorList::iterator  idx;
    
    public:
      iterator ():
          idx ()
      {
      }

      iterator (const iterator&  idx):
          idx (idx.idx)
      {
      }


      iterator (const FeatureVectorList::iterator&  idx):
          idx (idx)
      {
      }


      ImageFeaturesPtr  operator*()
      {
        //FeatureVectorPtr  fv = FeatureVectorList::iterator::operator*();
        //return  (ImageFeaturesPtr)(fv);
        return  (ImageFeaturesPtr)*idx;
      }

      iterator&   operator= (const iterator&  right)
      {
        idx = right.idx;
        return  *this;
      }

      bool  operator!= (const iterator&  right)  const
      {
        return  idx != right.idx;
      }


      bool  operator!= (const FeatureVectorList::iterator&  right)  const
      {
        return  idx != (FeatureVectorList::const_iterator)right;
      }


      bool  operator!= (const FeatureVectorList::const_iterator&  right)  const
      {
        return  idx != right;
      }




      bool  operator== (const iterator&  right)  const
      {
        return  idx == right.idx;
      }


      iterator&   operator++ (int x)
      {
        idx++;
        return  *this;
      }


      iterator&   operator++ ()
      {
        ++idx;
        return  *this;
      }
    };


    
    //iterator  begin ()  {return  KKQueue<FeatureVector>::begin ();}


  private:
    float  FlowRate (InstrumentDataPtr  id,
                     float              defaultFlowRate
                    );



    short          version;        // Represents the version of the Feature data,  when ever I update
                                   // the way Feastures are calculated I increment CurrentFeatureFileVersionNum
                                   // by 1.   This way if we load a older FeatureData file we can be aware
                                   // of this.  Methods like FeatureDataReSink will force the recalculation
                                   // of Feature data if not up-to-date.  Also works in coordination
                                   // with the version field in the ImageFeatures object.  A value of
                                   // 0 indicates that we do not know what Version the feature data is.
                                   // This can happen when not all the ImageFeatures objects in the list 
                                   // have the same version number.


    class  SpatialComparison
    {
    public:
      SpatialComparison () {}

      bool  operator()  (FeatureVectorPtr  p1,
                         FeatureVectorPtr  p2
                        )
      {
        double  sl1 = 0.0;
        double  sl2 = 0.0;

        if  (strcmp (p1->UnderlyingClass (), "ImageFeatures") == 0)
        {
          ImageFeaturesPtr i1 = dynamic_cast<ImageFeaturesPtr>(p1);
          sl1 = i1->SfCentroidRow (); 
        }

        if  (strcmp (p2->UnderlyingClass (), "ImageFeatures") == 0)
        {
          ImageFeaturesPtr i2 = dynamic_cast<ImageFeaturesPtr>(p2);
          sl2 = i2->SfCentroidRow (); 
        }


        return  (sl1 < sl2);
      }
    };

  };  /* ImageFeaturesList */

  typedef  ImageFeaturesList::ImageFeaturesListPtr  ImageFeaturesListPtr;


}  /* namespace MLL */

#endif

