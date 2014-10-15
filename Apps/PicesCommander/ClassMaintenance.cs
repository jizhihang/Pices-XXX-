using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using  PicesInterface;
using  SipperFile;
//**********************************************************************************
//* Developer:  Kurt Kramer                                                        *
//* Date        2009-08-08                                                         *
//*                                                                                *
//*                                                                                *
//**********************************************************************************


namespace PicesCommander
{
  public partial class ClassMaintenance : Form
  {
    private  PicesDataBase   mainDbConn     = null;
    private  PicesClassList  classes        = null;
    private  PicesClass      selectedClass  = null;
    private  PicesClass      mergeClass     = null;  // if Merging of two classes is seleceted this will point to the class to merge to.
    private  bool            addingNewClass = false;
    private  bool            deletingExistingClass = false;

    private  PicesRunLog     runLog = null;

    private  bool            validationErrorsFound = false;

    private  bool            nameChangeHasOccured = false;
    private  PicesClass      newClassEntry    = null;  // Entry with updated fields.
    private  String          newClassName     = "";    // Utilized by name change.
    private  String          origClassName    = "";    // Utilized by name change.
    private  PicesClass      origParent       = null;
    private  PicesGoalKeeper runLogGoalie     = null;
    private  Queue<String>   runLogMsgQueue   = null;  // List of messages that can be added to 'RunLog'.
    private  bool            updateRunning    = false;
    private  Thread          updateThread     = null;
    private  int             sipperFilesTotal = 0;
    private  int             sipperFilesUpdated = 0;
    private  PicesSipperFileList sipperFiles = null;

    private  int             lastHeight = 0;
    private  int             lastWidth  = 0;

    private  bool            allowUpdates = false;


    public ClassMaintenance()
    {
      runLog = new PicesRunLog ();
      runLogGoalie = new PicesGoalKeeper ("Harvesting Run Log");
      runLogMsgQueue = new Queue<String> ();

      InitializeComponent();

      allowUpdates = PicesSipperVariables.AllowUpdates ();
    }


    private  void  RunLogAddMsg (String  msg)
    {
      runLogGoalie.StartBlock ();

      if  (runLogMsgQueue == null)
        runLogMsgQueue = new Queue<String> ();

      runLogMsgQueue.Enqueue (msg);

      runLogGoalie.EndBlock ();
    }  /* RunLogAddMsg */



    private  void  RunLogMsgQueueFlush ()
    {
      runLogGoalie.StartBlock ();
      if  (runLogMsgQueue != null)
      {
        while  (runLogMsgQueue.Count > 0)
        {
          String  msg = runLogMsgQueue.Dequeue ();
        UpdateLog.AppendText (msg);
        }
      }

      runLogGoalie.EndBlock ();
    }  /* RunLogMsgQueueFlush */



    
    private  void  LoadClassesFromDataBase ()
    {
      classes = mainDbConn.MLClassLoadList ();
      ClassTree.Nodes.Clear ();

      ContextMenuStrip  cms = new ContextMenuStrip ();
      cms.Items.Add ("Insert a new class", null, InsertAClass);
     
      PicesClass  root = classes.RootNode;

      TreeNode  tn = new TreeNode (root.Name);
      tn.ContextMenuStrip = cms;
      ClassTree.Nodes.Add (tn);

      if  (root.Children != null)
      {
        foreach  (PicesClass pc in root.Children)
        {
          try
          {
            AddSubTreeToClassTree (pc, tn.Nodes, cms, 1);
          }
          catch  (Exception eee5)
          {
            MessageBox.Show (this, "'AddSubTreeToClassTree' caused an exception while procesing class[" + pc.Name + "]\n",
                             eee5.ToString (), MessageBoxButtons.OK
                            );
          }
        }
      }

      ClassTree.ExpandAll ();
    }  /* LoadClassesFromDataBase */


        
    private  void  AddSubTreeToClassTree (PicesClass         pc, 
                                          TreeNodeCollection nodes,
                                          ContextMenuStrip   cms,
                                          int                depth
                                         )
    {
      if  (depth > 10)
      {
        // This is just too deep something has gone wriong.
        return;
      }

      String  name = pc.Name;
      if  (!pc.StoredOnDataBase)
        name += "  ***NOT IN DATABASE***";

      TreeNode  tn = new TreeNode (pc.Name);
      tn.ContextMenuStrip = cms;

      nodes.Add (tn);
      if  (pc.Children != null)
      {
        foreach  (PicesClass  child  in  pc.Children)
        {
          try
          {
            AddSubTreeToClassTree (child, tn.Nodes, cms, depth + 1);
          }
          catch  (Exception eee)
          {
            MessageBox.Show (this, "AddSubTreeToClassTree'  Caused Exception\n" + eee.ToString (), "AddSubTreeToClassTree", MessageBoxButtons.OK);
          }
        }
      }
    }  /* AddSubTreeToClassTree */



