#include  "FirstIncludes.h"

#include  <stdlib.h>
#include  <memory>
#include  <math.h>

#include  <map>
#include  <string>
#include  <iostream>
#include  <fstream>
#include  <vector>

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;

#include  "Compressor.h"
#include  "OSservices.h"
#include  "Str.h"
using namespace KKU;

#include  "InstrumentDataFileManager.h"

#include  "DataBase.h"
#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"
using namespace MLL;

#include  "JobManager.h"
using namespace  JobManagment;


#include  "RandomSplitJob.h"
#include  "RandomSplitJobManager.h"
#include  "RandomSplits.h"



// C:\Pices\DataFiles\FeatureSelection\Simple
// -C Simple.cfg  -DataFile  Simple.data  -NumSplits 10 -SplitPercentage 70%

// C:\Users\kkramer\Dissertation\BiasAdjustmentExperiment\NineClassPlankton
// -c Plankton9Classes.cfg  -Format C45 -DataFile NineClassPlankton.data -NumSplits 30  -SplitPercentage 66%



RandomSplits::RandomSplits (int      argc, 
                            char**   argv,
                            RunLog&  _log
                           ):
   Application    (argc, argv),

   configFileName (),
   dataFileName   (),
   format         (FeatureFileIOPices::Driver ()),
   numFolds       (10),
   numSplits      (30),
   restart        (false),
   splitFraction  (0.7f)

{
  ProcessCmdLineParameters (argc, argv);
  if  (Abort ())
  {
    DisplayCommandLineParameters ();
    return;
  }
}



RandomSplits::~RandomSplits ()
{
}



void  RandomSplits::DisplayCommandLineParameters ()
{
  cout << endl << endl << endl << endl
       << "RandomSplits   -c <ConfigFileName>  -DataFile <FileName>  -format <Data File Format>  -NumSplits <Defaults to 30>  -SplitPercentage <Defaults to 70%>"  << endl
       << endl
       << "   -Config          : Config File Name"                                << endl
       << "   -DataFile        : Name a file that contains all the feature data." << endl
       << "   -Format          : " << FeatureFileIO::FileFormatsReadOptionsStr () << endl
       << "   -NumFolds        : Number of folds used to make Bias Matrix"        << endl
       << "   -NumSplits       : Number of Random Splits to generate."            << endl
       << "   -SplitPercentage : Percentage to be used for Training."             << endl
       << endl;
}



bool  RandomSplits::ProcessCmdLineParameter (char    parmSwitchCode, 
                                             KKStr   parmSwitch, 
                                             KKStr   parmValue
                                            )
{
  KKStr  parmValueUpper (parmValue);
  parmValueUpper.Upper ();

  parmSwitch.Upper ();

  if  ((parmSwitch == "-CONFIGFILENAME")  ||  (parmSwitch == "-C")  ||  (parmSwitch == "-CONFIG"))
  {
    configFileName = parmValue;
  }


  else if  ((parmSwitch == "-DATAFILE")  ||  (parmSwitch == "-DF"))
  {
    dataFileName = parmValue;
  }

  else if  ((parmSwitch == "-F")  ||  (parmSwitch == "-FORMAT"))
  {
    format = FeatureFileIO::FileFormatFromStr (parmValue);
    if  (!format)
    {
      log.Level (-1) << endl << endl 
                     << "ProcessCmdLineParameter   ***ERROR***    No such format as[" << parmValue<< "]." << endl
                     << endl;
      Abort (true);
    }
  }

  else if  (parmSwitch.EqualIgnoreCase ("-NumFolds")   ||  
            parmSwitch.EqualIgnoreCase ("-NF")         ||  
            parmSwitch.EqualIgnoreCase ("-NumOfFolds") ||
            parmSwitch.EqualIgnoreCase ("-NOF")
           )
  {
    numFolds =  parmValue.ToInt ();
    if  ((numFolds != 0)  &&  (numFolds < 2))
    {
      log.Level (-1) << endl
                     << "ProcessCmdLineParameter   ***ERROR***    Invalid NumFolds[" << parmValue<< "]." << endl
                     << "                                         Must be '0' or greter than '1'." << endl
                     << endl;
      Abort (true);
    }
  }

  else if  ((parmSwitch == "-NUMSPLITS")  ||  (parmSwitch == "-NS")  ||  (parmSwitch == "-SPLITS"))
  {
    numSplits =  parmValue.ToInt ();
  }


  else if  ((parmSwitch == "-SPLITPERCENTAGE")  ||  (parmSwitch == "-SP"))
  {
    bool  percentage = false;
    if  (parmValue.LastChar () == '%')
    {
      percentage = true;
      parmValue.ChopLastChar ();
    }
    splitFraction = parmValue.ToFloat ();
    if  (percentage)
      splitFraction = splitFraction / 100.0f;
    else
    {
      if  (splitFraction >= 1.0f)
      {
        log.Level (-1) << endl << endl
                       << "ProcessCmdLineParameter    ***ERROR***     Invalid Split Percentage[" << splitFraction << "]"  << endl
                       << endl
                       << "     If you want to enter as percentage include a '%' sign otherwise it is assumed to be a fraction." << endl
                       << endl;
      }
    }
  }

  else if  (parmSwitch.EqualIgnoreCase ("Restart"))
  {
    restart = true;
  }


  else
  {
    log.Level (-1) << endl << endl
                   << "ProcessCmdLineParameter    ***ERROR***" << endl
                   << endl
                   << "             Invalid Parameter[" << parmSwitch << "]" << endl
                   << endl;
    Abort (true);
  }


	return  !Abort ();
}  /* ProcessCmdLineParameter */






void  RandomSplits::Run ()
{
  bool  successful = false;
  RandomSplitJobManagerPtr  manager = new RandomSplitJobManager 
                                     (configFileName, 
                                      format, 
                                      dataFileName, 
                                      numSplits, 
                                      splitFraction,
                                      numFolds,
                                      log
                                     );
 
  manager->InitilizeJobManager (successful);
  if  (successful)
    manager->Run ();

  delete  manager;
  manager = NULL;
}




int  main (int     argc, 
           char**  argv
          )
{
  RunLog  log;

  InstrumentDataFileManager::InitializePush ();
  // Need to register the different types of Jobs.
  Job::RegisterConstructor ("RandomSplitJob",  RandomSplitJob::CreateEmptyInstance);
  
  RandomSplits  application (argc, argv, log);
  if  (!application.Abort ())
  {
    application.Run ();
  }

  InstrumentDataFileManager::InitializePop ();

  return  0;
}
