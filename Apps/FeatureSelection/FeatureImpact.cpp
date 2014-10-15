#include "FirstIncludes.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

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
#include "StatisticalFunctions.h"
#include "Str.h"
using namespace KKU;


#include "MLClass.h"
using namespace MLL;

#include "AccByFeatureSel.h"
#include "BinaryJob.h"
#include "BinaryJobList.h"
#include "FeatureImpact.h"




FeatureImpact::FeatureImpact (BinaryJobList&  jobs):
  fileDesc (jobs.FileDesc ()),
  log      (jobs.Log ())

{
  int  numOfFeatures = fileDesc->NumOfFields ();
  int  x;
  int  featureNum;

  for  (x = 0;  x < numOfFeatures;  x++)
  {
    accuracyImpacts.push_back (VectorFloat (0));
    fMeasureImpacts.push_back (VectorFloat (0));
  }

  uint  zed = 0;

  for  (zed = 0;  zed < jobs.size ();  zed++)
  {
    BinaryJob&  j = jobs[zed];

    if  (j.ParentId () < 0)
      continue;

    BinaryJobPtr  p = jobs.LookUpByJobId (j.ParentId ());
    if  (!p)
      continue;

    FeatureNumList  featuresRemoved (jobs.FileDesc ());
    FeatureNumList  featuresAdded   (jobs.FileDesc ());

    featuresAdded   = j.Features ()  - p->Features ();
    featuresRemoved = p->Features () - j.Features ();

    float  accuracyChange = j.Accuracy () - p->Accuracy ();
    float  fMeasureChange = j.FMeasure () - p->FMeasure ();

    for  (x = 0;  x < featuresRemoved.NumSelFeatures ();  x++)
    {
      featureNum = featuresRemoved[x];
      accuracyImpacts[featureNum].push_back (-accuracyChange);
      fMeasureImpacts[featureNum].push_back (-fMeasureChange);
    }

    for  (x = 0;  x < featuresAdded.NumSelFeatures ();  x++)
    {
      featureNum = featuresAdded[x];
      accuracyImpacts[featureNum].push_back (accuracyChange);
      fMeasureImpacts[featureNum].push_back (fMeasureChange);
    }
  }

  for  (featureNum = 0;  featureNum < numOfFeatures;  featureNum++)
  {
    float  mean;
    float  stdDev;
    
    CalcMeanAndStdDev (accuracyImpacts[featureNum], mean, stdDev);
    accuracyMeans.push_back (mean);
    accuracyStdDevs.push_back (stdDev);

    CalcMeanAndStdDev (fMeasureImpacts[featureNum], mean, stdDev);
    fMeasureMeans.push_back (mean);
    fMeasureStdDevs.push_back (stdDev);

    counts.push_back  ((uint)(accuracyImpacts[featureNum].size ()));
  }
}






FeatureImpact::~FeatureImpact ()
{
}




void   FeatureImpact::ReportHTML (ostream&  r)
{
  r << "<table align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"Feature Impact Table\" >"  << endl
    << "  <caption>Feature Impact Table</caption>"                                                                                     << endl
    << "  <colgroup align=\"center\" span=\"2\">"                                               << endl
    << "  <colgroup align=\"center\" span=\"5\">"                                               << endl
    << "  <thead valign=\"bottom\" style=\"font-weight:bold; font-size:larger; height:auto\">"  << endl
    << "     <tr>"                                                                              << endl
    << "       <th align=\"center\" colspan=\"2\">&nbsp</th>"                                   << endl
    << "       <th align=\"center\" colspan=\"4\">Impact</th>"                                  << endl
    << "       <th align=\"center\" colspan=\"1\">&nbsp</th>"                                   << endl
    << "     </tr>"                                                                             << endl
    << "     <tr>"                                                                              << endl
    << "       <th align=\"center\">Feature<br />Number</th>"    << endl
    << "       <th align=\"center\">Feature<br />Name</th>"      << endl
    << "       <th align=\"center\">Accuracy<br />Mean</th>"     << endl
    << "       <th align=\"center\">Accuracy<br />StdDev</th>"   << endl
    << "       <th align=\"center\">FMeasure<br />Mean</th>"     << endl
    << "       <th align=\"center\">FMeasure<br />StdDev</th>"   << endl
    << "       <th align=\"center\">Count</th>"                  << endl
    << "     </tr>"                                              << endl
    << "  </thead>"                                              << endl
    << endl;

  r << "<tbody style=\"font-weight:normal; font-weight:normal; font-size:large; height:auto; text-align:right\">" << endl;
  for   (uint featureNum = 0;  featureNum < accuracyMeans.size ();  featureNum++)
  {
    r << "<tr>" << endl
      << "<td align=\"center\">" << featureNum                           << "</td>"  << endl
      << "<td align=\"left\">"   << fileDesc->FieldName (featureNum)     << "</td>"  << endl
      << "<td>"                  << accuracyMeans   [featureNum] << "%"  << "</td>"  << endl
      << "<td>"                  << accuracyStdDevs [featureNum] << "%"  << "</td>"  << endl
      << "<td>"                  << fMeasureMeans   [featureNum] << "%"  << "</td>"  << endl
      << "<td>"                  << fMeasureStdDevs [featureNum] << "%"  << "</td>"  << endl
      << "<td>"                  << counts          [featureNum]         << "</td>"  << endl
      << "</tr>"
      << endl;
  }
  r << "</tbody>"  << endl;
  r << "</table>"  << endl;
  r << endl;
}  /* ReportHTML */




void   FeatureImpact::Report (ostream&  r)
{
  r << "Feature Impact Report" << endl
    << endl
    << "Feature"  << "\t"  << "Feature" << "\t" << "Mean"    << "\t" << "&"      << "\t" << ""       << endl
    << "Number"   << "\t"  << "Name"    << "\t" << "Impact"  << "\t" << "StdDev" << "\t" << "Count"  << endl;

  for   (uint featureNum = 0;  featureNum < accuracyMeans.size ();  featureNum++)
  {
    r << featureNum                       << "\t"  
      << fileDesc->FieldName (featureNum) << "\t"  
      << accuracyMeans   [featureNum]     << "\t"
      << accuracyStdDevs [featureNum]     << "\t"
      << fMeasureMeans   [featureNum]     << "\t"
      << fMeasureStdDevs [featureNum]     << "\t"
      << counts          [featureNum]     << "\t"
      << endl;
  }
  r << endl;
}   /* Report */



FeatureNumList   FeatureImpact::FeatureThatDontHurt ()
{
  FeatureNumList  features (fileDesc);
  for   (uint featureNum = 0;  featureNum < accuracyMeans.size ();  featureNum++)
  {
    if  (accuracyMeans[featureNum] > 0.0f)
      features.AddFeature (featureNum);
  }
  return  features;
}




void  FeatureImpact::UpdateAccByFeatureSelList (AccByFeatureSelListPtr  accByFeatureSelList,
                                                MLClassConstPtr      class1,
                                                MLClassConstPtr      class2
                                                )
{
  if  (!accByFeatureSelList)
  {
    log.Level (-1) << endl << endl 
      << "FeatureImpact::UpdateAccByFeatureSelList   ***ERROR***    accByFeatureSelList == NULL"  << endl
      << endl;
    return;
  }

  for   (uint featureNum = 0;  featureNum < accuracyMeans.size ();  featureNum++)
  {
    accByFeatureSelList->PushOnBack (new AccByFeatureSel (class1, class2, featureNum, accuracyMeans[featureNum], accuracyStdDevs[featureNum]));
  }
}  /* UpdateAccByFeatureSelList */

