#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Drawing;

#include  "Point.h"

#include  "PicesKKStr.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesPoint
  {
  public:
    PicesPoint (PicesPoint^  p);

    PicesPoint (int  _row,
                int  _col
               );

    PicesPoint (PointPtr  p);

    PicesPoint (Drawing::Point^  p);

    PicesPoint (const KKU::Point&  p);




    !PicesPoint ();
    ~PicesPoint ();

    property int   Row {int get ()  {return  row;}  void  set (int  _row)  {row = _row;}}
    property int   Col {int get ()  {return  col;}  void  set (int  _col)  {col = _col;}}

    bool  Equal (PicesPoint^  right);
    bool  NotEqual (PicesPoint^  right);

       
  private:
    int  row;
    int  col;

  };  /* PicesPoint */


  public  ref class  PicesPointList:  List<PicesPoint^>
  {
  public:
    PicesPointList ();

    /**
     *@breief  Construct a list of managed PicesPoint instances from the contents of the unmanaged list 'unmanagedImages'.
     *@details  Will take ownership of the instances of Raster in 'unmanagedImages'.  The list will be deleted and the unmanaged
     *          code should not try to access any of 'unmanagedImages' contents after this call.
     *@param[in,out]  unmanagedImages  List of unmanaged 'PicesPoint' objects; the ownership of these instances will be given 
     *                over to the new Managed instance of 'PicesPointList'; will be deleted and set to NULL.
     */
    PicesPointList (PointListPtr&  unmanagedPoints);

    PicesPointList (const PointList&  unmanagedPoints);

    PicesPointList (PicesPointList^  points);


    ~PicesPointList ();

    !PicesPointList ();

    void  AddList (PicesPointList^ list);

    /**
     *@brief Give ownership of the contents of 'list' to this Managed list.
     *@details  Will take ownership of the unmanaged instances of 'Raster' in 'list'.  These instances will be deleted
     *   by the ".net" garabage collector so make sure that no unmanaged code makes use of these instances after making 
     *   this call.
     *@param[in,out]  list  The list of 'Raster' objects to turn owneship of; will be deleted and be set to NULL;  do not make
     *                      use of any of these instances in the unmanaged code afetr making this call.
     */
    void  AddList (PointListPtr&  list);

    void  CleanUpMemory ();

    float  ComputeSegmentLens (float  heightFactor,
                               float  widthFactor
                              );

    KKU::PointListPtr  ToUnmanaged ();

    bool  Equal (PicesPointList^  right);
    bool  NotEqual (PicesPointList^  right);

  private:
  };  /* PicesPointList */



}
