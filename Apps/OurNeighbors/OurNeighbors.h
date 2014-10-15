#ifndef  _OURNEIGHBORS_
#define  _OURNEIGHBORS_

#include  "Application.h"
#include  "RunLog.h"
#include  "Str.h"
using namespace  KKU;

#include  "MLClass.h"
#include  "ImageFeatures.h"
using namespace  MLL;

#include  "Neighbor.h"
#include  "ClassSummary.h"


class  OurNeighbors: public Application
{
public:

	OurNeighbors (int32     argc, 
                char**  argv
               );

	~OurNeighbors();

	void	LookForNeighbors ();

	virtual const char* ApplicationName ()  {return  "OurNeighbors";}

	void	DisplayCommandLineParameters ();

	virtual bool ProcessCmdLineParameter (
                                        char    parmSwitchCode, 
                                        KKStr  parmSwitch, 
                                        KKStr  parmValue
                                       );

  void    RandomReport (ImageFeaturesList&  images);

private:
  LLoydsEntryListPtr   DeriveAllLLoydsBins (const ImageFeaturesList&  examples);


  LLoydsEntryPtr      DeriveLLoydsBins (const ImageFeaturesList&  examples,
                                        int32                       lloydsBinSize
                                       );

  MLClassConstPtr  DetermineClassFromFileName (const  KKStr&  fileName);

  int32               LastScanLine (const ImageFeaturesList&  images)  const;

  void                RemoveExcludedClasses (ImageFeaturesListPtr&  examples);

  void                ReportDuplicates ();



  int32                     bucketSize;
  bool                      cancelFlag;
  MLClassConstListPtr    excludedClasses;   // List of classes to exclude from report.
  KKStr                     featureFileName;
  KKStr                     fromPlanktonName;
  MLClassConstPtr        fromPlankton;
  ostream*                  html;
	ofstream*                 htmlFile;
	KKStr                     htmlFileName;
	MLClassConstPtr        mlClass;
	MLClassConstListPtr    mlClasses;
  int32                     lastScanLine;  // Last scan line in SIPPER file;  
  int32                     baseLLoydsBinSize;
  KKStr                     lloydsBinsFileName;
  NeighborType              neighborType;
  int32                     numOfIterations;
  int32                     numOfBuckets;
  bool                      randomizeLocations;
	ostream*                  report;
	ofstream*                 reportFile;
	KKStr                     reportFileName;
	KKStr                     sourceRootDirPath;
	KKStr                     startDirectory;
};

#endif
