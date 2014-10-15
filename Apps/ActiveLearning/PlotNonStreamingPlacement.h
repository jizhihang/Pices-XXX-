#ifndef  _PLOTNONSTREAMINGPLACEMENT_
#define  _PLOTNONSTREAMINGPLACEMENT_

#include  "ClassGroupTotals.h"
#include  "ImageClass.h"
#include  "SortOrderType.h"


class  PlotNonStreamingPlacement: public  Application
{
public:
  PlotNonStreamingPlacement (int     argc,
                             char**  argv
                            );


  ~PlotNonStreamingPlacement ();

  virtual const 
  char*          ApplicationName ()  {return  "PlotNonStreamingPoints";}


  void  ReportNonStreamPlacesMean ();

  virtual 
  bool         ProcessCmdLineParameter (char   parmSwitchCode, 
                                        KKStr  parmSwitch, 
                                        KKStr  parmValue
                                       );


private:

  ImageClassList       imageClasses;
  KKStr                rootDir;
  SortOrderType        sortOrder;
  ClassGroupTotalsPtr  totals;  
};


typedef  PlotNonStreamingPlacement*  PlotNonStreamingPlacementPtr;


#endif