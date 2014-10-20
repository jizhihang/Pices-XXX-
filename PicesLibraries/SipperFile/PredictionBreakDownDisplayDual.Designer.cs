namespace SipperFile
{
  partial class PredictionBreakDownDisplayDual
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
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle ();
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle2 = new System.Windows.Forms.DataGridViewCellStyle ();
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle3 = new System.Windows.Forms.DataGridViewCellStyle ();
      System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle4 = new System.Windows.Forms.DataGridViewCellStyle ();
      this.Classifier1Probabilities = new System.Windows.Forms.DataGridView ();
      this.ClassName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.PredProbability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.PredVotes = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.DisplayFeatureDataButton = new System.Windows.Forms.Button ();
      this.Classifier2Probabilities = new System.Windows.Forms.DataGridView ();
      this.Classifier2ClassName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.Classifier2PredProbability = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.Classifier2Votes = new System.Windows.Forms.DataGridViewTextBoxColumn ();
      this.Classifier1Name = new System.Windows.Forms.TextBox ();
      this.Classifier2Name = new System.Windows.Forms.TextBox ();
      ((System.ComponentModel.ISupportInitialize)(this.Classifier1Probabilities)).BeginInit ();
      ((System.ComponentModel.ISupportInitialize)(this.Classifier2Probabilities)).BeginInit ();
      this.SuspendLayout ();
      // 
      // Classifier1Probabilities
      // 
      this.Classifier1Probabilities.AllowUserToAddRows = false;
      this.Classifier1Probabilities.AllowUserToDeleteRows = false;
      this.Classifier1Probabilities.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.Classifier1Probabilities.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.ClassName,
            this.PredProbability,
            this.PredVotes});
      this.Classifier1Probabilities.Location = new System.Drawing.Point (12, 38);
      this.Classifier1Probabilities.Name = "Classifier1Probabilities";
      this.Classifier1Probabilities.ReadOnly = true;
      this.Classifier1Probabilities.RowHeadersVisible = false;
      this.Classifier1Probabilities.RowTemplate.Height = 24;
      this.Classifier1Probabilities.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
      this.Classifier1Probabilities.Size = new System.Drawing.Size (440, 549);
      this.Classifier1Probabilities.TabIndex = 5;
      this.Classifier1Probabilities.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.ClassificationProbabilities_MouseDoubleClick);
      this.Classifier1Probabilities.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ClassificationProbabilities_CellFormatting);
      // 
      // ClassName
      // 
      this.ClassName.HeaderText = "Class Name";
      this.ClassName.Name = "ClassName";
      this.ClassName.ReadOnly = true;
      this.ClassName.Width = 260;
      // 
      // PredProbability
      // 
      dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.PredProbability.DefaultCellStyle = dataGridViewCellStyle1;
      this.PredProbability.HeaderText = "Probabilty";
      this.PredProbability.Name = "PredProbability";
      this.PredProbability.ReadOnly = true;
      this.PredProbability.Width = 80;
      // 
      // PredVotes
      // 
      dataGridViewCellStyle2.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.PredVotes.DefaultCellStyle = dataGridViewCellStyle2;
      this.PredVotes.HeaderText = "Votes";
      this.PredVotes.Name = "PredVotes";
      this.PredVotes.ReadOnly = true;
      this.PredVotes.Width = 80;
      // 
      // DisplayFeatureDataButton
      // 
      this.DisplayFeatureDataButton.Location = new System.Drawing.Point (408, 593);
      this.DisplayFeatureDataButton.Name = "DisplayFeatureDataButton";
      this.DisplayFeatureDataButton.Size = new System.Drawing.Size (115, 23);
      this.DisplayFeatureDataButton.TabIndex = 11;
      this.DisplayFeatureDataButton.Text = "Display Feature Data";
      this.DisplayFeatureDataButton.UseVisualStyleBackColor = true;
      this.DisplayFeatureDataButton.Click += new System.EventHandler (this.DisplayFeatureDataButton_Click);
      // 
      // Classifier2Probabilities
      // 
      this.Classifier2Probabilities.AllowUserToAddRows = false;
      this.Classifier2Probabilities.AllowUserToDeleteRows = false;
      this.Classifier2Probabilities.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
      this.Classifier2Probabilities.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.Classifier2ClassName,
            this.Classifier2PredProbability,
            this.Classifier2Votes});
      this.Classifier2Probabilities.Location = new System.Drawing.Point (469, 38);
      this.Classifier2Probabilities.Name = "Classifier2Probabilities";
      this.Classifier2Probabilities.ReadOnly = true;
      this.Classifier2Probabilities.RowHeadersVisible = false;
      this.Classifier2Probabilities.RowTemplate.Height = 24;
      this.Classifier2Probabilities.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
      this.Classifier2Probabilities.Size = new System.Drawing.Size (440, 549);
      this.Classifier2Probabilities.TabIndex = 12;
      this.Classifier2Probabilities.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler (this.ClassificationProbabilities_MouseDoubleClick);
      this.Classifier2Probabilities.CellFormatting += new System.Windows.Forms.DataGridViewCellFormattingEventHandler (this.ClassificationProbabilities_CellFormatting);
      // 
      // Classifier2ClassName
      // 
      this.Classifier2ClassName.HeaderText = "Class Name";
      this.Classifier2ClassName.Name = "Classifier2ClassName";
      this.Classifier2ClassName.ReadOnly = true;
      this.Classifier2ClassName.Width = 260;
      // 
      // Classifier2PredProbability
      // 
      dataGridViewCellStyle3.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.Classifier2PredProbability.DefaultCellStyle = dataGridViewCellStyle3;
      this.Classifier2PredProbability.HeaderText = "Probabilty";
      this.Classifier2PredProbability.Name = "Classifier2PredProbability";
      this.Classifier2PredProbability.ReadOnly = true;
      this.Classifier2PredProbability.Width = 80;
      // 
      // Classifier2Votes
      // 
      dataGridViewCellStyle4.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
      this.Classifier2Votes.DefaultCellStyle = dataGridViewCellStyle4;
      this.Classifier2Votes.HeaderText = "Votes";
      this.Classifier2Votes.Name = "Classifier2Votes";
      this.Classifier2Votes.ReadOnly = true;
      this.Classifier2Votes.Width = 80;
      // 
      // Classifier1Name
      // 
      this.Classifier1Name.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Classifier1Name.Location = new System.Drawing.Point (12, 12);
      this.Classifier1Name.Name = "Classifier1Name";
      this.Classifier1Name.ReadOnly = true;
      this.Classifier1Name.Size = new System.Drawing.Size (440, 22);
      this.Classifier1Name.TabIndex = 13;
      // 
      // Classifier2Name
      // 
      this.Classifier2Name.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Classifier2Name.Location = new System.Drawing.Point (469, 12);
      this.Classifier2Name.Name = "Classifier2Name";
      this.Classifier2Name.ReadOnly = true;
      this.Classifier2Name.Size = new System.Drawing.Size (440, 22);
      this.Classifier2Name.TabIndex = 14;
      // 
      // PredictionBreakDownDisplayDual
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (924, 627);
      this.Controls.Add (this.Classifier2Name);
      this.Controls.Add (this.Classifier1Name);
      this.Controls.Add (this.Classifier2Probabilities);
      this.Controls.Add (this.DisplayFeatureDataButton);
      this.Controls.Add (this.Classifier1Probabilities);
      this.Name = "PredictionBreakDownDisplayDual";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "Prediction Breakdown Dual Classifier";
      this.Load += new System.EventHandler (this.ImageListDisplay_Load);
      ((System.ComponentModel.ISupportInitialize)(this.Classifier1Probabilities)).EndInit ();
      ((System.ComponentModel.ISupportInitialize)(this.Classifier2Probabilities)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.DataGridView Classifier1Probabilities;
    private System.Windows.Forms.Button DisplayFeatureDataButton;
    private System.Windows.Forms.DataGridView Classifier2Probabilities;
    private System.Windows.Forms.DataGridViewTextBoxColumn ClassName;
    private System.Windows.Forms.DataGridViewTextBoxColumn PredProbability;
    private System.Windows.Forms.DataGridViewTextBoxColumn PredVotes;
    private System.Windows.Forms.TextBox Classifier1Name;
    private System.Windows.Forms.TextBox Classifier2Name;
    private System.Windows.Forms.DataGridViewTextBoxColumn Classifier2ClassName;
    private System.Windows.Forms.DataGridViewTextBoxColumn Classifier2PredProbability;
    private System.Windows.Forms.DataGridViewTextBoxColumn Classifier2Votes;
  }
}