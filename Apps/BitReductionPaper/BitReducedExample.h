#ifndef  _BITREDUCEDEXAMPLE_
#define  _BITREDUCEDEXAMPLE_

#include  "FileDesc.h"
#include  "FeatureVector.h"
#include  "RBTree.h"
#include  "RunLog.h"



class  BitReducedExample;
typedef  BitReducedExample*  BitReducedExamplePtr;


class  BitReducedExampleKey;
typedef  BitReducedExampleKey*  BitReducedExampleKeyPtr;

//  The purpose of this class is to help do an analyis of the behavior that is going on
//  at different levels of compresion.


class BitReducedExample
{
public:
  BitReducedExample (FeatureVector&  example,
                     FileDescPtr     fileDesc,
                     int             bitReduction
                    );


  ~BitReducedExample ();

  FeatureVectorListPtr  OrigImages ()  {return origImages;}


  int  Compare (const BitReducedExample&  right)  const
  {
    int  z = Max (numOfFeatures, right.numOfFeatures);
    int  x;

    for  (x = 0; x < z;  x++)
    {
      if  (featureData[x] > right.featureData[x])
        return 1;

      else if  (featureData[x] < right.featureData[x])
        return -1;
    }

    return 0;
  }


  BitReducedExample&  operator+= (const BitReducedExample&  right)
  {
    origImages->AddQueue (*(right.origImages));
    return  *this;
  }


  bool  operator== (const BitReducedExample&  right) const
  {
    return  (Compare (right) == 0);
  }


  bool  operator> (const BitReducedExample&  right)  const
  {
    return  (Compare (right) > 0);
  }

  bool  operator<  (const BitReducedExample&  right)  const
  {
    return  (Compare (right) < 0);
  }

  BitReducedExampleKeyPtr  NodeKey ()  const  {return nodeKey;}


private:
  int*                      featureData;
  BitReducedExampleKeyPtr   nodeKey;
  int                       numOfFeatures;
  FeatureVectorListPtr      origImages;
};  /* BitReducedExample */


typedef  BitReducedExample*  BitReducedExamplePtr;




class  BitReducedExampleKey
{
public:
  BitReducedExampleKey (const BitReducedExample&  _bitRedExample):
    bitRedExample (_bitRedExample)
    {}

  bool  operator< (const BitReducedExampleKey&  right)  const
  {
    return  bitRedExample < right.bitRedExample;
  }

  bool  operator> (const BitReducedExampleKey&  right)  const
  {
    return  bitRedExample > right.bitRedExample;
  }

  bool  operator== (const BitReducedExampleKey&  right)  const
  {
    return  bitRedExample == right.bitRedExample;
  }

private:
  const BitReducedExample&  bitRedExample;
};

typedef  BitReducedExampleKey*  BitReducedExampleKeyPtr;






class  BitReducedExampleComparator
{
public:
   const BitReducedExampleKey&  ExtractKey (BitReducedExample*  entry)
   {
      return  *(entry->NodeKey ());
   }
};




class  BitReducedExampleTree: public  RBTree<BitReducedExample, BitReducedExampleComparator, BitReducedExampleKey>
{
public:
  BitReducedExampleTree (FeatureVectorList&  examples,
                         int                 _bitReduction,
                         FileDescPtr         _fileDesc,
                         RunLog&             _log
                        );

  ~BitReducedExampleTree ();

  void  Add (FeatureVector&  image);

  void  DeriveDuplicateCounts  (VectorInt&  crossClassCounts,
                                VectorInt&  crossClassCountImages
                               );

  BitReducedExamplePtr  GetEqual (BitReducedExamplePtr  image);


private:
  int                bitReduction;
  FileDescPtr        fileDesc;
  MLClassListPtr  mlClasses;
  RunLog&            log;
  int                numDuplicateExamples;
};


typedef  BitReducedExampleTree*  BitReducedExampleTreePtr;


#endif

