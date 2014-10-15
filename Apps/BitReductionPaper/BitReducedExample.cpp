#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <iostream>
#include  <fstream>
#include  <vector>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace  std;
using namespace KKU;


#include "BitReducedExample.h"



BitReducedExample::BitReducedExample (FeatureVector&  example,
                                      FileDescPtr     fileDesc,
                                      int             bitReduction
                                     ):
    featureData   (NULL),
    nodeKey       (NULL),
    numOfFeatures (example.NumOfFeatures ()),
    origImages    (NULL)
{
  const FFLOAT*  origFeatureData = example.FeatureData ();
  featureData = new int[numOfFeatures];
  for  (int x = 0;  x < numOfFeatures;  x++)
  {
    int temp = int (1000 * origFeatureData[x]);
    if (temp > 0)
      featureData[x] = temp >> bitReduction;
    else
      featureData[x] = -1 * ((-1*temp) >> bitReduction);
  }

  origImages = new FeatureVectorList (fileDesc, false, fileDesc->Log (), 0);
  origImages->PushOnBack (&example);


  nodeKey = new BitReducedExampleKey (*this);
}



BitReducedExample::~BitReducedExample ()
{
  delete[]  featureData;
  delete    nodeKey;
  delete    origImages;
}










BitReducedExampleComparator  bitReducedExampleComparator;

BitReducedExampleTree::BitReducedExampleTree (FeatureVectorList&  examples,
                                              int                 _bitReduction,
                                              FileDescPtr         _fileDesc,
                                              RunLog&             _log
                                             ):
  RBTree<BitReducedExample, BitReducedExampleComparator, BitReducedExampleKey> (bitReducedExampleComparator, true),
  bitReduction         (_bitReduction),
  fileDesc             (_fileDesc),
  log                  (_log),
  numDuplicateExamples (0)

{
  mlClasses = examples.ExtractListOfClasses ();

  FeatureVectorList::iterator  idx;

  for  (idx = examples.begin ();  idx != examples.end ();  idx++)
  {
    const FeatureVectorPtr  example = *idx;
    Add (*example);
  }
}

BitReducedExampleTree:: ~BitReducedExampleTree ()
{
   delete  mlClasses;
}



void  BitReducedExampleTree::Add (FeatureVector&  image)
{
  BitReducedExamplePtr  newEntry = new BitReducedExample (image, fileDesc, bitReduction);
  BitReducedExamplePtr  existingEntry = GetEqual (newEntry);

  if  (existingEntry)
  {
    numDuplicateExamples++;
    (*existingEntry) += (*newEntry);
    delete  newEntry;
  }
  else
  {
    RBTree<BitReducedExample, BitReducedExampleComparator, BitReducedExampleKey>::RBInsert (newEntry);
  }
}  /* Add */



 
BitReducedExamplePtr  BitReducedExampleTree::GetEqual (const BitReducedExamplePtr  entry)
{
  BitReducedExamplePtr  newEntry = RBTree<BitReducedExample, BitReducedExampleComparator, BitReducedExampleKey>::GetEqual (*entry);

  return  newEntry;
}





void  BitReducedExampleTree::DeriveDuplicateCounts  (VectorInt&  crossClassCounts,
                                                     VectorInt&  crossClassCountImages
                                                    )
{
  log.Level (10) << "DeriveDuplicateCounts   Starting" << endl;

  crossClassCounts.clear ();
  crossClassCountImages.clear ();

  uint  numOfClasses = mlClasses->QueueSize ();
  while  (crossClassCounts.size () < numOfClasses)
    crossClassCounts.push_back (0);

  while  (crossClassCountImages.size () < numOfClasses)
    crossClassCountImages.push_back (0);

  BitReducedExamplePtr next = GetFirst ();

  while  (next)
  {
    MLClassListPtr  classes = next->OrigImages ()->ExtractListOfClasses ();
    int  numClasses = classes->QueueSize () - 1;
    crossClassCounts[numClasses]++;
    crossClassCountImages[numClasses] += next->OrigImages ()->QueueSize ();
    delete  classes;
    next = GetNext ();
  }

  log.Level (10) << "DeriveDuplicateCounts   Exiting" << endl;
}  /* DeriveDuplicateCounts */