    private  void  PopulateClassMaintenancePanel (PicesClass pc)
    {
      if  (pc == null)
        return;

      ClassId.Text = pc.ClassId.ToString ("###,##0");
      ClassName.Text = pc.Name;
      ParentName.Text = pc.ParentName;
      Description.Text = pc.Description;
      MergeCheckBox.Checked = false;
      mergeClass = null;
      MergeClass.Enabled = false;
      MergeClass.Text = "";
      MergeClass.Visible = false;
      MergeSearchButton.Visible = false;
      MergeHelpTextLabel.Enabled = false;
      MergeHelpTextLabel.Visible = false;
      MergeLabel.Enabled = false;
      MergeLabel.Visible = false;
    }




    private  String  ValidClassNameFormat (String  name)
    {
      if  (String.IsNullOrEmpty (name))
        return "Class Name can not be blank";

      if  ((name[0] == '_')  ||  (name[name.Length - 1] == '_'))
        return "Underscore(_) can not be the frst or last character.";
      
      if  (name.Contains ("__"))
      {
        return  "Can not have more than one underscore character(_) in a row.";
      }

      String  invalidChars = "";
      for  (int x = 0;  x < name.Length;  x++)
      {
        char c = Char.ToUpper (name[x]);
        if  ((c >= '0')  &&  (c <= '9'))
          continue;

        if  ((c >= 'A')  &&  (c <= 'Z'))
          continue;

        if  (c == '_')
          continue;

        if  (invalidChars != "")
          invalidChars += ", ";
        invalidChars += "'" + c.ToString () + "'";
      }

      if  (invalidChars == "")
        return null;
      
      return  "Invalid Characters " + invalidChars;
    }  /* ValidClassNameFormat */



    private  void  ValidateClassName ()
    {
      errorProvider1.SetError (ClassName, null);
      
      if  (String.IsNullOrEmpty (ClassName.Text))
      {
        errorProvider1.SetError (ClassName, "Can not leave the name blank.");
        validationErrorsFound = true;
        return;
      }

      if  (addingNewClass)
      {
        String  errMsg = ValidClassNameFormat (ClassName.Text);
        if  (errMsg != null)
        {
          errorProvider1.SetError (ClassName, errMsg);
          validationErrorsFound = true;
        }
        else
        {
          PicesClass  existingClass = mainDbConn.MLClassLoad (ClassName.Text);
          if  (existingClass != null)
          {
            errorProvider1.SetError (ClassName, "This name is already in use.");
            validationErrorsFound = true;
          }
        }
      }

      else if  (ClassName.Text.ToUpper () != selectedClass.Name.ToUpper ())
      {
        // Looks like user has changed the class name;  make sure it is not already in use."
        PicesClass  existingClass = mainDbConn.MLClassLoad (ClassName.Text);
        if  (existingClass != null)
        {
          errorProvider1.SetError (ClassName, "This name is already in use.");
          validationErrorsFound = true;
        }
        else
        {
          String  errMsg = ValidClassNameFormat (ClassName.Text);
          if  (errMsg != null)
          {
            errorProvider1.SetError (ClassName, errMsg);
            validationErrorsFound = true;
          }
        }
      }
      return;
    }  /* ValidateClassName */



    private  void  ValidateParentName ()
    {
      errorProvider1.SetError (ParentSearchButton, null);
      
      if  (String.IsNullOrEmpty (ParentName.Text))
        return;

      PicesClass  parent = mainDbConn.MLClassLoad (ParentName.Text);
      if  (parent == null)
      {
        errorProvider1.SetError (ParentSearchButton, "No such class[" + ParentName.Text + "]");
        validationErrorsFound = true;
      }
      else if  (selectedClass.IsInMySubTree (parent))
      {
        errorProvider1.SetError (ParentSearchButton, "This will create a circular relationship.");
        validationErrorsFound = true;
      }
      return;
    }  /* ValidateParentName */



