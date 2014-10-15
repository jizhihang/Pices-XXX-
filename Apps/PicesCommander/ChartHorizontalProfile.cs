using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;
using System.IO;

using SipperFile;
using PicesInterface;

namespace PicesCommander
{
  public partial class ChartHorizontalProfile : Form
  {
    PicesClassList  classes       = null;
    PicesClassList  activeClasses = null;
    String          rootDir       = "";

    private  String  cruise         = null;
    private  String  station        = null;
    private  String  deployment     = null;
    private  char    classKeyToUse  = 'P';

    private  int     sizeMin        = 150;
    private  int     sizeMax        = 0;

    private  float   probMin        = 0.0f;
    private  float   probMax        = 1.0f;

    private  float   depthMin       = 0.0f;
    private  float   depthMax       = 0.0f;

    private  bool    filterSize   = false;
    private  bool    filterProb   = false;
    private  bool    filterDepth  = false;

    private  String  criteriaStr  = "";

    //  All plot entries along the x-axis will be with respect to 'initialStartTime' but offset by number of scan lines.
    private  DateTime  initialStartTime; /**< The Statt Time of the 1st SipperFile  in the deployment. */

    private  double    scanRate;         /**< Scan rate of camera during the deployment.               */

    private  PicesSipperFileList  sipperFiles = null;

    private  float           timeInterval  = 1.0f;
    private  float           scanLinesInterval = 29950.0f;
    private  bool            includeSubClasses = false;

    private  String          infinityStr = ((char)8734).ToString ();
    
    private  PicesDataBase   mainWinConn = null;
    private  PicesDataBase   threadConn  = null;  /**< This connection will only be use by the background thread. */

    private  PicesMsgQueue   statusMsgs = null;
    private  PicesMsgQueue   msgQueue   = null;
    private  PicesRunLog     runLog     = null;

    private  List<float>     timeVolumeProfile = null;

    private  PicesGoalKeeper  goalie = new PicesGoalKeeper ("ChartHorizontalProfile");

    private  bool  weightByVolume       = false;
    
    private  bool  buildPlotDataRunning = false;
    private  bool  buildPlotDataDone    = false;
    private  bool  cancelRequested      = false;

    private  int  heightLast = 0;
    private  int  widthLast  = 0;

    private  Color  Instr1OrigColor;

    private  Color[]  colorTable = {Color.Black,      Color.Red,        Color.Green,      
                                    Color.Blue,       Color.Crimson,    Color.DarkCyan,   
                                    Color.DarkOrange, Color.Lime,       Color.SeaGreen
                                   };

    private  String  configFileName = "";
    private  bool    formIsMaximized = false;


    private  class  PlotRequest
    {
      public  PlotRequest (char             _srcOfData,
                           String           _name, 
                           Color            _color,
                           SeriesChartType  _chartType
                          )
      {
        srcOfData  = _srcOfData;
        name       = _name;
        color      = _color;
        chartType  = _chartType;
      }

      public  char             srcOfData;   // 'I'=Instrument,  'C'=Class
      public  String           name;        // Class or Instrument name.
      public  Color            color;
      public  SeriesChartType  chartType;
    }


    List<PlotRequest>  plotRequests = null;


    private  class  DataSeriesToPlot
    {
      public  DataSeriesToPlot (char             _sourceType,     // 'C'=Class, 'I'=Instrument
                                String           _unitOfMeasure,
                                String           _legend,
                                List<DataPoint>  _data,
                                List<int>        _counts,
                                float            _minVal,
                                String           _formatStr,
                                SeriesChartType  _chartType,
                                Color            _color
                               ) 
      {
        sourceType    = _sourceType;
        unitOfMeasure = _unitOfMeasure;
        legend        = _legend;
        data          = _data;
        counts        = _counts;
        minVal        = _minVal;
        formatStr     = _formatStr;
        chartType     = _chartType;
        color         = _color;
      }

      public  char             sourceType;
      public  String           unitOfMeasure;
      public  String           legend;
      public  List<DataPoint>  data;
      public  List<int>        counts;
      public  float            minVal;
      public  String           formatStr;
      public  SeriesChartType  chartType;
      public  Color            color;
    }

    
    private  List<DataSeriesToPlot>  series = new List<DataSeriesToPlot> ();




    public ChartHorizontalProfile (String          _cruise,
                                   String          _station,
                                   String          _deployment,
                                   int             _sizeMin,
                                   int             _sizeMax,
                                   float           _probMin,
                                   float           _probMax,
                                   float           _depthMin,
                                   float           _depthMax,
                                   char            _classKeyToUse,
                                   PicesClassList  _classes,
                                   PicesClassList  _activeClasses,
                                   String          _rootDir
                                  )
    {
      cruise        = _cruise;
      station       = _station;
      deployment    = _deployment;
      sizeMin       = _sizeMin;
      sizeMax       = _sizeMax;
      probMin       = _probMin;
      probMax       = _probMax;
      depthMin      = _depthMin;
      depthMax      = _depthMax;
      classKeyToUse = _classKeyToUse;
      classes       = _classes;
      activeClasses = _activeClasses;
      rootDir       = _rootDir;

      if  ((sizeMin > 0)      &&  (sizeMax <= 0))      sizeMax  = int.MaxValue;
      if  ((probMin >= 0.0f)  &&  (probMax <= 0.0f))   probMax  = 1.0f;
      if  ((depthMin > 0.0f)  &&  (depthMax <= 0.0f))  depthMax = float.MaxValue;

      filterSize =  (sizeMin > 0)      ||  ((sizeMax > 0)       &&  (sizeMax > sizeMin));
      filterProb  = ((probMin > 0.0f)  ||   (probMax  < 1.0f))  &&  (probMax > probMin);
      filterDepth = ((depthMax > 0)  &&  (depthMax > depthMin));

      statusMsgs = new PicesMsgQueue ("DepthProfile-StatusMsgs");
      msgQueue   = new PicesMsgQueue ("DepthProfile");
      runLog     = new PicesRunLog (msgQueue);

      mainWinConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      configFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "ChartHorizontalProfile.cfg";

      BuildCriteriaString ();

      InitializeComponent ();
    }


    private  void  AddToStr (ref String existingStr, String  addOn)
    {
      if  (String.IsNullOrEmpty (addOn))
        return;

      if  (!String.IsNullOrEmpty (existingStr))
        existingStr += ", ";
      existingStr += addOn;
    }


    private  void  BuildCriteriaString ()
    {
      criteriaStr = "";
      //AddToStr (ref criteriaStr, cruise);
      //AddToStr (ref criteriaStr, station);
      //AddToStr (ref criteriaStr, deployment);

      String  sizeMaxStr = infinityStr;
      if  ((sizeMax > 0)  &&  (sizeMax < int.MaxValue))
        sizeMaxStr = sizeMax.ToString ("##,##0");

      String  depthMaxStr = infinityStr;
      if  ((depthMax > 0.0f)  ||  (depthMax < float.MaxValue))
        depthMaxStr = depthMax.ToString ("#,##0");

      if  (filterSize)
        AddToStr (ref criteriaStr, "Size(" + sizeMin + ", " + sizeMaxStr + ")");

      if  (filterProb)
        AddToStr (ref criteriaStr, "Prob(" + probMin.ToString ("p2") + ", " + probMax.ToString ("p2") + ")");

      if  (filterDepth)
        AddToStr (ref criteriaStr, "Depth(" + depthMin + ", " + depthMaxStr + ")");

      if  (classKeyToUse == 'V')
        AddToStr (ref criteriaStr, "Validated");
    }


