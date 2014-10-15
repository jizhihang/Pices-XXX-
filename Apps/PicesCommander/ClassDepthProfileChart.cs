using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;
using System.Threading;

using SipperFile;
using PicesInterface;

namespace PicesCommander
{
  public partial class ClassDepthProfileChart : Form
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

    private  float   depthIncrement = 1.0f;

    private  String  infinityStr = ((char)8734).ToString ();
    
    private  PicesDataBase   mainWinConn = null;
    private  PicesDataBase   threadConn  = null;  /**< This connection will only be use by the background thread. */

    private  PicesMsgQueue   statusMsgs = null;
    private  PicesMsgQueue   msgQueue   = null;
    private  PicesRunLog     runLog     = null;

    private  float[]    depthVolumeProfile = null;

    private  PicesGoalKeeper  goalie = new PicesGoalKeeper ("ClassDepthProfileChart");

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
                                float            _minVal,
                                String           _formatStr,
                                String[]         _axisLabels,
                                SeriesChartType  _chartType,
                                Color            _color
                               ) 
      {
        sourceType    = _sourceType;
        unitOfMeasure = _unitOfMeasure;
        legend        = _legend;
        data          = _data;
        minVal        = _minVal;
        formatStr     = _formatStr;
        axisLabels    = _axisLabels;
        chartType     = _chartType;
        color         = _color;
      }

      public  char             sourceType;
      public  String           unitOfMeasure;
      public  String           legend;
      public  List<DataPoint>  data;
      public  float            minVal;
      public  String           formatStr;
      public  String[]         axisLabels;
      public  SeriesChartType  chartType;
      public  Color            color;
    }

    
    private  List<DataSeriesToPlot>  series = new List<DataSeriesToPlot> ();




    public ClassDepthProfileChart (String          _cruise,
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

      configFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "ClassDepthProfileChart.cfg";

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
      if  ((depthMax > 0.0f)  &&  (depthMax < 100000.0))
        depthMaxStr = depthMax.ToString ("#,##0");

      if  (filterSize)
        AddToStr (ref criteriaStr, "size(" + sizeMin + ", " + sizeMaxStr + ")");

      if  (filterProb)
        AddToStr (ref criteriaStr, "prob(" + probMin.ToString ("p2") + ", " + probMax.ToString ("p2") + ")");

      if  (filterDepth)
        AddToStr (ref criteriaStr, "Depth(" + depthMin + ", " + depthMaxStr + ")");

      if  (classKeyToUse == 'V')
        AddToStr (ref criteriaStr, "Validated");
    }


    private  void  PopulateMinMaxFields ()
    {
      if  ((sizeMin > 0)  ||  (sizeMax > 0))
      {
        string   m;
        if  (sizeMin <= 0)
          m = "0";
        else
          m = sizeMin.ToString ();
        m += " - ";

        if  ((sizeMax <= 0)  ||  (sizeMax == int.MaxValue))
          m += infinityStr;
        else
          m += sizeMax.ToString ();

        SizeRange.Text = m;
      }

      if  ((depthMin > 0)  ||  (depthMax > 0))
      {
        string   m;
        if  (depthMin <= 0)
          m = "0";
        else
          m = depthMin.ToString ();
        m += " - ";

        if  ((depthMax <= 0)  ||  (depthMax == float.MaxValue))
          m += infinityStr;
        else
          m += depthMax.ToString ();
        DepthRange.Text = m;
      }

      if  (filterProb)
      {
        string   m = probMin.ToString ("P") + " - " + probMax.ToString ("P");
        ProbRange.Text = m;
      }
    }  /* PopulateMinMaxFields */


   
    private void ClassDepthProfileChart_Load (object sender, EventArgs e)
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

      Cruise.Text     = cruise;
      Station.Text    = station;
      Deployment.Text = deployment;
      PopulateMinMaxFields ();

      ContextMenuStrip cms = new ContextMenuStrip ();
      cms.Items.Add ("Copy Chart to clipboard",      null, CopyChartToClipboard);
      cms.Items.Add ("Save Chart to Disk",           null, SaveChartToDisk);
      ProfileChart.ContextMenuStrip = cms;

      LoadConfigurationFile ();
      String  selInst1Name = (String)Inst1ToPlot.SelectedItem;
      if  (String.IsNullOrEmpty (selInst1Name))
      {
        Inst1ToPlot.SelectedItem = "Volume-Sampled";
        SetInstrumentColor (FirstUnUsedColor (), Inst1ToPlot, Inst1ColorButton);
      }

      PlotButton_Click (this, null);

      //ClassesToPlot.DefaultCellStyle.SelectionBackColor = Color.White;
      //ClassesToPlot.DefaultCellStyle.SelectionForeColor = Color.Black;

      //TrainingModel2  trainModel = ActiveTrainingLibraries.Model1 ();
    }


    private  void  DisableControls ()
    {
      ClassesToPlot.Enabled = false;
      AddClassButton.Enabled = false;
      IncludeSubClasses.Enabled = false;
      DepthIncr.Enabled = false;
      Inst1ToPlot.Enabled = false;
      Inst1ColorButton.Enabled = false;
      PlotButton.Enabled = false;
      DepthAxisAuto.Enabled = false;
      DepthAxisMin.Enabled = false;
      DepthAxisMax.Enabled = false;
      DepthAxisInterval.Enabled = false;
    }


    private  void  EnableControls ()
    {
      ClassesToPlot.Enabled = true;
      AddClassButton.Enabled = true;
      IncludeSubClasses.Enabled = true;
      DepthIncr.Enabled = true;
      Inst1ToPlot.Enabled = true;
      Inst1ColorButton.Enabled = true;
      PlotButton.Enabled = true;

      DepthAxisAuto.Enabled = true;
      if  (!DepthAxisAuto.Checked)
      {
        DepthAxisMin.Enabled = true;
        DepthAxisMax.Enabled = true;
        DepthAxisInterval.Enabled = true;
      }
    }


    private  void  SaveConfiguration ()
    {
      System.IO.StreamWriter  o = null;
      try{o = new System.IO.StreamWriter (configFileName);}  catch  (Exception){o = null; return;}
      
      o.WriteLine ("//ClassDepthProfileChart Configuration File");
      o.WriteLine ("//");
      o.WriteLine ("//DateTime Saved" + "\t" + DateTime.Now.ToString ("F"));
      o.WriteLine ("//");
      o.WriteLine ("WidthLast"          + "\t" + widthLast);
      o.WriteLine ("HeightLast"         + "\t" + heightLast);
      o.WriteLine ("Maximized"          + "\t" + (formIsMaximized ? "YES":"NO"));
      o.WriteLine ("IncludeSubClasses"  + "\t" + (IncludeSubClasses.Checked ? "Yes" : "No"));
      o.WriteLine ("WeightByVolume"     + "\t" + (WeightByVolume.Checked ? "Yes" : "No"));

      o.WriteLine ("DepthAxisAuto"      + "\t" + (DepthAxisAuto.Checked ? "Yes" : "No"));
      o.WriteLine ("DepthAxisMin"       + "\t" + DepthAxisMin.Value);
      o.WriteLine ("DepthAxisMax"       + "\t" + DepthAxisMax.Value);
      o.WriteLine ("DepthAxisInterval"  + "\t" + DepthAxisInterval.Value);

      String  selInst1Str = GetSelectedInstruemnt (Inst1ToPlot);
      if  (!String.IsNullOrEmpty (selInst1Str))
      {
        o.WriteLine ("Inst1ToPlot"        + "\t" + selInst1Str);
        o.WriteLine ("Inst1ColorButton"   + "\t" + Inst1ColorButton.BackColor.Name);
      }
      
      o.WriteLine ("DepthIncr"          + "\t" + DepthIncr.Value);

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

          case  "DepthIncr":
            DepthIncr.Value = (decimal)PicesKKStr.StrToFloat (fieldValue);
            break;

          case  "DepthAxisAuto":
            DepthAxisAuto.Checked = PicesKKStr.StrToBool (fieldValue);
            break;
          
          case  "DepthAxisMin":
            DepthAxisMin.Value = Math.Max (0, (decimal)PicesKKStr.StrToFloat (fieldValue));
            break;

          case  "DepthAxisMax":
            DepthAxisMax.Value = Math.Max (0, (decimal)PicesKKStr.StrToFloat (fieldValue));
            break;

          case  "DepthAxisInterval":
            DepthAxisInterval.Value = Math.Max (0, (decimal)PicesKKStr.StrToFloat (fieldValue));
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



    private  void   ValidateDepthAxisSettings (ref List<String>  errors)
    {
      if  (DepthAxisAuto.Checked)
      {
        if  (DepthAxisMin.Value >= DepthAxisMax.Value)
          errors.Add ("Min-Depth must be less than Max-Depth");

        if  (DepthAxisInterval.Value <= 0)
          errors.Add ("The increment must be greater than 0.");

        else
        {
          Decimal  range = (DepthAxisMax.Value - DepthAxisMin.Value);
          if  ((range > 0)  &&  (range < DepthAxisInterval.Value))
            errors.Add ("Interval size is less than specified range.");
        }
      }
      return;
    }



    private  List<String>  ValidateFields ()
    {
      List<String>  errors = new List<string> ();
      ValidateClassSelections (ref errors);
      ValidateDepthAxisSettings (ref errors);
      
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


    private  float[]  MergeTwoLists (float[]  l1, float[] l2)
    {
      if  (l1 == null)       return l2;
      else if  (l2 == null)  return l1;

      int  minLen = 0;

      float[]  from = null;
      float[]  to   = null;

      if  (l1.Length > l2.Length)
      {from = l2;   to = l1;  minLen = l2.Length;}
      else
      {from = l1;   to = l2;  minLen = l1.Length;}

      for  (int idx = 0;  idx < minLen;  ++idx)
        to[idx] += from[idx];

      return  to;
    }
    

    private  uint[]  MergeTwoLists (uint[]  l1, uint[] l2)
    {
      if  (l1 == null)       return l2;
      else if  (l2 == null)  return l1;

      int  minLen = 0;

      uint[]  from = null;
      uint[]  to   = null;

      if  (l1.Length > l2.Length)
      {from = l2;   to = l1;  minLen = l2.Length;}
      else
      {from = l1;   to = l2;  minLen = l1.Length;}

      for  (int idx = 0;  idx < minLen;  ++idx)
        to[idx] += from[idx];

      return  to;
    }
    


    
    private  float[]  CountsByDepth (PicesClass   c)
    {
      String msg = "Extracting Counts for \"" + c.Name + "\"";
      statusMsgs.AddMsg (msg);
      msgQueue.AddMsg (msg);

      uint[]  counts = null;

      PicesSipperFileList  sipperFiles = threadConn.SipperFileLoad (cruise, station, deployment);
      foreach  (PicesSipperFile sf in sipperFiles)
      {
        if  (cancelRequested)
          break;

        statusMsgs.AddMsg ("Retrieving counts for Class[" + c.Name + "]  SipperFile[" + sf.SipperFileName + "]");
    
        uint[]  countsThisSipperFile 
          = threadConn.ImageGetDepthStatistics (null,               // PicesDataBaseImageGroup^  imageGroup,
                                                sf.SipperFileName,
                                                depthIncrement,     // depthIncrements,
                                                c,                  // mlClass,
                                                classKeyToUse,      // classKeyToUse,
                                                probMin,  probMax,
                                                sizeMin,  sizeMax
                                               );

        counts = MergeTwoLists (counts, countsThisSipperFile);
      }

      if  (cancelRequested)
        return null;
      
      if  (counts == null)
      {
        counts = new uint[1];
        counts[0] = 0;
      }

      float[]  countsFloat = new float[counts.Length];
      for  (int idx = 0;  idx < counts.Length;  ++idx)
        countsFloat[idx] = counts[idx];

      List<PicesClass> children = c.Children;
      if  (children != null)
      {
        foreach  (PicesClass  pc in children)
        {
          if  (cancelRequested)
            break;

          float[] countsForChild = CountsByDepth (pc);
          if  (countsForChild != null)
            countsFloat = MergeTwoLists (countsFloat, countsForChild);
        }
      }
      return  countsFloat;
    }  /* CountsByDepth */



    private  void  GetDepthVolumeProfile  ()
    {
      msgQueue.AddMsg ("Retrieving Depth-Volume profile");
      statusMsgs.AddMsg ("Retrieving Depth-Volume profile");
      PicesVolumeSampledStatList 
        volStats = threadConn.InstrumentDataGetVolumePerMeterDepth (cruise, station, deployment, depthIncrement);
      if  (volStats == null)
        return;

      depthVolumeProfile = volStats.ToArray ();
    }



    private  void  AddClassToSeries (PicesClass       pc,
                                     Color            color,
                                     SeriesChartType  charType
                                    )
    {
      String  unitOfMeasure = "Count";
      if  (WeightByVolume.Checked)
        unitOfMeasure = "Count/m-3";

      float[] counts = CountsByDepth (pc);
      if  ((counts == null)  ||  cancelRequested)
        return;

      if  (WeightByVolume.Checked)
      {
        unitOfMeasure = "Count/m-3";
        for  (int idx = 0;  idx < counts.Length;  ++idx)
        {
          float vol = 0;
          if  (idx < depthVolumeProfile.Length)
            vol = depthVolumeProfile[idx];

          if  (vol == 0.0f)
            counts[idx] = 0.0f;
          else
            counts[idx] = counts[idx] / vol;
        }
      }

      String[]  axisLabels = new String[counts.Length];
      List<DataPoint>  data = new List<DataPoint> ();
      for  (int idx = 0;  idx < axisLabels.Length;  ++idx)
      {
        axisLabels[idx] = ((float)idx * depthIncrement).ToString ("##0.0");
        data.Add (new DataPoint (counts[idx], idx * depthIncrement));
      }

      goalie.StartBlock ();

      series.Add (new DataSeriesToPlot ('C', unitOfMeasure, pc.Name, data, 0, "###,##0", axisLabels, charType, color));
      goalie.EndBlock ();
    }  /* AddClassToSeries */



    private  void  AddVolumeSampled (Color            color,
                                     SeriesChartType  chartType
                                    )
    {
      String  unitOfMeasure = "m-3";

      if  (depthVolumeProfile == null)
        GetDepthVolumeProfile  ();

      String[]  axisLabels = new String[depthVolumeProfile.Length];
      List<DataPoint>  data = new List<DataPoint> ();
      for  (int idx = 0;  (idx < axisLabels.Length)  &&  (!cancelRequested);  ++idx)
      {
        axisLabels[idx] = ((float)idx * depthIncrement).ToString ("##0.0");
        data.Add (new DataPoint (depthVolumeProfile[idx], idx * depthIncrement));
      }

      goalie.StartBlock ();
      series.Add (new DataSeriesToPlot ('I', unitOfMeasure, "Volume Sampled", data, 0, "#,##0.0", axisLabels, chartType, color));
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
      fieldNames.Add ("Depth");
      fieldNames.Add (instName);

      float[,] instrumentData 
        = threadConn.InstrumentDataGetSpecificFields (cruise, station, deployment,
                                                      fieldNames,
                                                      DateTime.MinValue, DateTime.MaxValue
                                                     );

      if  (instrumentData == null)
      {
        String  msg = "Instrument[" + instName + "] did not return any data.";
        statusMsgs.AddMsg (msg);
        msgQueue.AddMsg (msg);
        return; 
      }

      String  unitOfMeasure = PicesInstrumentData.FieldUnitOfMeasure (fieldNum);
      String  formatStr     = PicesInstrumentData.FieldFormatStr     (fieldNum);
      float   minVal        = PicesInstrumentData.FieldMinValue      (fieldNum);
      int  numRows = instrumentData.GetLength (0);

      String[]  axisLabels = new String[numRows];
      List<DataPoint>  data = new List<DataPoint> ();
      for  (int idx = 0;  ((idx < numRows)  &&  (!cancelRequested));  ++idx)
      {
        float  depth = instrumentData[idx, 0];
        float  value = instrumentData[idx, 1];
        if  (value < minVal)
          continue;
        axisLabels[idx] = depth.ToString ("##0.0");
        data.Add (new DataPoint (value, depth));
      }

      goalie.StartBlock ();
      series.Add (new DataSeriesToPlot ('I', unitOfMeasure, instName, data, minVal, formatStr, axisLabels, chartType, color));
      goalie.EndBlock ();
    }  /* AddInstrumentData */



    /// <summary>
    /// This next methid will be ran as a separate thread; it is respnable for collecting all the data needed to generate the plot.
    /// </summary>
    private  void  BuildPlotData ()
    {
      if  (buildPlotDataRunning)
        return;

      PicesDataBase.ThreadInit ();

      threadConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      buildPlotDataRunning = true;

      depthIncrement = (float)DepthIncr.Value;

      if  (WeightByVolume.Checked)
        GetDepthVolumeProfile ();

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


    private int  FindBestInetrvaleSize (int minSize, int maxSize, int range)
    {
      int  smallestInterval = range % maxSize;
      int  intervalThatIsSmallest = maxSize;

      int zed = maxSize - 1;
      while  (zed >= minSize)
      {
        int  mod = range % zed;
        if  (mod <= smallestInterval)
        {
          smallestInterval = mod;
          intervalThatIsSmallest = zed;
        }
        --zed;
      }
      return  intervalThatIsSmallest;
    }


    private  void  DetermineDepthMinMaxToPlot (ref float   depthMinToPlot,
                                               ref float   depthMaxToPlot,
                                               ref float   numIntervals,
                                               ref float   intervalSize,
                                               ref string  depthFormatStr
                                              )
    {
      float  min = float.MaxValue;
      float  max = float.MinValue;
      numIntervals = 10.0f;
      intervalSize = 10.0f;

      foreach  (DataSeriesToPlot dstp in series)
      {
        foreach  (DataPoint dp in dstp.data)
        {
          if  (dp.XValue == 0.0)
            continue;
          if  (dp.YValues[0] < min)
            min = (float)dp.YValues[0];

          if  (dp.YValues[0] > max)
            max = (float)dp.YValues[0];
        }
      }

      float  depthDelta = max - min;
      float  depthRange = 2.0f;
      numIntervals = 0.25f;
      depthFormatStr = "##,##0";
      intervalSize = -1.0f;


      if  (depthDelta <= 2.0f)
      {
        depthRange = 2.0f;
        intervalSize = 0.25f;
        depthFormatStr = "#,##0.00";
      }

      else if  (depthDelta <= 5.0f)
      {
        depthRange = (float)Math.Ceiling (depthDelta);
        intervalSize = 0.5f;
        depthFormatStr = "#,##0.0";
      }

      else if  (depthDelta <= 10.0f)
      {
        depthRange = (float)Math.Ceiling (depthDelta);
        intervalSize = FindBestInetrvaleSize (1, 3, (int)depthRange);
      }

      else if  (depthDelta < 15.0f)
      {
        depthRange = 1.0f * (float)Math.Ceiling ((depthDelta / 1.0f));
        intervalSize = FindBestInetrvaleSize (2, 4, (int)depthRange);
      }

      else if  (depthDelta < 20.0f)
      {
        depthRange = 2.0f * (float)Math.Ceiling ((depthDelta / 2.0f));
        intervalSize = FindBestInetrvaleSize (2, 5, (int)depthRange);
      }

      else if  (depthDelta < 50.0f)
      {
        depthRange = 5.0f * (float)Math.Ceiling ((depthDelta / 5.0f));
        intervalSize = FindBestInetrvaleSize (5, 10, (int)depthRange);
      }

      else if  (depthDelta < 75.0f)
      {
        depthRange = 5.0f * (float)Math.Ceiling ((depthDelta / 5.0f));
        intervalSize = FindBestInetrvaleSize (10, 15, (int)depthRange);
      }

      else if  (depthDelta < 100.0f)
        depthRange = 10.0f * (float)Math.Ceiling ((depthDelta / 10.0f));

      else if  (depthDelta <= 200f)
        depthRange = 20.0f * (float)Math.Ceiling ((depthDelta / 20.0f));

      else if  (depthDelta <= 300.0f)
        depthRange = 30.0f * (float)Math.Ceiling ((depthDelta / 30.0f));

      else if  (depthDelta <= 400.0f)
        depthRange = 40.0f * (float)Math.Ceiling ((depthDelta / 30.0f));

      else
        depthRange = 100.0f * (float)Math.Floor (depthDelta / 100.0f);

      if  (intervalSize < 0.0f)
        intervalSize = depthRange / 10.0f;

      numIntervals = (int)(0.5f + depthRange / intervalSize);

      float incrPerMeter = 1.0f / intervalSize;
      depthMinToPlot = (float)Math.Floor (min * incrPerMeter) / incrPerMeter;
      depthMaxToPlot = depthMinToPlot + intervalSize * numIntervals;
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

      float   depthMinToPlot = 0.0f;
      float   depthMaxToPlot = 100.0f;
      float   numIntervals   = 10.0f;
      float   intervalSize   = 10.0f;
      String  depthFormatStr = "##,##0";

      DetermineDepthMinMaxToPlot (ref depthMinToPlot,
                                  ref depthMaxToPlot,
                                  ref numIntervals,
                                  ref intervalSize,
                                  ref depthFormatStr
                                 );
      if  (!DepthAxisAuto.Checked)
      {
        depthMinToPlot = (float)DepthAxisMin.Value;
        depthMaxToPlot = (float)DepthAxisMax.Value;
        intervalSize   = (float)DepthAxisInterval.Value;
      }
      else
      {
        DepthAxisInterval.Value = (Decimal)intervalSize;
        DepthAxisMin.Value = (Decimal)depthMinToPlot;
        DepthAxisMax.Value = (Decimal)depthMaxToPlot;
      }

      ChartArea ca = ProfileChart.ChartAreas[0];
      ca.AxisY.IsReversed = true;
      ca.AxisY.Minimum = 0;
      ca.AxisY.Title = "Depth(m)";
      //ca.AxisY.Title = "Depth(" + depthMinToPlot + " - " + depthMaxToPlot + ")";

      ca.AxisX.Minimum = 0;
      ca.AxisX.Title = "Count/m-3";
      ca.AxisX2.MajorGrid.Enabled = false;

      ca.AxisY.IsReversed = true;
      ca.AxisY.Maximum = depthMaxToPlot;
      ca.AxisY.Minimum = depthMinToPlot;
      ca.AxisY.Interval = intervalSize;
      ca.AxisY.LabelStyle.Format = depthFormatStr;

      Decimal  distFromFirstInterval = (Decimal)depthMinToPlot % (Decimal)intervalSize;
      double  intervalOffSet = (double)((Decimal)intervalSize - distFromFirstInterval);
      if  (intervalOffSet == intervalSize)
        intervalOffSet = 0;

      ca.AxisY.IntervalOffset = intervalOffSet;
      ca.AxisY.MajorTickMark.Interval = intervalSize;
      ca.AxisY.MajorTickMark.IntervalOffset = intervalOffSet;

      float   intervalMinorSize = intervalSize / 2.0f;
      Decimal  distFromFirstIntervalMinor = (Decimal)depthMinToPlot % (Decimal)intervalMinorSize;
      double  intervalMinorOffSet = (double)((Decimal)intervalMinorSize - distFromFirstInterval);
      if  (intervalMinorOffSet == intervalMinorSize)
        intervalMinorOffSet = 0.0;

      ca.AxisY.MinorTickMark.Interval = intervalMinorSize;
      ca.AxisY.MinorTickMark.IntervalOffset = intervalMinorOffSet;

      ProfileChart.Series.Clear ();

      DataSeriesToPlot  instSeriesToPlot = null;

      // First we will plot class counts on CharArea1
      foreach  (DataSeriesToPlot dstp in series)
      {
        if  (dstp.sourceType != 'C')
        {
          instSeriesToPlot = dstp;
          continue;
        }

        float  minVal = dstp.minVal;
        Series s = new Series (dstp.legend.Replace ('_', '\n'));
        s.ChartArea = "ChartArea1";
        s.ChartType = dstp.chartType;
        s.Name = dstp.legend;

        ca.AxisX.Title = dstp.unitOfMeasure;
        ca.AxisX.LabelStyle.Format =  dstp.formatStr;
        ca.AxisX.Minimum = 0;
        s.XAxisType = AxisType.Primary;
 
        s.Color = dstp.color;
        s.BorderWidth = 2;

        foreach  (DataPoint dp in dstp.data)
        {
          if  (dp.XValue > minVal)
          {
            float  yValue = (float)dp.YValues[0];
            if  ((yValue >= depthMinToPlot)  &&  (yValue <= depthMaxToPlot))
              s.Points.Add (dp);
          }
        }

        ProfileChart.Series.Add (s);
      }

      if  (instSeriesToPlot != null)
      {
        float  minVal = instSeriesToPlot.minVal;
        Series s = new Series (instSeriesToPlot.legend);
        s.ChartArea = "ChartArea1";

        s.ChartType = instSeriesToPlot.chartType;
        s.Name = instSeriesToPlot.legend;
        s.BorderWidth = 2;

        ca.AxisX2.Title = instSeriesToPlot.unitOfMeasure;
        ca.AxisX2.LabelStyle.Format = instSeriesToPlot.formatStr;
        //ca.AxisX2.Minimum = dstp.minVal;
        ca.AxisX2.LabelStyle.Format = instSeriesToPlot.formatStr;
        s.XAxisType = AxisType.Secondary;

        s.Color = instSeriesToPlot.color;

        foreach  (DataPoint dp in instSeriesToPlot.data)
        {
          if  (dp.XValue > minVal)
          {
            float  yValue = (float)dp.YValues[0];
            if  ((yValue >= depthMinToPlot)  &&  (yValue <= depthMaxToPlot))
              s.Points.Add (dp);
          }
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

        plotRequests.Add (new PlotRequest ('C', cn, dgvr.DefaultCellStyle.BackColor, SeriesChartType.Line));
      }

      AddInstrumentToPlotRequest (Inst1ToPlot, Inst1ColorButton);
    }  /* SchedulePlotRequests */



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
        Status.Text = msg;
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


    private void ClassDepthProfileChart_Resize (object sender, EventArgs e)
    {
      int  heightDelta = Height - heightLast;
      int  widthDelta  = Width  - widthLast;

      ProfileChart.Height += heightDelta;
      ProfileChart.Width  += widthDelta;

      heightLast = Height;
      widthLast  = Width;
    }



    private void ClassDepthProfileChart_SizeChanged (object sender, EventArgs e)
    {
      if  (WindowState == FormWindowState.Maximized)
      {
        // Looks like user has pressed the Maximized button.  We have to trap it here because
        // the ResizeEnd envent does not trap when form is Maximized.
        //PicesCommanderFormResized ();
        ClassDepthProfileChart_Resize (sender, e);
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
          ClassDepthProfileChart_Resize (sender, e);
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

    private void ClassDepthProfileChart_FormClosing (object sender, FormClosingEventArgs e)
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


    private void DepthAxisAuto_CheckedChanged (object sender, EventArgs e)
    {
      if  (DepthAxisAuto.Checked)
      {
        DepthAxisMin.Enabled = false;
        DepthAxisMax.Enabled = false;
        DepthAxisInterval.Enabled = false;
      }
      else
      {
        DepthAxisMin.Enabled = true;
        DepthAxisMax.Enabled = true;
        DepthAxisInterval.Enabled = true;
      }
    }
  }
}