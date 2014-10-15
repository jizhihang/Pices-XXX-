namespace SipperFile
{
    partial class LogEntryViewer
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
          this.FieldsGridView = new System.Windows.Forms.DataGridView ();
          this.FieldName = new System.Windows.Forms.DataGridViewTextBoxColumn ();
          this.FieldValue = new System.Windows.Forms.DataGridViewTextBoxColumn ();
          this.CmdLine = new System.Windows.Forms.TextBox ();
          this.CommandLineLanel = new System.Windows.Forms.Label ();
          this.CloseButton = new System.Windows.Forms.Button ();
          ((System.ComponentModel.ISupportInitialize)(this.FieldsGridView)).BeginInit ();
          this.SuspendLayout ();
          // 
          // FieldsGridView
          // 
          this.FieldsGridView.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
          this.FieldsGridView.Columns.AddRange (new System.Windows.Forms.DataGridViewColumn[] {
            this.FieldName,
            this.FieldValue});
          this.FieldsGridView.Location = new System.Drawing.Point (12, 16);
          this.FieldsGridView.Name = "FieldsGridView";
          this.FieldsGridView.RowHeadersVisible = false;
          this.FieldsGridView.Size = new System.Drawing.Size (933, 409);
          this.FieldsGridView.TabIndex = 5;
          // 
          // FieldName
          // 
          this.FieldName.HeaderText = "Field Name";
          this.FieldName.Name = "FieldName";
          this.FieldName.ReadOnly = true;
          this.FieldName.Width = 250;
          // 
          // FieldValue
          // 
          this.FieldValue.HeaderText = "Value";
          this.FieldValue.Name = "FieldValue";
          this.FieldValue.Width = 650;
          // 
          // CmdLine
          // 
          this.CmdLine.Location = new System.Drawing.Point (12, 457);
          this.CmdLine.Multiline = true;
          this.CmdLine.Name = "CmdLine";
          this.CmdLine.Size = new System.Drawing.Size (933, 111);
          this.CmdLine.TabIndex = 6;
          // 
          // CommandLineLanel
          // 
          this.CommandLineLanel.AutoSize = true;
          this.CommandLineLanel.Location = new System.Drawing.Point (13, 441);
          this.CommandLineLanel.Name = "CommandLineLanel";
          this.CommandLineLanel.Size = new System.Drawing.Size (77, 13);
          this.CommandLineLanel.TabIndex = 7;
          this.CommandLineLanel.Text = "Command Line";
          // 
          // CloseButton
          // 
          this.CloseButton.Location = new System.Drawing.Point (434, 574);
          this.CloseButton.Name = "CloseButton";
          this.CloseButton.Size = new System.Drawing.Size (75, 23);
          this.CloseButton.TabIndex = 8;
          this.CloseButton.Text = "Close";
          this.CloseButton.UseVisualStyleBackColor = true;
          this.CloseButton.Click += new System.EventHandler (this.CloseButton_Click);
          // 
          // LogEntryViewer
          // 
          this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
          this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
          this.ClientSize = new System.Drawing.Size (959, 603);
          this.Controls.Add (this.CloseButton);
          this.Controls.Add (this.CommandLineLanel);
          this.Controls.Add (this.CmdLine);
          this.Controls.Add (this.FieldsGridView);
          this.Name = "LogEntryViewer";
          this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
          this.Text = "LogEntryViewer";
          ((System.ComponentModel.ISupportInitialize)(this.FieldsGridView)).EndInit ();
          this.ResumeLayout (false);
          this.PerformLayout ();

        }

        #endregion

        private System.Windows.Forms.DataGridView FieldsGridView;
        private System.Windows.Forms.TextBox CmdLine;
        private System.Windows.Forms.Label CommandLineLanel;
        private System.Windows.Forms.Button CloseButton;
        private System.Windows.Forms.DataGridViewTextBoxColumn FieldName;
        private System.Windows.Forms.DataGridViewTextBoxColumn FieldValue;

    }
}