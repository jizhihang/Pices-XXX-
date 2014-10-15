#include "FirstIncludes.h"

#include <fstream>
#include <iostream>
#include <list>
#include <string.h>
#include <string>
#include <vector>

#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace std;
using namespace KKU;


#include "ChainHash.h"
#include "FeatureNumList.h"

using namespace MLL;


// this constructor is here for the notFound node - KNS
ChainHashNode::ChainHashNode (int32          _example_index,
                              svm_node_int*  example_features,
                              int32          _example_class_label
                             )
  :
    index               (_example_index),
    label               (_example_class_label),
    bucketListIndex     (-1),
    example             (NULL),
    int_x               (NULL)
{
}


ChainHashNode::ChainHashNode (const ChainHashNode& right):

    index               (right.index),
    label               (right.label),
    bucketListIndex     (right.bucketListIndex),
    example             (right.example),
    int_x               (NULL)

{
  int_x = new int32[length];
  memcpy (int_x, right.int_x, sizeof(int32)*length);
}



ChainHashNode::ChainHashNode (int32            _example_index,
                              int32            _example_class_label,
                              FeatureVectorPtr _example,
                              bool&            _sucessfull
                             )
  :
    index           (_example_index),
    label           (_example_class_label),
    bucketListIndex (-1),
    example         (_example),
    int_x           (NULL)
{
  if (!example)
  {
    _sucessfull = false;
    return;
  }

  int_x = new int32[length];
  if (!int_x)
  {
    _sucessfull = false;
    return;
  }

  int32 temp;

  const FFLOAT*  feature_data = example->FeatureData();
  for (int32 i=0; i < numSelectedFeatures; i++)
  {
    int32  feature_num = selectedFeatures[i];

    if  ((featureTypes[feature_num] != NominalAttribute)  &&  (featureTypes[feature_num] != SymbolicAttribute))
    {
      temp = int32 (1000 * feature_data[feature_num]);
      if (temp > 0)
        int_x[i] = temp >> bitsToReduceByFeature[i];
      else
        int_x[i] = -1 * ((-1*temp) >> bitsToReduceByFeature[i]);
    }
    else
    {
      int_x[i] = (int32)feature_data[feature_num];
    }
  }

  _sucessfull = true;
}



ChainHashNode::~ChainHashNode ( )
{
  if (int_x)
  {
    delete [] int_x;
  }
}


int32  ChainHashNode::hashKey ()  const
{
  int32 sum = 0;

  for (int32 i=0; i < numSelectedFeatures; i++)
  {
    //sum += int_x[i];
    //sum = sum % hashSize;

    sum = (sum << 2) + int_x[i];
    sum = sum % hashSize;
  }

  return abs(sum);
}  /* hashKey */




bool  ChainHashNode::operator==(const ChainHashNode& right) const
{
  // if the class labels are not the same, then the two cannot be equal
  if  (label != right.label)
  {
    return false;
  }

  if (!int_x || !right.int_x || example->NumOfFeatures() != right.example->NumOfFeatures())
  {
    return false;
  }


  for (int32 i=0; i < numSelectedFeatures; i++)
  {
    if  (int_x[i] != right.int_x[i])
      return false;
  }

  return true;
}  /* operator== */




void  ChainHashNode::setLength (int32 len)
{
  length = len;
}



void  ChainHashNode::setHashSize (int32 hs)
{
  hashSize = hs;
}



void  ChainHashNode::SetSelectedFeatures (const FeatureNumList&  _selectedFeatures)
{
  if  (selectedFeatures)
    delete  selectedFeatures;

  numSelectedFeatures = _selectedFeatures.NumOfFeatures ();
  selectedFeatures    = new int32[_selectedFeatures.NumOfFeatures ()];
  for  (int32 x = 0;  x < numSelectedFeatures;  x++)
    selectedFeatures[x] = _selectedFeatures[x];
}



void  ChainHashNode::SetFeatureTypes (const AttributeTypeVector&  _featureTypes)
{
  featureTypes = _featureTypes;
}



void  ChainHashNode::SetBitsToReduceByFeature (const VectorInt32&  _bitsToReduceByFeature)
{
  bitsToReduceByFeature = _bitsToReduceByFeature;
}





// initialize static variables. Visual Studios freaks out if you try to do this initialization
// in the header file, thus we have a 3 line implementation file
AttributeTypeVector ChainHashNode::featureTypes;
int32               ChainHashNode::hashSize = 0;
int32               ChainHashNode::length = 0;
int32               ChainHashNode::numSelectedFeatures  = 0;
int32*              ChainHashNode::selectedFeatures = NULL;
VectorInt32         ChainHashNode::bitsToReduceByFeature;



vector<KKStr>  GetGroupForExample (vector< vector <KKStr > >&  groups,
                                   KKStr                       example
                                  )
{
  vector< vector <KKStr > >::const_iterator  idx1;

  for  (idx1 = groups.begin ();  idx1 != groups.end ();  idx1++)
  {
    vector <KKStr >::const_iterator  idx2;
    for  (idx2 = (*idx1).begin ();  idx2 != (*idx1).end ();  idx2++)
    {
      if  (*idx2 == example)
        return  *idx1;
    }
  }

  return  vector<KKStr> ();
}  /* GetGroupForExample */




int32  LookUpExampleInGroup (vector<KKStr>&   g,
                             KKStr            i
                          )
{
  int32  idx;

  for  (idx = 0;  idx < int32 (g.size ());  idx++)
  {
    if  (i == g[idx])
      return idx;
  }

  return -1;
}  /* LookUpExampleInGroup */




void  CompareTwoGroups (vector<KKStr>&  g1,
                        vector<KKStr>&  g2,
                        std::ostream&              r
                       )

{
  if  (g1.size () != g2.size ())
  {
    r << "Groups are of different size." << endl;
  }

  vector<KKStr>::const_iterator  idx;

  for  (idx = g1.begin ();  idx != g1.end ();  idx++)
  {
    KKStr i = *idx;
    int32  group2Idx = LookUpExampleInGroup (g2, i);
    if  (group2Idx < 0)
    {
      r << "Example[" << i << "] is in group1 but not in group2" << endl;
    }
  }


  for  (idx = g2.begin ();  idx != g2.end ();  idx++)
  {
    KKStr i = *idx;
    int32  group1Idx = LookUpExampleInGroup (g1, i);
    if  (group1Idx < 0)
    {
      r << "Example[" << i << "] is in group2 but not in group1" << endl;
    }
  }
}  /* CompareTwoGroups */



void  CompareTwoSetsOfGroups (vector< vector <KKStr > >&  groups1,
                              vector< vector <KKStr > >&  groups2,
                              std::ostream&                r
                             )
{
  vector< vector <KKStr > >::const_iterator  idx;

  for  (idx = groups1.begin ();  idx != groups1.end ();  idx++)
  {
    vector <KKStr >  g1 = *idx;

    if  (g1.size () == 0)
    {
      continue;
    }

    KKStr  i = g1[0];
    vector <KKStr>  g2 = GetGroupForExample (groups2, i);

    CompareTwoGroups (g1, g2, r);
  }
 
}  /* CompareTwoSetsOfGroups */
