#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <math.h>
#include  <time.h>

#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>


#include  "MemoryDebug.h"


using namespace std;

#include  "BasicTypes.h"
#include  "OSservices.h"
#include  "StatisticalFunctions.h"
#include  "Str.h"
using namespace  KKU;

#include  "FeatureFileIO.h"
#include  "Neighbor.h"
using namespace  MLL;


#include  "RandomNND.h"


/******************************************************************************
 * command line parameters that have been used for testing purposes
 ******************************************************************************/



uint32  BigRandomNumber ()
{
  return  (uint32)LRand48 ();
}



RandomNND::RandomNND (int32                 _scanLines,
                      ImageFeaturesList&  _images,
                      int32                 _numIterations,
                      int32                 _numOfBuckets,
                      int32                 _bucketSize,
                      ostream&            _report,
                      RunLog&             _log
                     ):

  bucketSize     (_bucketSize),
  images         (_images),
  log            (_log),
  numIterations  (_numIterations),
  numOfBuckets   (_numOfBuckets),
  numOfParticles (0),
  realData       (NULL),
  realDataU2Stat (0.0f),
  report         (_report),
  scanLines      (_scanLines),
  widthMax       (4096.0f)

{
  if  (numIterations < 1)
  {
    log.Level (-1) << "RandomNND::RandomNND   Invalid numIterations[" << numIterations << "]" << endl;
    osWaitForEnter ();
    exit (-1);
  }

  if  ((numOfBuckets < 1)  &&  (bucketSize < 1))
    numOfBuckets = 100;

  numOfParticles = images.QueueSize ();
}




/******************************************************************************
 * Destructor
 ******************************************************************************/
RandomNND::~RandomNND ()
{
  DeleteData ();
}





void  RandomNND::DeleteData ()
{
  delete  realData;
}  /* DeleteData */




VectorIntPtr  RandomNND::GetHistogramForARandomDistribution (VectorDouble&  allDists,
                                                             double&        mean,
                                                             double&        stdDev,
                                                             double&        min,
                                                             double&        max
                                                            )
{
  NeighborListPtr  randomDistribution = new NeighborList (true, numOfParticles, log);

  ImageFeaturesList::iterator  idx;

  for  (idx = images.begin (); idx != images.end ();  idx++)
  {
    ImageFeaturesPtr  image = *idx;

    double radius = (double)sqrt ((image->OrigSize ()) / 3.1415926);
    double width  = widthMax - (2 * radius);
    double colLeft = radius;
    double col = LRand48 () % (int32)(width + 0.5f) + colLeft;
    double row = (double)(LRand48 () % scanLines);

    randomDistribution->PushOnBack (new Neighbor (row, col, "", NULL, image->OrigSize ()));
  }

  randomDistribution->FindNearestNeighbors (AnyPlanktonClass, NULL);

  VectorIntPtr  distributionHistogram = randomDistribution->HistogramByDistance (numOfBuckets, bucketSize);

  randomDistribution->CalcStatistics (mean, stdDev, min, max);

  {
    // Add distances of this iteration to the 'allDist'  
    NeighborList::const_iterator idx;
    for  (idx = randomDistribution->begin ();  idx != randomDistribution->end ();  idx++)
      allDists.push_back ((*idx)->Dist ());
  }

  delete  randomDistribution;  randomDistribution = NULL;

  return  distributionHistogram;
}  /* GetHistogramForARandomDistribution */





