#include "FirstIncludes.h"

#include <stdlib.h>
#include <memory>
#include <math.h>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "MemoryDebug.h"
#include "BasicTypes.h"

using namespace std;

#include "OSservices.h"
#include "Str.h"
#include "StatisticalFunctions.h"
using namespace KKU;


// From PICL
#include "FeatureFileIO.h"
#include "FeatureFileIOPices.h"
#include "MLClass.h"
#include "ImageFeatures.h"
#include "LLoydsEntry.h"
using namespace  MLL;


#include "ClassSummary.h"
#include "OurNeighbors.h"
#include "Neighbor.h"
#include "RandomNND.h"



// -s C:\TrainingApp\ClassifiedImages\hrs020701 -r NeighborReport.txt


/******************************************************************************
 * command line parameters that have been used for testing purposes
 ******************************************************************************/

// -s "C:\TrainingApp\ClassifiedImages" -r "report.txt"


// 2005-06-03
// -src C:\TrainingApp\ClassifiedImages\HRS030208b  -Report TestTeport.txt  -random


// 2005-08-31
//  -src C:\TrainingApp\ClassifiedImages\hrs030204  -Report TestTeport.txt  -random -B 50 -bs 20 -I 20 -FP Cnidaria


// C:\TrainingApp\ClassifiedImages\hrs020404_23
//  -src C:\TrainingApp\ClassifiedImages\hrs020404_23  -Report TestTeport.txt  -random -B 50 -bs 20 -I 20


// C:\TrainingApp\ClassifiedImages\hrs020501_man
// -src C:\TrainingApp\ClassifiedImages\hrs020501_man  -report TestReport.txt 
// -src C:\TrainingApp\ClassifiedImages\hrs020403  -report TestReport.txt

// -src D:\TrainingApp\ExtractedImages\hrs020505_man  -report  TestReport.txt  -random  -b 60 -I 50  -LLoydsBinSize 20000  -nnt same  -ExcludeClass Artifact_lines
// -CmdFile  cmdLine

// C:\PICES\EXE>ourneighbors -s D:\TrainingApp\ExtractedImages\HRS0207_07_man -nnt same -r D:\TrainingApp\ExtractedImages\HRS0207_07_man\neighbor.txt -lloydsbinsize 31297 -bs 313 -bc 500 -random -i 500

OurNeighbors::OurNeighbors (int32 argc, char**  argv) :
  Application              (argc, argv),
  baseLLoydsBinSize        (20000),
  bucketSize               (0),
  cancelFlag               (false),
  excludedClasses          (NULL),
  sourceRootDirPath        (),
  mlClass               (MLClass::CreateNewMLClass ("UnKnown")),
  mlClasses             (new MLClassConstList ()),
  report                   (NULL),
  fromPlanktonName         (),
  fromPlankton             (NULL),
  lastScanLine             (0),
  lloydsBinsFileName       (),
  neighborType             (AnyPlanktonClass),
  numOfBuckets             (0),
  numOfIterations          (20),
  randomizeLocations       (false)

