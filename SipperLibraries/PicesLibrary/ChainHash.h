#ifndef _CHAINHASH_
#define _CHAINHASH_

#include  <math.h>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

#include <time.h>
#include <stdio.h>

#include "OSservices.h"

#include "Attribute.h"
#include "FeatureNumList.h"
#include "FileDesc.h"
#include "FeatureVector.h"
#include "svm.h"

namespace MLL 
{

  class  ChainHashNode;
  typedef  ChainHashNode*  ChainHashNodePtr;


  struct svm_node_int
  {
    KKU::int32 index;
    KKU::int32 value;
  };



  template<class T>
  class ChainHash 
  {
  typedef  T* Tptr;
  typedef  KKU::int32  int32;
  typedef  KKU::uint32 uint32;

  typedef  std::list<T*>  Tlist;

  public:
    ChainHash (const T&  notFound, 
               int32     size = 10007
              );

    ChainHash (const T& notFound);

    ~ChainHash ( );

    void  DoSomeStats ();

    T*    find (const T& x) const;  // returns NULL if not found.

    void  makeEmpty ();

    void  insert(T*  x);

    void  remove(const T &x);

    vector<list<int32> >&  getBucketList ();

    ChainHashNodePtr            GetNodeForExample (FeatureVectorPtr  example);
    FeatureVectorPtr            GetImageForIndexNum (int32  indexNum);
    vector< vector <KKStr > >*  GroupExamples ();
    
  private:
    int32                 tableSize;
    Tlist*                bucket;
    const T&              NOT_FOUND;
    int32                 count;
    vector<list<int32> >  bucketList;
    vector<T*>            nodeList;    /**< Where we keep a single list of all nodes
                                        * inserted,  this way we will be able to
                                        * clean up when being deleted.
                                        */
  };





  /* set hashSize, length and init first before hashing */
  class ChainHashNode
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    // this constructor is here for the notFound node - KNS
    ChainHashNode ( int32        _example_index,
                    svm_node_int *example_features, 
                    int32        _example_class_label
                   );

    ChainHashNode (const ChainHashNode& right);

    ChainHashNode ( int32                  _example_index,
                    int32                  _example_class_label,
                    FeatureVectorPtr       _example,
                    bool&                  _sucessfull
                  );

    ~ChainHashNode ();


    int32  hashKey() const;


    // returns the prob.x array index (or in other words, the original index number of 
    // the example in the non-compressed list
    inline int32 getIndex() const  {return index;}


    // The bucketList is a list lists containing integers that are indices into the original 
    // example list. The pos number is the new index for each example in the list at that position
    // This list may be redundant. - KNS
    inline void setBucketListIndex (int32 pos)  {bucketListIndex = pos;}


    // returns the position in the new example list of the the node in question
    inline int32 getBucketListIndex () const  {return  bucketListIndex;}

    FeatureVectorPtr  Image ()   {return example;}

    bool operator==(const ChainHashNode& right) const;

    static void  setLength (int32 len);

    static void  setHashSize (int32 hs);

    static void  SetSelectedFeatures (const FeatureNumList&  _selectedFeatures);

    static void  SetFeatureTypes (const MLL::AttributeTypeVector&  _featureTypes);

    static void  SetBitsToReduceByFeature (const VectorInt32&  _bitsToReduceByFeature);

    static void  init();


  private:
    int32 index;
    int32 label;
    int32 bucketListIndex;

    FeatureVectorPtr  example;
    int32*            int_x;        // Will only contain selected features.
    //bool              craponstick;

    static MLL::AttributeTypeVector  featureTypes;
    static int32                     hashSize;
    static int32                     length;

    static int32                     numSelectedFeatures;  // I know this is breaking some rules by
    static int32*                    selectedFeatures;     // not using FeatureNumList, but I really
                                                           // want some speed, speed, and speed.

