using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

using  SipperFile;
using  SipperFileViewer;
using  PicesInterface;

//***********************************************************************************************************
//*                                                                                                         *
//* Developer: Kurt Kramer                                                                                  *
//* Date:      2010-01-09                                                                                   *
//*                                                                                                         *
//* Will creaste two plots of instrument data, one for he down-cast of SIPPER and the other for the up-cast.*
//*                                                                                                         *
//* Used by PicesCommander to display a Plot of Instrument Data by depth. Will load instrument data for     *
//* given criteria and then seperate into Up and Down casts.  It does this by sorting into Temporal order   *
//* and then locating the entry that is at the deapest depth.  Data that ocurs before this point will be    *
//* considered down-cast and data after up-cast.                                                            *
//*                                                                                                         *
//*                                                                                                         *
//***********************************************************************************************************


namespace PicesCommander
{
  public class  DepthPlotDisplay
  {
    private  PicesDataBase    dbConn         = null;
    private  String           cruiseName     = null;
    private  String           stationName    = null;
    private  String           deploymentNum  = null;

    private  SipperConfigRec  configRec = null;


    private  DepthPlot2       upCastPlot   = null;
    private  DepthPlot2       downCastPlot = null;

    private  PicesCommander   parent = null;


    public  DepthPlotDisplay (PicesCommander _parent,
                              PicesDataBase  _dbConn,
                              String         _cruiseName,
                              String         _stationName,
                              String         _deploymentNum
                             )
    {
      if  (String.IsNullOrEmpty (_cruiseName))
        cruiseName = "";
      else
        cruiseName = _cruiseName;

      if  (String.IsNullOrEmpty (_stationName))
        stationName = "";
      else
        stationName = _stationName;

      if  (String.IsNullOrEmpty (_deploymentNum))
        deploymentNum = "";
      else
        deploymentNum = _deploymentNum;

      parent    = _parent;
      dbConn    = _dbConn;
      configRec = new SipperConfigRec ();
    }



    public  void  Show ()
    {
      if  ((String.IsNullOrEmpty (cruiseName))  ||  (String.IsNullOrEmpty (stationName))  ||  (String.IsNullOrEmpty (deploymentNum)))
      {
        MessageBox.Show ("You need to set 'Cruise', 'Station', and 'Deployment'  to use this function.");
        return;
      }

      SummarizeInstrumentDataForADeployment ();
    }  /* Show */



    private  int  DatePicesInstrumentComparer (PicesInstrumentData  i1,
                                               PicesInstrumentData  i2
                                              )
    {
      if  (i1.CtdDate < i2.CtdDate)
        return -1;

      else if  (i1.CtdDate > i2.CtdDate)
        return  1;

      else if  (i1.Depth < i2.Depth)
        return -1;

      else if  (i1.Depth > i2.Depth)
        return 1;

      return  i1.ScanLine.CompareTo (i2.ScanLine);

    }  /* DatePicesInstrumentComparer */



    private  void  SummarizeInstrumentDataForADeployment ()
    {

      PicesInstrumentDataList  data = dbConn.InstrumentDataLoad (cruiseName, stationName,  deploymentNum);
      // We will assume that all entries are in order of date and time.  We want 
      // to first decide where the break in UpCast and Down cast occur.

      if  ((data == null)  ||  (data.Count < 1))
      {
        MessageBox.Show ("There is no Instrument Data for Cruise[" + cruiseName + "]  Station[" +  stationName + " Deployment [" + deploymentNum + "]");
        return;
      }


      // Will now locate the InstrumentData record that seperates the DownCast from the UpCast
      data.Sort (DatePicesInstrumentComparer);
      int  idx;
      float  maxDepth = -9999.9f;
      int    maxDepthIdx = - 9999;
      for  (idx = 0;  idx < data.Count;  idx++)
      {
        if  (data[idx].Depth > maxDepth)
        {
          maxDepth = data[idx].Depth;
          maxDepthIdx = idx;
        }
      }

      DateTime  refTime   = new DateTime (1999, 1, 1);
      DateTime  startTime = new DateTime (1999, 1, 1);
      bool  startTimeSet = false;

      InstrumentDataList  downCast = new InstrumentDataList ();
      InstrumentDataList  upCast   = new InstrumentDataList ();

      for  (idx = 0;  idx < data.Count;  idx++)
      {
        PicesInstrumentData pid = data[idx];
        if  (pid.CtdDate.CompareTo (refTime) < 0)
          continue;

        if  ((idx % 4) == 0)
        {
          if  (!startTimeSet)
          {
            startTime = pid.CtdDate;
            startTimeSet = true;
          }

          if  (idx <= maxDepthIdx)
            downCast.Add (new InstrumentData (pid));
          else
            upCast.Add   (new InstrumentData (pid));
        }
      }

      DateTime  endTime = data[data.Count - 1].CtdDate;

      data = null;

      String  title = cruiseName + "-" + stationName + "-" + deploymentNum + "  From [" + startTime.ToString ("u") + "]  to  [" + endTime.ToString ("u") + "]";

      {
        upCastPlot = new DepthPlot2 (configRec, downCast, title + "    Down Cast");
        upCastPlot.Show (parent);
      }

      {
        downCastPlot = new DepthPlot2 (configRec, upCast, title + "    Up Cast");
        downCastPlot.Show (parent);
      }
    }  /* SummarizeInstrumentDataForADeployment */

  }
}
