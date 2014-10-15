#ifndef  _PASSASSIGNMENTS_
#define  _PASSASSIGNMENTS_


#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "RunLog.h"


using namespace  MLL;


class  PassAssignments 
{
public:
  PassAssignments (FileDescPtr         _fileDesc,
                   KKStr              _fileName,
                   FeatureVectorList&  _images,
                   RunLog&             _log,
                   bool&               _successfull
                  );


  PassAssignments (FileDescPtr         _fileDesc,
                   FeatureVectorList&  _images,
                   MLClassList&     _imageClasses,
                   int                 _numOfRandomPasses,
                   int                 _initialTrainingImagesPerClass,
                   bool                _usePriorForIIPC,
                   RunLog&             _log
                  );

  ~PassAssignments ();



  void  ExtractInitialTrainingAndTestImages (int                    pass,
                                             FeatureVectorList&     masterList,
                                             MLClassList&        imageClasses,
                                             FeatureVectorListPtr&  initialTraingImages,
                                             FeatureVectorListPtr&  testImages
                                            );
  
  void  Save (KKStr  _fileName);


private:

  void  AllocateAssignments ();

  vector<float>  DerivePriorWeights (MLClassList&    imageClasses,
                                     FeatureVectorList& images,
                                     bool               usePriorForIIPC
                                    );
 
  void  FreeUpAssignments ();

  bool  ImageInPass (const KKStr&  imageFileName, 
                     int            pass
                    );

  void  PopulateImagesPerClass (FeatureVectorList&  images);

  void  Read (KKStr              fileName,
              FeatureVectorList&  images,
              bool&               successfull
             );



  KKStrPtr**      assignments;

  FileDescPtr     fileDesc;
  

  // Class List that was initialized with, will not own the
  // entries,  but will keep order, so as to be able to know
  // wich element in imagesPerClass matches with which class
  MLClassList  imageClasses;

  // Will be allocated to same size as imagesClasses.QueueSize ()
  // Reflect the number of images in each given class.  Only 
  // Reflect the number of images in each given class.  Only 
  int*            imagesPerClass;

  int             initialTrainingImagesPerClass;
  RunLog&         log;
  int             numOfClasses;

  int             numOfRandomPasses;
  int             totalNumOfInitialTrainingImages;
  bool            usePriorForIIPC;
};


typedef  PassAssignments*  PassAssignmentsPtr;

#endif

