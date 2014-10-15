using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;
using  SipperFile;


namespace PicesCommander
{
  public partial class AssignSelectedImagesToGroup : Form
  {
    private  PicesDataBase           dbConn = null;
    private  String                  groupName = "";
    private  bool                    newGroupCreated = false;
    private  PicesRunLog             runLog = null;
    private  PicesDataBaseImageList  selImages = null;
    private  bool                    validationErrorFound = true;

    public  bool  NewGroupCreated  {get {return newGroupCreated;}}

    public  String NewGroupName    {get {return groupName;}}


    public AssignSelectedImagesToGroup (PicesDataBaseImageList  _selImages,
                                        String                  _suggestedGroupName
                                       )
    {
      selImages = _selImages;
      InitializeComponent ();
      GroupName.Text = CleanUpGroupName (_suggestedGroupName);

      runLog  = new PicesRunLog ();
      dbConn  = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      if  (dbConn == null)
        dbConn = new PicesDataBase (runLog);

      if  (selImages == null)
      {
        MessageBox.Show (this, "Assign Group to Selected Images", "There are no images curently selected.", MessageBoxButtons.OK);
        Close ();
      }
    }


    private  String  CleanUpGroupName (String  suggestedName)
    {
      groupName = "";
      for  (int x = 0;  x < suggestedName.Length; ++x)
      {
        char c = suggestedName[x];
        if  (((c >= 'A')  &&  (c <= 'Z'))  ||
             ((c >= 'a')  &&  (c <= 'z'))  ||
             ((c >= '0')  &&  (c <= '9'))  ||
             (c == '-')                    ||
             (c == '_')
            )
          groupName += c.ToString ();
      }
      return  groupName;
    }




    private void CancelButon_Click (object sender, EventArgs e)
    {
      Close ();
    }


    private  void  ValidateGroupName ()
    {
      errorProvider1.SetError (GroupName, "");
      String   errorDesc = "";
      bool     valid = false;

      groupName = GroupName.Text;
      PicesDataBaseImageGroup.ValidName (ref groupName, ref errorDesc, ref valid);
      if  (!valid)
      {
        errorProvider1.SetError (GroupName, errorDesc);
        validationErrorFound = true;
      }

      if  (dbConn.ImageGroupLoad (groupName) != null)
      {
        errorDesc = "This Group name already exists.";
        errorProvider1.SetError (GroupName, errorDesc);
        validationErrorFound = true;
      }

      GroupName.Text = groupName;
    }  /* ValidateGroupName */



    private  void  UpdateDatabase ()
    {
      PicesDataBaseImageGroup ig = new PicesDataBaseImageGroup (-1, GroupName.Text, Description.Text, 0);
      dbConn.ImageGroupInsert (ig);
      if  (!dbConn.Valid ())
      {
        MessageBox.Show (this, 
                         "Error Creating new Image Group" + "\n" + dbConn.LastErrorDesc (), 
                         "Assign Selected Images to Group", 
                         MessageBoxButtons.OK
                        );
      }
      else
      {
        int zed = 0;
        while  (zed < selImages.Count)
        {
          PicesDataBaseImageList subSet = new PicesDataBaseImageList ();
          while ((subSet.Count  < 100) && (zed < selImages.Count))
          {
            subSet.Add (selImages[zed]);
            ++zed;
          }
          dbConn.ImageGroupEntriesInsert (ig.ImageGroupId, subSet);
          if (!dbConn.Valid ())
            break;
        }
        if  (dbConn.Valid ())
        {
          newGroupCreated = true;
          MessageBox.Show (this, "Group[" + GroupName.Text + "] Created successfully.", "Assign Selected Images to Group", MessageBoxButtons.OK);
        }
        else
        {
          MessageBox.Show (this, "Error Creating new Image Group" + "\n" + dbConn.LastErrorDesc (), "Assign Selected Images to Group", MessageBoxButtons.OK);
        }
      }
    }  /* UpdateDatabase */




    private void MakeAssignmentButton_Click (object sender, EventArgs e)
    {
      validationErrorFound = false;
      ValidateGroupName ();
      if  (validationErrorFound)
        return;

      UpdateDatabase ();
      Close ();
    }



    private void AssignSelectedImagesToGroup_FormClosing (object sender, FormClosingEventArgs e)
    {
      //dbConn.Close ();
      dbConn = null;
      GC.Collect ();
    }
  }
}