    private  void  ValidateMergeWithAnotherClass ()
    {
      if  (MergeCheckBox.Checked)
      {
        MergeClass.Visible = true;
        MergeClass.ReadOnly = true;
        MergeSearchButton.Visible = true;
        MergeSearchButton.Enabled = true;
        MergeHelpTextLabel.Visible = true;
        MergeHelpTextLabel.Enabled = true;
        MergeLabel.Visible = true;
        MergeLabel.Enabled = true;
        ClassName.Text   = selectedClass.Name;
        Description.Text = selectedClass.Description;
        ParentName.Text  = selectedClass.ParentName;
      }
      else
      {
        MergeClass.Enabled = false;
        MergeClass.Visible = false;
        MergeClass.ReadOnly = true;
        mergeClass = null;
        MergeSearchButton.Visible = false;
        MergeSearchButton.Enabled = false;
        MergeHelpTextLabel.Visible = false;
        MergeHelpTextLabel.Enabled = false;
        MergeLabel.Visible = false;
        MergeLabel.Enabled = false;
      }
    }  /* ValidateMergeWithAnotherClass */



    private  void  ValidateMergeClass ()
    {
      if  (MergeCheckBox.Checked)
      {
        errorProvider1.SetError (MergeClass, null);
        mergeClass = mainDbConn.MLClassLoad (MergeClass.Text);
        if  (mergeClass == null)
        {
          errorProvider1.SetError (MergeClass, "You must select a valid class to merge to.");
          validationErrorsFound = true;
        }
      }
    }  /* ValidateMergeClass */



    private  void  ValidateAllFields ()
    {
      validationErrorsFound = false;
      ValidateClassName ();
      ValidateParentName ();
      ValidateMergeWithAnotherClass ();
      ValidateMergeClass ();
    }


    private  bool  ChangesMade ()
    {
      if  (selectedClass == null)
        return  false;

      if  ((ClassName.Text   != selectedClass.Name)        ||
           (ParentName.Text  != selectedClass.ParentName)  ||
           (Description.Text != selectedClass.Description)
          )
        return true;
      else
        return false;
    }



    private  void  InsertAClass (Object sender, EventArgs e)
    {
      addingNewClass = true;
      MergeCheckBox.Checked = false;
      PicesClass  newSelectedClass = new PicesClass ("", selectedClass);
      selectedClass = newSelectedClass;
      EnableAllFields ();
      PopulateClassMaintenancePanel (selectedClass);
      ClassName.Focus ();
    }  /* InsertAClass */




    private  void  DisableAllFields ()
    {
      ClassTree.Enabled          = false;
      ClassName.Enabled          = false;
      ParentName.Enabled         = false;
      ParentSearchButton.Enabled = false;
      Description.Enabled        = false;
      UpdateButton.Enabled       = false;
      DeleteButton.Enabled       = false;
      MergeCheckBox.Enabled      = false;
      MergeClass.Enabled         = false;
      MergeHelpTextLabel.Enabled = false;
      MergeSearchButton.Enabled  = false;
      MergeLabel.Enabled         = false;
    } /* DisableAllFields */



    private  void  EnableAllFields ()
    {
      ClassTree.Enabled          = true;
      ClassName.Enabled          = true;
      ParentName.Enabled         = true;
      Description.Enabled        = true;
      if  (allowUpdates)
      {
        UpdateButton.Enabled       = true;
        DeleteButton.Enabled       = true;
      }
      ParentSearchButton.Enabled = true;

      if  (addingNewClass)
      {
        MergeCheckBox.Checked = false;
        MergeCheckBox.Enabled = false;
        DeleteButton.Enabled  = false;
      }
      else
      {
        MergeCheckBox.Checked = false;
        MergeCheckBox.Enabled = true;
        DeleteButton.Enabled  = true;
      }

      if  (MergeCheckBox.Checked)
      {
        MergeClass.Visible  = true;
        MergeClass.ReadOnly = true;
        MergeClass.Enabled  = true;
        MergeSearchButton.Visible = true;
        MergeSearchButton.Enabled = true;
        MergeHelpTextLabel.Enabled = true;
        MergeHelpTextLabel.Visible = true;
        MergeLabel.Enabled = true;
        MergeLabel.Visible = true;
      }
      else
      {
        MergeClass.Visible         = false;
        MergeClass.ReadOnly        = true;
        MergeClass.Enabled         = false;
        MergeSearchButton.Visible  = false;
        MergeSearchButton.Enabled  = false;
        MergeHelpTextLabel.Enabled = false;
        MergeHelpTextLabel.Visible = false;
        MergeLabel.Enabled         = false;
        MergeLabel.Visible         = false;
      }
    } /* EnableAllFields */

    