{
	ProcessCmdLineParameters (argc, argv);

	if  (Abort ())
	{
		DisplayCommandLineParameters ();
		return;
	}

  if  (!excludedClasses)
    excludedClasses = new MLClassConstList ();

	if  (sourceRootDirPath.Empty ())
		sourceRootDirPath = osGetCurrentDirectory ();


  if  (featureFileName.Empty ())
	{
		KKStr  path, root, extension;
		osParseFileName (sourceRootDirPath, path, root, extension);
		featureFileName = path;
		osAddLastSlash (featureFileName);
		featureFileName << root + DS + root + ".data";
	}

	if  (reportFileName.Empty ())
	{
		reportFile = NULL;
		report = &cout;
	}
	else
	{
		reportFile = new ofstream (reportFileName.Str ());
		report = reportFile;
	}

	htmlFile = NULL;
	if  (!htmlFileName.Empty ())
	{
		htmlFile = new ofstream (htmlFileName.Str());
		html = htmlFile;
	}

  lloydsBinsFileName = osRemoveExtension (reportFileName) + "_LLoydsBins.data";

	*report << endl;
	*report << "------------------------------------------------------------------------"  << endl;
  *report << "Run Date                [" << osGetLocalDateTime ()               << "]."  << endl;
  *report << "Build Date              [" << __DATE__ << "  " << __TIME__        << "]."  << endl;
  *report << "Report File Name        [" << reportFileName                      << "]."  << endl;
	*report << "HTML File Name          [" << htmlFileName                        << "]."  << endl;
	*report << "Source Root Directory   [" << sourceRootDirPath                   << "]."  << endl;
	*report << "Feature File            [" << featureFileName                     << "]."  << endl;
  *report << "Neighbor Type           [" << NeighborTypeToStr (neighborType)    << "]."  << endl;
  *report << "From Plankton           [" << fromPlanktonName                    << "]."  << endl;
  *report << "RandomizeLocations      [" << (randomizeLocations ? "Yes" : "No") << "]."  << endl;
  if  (randomizeLocations)
  {
    *report << "Iterations              [" << numOfIterations                     << "]."  << endl;
    *report << "BucketSize              [" << bucketSize                          << "]."  << endl;
    *report << "BucketCount             [" << numOfBuckets                        << "]."  << endl;
  }
  *report << "BaseLLoydsBinSize       [" << baseLLoydsBinSize                     << "]."  << endl;
  *report << "ExcludedClasses         [" << excludedClasses->ToCommaDelimitedStr ()  << "]."  << endl;
	*report << "------------------------------------------------------------------------"  << endl;
	*report << endl;
}




/******************************************************************************
 * Destructor
 ******************************************************************************/
OurNeighbors::~OurNeighbors ()
{
	if  (reportFile)
	{
		reportFile->close ();
		delete  reportFile;
	}

	if  (htmlFile)
	{
		htmlFile->close ();
		delete  htmlFile;
	}

  delete  excludedClasses;  excludedClasses = NULL;

	//delete  mlClass;
}



/******************************************************************************
 * ProcessCmdLineParamters
 * DESC: Extracts parameters from the command line
 ******************************************************************************/
