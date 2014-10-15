using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using PicesInterface;



namespace GradeTrainingModel
{
  public partial class GetRunTimeParameters : Form
  {
    private  String                      configFileName     = null;
    private  bool                        crossValidation    = false;
    private  PicesTrainingConfiguration  curSelModel        = null;
    private  TrainingModel2              trainingModel      = null;
    private  PicesFeatureVectorList      groundTruth        = null;
    private  PicesClassList              origClassifierClasses = null;

    private  PicesClass                  otherClass         = null;   /**< Will specify the Other clause from the configuration file when a dual c;lassifier is utilized. */
    private  PicesClass                  noAgreementClass   = null;

    private  uint                        trainingLevel      = 1;
    private  String                      trainingModelName  = "";
    private  bool                        trainingModelValid = false;
    private  uint                        predictionLevel    = 1;
    private  uint                        maxLevel           = 1;  // This is highest Hierarchial Level supported by the configuration file.

    private  int                         numOfFolds         = 10;

    private  PicesMsgQueue               msgQueue = null;
    private  PicesRunLog                 runLog = null;

    private  bool                        cancelProcessing = false;

    private  Thread                      loadingThread = null;
    private  bool                        loadingThreadRunning = false;

    private  ConfusionMatrix             confusionMatrix = null;
    private  bool                        displayConfusionMatrix = false;

    private  int                         numGroundTruthImages = 0;
    private  int                         numGroundTruthImagesProcessed = 0;

    private  String                      blockFlag = "BlockFlag";
    private  bool                        blocked = false;



    public GetRunTimeParameters()
    {
      msgQueue = new PicesMsgQueue ("GradeTrainingModel");
      runLog = new PicesRunLog (msgQueue);

      InitializeComponent();
      noAgreementClass = PicesClassList.GetUniqueClass ("NoAgreement", "");
      
      configFileName = OSservices.AddSlash (PicesSipperVariables.PicesConfigurationDirectory ()) + "GradeTrainingModel.txt";
      
      LoadTrainingModelComboBox ();
      InitializeTrainAndPredLevelFields ();
      EnableComponentsForParameterEntry ();
    }


    private  void  BlockStart ()
    {
      bool  blockEstablished = false;
      while  (!blockEstablished)
      {
        lock (blockFlag)
        {
          if  (!blocked)
          {
            blockEstablished = true;
            blocked = true;
          }
        }

        if  (!blockEstablished)
          Thread.Sleep (100);
      }
    }  /* BlockStart */



    private  void  BlockEnd ()
    {
      lock  (blockFlag)
      {
        blocked = false;
      }
    }  /* BlockEnd */



    private  void  CleanUpMemory ()
    {
      if  (loadingThreadRunning)
        CancelProcessing ();

      if  (confusionMatrix != null)
      {
        confusionMatrix.Close ();
        confusionMatrix = null;
      }

      if  (trainingModel != null)
      {
        BlockStart ();
        trainingModel = null;
        BlockEnd ();
      }
    }  /* CleanUpMemory */



    
    private  void  EnableComponentsForParameterEntry ()
    {
      TrainingModelSelected.Enabled = true;
      CrossValidation.Enabled       = true;
      GroundTruth.Enabled           = !crossValidation;
      GroundTruthButton.Enabled     = true;
      TrainingLevel.Enabled         = true;
      PredictionLevel.Enabled       = true;
      StartButon.Enabled            = true;
      CancelButon.Enabled           = false;
      RunLogOutPut.Enabled          = true;
      timer1.Enabled                = true;
    }   


    private  void  EnableComponentsForProcessing ()
    {
      TrainingModelSelected.Enabled = false;
      CrossValidation.Enabled       = false;
      GroundTruth.Enabled           = false;
      GroundTruthButton.Enabled     = false;
      TrainingLevel.Enabled         = false;
      PredictionLevel.Enabled       = false;
      StartButon.Enabled            = false;
      CancelButon.Enabled           = true;
      RunLogOutPut.Enabled          = true;
      timer1.Enabled                = true;
    }   