    private void ClassMaintenance_Load (object sender, EventArgs e)
    {
      lastHeight = Height;
      lastWidth  = Width;

      if  (runLog == null)
        runLog = new PicesRunLog ();
      mainDbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      LoadClassesFromDataBase ();
    }



    private  TreeNode  LocateClassTreeNode (String              name,
                                            TreeNodeCollection  tnc
                                           )
    {
      foreach  (TreeNode tn in tnc)
      {
        if  (tn.Text == name)
          return  tn;

        TreeNode  selectedTreeNode = LocateClassTreeNode (name, tn.Nodes);
        if  (selectedTreeNode != null)
          return  selectedTreeNode;
      }

      return null;
    }  /* LocateClassTreeNode */




    private void ClassTree_AfterSelect(object sender, TreeViewEventArgs e)
    {
      if  (e == null)
        return;
      
      if  (updateRunning)
      {
        ClassTree.SelectedNode = LocateClassTreeNode (selectedClass.Name, ClassTree.Nodes);
        return;
      }

      if  (ChangesMade ())
      {
        DialogResult dr = MessageBox.Show ("Do you want to loose changes that were made ?", "Changes Made", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.No)
        {
          ClassTree.SelectedNode = LocateClassTreeNode (selectedClass.Name, ClassTree.Nodes);
          return;
        }
      }

      origParent = null;
      addingNewClass = false;
      deletingExistingClass = false;
      String className = e.Node.Text;
      selectedClass = classes.LookUpByName (className);
      if  (selectedClass == null)
      {
        MergeCheckBox.Enabled = false;
        DeleteButton.Enabled  = false;
        UpdateButton.Enabled  = false;
        return;
      }

      origParent = selectedClass.Parent;

      EnableAllFields ();
      
      if  (selectedClass.Parent == null)
      {
        MergeCheckBox.Enabled = false;
        DeleteButton.Enabled  = false;
        UpdateButton.Enabled  = false;
      }

      PopulateClassMaintenancePanel (selectedClass);
    }


    private  void  StartUpdateProcedure ()
    {
      if  (!allowUpdates)
      {
        MessageBox.Show (this, "Updates are not allowed.", "Class Maintenance", MessageBoxButtons.OK);
        return;
      }

      if  (addingNewClass)
        newClassEntry = new PicesClass (ClassName.Text, ParentName.Text);
      else
        newClassEntry = selectedClass;

      newClassEntry.Description = Description.Text;
      newClassEntry.Parent      = PicesClassList.GetUniqueClass (ParentName.Text, null);

      DisableAllFields ();
      updateThread = new Thread (new ThreadStart (UpdateProcess));
      updateThread.Start ();
      Thread.Sleep (50);
      updateTimer.Enabled = true;

      UpdatingProgressBar.Value   = 0;
      UpdatingProgressBar.Visible = true;
    }  /* PerformUpdate */
    

    private  void  UpdateProcess ()
    {
      PicesDataBase.ThreadInit ();
      updateRunning = true;

      PicesRunLog  updRunLog = new PicesRunLog ();
      PicesDataBase  updDbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (updRunLog);

      sipperFilesTotal   = 0;
      sipperFilesUpdated = 0;

      if  (addingNewClass  ||  deletingExistingClass  || (mergeClass != null))
      {
        sipperFiles = updDbConn.SipperFileLoad ("", "", "");
        if  (sipperFiles == null)
        {
          RunLogAddMsg ("Could not retrieve list of SIPPER files update failed." + "\n");
          updateRunning = false;
          return;
        }
        sipperFilesTotal = sipperFiles.Count;
      }

      if  (addingNewClass)
        UpdateProcessAdd (updDbConn);
      
      else if  (deletingExistingClass)
        UpdateProcessDelete (updDbConn);

      else if  (mergeClass != null)
        UpdateProcessMerge (updDbConn);

      else if  (nameChangeHasOccured)
        UpdateProcessNameChange (updDbConn);

      else
        UpdateDataFields (updDbConn);

      updDbConn.Close ();
      updDbConn = null;
      GC.Collect ();

      PicesDataBase.ThreadEnd ();
      updateRunning = false;
    }  /* UpdateProcess */



