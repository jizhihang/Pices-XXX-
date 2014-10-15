#include  "FirstIncludes.h"
#include  <stdlib.h>
#include  <stdio.h>
#include  <math.h>
#include  <memory>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
using namespace std;


#include  "BasicTypes.h"


using namespace KKU;



#include  "VolumeSampledStat.h"

using  namespace  SipperHardware;


VolumeSampledStat::VolumeSampledStat ():
    binId         (0),
    binDepth      (0.0f),
    volumeSampled (0.0f)
{
}


VolumeSampledStat::VolumeSampledStat (const VolumeSampledStat&  _stat):
    binId         (_stat.binId),
    binDepth      (_stat.binDepth),
    volumeSampled (_stat.volumeSampled)
{
}


VolumeSampledStat::VolumeSampledStat (int32   _binId,
                                      float   _binDepth,
                                      float   _volumeSampled
                                     ):
    binId         (_binId),
    binDepth      (_binDepth),
    volumeSampled (_volumeSampled)
{
}





VolumeSampledStat::~VolumeSampledStat ()
{
}



void  VolumeSampledStat::AddToVolumeSampled (float _volumeSampled)
{
  volumeSampled += volumeSampled;
}




VolumeSampledStatList::VolumeSampledStatList ():
  map<int32, VolumeSampledStatPtr> ()
{
}



VolumeSampledStatList::~VolumeSampledStatList ()
{
  iterator idx;
  for  (idx = begin ();  idx != end ();  ++idx)
  {
    delete  idx->second;
    idx->second = NULL;
  }
}


VolumeSampledStatPtr  VolumeSampledStatList::Locate (int binId)
{
  iterator  idx;
  idx = find (binId);
  if  (idx == end ())
    return NULL;
  else
    return idx->second;
}



void  VolumeSampledStatList::Add (const VolumeSampledStat&  stat)
{
  VolumeSampledStatPtr  existingStat = Locate (stat.BinId ());
  if  (existingStat)
  {
    existingStat->VolumeSampled (existingStat->VolumeSampled () + stat.VolumeSampled ());
  }
  else
  {
    insert (Pair (stat.BinId (), new VolumeSampledStat (stat)));
  }
}  /* Add */



void  VolumeSampledStatList::Give (VolumeSampledStatPtr&  stat)
{
  VolumeSampledStatPtr  existingStat = Locate (stat->BinId ());
  if  (existingStat)
  {
    existingStat->AddToVolumeSampled (stat->VolumeSampled ());
    delete  stat;
    stat = NULL;
  }
  else
  {
    insert (Pair (stat->BinId (), stat));
  }
}




VolumeSampledStatList&  VolumeSampledStatList::operator+= (const VolumeSampledStatList&  right)
{
  const_iterator  idx;
  for  (idx = right.begin ();  idx != right.end ();  ++idx)
  {
    Add (*(idx->second));
  }
  return *this;
}