    private  void  PopulateMinMaxFields ()
    {
      SizeProbDepthCriteria.Text = criteriaStr;
    }  /* PopulateMinMaxFields */


   
    private void ChartHorizontalProfile_Load (object sender, EventArgs e)
    {
      heightLast = Height;
      widthLast  = Width;

      Instr1OrigColor = Inst1ToPlot.BackColor;

      {
        DataGridViewButtonColumn dgvbc = (DataGridViewButtonColumn)(ClassesToPlot.Columns[1]);
        dgvbc.Text = "Browse";
        dgvbc.UseColumnTextForButtonValue = true;
        dgvbc = (DataGridViewButtonColumn)(ClassesToPlot.Columns[2]);
        dgvbc.Text = "Remove";
        dgvbc.UseColumnTextForButtonValue = true;
        dgvbc = (DataGridViewButtonColumn)(ClassesToPlot.Columns[3]);
        dgvbc.Text = "Color";

        dgvbc.UseColumnTextForButtonValue = true;
      }

      ClassesToPlot.Rows.Clear ();
      //AddClassButton_Click (AddClassButton, null);

      //{
      //  DataGridViewButtonCell dgvbc = (DataGridViewButtonCell)(ClassesToPlot.Rows[0].Cells[1]);
      //  dgvbc.Value = "Browse";
      //  dgvbc = (DataGridViewButtonCell)(ClassesToPlot.Rows[0].Cells[2]);
      //  dgvbc.Value = "Remove";
      //  dgvbc = (DataGridViewButtonCell)(ClassesToPlot.Rows[0].Cells[3]);
      //  dgvbc.Value = colorTable[0].Name;
      //}
      
      Inst1ToPlot.Items.Clear ();
      Inst1ToPlot.Items.Add ("");

      Inst1ToPlot.Items.Add ("Volume-Sampled");

      for  (int xx = 0;  xx < InstrumentData.NumDataFields (); ++xx)
      {
        String s = InstrumentData.FieldName (xx);
        Inst1ToPlot.Items.Add (s);
      }

      String  cruiseStationDeployment = cruise + " - " + station;
      if  (!String.IsNullOrEmpty (deployment))
        cruiseStationDeployment += " - " + deployment;

      CruiseStationDeployment.Text = cruiseStationDeployment;

      PopulateMinMaxFields ();

      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("Copy Chart to clipboard",               null, CopyChartToClipboard);
      cms.Items.Add ("Save Chart to Disk",                    null, SaveChartToDisk);
      cms.Items.Add ("Copy Data Tab-Delimited to Clipboard",  null, SaveTabDelToClipBoard);
      cms.Items.Add ("Save Data Tab-Delimited to Disk",       null, SaveTabDelToDisk);
      ProfileChart.ContextMenuStrip = cms;

      LoadConfigurationFile ();
      String  selInst1Name = (String)Inst1ToPlot.SelectedItem;
      if  (String.IsNullOrEmpty (selInst1Name))
      {
        Inst1ToPlot.SelectedItem = "Volume-Sampled";
        SetInstrumentColor (FirstUnUsedColor (), Inst1ToPlot, Inst1ColorButton);
      }

      PlotButton_Click (this, null);
    }


    private  void  DisableControls ()
    {
      ClassesToPlot.Enabled = false;
      AddClassButton.Enabled = false;
      IncludeSubClasses.Enabled = false;
      TimeIncr.Enabled = false;
      Inst1ToPlot.Enabled = false;
      Inst1ColorButton.Enabled = false;
      PlotButton.Enabled = false;
      RePlotButton.Enabled = false;
    }


    private  void  EnableControls ()
    {
      ClassesToPlot.Enabled = true;
      AddClassButton.Enabled = true;
      IncludeSubClasses.Enabled = true;
      TimeIncr.Enabled = true;
      Inst1ToPlot.Enabled = true;
      Inst1ColorButton.Enabled = true;
      PlotButton.Enabled = true;
      RePlotButton.Enabled = true;
    }


    private  void  SaveConfiguration ()
    {
      System.IO.StreamWriter  o = null;
      try{o = new System.IO.StreamWriter (configFileName);}  catch  (Exception){o = null; return;}
      
      o.WriteLine ("//ChartHorizontalProfile Configuration File");
      o.WriteLine ("//");
      o.WriteLine ("//DateTime Saved" + "\t" + DateTime.Now.ToString ("F"));
      o.WriteLine ("//");
      o.WriteLine ("WidthLast"          + "\t" + widthLast);
      o.WriteLine ("HeightLast"         + "\t" + heightLast);
      o.WriteLine ("Maximized"          + "\t" + (formIsMaximized ? "YES":"NO"));
      o.WriteLine ("IncludeSubClasses"  + "\t" + (IncludeSubClasses.Checked ? "Yes" : "No"));
      o.WriteLine ("WeightByVolume"     + "\t" + (WeightByVolume.Checked ? "Yes" : "No"));

      String  selInst1Str = GetSelectedInstruemnt (Inst1ToPlot);
      if  (!String.IsNullOrEmpty (selInst1Str))
      {
        o.WriteLine ("Inst1ToPlot"        + "\t" + selInst1Str);
        o.WriteLine ("Inst1ColorButton"   + "\t" + Inst1ColorButton.BackColor.Name);
      }
      
      o.WriteLine ("TimeIncr"          + "\t" + TimeIncr.Value);

      foreach  (DataGridViewRow  dgvr in ClassesToPlot.Rows)
      {
        if  (dgvr.Cells.Count < 4)
          continue;

        String  cn = (String)dgvr.Cells[0].Value;
        PicesClass  pc = classes.LookUpByName (cn);
        if  (pc == null)
          continue;

        Color c = dgvr.DefaultCellStyle.BackColor;

        o.WriteLine ("ClassToPlot" + "\t" + pc.Name + "\t" + c.Name); 
      }

      o.Close ();
      o = null;
    }


    private  void  SetInstrumentColor (Color     c,
                                       ComboBox  comboBox,
                                       Button    button
                                      )
    {
      try
      {
        Color  revColor = DeriveOppositeColor (c);
        comboBox.BackColor = c;
        comboBox.ForeColor = revColor;
        button.BackColor = c;
        button.ForeColor = revColor;
        button.Text = c.Name;
      }
      catch (Exception)
      {
        comboBox.BackColor = Color.Black;
        comboBox.ForeColor = Color.White;
        button.BackColor = Color.Black;
        button.ForeColor = Color.White;
        button.Text = c.Name;
      }
    }



    private  String   VerifyClassName (String n)
    {
      if  (String.IsNullOrEmpty (n))
        return  "";

      PicesClass  pc = PicesClassList.GetUniqueClass (n, "");
      if  (pc == null)
        return null;
      else
        return pc.Name;
    }


    private  Color  VerifyColor (String  colorStr)
    {
      if  (String.IsNullOrEmpty (colorStr))
        return  FirstUnUsedColor ();

      Color c = Color.Black;
      try  {c = Color.FromName (colorStr);}
      catch (Exception)  {c = FirstUnUsedColor ();}
      return  c;
    }


    private  String  GetSelectedInstruemnt (ComboBox  cb)
    {
      if  (cb == null)                return null;
      if  (cb.SelectedItem == null)   return null;

      String name = (String)cb.SelectedItem;
      if  (String.IsNullOrEmpty (name))
        return null;
      return  name;
    }


    private  void  SetRowColor (int    rowIdx,
                                Color  c
                               )
    {
      Color  oc = DeriveOppositeColor (c);
      ClassesToPlot.Rows[rowIdx].DefaultCellStyle.BackColor = c;
      ClassesToPlot.Rows[rowIdx].DefaultCellStyle.ForeColor = oc;
      ClassesToPlot.Rows[rowIdx].Cells[3].Value = c.Name;

      ClassesToPlot[0, rowIdx].Style.BackColor = c;
      ClassesToPlot[0, rowIdx].Style.BackColor = oc;
      ClassesToPlot[1, rowIdx].Style.BackColor = c;
      ClassesToPlot[1, rowIdx].Style.BackColor = oc;
      ClassesToPlot[2, rowIdx].Style.BackColor = c;
      ClassesToPlot[2, rowIdx].Style.BackColor = oc;
      ClassesToPlot[3, rowIdx].Style.BackColor = c;
      ClassesToPlot[3, rowIdx].Style.BackColor = oc;
    }