    private  void  UpdateProcessAdd (PicesDataBase  updDbConn)
    {
      updDbConn.MLClassInsert (newClassEntry);
      if  (updDbConn.Valid ())
      {
        RunLogAddMsg ("Class [" + newClassEntry.Name + "]  Inserted into database." + "\n");
      }
      else
      {
        RunLogAddMsg ("Error inserting Class [" + newClassEntry.Name + "]  into database." + "\n");
        RunLogAddMsg (updDbConn.LastErrorDesc () + "\n");
      }
    }


    private  void  UpdateProcessDelete (PicesDataBase  updDbConn)
    {
      RunLogAddMsg ("Deleting Class[" + selectedClass.Name + "]" + "\n");
      // Go through each sipper file and change the class name in there related Imaeges table entries.

      TimeSpan  startProcessTime = System.Diagnostics.Process.GetCurrentProcess ().TotalProcessorTime;

      PicesDataBaseLogEntry  logEntry = 
                 updDbConn.LogEntriesProcessStart ("DL", 
                                                   "ClassMaintenanceDelete",  
                                                   ExecutableDateTimeStamp (),
                                                   "ClassMaintenance::UpdateProcessDelete " + selectedClass.Name,
                                                   ""
                                                  );

      String sqlCmd = "Call MLClassDeleteComprehensive (\"" + selectedClass.Name + "\")";
      updDbConn.QueryStatement (sqlCmd, null);

      PicesClassList.UpdateParent (selectedClass, selectedClass.Parent);

      if  (selectedClass.Parent != null)
      {
        if  (selectedClass.Parent.Children != null)
          selectedClass.Parent.Children.Remove (mergeClass);
      }

      TimeSpan  endProcessTime = System.Diagnostics.Process.GetCurrentProcess ().TotalProcessorTime;

      updDbConn.LogEntriesProcessEnd (logEntry, (float)((startProcessTime - endProcessTime).TotalSeconds), "Done");

      selectedClass.StoredOnDataBase = false;
    }  /* UpdateProcessDelete */



    private  void  UpdateProcessMerge (PicesDataBase  updDbConn)
    {
      RunLogAddMsg ("Merging Class [" + selectedClass.Name + "]  into [" + mergeClass.Name + "]" + "\n");
      // First:   Update the Images table.
      // Second:  Update Classes table.

      String sqlCmd = "Call MLClassMergeComprehensive (" + "\"" + selectedClass.Name  + "\"" + ", " +  
                                                           "\"" + mergeClass.Name     + "\"" +
                                                     ")";
      updDbConn.QueryStatement (sqlCmd, null);

      PicesClassList.UpdateParent (mergeClass, selectedClass);

      if  (mergeClass.Parent != null)
      {
        if  (mergeClass.Parent.Children != null)
          mergeClass.Parent.Children.Remove (mergeClass);
      }
      selectedClass.StoredOnDataBase = false;

      selectedClass = mergeClass;
    }  /* UpdateProcessMerge */



    private  void  UpdateProcessNameChange (PicesDataBase  updDbConn)
    {
      RunLogAddMsg ("Change ClassName[" + origClassName + "]  to [" + newClassName + "]" + "\n");

      bool  successful = false;
      PicesClass.ChangeNameOfClass (selectedClass, newClassName, ref successful);
      if  (!successful)
        RunLogAddMsg ("  *** Name Change Failed ***" + "\n");
      else
      {
        // This call will also update the ParentName in other classes that point to teh old name.
        updDbConn.MLClassUpdate (origClassName, selectedClass);
      }
    }  /* UpdateProcessNameChange */



    private  void  UpdateDataFields (PicesDataBase  updDbConn)
    {
      updDbConn.MLClassUpdate (selectedClass.Name, newClassEntry);
      if  (newClassEntry.Parent != origParent)
      {
        if  ((origParent != null)  &&  (origParent.Children != null))
          origParent.Children.Remove (selectedClass);
      }
    }  /* UpdateDataFields */



    private DateTime ExecutableDateTimeStamp ()
    {
      DateTime  compilationDateTime;
      System.IO.FileInfo fi = new System.IO.FileInfo(Application.ExecutablePath.Trim());
      try
      {
        compilationDateTime =  fi.LastWriteTime;
      }
      catch (Exception)
      {
        compilationDateTime = DateTime.Now;
      }
      finally
      {
        fi = null;
      }

      return  compilationDateTime;
    }


    private void UpdateButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        MessageBox.Show (this, "Updates are not allowed.", "Class Maintenance", MessageBoxButtons.OK);
        return;
      }

