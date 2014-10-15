#ifndef  _DUPLICATEIMAGES_
#define  _DUPLICATEIMAGES_

/** 
 *@class MLL::DuplicateImages
 *@author  Kurt Kramer
 *@brief   Detects duplicate images in a given FeaureVectorList objects.
 *@details  Will derive a list of duplicate FeatureVector objects in a given list.  It will 
 *          use both the Image File Name and feature data to detect duplicates.  A duplicate
 *          can be detected in two ways.  If two or more entries have the same ImageFileName  
 *          or FeatureData.
 *         
 *          The simplest way to use this object is to create an instance with a FeatureVectorList 
 *          object that you are concerend with.  Then call the method DupImages (), which will 
 *          return the list of dupliactes found via a structure called DuplicateImageList.
 */


#include  "RunLog.h"
#include  "FeatureVector.h"


namespace MLL 
{

  class  DuplicateImageList;
  typedef  DuplicateImageList*  DuplicateImageListPtr;

  class  DuplicateImage;
  typedef  DuplicateImage*  DuplicateImagePtr;



  #ifndef  _IMAGEFEATURESDATAINDEXED_
  class  ImageFeaturesDataIndexed;
  typedef  ImageFeaturesDataIndexed*  ImageFeaturesDataIndexedPtr;
  #endif


  #ifndef  _FEATUREVECTOR_
  class FeatureVector;
  typedef  FeatureVector* FeatureVectorPtr;
  class FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif

  #ifndef  _IMAGEFEATURESNAMEINDEXED_
  class  ImageFeaturesNameIndexed;
  typedef  ImageFeaturesNameIndexed* ImageFeaturesNameIndexedPtr;
  #endif




  class DuplicateImages
  {
  public:
    /**
     *@brief  You would use this instance to search for duplicates in the list of 'examples'.
     *@details  You can still call 'AddExamples' and 'AddSingleImage'; 
     */
    DuplicateImages (FeatureVectorListPtr  _examples,
                     RunLog&               _log
                    );


    DuplicateImages (FileDescPtr  _fileDesc,
                     RunLog&      _log
                    );

    ~DuplicateImages ();


    /** @brief  Will add all the examples; be carefull of ownership. */
    bool                   AddExamples (FeatureVectorListPtr  examples);

    /**
     *@brief  Add one more FeatureVector to the list.
     *@details  Will add one more image to list and if it turns out to be a duplicate will 
     *          return pointer to a "DuplicateImage" structure that will contain a list of 
     *          all images that it is duplicate to. If no duplicate found will then return 
     *          a NULL pointer.      
     *@param[in]  image  FeatureVecvtor that you want to add to the list.  
     */
    DuplicateImagePtr      AddSingleImage (FeatureVectorPtr  image);

    DuplicateImageListPtr  DupImages () const {return dupExamples;}

    int32                  DuplicateCount     ()  const {return duplicateCount;}
    int32                  DuplicateDataCount ()  const {return duplicateDataCount;}
    int32                  DuplicateNameCount ()  const {return duplicateNameCount;}

    bool                   DuplicatesFound ()  const;

    bool                   ExampleInDetector (FeatureVectorPtr    fv);

    FeatureVectorListPtr   ListOfExamplesToDelete ();

    void                   PurgeDuplicates (FeatureVectorListPtr  examples,
                                            ostream*              report
                                           );  /**<  if not equal NULL will list examles being purged. */

    void                   ReportDuplicates (ostream&  o);


  private:
    void  FindDuplicates (FeatureVectorListPtr  examples);  /**< Used to build duplicate list from current contents of examples. */

    int32                        duplicateCount;
    int32                        duplicateDataCount;
    int32                        duplicateNameCount;
    DuplicateImageListPtr        dupExamples;
    ImageFeaturesDataIndexedPtr  featureDataTree;
    FileDescPtr                  fileDesc;
    RunLog&                      log;
    ImageFeaturesNameIndexedPtr  nameTree;
  };


  typedef  DuplicateImages*  DuplicateImagesPtr;




  class  DuplicateImage
  {
  public:
    DuplicateImage (FileDescPtr       _fileDesc,
                    FeatureVectorPtr  _image1,
                    FeatureVectorPtr  _image2,
                    RunLog&           _log
                   );

    ~DuplicateImage ();

    void  AddADuplicate (FeatureVectorPtr  image);

    bool                    AllTheSameClass ();

    bool                    AlreadyHaveImage (FeatureVectorPtr image);

    const 
      FeatureVectorListPtr  DuplicatedImages ()  {return &duplicatedImages;}

    FeatureVectorPtr        FirstImageAdded  ()  {return firstImageAdded;}

    FeatureVectorPtr        ImageWithSmallestScanLine (); 

  private:
    FileDescPtr        fileDesc;
    FeatureVectorList  duplicatedImages;
    FeatureVectorPtr   firstImageAdded;
  };





  typedef  DuplicateImage*  DuplicateImagePtr;




  class  DuplicateImageList: public KKQueue<DuplicateImage>
  {
  public:
    DuplicateImageList (bool _owner = true, 
                        int32  _size  = 100
                       );
    ~DuplicateImageList ();

    DuplicateImagePtr  LocateByImage (FeatureVectorPtr  image);

  private:
  };

  typedef  DuplicateImageList*  DuplicateImageListPtr;

}  /* namespace MLL */

#endif