    private  Color  ColorFromStr (String s)
    {

      Color c = Color.FromName(s);
      if  (c.A == 0)
      {
        int  value = 0;
        try  {value = Convert.ToInt32(s, 16);}
        catch  (Exception) {value = 0;}
        c = Color.FromArgb (value);
      }
      return c;
    }  /* ColorFromStr*/



    private  void   LoadConfigurationFile ()
    {
      System.IO.StreamReader i = null;

      try {i = new System.IO.StreamReader (configFileName);}  catch  (Exception) {i = null;}
      if  (i == null)
        return;

      int  savedWidth  = 0;
      int  savedHeight = 0;
      bool screenWasMaximized = false;

      String  nextLine = null;

      while  (true)
      {
        try  {nextLine = i.ReadLine ();}  catch (Exception) {break;}
        if  (nextLine == null)
          break;

        nextLine = nextLine.Trim ();
        
        if  ((nextLine.Length < 3)  ||  (nextLine.Substring (0, 2) == "//"))
          continue;

        String[] fields = nextLine.Split ('\t');
        if  (fields.Length < 2)
          continue;

        String  fieldName  = fields[0];
        String  fieldValue = fields[1];

        switch  (fieldName)
        { 
          case  "WidthLast":
            savedWidth = PicesKKStr.StrToInt (fieldValue);
            break;

          case  "HeightLast":
            savedHeight = PicesKKStr.StrToInt (fieldValue);
            break;

          case  "Maximized":
            screenWasMaximized  = (fieldValue.ToUpper () == "YES");
            break;

          case  "IncludeSubClasses":
            IncludeSubClasses.Checked = PicesKKStr.StrToBool (fieldValue);
            break;

          case  "WeightByVolume":
            WeightByVolume.Checked = PicesKKStr.StrToBool (fieldValue);
            break;

          case  "Inst1ToPlot":
            if  (!String.IsNullOrEmpty (fieldValue))
              Inst1ToPlot.SelectedItem = fieldValue;     
            break;

          case  "Inst1ColorButton":
            if  ((!String.IsNullOrEmpty (fieldValue))  &&  (GetSelectedInstruemnt (Inst1ToPlot) != null))
              SetInstrumentColor (VerifyColor (fieldValue), Inst1ToPlot, Inst1ColorButton);
            break;

          case  "TimeIncr":
            TimeIncr.Value = (decimal)PicesKKStr.StrToFloat (fieldValue);
            break;

          case  "ClassToPlot":        
            {
              if  (fields.Length < 3)
                continue;
              String  className = VerifyClassName (fields[1]);
              if  (!String.IsNullOrEmpty (className))
              {
                String  colorStr = fields[2];
                PicesClass pc = PicesClassList.GetUniqueClass (className, "");
                if  (pc != null)
                {
                  Color c = ColorFromStr (colorStr);
                  DataGridViewRow dgvr = new DataGridViewRow ();
                  int  newRowIdx = ClassesToPlot.Rows.Count;
                  ClassesToPlot.Rows.Insert (newRowIdx, dgvr);
                  ClassesToPlot.Rows[newRowIdx].Cells[0].Value = pc.Name;
                  ClassesToPlot.Rows[newRowIdx].Cells[3].Value = c.Name;
                  SetRowColor (newRowIdx, c);
                }
              }
            }
            break;
        }
      }

      i.Close ();

      if  (savedWidth > Screen.PrimaryScreen.Bounds.Width)
        savedWidth = Screen.PrimaryScreen.Bounds.Width;

      if  (savedHeight > Screen.PrimaryScreen.Bounds.Height)
        savedHeight = Screen.PrimaryScreen.Bounds.Height;

      if  (screenWasMaximized)
      {
        //savedWidth = Screen.PrimaryScreen.Bounds.Width;
        //savedHeight = savedHeight = Screen.PrimaryScreen.Bounds.Height;
        this.WindowState = FormWindowState.Maximized; 
      }
      else
      {
        Height = Math.Max (savedHeight, MinimumSize.Height);
        Width  = Math.Max (savedWidth,  MinimumSize.Width);
      }

      OnResizeEnd (new EventArgs ());
    }  /* LoadConfigurationFile */


    private  Color  DeriveOppositeColor (Color c)
    {
      byte red   = c.R;
      byte green = c.G;
      byte blue  = c.B;

      int totColor = red + green + blue;
      if  (totColor > (255 * 3 / 2))
        return Color.Black;
      else
        return Color.White;
    }


    

    private void ClassesToPlot_RowsAdded (object sender, DataGridViewRowsAddedEventArgs e)
    {
      int zed = e.RowIndex;
      DataGridView dgv = (DataGridView)sender;
      int  numRows = dgv.Rows.Count;
      int  numCols = dgv.Rows[zed].Cells.Count;
      Object o = dgv.Rows[zed].Cells[1].Value;
    }


    private void ClassesToPlot_CellClick (object sender, DataGridViewCellEventArgs e)
    {
      if  (e.ColumnIndex == 1)
      {
        int rowIdx = e.RowIndex;
        if  (rowIdx < 0)
          return;

        SelectAPicesClass  sapc = new SelectAPicesClass (classes, rootDir, activeClasses);

        Object  curClassNameObj = ClassesToPlot.Rows[rowIdx].Cells[0].Value;
        if  (curClassNameObj != null)
          sapc.SelectedClass = PicesClassList.GetUniqueClass (curClassNameObj.ToString (), "");
        sapc.ShowDialog (this);
        if  (sapc.SelectionMade)
        {
          PicesClass pc = sapc.SelectedClass;
          if  (pc != null)
          {
            ClassesToPlot.Rows[rowIdx].Cells[0].Value = pc.Name;
          }
        }
      }

      else if  (e.ColumnIndex == 2)
      {
        int rowIdx = e.RowIndex;
        ClassesToPlot.Rows.Remove (ClassesToPlot.Rows[rowIdx]);
      }

      else if  (e.ColumnIndex == 3)
      {
        int rowIdx = e.RowIndex;
        Color c = ClassesToPlot.Rows[rowIdx].DefaultCellStyle.BackColor;
        ColorDialog cd = new ColorDialog ();
        cd.Color = c;
        cd.AllowFullOpen = false;
        cd.FullOpen = false;
        cd.SolidColorOnly = true;
        DialogResult dr = cd.ShowDialog ();
        if  (dr == DialogResult.OK)
        {
          Color  c2 = cd.Color;
          SetRowColor (rowIdx, c2);
        }
      }
    }

    
    private Color  FirstUnUsedColor ()
    {
      foreach  (Color c in colorTable)
      {
        bool  colorIsUsed = false;
        foreach  (DataGridViewRow r in ClassesToPlot.Rows)
        {
          if  (r.DefaultCellStyle.BackColor == c)
          {
            colorIsUsed = true;
            break;
          }
        }
        if  (!colorIsUsed)
        {
          if  (c == Inst1ColorButton.BackColor)
            colorIsUsed = true;

          else
            return c;
        }
      }
      return  Color.Black;
    }


