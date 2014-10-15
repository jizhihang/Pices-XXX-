#include "FirstIncludes.h"

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <math.h>


#include "MemoryDebug.h"

using namespace std;

#include "BasicTypes.h"
#include "OSservices.h"
#include "Str.h"
using namespace KKU;


#include  "Neighbor.h"
using namespace MLL;


KKStr  NeighborTypeToStr (NeighborType  n)
{
  if  (n == AnyPlanktonClass)
    return "Any";

  else if  (n == SamePlanktonClass)
    return  "Same";

  return "";
}  /* NeighborTypeToStr */




Neighbor::Neighbor (double              _row,
                    double              _col,
                    const KKStr&        _fileName,
                    MLClassConstPtr  _mlClass,
                    double              _size
                   ):

  row             (_row),
  col             (_col),
  fileName        (_fileName),
  mlClass      (_mlClass),
  nearestNeighbor (NULL),
  size            (_size),
  squareDist      (0.0),
  dist            (0.0),
  distCalculated  (false)

{
}



Neighbor::Neighbor (ImageFeaturesPtr  image):
   row             (image->SfCentroidRow ()),
   col             (image->SfCentroidCol ()),
   fileName        (image->ImageFileName ()),
   mlClass      (image->MLClass    ()),
   size            (image->OrigSize      ()),
   nearestNeighbor (NULL),
   squareDist      (0.0f),
   dist            (0.0f),
   distCalculated  (false)
{
}



KKStr  Neighbor::noClassName = "";






MLClassConstPtr   Neighbor::NearestNeighborClass     ()  const
{
  if  (nearestNeighbor)
    return  nearestNeighbor->MLClass ();
  else
    return  NULL;
}


const KKStr&   Neighbor::NearestNeighborClassName ()  const
{
  if  (nearestNeighbor)
    return  nearestNeighbor->ClassName ();
  else
    return  KKStr::EmptyStr ();
}


double  Neighbor::NearestNeighborSize ()  const
{
  if  (nearestNeighbor)
    return  nearestNeighbor->Size ();
  else
    return  0.0;
}


double   Neighbor::NearestNeighborRow ()  const
{
  if  (nearestNeighbor)
    return  nearestNeighbor->Row ();
  else
    return  0.0;
}


KKStr  Neighbor::NearestNeighborRowCol ()  const
{
  if  (nearestNeighbor)
    return  nearestNeighbor->RowCol ();
  else
    return  "";
}


KKStr  Neighbor::RowCol ()
{
  KKStr  rowCol = StrFormatDouble (row, "ZZZZZZZ0.0") + "," + StrFormatDouble (col, "ZZZZZZZ0.0");
  return  rowCol;
}  /* RowCol */


double  Neighbor::Dist ()
{
  if  (distCalculated)
    return  dist;

  dist = sqrt (squareDist);
  distCalculated = true;
  return  dist;
}  /* Dist */



void  Neighbor::SquareDist (double      _squareDist)       
{
  squareDist     = _squareDist;
  distCalculated = false;
}


const  
KKStr&  Neighbor::ClassName ()
{
  if  (!mlClass)
    return  noClassName;

  return  mlClass->UpperName ();
} /* ClassName */




void  Neighbor::CompareDist (NeighborPtr  n,
                             double        _squareDist
                            )
{
  if  (!nearestNeighbor)
  {
    squareDist = _squareDist;
    nearestNeighbor = n;
  } 
  else
  {
    if  (_squareDist < squareDist)
    {
      squareDist = _squareDist;
      nearestNeighbor = n;
    }
  }
}  /* CompareDist */




void  Neighbor::CompareDist (NeighborPtr  n)
{
  double  deltaRow = double (row - n->Row ());
  double  deltaCol = col - n->Col ();
  double  z = deltaRow * deltaRow + deltaCol * deltaCol;

  if  (!nearestNeighbor)
  {
    nearestNeighbor = n;
    squareDist      = z;
  }

  else
  {
    if  (z < squareDist)
    {
      squareDist = z;
      nearestNeighbor = n;
    }
  }

  n->CompareDist (this, z);
}  /* CompareDist */




