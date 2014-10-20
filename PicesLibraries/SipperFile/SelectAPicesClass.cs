using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;

namespace SipperFile
{
  public partial class SelectAPicesClass : Form
  {
    private  PicesClass  selectedClass = null;
    private  bool        selectionMade = false;  /**<  Indicates that the user made a positve effort to select a class.  */

    public  PicesClass  SelectedClass  
    {
      get {return selectedClass;}
      set 
      {
        selectedClass = value;
        SetSelectTreeNode (MLClassChoices.Nodes);
      }
    }

    public  bool  SelectionMade  {get {return  selectionMade;}}
  
    private Font  activeClassFont = null;

    private  int  lastWidth  = 0;
    private  int  lastHeight = 0;

    private  String  rootDir;

    private  PicesClassList  activeClasses = null;


    private  TreeNode  selectedNode = null;

    /// <summary>
    /// 
    /// </summary>
    /// <param name="classes"></param>
    /// <param name="_rootDir">Root directory of training library that contains examples for the classes.</param>
    /// <param name="_activeClasses">List of classes that current classifer supports;  these classes will be highlighted.</param>
    public SelectAPicesClass (PicesClassList  classes,
                              String          _rootDir,
                              PicesClassList  _activeClasses
                             )
    {
      activeClasses = _activeClasses;
      rootDir = OSservices.AddSlash (_rootDir);
      InitializeComponent();

      activeClassFont = new Font (MLClassChoices.Font.FontFamily,MLClassChoices.Font.SizeInPoints + 1, FontStyle.Bold);

      selectedNode = null;

      PicesClass  root = classes.RootNode;
      bool activeClass = false;
      AddSubTreeToClassChoser (root, MLClassChoices.Nodes, ref activeClass, 0);
      if  (selectedNode != null)
        MLClassChoices.SelectedNode = selectedNode;

      MLClassChoices.ExpandAll ();

      //foreach  (PicesClass pc in root.Children)
      //  AddSubTreeToClassChoser (pc, MLClassChoices.Nodes, ref activeClass);
    }


    private  void  SetSelectTreeNode (TreeNodeCollection nodes)
    {
      if  ((nodes == null)  ||  (selectedClass == null))
        return;

      selectedNode = null;

      foreach (TreeNode tn in nodes)
      {
        if  (tn.Text == selectedClass.Name)
        {
          selectedNode = tn;
          MLClassChoices.SelectedNode = selectedNode;
          return;
        }
        else if  (tn.Nodes != null)
        {
          SetSelectTreeNode (tn.Nodes);
          if  (selectedNode != null)
            return;
        }
      }
    }  /* SetSelectTreeNode */


        
    private  void  AddSubTreeToClassChoser (PicesClass         pc, 
                                            TreeNodeCollection nodes,
                                            ref bool           activeClass,
                                            int                depth
                                           )
    {
      if  (depth > 10)
        return;

      bool  iAmActive = false;
      if (activeClasses != null)
        iAmActive = (activeClasses.LookUpIndex (pc) >= 0);
      bool  oneOfMyChildrenAreActive = false;

      TreeNode  tn = new TreeNode (pc.Name);
      nodes.Add (tn);
      if  (pc == selectedClass)
        selectedNode = tn;

      if  (pc.Children != null)
      {
        foreach  (PicesClass  child  in  pc.Children)
        {
          if  (child == pc)
            continue;

          if  (pc.ParentLoopCheck (child))
            continue;

          bool  childActive = false;
          AddSubTreeToClassChoser (child, tn.Nodes, ref childActive, depth + 1);
          if  (childActive)
            oneOfMyChildrenAreActive = true;
        }
      }

      if  (iAmActive  ||  oneOfMyChildrenAreActive)
      {
        activeClass = true;
        if  (iAmActive)
        {
          tn.ForeColor = Color.DarkGreen;
          tn.NodeFont = activeClassFont;
        }
        else if  (oneOfMyChildrenAreActive)
          tn.ForeColor = Color.Green;
      }
      else
      {
        tn.ForeColor = Color.Gray;
      }
    }



    private void CancelButon_Click(object sender, EventArgs e)
    {
      selectedClass = null;
      Close ();
      return;
    }


    private void MLClassChoices_NodeMouseDoubleClick (object sender, TreeNodeMouseClickEventArgs e)
    {
      if  (e.Button == MouseButtons.Left)
      {
        selectionMade = true;
        selectedClass = PicesClassList.GetUniqueClass (e.Node.Text, "");
        Close ();
        return;
      }
      else if  (e.Button == MouseButtons.Right)
      {
        if  (String.IsNullOrEmpty (rootDir))
          return;

        String  fullDirPath = rootDir + e.Node.Text;

        DisplayPicesImages  dpi = new DisplayPicesImages (fullDirPath);
        dpi.ShowDialog ();
        dpi.Dispose ();
        dpi = null;
      }
    }


    private void SelectAPicesClass_Resize(object sender, EventArgs e)
    {
      if  (lastHeight == 0)
        return;

      int  deltaHeight = Height - lastHeight;
      int  deltaWidth  = Width  - lastWidth;

      CancelButon.Top += deltaHeight;
      CancelButon.Left += deltaWidth;
      MLClassChoices.Width += deltaWidth;
      MLClassChoices.Height += deltaHeight;

      SelectClassButton.Top += deltaHeight;

      lastHeight = Height;
      lastWidth  = Width;
    }

    private void SelectClassButton_Click (object sender, EventArgs e)
    {
      if  (MLClassChoices.SelectedNode != null)
      {
        selectionMade = true;
        selectedClass = PicesClassList.GetUniqueClass (MLClassChoices.SelectedNode.Text, "");
        Close ();
        return;
      }
    }


    private void SelectAPicesClass_Load (object sender, EventArgs e)
    {
      lastHeight = Height;
      lastWidth  = Width;
    }
  }
}