    private  String  CheckIfClassAlreadyIncluded (PicesClass  c)
    {
      String  error = null;

      PicesClassList classesAlreadyChecked = new PicesClassList ();
      foreach  (DataGridViewRow dgvr in ClassesToPlot.Rows)
      {
        if  (dgvr.Cells.Count < 2)
          continue;

        String  className = (String)dgvr.Cells[0].Value;
        PicesClass  existingClass = PicesClassList.GetUniqueClass (className, "");
        if  (c == existingClass)
        {
          error = "Class[" + c.Name + "] is already been selected.";
          break;
        }

        else if  (IncludeSubClasses.Checked)
        {
          if  (existingClass.IsInMySubTree (c))
          {
            error = "Class[" + c.Name + "] is included in Class[" + existingClass.Name + "]";
            break;
          }
          else if  (c.IsInMySubTree (existingClass))
          {
            error = "Class[" + c.Name + "] is a Parent Class to Class[" + existingClass.Name + "]";
          }
        }
      }

      return  error;
    }  /* CheckIfClassAlreadyIncluded */



    private  void   ValidateClassSelections (ref List<String>  errors)
    {
      PicesClassList classesAlreadyChecked = new PicesClassList ();
      foreach  (DataGridViewRow dgvr in ClassesToPlot.Rows)
      {
        if  (dgvr.Cells.Count < 2)
          continue;

        String  className = (String)dgvr.Cells[0].Value;
        PicesClass  nextClass = PicesClassList.GetUniqueClass (className, "");

        String  error = null;

        foreach  (PicesClass existingClass in classesAlreadyChecked)
        {
          if  (nextClass == existingClass)
          {
            error = "Class[" + nextClass.Name + "] is included more than once.";
            break;
          }
          else if  (IncludeSubClasses.Checked)
          {
            if  (existingClass.IsInMySubTree (nextClass))
            {
              // We are adding a class that is already been included as part of a previously listed class.
              error = "Class[" + nextClass.Name + "] is included in Class[" + existingClass.Name + "]";
              break;
            }

            else if  (nextClass.IsInMySubTree (existingClass))
            {
              // We are adding a class that is already been included as part of a previously listed class.
              error = "Class[" + nextClass.Name + "] is a Parent to Class[" + existingClass.Name + "]";
              break;
            }
          }
        }

        if  (error != null)
          errors.Add (error);
        else
          classesAlreadyChecked.Add (nextClass);
      }
      return;
    }  /* ValidateClassSelections */



    private  List<String>  ValidateFields ()
    {
      List<String>  errors = new List<string> ();
      ValidateClassSelections (ref errors);
      
      if  (errors.Count < 1)
        return null;
      else
        return errors;
    }



    private void AddClassButton_Click (object sender, EventArgs e)
    {
      SelectAPicesClass sapc = new SelectAPicesClass (classes, rootDir, activeClasses);
      sapc.ShowDialog (this);
      if  (sapc.SelectionMade)
      {
        PicesClass  selectedClass = sapc.SelectedClass;
        String  errMsg = CheckIfClassAlreadyIncluded (selectedClass);
        if  (errMsg != null)
        {
          MessageBox.Show (this, errMsg, "Class Selection", MessageBoxButtons.OK);
        }
        else
        {
          Color c = FirstUnUsedColor ();
          //DataGridViewRow dgvr = new DataGridViewRow ();
          int  newRowIdx = ClassesToPlot.Rows.Count;
          ClassesToPlot.Rows.Insert (newRowIdx, 1);
          ClassesToPlot[0,newRowIdx].Value = selectedClass.Name;
          ClassesToPlot[3,newRowIdx].Value = c.Name;
          SetRowColor (newRowIdx, c);
        }
      }
    }

    


    private  float[]  VolumeByTime (PicesSipperFile  sf)
    {
      float[]  volumeByTime = null;

      


      return  volumeByTime;
    }
                                 

    /// <summary>
    /// Returns a Volume by 
    /// </summary>
    /// <param name="sf"></param>
    /// <returns></returns>
    /// 
    private  List<DataPoint>   RetrieveVolumePerInstrumentDataRec (PicesSipperFile  sf)
    {
      long  initailTimeOffset = sf.DateTimeStart.ToBinary ();

      TimeSpan  deltaTime = (sf.DateTimeStart - initialStartTime);
      double  startScanLineOffset = deltaTime.TotalSeconds * scanRate;

      List<DataPoint>  flowRates = null;

      String  sqlStr = "select  id.ScanLine as ScanRow, id.FlowRate1 as FlowRate" + "\n" +
                       "        from  InstrumentData id"                           + "\n" +
                       "        where id.SipperFileId = (select sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = \"" + sf.SipperFileName + "\")";

      String[]  colNames = {"ScanRow", "FlowRate"};
      String[][]  results = threadConn.QueryStatement (sqlStr, colNames);
      if  ((results == null)  ||  (results.Length < 1))
        return null;

      double  timePerInstrumentDataRec = 4096 / scanRate;

      flowRates = new List<DataPoint> ();

      for  (int x = 0;  x < results.Length;  ++x)
      {
        int  scanLine = PicesKKStr.StrToInt (results[x][0]);
        double  flowRate = PicesKKStr.StrToFloat (results[x][1]);
        double  volume = 0.098 * 0.098 * (flowRate * timePerInstrumentDataRec);
        flowRates.Add (new DataPoint (startScanLineOffset + scanLine, volume));
      }
      return  flowRates;
    }  /* RetrieveVolumePerInstrumentDataRec */




    /// <summary>
    /// Returns a Volume by 
    /// </summary>
    /// <param name="sf"></param>
    /// <returns></returns>
    private  float[]   RetrieveSipperFileFlowRates (PicesSipperFile  sf)
    {
      String  sqlStr = "select  id.ScanLine as ScanRow, id.FlowRate1 as FlowRate" + "\n" +
                       "        from  InstrumentData id"                           + "\n" +
                       "        where id.SipperFileId = (select sf.SipperFileId  from  SipperFiles sf  where  sf.SipperFileName = \"" + sf.SipperFileName + "\")";

      String[]  colNames = {"ScanRow", "FlowRate"};
      String[][]  results = threadConn.QueryStatement (sqlStr, colNames);
      if  ((results == null)  ||  (results.Length < 1))
        return null;

      double  timePerInstrumentDataRec = 4096 / scanRate;

      float[]  flowRates = new float[results.Length];

      for  (int x = 0;  x < results.Length;  ++x)
      {
        int  scanLine = PicesKKStr.StrToInt (results[x][0]);
        int  idx = scanLine / 4096;
        if  ((idx >= 0)  &&  (idx < results.Length))
        {
          double  flowRate = PicesKKStr.StrToFloat (results[x][1]);
          flowRates[idx] = (float)flowRate;
        }
      }
      return  flowRates;
    }  /* RetrieveSipperFileFlowRates */




    private  void  GetTimeVolumeProfile (PicesSipperFile  sf,
                                         ref List<float>  volProfile
                                        )
    {
      int  startSipperFileScanLine = StartScanLineForSipperFile (sf);
 
      float[]  flowRates = RetrieveSipperFileFlowRates (sf);
      if  (flowRates == null)
        return;

      int  maxTimeIntervalIdx = 1 + (int)(0.5f + (startSipperFileScanLine + flowRates.Length * 4096) / scanLinesInterval);

      while  (volProfile.Count < maxTimeIntervalIdx)
        volProfile.Add (0.0f);

      for  (int idx = 0;  idx < flowRates.Length;  ++idx)
      {
        int  idScanLine     = startSipperFileScanLine + idx * 4096;
        int  idScanLineNext = idScanLine + 4096;

        int  intervalIdx     = (int)(0.5f + idScanLine           / scanLinesInterval);
        int  intervalLastIdx = (int)(0.5f + (idScanLineNext - 1) / scanLinesInterval);

        int  intervalScanLine = idScanLine;

        while  (intervalIdx <= intervalLastIdx)
        {
          int  intervalNextScanLine = (int)(0.5f + (intervalIdx + 1) * scanLinesInterval) - 1;
          if  (intervalNextScanLine >= idScanLineNext)
            intervalNextScanLine = idScanLineNext - 1;

          int  deltaScanLines = 1 + intervalNextScanLine - intervalScanLine;
          float  vol = (float)((flowRates[idx] * (deltaScanLines / scanRate)) * 0.098f * 0.098f);

          while  (intervalIdx >= volProfile.Count)
            volProfile.Add (0.0f);

          volProfile[intervalIdx] += vol;
          ++intervalIdx;
        }
      }
    }  /* GetTimeVolumeProfile */