NeighborList::NeighborList (bool     _owner, 
                            int32      _size,
                            RunLog&  _log
                           ):
  KKQueue<Neighbor>  (_owner, _size),
  log              (_log)
{
}





NeighborList::NeighborList (ImageFeaturesList&  images,
                            RunLog&             _log
                           ):
  KKQueue<Neighbor> (true, images.QueueSize ()),
  log             (_log)
{
  ImageFeaturesList::iterator  idx;

  for  (idx = images.begin ();  idx != images.end ();  idx++)
  {
    ImageFeaturesPtr  image = NULL;
    PushOnBack (new Neighbor (*idx));
  }
}



class  RowComparison: public  QueueComparison<Neighbor>
{
public:
   RowComparison ():  QueueComparison<Neighbor> () {}

   bool  operator ()  (NeighborPtr  p1,  
                       NeighborPtr  p2
                      )
   {
     if  (p1->Row () < p2->Row ())
       return true;

     if  (p1->Row () > p2->Row ())
       return false;

     return  p1->Col () < p2->Col ();
   }
};



class  ClassComparison: public  QueueComparison<Neighbor>
{
public:
   ClassComparison ():  QueueComparison<Neighbor> () {}

   bool  operator ()  (NeighborPtr  p1,  
                       NeighborPtr  p2
                      )
   {
     const KKStr&  p1ClassName = p1->ClassName ();
     const KKStr&  p2ClassName = p2->ClassName ();

     if  (p1ClassName < p2ClassName)
       return  true;

     if  (p1ClassName > p2ClassName)
       return  false;

     if  (p1->NearestNeighbor () == NULL)
     {
       if  (p2->NearestNeighbor () != NULL)
         return true;
       else
         return false;
     }

     else
     {
       if  (p2->NearestNeighbor () == NULL)
         return false;
     }

     {
       const KKStr&  p1ClassName = p1->NearestNeighborClassName ();
       const KKStr&  p2ClassName = p2->NearestNeighborClassName ();

       if  (p1ClassName < p2ClassName)
         return  true;

       else if  (p1ClassName > p2ClassName)
         return  false;

       if  (p1->Row () < p2->Row ())
         return true;

       else if  (p2->Row () < p1->Row ())
         return false;

       return  p1->Col () > p2->Col ();
     }
   }
};





class  ClassRowComparison: public  QueueComparison<Neighbor>
{
public:
   ClassRowComparison ():  QueueComparison<Neighbor> () {}

   bool  operator ()  (NeighborPtr  p1,  
                       NeighborPtr  p2
                      )
   {
     const KKStr&  p1ClassName = p1->ClassName ();
     const KKStr&  p2ClassName = p2->ClassName ();

     if  (p1ClassName < p2ClassName)
       return  true;

     if  (p1ClassName > p2ClassName)
       return  false;

     if  (p1->Row () < p2->Row ())
       return true;

     else if  (p2->Row () < p1->Row ())
       return false;

     return  p1->Col () > p2->Col ();
   }
};  /* ClassRowComparison */





void  NeighborList::SortByRow ()
{
  RowComparison c;
  sort (begin (), end (), c);
}  /* SortByRow */




void  NeighborList::SortByClass ()
{
  ClassComparison c;
  sort (begin (), end (), c);
}  /* SortByClass */




void  NeighborList::SortByClassRow ()
{
  ClassRowComparison c;
  sort (begin (), end (), c);
}  /* SortByClass */



void  NeighborList::FindNearestNeighbors (NeighborType        neighborType,
                                          MLClassConstPtr  restrictedClass
                                         )
{
  SortByRow ();

  int32  x;

  for  (x = 0;  x < QueueSize ();  x++)
  {
    // First lets look back to see if something closer
    
    int32 z = x + 1;

    NeighborPtr  curNeighbor = IdxToPtr (x);

    bool  atLeastOneNeighborFound = false;
    while  (z < QueueSize ())
    {
      NeighborPtr  nextNeighbor = IdxToPtr (z);

      if  ((neighborType == AnyPlanktonClass)  ||
           (curNeighbor->MLClass () == nextNeighbor->MLClass ())
          )
      {
        atLeastOneNeighborFound = true;
        curNeighbor->CompareDist (nextNeighbor);
      }

      double  deltaRow = double (curNeighbor->Row () - nextNeighbor->Row ());
      double  squareRowDist = deltaRow * deltaRow;

      if  (atLeastOneNeighborFound  &&  (squareRowDist > curNeighbor->SquareDist ()))
      {
        // No point searching any farther ahead.  We can not posibly find 
        // anything that will be closer.
        break;
      }

      z++;
    }
  }

}  /* FindNearestNeighbors */