bool  OurNeighbors::ProcessCmdLineParameter (
                                             char    parmSwitchCode, 
                                             KKStr  parmSwitch, 
                                             KKStr  parmValue
                                            )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();

  parmSwitch.Upper ();

  if  ((parmSwitch == "-BUCKETCOUNT")  ||  (parmSwitch == "-BC"))
  {
    numOfBuckets = atoi (parmValue.Str ());
    if  (numOfBuckets < 2)
    {
      log.Level (-1) << "ProcessCmdLineParameter   *** Invalid Buckets[" << numOfBuckets << "] parameter" << endl;
      Abort (true);
    }
  }

  else if  ((parmSwitch == "-BUCKETSIZE")  ||  (parmSwitch == "-BS"))
  {
    bucketSize = atoi (parmValue.Str ());
    if  (bucketSize < 1)
    {
      log.Level (-1) << "ProcessCmdLineParameter   *** Invalid BucketSize[" << bucketSize << "] parameter" << endl;
      Abort (true);
    }
  }

  else if  ((parmSwitch == "-EXCLUDECLASS")  ||  (parmSwitch == "-EC"))
  {
    if  (parmValue.Empty ())
    {
      log.Level (-1) << endl << endl
                     << "ProcessCmdLineParameter   '-ExcludeClass'  missing class name to exclude." << endl
                     << endl;
      Abort (true);
    }

    else 
    {
      if  (!excludedClasses)
        excludedClasses = new MLClassConstList ();

      if  (excludedClasses->LookUpByName (parmValue) != NULL)
      {
        log.Level (-1) << "ProcessCmdLineParameter  '-ExcludeClass[" << parmValue << "]'  Specified more than once." << endl;
      }
      else
      {
        excludedClasses->PushOnBack (MLClass::CreateNewMLClass (parmValue));
      }
    }
  }


  else if  ((parmSwitch == "-FROMPLANKTON")  ||  (parmSwitch == "-FP"))
  {
    fromPlanktonName = parmValue;
  }

  else if  ((parmSwitch == "-H")  ||  (parmSwitch == "-HTML"))
		htmlFileName = parmValue;

  else if  ((parmSwitch == "-ITERATIONS")  ||  (parmSwitch == "-I"))
  {
    numOfIterations = atoi (parmValue.Str ());
    if  (numOfIterations < 1)
    {
      log.Level (-1) << "ProcessCmdLineParameter   *** Invalid Iterations[" << numOfIterations << "] parameter" << endl;
      Abort (true);
    }
  }


  else if  ((parmSwitch == "-LLOYDSBINSIZE")      ||  
            (parmSwitch == "-LBS")                ||  
            (parmSwitch == "-BASELLOYDSBINSIZE")  ||  
            (parmSwitch == "-BLBS")
           )
  {
    baseLLoydsBinSize = atoi (parmValue.Str ());
    if  (baseLLoydsBinSize < 10)
    {
      log.Level (-1) << endl << endl 
                     << "ProcessCmdLineParameter   *** Invalid BaseLLoydsBinSize[" << baseLLoydsBinSize << "] parameter  ***" << endl
                     << endl;
      Abort (true);
    }
  }


  else if  ((parmSwitch == "-NEARESTNEIGHBORTYPE")  ||  (parmSwitch == "-NNT"))
  {
    if  ((parmValue == '0')  ||  (parmValueUpper == "ANY"))
      neighborType = AnyPlanktonClass;

    else if  ((parmValue == "1")  ||  (parmValueUpper == "SAME"))
      neighborType = SamePlanktonClass;
  }


  else if  ((parmSwitch == "-R")  ||  (parmSwitch == "-REPORT"))
		reportFileName = parmValue;


  else if  ((parmSwitch == "-RAND")  ||  (parmSwitch == "-RANDOM"))
		randomizeLocations = true;


  else if  ((parmSwitch == "-S")  ||  (parmSwitch == "-SRC"))
		sourceRootDirPath = parmValue;


	return  !Abort ();
}  /* ProcessCmdLineParameter */




/******************************************************************************
 * DisplayCommandLineParameters()
 * DESC: Displays a help message to the user
 ******************************************************************************/
void   OurNeighbors::DisplayCommandLineParameters ()
{
	log.Level (0) << "OurNeighbors  -report <xxx>  -src <xxxx>  -html <xxxx>"       << endl;
	log.Level (0)                                                                   << endl;
	log.Level (0) << "    -report   Report File,  Defaults to Command Line."        << endl;
	log.Level (0)                                                                   << endl;
	log.Level (0) << "    -src      Source Directory Tree to Search for images."    << endl;
	log.Level (0)                                                                   << endl;
	log.Level (0) << "    -NearestNeighborType <0 | 1>."                            << endl;
	log.Level (0) << "                      0 - Nearest Image (default)"            << endl;
	log.Level (0) << "                      1 - Nearest Image of Same Class"        << endl;
	log.Level (0)                                                                   << endl;
	log.Level (0) << "    -FromPlankton(FP) <Plankton Class>"                       << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -Random           Randomize the Row and Col Info."        << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -Buckets(B)       Number Buckets if Random Histogram."    << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -BucketSize(BS)   Histogram Bucket Size."                 << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -Iterations(I)    Number of Random NND's to perform."     << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -ExcludeClass(EC) Name of class to exclude.  You may "    << endl;
  log.Level (0) << "                      specify this parameter more than once." << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -CmdFile(CF)      Name of file that contains additional"  << endl;
  log.Level (0) << "                      command line parameters."               << endl;
  log.Level (0)                                                                   << endl;
	log.Level (0) << "    -LLoydsBinSize(LBS)  Numbetr of scan lines per bin"       << endl;
}  /* DisplayCommandLineParameters */