    private  void  LoadConfiguraionData ()
    {
      StreamReader f = null;

      try
      {
        f = new StreamReader (configFileName);
      }
      catch (Exception)
      {
        return;
      }

      string txtLine = f.ReadLine ();
      while (txtLine != null)
      {
        if (txtLine.Substring (0, 2) != "//")
        {
          String[]  fields = txtLine.Split ('\t');
          if  (fields.Length > 1)
          {
            string   rowType = fields[0].ToLower ().Trim ();
          

            if  (rowType == "crossvalidation")
            {
              crossValidation = ((fields[1] == "Y")  ||  (fields[1] == "T"));
              CrossValidation.Checked = crossValidation;
            }

            
            if  (rowType == "trainingmodelselected")
            {
              if  (fields.Length >= 2)
              {
                TrainingModelSelected.Text = fields[1];
                trainingModelName  = fields[1];
              }
            }

            else if  (rowType == "groundtruth")
            {
              if  (fields.Length >= 2)
              {
                GroundTruth.Text = fields[1];
              }
            }

            else if  (rowType == "traininglevel")
            {
              if  (fields.Length >= 2)
              {
                trainingLevel = PicesKKStr.StrToUint (fields[1]);
                TrainingLevel.Text = trainingLevel.ToString ();
              }
            }

            else if  (rowType == "predictionlevel")
            {
              if  (fields.Length >= 2)
              {
                predictionLevel = PicesKKStr.StrToUint (fields[1]);
                PredictionLevel.Text = trainingLevel.ToString ();
              }
            }

            else if  (rowType == "endoffile")
              break;
          }
        }
        
        txtLine = f.ReadLine();
      }

      ValidateTrainingModel ();
      InitializeTrainAndPredLevelFields ();

      f.Close();
    }  /* LoadConfiguraionData */
    

    
    private  void  SaveConfiguration ()
    {
      StreamWriter w = null;
      try
      {
        w = new StreamWriter (configFileName);
      }
      catch (Exception e)
      {
        MessageBox.Show ("Exception Writing Configuration File.  [" + e.ToString() + "]");
        return;
      }

      w.WriteLine ("// Grade Training Mode Configuration");
      w.WriteLine ("// DateTime" + "\t" + DateTime.Now.ToString());
      w.WriteLine ("//");
      w.WriteLine ("//");
      w.WriteLine ("TrainingModelSelected"   + "\t" + TrainingModelSelected.Text);
      w.WriteLine ("CrossValidation"         + "\t" + (crossValidation ? "Y" : "N"));
      w.WriteLine ("GroundTruth"             + "\t" + GroundTruth.Text);
      w.WriteLine ("TrainingLevel"           + "\t" + trainingLevel.ToString ());
      w.WriteLine ("PredictionLevel"         + "\t" + predictionLevel.ToString ());
      w.WriteLine ("EndOfFile");

      w.Close();
      
      return;

    }  /* SaveConfiguration */


    private  bool AreParametersValid ()
    {
      ValidateTrainingModel ();
      if  (!trainingModelValid)
        return  false;
      
      trainingLevel   = PicesKKStr.StrToUint (TrainingLevel.Text);
      predictionLevel = PicesKKStr.StrToUint (PredictionLevel.Text);

      if  ((trainingLevel < 1)  ||  (trainingLevel > maxLevel))
      {
        MessageBox.Show ("Invalid Training Level Specified, must be between[1 - " + maxLevel.ToString () + "]");
        return false;
      }

      if  ((predictionLevel < 1)  ||  (predictionLevel > trainingLevel))
      {
        MessageBox.Show ("Invalid Prediction Level Specified, must be between[1 - " + trainingLevel.ToString () + "]");
        return false;
      }

      if  (!Directory.Exists (GroundTruth.Text))
      {
        MessageBox.Show ("Ground Truth Directory is not valid");
        return false;
      }

      return  true;
    }  /* AreParametersValid */