double  NeighborList::LargestDist ()  const
{
  double  largestDist = 0.0f;

  const_iterator  idx;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    if  ((*idx)->Dist () > largestDist)
      largestDist = (*idx)->Dist ();
  }

  return  largestDist;
}  /* LargestDist */





void  NeighborList::ReportClassNeighbor (MLClassConstListPtr  mlClasses,
                                         ostream&                r
                                        )
{
  SortByClass ();

  mlClasses->SortByName ();

  int32     numOfClasses   = mlClasses->QueueSize ();
  int32*    classCounts    = new int32     [numOfClasses];
  int32**   neighborCounts = new int32*    [numOfClasses];
  double**  neighborDists  = new double* [numOfClasses];
  double**  neighborSizes  = new double* [numOfClasses];

  int32  x,y;
  for  (x = 0;  x < numOfClasses;  x++)
  {
    classCounts[x] = 0;
    neighborCounts[x] = new int32    [numOfClasses];
    neighborDists [x] = new double [numOfClasses];
    neighborSizes [x] = new double [numOfClasses];
    for  (y = 0;  y < numOfClasses;  y++)
    {
      neighborCounts[x][y] = 0;
      neighborDists [x][y] = 0.0;
      neighborSizes [x][y] = 0.0;
    }
  }


  NeighborList::iterator  next = begin ();
  if  (next == end ())
  {
    r << endl
      << "*** No Data To Report ***" << endl
      << endl;
    return;
  }

  NeighborPtr  n = *next;
  while  (n)
  {
    MLClassConstPtr  fromClass = n->MLClass ();
    int32               fromClassIdx = mlClasses->PtrToIdx (fromClass);

    while  (n  &&  (n->MLClass () == fromClass))
    {
      // Start doing stuff for from class Totals

      MLClassConstPtr  toClass    = n->NearestNeighborClass ();
      int32               toClassIdx = -1;
      if  (toClass)
        toClassIdx = mlClasses->PtrToIdx (toClass);

      //// Print Headers
      //r << "-----From-----" << "\t\t" << "-----To-----" << endl
      //  << fromClass->Name () << "\t" << "Size" << "\t" << toClass->Name () << "\t" << "Size" << "\t" << "Dist" << endl
      //  << "======"           << "\t" << "====" << "\t" << "======"         << "\t" << "====" << "\t" << "====" << endl;

      double  distTotal = 0.0f;
      int32     numInThisGroup = 0;
      double  distMin = FLT_MAX;
      double  distMax = FLT_MIN;

      NeighborList::iterator  groupStart = next;

      while  ((n)                                        &&  
              (n->MLClass           () == fromClass)  &&
              (n->NearestNeighborClass () == toClass)
             )
      {
        //NeighborPtr  nn = n->NearestNeighbor ();

        // n = current image,  nn = nearest neighbor of n

        //r << StrFormatDouble (n->Row  (), "zzzzzzzz0.0")  << "," << StrFormatDouble (n->Col  (), "zzzzzzzz0.0") << "\t" << n->Size  () << "\t"
        //  << StrFormatDouble (nn->Row (), "zzzzzzzz0.0")  << "," << StrFormatDouble (nn->Col (), "zzzzzzzz0.0") << "\t" << nn->Size () << "\t"
        //  << n->Dist ()
        //  << endl;

        distTotal += n->Dist ();
        distMin = Min (n->Dist (), distMin);
        distMax = Max (n->Dist (), distMax);
        
        classCounts [fromClassIdx]++;

        if  (toClassIdx >= 0)
        {
          neighborCounts[fromClassIdx][toClassIdx]++;
          neighborDists [fromClassIdx][toClassIdx] += n->Dist ();
          neighborSizes [fromClassIdx][toClassIdx] += n->Size ();
        }

        numInThisGroup++;

        next++;
        if  (next != end ())
          n = *next;
        else
          n = NULL;
      }

      double  distMean = distTotal / (double)numInThisGroup;

      {
        // calculate Statistics
        NeighborList::iterator  idx;
        double  totalSquared = 0.0f;

        for  (idx = groupStart;  idx != next;  idx++)
        {
          NeighborPtr n = *idx;
          double  delta = distMean - n->Dist ();
          double  deltaSquared = delta * delta;
          totalSquared += deltaSquared;
        }

        double  distVar = totalSquared / (double)numInThisGroup;
        double  distStdDev = sqrt (distVar);

        //r << "\t\t\t" << "--------------"                    << endl
        //  << "\t\t\t" << "Mean Dist"  << "\t"  << distMean   << endl
        //  << "\t\t\t" << "Min Dist"   << "\t"  << distMin    << endl
        //  << "\t\t\t" << "Max Dist"   << "\t"  << distMax    << endl
        //  << "\t\t\t" << "StdDev"     << "\t"  << distStdDev << endl
        //  << endl;
      }

      // Report From Class Totals;
    }
  }


  // Lets make Summary Reports
  int32  idx1, idx2;
  KKStr  title1, title2, title3;

  mlClasses->ExtractThreeTitleLines (title1, title2, title3);

  title1.TrimRight ("\n\r\t ");

  r << endl
    << endl
    << endl
    << "Counts by Nearest Neighbors" << endl;

  r << "\t"            << "\t" << title1 << endl;
  r << "\t"            << "\t" << title2 << endl;
  r << "\t" << "Total" << "\t" << title3 << endl;

  for  (idx1 = 0;  idx1 < numOfClasses;  idx1++)
  {
    MLClassConstPtr  class1 = mlClasses->IdxToPtr (idx1);

    r << class1->Name () << "\t" << classCounts[idx1];

    for  (idx2 = 0;  idx2 < numOfClasses;  idx2++)
    {
      r << "\t" << neighborCounts[idx1][idx2];
    }
    r << endl;
  }

  r << endl
    << endl
    << endl
    << "Percentage Neighbors" << endl;

  r << "\t"               << "\t" << title1 << endl;
  r << "\t"               << "\t" << title2 << endl;
  r << "\t" << "% of All" << "\t" << title3 << endl;

  for  (idx1 = 0;  idx1 < numOfClasses;  idx1++)
  {
    MLClassConstPtr  class1 = mlClasses->IdxToPtr (idx1);

    double  p = 100.0f *  (double)classCounts[idx1] / (double)QueueSize ();

    r << class1->Name () << "\t" << StrFormatDouble (p, "zzz0.00") << "%";

    for  (idx2 = 0;  idx2 < numOfClasses;  idx2++)
    {
      if  (classCounts[idx1] == 0)
        p = 0.0f;
      else 
        p = 100.0f * (double)neighborCounts[idx1][idx2] / (double)classCounts[idx1];

      r << "\t" << StrFormatDouble (p, "zzz0.00") << "%";
    }
    r << endl;
  }



  r << endl
    << endl
    << endl
    << "Mean Distances Between Neighbors" << endl;

  r << "\t" << title1 << endl;
  r << "\t" << title2 << endl;
  r << "\t" << title3 << endl;

  for  (idx1 = 0;  idx1 < numOfClasses;  idx1++)
  {
    MLClassConstPtr  class1 = mlClasses->IdxToPtr (idx1);
    r << class1->Name ();

    for  (idx2 = 0;  idx2 < numOfClasses;  idx2++)
    {
      double  meanDist = 0.0f;
      if  (neighborCounts[idx1][idx2] != 0)
        meanDist = neighborDists[idx1][idx2] / (double)neighborCounts[idx1][idx2];
      r << "\t" << meanDist;
    }
    r << endl;
  }


  r << endl
    << endl
    << endl
    << "Mean Sizes" << endl;

  if  (!title1.Empty ())
    r << "\t" << title1 << endl;
  r << "\t" << title2 << endl;
  r << "\t" << title3 << endl;

  for  (idx1 = 0;  idx1 < numOfClasses;  idx1++)
  {
    MLClassConstPtr  class1 = mlClasses->IdxToPtr (idx1);
    r << class1->Name ();

    for  (idx2 = 0;  idx2 < numOfClasses;  idx2++)
    {
      double  meanSize = 0.0f;
      if  (neighborCounts[idx1][idx2] != 0)
        meanSize = neighborSizes[idx1][idx2] / (double)neighborCounts[idx1][idx2];
      r << "\t" << meanSize;
    }
    r << endl;
  }


  for  (x = 0;  x < numOfClasses;  x++)
  {
    delete  neighborCounts[x];
    delete  neighborDists [x];
    delete  neighborSizes [x];
  }

  delete  classCounts;
  delete  neighborCounts;
  delete  neighborDists;
  delete  neighborSizes;
}  /* ReportClassNeighbor */






