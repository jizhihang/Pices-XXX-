#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>

#include  <iostream>
#include  <fstream>
#include  <vector>
#include  <iomanip>


#include  "MemoryDebug.h"
#include  "BasicTypes.h"

using namespace std;
using namespace KKU;


#include <sys/types.h>

//#include  "KKQueue.h"
//#include  "Str.h"

#include  "Application.h"


#include  "PlotNonStreamingPlacement.h"

#include  "ClassGroupHistory.h"
#include  "MLClass.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "SortOrderType.h"








PlotNonStreamingPlacement::PlotNonStreamingPlacement (int     argc,
                                                      char**  argv
                                                     ):
  Application  (argc, argv),
  mlClasses (log),
  sortOrder    (NULL_SortOrder),
  totals       (NULL)
{
  ProcessCmdLineParameters (argc, argv);
  

  if  (rootDir.Empty ())
  {
    rootDir = osGetCurrentDirectory ();
  }

  if  (sortOrder == NULL_SortOrder)
  {
    log.Level (-1)  << endl
                    << endl
                    << "PlotNonStreamingPlacement    No Sort Order not specified." << endl
                    << endl;
    Abort (true);
    return;
  }

  totals = ClassGroupTotals::ConsolidateOneDirectoryOneSortOrder (rootDir, mlClasses, sortOrder, log);
  if  (!totals)
  {
    Abort (true);
  }

}  /* PlotNonStreamingPlacement */



PlotNonStreamingPlacement::~PlotNonStreamingPlacement ()
{
  delete  totals;
}



bool  PlotNonStreamingPlacement::ProcessCmdLineParameter (char    parmSwitchCode, 
                                                          KKStr  parmSwitch, 
                                                          KKStr  parmValue
                                                         )
{
  parmSwitch.Upper ();

  if  ((parmSwitch == "-SORTORDER")  ||  (parmSwitch == "-SO"))
  {
    sortOrder = SortOrderTypeFromStr (parmValue);
  }

  return  !Abort ();
}  /* ProcessCmdLineParameter */



void  PlotNonStreamingPlacement::ReportNonStreamPlacesMean ()
{
  KKStr  fullReportFileName (rootDir);
  osAddLastSlash (fullReportFileName);
  fullReportFileName << "NonStreamPlaces_"
                     << SortOrderDescrShort (sortOrder)
                     << "_IIPC-"  << StrFormatInt (totals->IIPC () , "000")
                     << "_IPR-"   << StrFormatInt (totals->IPR (), "00")
                     << ".txt";

  ofstream  r (fullReportFileName.Str ());

  if  (!r.is_open ())
  {
    log.Level (-1) << endl
                   << endl
                   << "ReportNonStreamPlacesMean    *** ERROR ***      Opening File[" 
                   << fullReportFileName << "]." <<endl
                   << endl;
    osWaitForEnter ();
    exit (-1);
  }

  r << "Mean Non Stream Places" << endl
    << endl
    << "Directory        [" << rootDir << "]." << endl
    << "Sort Order       [" << SortOrderDescr (sortOrder)   << "]" << endl
    << "IIPC             [" << totals->IIPC ()              << "]" << endl
    << "IPR              [" << totals->IPR ()               << "]" << endl
    << "NumOfRetrainings [" << totals->NumOfRetrainings ()  << "]" << endl
    << "NumRandomPasses  [" << totals->NumOfRandomPasses () << "]" << endl
    << endl
    << endl;
 
  int  numRetrainingsToDo = Min (40, (int)totals->NumOfRetrainings ());

  int  retraining;

  vector<VectorFloat>  nonStreamPlaces;
  r << "Count";
  for  (retraining = 0;  retraining < numRetrainingsToDo;  retraining++)
  {
    r << "\t" << retraining;
    nonStreamPlaces.push_back (totals->MeanNonStreamPlaces (retraining));
  }
  r << endl;

  int  x;

  for  (x = 0;  x < totals->IPR ();  x++)
  {
    r << x;
    for  (retraining = 0;  retraining < numRetrainingsToDo;  retraining++)
    {
      r << "\t";
      if  (x < (int)nonStreamPlaces[retraining].size ())
      {
        r << nonStreamPlaces[retraining][x];
      }
    }
    r << endl;
  }

  int  randomPass = 0;
  for  (randomPass = 0;  randomPass < totals->NumOfRandomPasses ();  randomPass++)
  {
    r << endl
      << endl
      << endl
      << "Results for Random Pass[" << randomPass << "]." << endl
      << endl;

    for  (retraining = 0;  retraining < numRetrainingsToDo;  retraining++)
    {
      r << "\t" << retraining;
    }
    r << endl;

    for  (x = 0;  x < totals->IPR ();  x++)
    {
      r << x;
      for  (retraining = 0;  retraining < numRetrainingsToDo;  retraining++)
      {
        const TrainingImageList trainingImages = totals->TrainingImages (retraining, randomPass);
        r << "\t";
        if  (x < (int)trainingImages.size ())
          r << trainingImages[x].NonStreamPlace ();
      }
      r << endl;
    }
  }


  r.close ();
}  /* ReportNonStreamPlacesMean */





int  main (int  argc,  char**  argv)
{
  PlotNonStreamingPlacement  app (argc, argv);
  if  (app.Abort ())
    return -1;

  app.ReportNonStreamPlacesMean ();
}  /* main */