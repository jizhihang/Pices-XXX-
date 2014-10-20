#include "StdAfx.h"
#include  "FirstIncludes.h"

#include <stdio.h>
#include <math.h>


#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <istream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;


using namespace System;
using namespace System::Collections;
using namespace System::Threading;
using namespace System::Windows::Forms;

#include "MemoryDebug.h"
#include "BasicTypes.h"
#include "Point.h"
using namespace KKU;


#include "PicesKKStr.h"
#include "PicesPoint.h"


using namespace  PicesInterface;

namespace  PicesInterface
{

  PicesPoint::PicesPoint  (PicesPoint^  p):
      row (0), col (0)
  {
    row = p->row;
    col = p->col;
  }


  PicesPoint::PicesPoint (int  _row,
                          int  _col
                         ):
      row (_row),
      col (_col)
  {
  }


  PicesPoint::PicesPoint (PointPtr  p):
     row (0), col (0)
  {
    if  (p)
    {
      row = p->Row ();
      col = p->Col ();
    }
  }


  PicesPoint::PicesPoint (Drawing::Point^  p):
    row (0), col (0)
  {
    row = p->Y;
    col = p->X;
  }



  PicesPoint::PicesPoint (const KKU::Point&  p):
    row (p.Row ()), col (p.Col ())
  {
  }



  PicesPoint::!PicesPoint ()
  {
  }



  PicesPoint::~PicesPoint ()
  {
    this->!PicesPoint ();
  }


  bool  PicesPoint::Equal (PicesPoint^  right)
  {
    return  ((right != nullptr)  &&  (row == right->row)  &&  (col == right->col));
  }


  bool  PicesPoint::NotEqual (PicesPoint^  right)
  {
    return  ((right == nullptr)  ||  (row != right->row)  ||  (col != right->col));
  }




  PicesPointList::PicesPointList ()
  {
  }


  PicesPointList::PicesPointList (const PointList&  unmanagedPoints)
  {
    PointList::const_iterator  idx;
    for  (idx = unmanagedPoints.begin ();  idx != unmanagedPoints.end ();  idx++)
    {
      PointPtr  p = *idx;
      PicesPoint^ pi = gcnew PicesPoint (p);
      Add (pi);
    }
  }



  PicesPointList::PicesPointList (PointListPtr&  unmanagedImages)
  {
    if  (unmanagedImages == NULL)
      return;

    PointList::iterator  idx;
    for  (idx = unmanagedImages->begin ();  idx != unmanagedImages->end ();  idx++)
    {
      PointPtr  p = *idx;
      PicesPoint^ pi = gcnew PicesPoint (p);
      Add (pi);
    }
    delete  unmanagedImages;
    unmanagedImages = NULL;
  }



  PicesPointList::PicesPointList (PicesPointList^  points)
  {
    for each (PicesPoint^ p in points)
    {
      Add (p);
    }
  }




  PicesPointList::~PicesPointList ()
  {
    CleanUpMemory ();
  }



  PicesPointList::!PicesPointList ()
  {
    CleanUpMemory ();
  }


  void  PicesPointList::CleanUpMemory ()
  {
  }


  void  PicesPointList::AddList (PicesPointList^ list)
  {
    for each  (PicesPoint^ p in list)
    {
      Add (p);
    }
  }



  void  PicesPointList::AddList (PointListPtr&  list)
  {
    if  (!list)  return;
    PointList::iterator  idx;
    for  (idx = list->begin ();  idx != list->end ();  ++idx)
    {
      PointPtr  p = *idx;
      Add (gcnew PicesPoint (p));
    }

    list->Owner (false);
    delete  list;
    list = NULL;
  }



  KKU::PointListPtr  PicesPointList::ToUnmanaged ()
  {
    KKU::PointListPtr  unmanagedList = new KKU::PointList (true);
    for each  (PicesPoint^ p in *this)
    {
      unmanagedList->PushOnBack (new KKU::Point (p->Row, p->Col));
    }
    return  unmanagedList;
  }


  float  PicesPointList::ComputeSegmentLens (float  heightFactor,
                                             float  widthFactor
                                            )
  {
    if  (Count < 1)
      return 0.0f;

    float  totalLen = 0.0f;
    PicesPoint^  lastPoint = this[0];

    int x = 1;
    while  (x < Count)
    {
      PicesPoint^  nextPoint = this[x];
      float deltaRow = (float)(lastPoint->Row - nextPoint->Row) * heightFactor;
      float deltaCol = (float)(lastPoint->Col - nextPoint->Col) * widthFactor;
      float segLen = (float)Math::Sqrt ((float)(deltaRow * deltaRow) + (float)(deltaCol * deltaCol));
      totalLen += segLen;
      lastPoint = nextPoint;
      ++x;
    }
    return  totalLen;
  }


  bool  PicesPointList::Equal (PicesPointList^  right)
  {
    if  ((right == nullptr)  ||  (right->Count != Count))
      return false;

    for  (int idx = 0;  idx < Count;  ++idx)
    {
      if  (this[idx]->NotEqual (right[idx]))
        return false;
    }

    return  true;
  }


  bool  PicesPointList::NotEqual (PicesPointList^  right)
  {
    if  ((right == nullptr)  ||  (right->Count != Count))
      return true;

    for  (int idx = 0;  idx < Count;  ++idx)
    {
      if  (this[idx]->NotEqual (right[idx]))
        return true;
    }
    return  false;
  }

}  /* PicesInterface */