/****************************************************************************************
 *                           DetermineClassFromFileName                                 
 *                                                                                      
 * Given an Image File Name(fileName)  will return and mlClass.  Will use the      
 * path name in fileName to determine class.  If the class does not exist in            
 * mlClasses, will create a new instance and add it to mlClasses.                  
 *
 * After talking to the user it has come to light that the first sub-directory in
 * a path will dictate what class he/she feels a image belongs to.
 *   Ex:
 *    .../Protista/
 *    .../Protista/SubDir1
 *    .../Trichodesmium
 *    .../Trichodesmium/SubDir1
 *    .../Trichodesmium/SubDir2
 *    
 *  The idea is that when trying to determine what class a image really is we look
 *  at the first sub-dir name in the path.  We may also have to deel with seq-num's 
 *  as part of the name,  in that case we strip the _ and following numbers from the 
 *  name to get the correct class name.
 ****************************************************************************************/
MLClassConstPtr  OurNeighbors::DetermineClassFromFileName (const  KKStr&  fileName)
{
	int32  x;
	KKStr filename_copy = fileName;

	// If there are no path seperator characters('\'  or  '/')  charaters in name
	// then we will not beable to determine the class.
	
  x = osLocateFirstSlashChar (filename_copy);
	if  (x <= 0)
    return  mlClasses->GetUnKnownClass ();


  KKStr  className = filename_copy.SubStrPart (0, x - 1);
  
  // now lets get rid of any posible trailing seq number.
  // We are assuming that a underscore{"_") character seperates the calss name from the seq number.
  // So if there is an underscore character,  and all the characters to the right of it are
  // underscore charcters,  then we will remove teh underscore and the following numbers.
	x = className.LocateLastOccurrence ('_');
  if  (x > 0)
  {
    // Now lets eliminate any seqence number in name
    // We are assuming that a underscore{"_") character seperates the class name from the seq number.
    // So if there is an underscore character,  and all the characters to the right of it are
    // numeric charcters,  then we will remove teh underscore and the following numbers.

    int32  y = x + 1;

    bool  allFollowingCharsAreNumeric = true;
    while  ((y < className.Len ()) &&  (allFollowingCharsAreNumeric))
    {
      char  ch = className[y];
      allFollowingCharsAreNumeric = ((ch >= '0')  &&  (ch <= '9'));
      y++;
    }

    if  (allFollowingCharsAreNumeric)
    {
      className = className.SubStrPart (0, x - 1);
    }
  }

  // Now that we have a string with the class name,  lets get a pointer 
  // to a mlClass object from mlClasses ,  if none there then we get 
  // to create a new class.
	MLClassConstPtr  mlClass = mlClasses->GetMLClassPtr (className);

	return  mlClass;
}  /* DetermineClassFromFileName */





int32  OurNeighbors::LastScanLine (const ImageFeaturesList&  images)  const
{
  int32  lastScanLine = 0;
  ImageFeaturesList::const_iterator  idx;
  for  (idx = images.begin ();  idx != images.end ();  idx++)
  {
    const ImageFeaturesPtr image = *idx;
    if  (image->SfCentroidRow () > lastScanLine)
      lastScanLine = (int32)(image->SfCentroidRow () + 0.5f);
  }

  return  lastScanLine;
}  /* LastScanLine */





double  Z_Score (double  sampleMeanNND, 
                 double  randomMeanNND,
                 double  randomStdDevNND,
                 int32     N
                )

{
  double  z_Score = double ((double (sampleMeanNND) - double (randomMeanNND))   /   (double (randomStdDevNND) / sqrt (double (N))));
  return  z_Score;
}






