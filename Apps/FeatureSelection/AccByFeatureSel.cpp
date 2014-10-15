#include "FirstIncludes.h"

#ifdef  WIN32
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>
#else
  #include <stdlib.h>
#endif

#include  <stdlib.h>
#include  <stdio.h>

#include  <algorithm>
#include  <fstream>
#include  <map>
#include  <iostream>
#include  <map>
#include  <vector>

#ifdef  WIN32
#include <io.h>
#include <windows.h>
#else
//#include  <sys/loadavg.h>
#include <unistd.h>
#endif

#include  "MemoryDebug.h"
using namespace std;


#include "BasicTypes.h"
#include "KKQueue.h"
#include "OSservices.h"
#include "RunLog.h"
#include "Str.h"
using namespace KKU;


#include  "MLClass.h"
using namespace  MLL;

#include  "AccByFeatureSel.h"



AccByFeatureSel::AccByFeatureSel (MLClassConstPtr  _class1,
                                  MLClassConstPtr  _class2,
                                  int                 _featureNum,
                                  float               _avgAccGain,
                                  float               _stdDevAccGain
                                 ):
  class1        (_class1),
  class2        (_class2),
  featureNum    (_featureNum),
  avgAccGain    (_avgAccGain),
  stdDevAccGain (_stdDevAccGain)
{
}


AccByFeatureSel::~AccByFeatureSel ()
{
}



const KKStr&   AccByFeatureSel::Class1Name () const
{
  if  (class1)
    return  class1->Name ();
  else
    return  KKStr::EmptyStr ();
}


const KKStr&   AccByFeatureSel::Class2Name () const
{
  if  (class2)
    return  class2->Name ();
  else
    return  KKStr::EmptyStr ();
}



AccByFeatureSelList::AccByFeatureSelList (bool  _owner):
  KKQueue<AccByFeatureSel> (_owner, 100)
{
}



bool  AccByFeatureSelList::FeatureNumComparator::operator() 
                 (AccByFeatureSelPtr  abfs1,
                  AccByFeatureSelPtr  abfs2
                )
{
  int  delta = abfs1->FeatureNum () - abfs2->FeatureNum ();
  if  (delta < 0)  return true;  else if  (delta > 0) return false;

  delta = abfs1->Class1Name ().CompareTo (abfs2->Class1Name ());
  if  (delta < 0)  return true;  else if  (delta > 0)  return false;

  return abfs1->Class2Name () < abfs2->Class2Name ();
}



void  AccByFeatureSelList::SortByFeatureNumThenByClassCombo ()
{
  FeatureNumComparator c;
  sort (begin (), end (),  c);
}