      origClassName = selectedClass.Name;
      newClassName  = ClassName.Text;

      ValidateAllFields ();
      if  (validationErrorsFound)
        return;

      if  (MergeCheckBox.Checked)
      {
        DialogResult dr = MessageBox.Show ("Merging two classes will take a lot of time.  Is this what you want to do ?", "Merge Classses", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.No)
          return;
      }
      
      if  (!addingNewClass)
      {
        nameChangeHasOccured = (selectedClass.Name != ClassName.Text);
      }

      updateTimer.Enabled = true;
      StartUpdateProcedure ();
    }



    private void DeleteButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
      {
        MessageBox.Show (this, "Updates are not allowed.", "Class Maintenance", MessageBoxButtons.OK);
        return;
      }

      if  ((!addingNewClass)  &&  (selectedClass != null))
      {
        DialogResult dr = MessageBox.Show ("Are you sure that you want to delete this Class ?", "Delete", MessageBoxButtons.YesNo);
        if  (dr == DialogResult.Yes)
        {
          deletingExistingClass = true;
          StartUpdateProcedure ();
        }
      }
    }


    private void updateTimer_Tick(object sender, EventArgs e)
    {
      RunLogMsgQueueFlush ();
      if  (!updateRunning)
      {
        // UpdateHasCompleted
        updateTimer.Enabled = false;
        UpdatingProgressBar.Visible = false;
        LoadClassesFromDataBase ();

        if  (selectedClass != null)
        {
          ClassName.Text   = selectedClass.Name;
          ParentName.Text  = selectedClass.ParentName;
          Description.Text = selectedClass.Description;
        }

        EnableAllFields ();
        ClassTree.SelectedNode = LocateClassTreeNode (selectedClass.Name, ClassTree.Nodes);
      }

      UpdatingProgressBar.Maximum  = sipperFilesTotal;
      UpdatingProgressBar.Value    = sipperFilesUpdated;
    }


    private void ParentSearchButton_Click(object sender, EventArgs e)
    {
      SelectAPicesClass  sac = new SelectAPicesClass (classes, "", null);
      sac.SelectedClass = PicesClassList.GetUniqueClass (ParentName.Text, "");
      sac.ShowDialog ();
      if  (sac.SelectedClass != null)
      {
        ParentName.Text = sac.SelectedClass.Name;
        ValidateParentName ();
      }
    }


    private void ParentName_Validating(object sender, CancelEventArgs e)
    {
      ValidateParentName ();
    }


    private void ClassName_Validating(object sender, CancelEventArgs e)
    {
      ValidateClassName ();
    }

    
    private void MergeCheckBox_Validating(object sender, CancelEventArgs e)
    {
      ValidateMergeWithAnotherClass ();
    }  


    private void MergeCheckBox_CheckedChanged(object sender, EventArgs e)
    {
      ValidateMergeWithAnotherClass ();
    }


    private void MergeClass_Validating(object sender, CancelEventArgs e)
    {
      ValidateMergeClass ();
    }


    private void MergeSearchButton_Click(object sender, EventArgs e)
    {
      SelectAPicesClass  sac = new SelectAPicesClass (classes, "", null);
      sac.SelectedClass = PicesClassList.GetUniqueClass (MergeClass.Text, "");
      sac.ShowDialog ();
      if  (sac.SelectedClass != null)
      {
        MergeClass.Text = sac.SelectedClass.Name;
        ValidateMergeClass ();
      }
    }


    private void ClassMaintenance_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  (updateRunning)
      {
        MessageBox.Show ("We are in the middle of updating;  please wait until completed before closing window.");
        e.Cancel = true;
        return;
      }

      mainDbConn.Close ();
      mainDbConn = null;
      GC.Collect ();
    }



    private void ClassMaintenance_SizeChanged(object sender, EventArgs e)
    {
      if  ((lastHeight < 1)  ||  (lastWidth < 1))
        return;

      int  heightDelta = Height - lastHeight;
      int  widthDelta  = Width  - lastWidth;

      ClassMaintenancePanel.Left += widthDelta;
      ClassMaintenancePanel.Height += heightDelta;
      UpdatingProgressBar.Top += heightDelta;
      UpdateLog.Height += heightDelta;
      ClassTree.Width += widthDelta;
      ClassTree.Height += heightDelta;
      lastHeight = Height;
      lastWidth  = Width;
    }
  }
}
