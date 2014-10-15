#ifndef  _FEATUREIMPACT_
#define  _FEATUREIMPACT_

#include  "KKQueue.h"
#include  "RunLog.h"
#include  "Str.h"


#if  !defined(_ACCBYFEATURESEL_)
class  AccByFeatureSel;
typedef  AccByFeatureSel*  AccByFeatureSelPtr;
class  AccByFeatureSelList;
typedef  AccByFeatureSelList*  AccByFeatureSelListPtr;
#endif

#include  "BinaryJobList.h"


class FeatureImpact
{
public:
  typedef  KKU::uint  uint;
  typedef  FeatureImpact*  FeatureImpactPtr;

  FeatureImpact (BinaryJobList&  jobs);

  ~FeatureImpact ();


  void   Report (ostream&  r);

  void   ReportHTML (ostream&  r);

  FeatureNumList  FeatureThatDontHurt ();

  void  UpdateAccByFeatureSelList (AccByFeatureSelListPtr  accByFeatureSelList,
                                   MLClassConstPtr      class1,
                                   MLClassConstPtr      class2
                                  );

private:
  FileDescPtr   fileDesc;
  RunLog&       log;
  VectorFloat   accuracyMeans;
  VectorFloat   accuracyStdDevs;

  VectorFloat   fMeasureMeans;
  VectorFloat   fMeasureStdDevs;
  vector<uint>  counts;

  vector<VectorFloat>  accuracyImpacts;
  vector<VectorFloat>  fMeasureImpacts;
};  /* FeatureImpact */


typedef  FeatureImpact::FeatureImpactPtr  FeatureImpactPtr;



#endif