    private  void  LoadTrainingModelComboBox ()
    {
      String[]  configFileNames = 
          PicesInterface.TrainingModel2.GetListOfTrainingModels ();

      for  (int x = 0;  x < configFileNames.Length;  x++)
        TrainingModelSelected.Items.Add (configFileNames[x]);
    }



    private  void  InitializeTrainAndPredLevelFields ()
    {
      TrainingLevel.Items.Clear ();
      PredictionLevel.Items.Clear ();

      if  (curSelModel == null)
      {
        TrainingLevel.Items.Add ("1");
        PredictionLevel.Items.Add ("1");
        maxLevel        = 1;
        trainingLevel   = 1;
        predictionLevel = 1;
      }
      else
      {
        maxLevel = curSelModel.NumHierarchialLevels ();

        if  (trainingLevel > maxLevel)
          trainingLevel = maxLevel;

        if  (predictionLevel > trainingLevel)
          predictionLevel = trainingLevel;

        for  (uint x = 1;  x <= maxLevel;  x++)
        {
          TrainingLevel.Items.Add (x.ToString ());
          PredictionLevel.Items.Add (x.ToString ());
        }
      }

      TrainingLevel.Text   = trainingLevel.ToString ();
      PredictionLevel.Text = predictionLevel.ToString ();
    }  /* InitializeTrainAndPredLevelFields */
    



    private  void  ValidateTrainingModel ()
    {
      if  (string.IsNullOrEmpty (TrainingModelSelected.Text))
        return;

      trainingModelName = TrainingModelSelected.Text;
      curSelModel       = new PicesTrainingConfiguration (trainingModelName, runLog);
      trainingModelValid = curSelModel.Valid ();
      if  (!trainingModelValid)
      {
        String  alarmMsg = "Error Loading Configuration File";
        alarmMsg += "\n\n";
        alarmMsg += curSelModel.LoadLogStream ();

        MessageBox.Show (this, alarmMsg, "Could not load Config file",  MessageBoxButtons.OK);
      }
      else
      {
        otherClass = curSelModel.OtherClass ();
        if  (crossValidation)
          GroundTruth.Text = curSelModel.RootDir ();

        origClassifierClasses = curSelModel.ExtractClassList ();
        InitializeTrainAndPredLevelFields ();
      }
    }  /* ValidateTrainingModel */


    private  void  LoadTrainingModelAndGroundTruth ()
    {
      if  (!AreParametersValid ())
        return;

      SaveConfiguration ();

      EnableComponentsForProcessing ();

      if  (crossValidation)
        GroundTruth.Text = curSelModel.RootDir ();
      
      confusionMatrix = new ConfusionMatrix (curSelModel, origClassifierClasses, trainingModel, trainingModelName, GroundTruth.Text, trainingLevel, predictionLevel);
      
      loadingThread = new Thread (new ThreadStart (LoadData));
      loadingThread.Start ();
    }  /* LoadTrainingModels */


    
    private  void  LoadData ()
    {
      loadingThreadRunning = true;

      if  (trainingModel != null)
      {
        BlockStart ();
        trainingModel = null;
        BlockEnd ();
      }

      if  (groundTruth != null)
      {
        BlockStart ();
        groundTruth        = null;
        BlockEnd ();
      }

      if  (crossValidation)
        GradeAgainstCrossValidationRun ();
      else
        GradeAgainstGroundTruth ();

      if  (!cancelProcessing)
        displayConfusionMatrix = true;

      loadingThreadRunning = false;
    }  /* LoadData */



    private  void  PerformPredictions ()
    {
      PicesPrediction p1 = new PicesPrediction (null, 0, 0.0f);
      PicesPrediction p2 = new PicesPrediction (null, 0, 0.0f);

      foreach  (PicesFeatureVector  fv  in groundTruth)
      {
        PicesClass  knownClass = fv.MLClass;
        if  (cancelProcessing)
          break;
        trainingModel.PredictClass (fv, p1, p2);
        PicesClass predClass = p1.MLClass;

        confusionMatrix.MakePrediction (knownClass, predClass, p1.Probability, fv, trainingModel);
        numGroundTruthImagesProcessed++;
      }
    }  /* PerformPredictions*/



