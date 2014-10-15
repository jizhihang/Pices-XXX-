#pragma once

using namespace System;
using namespace System::Drawing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;


#include  "VolumeSampledStat.h"
using namespace  SipperHardware;

#include  "PicesKKStr.h"
#include  "PicesRunLog.h"


namespace PicesInterface 
{
  public  ref class PicesVolumeSampledStat
  {
  public:
    PicesVolumeSampledStat (PicesVolumeSampledStat^  stat);

    PicesVolumeSampledStat (const VolumeSampledStat&  stat);

    !PicesVolumeSampledStat ();
    ~PicesVolumeSampledStat ();

    property int    BinId         {int   get ()  {return  binId;}          void  set (int    _binId)          {binId         = _binId;}}
    property float  BinDepth      {float get ()  {return  binDepth;}       void  set (float  _binDepth)       {binDepth      = _binDepth;}}
    property float  VolumeSampled {float get ()  {return  volumeSampled;}  void  set (float  _volumeSampled)  {volumeSampled = _volumeSampled;}}
       
  private:
    int32   binId;
    float   binDepth;
    float   volumeSampled;
  };  /* PicesVolumeSampledStat */


  public  ref class  PicesVolumeSampledStatList:  List<PicesVolumeSampledStat^>
  {
  public:
    PicesVolumeSampledStatList ();

    /**
     *@breief  Construct a list of managed PicesVolumeSampledStat instances from the contents of the unmanaged list 'unmanagedImages'.
     *@details  Will NOT take ownership of the instances of 'VolumeSampledStat' in 'stats but will create managed copies.
     *@param[in]  stats  List of unmanaged 'VolumeSampledStat' objects; new Managed instances of 'VolumeSampledStat' will be created and added to list.
     */
    PicesVolumeSampledStatList (const VolumeSampledStatList&  stats);

    ~PicesVolumeSampledStatList ();

    !PicesVolumeSampledStatList ();

    /**
     *@brief Duplicates the contsts of 'list' and adds to this instance.
     */
    void  AddList (PicesVolumeSampledStatList^ list);

    void  AddList (const VolumeSampledStatList&  stats);

    void  CleanUpMemory ();

    int   LargestBinId ();

    /** Returns reference to instance that has 'binId' or 'nullptr' if no instance with that 'binId'. */
    PicesVolumeSampledStat^  Locate (int binId);


    /**
     *@brief  Adds the Volume values of 'right' into this instance.
     *@details Each instance in 'right' will be added to the instance of 'PicesVolumeSampledStat' that has the same
     * binId.  If no instance exists will then create a new instance with this BinId.
     */
    void   MergeIn (PicesVolumeSampledStatList^ right);

    array<float>^  ToArray ();


  private:
  };  /* PicesVolumeSampledStatList */



}  /* PicesInterface */

