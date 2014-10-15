using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

using  SipperFile;


using PicesInterface;


namespace SipperFileViewer
{
  public partial class AutoDetectionParamters : Form
  {
    private  string            activeTrainingModel1Name = null;
    private  string            activeTrainingModel2Name = null;
    private  int               blobMinSize             = 0;
    private  SipperFileViewer  viewer = null;
       

    public  bool  RebuildTrainingLibrary1  {get {return  rebuildTrainingLibrary1.Checked;}  set  {rebuildTrainingLibrary1.Checked = value;}}
    public  bool  RebuildTrainingLibrary2  {get {return  rebuildTrainingLibrary2.Checked;}  set  {rebuildTrainingLibrary2.Checked = value;}}



    public AutoDetectionParamters (SipperFileViewer  _viewer)
    {
      viewer = _viewer;
      InitializeComponent();
    }



    private  void  SetTrainingLibrarySelected (string    modelName, 
                                               ComboBox  control
                                              )
    {
      int  idx = 0;
      for  (idx = 0;  idx < control.Items.Count;  idx++)
      {
        if  (modelName == (string)(control.Items[idx]))
        {
          // control.SelectedIndexChanged -= TrainingLibrary1_SelectedIndexChanged;
          control.SelectedIndex = idx;
          //control.SelectedIndexChanged += TrainingLibrary1_SelectedIndexChanged;
          return;
        }
      }

      control.SelectedIndex = -1;
    }  /* SetTrainingLibrarySelected*/

 

    private  void  AutoDetectionParamters_Load (object sender, EventArgs e)
    {
      activeTrainingModel1Name = ActiveTrainingLibraries.Model1Name;
      activeTrainingModel2Name = ActiveTrainingLibraries.Model2Name;

      blobMinSize              = viewer.BlobMinSize;
      if  (activeTrainingModel1Name == null)
        activeTrainingModel1Name = "";

      if  (activeTrainingModel2Name == null)
        activeTrainingModel2Name = "";

      LoadTrainingModelNames ();

      SetTrainingLibrarySelected (activeTrainingModel1Name, TrainingLibrary1);
      SetTrainingLibrarySelected (activeTrainingModel2Name, TrainingLibrary2);

      MinBlobSize.Text = blobMinSize.ToString ("###0");
    }



    private  string  LoadTextFile (string  fileName)
    {
      string  result = "";

      StreamReader  sr = null;
      try
      {
        sr = new StreamReader (fileName);
      }
      catch (Exception)
      {
        return  result;
      }


      while  (!sr.EndOfStream)
      {
        result += sr.ReadLine () + "\n";
      }

      sr.Close ();
      return  result;
    }  /* LoadTextFile */



    private  void  LoadActiveTrainingModel (ref string         modelName, 
                                            ref ComboBox       control,
                                            bool               useExistingTrainedModel
                                           )

    {
      modelName = (string)control.SelectedItem;
      SetTrainingLibrarySelected (modelName, control);
    }  /* LoadActiveTrainingModel */



    private  bool  TrainingLibrary_SelectedIndexChangedAlreadyCalled = false;
    



    private void UpdateButton_Click (object sender, EventArgs e)
    {
      if  (!int.TryParse (MinBlobSize.Text, out blobMinSize))
      {
        MessageBox.Show ("Invalid data in 'MinimumSize'", "Validating MinimumSize");
      }
      else
      {
        if  (blobMinSize < 10)
        {
          MessageBox.Show ("'MinimumSize'  must be greater than 10", "Validating MinimumSize");
        }
        else
        {
          viewer.BlobMinSize              = blobMinSize;
          ActiveTrainingLibraries.SetModel1 (activeTrainingModel1Name, this.rebuildTrainingLibrary1.Checked, null);
          ActiveTrainingLibraries.SetModel2 (activeTrainingModel2Name, this.rebuildTrainingLibrary2.Checked, null);
          this.Close ();
        }
      }
    }  /* UpdateButton_Click */
    
   
   
    private  void  LoadTrainingModelNames ()
    {
      string[]  modelNames = TrainingModel2.GetListOfTrainingModels ();;
      if  (modelNames == null)
      {
        TrainingLibrary1.Items.Add ("None");
        TrainingLibrary2.Items.Add ("None");
      }
      else
      {
        foreach  (string s in modelNames)
        {
          TrainingLibrary1.Items.Add (s);
          TrainingLibrary2.Items.Add (s);
        }
      }
      
      if  (!String.IsNullOrEmpty (activeTrainingModel1Name))
        SetTrainingLibrarySelected (activeTrainingModel1Name, TrainingLibrary1);

      if (!String.IsNullOrEmpty(activeTrainingModel2Name))
          SetTrainingLibrarySelected(activeTrainingModel2Name, TrainingLibrary2);

    }  /* LoadTrainingModelNames */




    private void TrainingLibrary1_SelectedIndexChanged (object sender, EventArgs e)
    {
      if   (TrainingLibrary_SelectedIndexChangedAlreadyCalled)
        return;
      TrainingLibrary_SelectedIndexChangedAlreadyCalled = true;
      if  ((string)TrainingLibrary1.SelectedItem != activeTrainingModel1Name)
        LoadActiveTrainingModel (ref activeTrainingModel1Name, ref TrainingLibrary1, true);
      MinBlobSize.Focus ();
      TrainingLibrary_SelectedIndexChangedAlreadyCalled = false;
    }


    private void TrainingLibrary2_SelectedIndexChanged (object sender, EventArgs e)
    {
      if   (TrainingLibrary_SelectedIndexChangedAlreadyCalled)
        return;
      TrainingLibrary_SelectedIndexChangedAlreadyCalled = true;
      if  ((string)TrainingLibrary2.SelectedItem != activeTrainingModel2Name)
        LoadActiveTrainingModel (ref activeTrainingModel2Name, ref TrainingLibrary2, true);
      MinBlobSize.Focus ();
      TrainingLibrary_SelectedIndexChangedAlreadyCalled = false;
    }

  }
}