    private  List<float>  GetTimeVolumeProfile ()
    {
      List<float> volProfile = new List<float> ();

      foreach  (PicesSipperFile sf in sipperFiles)
      {
        if  (cancelRequested)
          break;

        int  startSipperFileScanLine = StartScanLineForSipperFile (sf);

        statusMsgs.AddMsg ("Retrieving volume profile for SipperFile[" + sf.SipperFileName + "]");
    
        GetTimeVolumeProfile (sf, ref volProfile);
      }

      if  (cancelRequested)
        return null;
      else
        return  volProfile;
    }  /* GetTimeVolumeProfile */




    private  void   CountsByTime (PicesSipperFile  sf,
                                  PicesClass       c,
                                  float            timeInterval,
                                  ref List<int>    counts
                                 )
    {
      int  scanLines = (int)(timeInterval * sf.ScanRate + 0.5f);
      int  sl = scanLines;
      int  startScanLine = StartScanLineForSipperFile (sf);

      String  classFieldToUse = "i.Class1Id";
      if  (classKeyToUse == 'V')
        classFieldToUse = "i.ClassValidatedId";

      List<String>  whereClauses = new List<string> ();
      whereClauses.Add (classFieldToUse + " = (Select c.ClassId from Classes c  where c.ClassName = \"" + c.Name + "\")");
      whereClauses.Add ("i.SipperFileId = (select sf.SipperFileId from SipperFiles sf  where sf.SipperFileName = \"" + sf.SipperFileName + "\")");
      if  (filterProb)
        whereClauses.Add ("i.Class1Prob >= " + probMin  + "  and  i.Class1Prob <= " + probMax);

      if  (filterSize)
        whereClauses.Add ("i.PixelCount >= " + sizeMin  + "  and  i.PixelCount <= " + sizeMax);

      if  (filterDepth)
        whereClauses.Add ("i.Depth >= "      + depthMin + "  and  i.Depth <= " + depthMax);

      String  whereStr =  "";
      for  (int x = 0;  x < whereClauses.Count;  ++x)
      {
        if  (x > 0)  
          whereStr += " and " + "\n" + "             ";
        whereStr += whereClauses[x];
      }

      String  intervalIdx = "Floor ((" + startScanLine.ToString () + " + i.TopLeftRow) / " + sl + ")";

      String  sqlString = "select count(*)"                + " as Count, " + 
                                  intervalIdx              + " as IntervalIdx, " + 
                                  intervalIdx + " * " + sl + " as ScanRow"           + "\n" +
                          "       from images i"                                     + "\n" +
                          "       where " + whereStr                                 + "\n" +
                          "       group by " + intervalIdx;

      String[]  colNames = {"Count", "IntervalIdx", "ScanRow"};
      String[][]  results = threadConn.QueryStatement (sqlString, colNames);
      if  ((results == null)  ||  (results.Length < 1))
        return;

      String  lastIdxStr = results[results.Length - 1][1];
      int  lastIdx = PicesKKStr.StrToInt (lastIdxStr);

      while  (counts.Count < lastIdx)
        counts.Add (0);

      for  (int x = 0; x < results.Length; x++)
      {
        int  count = PicesKKStr.StrToInt (results[x][0]);
        int  idx   = PicesKKStr.StrToInt (results[x][1]);

        if  ((idx < 0)  ||  (idx >= lastIdx))
          continue;
        counts[idx] += count;
      }

      if  (includeSubClasses)
      {
        foreach (PicesClass subClass in c.Children)
        {
          CountsByTime (sf, subClass, timeInterval, ref counts);
        }
      }

      return;
    }  /* CountsByTime */




    private int  DateToDaysSince1980 (DateTime  dt)
    {
      int yearsAlreadyPast = ((dt.Year - 1980) - 1);
      int  days = yearsAlreadyPast * 365 + (yearsAlreadyPast / 4);
      days += dt.DayOfYear - 1;
      return  days;
    }

    private  int  secsInDay = 24 * 60 * 60;

    
    
    private  double  DateTimeToSecs (DateTime dt)
    {
      int  daysPastAlready = DateToDaysSince1980 (dt);
      return  (double)daysPastAlready * secsInDay + dt.Hour * 3600 + dt.Minute * 60 + dt.Second;
    }



    private  int  StartScanLineForSipperFile (PicesSipperFile sf)
    {
      int  startLine = 0;

      TimeSpan  ts = sf.DateTimeStart - initialStartTime;
      startLine = Math.Max (0, (int)((float)((int)(ts.TotalSeconds + 0.5f)) * scanRate));
      
      return  startLine;
    }  /* StartScanLineForSipperFile */




    private  List<int>  CountsByTime (PicesClass  c)
    {
      List<int> counts = new List<int> ();

      foreach  (PicesSipperFile sf in sipperFiles)
      {
        if  (cancelRequested)
          break;

        int  startSipperFileScanLine = StartScanLineForSipperFile (sf);
        statusMsgs.AddMsg ("Retrieving counts for Class[" + c.Name + "]  SipperFile[" + sf.SipperFileName + "]");
        CountsByTime (sf, c, timeInterval, ref counts);
      }

      if  (cancelRequested)
        return null;
      else
        return  counts;
    }  /* CountsByTime */



    
    private  void  AddClassToSeries (PicesClass       pc,
                                     Color            color,
                                     SeriesChartType  charType
                                    )
    {
      String  unitOfMeasure = "Count";
      if  (WeightByVolume.Checked)
        unitOfMeasure = "Count/m-3";

      List<int>  counts = CountsByTime (pc);
      int minLen = Math.Min (counts.Count, timeVolumeProfile.Count);
      List<float>  weightedCounts = new List<float> ();

      for  (int x = 0;  x < minLen;  ++x)
      {
        float  weightedCount = 0.0f;
        if  (timeVolumeProfile[x] > 0.0f)
          weightedCount = counts[x] / timeVolumeProfile[x];

        weightedCounts.Add (weightedCount);
      }

      List<DataPoint>  data = new List<DataPoint> ();
      for  (int idx = 0;  idx < counts.Count;  ++idx)
      {
        if  (weightByVolume)
          data.Add (new DataPoint (idx * scanLinesInterval, weightedCounts[idx]));
        else
          data.Add (new DataPoint (idx * scanLinesInterval, counts[idx]));
      }

      goalie.StartBlock ();

      series.Add (new DataSeriesToPlot ('C', unitOfMeasure, pc.Name, data, counts, 0, "###,##0", charType, color));
      goalie.EndBlock ();
    }  /* AddClassToSeries */



    private  void  AddVolumeSampled (Color            color,
                                     SeriesChartType  chartType
                                    )
    {
      String  unitOfMeasure = "m-3";

      List<DataPoint>  data = new List<DataPoint> ();
      for  (int idx = 0;  (idx < timeVolumeProfile.Count)  &&  (!cancelRequested);  ++idx)
      {
        data.Add (new DataPoint (idx * scanLinesInterval, timeVolumeProfile[idx]));
      }

      goalie.StartBlock ();
      series.Add (new DataSeriesToPlot ('I', unitOfMeasure, "Volume Sampled", data, null, 0, "#,##0.0", chartType, color));
      goalie.EndBlock ();
    }  /* AddVolumeSampled */



