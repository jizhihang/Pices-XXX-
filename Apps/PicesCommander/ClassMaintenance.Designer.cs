namespace PicesCommander
{
  partial class ClassMaintenance
  {
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
      this.components = new System.ComponentModel.Container ();
      this.TitleLabel = new System.Windows.Forms.Label ();
      this.ClassTree = new System.Windows.Forms.TreeView ();
      this.ClassIdLabel = new System.Windows.Forms.Label ();
      this.NameLabel = new System.Windows.Forms.Label ();
      this.ParentLabel = new System.Windows.Forms.Label ();
      this.DescriptionLabel = new System.Windows.Forms.Label ();
      this.ClassId = new System.Windows.Forms.TextBox ();
      this.ClassName = new System.Windows.Forms.TextBox ();
      this.Description = new System.Windows.Forms.TextBox ();
      this.ClassMaintenancePanel = new System.Windows.Forms.Panel ();
      this.MergeHelpTextLabel = new System.Windows.Forms.Label ();
      this.UpdatingProgressBar = new System.Windows.Forms.ProgressBar ();
      this.MergeLabel = new System.Windows.Forms.Label ();
      this.MergeClass = new System.Windows.Forms.TextBox ();
      this.MergeSearchButton = new System.Windows.Forms.Button ();
      this.MergeCheckBox = new System.Windows.Forms.CheckBox ();
      this.ParentName = new System.Windows.Forms.TextBox ();
      this.ParentSearchButton = new System.Windows.Forms.Button ();
      this.UpdateLog = new System.Windows.Forms.TextBox ();
      this.UpdateButton = new System.Windows.Forms.Button ();
      this.DeleteButton = new System.Windows.Forms.Button ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.updateTimer = new System.Windows.Forms.Timer (this.components);
      this.ClassMaintenancePanel.SuspendLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (342, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (169, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Class Maintenance";
      // 
      // ClassTree
      // 
      this.ClassTree.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ClassTree.FullRowSelect = true;
      this.ClassTree.Location = new System.Drawing.Point (12, 55);
      this.ClassTree.Name = "ClassTree";
      this.ClassTree.Size = new System.Drawing.Size (344, 514);
      this.ClassTree.TabIndex = 1;
      this.ClassTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler (this.ClassTree_AfterSelect);
      // 
      // ClassIdLabel
      // 
      this.ClassIdLabel.AutoSize = true;
      this.ClassIdLabel.Location = new System.Drawing.Point (11, 15);
      this.ClassIdLabel.Name = "ClassIdLabel";
      this.ClassIdLabel.Size = new System.Drawing.Size (44, 13);
      this.ClassIdLabel.TabIndex = 2;
      this.ClassIdLabel.Text = "Class Id";
      // 
      // NameLabel
      // 
      this.NameLabel.AutoSize = true;
      this.NameLabel.Location = new System.Drawing.Point (11, 51);
      this.NameLabel.Name = "NameLabel";
      this.NameLabel.Size = new System.Drawing.Size (35, 13);
      this.NameLabel.TabIndex = 3;
      this.NameLabel.Text = "Name";
      // 
      // ParentLabel
      // 
      this.ParentLabel.AutoSize = true;
      this.ParentLabel.Location = new System.Drawing.Point (11, 85);
      this.ParentLabel.Name = "ParentLabel";
      this.ParentLabel.Size = new System.Drawing.Size (38, 13);
      this.ParentLabel.TabIndex = 4;
      this.ParentLabel.Text = "Parent";
      // 
      // DescriptionLabel
      // 
      this.DescriptionLabel.AutoSize = true;
      this.DescriptionLabel.Location = new System.Drawing.Point (11, 120);
      this.DescriptionLabel.Name = "DescriptionLabel";
      this.DescriptionLabel.Size = new System.Drawing.Size (60, 13);
      this.DescriptionLabel.TabIndex = 5;
      this.DescriptionLabel.Text = "Description";
      // 
      // ClassId
      // 
      this.ClassId.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ClassId.Location = new System.Drawing.Point (76, 12);
      this.ClassId.Name = "ClassId";
      this.ClassId.ReadOnly = true;
      this.ClassId.Size = new System.Drawing.Size (100, 22);
      this.ClassId.TabIndex = 6;
      this.ClassId.TabStop = false;
      // 
      // ClassName
      // 
      this.ClassName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ClassName.Location = new System.Drawing.Point (76, 47);
      this.ClassName.Name = "ClassName";
      this.ClassName.Size = new System.Drawing.Size (382, 22);
      this.ClassName.TabIndex = 7;
      this.ClassName.Validating += new System.ComponentModel.CancelEventHandler (this.ClassName_Validating);
      // 
      // Description
      // 
      this.Description.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Description.Location = new System.Drawing.Point (76, 117);
      this.Description.Multiline = true;
      this.Description.Name = "Description";
      this.Description.Size = new System.Drawing.Size (382, 85);
      this.Description.TabIndex = 9;
      // 
      // ClassMaintenancePanel
      // 
      this.ClassMaintenancePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.ClassMaintenancePanel.Controls.Add (this.MergeHelpTextLabel);
      this.ClassMaintenancePanel.Controls.Add (this.UpdatingProgressBar);
      this.ClassMaintenancePanel.Controls.Add (this.MergeLabel);
      this.ClassMaintenancePanel.Controls.Add (this.MergeClass);
      this.ClassMaintenancePanel.Controls.Add (this.MergeSearchButton);
      this.ClassMaintenancePanel.Controls.Add (this.MergeCheckBox);
      this.ClassMaintenancePanel.Controls.Add (this.ParentName);
      this.ClassMaintenancePanel.Controls.Add (this.ParentSearchButton);
      this.ClassMaintenancePanel.Controls.Add (this.UpdateLog);
      this.ClassMaintenancePanel.Controls.Add (this.UpdateButton);
      this.ClassMaintenancePanel.Controls.Add (this.DeleteButton);
      this.ClassMaintenancePanel.Controls.Add (this.ClassId);
      this.ClassMaintenancePanel.Controls.Add (this.Description);
      this.ClassMaintenancePanel.Controls.Add (this.ClassIdLabel);
      this.ClassMaintenancePanel.Controls.Add (this.NameLabel);
      this.ClassMaintenancePanel.Controls.Add (this.ClassName);
      this.ClassMaintenancePanel.Controls.Add (this.ParentLabel);
      this.ClassMaintenancePanel.Controls.Add (this.DescriptionLabel);
      this.ClassMaintenancePanel.Location = new System.Drawing.Point (360, 55);
      this.ClassMaintenancePanel.Name = "ClassMaintenancePanel";
      this.ClassMaintenancePanel.Size = new System.Drawing.Size (473, 514);
      this.ClassMaintenancePanel.TabIndex = 10;
      // 
      // MergeHelpTextLabel
      // 
      this.MergeHelpTextLabel.AutoSize = true;
      this.MergeHelpTextLabel.Location = new System.Drawing.Point (75, 250);
      this.MergeHelpTextLabel.Name = "MergeHelpTextLabel";
      this.MergeHelpTextLabel.Size = new System.Drawing.Size (321, 13);
      this.MergeHelpTextLabel.TabIndex = 20;
      this.MergeHelpTextLabel.Text = "Specify the class that you want merged into current selected class.";
      // 
      // UpdatingProgressBar
      // 
      this.UpdatingProgressBar.Location = new System.Drawing.Point (14, 483);
      this.UpdatingProgressBar.Name = "UpdatingProgressBar";
      this.UpdatingProgressBar.Size = new System.Drawing.Size (444, 20);
      this.UpdatingProgressBar.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
      this.UpdatingProgressBar.TabIndex = 19;
      this.UpdatingProgressBar.Visible = false;
      // 
      // MergeLabel
      // 
      this.MergeLabel.AutoSize = true;
      this.MergeLabel.Location = new System.Drawing.Point (11, 269);
      this.MergeLabel.Name = "MergeLabel";
      this.MergeLabel.Size = new System.Drawing.Size (63, 13);
      this.MergeLabel.TabIndex = 18;
      this.MergeLabel.Text = "Merge From";
      // 
      // MergeClass
      // 
      this.MergeClass.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MergeClass.Location = new System.Drawing.Point (75, 265);
      this.MergeClass.Name = "MergeClass";
      this.MergeClass.ReadOnly = true;
      this.MergeClass.Size = new System.Drawing.Size (332, 22);
      this.MergeClass.TabIndex = 16;
      this.MergeClass.TabStop = false;
      this.MergeClass.Validating += new System.ComponentModel.CancelEventHandler (this.MergeClass_Validating);
      // 
      // MergeSearchButton
      // 
      this.MergeSearchButton.Location = new System.Drawing.Point (417, 264);
      this.MergeSearchButton.Name = "MergeSearchButton";
      this.MergeSearchButton.Size = new System.Drawing.Size (41, 23);
      this.MergeSearchButton.TabIndex = 17;
      this.MergeSearchButton.Text = "Srch";
      this.MergeSearchButton.UseVisualStyleBackColor = true;
      this.MergeSearchButton.Click += new System.EventHandler (this.MergeSearchButton_Click);
      // 
      // MergeCheckBox
      // 
      this.MergeCheckBox.AutoSize = true;
      this.MergeCheckBox.Location = new System.Drawing.Point (78, 219);
      this.MergeCheckBox.Name = "MergeCheckBox";
      this.MergeCheckBox.Size = new System.Drawing.Size (145, 17);
      this.MergeCheckBox.TabIndex = 15;
      this.MergeCheckBox.Text = "Merge with another Class";
      this.MergeCheckBox.UseVisualStyleBackColor = true;
      this.MergeCheckBox.Validating += new System.ComponentModel.CancelEventHandler (this.MergeCheckBox_Validating);
      this.MergeCheckBox.CheckedChanged += new System.EventHandler (this.MergeCheckBox_CheckedChanged);
      // 
      // ParentName
      // 
      this.ParentName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ParentName.Location = new System.Drawing.Point (78, 83);
      this.ParentName.Name = "ParentName";
      this.ParentName.ReadOnly = true;
      this.ParentName.Size = new System.Drawing.Size (329, 22);
      this.ParentName.TabIndex = 8;
      this.ParentName.Validating += new System.ComponentModel.CancelEventHandler (this.ParentName_Validating);
      // 
      // ParentSearchButton
      // 
      this.ParentSearchButton.Location = new System.Drawing.Point (417, 81);
      this.ParentSearchButton.Name = "ParentSearchButton";
      this.ParentSearchButton.Size = new System.Drawing.Size (41, 23);
      this.ParentSearchButton.TabIndex = 14;
      this.ParentSearchButton.Text = "Srch";
      this.ParentSearchButton.UseVisualStyleBackColor = true;
      this.ParentSearchButton.Click += new System.EventHandler (this.ParentSearchButton_Click);
      // 
      // UpdateLog
      // 
      this.UpdateLog.Location = new System.Drawing.Point (14, 335);
      this.UpdateLog.Multiline = true;
      this.UpdateLog.Name = "UpdateLog";
      this.UpdateLog.ReadOnly = true;
      this.UpdateLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
      this.UpdateLog.Size = new System.Drawing.Size (444, 142);
      this.UpdateLog.TabIndex = 13;
      this.UpdateLog.TabStop = false;
      // 
      // UpdateButton
      // 
      this.UpdateButton.Location = new System.Drawing.Point (14, 306);
      this.UpdateButton.Name = "UpdateButton";
      this.UpdateButton.Size = new System.Drawing.Size (75, 23);
      this.UpdateButton.TabIndex = 12;
      this.UpdateButton.Text = "Update";
      this.UpdateButton.UseVisualStyleBackColor = true;
      this.UpdateButton.Click += new System.EventHandler (this.UpdateButton_Click);
      // 
      // DeleteButton
      // 
      this.DeleteButton.Location = new System.Drawing.Point (383, 306);
      this.DeleteButton.Name = "DeleteButton";
      this.DeleteButton.Size = new System.Drawing.Size (75, 23);
      this.DeleteButton.TabIndex = 11;
      this.DeleteButton.TabStop = false;
      this.DeleteButton.Text = "Delete";
      this.DeleteButton.UseVisualStyleBackColor = true;
      this.DeleteButton.Click += new System.EventHandler (this.DeleteButton_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // updateTimer
      // 
      this.updateTimer.Tick += new System.EventHandler (this.updateTimer_Tick);
      // 
      // ClassMaintenance
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (839, 576);
      this.Controls.Add (this.ClassMaintenancePanel);
      this.Controls.Add (this.ClassTree);
      this.Controls.Add (this.TitleLabel);
      this.MinimumSize = new System.Drawing.Size (855, 614);
      this.Name = "ClassMaintenance";
      this.Text = "ClassMaintenance";
      this.Load += new System.EventHandler (this.ClassMaintenance_Load);
      this.SizeChanged += new System.EventHandler (this.ClassMaintenance_SizeChanged);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.ClassMaintenance_FormClosing);
      this.ClassMaintenancePanel.ResumeLayout (false);
      this.ClassMaintenancePanel.PerformLayout ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.TreeView ClassTree;
    private System.Windows.Forms.Label ClassIdLabel;
    private System.Windows.Forms.Label NameLabel;
    private System.Windows.Forms.Label ParentLabel;
    private System.Windows.Forms.Label DescriptionLabel;
    private System.Windows.Forms.TextBox ClassId;
    private System.Windows.Forms.TextBox ClassName;
    private System.Windows.Forms.TextBox Description;
    private System.Windows.Forms.Panel ClassMaintenancePanel;
    private System.Windows.Forms.Button UpdateButton;
    private System.Windows.Forms.Button DeleteButton;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Timer updateTimer;
    private System.Windows.Forms.TextBox UpdateLog;
    private System.Windows.Forms.TextBox ParentName;
    private System.Windows.Forms.Button ParentSearchButton;
    private System.Windows.Forms.TextBox MergeClass;
    private System.Windows.Forms.Button MergeSearchButton;
    private System.Windows.Forms.CheckBox MergeCheckBox;
    private System.Windows.Forms.Label MergeLabel;
    private System.Windows.Forms.ProgressBar UpdatingProgressBar;
    private System.Windows.Forms.Label MergeHelpTextLabel;
  }
}