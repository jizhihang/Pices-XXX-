#ifndef  _RANDOMNND_
#define  _RANDOMNND_

#include "Application.h"
#include "RunLog.h"
#include "Str.h"
using namespace  KKU;

#include "MLClass.h"
#include "ImageFeatures.h"
using namespace  MLL;


#include "Neighbor.h"




class  RandomNND
{
public:

	RandomNND (int32                 _scanLines,
             ImageFeaturesList&  _images,
             int32                 _numIterations,
             int32                 _numOfBuckets,
             int32                 _bucketSize,
             ostream&            _report,
             RunLog&             _log
            );

	~RandomNND();

  void	GenerateReport ();


  double  NND_Mean       ()  const  {return  nnd_Mean;}       // Mean NND of all iterations combined.
  double  NND_StdDev     ()  const  {return  nnd_StdDev;}     // StdDev of NND of all iterations combined.
  double  RealDataU2Stat ()  const  {return  realDataU2Stat;}


private:

  void          DeleteData ();

  VectorIntPtr  GetHistogramForARandomDistribution (VectorDouble& allDists,
                                                    double&       mean,
                                                    double&       stdDev,
                                                    double&       min,
                                                    double&       max
                                                   );




  int32                 bucketSize;
  ImageFeaturesList&  images;
  RunLog&             log;
  double              nnd_Mean;    // mean 'nnd' of all iterations combined.
  double              nnd_StdDev;  // std deviation of 'nnd' of all iterations combined.
  int32                 numOfBuckets;
  int32                 numOfParticles;
  int32                 numIterations;
  NeighborListPtr     realData;
  double               realDataU2Stat;
	ostream&            report;
  int32                 scanLines;
  double               widthMax;
};

#endif