void  NeighborList::ReportClassRow (MLClassConstListPtr  mlClasses,
                                    ostream&                r
                                   )
{
  SortByClassRow ();

  mlClasses->SortByName ();


  NeighborList::iterator  next = begin ();
  if  (next == end ())
  {
    r << endl
      << "*** No Data To Report ***" << endl
      << endl;
    return;
  }


  int32     numOfClasses   = mlClasses->QueueSize ();
  int32*    classCounts    = new int32  [numOfClasses];
  double*  distsMean      = new double[numOfClasses];
  double*  distsStdDev    = new double[numOfClasses];
  double*  distsMin       = new double[numOfClasses];
  double*  distsMax       = new double[numOfClasses];
  double*  sizesMean      = new double[numOfClasses];
  double*  sizesStdDev    = new double[numOfClasses];
  double*  sizesMin       = new double[numOfClasses];
  double*  sizesMax       = new double[numOfClasses];

  int32  x;
  for  (x = 0;  x < numOfClasses;  x++)
  {
    classCounts[x] = 0;
    distsMean  [x] = 0.0f;
    distsStdDev[x] = 0.0f;
    distsMin   [x] = 0.0f;
    distsMax   [x] = 0.0f;
    sizesMean  [x] = 0.0f;
    sizesStdDev[x] = 0.0f;
    sizesMin   [x] = 0.0f;
    sizesMax   [x] = 0.0f;
  }


  NeighborPtr  n = *next;
  while  (n)
  {
    MLClassConstPtr  fromClass = n->MLClass ();
    int32            fromClassIdx = mlClasses->PtrToIdx (fromClass);

    int32    numInThisGroup = 0;

    double  distTotal = 0.0f;
    double  distMin = FLT_MAX;
    double  distMax = FLT_MIN;

    double  sizeTotal = 0.0f;
    double  sizeMin   = FLT_MAX;
    double  sizeMax   = FLT_MIN;

    NeighborList::iterator  groupStart = next;

    // Print Headers
    r << "-----From-----" << "\t\t" << "-----To-----" << endl
      << fromClass->Name () << "\t" << "Size" << "\t" << "NeighborClass" << "\t" << "Position" << "\t" << "Size" << "\t" << "Dist" << endl
      << "======"           << "\t" << "====" << "\t" << "=============" << "\t" << "========" << "\t" << "====" << "\t" << "====" << endl;

    while  ((n)  &&  (fromClass == n->MLClass ()))
    {
      //NeighborPtr  nn = n->NearestNeighbor ();

      KKStr   neighborClassName = n->NearestNeighborClassName ();
      KKStr   neighborRowCol    = n->NearestNeighborRowCol    ();
      double  neighborSize      = n->NearestNeighborSize      ();

      // n = current image,  nn = nearest neighbor of n

      r << n->RowCol () << "\t" 
        << n->Size  ()  << "\t";

      r << neighborClassName << "\t" 
        << neighborRowCol    << "\t"
        << neighborSize      << "\t";

      r << n->Dist ()
        << endl;

      distTotal += n->Dist ();
      distMin = Min (n->Dist (), distMin);
      distMax = Max (n->Dist (), distMax);

      sizeTotal += neighborSize;
      sizeMin = Min (sizeMin, neighborSize);
      sizeMax = Max (sizeMax, neighborSize);
        
      classCounts [fromClassIdx]++;
      numInThisGroup++;

      next++;
      if  (next != end ())
         n = *next;
      else
         n = NULL;
    }

    double  distMean = distTotal / (double)numInThisGroup;
    double  sizeMean = sizeTotal / (double)numInThisGroup;

    {
      // calculate Statistics
      NeighborList::iterator  idx;
      double  distTotalSquared = 0.0f;
      double  sizeTotalSquared = 0.0f;

      for  (idx = groupStart;  idx != next;  idx++)
      {
        NeighborPtr n = *idx;
        double  delta = distMean - n->Dist ();
        double  deltaSquared = delta * delta;
        distTotalSquared += deltaSquared;

        double  nearestNeighborSize = n->NearestNeighborSize ();

        delta = sizeMean - nearestNeighborSize;
        deltaSquared = delta * delta;
        sizeTotalSquared += deltaSquared;
      }

      double  distVar = distTotalSquared / (double)numInThisGroup;
      double  distStdDev = sqrt (distVar);

      double  sizeVar = sizeTotalSquared / (double)numInThisGroup;
      double  sizeStdDev = sqrt (sizeVar);

      r << "--------------------------------------------------------" << endl;

      r << "Distance" << "\t" << "Mean"   << "\t" << distMean   
        <<               "\t" << "StdDev" << "\t" << distStdDev
        <<               "\t" << "Min"    << "\t" << distMin
        <<               "\t" << "Max"    << "\t" << distMax
        << endl;

      r << "To-Size"  << "\t" << "Mean"   << "\t" << sizeMean   
        <<               "\t" << "StdDev" << "\t" << sizeStdDev
        <<               "\t" << "Min"    << "\t" << sizeMin
        <<               "\t" << "Max"    << "\t" << sizeMax
        << endl;

      r << endl;
                   
      distsMean   [fromClassIdx] = distMean;
      distsStdDev [fromClassIdx] = distStdDev;
      distsMin    [fromClassIdx] = distMin;
      distsMax    [fromClassIdx] = distMax;

      sizesMean   [fromClassIdx] = sizeMean;
      sizesStdDev [fromClassIdx] = sizeStdDev;
      sizesMin    [fromClassIdx] = sizeMin;
      sizesMax    [fromClassIdx] = sizeMax;
    }
  }


  r << endl
    << endl
    << endl;


  r << "\t\t" << "----Distances----" << "\t\t\t\t" << "----ToSizes----" << endl;

  r << "Class"  << "\t" 
    << "Count"  << "\t" 

    << "Mean"   << "\t" 
    << "StdDev" << "\t"
    << "Min"    << "\t" 
    << "Max"    << "\t"

    << "Mean"   << "\t" 
    << "StdDev" << "\t"
    << "Min"    << "\t" 
    << "Max"

    << endl;

  int32  idx1;
  for  (idx1 = 0;  idx1 < numOfClasses;  idx1++)
  {
    MLClassConstPtr  class1 = mlClasses->IdxToPtr (idx1);
    r << class1->Name ()    << "\t" 
      << classCounts[idx1]  << "\t"

      << distsMean  [idx1]  << "\t"
      << distsStdDev[idx1]  << "\t"
      << distsMin   [idx1]  << "\t"
      << distsMax   [idx1]  << "\t"

      << sizesMean  [idx1]  << "\t"
      << sizesStdDev[idx1]  << "\t"
      << sizesMin   [idx1]  << "\t"
      << sizesMax   [idx1]

      << endl;
  }


  delete  classCounts;
  delete  distsMean;
  delete  distsStdDev;
  delete  distsMin;
  delete  distsMax;
  delete  sizesMean;
  delete  sizesStdDev;
  delete  sizesMin;
  delete  sizesMax;
}  /* ReportClassRow */