    private  void  AddInstrumentData (String           instName, 
                                      Color            color,
                                      SeriesChartType  chartType
                                     )
    {
      statusMsgs.AddMsg ("Extracting " + instName + " data.");

      if  (instName == "Volume-Sampled")
      {
        AddVolumeSampled (color, chartType);
        return;
      }

      int  fieldNum = PicesInstrumentData.GetFieldNum (instName);
      if  (fieldNum < 0)
        return;

      List<String>  fieldNames = new List<string> ();
      fieldNames.Add ("CTDDateTime");
      fieldNames.Add (instName);


      String  whereStr = "where  (sf.CruiseName = \"" + cruise + "\"";
      if  (!String.IsNullOrEmpty (station))
        whereStr += ("  and   sf.StationName = \"" + station + "\"");

      if  (!String.IsNullOrEmpty (deployment))
        whereStr += ("  and   sf.DeploymentNum = \"" + deployment + "\"");
      whereStr += ")";

      String  sqlStr = "select id.CTDDateTime, id." + instName + "\n" + 
                       "       from InstrumentData id"         + "\n" + 
                       "       join(SipperFiles sf)  on(sf.SipperFileId = id.SipperFileId)" + "\n" +
                       "       " + whereStr + "\n" +
                       "       order by id.CTDDateTime";

      String[][]  results = threadConn.QueryStatement (sqlStr, null);
      if  (results == null)
      {
        String  msg = "Instrument[" + instName + "] did not return any data.";
        statusMsgs.AddMsg (msg);
        msgQueue.AddMsg (msg);
        return; 
      }

      String  unitOfMeasure = PicesInstrumentData.FieldUnitOfMeasure (fieldNum);
      String  formatStr     = PicesInstrumentData.FieldFormatStr     (fieldNum);
      float   minVal        = PicesInstrumentData.FieldMinValue      (fieldNum);
      int  numRows = results.Length;

      List<DataPoint>  data = new List<DataPoint> ();
      for  (int idx = 0;  ((idx < numRows)  &&  (!cancelRequested));  ++idx)
      {
        DateTime  dt = PicesKKStr.StrToDateTime (results[idx][0]);
        float     value = PicesKKStr.StrToFloat (results[idx][1]);
        if  (value < minVal)
          continue;

        if  (dt < initialStartTime)
          continue;

        TimeSpan  ts = dt - initialStartTime;
        if  (ts.Hours > 12)
          continue;

        double  intervalScanLines = ts.TotalSeconds * scanRate;
        int  intervalIDX = (int)(0.5f + intervalScanLines / scanLinesInterval);
        //while  (data.Count <= intervalIDX)
        //{
        //  data.Add (new DataPoint (data.Count * scanLinesInterval, 0.0));
        //}
        //
        //data[intervalIDX].YValues[0] = value;
        data.Add (new DataPoint (intervalScanLines, value));
      
      
      }

      goalie.StartBlock ();
      series.Add (new DataSeriesToPlot ('I', unitOfMeasure, instName, data, null, minVal, formatStr, chartType, color));
      goalie.EndBlock ();
    }  /* AddInstrumentData */



    /// <summary>
    /// This next method will be ran as a separate thread; it is respnable for collecting all the data needed to generate the plot.
    /// </summary>
    private  void  BuildPlotData ()
    {
      if  (buildPlotDataRunning)
        return;

      PicesDataBase.ThreadInit ();

      threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      buildPlotDataRunning = true;

      timeVolumeProfile = GetTimeVolumeProfile ();

      series = new List<DataSeriesToPlot> ();

      foreach  (PlotRequest pr in plotRequests)
      {
        if  (cancelRequested)
          break;

        String  cn = pr.name;

        if  (pr.srcOfData == 'C')
        {
          PicesClass  pc = classes.LookUpByName (cn);
          if  (pc == null)
            continue;

          AddClassToSeries (pc, pr.color, pr.chartType);
        }

        else if  (pr.srcOfData == 'I')
        {
          AddInstrumentData (pr.name, pr.color, pr.chartType);
        }
      }

      threadConn.Close ();
      threadConn = null;
      GC.Collect ();
      PicesDataBase.ThreadEnd ();

      if  (cancelRequested)
        statusMsgs.AddMsg ("Plottingof data CANCELLED!!!");
      else
        statusMsgs.AddMsg ("Building of plot data completed !!!");

      buildPlotDataRunning = false;
      buildPlotDataDone = true;
    }  /* BuildPlotData */



    private  void  CancelBackGroundThread ()
    {
      cancelRequested = true;
      if  (threadConn != null)
        threadConn.CancelFlag = true;
      statusMsgs.AddMsg ("Cancel Requested");
    }


    private  void  UpdateChartAreas ()
    {
      goalie.StartBlock ();
      
      ProfileChart.Titles.Clear ();
      ProfileChart.Titles.Add ("Cruise: " + cruise + "  Station: " + station + "  Deployment: " + deployment);

      if  (!String.IsNullOrEmpty (criteriaStr))
        ProfileChart.Titles.Add (criteriaStr);

      if  (series.Count < 1)
      {
        ProfileChart.Series.Clear ();
        goalie.EndBlock ();
        return;
      }

      ChartArea ca = ProfileChart.ChartAreas[0];
      ca.AxisY.Minimum = 0;
      ca.AxisY.Title = "Counts";
      if  (weightByVolume)
        ca.AxisY.Title = "Counts/m-3";

      ca.AxisX.Minimum = 0;
      ca.AxisX.Title = "Time";

      ca.AxisY.Title = "Count";
      ca.AxisY.Minimum = 0;

      ProfileChart.Series.Clear ();

      DataSeriesToPlot  instSeriesToPlot = null;

      int  seriesIDX = 0;


      int  distBetweenLabels = 50; 
      if  (series.Count > 0)
        distBetweenLabels = series[0].data.Count / 8;

      // First we will plot class counts on CharArea1
      foreach  (DataSeriesToPlot dstp in series)
      {
        if  (dstp.sourceType != 'C')
        {
          instSeriesToPlot = dstp;
          continue;
        }

        Series s = new Series (dstp.legend.Replace ('_', '\n'));
        s.ChartArea = "ChartArea1";
        s.ChartType = dstp.chartType;
        s.Name = dstp.legend;

        ca.AxisY.Title = dstp.unitOfMeasure;
        ca.AxisY.LabelStyle.Format = dstp.formatStr;
        ca.AxisY.Minimum = 0;
        ca.AxisY.IsReversed = false;
        s.YAxisType = AxisType.Primary;
        s.XAxisType = AxisType.Primary;
 
        s.Color = dstp.color;
        s.BorderWidth = 2;

        int  dpIDX = 0;
        foreach  (DataPoint dp in dstp.data)
        {
          float  yValue = (float)dp.YValues[0];
          if  (seriesIDX == 0)
          {
            DateTime  ctdDateTime = initialStartTime;
            double  offsetSecs = dpIDX * timeInterval;
            ctdDateTime  = ctdDateTime.AddSeconds (offsetSecs);
            dp.AxisLabel = ctdDateTime.ToString ("HH:mm:ss");
          }
          s.Points.Add (dp);
          ++dpIDX;
        }

        ProfileChart.Series.Add (s);
        ++seriesIDX;
      }

      if  (instSeriesToPlot != null)
      {
        float  minVal = instSeriesToPlot.minVal;

        if  (instSeriesToPlot.legend == "Depth")
        {
          ca.AxisY2.IsReversed = true;
        }
        else
        {
        }

        Series s = new Series (instSeriesToPlot.legend);
        s.ChartArea = "ChartArea1";

        s.ChartType = instSeriesToPlot.chartType;
        s.Name = instSeriesToPlot.legend;
        s.BorderWidth = 2;

        s.YAxisType = AxisType.Secondary;

        s.Color = instSeriesToPlot.color;

        int  dpIDX = 0;
        foreach  (DataPoint dp in instSeriesToPlot.data)
        {
          float  yValue = (float)dp.YValues[0];
          //DateTime  ctdDateTime = initialStartTime;
          //double  offsetSecs = dpIDX * timeInterval;
          //ctdDateTime = ctdDateTime.AddSeconds (offsetSecs);
          //dp.AxisLabel = ctdDateTime.ToString ("HH:mm:ss");
          s.Points.Add (dp);
          ++dpIDX;
        }

        s.Legend = "Legend1";

        ProfileChart.Series.Add (s);
      }

      ProfileChart.ChartAreas[0].RecalculateAxesScale ();

      goalie.EndBlock ();
    }  /* UpdateChartAreas */




