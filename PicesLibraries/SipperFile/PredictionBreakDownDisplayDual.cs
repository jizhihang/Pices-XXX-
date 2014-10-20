using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.IO;

using PicesInterface;
using SipperFile;


namespace SipperFile
{
  public partial class PredictionBreakDownDisplayDual : Form
  {
    String               fileName        = null;
    PicesFeatureVector   featureVector   = null;
    PicesFeatureVector   dbFeatureVector = null;
    PicesInstrumentData  instrumentData  = null;
    int                  lastHeight      = 0;
    int                  lastWidth       = 0;
    Font                 probFont        = new Font (FontFamily.GenericMonospace, 10, FontStyle.Regular); 
    PicesRunLog          runLog          = null;
    
    TrainingModel2       trainingModel      = null;


    public  PredictionBreakDownDisplayDual (PicesDataBase       _dbConn,
                                            String              _fileName,
                                            TrainingModel2      _trainingModel,
                                            PicesInstrumentData _instrumentData,  /**<  Instrument data that goes with image,  if null will read from dtabase. */
                                            PicesRunLog         _runLog
                                           )
    {
      if  (_runLog == null)
        runLog = new PicesRunLog ();
      else
        runLog = _runLog;

      fileName       = _fileName;
      instrumentData = _instrumentData;
      trainingModel  = _trainingModel;

      PicesDataBase  dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      PicesRaster  raster = dbConn.ImageFullSizeFind (fileName);
      if  (raster == null)
        return;

      if  (_dbConn != null)
      {
        dbFeatureVector = _dbConn.FeatureDataRecLoad (_fileName);
        if  (instrumentData == null)
        {
          PicesDataBaseImage  image = dbConn.ImageLoad (fileName);
          if  (image != null)
            instrumentData = dbConn.InstrumentDataGetByScanLine (image.SipperFileName, image.TopLeftRow);
        }
      }

      featureVector = new PicesFeatureVector (raster, fileName, null, runLog);
     
      if  (instrumentData != null)
        featureVector.AddInstrumentData (instrumentData);

      dbConn.Close ();
      dbConn = null;
      GC.Collect ();

      InitializeComponent ();
    }



    public PredictionBreakDownDisplayDual (PicesDataBase       _dbConn,
                                           PicesDataBaseImage  _image,
                                           PicesRaster         _raster,
                                           TrainingModel2      _trainingModel,
                                           PicesInstrumentData _instrumentData,  /**<  Instrument data that goes with image,  if null will read from dtabase. */
                                           PicesRunLog         _runLog
                                          )
    {
      if  (_runLog == null)
        runLog = new PicesRunLog ();
      else
        runLog = _runLog;
        
      if  (_image == null)
        return;

      fileName       = _image.ImageFileName;
      instrumentData = _instrumentData;
      trainingModel  = _trainingModel;

      PicesRaster raster = _raster;
      if  (_dbConn != null)
      {
        if  (raster == null)
         raster = _dbConn.ImageFullSizeFind (fileName);

        if  (instrumentData == null)
          instrumentData = _dbConn .InstrumentDataGetByScanLine (_image.SipperFileName, _image.TopLeftRow);

        dbFeatureVector = _dbConn.FeatureDataRecLoad (fileName);
      }

      if  (raster == null)
        return;

      //raster = raster.ConnectedComponent ();

      featureVector = new PicesFeatureVector (raster, fileName, null, runLog);
      if  (instrumentData != null)
        featureVector.AddInstrumentData (instrumentData);

      InitializeComponent ();
    }




    private void ImageListDisplay_Load (object sender, EventArgs e)
    {
      lastHeight = Height;
      lastWidth  = Width;

      UpdateDetailProbabilityDisplay ();
    }  /* LoadImageGrid */



    private  void  UpdateDetailProbabilityDisplay ()
    {
      if  ((trainingModel == null)  ||  (trainingModel.ModelType != TrainingModel2.ModelTypes.Dual))
        return;
    
      PicesPredictionList  class1Results = null;
      PicesPredictionList  class2Results = null;

      String  classifier1Desc = null;
      String  classifier2Desc = null;


      PicesFeatureVector  fvToUse = dbFeatureVector;
      if  (fvToUse == null)
        fvToUse = featureVector;

      trainingModel.ProbabilitiesByClassDual (fvToUse, 
                                              ref classifier1Desc,
                                              ref classifier2Desc,
                                              ref class1Results, 
                                              ref class2Results
                                             );

      this.Classifier1Name.Text = classifier1Desc;
      this.Classifier2Name.Text = classifier2Desc;

      if  (class1Results != null)
      {
        Classifier1Probabilities.Rows.Clear ();

        foreach  (PicesPrediction p in class1Results)
        {
          Object[]  data = new Object[3];
          data[0] = p.ClassName;
          data[1] = p.Probability;
          data[2] = p.Votes;
          Classifier1Probabilities.Rows.Add (data);
        }
      }

      if  (class2Results != null)
      {
        Classifier2Probabilities.Rows.Clear ();

        foreach  (PicesPrediction p in class2Results)
        {
          Object[]  data = new Object[3];
          data[0] = p.ClassName;
          data[1] = p.Probability;
          data[2] = p.Votes;
          Classifier2Probabilities.Rows.Add (data);
        }
      }
    }  /* UpdateDetailProbabilityDisplay */




    private void  DisplayTrainingImages (PicesClass  mlClass)
    {
      if  (trainingModel== null)
        return;
      
      String  dir = trainingModel.DirectoryPathForClass (mlClass);
      if  (String.IsNullOrEmpty (dir))
        return;

      DisplayPicesImages  dpi = new DisplayPicesImages (dir);
      dpi.ShowDialog ();
      dpi.Dispose ();
      dpi = null;
    }  /* DisplayTrainingImages */



 
    private void ClassificationProbabilities_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
    {
      DataGridView  dgv = (DataGridView)sender;

      if  (e.Value == null)
        return;

      if  (e.ColumnIndex == 1)
      {
        e.CellStyle.Font      = probFont;
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        e.Value = ((double)e.Value).ToString ("P2").PadLeft (8);
        e.FormattingApplied = true;
      }

      else if  (e.ColumnIndex == 2)
      {
        e.CellStyle.Font      = probFont;
        e.CellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
        e.Value = ((int)e.Value).ToString ("#,##0");
        e.FormattingApplied = true;
      }
    }


    private void ClassificationProbabilities_MouseDoubleClick (object sender, MouseEventArgs e)
    {
      if  (e.Button == MouseButtons.Right)
      {
        DataGridView  dgv = (DataGridView)sender;

        DataGridViewSelectedCellCollection dgvscc = dgv.SelectedCells;
        if  ((dgvscc == null)  ||  (dgvscc.Count != 1))
          return;

        String  className = (String)dgvscc[0].Value;
        PicesClass  pc = PicesClassList.GetUniqueClass (className, "");
        if  (pc != null)
          DisplayTrainingImages (pc);
      }
    }



    private void DisplayFeatureDataButton_Click (object sender, EventArgs e)
    {
      FeatureDataDisplay  fdd = new FeatureDataDisplay (fileName, this.featureVector, dbFeatureVector);
      fdd.ShowDialog (this);
    }  /* DisplayWorstTrainingImages2 */
  }
}