void	OurNeighbors::RandomReport (ImageFeaturesList&  images)
{
   double   allClassesMeanNNDAnyClass    = 0.0f;
   double   allClassesMeanStdDevAnyClass = 0.0f;

  ClassSummaryList  classSummaries (log);

  MLClassConstList::iterator  classIdx;

  VectorKKStr  zScoreSummaryLines;

  for  (classIdx = mlClasses->begin ();  classIdx != mlClasses->end ();  classIdx++)
  {
    MLClassConstPtr  mlClass = *classIdx;

    if  (fromPlankton  &&  (fromPlankton != mlClass))
      continue;

    double       randomMeanNND   = 0.0f;
    double       randomStdDevNND = 0.0f;
    double       realDataU2Stat  = 0.0f;
    double       sampleMeanNND   = 0.0f;
    double       sampleStdDevNND = 0.0f;
    double       sampleMaxDist   = 0.0f;
    double       sampleMinDist   = 0.0f;


    ImageFeaturesListPtr  imagesInClass = images.ExtractImagesForAGivenClass (mlClass);
    if  (imagesInClass->QueueSize () > 0)
    {
      // We are going to make a list of images that has duplicate instances of 'ImageFeatures' objects 
      // This way when we Randomize the locations of each 'sfCentroidRow' and 'sfCentroidCol' we do not 
      // imapct on the original data.
      ImageFeaturesListPtr  imagesToRandomize = new ImageFeaturesList (*imagesInClass,
                                                                       true  // 'true means we want to own the data so new instances will be created.
                                                                      );

      LLoydsEntryListPtr  lloydsEntries =  DeriveAllLLoydsBins (*imagesToRandomize);
      {
        // We will now get mean and stdDev of nnd for this class
        NeighborList  neighbors (*imagesToRandomize, log);
        neighbors.FindNearestNeighbors (neighborType, mlClass);
        neighbors.CalcStatistics (sampleMeanNND, sampleStdDevNND, sampleMaxDist, sampleMinDist);
      }


      KKStr  randomReportFileName;

      if  (reportFileName.Empty ())
        randomReportFileName = "RandomDistanceReport";
      else
        randomReportFileName = osRemoveExtension  (reportFileName) + "_Random";

      randomReportFileName << "_" << mlClass->Name () << ".txt";

      ofstream  randomReport (randomReportFileName.Str ());

      randomReport << "Random Distribution Report" << endl 
                   << endl;

      randomReport << "Source Directory  [" << sourceRootDirPath   << "]" << endl;
      randomReport << "Class             [" << mlClass->Name () << "]" << endl;

      RandomNND  randomizeLocations (lastScanLine, 
                                     *imagesToRandomize, 
                                     numOfIterations, 
                                     numOfBuckets, 
                                     bucketSize, 
                                     randomReport, 
                                     log
                                    );  

      randomizeLocations.GenerateReport ();

      randomMeanNND   = randomizeLocations.NND_Mean ();
      randomStdDevNND = randomizeLocations.NND_StdDev ();
      realDataU2Stat  = randomizeLocations.RealDataU2Stat ();

      //double  sampleMeanNND   = 0.0f;
      //double  sampleStdDevNND = 0.0f;

      double  z_Score = Z_Score (sampleMeanNND, randomMeanNND, randomStdDevNND, imagesToRandomize->QueueSize ());

      randomReport << endl << endl << endl
                   << "Z-Score" << endl
                   << "=======" << endl
                   << endl    
                   << "SampleMeanNND   " << "\t" << sampleMeanNND    << endl
                   << "SampleStdDevNND " << "\t" << sampleStdDevNND  << endl
                   << "RandomMeanNND   " << "\t" << randomMeanNND    << endl
                   << "RandomStdDevNND " << "\t" << randomStdDevNND  << endl
                   << "------- Z-Score " << "\t" << z_Score          << endl
                   << endl;
                  

      KKStr  zScoreSummaryLine = mlClass->Name () + "\t" +
                                  StrFormatDouble (sampleMeanNND,   "###,##0.00")  + "\t" +
                                  StrFormatDouble (sampleStdDevNND, "###,##0.00")  + "\t" +
                                  StrFormatDouble (randomMeanNND,   "###,##0.00")  + "\t" +
                                  StrFormatDouble (randomStdDevNND, "###,##0.00")  + "\t" +
                                  StrFormatDouble (z_Score,         "###,##0.000");

      zScoreSummaryLines.push_back (zScoreSummaryLine);

      // The new instance on 'ClassSummary' that is aboiut to be created will take ownership
      // of lloydsBins.
      classSummaries.PushOnBack (new ClassSummary (mlClass, lloydsEntries, (float)realDataU2Stat, (float)z_Score));

      delete  imagesToRandomize;  imagesToRandomize = NULL;
    }

    delete  imagesInClass;  imagesInClass = NULL;
  }

  if  (!fromPlankton)
  {
    LLoydsEntryListPtr  allClassesLLoydsEntries = DeriveAllLLoydsBins (images);

    // Create a report for all classes
    KKStr  randomReportFileName;
    if  (reportFileName.Empty ())
      randomReportFileName = "RandomDistanceReport_All.txt";
    else
      randomReportFileName = osRemoveExtension  (reportFileName) + "_Random_All.txt";

    ofstream randomReport (randomReportFileName.Str ());

    randomReport << "Source Directory  [" << sourceRootDirPath  << "]" << endl;
    randomReport << "Class             [" << "All"              << "]" << endl;

    {
      // Find the mean and stddev of Nearest Neighbor regardless of class.
      NeighborList  allClassesNeighbors (images, log);
      allClassesNeighbors.FindNearestNeighbors (AnyPlanktonClass, fromPlankton);

      double  allClassesMinDistAnyClass    = 0.0f;
      double  allClassesMaxDistAnyClass    = 0.0f;

      allClassesNeighbors.CalcStatistics (allClassesMeanNNDAnyClass,
                                          allClassesMeanStdDevAnyClass, 
                                          allClassesMinDistAnyClass,
                                          allClassesMaxDistAnyClass
                                         );
    }


    RandomNND  randomizeLocations (lastScanLine, 
                                   images, 
                                   numOfIterations, 
                                   numOfBuckets, 
                                   bucketSize, 
                                   randomReport, 
                                   log
                                  );

    randomizeLocations.GenerateReport ();

    // All classes Z-Score
    double  allClassesRandomMeanNND   = randomizeLocations.NND_Mean   ();
    double  allClassesRandomStdDevNND = randomizeLocations.NND_StdDev ();
    double  allClassesRealDataU2Stat  = randomizeLocations.RealDataU2Stat ();
    double  z_Score = Z_Score (allClassesMeanNNDAnyClass, allClassesRandomMeanNND, allClassesRandomStdDevNND, images.QueueSize ());

    KKStr  zScoreSummaryLine = KKStr ("All-Classes") + "\t" +
                                StrFormatDouble (allClassesMeanNNDAnyClass,     "###,##0.00")  + "\t" +
                                StrFormatDouble (allClassesMeanStdDevAnyClass,  "###,##0.00")  + "\t" +
                                StrFormatDouble (allClassesRandomMeanNND,       "###,##0.00")  + "\t" +
                                StrFormatDouble (allClassesRandomStdDevNND,     "###,##0.00")  + "\t" +
                                StrFormatDouble (z_Score,                       "###,##0.00");

    zScoreSummaryLines.push_back (zScoreSummaryLine);


    randomReport << endl << endl << endl
                 << "Z-Score" << endl
                 << "=======" << endl
                 << endl    
                 << "SampleMeanNND   " << "\t" << allClassesMeanNNDAnyClass     << endl
                 << "SampleStdDevNND " << "\t" << allClassesMeanStdDevAnyClass  << endl
                 << "RandomMeanNND   " << "\t" << allClassesRandomMeanNND       << endl
                 << "RandomStdDevNND " << "\t" << allClassesRandomStdDevNND     << endl
                 << "------- Z-Score " << "\t" << z_Score                       << endl
                 << endl;

    classSummaries.PushOnBack (new ClassSummary (MLClass::CreateNewMLClass (KKStr ("AllClasses")), 
                                                 allClassesLLoydsEntries, 
                                                 (float)allClassesRealDataU2Stat, 
                                                 (float)z_Score
                                                )
                              );
  }

  {
    // Z-Score Summary Report
    KKU::uint32  x;

    *report << std::endl << std::endl
      << "Z-Score Summary By Class" << std::endl
            << std::endl
            << "ClassName"  << "\t"  << "SampleMean"  << "\t"  << "SampleStdDev" << "\t" << "RandomMean" << "\t" << "RandomStdDev" << "\t" << "Z-Score" << std::endl
            << "========="  << "\t"  << "=========="  << "\t"  << "============" << "\t" << "==========" << "\t" << "============" << "\t" << "=======" << std::endl;

    for  (x = 0;  x < zScoreSummaryLines.size ();  x++)
      *report << zScoreSummaryLines[x] << std::endl;
  }

  *report << endl << endl << endl;
  classSummaries.SummaryReport (*report);

  *report << endl << endl << endl;
  classSummaries.SpatialOverlapReport (*report);

  classSummaries.SaveLLoydsBinsData (lloydsBinsFileName, sourceRootDirPath, lastScanLine, baseLLoydsBinSize);
                                           
}  /* RandomReport */






