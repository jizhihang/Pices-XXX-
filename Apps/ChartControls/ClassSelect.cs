using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

using  PicesInterface;


namespace ChartControls
{
  public partial class ClassSelect : UserControl
  {
    public delegate void ClassChangedHandler ();

    PicesClassList  activeClasses     = null;
    PicesClassList  classes           = null;
    PicesClass      origSelectedClass = null;
    String          rootDir           = null;
    PicesClass      selectedClass     = null;


    private  int heightLast = 0;
    private  int widthLast  = 0;


    public ClassSelect (PicesClass      _selectedClass,
                        PicesClassList  _classes,
                        String          _rootDir,
                        PicesClassList  _activeClasses
                       )
    {
      heightLast = Height;
      widthLast  = Width;

      this.SizeChanged += new System.EventHandler (this.OnSizeChanged);

      selectedClass     = _selectedClass;
      origSelectedClass = _selectedClass;
      classes           = _classes;
      rootDir           = _rootDir;
      activeClasses     = _activeClasses;
      InitializeComponent ();
    }



    private void OnSizeChanged (object sender, EventArgs e)
    {
      int  heightDelta = Height - heightLast;
      int  widthDelta  = Width  - widthLast;

      Browse.Left += widthDelta;
      ClassName.Width += widthDelta;

      heightLast = Height;
      widthLast  = Width;
    }

    
    /// <summary>
    /// Did the user change the specified class?
    /// </summary>
    [Bindable (true), Category ("Class Selection"),
    Description ("Sepcifies if user has changed the selected class.")]
    public bool SelectedClassChanged
    {
      get {return selectedClass != origSelectedClass;}
    } 



    /// <summary>
    /// Did the user change the specified class?
    /// </summary>
    [Bindable (true), Category ("Class Selection"),
    Description ("Sepcifies selected class.")]
    public PicesClass SelectedClass
    {
      get {return selectedClass;}
    }




    private void Browse_Click (object sender, EventArgs e)
    {
      SelectAPicesClass  sapc = new SelectAPicesClass (classes, rootDir, activeClasses);
      sapc.Select ();
      if  (sapc.SelectionMade)
      {
        ClassName.Text = sapc.SelectedClass.Name;
        if  (sapc.SelectedClass != selectedClass)
          OnSelectedLabelChanged ();
      }
    }

  
    // Declare the event, which is associated with our
    // delegate SelectedLabelChanged(). Add some attributes
    // for the Visual C# control property.
    [Category ("Action")]
    [Description ("Fires when the Selected label changes.")]
    public event ClassChangedHandler SelectedLabelChanged;



    protected virtual void OnSelectedLabelChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if  (Enabled  &&  (SelectedLabelChanged != null))
      {
        Enabled = false;
        SelectedLabelChanged ();  // Notify Subscribers
        Enabled = true;
      }
    }
















    public class SelectAPicesClass : Form
    {
      private  PicesClass  selectedClass = null;
      private  bool        selectionMade = false;  /**<  Indicates that the user made a positve effort to select a class.  */

      public  PicesClass  SelectedClass  
      {
        get {return selectedClass;}
        set 
        {
          selectedClass = value;
          SetSelectTreeNode (ImageClassChoices.Nodes);
        }
      }

      public  bool  SelectionMade  {get {return  selectionMade;}}
    
      private Font  activeClassFont = null;

      private  int  lastWidth  = 0;
      private  int  lastHeight = 0;

      private  String  rootDir;

      private  PicesClassList  activeClasses = null;

      /// <summary>
      /// Required designer variable.
      /// </summary>
      private System.ComponentModel.IContainer components = null;

      /// <summary>
      /// Clean up any resources being used.
      /// </summary>
      /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
      protected override void Dispose(bool disposing)
      {
        if (disposing && (components != null))
        {
          components.Dispose();
        }
        base.Dispose(disposing);
      }

      #region Windows Form Designer generated code