void	RandomNND::GenerateReport ()
{
  log.Level (10) << "RandomNND::GenerateReport" << endl;

  DeleteData ();

  int32 bucket;
  int32 iteration;
  // int32 particle;

  double distAvgTotal = 0.0f;

  int32  realDataDistTotal = 0;

  VectorDouble  allDists;

  VectorDouble     u2Stats (numIterations, 0.0f);
  VectorIntPtr*   distHistograms  = new VectorIntPtr [numIterations];
  VectorInt       fracOfEventsTot (numIterations, 0);

  VectorDouble  means   (numIterations, 0);
  VectorDouble  stdDevs (numIterations, 0);
  VectorDouble  mins    (numIterations, 0);
  VectorDouble  maxs    (numIterations, 0);


  realData = new NeighborList (true, numOfParticles, log);
  ImageFeaturesList::iterator  idx;
  for  (idx = images.begin (); idx != images.end ();  idx++)
  {
    ImageFeaturesPtr  image = *idx;
    realData->PushOnBack (new Neighbor (image->SfCentroidRow (), image->SfCentroidCol (), "", NULL, image->OrigSize ()));
  }
  realData->FindNearestNeighbors (AnyPlanktonClass, NULL);
  double largestDist = realData->LargestDist ();

  if  ((numOfBuckets < 1)  &&  (bucketSize < 1))
     numOfBuckets = 100;

  if  (numOfBuckets < 1)
    numOfBuckets = (int32)(largestDist / (double)bucketSize);

  else if  (bucketSize < 1)
    bucketSize = (int32)(largestDist / (double)numOfBuckets);

  VectorIntPtr   realDataHistogram = realData->HistogramByDistance (numOfBuckets, bucketSize);


  for  (iteration = 0;  iteration < numIterations;  iteration++)
  {
    log.Level (10) << "GenerateReport    Iteration[" << iteration << "]" << endl;

    distHistograms[iteration] = GetHistogramForARandomDistribution (allDists,
                                                                    means   [iteration],
                                                                    stdDevs [iteration],
                                                                    mins    [iteration],
                                                                    maxs    [iteration]
                                                                   );
  }


  double  avgTotal = 0.0f;

  report << "Run Date/Time     [" << osGetLocalDateTime ()  << "]" << endl;
  report << "Scan Lines        [" << scanLines              << "]" << endl;
  report << "Particles         [" << numOfParticles         << "]" << endl;
  report << "NumOfBuckets      [" << numOfBuckets           << "]" << endl;
  report << "BucketSize        [" << bucketSize             << "]" << endl;
  report << "Iterations        [" << numIterations          << "]" << endl;

  report << endl << endl;

  report <<         "Bucket"
         << "\t" << "Min" 
         << "\t" << "Max" 
         << "\t" << "Avg" 
         << "\t" << "StdDev"
         << "\t" << "RealData"
         << "\t" << "Min*"
         << "\t" << "Max*"
         << "\t" << "Avg*"
         << "\t" << "RealData*";

  report << endl;

  for  (bucket = 0;  bucket < numOfBuckets;  bucket++)
  {
    int32  distMin   = INT_MAX;
    int32  distMax   = INT_MIN;
    int32  distTotal = 0;

    double  distMinFP = FLT_MAX;
    double  distMaxFP = -9999.99f;

    int32  bucketValStart = bucket * bucketSize;
    int32  bucketValEnd   = bucketValStart + bucketSize - 1;

    if  (bucket == (numOfBuckets - 1))
      report << ">=" << bucketValStart;
    else
      report << bucketValStart << "-" << bucketValEnd;

    for  (iteration = 0;  iteration < numIterations;  iteration++)
    {
      int32  dist = (*(distHistograms[iteration]))[bucket];

      distMin   =  Min (distMin, dist);
      distMax   =  Max (distMax, dist);
      distTotal += dist;

      fracOfEventsTot[iteration] += dist;

      double  distFP = (double)(fracOfEventsTot[iteration]) / (double)numOfParticles;

      distMinFP = Min (distMinFP, distFP);
      distMaxFP = Max (distMaxFP, distFP);
    }

    int32 realDataDist = (*realDataHistogram)[bucket];

    double distAvg = (double)distTotal / (double)numIterations;
    distAvgTotal += distAvg;
    double  distAvgFP = distAvgTotal / (double)numOfParticles;

    if  (distAvg > 0.0f)
    {
      for  (iteration = 0;  iteration < numIterations;  iteration++)
      {
        int32  dist = (*(distHistograms[iteration]))[bucket];

        double  delta = (double)dist - distAvg;
        double  u2Val = delta * delta / distAvg;
        u2Stats[iteration] += u2Val;
      }

      {
        double  delta = (double)realDataDist - distAvg;
        double  u2Val = delta * delta / distAvg;
        realDataU2Stat += u2Val;
      }
    }

    // Calc StdDev for this Bucket.
    double  deltaSquareTotal = 0.0f;
    for  (iteration = 0;  iteration < numIterations;  iteration++)
    {
      int32  dist = (*(distHistograms[iteration]))[bucket];
      double  delta = (double)dist - distAvg;
      deltaSquareTotal += delta * delta;
    }

    double  variance = deltaSquareTotal / numIterations;
    double  distStdDev = sqrt (variance);

    realDataDistTotal += realDataDist;
    double  realDataDistFP = (double)realDataDistTotal / (double)numOfParticles;

    report << "\t" << distMin 
           << "\t" << distMax 
           << "\t" << distAvg 
           << "\t" << distStdDev
           << "\t" << realDataDist
           << "\t" << distMinFP
           << "\t" << distMaxFP
           << "\t" << distAvgFP
           << "\t" << realDataDistFP;

    report << endl;
  }


  // report u2 Statistics now
  report << endl << endl << endl << endl;
  report << "U2 Statistics" << endl;

  report << endl
         << "Real_Data_U2_stat" << "\t" << realDataU2Stat << endl
         << endl;

  int32  numTimesRealGreaterThanRandom = 0;
  for  (iteration = 0;  iteration < numIterations;  iteration++)
  {
    report << iteration << "\t" << u2Stats[iteration];
    if  (realDataU2Stat > u2Stats[iteration])
    {
      report << "  *****";
      numTimesRealGreaterThanRandom++;
    }
    report << endl;
  }

  double  percentGreater = 100.0f * (double)numTimesRealGreaterThanRandom / (double)numIterations;
  report << endl
         << "U2 > " << numTimesRealGreaterThanRandom << " of " << numIterations << " simulations" << endl
         << "     " << StrFormatDouble (percentGreater, "ZZ0.00") << "%" << endl;


  {
    // Calculate Mean and StdDev of all iterations combined of dist. to nearest neighbor

    report << endl << endl
           << "Iteration" << "\t" << "Mean"  << "\t" << "StdDev" << "\t" << "Min" << "\t" << "Max" << endl;

    int32  origPrecision = report.precision ();

    for  (iteration = 0;  iteration < numIterations;  iteration++)
    {
      
      report << iteration;


      report << "\t" << StrFormatDouble (means   [iteration], "####0.00000")
             << "\t" << StrFormatDouble (stdDevs [iteration], "####0.00000")
             << "\t" << StrFormatDouble (mins    [iteration], "####0.00000")
             << "\t" << StrFormatDouble (maxs    [iteration], "####0.00000")
             << endl;
    }



    double  allMean, allStdDev;
    CalcMeanAndStdDev (allDists, allMean, allStdDev);
    report << "All" << "\t" << StrFormatDouble (allMean, "####0.00000") << "\t" << StrFormatDouble (allStdDev, "####0.00000") << endl
           << endl 
           << endl;


    report.precision (origPrecision);

    nnd_Mean   = allMean;
    nnd_StdDev = allStdDev;
  }


  for  (iteration = 0;  iteration < numIterations;  iteration++)
  {
    delete  distHistograms[iteration];
    distHistograms[iteration] = NULL;
  }

  delete[]  distHistograms;
  delete  realDataHistogram;
}  /* GenerateReport */


