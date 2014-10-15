#ifndef  _SPATIALANALYSIS_
#define  _SPATIALANALYSIS_

#include  "Application.h"
#include  "DataBase.h"
#include  "RandomNumGenerator.h"
#include  "RunLog.h"
#include  "SipperFile.h"
#include  "Str.h"



#ifndef  _IMAGEFEATURES_
class    ImageFeatures;
typedef  ImageFeatures*  ImageFeaturesPtr;
class    ImageFeaturesList;
typedef  ImageFeaturesList*  ImageFeaturesListPtr;
#endif

#ifndef  _MLCLASS_
class  MLClass;
typedef  MLClass*  MLClassConstPtr;
class  MLClassConstList;
typedef  MLClassConstList*  MLClassConstListPtr;
#endif



class  SpatialAnalysis: public Application
{
public:
  typedef  KKU::uint  uint;

  typedef  enum  {NullStat, BQVStat, Davis92Stat, PQVStat, TTLQCStat}  StatType;

  SpatialAnalysis(int     argc, 
                  char**  argv
                 );

  ~SpatialAnalysis();


  void   RunSpatialAnalysis ();

  void   RunDavis92SpatialAnalysis ();

  StatType  TypeOfStat ()  {return  statType;}


private:


  KKStr  StatTypeToStr (StatType  statType);

  StatType  StatTypeFromStr (KKStr  s);

  VectorFloat  CalculatePatchinessIndexUsingDavis92
                       (const VectorDouble&   particles,        // Particle Locations
                        float                 windowSize,
                        uint                  binCount,
                        double                transactLength
                       );

  
  VectorFloat   CalculatePatchinessIndexUsingDavis92Integrated 
                       (const VectorDouble&   particles,          // Particle Locations
                        float                 windowSize,
                        uint                  binCount,
                        double                transactLength
                       );


  VectorFloat   CalculatePatchinessIndexUsingDavis92Integrated2 
                       (const VectorDouble&   particles,          // Particle Locations
                        float                 windowSize,
                        uint                  binCount,
                        double                transactLength
                       );



  void  CalcStatistic (ImageFeaturesList&   src,
                       VectorFloat&         densityByQuadrat,
                       VectorFloat&         stats,
                       long                 maxNumScanLines
                      );


  double   Davis92DesityForBinSizeH (const VectorDouble&  x,
                                     int                  startParticle,   // Particle Index in 'x'
                                     float                binSize
                                    );

  float   Davis92PatchinessIndexForBinSize (const VectorDouble&  x,
                                            float                binSize,
                                            double               transactLength
                                           );
  

  void   DisplayCommandLineParameters ();
  
  void   GetSipperFileNameAndInstrumentData ();

  void   OneSpatialAnalysisRun (ImageFeaturesList&   src,
                                long                 maxNumScanLines,
                                VectorFloat&         densityByQuadrat,
                                VectorFloat&         statistic,
                                VectorFloat&         mean,
                                VectorFloat&         stdDev
                               );

 void  OneDavis92SpatialAnalysisRun (ImageFeaturesList&     src,
                                     long                   maxNumScanLines,
                                     VectorFloat&           patchinessStats,
                                     VectorFloat&           patchinessStatMeans,
                                     VectorFloat&           patchinessStatStdDevs,
                                     VectorFloat&           patchinessStatMins,
                                     VectorFloat&           patchinessStatMaxs,
                                     VectorFloat&           patchinessStatLowerConfidences,
                                     VectorFloat&           patchinessStatUpperConfidences
                                    );

 
 bool   ProcessCmdLineParameter (char    parmSwitchCode, 
                                 KKStr   parmSwitch, 
                                 KKStr   parmValue
                                );

  void   RandomizeLocations (ImageFeaturesList&  examples,
                             long                maxNumScanLines
                            );


  void   PrintSpatialHistogramReport ();



  int                   bucketCount;    // Number of buckets per Window;  see WindowSize
  float                 bucketSize;     // Number of meters to a bucket
  bool                  cancelFlag;
  float                 confidenceInterval;
  uint                  lowerConfIntervalIdx;
  uint                  upperConfIntervalIdx;

  ImageFeaturesListPtr  data;
  DataBasePtr           dbConn;
  float                 defaultFlowRate;
  float                 defaultScanRate;
  InstrumentDataListPtr instrumentData;
  float                 quadratSize;
  int                   numOfIterations;
  ofstream*             r;
  RandomNumGenerator    randNums;
  KKStr                 reportFileName;
  SipperFilePtr         sipperFile;
  KKStr                 sipperFileName;
  KKStr                 sourceRootDirPath;
  StatType              statType;
  double                transactLength;
  float                 windowSize;

};



#endif