void  NeighborList::ReportClassRowRestricted (MLClassConstListPtr  mlClasses,
                                              ostream&                r,
                                              MLClassConstPtr      restrictedClass
                                             )
{
  SortByClassRow ();

  mlClasses->SortByName ();

  NeighborList::iterator  next = begin ();
  if  (next == end ())
  {
    r << endl
      << "*** No Data To Report ***" << endl
      << endl;
    return;
  }

  int32     numOfClasses   = mlClasses->QueueSize ();
  int32    classCounts    = 0;
  double  distsMean      = 0.0;
  double  distsStdDev    = 0.0;
  double  distsMin       = 0.0;
  double  distsMax       = 0.0;
  double  sizesMean      = 0.0;
  double  sizesStdDev    = 0.0;
  double  sizesMin       = 0.0;
  double  sizesMax       = 0.0;

  // Lets skip to the restricted Class
  NeighborPtr  n = *next;
  do  {
    next++;
    if  (next != end ())
       n = *next;
    else
       n = NULL;
  }  
  while  (n->MLClass () != restrictedClass);


  if  (n == NULL)
  {
    // There was no data for the restricted class
    r << endl
      << endl
      << "No data for Class[" << restrictedClass->Name () << "]" << endl
      << endl;
    return;
  }


  while  ((n)  &&  (n->MLClass () == restrictedClass))
  {
    MLClassConstPtr  fromClass = n->MLClass ();

    int32    numInThisGroup = 0;

    double  distTotal = 0.0f;
    double  distMin = FLT_MAX;
    double  distMax = FLT_MIN;

    double  sizeTotal = 0.0f;
    double  sizeMin   = FLT_MAX;
    double  sizeMax   = FLT_MIN;

    NeighborList::iterator  groupStart = next;

    // Print Headers
    r << "-----From-----" << "\t\t" << "-----To-----" << endl
      << fromClass->Name () << "\t" << "Size" << "\t" << "NeighborClass" << "\t" << "Position" << "\t" << "Size" << "\t" << "Dist" << endl
      << "======"           << "\t" << "====" << "\t" << "=============" << "\t" << "========" << "\t" << "====" << "\t" << "====" << endl;

    while  ((n)  &&  (fromClass == n->MLClass ()))
    {
      //NeighborPtr  nn = n->NearestNeighbor ();
 
      // n = current image,  nn = nearest neighbor of n

      double  nearestNeighborSize = n->NearestNeighborSize ();

      r << n->RowCol () << "\t" 
        << n->Size  ()  << "\t";

      r << n->NearestNeighborClassName () << "\t" 
        << n->NearestNeighborRowCol    ()  << "\t"
        << nearestNeighborSize             << "\t";

      r << n->Dist ()
        << endl;

      distTotal += n->Dist ();
      distMin = Min (n->Dist (), distMin);
      distMax = Max (n->Dist (), distMax);

      sizeTotal += nearestNeighborSize;
      sizeMin = Min (sizeMin, nearestNeighborSize);
      sizeMax = Max (sizeMax, nearestNeighborSize);
        
      classCounts++;
      numInThisGroup++;

      next++;
      if  (next != end ())
         n = *next;
      else
         n = NULL;
    }

    double  distMean = distTotal / (double)numInThisGroup;
    double  sizeMean = sizeTotal / (double)numInThisGroup;

    {
      // calculate Statistics
      NeighborList::iterator  idx;
      double  distTotalSquared = 0.0f;
      double  sizeTotalSquared = 0.0f;

      for  (idx = groupStart;  idx != next;  idx++)
      {
        NeighborPtr n = *idx;
        double  delta = distMean - n->Dist ();
        double  deltaSquared = delta * delta;
        distTotalSquared += deltaSquared;

        delta = sizeMean - n->NearestNeighborSize ();
        deltaSquared = delta * delta;
        sizeTotalSquared += deltaSquared;
      }

      double  distVar = distTotalSquared / (double)numInThisGroup;
      double  distStdDev = sqrt (distVar);

      double  sizeVar = sizeTotalSquared / (double)numInThisGroup;
      double  sizeStdDev = sqrt (sizeVar);

      r << "--------------------------------------------------------" << endl;

      r << "Distance" << "\t" << "Mean"   << "\t" << distMean   
        <<               "\t" << "StdDev" << "\t" << distStdDev
        <<               "\t" << "Min"    << "\t" << distMin
        <<               "\t" << "Max"    << "\t" << distMax
        << endl;

      r << "To-Size"  << "\t" << "Mean"   << "\t" << sizeMean   
        <<               "\t" << "StdDev" << "\t" << sizeStdDev
        <<               "\t" << "Min"    << "\t" << sizeMin
        <<               "\t" << "Max"    << "\t" << sizeMax
        << endl;

      r << endl;
                   
      distsMean   = distMean;
      distsStdDev = distStdDev;
      distsMin    = distMin;
      distsMax    = distMax;

      sizesMean   = sizeMean;
      sizesStdDev = sizeStdDev;
      sizesMin    = sizeMin;
      sizesMax    = sizeMax;
    }
  }


  r << endl
    << endl
    << endl;


  r << "\t\t" << "----Distances----" << "\t\t\t\t" << "----ToSizes----" << endl;

  r << "Class"  << "\t" 
    << "Count"  << "\t" 

    << "Mean"   << "\t" 
    << "StdDev" << "\t"
    << "Min"    << "\t" 
    << "Max"    << "\t"

    << "Mean"   << "\t" 
    << "StdDev" << "\t"
    << "Min"    << "\t" 
    << "Max"

    << endl;

  r << restrictedClass->Name () << "\t" 
    << classCounts              << "\t"

    << distsMean                << "\t"
    << distsStdDev              << "\t"
    << distsMin                 << "\t"
    << distsMax                 << "\t"

    << sizesMean                << "\t"
    << sizesStdDev              << "\t"
    << sizesMin                 << "\t"
    << sizesMax

    << endl;
}  /* ReportClassRowRestricted */