    private  void  GradeAgainstGroundTruth ()
    {

      if  (!cancelProcessing)
      {
        if  (trainingModel != null)
        {
          trainingModel = null;
        }

        trainingModel = new TrainingModel2 (runLog, trainingModelName);
        if  (trainingModel.Valid)
        {
          if  (trainingLevel < maxLevel)
            trainingModel.LoadTrainingModelForGivenLevel (trainingLevel, msgQueue);
          else
            trainingModel.LoadExistingModelOtherwiseBuild (msgQueue);
        }
      }


      if  (!cancelProcessing)
      {
        groundTruth = new PicesFeatureVectorList ();
        groundTruth.LoadInSubDirectoryTree (GroundTruth.Text, 
                                            true,        // true = useDirectoryNameForClassName,
                                            runLog,
                                            false        // false = Don't rewiteRootFeatureFile
                                           );

        numGroundTruthImages = groundTruth.Count;
        numGroundTruthImagesProcessed = 0;
      }

      if  (cancelProcessing)
      {
        if  (trainingModel != null)
        {
          BlockStart ();
          trainingModel = null;
          BlockEnd ();
        }
      }
      else
      {
        if  (!trainingModel.Valid)
        {
          MessageBox.Show ("Error Loading Training Model;  refer to dialog box");
          cancelProcessing = true;
        }

        else if  (!cancelProcessing)
        {
          confusionMatrix.SetTrainingModel (trainingModel);
          PerformPredictions ();
        }
      }

      if  (groundTruth != null)
      {
        BlockStart ();
        groundTruth = null;
        BlockEnd ();
      }
    }  /* GradeAgainstGroundTruth */



