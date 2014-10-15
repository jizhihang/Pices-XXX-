#ifndef  _ACCBYFEATURESEL_
#define  _ACCBYFEATURESEL_

#include  "KKQueue.h"
#include  "RunLog.h"
#include  "Str.h"

#include  "MLClass.h"


class  AccByFeatureSel
{
public:
  typedef  AccByFeatureSel*  AccByFeatureSelPtr;

  AccByFeatureSel (MLClassConstPtr  class1,
                   MLClassConstPtr  class2,
                   int                 featureNum,
                   float               avgAccGain,
                   float               stdDevAccGain
                  );

  ~AccByFeatureSel ();

  MLClassConstPtr  Class1        () const {return class1;}
  MLClassConstPtr  Class2        () const {return class2;}
  const KKStr&        Class1Name    () const;
  const KKStr&        Class2Name    () const;
  int                 FeatureNum    () const {return featureNum;}
  float               AvgAccGain    () const {return avgAccGain;}
  float               StdDevAccGain () const {return stdDevAccGain;}

private:
  MLClassConstPtr  class1;
  MLClassConstPtr  class2;
  int                 featureNum;
  float               avgAccGain;
  float               stdDevAccGain;
};  /* AccByFeatureSel */


typedef  AccByFeatureSel::AccByFeatureSelPtr  AccByFeatureSelPtr;


class  AccByFeatureSelList:  public KKQueue<AccByFeatureSel>
{
public:
  AccByFeatureSelList (bool  _owner);

  void  SortByFeatureNumThenByClassCombo ();

private:
  class  FeatureNumComparator
  {
  public:
    bool  operator() (AccByFeatureSelPtr  abfs1,
                      AccByFeatureSelPtr  abfs2
                     );
  };
};

typedef  AccByFeatureSelList*  AccByFeatureSelListPtr;



#endif