VectorIntPtr  NeighborList::HistogramByDistance (int32  numOfBuckets,
                                                 int32  bucketSize
                                                )
{

  if  ((numOfBuckets < 1) &&  (bucketSize < 1))
  {
    log.Level (-1) << endl
                   << "NeighborList::HistogramByDistance    *** ERROR ***" << endl
                   << "                                     Invalid Parameters"  << endl
                   << endl
                   << "NumOfBuckets[" << numOfBuckets << "]" << endl
                   << "bucketSize  [" << bucketSize   << "]" << endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }


  VectorIntPtr  buckets = new vector<int32> (numOfBuckets, 0);

  iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    NeighborPtr  n = *idx;
    int32  bucketMum = (int32)((double)(n->Dist ()) / (double)bucketSize);
    if  (bucketMum >= numOfBuckets)
      bucketMum = numOfBuckets - 1;
    
    else if  (bucketMum < 0)
      bucketMum = 0;

    (*buckets)[bucketMum]++;
  }
  
  return  buckets;
}  /*   HistogramByDistance  */




void  NeighborList::CalcStatistics (double&  mean,
                                    double&  stdDev,
                                    double&  minDist,
                                    double&  maxDist
                                   ) const
{
  maxDist = 0.0f;
  mean    = 0.0f;
  minDist = FLT_MAX;
  stdDev  = 0.0f;

  if  (QueueSize () < 1)
    return;

  double  totalDist = 0.0;

  NeighborList::const_iterator  idx;
  for  (idx = begin ();  idx != end ();  idx++)
  {
    double  dist = (*idx)->Dist ();
    totalDist += dist;
    if  (dist < minDist)
      minDist = dist;

    if  (dist > maxDist)
      maxDist = dist;
  }

  mean = totalDist / double (QueueSize ());
  double  totalSquareDelta = 0.0f;

  for  (idx = begin ();  idx != end ();  idx++)
  {
    double  delta = mean - (*idx)->Dist ();
    totalSquareDelta += (delta * delta);
  }

  double  varience = totalSquareDelta / double (QueueSize ());
  stdDev = sqrt (varience);
}  /* CalcStatistics */