    private  void  GradeAgainstCrossValidationRun ()
    {
      if  (curSelModel == null)
        curSelModel = new PicesTrainingConfiguration (configFileName, runLog);

      if  (!curSelModel.Valid ())
      {
        MessageBox.Show ("Configuration File[" + configFileName + "] is not valid");
        return;
      }

      bool changesMade = false;
      PicesFeatureVectorList  data = curSelModel.LoadFeatureDataFromTrainingLibraries (changesMade);
      if  (cancelProcessing)
      {
        data = null;
        return;
      }

      if  (data == null)
      {
        MessageBox.Show ("No data was loaded from Configuration File[" + configFileName + "] is not valid");
        return;
      }

      PicesFeatureVectorList  examples = data.StratifyAmoungstClasses (numOfFolds);

      int  imageCount       = examples.Count;
      int  numImagesPerFold = (imageCount + numOfFolds - 1) / numOfFolds;

      int  firstInGroup     = 0;

      int  foldNum;

      for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
      {
        int  lastInGroup;

        // If We are doing the last Fold Make sure that we are including all the examples 
        // that have not been tested.
        if  (foldNum == (numOfFolds - 1))
          lastInGroup = imageCount;
        else
          lastInGroup = firstInGroup + numImagesPerFold - 1;

        PicesFeatureVectorList  trainingImages = new PicesFeatureVectorList ();
        PicesFeatureVectorList  testImages     = new PicesFeatureVectorList ();

        //log.Level (30) << "Fold Num["        << foldNum        << "]   "
        //               << "FirstTestImage["  << firstInGroup   << "]   "
        //               << "LastInGroup["     << lastInGroup    << "]."
        //               << endl;

        for  (int  x = 0; (x < imageCount)  &&  (!cancelProcessing); x++)
        {
          PicesFeatureVector  newImage = new PicesFeatureVector (examples[x]);

          if  ((x >= firstInGroup)  &&  (x <= lastInGroup))
            testImages.Add (newImage);
          else
            trainingImages.Add (newImage);
        }

        if  (cancelProcessing)
          break;
    
        CrossValidate (testImages, trainingImages, foldNum);

        firstInGroup = firstInGroup + numImagesPerFold;
      }

      if  (!cancelProcessing)
      {
        if  (trainingModel != null)
        {
          trainingModel = null;
        }

        msgQueue.AddMsg ("\n" + "Building Final Classifier for User Querey");
        trainingModel = new TrainingModel2 (runLog, curSelModel);
        trainingModel.BuildTrainingModel (data);
      }

      if  ((!cancelProcessing)  &&  (trainingModel != null)  &&  (!trainingModel.Valid))
      {
        MessageBox.Show ("Error Loading Training Model;  refer to dialog box");
        cancelProcessing = true;
      }

      else if  (!cancelProcessing)
      {
        confusionMatrix.SetTrainingModel (trainingModel);
      }

      // We no longer need the Training Model or Ground Truth loaded
      BlockStart ();
      data = null;
      BlockEnd ();
    }  /* GradeAgainstCrossValidationRun */
      
      
     
      
    void  CrossValidate (PicesFeatureVectorList   testExamples, 
                         PicesFeatureVectorList   trainingExamples,
                         int                      foldNum
                        )
    {
      msgQueue.AddMsg ("\n\n" + "Starting Fold[" + (foldNum + 1).ToString () + "]  of Fold[" + numOfFolds + "]" + "\n");
      
      if  (trainingModel != null)
      {
        trainingModel = null;
      }

      msgQueue.AddMsg ("Building Classifier");

      try
      {
        trainingModel = new TrainingModel2 (runLog, curSelModel);
        trainingModel.BuildTrainingModel (trainingExamples);
      }
      catch  (Exception e)
      {
        msgQueue.AddMsg ("\n\n\n" + "Exception occure building training model" + "\n" + e.ToString () + "\n\n");
        MessageBox.Show (this, "Exception Occured Building Training Model." + "\n" + e.ToString (), "CrossValidate", MessageBoxButtons.OK);
        trainingModel = null;
        return;
      }

      if  (cancelProcessing)
      {
        trainingModel = null;
        return;
      }

      if  (!trainingModel.Valid)
      {
        MessageBox.Show (this, "Training Model is Invalid.", "Ground Thruthing", MessageBoxButtons.OK);
      }
      else
      {
        msgQueue.AddMsg ("\n" + "Predictions for fold[" + (foldNum + 1).ToString () + "]");
        
        PicesClass  knownClass = null;

        foreach  (PicesFeatureVector example in testExamples)
        {
          if  (cancelProcessing)
            break;

          knownClass = example.MLClass;

          PicesPrediction prediction = trainingModel.PredictClass (example);
          if  ((otherClass != null)  &&  (prediction.MLClass == noAgreementClass))
          {
            prediction.MLClass = otherClass;
          }
          /**@todo  Add code for "NoAgreement:" to be treeted as a Other Class when there is "NoAgreement".  */

          confusionMatrix.MakePrediction (knownClass, prediction.MLClass, prediction.Probability, example, trainingModel);
        }
      }

      trainingModel.CleanUp ();
      trainingModel = null;
    }  /* CrossValidate */




    private  void  CancelProcessing ()
    {
      CancelButon.Enabled = false;

      cancelProcessing = true;
      BlockStart ();

      if  (trainingModel != null)
        trainingModel.CancelLoad ();

      if  (groundTruth != null)
        groundTruth.CancelLoad ();
        
      if  (curSelModel != null)
         curSelModel.CancelLoad ();

      BlockEnd ();

      int  sleepCount = 0;
      while  (loadingThreadRunning)
      {
        if  (sleepCount > 50)
          break;
        Thread.Sleep (200);
        sleepCount++;
      }

      if  (loadingThread != null)
      {
        loadingThread.Abort ();
        loadingThread = null;
      }


      EnableComponentsForParameterEntry ();
    }  /* CancelProcessing*/





    private void GroundTruthButton_Click(object sender, EventArgs e)
    {
      groundTruthDialog.SelectedPath = GroundTruth.Text;
      if  (groundTruthDialog.ShowDialog (this) == DialogResult.OK)
      {
        GroundTruth.Text = groundTruthDialog.SelectedPath;
      }
    }