/******************************************************************************
 * LookForNeighbors
 * DESC: Builds a new data file in the root directory that reflects the currect 
 * location of all the images. This new data file is then compared to old one
 * and a grading report that indicates the number of classes correctly predicted
 * by the classifier is generated
 ******************************************************************************/
void	OurNeighbors::LookForNeighbors ()
{
	ImageFeaturesListPtr  currentImageFeatures = NULL;
	KKStr                relativeDir;

	log.Level (10) << "OurNeighbors::LookForNeighbors" << endl;


	/*
	 * create an image feature list from the source directory that corresponds to the
	 * current locations of the actual image files. Where possible, the feature data 
	 * file will be used. However, if an image has been moved it's features will have
	 * to be recalculated (which is handled by the function call) and we'll have to
	 * look in the origImageFeatures list for the original predicted class. We must do 
	 * this since the the predicted class for an image file should NEVER change between
	 * classification runs.
	 */
  currentImageFeatures =  FeatureFileIOPices::LoadInSubDirectoryTree 
                                 (sourceRootDirPath,
                                  *mlClasses,
                                  false,           // useDirectoryNameForClassName,
                                  cancelFlag,
                                  false,           // rewiteRootFeatureFile
                                  log
                                 );

  currentImageFeatures->FixSipperFileScanLineAndColFields ();
  lastScanLine = LastScanLine (*currentImageFeatures);
  {
    // Make sure Class name matches subdirectory that Example was found in.
    ImageFeaturesList::iterator  idx;
    for  (idx = currentImageFeatures->begin ();  idx != currentImageFeatures->end ();  idx++)
    {
      ImageFeaturesPtr image = *idx;
      MLClassConstPtr  mlClass = DetermineClassFromFileName (image->ImageFileName ());
      if  (mlClass)
        image->MLClass (mlClass);
    }
  }


  if  (excludedClasses)  
  {
    if  (excludedClasses->QueueSize () > 0)
      RemoveExcludedClasses (currentImageFeatures);
  }


  //if  (randomizeLocations)
  //  RandomizeLocations (*currentImageFeatures);

  if  (!fromPlanktonName.Empty ())
  {
    fromPlankton = mlClasses->LookUpByName (fromPlanktonName);
    if  (!fromPlankton)
    {
      log.Level (-1) << endl
                     << endl
                     << endl
                     << "LookForNeighbors     ****** ERROR *******" << endl
                     << endl
                     << "No images that are of PlanktonName[" << fromPlanktonName << "]" << endl
                     << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }

  // We will now build Neighbor List
  NeighborList  neighbors (*currentImageFeatures, log);
  neighbors.FindNearestNeighbors (neighborType, fromPlankton);


  double  allClassesMeanNND    = 0.0f;
  double  allClassesMeanStdDev = 0.0f;
  double  allClassesMinDist    = 0.0f;
  double  allClassesMaxDist    = 0.0f;

  neighbors.CalcStatistics (allClassesMeanNND,
                            allClassesMeanStdDev, 
                            allClassesMinDist,
                            allClassesMaxDist
                           );



  if  (fromPlankton)
    neighbors.ReportClassRowRestricted (mlClasses, *report, fromPlankton);
  else
    neighbors.ReportClassRow (mlClasses, *report);

  neighbors.ReportClassNeighbor (mlClasses, *report);

  if  (randomizeLocations)
    RandomReport (*currentImageFeatures);

	log.Level (10) << "OurNeighbors::LookForNeighbors   Exiting"  << endl;
}  /* LookForNeighbors */




void	OurNeighbors::RemoveExcludedClasses (ImageFeaturesListPtr&  examples)
{
   bool  keepClass = true;
   MLClassConstPtr  oldClass = NULL;

   examples->SortByClass ();

   ImageFeaturesListPtr  examplesToKeep = new ImageFeaturesList (examples->FileDesc (), 
                                                                 true, // true = We will own images,
                                                                 log,
                                                                 examples->QueueSize ()
                                                                );
   examples->Owner (false);

   ImageFeaturesList::iterator  idx;

   for  (idx = examples->begin ();  idx != examples->end ();  idx++)
   {
     ImageFeaturesPtr  i = *idx;
     if  (oldClass != i->MLClass ())
     {
       oldClass = i->MLClass ();
       keepClass = (excludedClasses->LookUpByName (oldClass->Name ()) == NULL);
     }

     if  (keepClass)
     {
       examplesToKeep->PushOnBack (i);
     }
     else
     {
       delete  i;  
     }
   }

   delete  examples;  
   examples = examplesToKeep;
}  /* RemoveExcludedClasses */




LLoydsEntryListPtr   OurNeighbors::DeriveAllLLoydsBins (const ImageFeaturesList&  examples)
{
  int32  lloydsBinSize = baseLLoydsBinSize;
  int32  maxLLoydsBinSize = lastScanLine / 4;

  LLoydsEntryListPtr  lloydsEntries = new LLoydsEntryList (log);

  while  (lloydsBinSize <= maxLLoydsBinSize)
  {
    LLoydsEntryPtr   lloydsBins = DeriveLLoydsBins (examples, lloydsBinSize);
    lloydsEntries->PushOnBack (lloydsBins);

    lloydsBinSize += baseLLoydsBinSize;
  }

  return  lloydsEntries;
}  /* DeriveAllLLoydsBins */




LLoydsEntryPtr  OurNeighbors::DeriveLLoydsBins (const ImageFeaturesList&  examples,
                                                int32                       lloydsBinSize
                                               )
{
  double  lloydsIndex = 0.0;

  int32  numLLoydsBins = int32 (lastScanLine + lloydsBinSize - 1)  / int32 (lloydsBinSize);

  VectorIntPtr  lloydsBins = new VectorInt (numLLoydsBins, 0);

  ImageFeaturesList::const_iterator  idx;
  for  (idx = examples.begin ();  idx != examples.end ();  idx++)
  {
    const ImageFeaturesPtr i = *idx;
    int32 lloydsBin =  int32 (i->SfCentroidRow () / double (lloydsBinSize));
    if  (lloydsBin >= numLLoydsBins)
    {
      // This can not happen;  but if it does; then I must of screwed uo the programming.
      log.Level (-1) << endl << endl << endl
                     << "OurNeighbors::DeriveLLoydsBins       **** ERROR ****" << endl
                     << endl
                     << "              An invalid lloydsBin[" << lloydsBin << "] was derived." << endl
                     << "              must be in range of  [0 - " << (numLLoydsBins - 1) << "]" << endl
                     << endl;

      lloydsBin = numLLoydsBins - 1;
    }

    (*lloydsBins)[lloydsBin]++;
  }

  lloydsIndex = LLoydsIndexOfPatchiness (*lloydsBins);

  return  new LLoydsEntry (lloydsBinSize, lloydsBins, lloydsIndex);
}  /* DeriveLLoydsBins */




int32  main (int32     argc,
           char**  argv
          )
{
  OurNeighbors  nearestNeighborReport (argc, argv);
  nearestNeighborReport.LookForNeighbors ();
}