    static VectorInt32               bitsToReduceByFeature;
  };


  typedef  ChainHashNode*  ChainHashNodePtr;
  #endif




  template<class T>
  ChainHash<T>::ChainHash (const T&  notFound, 
                           int32     size
                          )
    :
      tableSize (size), 
      bucket    (new Tlist[size]), 
      NOT_FOUND (notFound),
      count     (0)
  {
  }


  template<class T>
  ChainHash<T>::ChainHash (const T& notFound)
    :
      NOT_FOUND (notFound),
      count(0)
  {
  }


  template<class T>
  ChainHash<T>::~ChainHash ( )
  {
    typename  vector<T*>::iterator  idx;

    for  (idx = nodeList.begin ();  idx != nodeList.end ();  idx++)
      delete (*idx);

    delete[] bucket;
    bucket = NULL;
  }



  template<class T>
  T*   ChainHash<T>::find (const T &x) const
  {
    int32 index = x.hashKey ();

    if  (bucket[index].size() == 0)
      return NOT_FOUND;

    typename  Tlist::iterator it;

    for  (it = bucket[index].begin ();  it != bucket[index].end ();  it++)
    {
      T*  t = *it;
      if  (*t == x)
        return t;
    }

    return  NULL;
  }  /* find */




  template<class T>
  void  ChainHash<T>::makeEmpty ()
  {
    for  (int32 i = 0;  i < tableSize;  i++)
    {
      typename Tlist::iterator idx;
      for  (idx = bucket[i].begin ();  idx !=  bucket[i].end ();  idx++)
      {
        delete *idx;
        *idx = NULL;
      }

      bucket[i].clear();
    }
  }  /* makeEmpty */




  template<class T>
  void  ChainHash<T>::insert (T*  x)
  {
    if  (!x)
    {
      cerr << endl 
           << endl
           << "ChainHash<T>::insert   ERROR x == NULL" << endl
           << endl;
      osWaitForEnter ();
    }

    nodeList.push_back (x);

    int32 index = x->hashKey();

    if  (bucket[index].size() == 0)
    {
      // There are no nodes tha have this hash key.

      //update the position of x in bucketList
      x->setBucketListIndex(count);   // This node must be unique so we are going
      count++;                        // to give it its own bucketList entry
      bucket[index].push_back(x);
      list<int32> l;
      l.push_back (x->getIndex());
      bucketList.push_back(l);
    }
    else
    {
      typename Tlist::iterator  it;
      for  (it = bucket[index].begin();  it != bucket[index].end();  it++)
      {
        if  (**it == *x)
          break;
      }

      // If x has a duplicate in the bucketList, then add it to the list of images to be mashed 
      // at the corresponding position in the compressed example list. By duplicate, we mean that 
      // all of the integer features are equivalent for both nodes.
      //
      // IN OTHER WORDS, this is the binning process responsible for the compression. This test
      // determines which images get combined.

      if (it != bucket[index].end())
      {
        int32 pos = (**it).getBucketListIndex ();
        bucketList[pos].push_back(x->getIndex());
        x->setBucketListIndex (pos);
      }
      else
      {
        x->setBucketListIndex (count);
        count++;
        bucket[index].push_back (x);
        list<int32> l;
        l.push_back(x->getIndex ());
        bucketList.push_back (l);
      }
    }
  }  /* insert */



  template<class T>
  void  ChainHash<T>::DoSomeStats ()
  {
    int32   c     = 0;
    int32   total = 0;
    int32   x;

    for  (x = 0;  x < tableSize;  x++)
    {
      if  (bucket[x].size () > 0)
      {
        c++;
        total += (int32)bucket[x].size ();
      }
    }

    float  ussage = (float)c / (float)tableSize;
    float  mean = (float)total / (float)c;

    float  varTot = 0;
    for  (x = 0;  x < tableSize;  x++)
    {
      if  (bucket[x].size () > 0)
      {
        float  delta = mean - bucket[x].size ();
        varTot += delta * delta;
      }
    }

    float  var    = varTot / (float)c;
    float  stdDev = sqrt (var);


    /*
    int32 nodesInBucketList = 0;
    {
      vector<list<int32> >::const_iterator  idx;

      for  (idx = bucketList.begin ();  idx != bucketList.end ();  idx++)
      {
        list<int32>::const_iterator  idx2;

        for (idx2 = (*idx).begin ();  idx2 != (*idx).end ();  idx2++)
        {
          nodesInBucketList++;
        }
      }
    }
    */


    cout << endl
         << "Buckets Used     [" << c << "]" << endl
         << "Ussage           [" << StrFormatDouble (ussage * 100.0f, "ZZ0.00000") << "%]" << endl
         << "Mean             [" << StrFormatDouble (mean,   "ZZ0.00000")          << "]"  << endl
         << "Var              [" << StrFormatDouble (var,    "ZZZ,ZZ0.00000")      << "]"  << endl
         << "StdDev           [" << StrFormatDouble (stdDev, "ZZZ,ZZ0.00000")      << "]"  << endl
  //       << "BucketList.size  [" << bucketList.size ()                             << "]"  << endl
  //       << "NodesInBucketList[" << nodesInBucketList                              << "]"  << endl
         << endl;

  }  /* DoSomeStats */






  template<class T>
  void  ChainHash<T>::remove (const T &x)
  {
    int32 index = x.hashKey();

    typename Tlist::iterator  it;

    for  (it = bucket[index].begin();  it != bucket[index].end();  it++)
    {
      if  (**it == x)
      {
        bucket[index].erase (it);
        break;
      }
    }
  }  /* remove */




  template<class T>
  vector<list<int32> >&  ChainHash<T>::getBucketList()
  {
    return  bucketList;
  }


  template<class T>
  ChainHashNodePtr  ChainHash<T>::GetNodeForExample (FeatureVectorPtr  example)
  {
    typename  vector<T*>::iterator  idx;

    for  (idx = nodeList.begin ();  idx != nodeList.end ();  idx++)
    {
      ChainHashNodePtr  n = *idx;
      if  (n->example == example)
        return n;
    }

    return  NULL;
  }  /* GetNodeForExample */




  template<class T>
  FeatureVectorPtr  ChainHash<T>::GetImageForIndexNum (int32  indexNum)
  {
    typename  vector<T*>::iterator  idx;

    for  (idx = nodeList.begin ();  idx != nodeList.end ();  idx++)
    {
      ChainHashNodePtr  n = *idx;
      if  (n->getIndex () == indexNum)
        return n->Image ();
    }

    return  NULL;
  }  /* GetNodeForExample */





  template<class T>
  vector< vector <KKStr > >*  ChainHash<T>::GroupExamples ()
  {
    vector<list<int32> >::const_iterator  idx1;


    vector< vector <KKStr > >*  allGroups = new vector< vector <KKStr > > ();

    for  (idx1 = bucketList.begin ();  idx1 != bucketList.end ();  idx1++)
    {
      list<int32>   groupIndexes = *idx1;

      vector<KKStr>  groupList;

      list<int32>::const_iterator idx2;

      for  (idx2 = groupIndexes.begin ();  idx2 != groupIndexes.end ();  idx2++)
      {
        FeatureVectorPtr  example = GetImageForIndexNum (*idx2);
        if  (!example)
        {
          cerr << endl << endl << "*** ERROR ***  ChainHash<T>::GroupExamples ()" << endl
               << endl
               << "     *** Could not locate example for Index[" << *idx2 << "]" << endl
               << endl;
        }
        else
        {
          groupList.push_back (example->ImageFileName ());
        }
      }

      allGroups->push_back (groupList);
    }

    return  allGroups;
  }  /* GroupExamples; */




  void  CompareTwoSetsOfGroups (vector< vector <KKStr > >&  groups1,
                                vector< vector <KKStr > >&  groups2,
                                std::ostream&                r
                               );



}  /* namespace MLL */