    private  void  AddInstrumentToPlotRequest (ComboBox  instrument, 
                                               Button    color
                                              )
    {
      String  name = GetSelectedInstruemnt (instrument);
      if  (String.IsNullOrEmpty (name))
        return;
      
      if  (name == "Volume-Sampled")
        plotRequests.Add (new PlotRequest ('I', name, color.BackColor, SeriesChartType.Line));
      
      else if  (PicesInstrumentData.GetFieldNum (name) >= 0)
        plotRequests.Add (new PlotRequest ('I', name, color.BackColor, SeriesChartType.Point));
    }



    private  void  SchedulePlotRequests ()
    {
      plotRequests = new List<PlotRequest> ();
      foreach  (DataGridViewRow dgvr in ClassesToPlot.Rows)
      {
        if  (dgvr.Cells.Count < 3)
          continue;

        String  cn = (String)dgvr.Cells[0].Value;
        PicesClass  pc = classes.LookUpByName (cn);
        if  (pc == null)
          continue;

        plotRequests.Add (new PlotRequest ('C', cn, dgvr.DefaultCellStyle.BackColor, SeriesChartType.Column));
      }

      AddInstrumentToPlotRequest (Inst1ToPlot, Inst1ColorButton);
    }  /* SchedulePlotRequests */

    
    
    private  float  DetermineScanRate ()
    {
      float  scanRate = 29950;

      String  sqlStr = "select avg (sf.ScanRate)  as  ScanRate"      + "\n" +
                       "       from SipperFiles sf"                  + "\n" +
                       "       where sf.CruiseName = \"" + cruise        + "\"  and  " + 
                                    "sf.StationName = \"" + station      + "\"  and  " +
                                    "sf.DeploymentNum = \"" + deployment + "\"  and  " +
                                    "sf.ScanRate > 1000" + 
                                    "\n";

      String[]  colNames = {"ScanRate"};
      
      String[][]  results = mainWinConn.QueryStatement (sqlStr, colNames);
      if  ((results != null)  ||  (results.Length == 1))
      {
        scanRate = PicesKKStr.StrToFloat (results[0][0]);
        if  ((scanRate < 1000)  ||  (scanRate > 40000))
          scanRate = 29950;
      }

      return scanRate;
    }  /* DetermineScanRate */



    private void PlotButton_Click (object sender, EventArgs e)
    {
      List<String>  errors = ValidateFields ();
      if  (errors != null)
      {
        String  errMsg = "Errors is class selection";
        foreach (String s in errors)
          errMsg += "\n" + s;
        MessageBox.Show (this, errMsg, "Validating Class Selection", MessageBoxButtons.OK);
      }
      else
      {
        DisableControls ();
        SaveConfiguration ();
        PlotButton.Enabled = false;
        if  (buildPlotDataRunning)
          return;

        SchedulePlotRequests ();

        includeSubClasses = IncludeSubClasses.Checked;
        scanRate = DetermineScanRate ();
        timeInterval = (int)TimeIncr.Value;
        scanLinesInterval = (float)(timeInterval * scanRate);
        sipperFiles = mainWinConn.SipperFileLoad (cruise, station, deployment);

        initialStartTime = sipperFiles.DateTimeStart;

        timer1.Enabled = true;
        Thread t = new Thread (new ThreadStart (BuildPlotData));

        t.Start ();
      }
    }


    private void RePlotButton_Click (object sender, EventArgs e)
    {
      UpdateChartAreas ();
    }


    int ticks = 0;
    private void timer1_Tick (object sender, EventArgs e)
    {
      ++ticks;

      String msg = statusMsgs.GetNextMsg ();
      while  (msg != null)
      {
        Status.AppendText (msg + "\n");
        msg = statusMsgs.GetNextMsg ();
      }

      if   (buildPlotDataRunning)
      {
        if  ((ticks % 20) == 0)
          UpdateChartAreas ();
      }

      if  (buildPlotDataDone)
      {
        UpdateChartAreas ();
        PlotButton.Enabled = true;
        buildPlotDataDone = false;
        cancelRequested = false;
        EnableControls ();
      }
    }


    private void ChartHorizontalProfile_Resize (object sender, EventArgs e)
    {
      int  heightDelta = Height - heightLast;
      int  widthDelta  = Width  - widthLast;

      ProfileChart.Height += heightDelta;
      ProfileChart.Width  += widthDelta;

      Status.Width += widthDelta;

      heightLast = Height;
      widthLast  = Width;
    }



    private void ChartHorizontalProfile_SizeChanged (object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Maximized)
      {
        // Looks like user has pressed the Maximized button.  We have to trap it here because
        // the ResizeEnd envent does not trap when form is Maximized.
        //PicesCommanderFormResized ();
        ChartHorizontalProfile_Resize (sender, e);
        formIsMaximized = true;
      }
      else if  (WindowState == FormWindowState.Normal)
      {
        if  (formIsMaximized)
        {
          // We normally trap the ResizeEnd event;  but when the form was already maximized and the user
          // presses the button to unmaximize.  the ResizeEnd does not trap that.  So we check to 
          // see if the form was already maximize.  If so then we resized the form.
          //PicesCommanderFormResized ();
          ChartHorizontalProfile_Resize (sender, e);
          formIsMaximized = false;
        }
      }
    }



    private void ClassesToPlot_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
    {
      int rowIdx = e.RowIndex;
      if  ((rowIdx < 0)  ||  (rowIdx >= ClassesToPlot.Rows.Count))
        return;

      DataGridViewRow  dgvr = ClassesToPlot.Rows[rowIdx];
      if  (dgvr.Cells.Count < 4)
        return;

      Color  backColor = ClassesToPlot.Rows[rowIdx].DefaultCellStyle.BackColor;
      Color  foreColor = ClassesToPlot.Rows[rowIdx].DefaultCellStyle.ForeColor;
      
      e.CellStyle.BackColor = backColor;
      e.CellStyle.ForeColor = foreColor;
      
      if  (e.ColumnIndex == 3)
      {
        e.Value = backColor.Name;
      }
    }



    private void Inst1ToPlot_Enter (object sender, EventArgs e)
    {
      if  (Inst1ToPlot.BackColor == Instr1OrigColor)
        SetInstrumentColor (FirstUnUsedColor (), Inst1ToPlot, Inst1ColorButton);
    }



    private  void  PromptInstrumentDataCorlor (ComboBox  instrComboBox,
                                               Button    instrColorButton
                                              )
    {
      ColorDialog cd = new ColorDialog ();
      cd.Color = instrComboBox.BackColor;
      cd.AllowFullOpen = false;
      cd.AnyColor = true;
      DialogResult dr = cd.ShowDialog (instrColorButton);
      if  (dr == DialogResult.OK)
        SetInstrumentColor (cd.Color, instrComboBox, instrColorButton);
    }



