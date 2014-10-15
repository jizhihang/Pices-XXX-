using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;


namespace PicesCommander
{
  public partial class TrainingModelCreateFromDirectory : Form
  {
    private  bool  validationErrorsFound = false;
    private  PicesTrainingConfiguration  config = null;
    private  PicesRunLog  runLog = new PicesRunLog ();

    private  String  lastDirectoryValidated = "";
    private  String  lastModelNameValidated = "";


    public TrainingModelCreateFromDirectory()
    {
      InitializeComponent();
    }


    private  void  LoadDirectoryTree ()
    {
      config = new PicesTrainingConfiguration (new DirectoryInfo (SourceDirectory.Text), runLog);
    }



    private  int  ImageCount (String  dirName)
    {
      DirectoryInfo di = null;
      
      try {di = new DirectoryInfo (dirName);}  catch (Exception) {di = null;}
      if  ((di == null)  ||  (!di.Exists))
        return 0;

      FileInfo[]  files = di.GetFiles ("*.bmp");
      if  (files == null)
        return 0;
      return  files.Length;
    }  /* ImageCount*/



    private  void  LoadClassList ()
    {
      ClassList.Items.Clear ();

      if  (config == null)
        return;

      if  (!config.Valid ())
        return;

      PicesTrainingClassList  trainingClasses  = config.TrainingClasses ();
      if  (trainingClasses != null)
      {
        int  idx = 0;
        foreach  (PicesTrainingClass  tc  in  trainingClasses)
        {
          String  expDir = tc.ExpandedDirectory (config.RootDir ());
          ClassList.Items.Add (tc.MLClass.Name);
          ClassList.Items[idx].SubItems.Add (tc.Directory);
          ClassList.Items[idx].SubItems.Add (ImageCount (expDir).ToString ("#,###,##0"));
          idx++;
        }
      }
    }  /* LoadClassList */



    private  void  ValidateSourceDirectory ()
    {
      errorProvider1.SetError (SourceDirectory, null);
      if  (String.IsNullOrEmpty (SourceDirectory.Text))
      {
        errorProvider1.SetError (SourceDirectory, "Can not leave Source Directory blank");
        validationErrorsFound = true;
      }
      else
      {
        DirectoryInfo di = null;
        try  {di = new DirectoryInfo (SourceDirectory.Text);}
        catch  (Exception e)
        {
          errorProvider1.SetError (SourceDirectory, "Error getting directory information.  " + e.ToString ());
          validationErrorsFound = true;
          di = null;
        }
        if  ((di != null)  &&  (!di.Exists))
        {
          errorProvider1.SetError (SourceDirectory, "Directory does not exist.");
          validationErrorsFound = true;
        }

        if  ((SourceDirectory.Text.CompareTo (lastDirectoryValidated) != 0)  &&  (!validationErrorsFound))
        {
          config = new PicesTrainingConfiguration (di, runLog);
          LoadClassList ();
          ModelName.Text = OSservices.GetRootNameOfDirectory (SourceDirectory.Text);
        }
      }

      lastDirectoryValidated = SourceDirectory.Text;
    }  /* ValidateSourceDirectory */



    private  void  ValidateModelName ()
    {
      errorProvider1.SetError (ModelName, null);
      if  (String.IsNullOrEmpty (ModelName.Text))
      {
        validationErrorsFound = true;
        errorProvider1.SetError (ModelName, "Can not leave Model Name Blank");
      }
      String s = ModelName.Text.Trim ().ToUpper ();
      String  invalidChars = "";
      for  (int x = 0;  x < s.Length;  x++)
      {
        char  c = s[x];
        if  (((c >= 'A')  &&  (c <= 'Z'))  ||  ((c >= '0')  &&  (c <= '9')))
          continue;
        if  ((c == '-')  ||  (c == '_'))
          continue;
        if  (invalidChars.Length > 0)
          invalidChars += ", ";
        invalidChars += c.ToString ();
      }

      if  (invalidChars.Length > 0)
      {
        validationErrorsFound = true;
        errorProvider1.SetError (ModelName, "Invalid Characters[" + invalidChars + "]");
      }

      lastModelNameValidated = ModelName.Text.Trim ();
    }  /* ValidateModelName */



    private  void  ValidateAll ()
    {
      validationErrorsFound = false;
      ValidateSourceDirectory ();
      ValidateModelName ();
    }

    
    private  void  CreateTrainingModel ()
    {
      String  fileName =  OSservices.AddSlash (PicesSipperVariables.TrainingModelsConfigurationDir ()) + ModelName.Text + ".cfg";
      config.Save (fileName);
    }  /* CreateTrainingModel */


    
    private void  CreateButtonClick(object sender, EventArgs e)
    {
      ValidateAll ();
      if  (validationErrorsFound)
        return;

      DialogResult dr = MessageBox.Show (this, "Create Training Model Yes/No ?", "Create Model", MessageBoxButtons.YesNo);
      if  (dr == DialogResult.Yes)
      {
        CreateTrainingModel ();
        Close ();
      }
    }


    private void SourceDirectory_Validating(object sender, CancelEventArgs e)
    {
      if  (!SourceDirectoryBrowseButton.Focused)
        ValidateSourceDirectory ();
    }


    private void ModelName_Validating(object sender, CancelEventArgs e)
    {
      ValidateModelName ();
    }


    private void SourceDirectoryBrowseButton_Click(object sender, EventArgs e)
    {
      FolderBrowserDialog  fbd = new FolderBrowserDialog ();
      if  (!String.IsNullOrEmpty (lastDirectoryValidated))
        fbd.SelectedPath = lastDirectoryValidated;
      else
      {
        fbd.SelectedPath = PicesSipperVariables.PicesHomeDir ();
      }

      DialogResult dr = fbd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        SourceDirectory.Text = fbd.SelectedPath;
        ValidateSourceDirectory ();
      }
    }

    private void CancelAppButton_Click(object sender, EventArgs e)
    {
      Close ();
    }

    private void TrainingModelCreateFromDirectory_FormClosing(object sender, FormClosingEventArgs e)
    {

    }
  }  /* TrainingModelCreateFromDirectory */

}