    private void groundTruthDialog_HelpRequest(object sender, EventArgs e)
    {

    }



    private void TrainingLevel_SelectionChangeCommitted(object sender, EventArgs e)
    {
      TrainingLevel.Text = (TrainingLevel.Items [TrainingLevel.SelectedIndex]).ToString ();
      trainingLevel = PicesKKStr.StrToUint (TrainingLevel.Text);
      InitializeTrainAndPredLevelFields ();
    }


    private void PredictionLevel_SelectionChangeCommitted(object sender, EventArgs e)
    {
      PredictionLevel.Text = (PredictionLevel.Items [PredictionLevel.SelectedIndex]).ToString ();
      predictionLevel = PicesKKStr.StrToUint (PredictionLevel.Text);
      InitializeTrainAndPredLevelFields ();
    }


    private void TrainingModel_SelectedValueChanged (object sender, EventArgs e)
    {
      trainingModelName = TrainingModelSelected.Text;
      ValidateTrainingModel ();
     }
    
    
    private void timer1_Tick (object sender, EventArgs e)
    {
      String  msg = msgQueue.GetNextMsg ();
      while  (msg != null)
      {
        RunLogOutPut.AppendText (msg + "\n");
        msg = msgQueue.GetNextMsg ();
      }

      if  (displayConfusionMatrix)
      {
        // confusionMatrix.TallyPredictions ();
        confusionMatrix.Show ();
        displayConfusionMatrix = false;
      }

      if  (numGroundTruthImages < 1)
        PredictionProgressBar.Value = 0;
      else
        PredictionProgressBar.Value = 100 * numGroundTruthImagesProcessed / numGroundTruthImages;

      GroundTruthCount.Text     = numGroundTruthImages.ToString ();
      GroundTruthProcessed.Text = numGroundTruthImagesProcessed.ToString ();
    }



    private void StartButon_Click(object sender, EventArgs e)
    {
      cancelProcessing = false;
      LoadTrainingModelAndGroundTruth ();
      bool timerEnabled = timer1.Enabled;
    }


    private void CancelButon_Click(object sender, EventArgs e)
    {
      if  (loadingThreadRunning)
      {
        DialogResult dr = MessageBox.Show (this, "Are you sure you want to Cancel?", "Grade Training Model", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.No)
          return;
      }

      CancelProcessing ();
    }


    private void GetRunTimeParameters_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  (loadingThreadRunning)
      {
        DialogResult dr = MessageBox.Show (this, "Are you sure you want to Close this form?", "Grade Training Model", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.No)
        {
          e.Cancel = true;
          return;
        }
        else
        {
          CancelProcessing ();
        }
      }

      SaveConfiguration ();
      CleanUpMemory ();
    }


    private void GetRunTimeParameters_Load (object sender, EventArgs e)
    {
      LoadConfiguraionData ();
      PicesDataBase  dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);

      dbConn.SipperFileLoadOracle ();

      dbConn.Close ();
      dbConn = null;
    }


    private void CrossValidation_CheckedChanged (object sender, EventArgs e)
    {
      crossValidation = CrossValidation.Checked;
      if  (crossValidation)
      {
        if  (curSelModel != null)
          GroundTruth.Text = curSelModel.RootDir ();
        GroundTruth.Enabled = false;
      }
      else
      {
        GroundTruth.Enabled = true;
      }
    }


    private void EditModel_Click (object sender, EventArgs e)
    {
      String  fullModelName = PicesTrainingConfiguration.GetEffectiveConfigFileName (trainingModelName);

      Process proc = new Process();
      proc.StartInfo.WorkingDirectory = @"";
      proc.StartInfo.FileName = "notepad";
      proc.StartInfo.Arguments = fullModelName;
      //proc.StartInfo.FileName = batFileName;
      //proc.StartInfo.Arguments = arguments;
      proc.StartInfo.UseShellExecute = true;
      proc.StartInfo.RedirectStandardOutput = false;
      proc.StartInfo.RedirectStandardError = false;
      proc.Start();
      proc.WaitForExit ();
    }
  }
}