    private void Inst1ColorButton_Click (object sender, EventArgs e)
    {
      PromptInstrumentDataCorlor (Inst1ToPlot, Inst1ColorButton);
    }


    private  void  CopyChartToClipboard (Object sender, EventArgs e)
    {
      using (System.IO.MemoryStream ms = new System.IO.MemoryStream ())
      {
        ProfileChart.SaveImage (ms, ChartImageFormat.Bmp);
        Bitmap bm = new Bitmap(ms);
        Clipboard.SetImage(bm);
      }
    }


    private  void  SaveChartToDisk (Object sender, EventArgs e)
    {
      SaveFileDialog sfd = new SaveFileDialog ();
      sfd.Filter = "JPeg Image|*.jpg|Bitmap Image|*.bmp|Gif Image|*.gif";
      sfd.Title = "Save chart as Image File";
      sfd.DefaultExt = "jpg";
      sfd.AddExtension = true;
      sfd.CheckFileExists = true;
      DialogResult dr = sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        String  fn = sfd.FileName;
        System.IO.FileInfo fi = new System.IO.FileInfo (fn);
        String ext = fi.Extension.ToLower ();

        ChartImageFormat cif = ChartImageFormat.Jpeg;
        if  (ext == "jpg")
          cif = ChartImageFormat.Jpeg;

        else if  (ext == "bmp")
          cif = ChartImageFormat.Bmp;

        else if  (ext == "gif")
          cif = ChartImageFormat.Gif;
        
        ProfileChart.SaveImage (fn, cif);
      }
    }



    private  void  SaveTabDelToDisk (String fileName)
    {
      System.IO.StreamWriter sw = null;
      try  {sw = new System.IO.StreamWriter (fileName, false, System.Text.Encoding.ASCII);}
      catch (Exception) {sw = null;}

      if  (sw == null)
      {
        MessageBox.Show (this, "Could not open file[" + fileName + "]", "Save Tab-Delimited", MessageBoxButtons.OK);
        return;
      }
      WriteTabDelToStream (sw);
      sw.Close ();
      sw = null;

      MessageBox.Show (this, "GPS Data written to \"" + fileName + "\".", "", MessageBoxButtons.OK);
    }


    private  void  SaveTabDelToClipBoard (Object sender, EventArgs e)
    {
      StringWriter  sw = new StringWriter ();
      WriteTabDelToStream (sw);
      Clipboard.SetText (sw.ToString ());
      sw.Close ();
      sw = null;
    }

    
    private  void  SaveTabDelToDisk (Object sender, EventArgs e)
    {
      SaveFileDialog sfd = new SaveFileDialog ();
      sfd.Filter = "Tab delimited data to disk|*.txt";
      sfd.Title = "Save info as tab-delimited data to disk.";
      sfd.DefaultExt = "txt";
      sfd.AddExtension = true;
      sfd.OverwritePrompt = true;

      String fn = cruise + "-" + station;
      if  (!String.IsNullOrEmpty (deployment))
        fn += ("-" + deployment);
      fn += "_HorizontalProfile";
      fn += ".txt";
      sfd.FileName = fn;

      //sfd.CheckFileExists = true;
      DialogResult dr = sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        fn = sfd.FileName;
        SaveTabDelToDisk (fn);
      }
    }

       
    private  void  WriteTabDelToStream (TextWriter tw)
    {
      DateTime  curTime = DateTime.Now;
      tw.WriteLine ("ChartHorizontalProfile"); 
      tw.WriteLine ("DateTime"  + "\t" + curTime.ToString ("yyyy/MM/dd HH:mm:ss")); 
      tw.WriteLine ();
      tw.WriteLine ("Cruise"     + "\t" + cruise);
      tw.WriteLine ("Station"    + "\t" + station);
      tw.WriteLine ("Deployment" + "\t" + deployment);
      tw.WriteLine ("Criteria"   + "\t" + criteriaStr);

      int  maxClassIdx = 0;
      foreach  (DataSeriesToPlot  dstp  in  series)
      {
        if  (dstp.sourceType == 'C')
        {
          maxClassIdx = Math.Max (maxClassIdx, dstp.data.Count);
          tw.WriteLine ("Class-To-Plot" + "\t" + dstp.legend);
        }
        else
        {
          tw.WriteLine ("Instrument"    + "\t" + dstp.legend);
        }
      }

      tw.WriteLine ("Weight-by-Volume"     + "\t" + (weightByVolume    ? "Yes" : "No"));
      tw.WriteLine ("Include-Sub-Classes"  + "\t" + (includeSubClasses ? "Yes" : "No"));
      tw.WriteLine ();
      tw.WriteLine ();
      tw.Write ("ScanLine" + "\t" + "CtdDateTime" + "\t" + "VolumeSampled");
      foreach  (DataSeriesToPlot  dstp  in  series)
      {
        if  (dstp.sourceType == 'C')
          tw.Write ("\t" + dstp.legend);
      }
      tw.WriteLine ();
      
      for  (int idx = 0;  idx < maxClassIdx;  ++idx)
      {
        int  scanLine = (int)(0.5f + (float)idx * scanLinesInterval);
        DateTime  ctdDateTime = initialStartTime;
        double  offsetSecs = idx * timeInterval;
        ctdDateTime = ctdDateTime.AddSeconds (offsetSecs);

        tw.Write (scanLine + "\t" + ctdDateTime.ToString ("yyyy-MM-dd hh:mm:ss") + "\t");
        if  ((timeVolumeProfile != null)  &&  (timeVolumeProfile.Count > idx))
          tw.Write (timeVolumeProfile[idx]);
        foreach  (DataSeriesToPlot  dstp  in  series)
        {
          if  (dstp.sourceType == 'C')
          {
            tw.Write ("\t");
            if  (dstp.data.Count > idx)
              tw.Write (dstp.data[idx].YValues[0]);
          }
        }
        tw.WriteLine ();
      }

      foreach  (DataSeriesToPlot  dstp  in  series)
      {
        if  (dstp.sourceType != 'C')
        {
          tw.WriteLine ();
          tw.WriteLine ();
          tw.WriteLine ("ScanLine" + "\t" + "CtdDateTime" + "\t" + dstp.legend);

          foreach  (DataPoint  dp in  dstp.data)
          {
            int  scanLine = (int)dp.XValue;
            DateTime  ctdDateTime = initialStartTime;
            double  deltaTime = scanLine / scanRate;
            ctdDateTime = ctdDateTime.AddSeconds (deltaTime);
            tw.WriteLine (scanLine.ToString () + "\t" + ctdDateTime.ToString ("yyyy-MM-dd hh:mm:ss") + "\t" + dp.YValues[0].ToString ());
          }
        }
      }
      
      tw.WriteLine ();
      tw.WriteLine ("End of ChartHorizontalProfile");
    }  /* WriteTabDelToStream */


 
    private void ChartHorizontalProfile_FormClosing (object sender, FormClosingEventArgs e)
    {
      if  (buildPlotDataRunning)
      {
        if (e.CloseReason == CloseReason.WindowsShutDown)
        {
          CancelBackGroundThread ();
          while  (buildPlotDataRunning)
          {
            Thread.Sleep (100);
          }
        }
        else
        {
          MessageBox.Show (this, "Plot data is being collected; press \"Cancel Button\" to Stop before exiting.", "Form Closing", MessageBoxButtons.OK);
          e.Cancel = true;
          return;
        }
      }

      SaveConfiguration ();
      
      mainWinConn.Close ();
      mainWinConn = null;
      GC.Collect ();
    }



    private void CancelPlotButton_Click (object sender, EventArgs e)
    {
      if  (buildPlotDataRunning)
        CancelBackGroundThread ();
    }
  }
}
