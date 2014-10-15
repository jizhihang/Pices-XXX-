#ifndef  _IMAGEFEATURESNAMEINDEXED_
#define  _IMAGEFEATURESNAMEINDEXED_


#include  "RBTree.h"
#include  "Str.h"


namespace MLL {

#ifndef  _FeatureVectorDefined_
class  FeatureVector;
typedef  FeatureVector*  FeatureVectorPtr;
#endif

#ifndef  _FeatureVectorListDefined_
class  FeatureVectorList;
typedef  FeatureVectorList*  FeatureVectorListPtr;
#endif



class  ExtractImageFileName;


class  ImageFeaturesNameIndexed: public  RBTree<FeatureVector, ExtractImageFileName, KKStr>
{
public:
  ImageFeaturesNameIndexed ();

  ImageFeaturesNameIndexed (FeatureVectorList&  images);

private:
};


typedef  ImageFeaturesNameIndexed*  ImageFeaturesNameIndexedPtr;





class  ExtractImageFileName
{
public:
   KKStr  ExtractKey (FeatureVectorPtr  image);
};

}  /* namespace MLL */


#endif

