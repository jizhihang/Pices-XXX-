namespace PicesCommander
{
  partial class AssignSelectedImagesToGroup
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
      this.components = new System.ComponentModel.Container ();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager (typeof (AssignSelectedImagesToGroup));
      this.MakeAssignmentButton = new System.Windows.Forms.Button ();
      this.CancelButon = new System.Windows.Forms.Button ();
      this.GroupName = new System.Windows.Forms.TextBox ();
      this.GroupNameLabel = new System.Windows.Forms.Label ();
      this.textBox1 = new System.Windows.Forms.TextBox ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.Description = new System.Windows.Forms.TextBox ();
      this.DescriptionLabel = new System.Windows.Forms.Label ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // MakeAssignmentButton
      // 
      this.MakeAssignmentButton.Location = new System.Drawing.Point (12, 317);
      this.MakeAssignmentButton.Name = "MakeAssignmentButton";
      this.MakeAssignmentButton.Size = new System.Drawing.Size (111, 23);
      this.MakeAssignmentButton.TabIndex = 0;
      this.MakeAssignmentButton.Text = "Make Assignment";
      this.MakeAssignmentButton.UseVisualStyleBackColor = true;
      this.MakeAssignmentButton.Click += new System.EventHandler (this.MakeAssignmentButton_Click);
      // 
      // CancelButon
      // 
      this.CancelButon.Location = new System.Drawing.Point (464, 316);
      this.CancelButon.Name = "CancelButon";
      this.CancelButon.Size = new System.Drawing.Size (75, 23);
      this.CancelButon.TabIndex = 1;
      this.CancelButon.Text = "Cancel";
      this.CancelButon.UseVisualStyleBackColor = true;
      this.CancelButon.Click += new System.EventHandler (this.CancelButon_Click);
      // 
      // GroupName
      // 
      this.GroupName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.GroupName.Location = new System.Drawing.Point (12, 139);
      this.GroupName.Name = "GroupName";
      this.GroupName.Size = new System.Drawing.Size (527, 22);
      this.GroupName.TabIndex = 2;
      // 
      // GroupNameLabel
      // 
      this.GroupNameLabel.AutoSize = true;
      this.GroupNameLabel.Location = new System.Drawing.Point (9, 123);
      this.GroupNameLabel.Name = "GroupNameLabel";
      this.GroupNameLabel.Size = new System.Drawing.Size (67, 13);
      this.GroupNameLabel.TabIndex = 3;
      this.GroupNameLabel.Text = "Group Name";
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point (12, 44);
      this.textBox1.Multiline = true;
      this.textBox1.Name = "textBox1";
      this.textBox1.ReadOnly = true;
      this.textBox1.Size = new System.Drawing.Size (527, 50);
      this.textBox1.TabIndex = 4;
      this.textBox1.Text = resources.GetString ("textBox1.Text");
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // Description
      // 
      this.Description.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Description.Location = new System.Drawing.Point (12, 191);
      this.Description.Multiline = true;
      this.Description.Name = "Description";
      this.Description.Size = new System.Drawing.Size (527, 74);
      this.Description.TabIndex = 5;
      // 
      // DescriptionLabel
      // 
      this.DescriptionLabel.AutoSize = true;
      this.DescriptionLabel.Location = new System.Drawing.Point (9, 175);
      this.DescriptionLabel.Name = "DescriptionLabel";
      this.DescriptionLabel.Size = new System.Drawing.Size (60, 13);
      this.DescriptionLabel.TabIndex = 6;
      this.DescriptionLabel.Text = "Description";
      // 
      // AssignSelectedImagesToGroup
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (556, 346);
      this.Controls.Add (this.DescriptionLabel);
      this.Controls.Add (this.Description);
      this.Controls.Add (this.textBox1);
      this.Controls.Add (this.GroupNameLabel);
      this.Controls.Add (this.GroupName);
      this.Controls.Add (this.CancelButon);
      this.Controls.Add (this.MakeAssignmentButton);
      this.Name = "AssignSelectedImagesToGroup";
      this.Text = "Assign Selected Images to Group";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.AssignSelectedImagesToGroup_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Button MakeAssignmentButton;
    private System.Windows.Forms.Button CancelButon;
    private System.Windows.Forms.TextBox GroupName;
    private System.Windows.Forms.Label GroupNameLabel;
    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.Label DescriptionLabel;
    private System.Windows.Forms.TextBox Description;
  }
}