      /// <summary>
      /// Required method for Designer support - do not modify
      /// the contents of this method with the code editor.
      /// </summary>
      private void InitializeComponent()
      {
        this.ImageClassChoices = new System.Windows.Forms.TreeView ();
        this.CancelButon = new System.Windows.Forms.Button ();
        this.SelectClassButton = new System.Windows.Forms.Button ();
        this.SuspendLayout ();
        // 
        // ImageClassChoices
        // 
        this.ImageClassChoices.Location = new System.Drawing.Point (13, 14);
        this.ImageClassChoices.Name = "ImageClassChoices";
        this.ImageClassChoices.Size = new System.Drawing.Size (363, 468);
        this.ImageClassChoices.TabIndex = 0;
        this.ImageClassChoices.NodeMouseDoubleClick += new System.Windows.Forms.TreeNodeMouseClickEventHandler (this.ImageClassChoices_NodeMouseDoubleClick);
        // 
        // CancelButon
        // 
        this.CancelButon.Location = new System.Drawing.Point (301, 494);
        this.CancelButon.Name = "CancelButon";
        this.CancelButon.Size = new System.Drawing.Size (75, 23);
        this.CancelButon.TabIndex = 1;
        this.CancelButon.Text = "Cancel";
        this.CancelButon.UseVisualStyleBackColor = true;
        this.CancelButon.Click += new System.EventHandler (this.CancelButon_Click);
        // 
        // SelectClassButton
        // 
        this.SelectClassButton.Location = new System.Drawing.Point (13, 494);
        this.SelectClassButton.Name = "SelectClassButton";
        this.SelectClassButton.Size = new System.Drawing.Size (75, 23);
        this.SelectClassButton.TabIndex = 2;
        this.SelectClassButton.Text = "Select Class";
        this.SelectClassButton.UseVisualStyleBackColor = true;
        this.SelectClassButton.Click += new System.EventHandler (this.SelectClassButton_Click);
        // 
        // SelectAPicesClass
        // 
        this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
        this.ClientSize = new System.Drawing.Size (388, 529);
        this.Controls.Add (this.SelectClassButton);
        this.Controls.Add (this.CancelButon);
        this.Controls.Add (this.ImageClassChoices);
        this.Name = "SelectAPicesClass";
        this.Text = "SelectAPicesClass";
        this.Load += new System.EventHandler (this.SelectAPicesClass_Load);
        this.Resize += new System.EventHandler (this.SelectAPicesClass_Resize);
        this.ResumeLayout (false);

      }

      #endregion

      private System.Windows.Forms.TreeView ImageClassChoices;
      private System.Windows.Forms.Button CancelButon;
      private System.Windows.Forms.Button SelectClassButton;



      
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

        activeClassFont = new Font (ImageClassChoices.Font.FontFamily,ImageClassChoices.Font.SizeInPoints + 1, FontStyle.Bold);

        selectedNode = null;

        PicesClass  root = classes.RootNode;
        bool activeClass = false;
        AddSubTreeToClassChoser (root, ImageClassChoices.Nodes, ref activeClass);
        if  (selectedNode != null)
          ImageClassChoices.SelectedNode = selectedNode;

        ImageClassChoices.ExpandAll ();

        //foreach  (PicesClass pc in root.Children)
        //  AddSubTreeToClassChoser (pc, ImageClassChoices.Nodes, ref activeClass);
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
            ImageClassChoices.SelectedNode = selectedNode;
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
                                              ref bool           activeClass
                                             )
      {
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
            bool  childActive = false;
            AddSubTreeToClassChoser (child, tn.Nodes, ref childActive);
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


      private void ImageClassChoices_NodeMouseDoubleClick (object sender, TreeNodeMouseClickEventArgs e)
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

          //DisplayPicesImages  dpi = new DisplayPicesImages (fullDirPath);
          //dpi.ShowDialog ();
          //dpi.Dispose ();
          //dpi = null;
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
        ImageClassChoices.Width += deltaWidth;
        ImageClassChoices.Height += deltaHeight;

        SelectClassButton.Top += deltaHeight;

        lastHeight = Height;
        lastWidth  = Width;
      }

      private void SelectClassButton_Click (object sender, EventArgs e)
      {
        if  (ImageClassChoices.SelectedNode != null)
        {
          selectionMade = true;
          selectedClass = PicesClassList.GetUniqueClass (ImageClassChoices.SelectedNode.Text, "");
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
}

