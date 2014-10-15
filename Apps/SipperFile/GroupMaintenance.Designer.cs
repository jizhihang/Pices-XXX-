namespace SipperFile
{
  partial class GroupMaintenance
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose (bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose ();
      }
      base.Dispose (disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent ()
    {
      this.ImageGroupGrid = new System.Windows.Forms.DataGridView ();
      this.GroupId = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.GroupName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.GroupDescription = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.ImageCount = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.Delete = new System.Windows.Forms.DataGridViewButtonColumn ();
      ((System.ComponentModel.ISupportInitialize)(this.ImageGroupGrid)).BeginInit ();
      this.SuspendLayout ();
      // 
      // ImageGroupGrid
      // 
      this.ImageGroupGrid.AllowUserToAddRows = false;
      this.ImageGroupGrid.AllowUserToOrderColumns = true;
      this.ImageGroupGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.ImageGroupGrid.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.GroupId,
            this.GroupName,
            this.GroupDescription,
            this.ImageCount,
            this.Delete});
      this.ImageGroupGrid.EditMode = System.Windows.Forms.DataGridViewEditMode.EditProgrammatically;
      this.ImageGroupGrid.Location = new System.Drawing.Point (12, 28);
      this.ImageGroupGrid.MultiSelect = false;
      this.ImageGroupGrid.Name = "ImageGroupGrid";
      this.ImageGroupGrid.RowHeadersVisible = false;
      this.ImageGroupGrid.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
      this.ImageGroupGrid.Size = new System.Drawing.Size (966, 523);
      this.ImageGroupGrid.TabIndex = 0;
      this.ImageGroupGrid.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler (this.ImageGroupGrid_CellClick);
      // 
      // GroupId
      // 
      this.GroupId.HeaderText = "Id";
      this.GroupId.Name = "GroupId";
      this.GroupId.ReadOnly = true;
      this.GroupId.Width = 50;
      // 
      // GroupName
      // 
      this.GroupName.HeaderText = "Name";
      this.GroupName.Name = "GroupName";
      this.GroupName.ReadOnly = true;
      this.GroupName.Width = 250;
      // 
      // GroupDescription
      // 
      this.GroupDescription.HeaderText = "Description";
      this.GroupDescription.Name = "GroupDescription";
      this.GroupDescription.ReadOnly = true;
      this.GroupDescription.Width = 500;
      // 
      // ImageCount
      // 
      this.ImageCount.HeaderText = "ImageCount";
      this.ImageCount.Name = "ImageCount";
      this.ImageCount.ReadOnly = true;
      // 
      // Delete
      // 
      this.Delete.HeaderText = "Delete";
      this.Delete.Name = "Delete";
      this.Delete.Width = 50;
      // 
      // GroupMaintenance
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (990, 563);
      this.Controls.Add (this.ImageGroupGrid);
      this.Name = "GroupMaintenance";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "Group Maintenance";
      this.Load += new System.EventHandler (this.GroupMaintenance_Load);
      this.Resize += new System.EventHandler (this.GroupMaintenance_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.ImageGroupGrid)).EndInit ();
      this.ResumeLayout (false);

    }

    #endregion

    private System.Windows.Forms.DataGridView ImageGroupGrid;
    private System.Windows.Forms.DataGridViewTextBoxColumn GroupId;
    private System.Windows.Forms.DataGridViewTextBoxColumn GroupName;
    private System.Windows.Forms.DataGridViewTextBoxColumn GroupDescription;
    private System.Windows.Forms.DataGridViewTextBoxColumn ImageCount;
    private System.Windows.Forms.DataGridViewButtonColumn Delete;
  }
}