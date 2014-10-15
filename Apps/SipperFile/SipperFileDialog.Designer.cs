namespace SipperFile
{
  partial class SipperFileDialog
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
      this.SipperFileNameLabel = new System.Windows.Forms.Label ();
      this.ScanLinesLabel = new System.Windows.Forms.Label ();
      this.SipperFileName = new System.Windows.Forms.TextBox ();
      this.ScanLines = new System.Windows.Forms.TextBox ();
      this.NumImagesLabel = new System.Windows.Forms.Label ();
      this.NumImages = new System.Windows.Forms.TextBox ();
      this.DescriptionLabel = new System.Windows.Forms.Label ();
      this.Description = new System.Windows.Forms.TextBox ();
      this.ScanRateLael = new System.Windows.Forms.Label ();
      this.ScanRate = new System.Windows.Forms.TextBox ();
      this.SerialPort0Label = new System.Windows.Forms.Label ();
      this.SerialPort0 = new System.Windows.Forms.ComboBox ();
      this.SerialPort1 = new System.Windows.Forms.ComboBox ();
      this.SerialPort1Label = new System.Windows.Forms.Label ();
      this.SerialPort2 = new System.Windows.Forms.ComboBox ();
      this.SerialPort2Label = new System.Windows.Forms.Label ();
      this.SerialPort3 = new System.Windows.Forms.ComboBox ();
      this.SerialPort3Label = new System.Windows.Forms.Label ();
      this.CTDExt0Label = new System.Windows.Forms.Label ();
      this.CTDExt1Label = new System.Windows.Forms.Label ();
      this.CTDExt2Label = new System.Windows.Forms.Label ();
      this.CTDExt3Label = new System.Windows.Forms.Label ();
      this.CTDExt0 = new System.Windows.Forms.ComboBox ();
      this.CTDExt1 = new System.Windows.Forms.ComboBox ();
      this.CTDExt2 = new System.Windows.Forms.ComboBox ();
      this.CTDExt3 = new System.Windows.Forms.ComboBox ();
      this.UpdateButton = new System.Windows.Forms.Button ();
      this.CancelReqButton = new System.Windows.Forms.Button ();
      this.errorProvider1 = new System.Windows.Forms.ErrorProvider (this.components);
      this.Longitude = new System.Windows.Forms.TextBox ();
      this.LongitudeLabel = new System.Windows.Forms.Label ();
      this.Latitude = new System.Windows.Forms.TextBox ();
      this.DateTimeStartLabel = new System.Windows.Forms.Label ();
      this.DateTimeStart = new System.Windows.Forms.DateTimePicker ();
      this.Depth = new System.Windows.Forms.TextBox ();
      this.DepthLabel = new System.Windows.Forms.Label ();
      this.LatitudeLabel = new System.Windows.Forms.Label ();
      this.ExtractionStatusLabel = new System.Windows.Forms.Label ();
      this.ExtractionStatus = new System.Windows.Forms.ComboBox ();
      this.ExtractionScanLineStartLable = new System.Windows.Forms.Label ();
      this.ExtractionScanLineStart = new System.Windows.Forms.TextBox ();
      this.ExtractionScanLineEnd = new System.Windows.Forms.TextBox ();
      this.ExtractionScanLineEndLabel = new System.Windows.Forms.Label ();
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).BeginInit ();
      this.SuspendLayout ();
      // 
      // TitleLabel
      // 
      this.TitleLabel.AutoSize = true;
      this.TitleLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.TitleLabel.Location = new System.Drawing.Point (182, 9);
      this.TitleLabel.Name = "TitleLabel";
      this.TitleLabel.Size = new System.Drawing.Size (215, 24);
      this.TitleLabel.TabIndex = 0;
      this.TitleLabel.Text = "Sipper File Maintenance";
      // 
      // SipperFileNameLabel
      // 
      this.SipperFileNameLabel.AutoSize = true;
      this.SipperFileNameLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFileNameLabel.Location = new System.Drawing.Point (13, 55);
      this.SipperFileNameLabel.Name = "SipperFileNameLabel";
      this.SipperFileNameLabel.Size = new System.Drawing.Size (87, 13);
      this.SipperFileNameLabel.TabIndex = 1;
      this.SipperFileNameLabel.Text = "Sipper File Name";
      // 
      // ScanLinesLabel
      // 
      this.ScanLinesLabel.AutoSize = true;
      this.ScanLinesLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanLinesLabel.Location = new System.Drawing.Point (213, 58);
      this.ScanLinesLabel.Name = "ScanLinesLabel";
      this.ScanLinesLabel.Size = new System.Drawing.Size (60, 13);
      this.ScanLinesLabel.TabIndex = 2;
      this.ScanLinesLabel.Text = "Scan Lines";
      // 
      // SipperFileName
      // 
      this.SipperFileName.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SipperFileName.Location = new System.Drawing.Point (16, 71);
      this.SipperFileName.Name = "SipperFileName";
      this.SipperFileName.ReadOnly = true;
      this.SipperFileName.Size = new System.Drawing.Size (150, 22);
      this.SipperFileName.TabIndex = 3;
      this.SipperFileName.TabStop = false;
      // 
      // ScanLines
      // 
      this.ScanLines.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanLines.Location = new System.Drawing.Point (216, 71);
      this.ScanLines.Name = "ScanLines";
      this.ScanLines.ReadOnly = true;
      this.ScanLines.Size = new System.Drawing.Size (150, 22);
      this.ScanLines.TabIndex = 4;
      this.ScanLines.TabStop = false;
      this.ScanLines.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // NumImagesLabel
      // 
      this.NumImagesLabel.AutoSize = true;
      this.NumImagesLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.NumImagesLabel.Location = new System.Drawing.Point (399, 58);
      this.NumImagesLabel.Name = "NumImagesLabel";
      this.NumImagesLabel.Size = new System.Drawing.Size (66, 13);
      this.NumImagesLabel.TabIndex = 5;
      this.NumImagesLabel.Text = "Num Images";
      // 
      // NumImages
      // 
      this.NumImages.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.NumImages.Location = new System.Drawing.Point (402, 71);
      this.NumImages.Name = "NumImages";
      this.NumImages.ReadOnly = true;
      this.NumImages.Size = new System.Drawing.Size (150, 22);
      this.NumImages.TabIndex = 6;
      this.NumImages.TabStop = false;
      this.NumImages.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // DescriptionLabel
      // 
      this.DescriptionLabel.AutoSize = true;
      this.DescriptionLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DescriptionLabel.Location = new System.Drawing.Point (13, 105);
      this.DescriptionLabel.Name = "DescriptionLabel";
      this.DescriptionLabel.Size = new System.Drawing.Size (60, 13);
      this.DescriptionLabel.TabIndex = 7;
      this.DescriptionLabel.Text = "Description";
      // 
      // Description
      // 
      this.Description.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Description.Location = new System.Drawing.Point (16, 121);
      this.Description.Multiline = true;
      this.Description.Name = "Description";
      this.Description.Size = new System.Drawing.Size (536, 70);
      this.Description.TabIndex = 2;
      // 
      // ScanRateLael
      // 
      this.ScanRateLael.AutoSize = true;
      this.ScanRateLael.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanRateLael.Location = new System.Drawing.Point (444, 349);
      this.ScanRateLael.Name = "ScanRateLael";
      this.ScanRateLael.Size = new System.Drawing.Size (58, 13);
      this.ScanRateLael.TabIndex = 9;
      this.ScanRateLael.Text = "Scan Rate";
      // 
      // ScanRate
      // 
      this.ScanRate.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ScanRate.Location = new System.Drawing.Point (447, 365);
      this.ScanRate.Name = "ScanRate";
      this.ScanRate.Size = new System.Drawing.Size (105, 22);
      this.ScanRate.TabIndex = 50;
      this.ScanRate.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // SerialPort0Label
      // 
      this.SerialPort0Label.AutoSize = true;
      this.SerialPort0Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort0Label.Location = new System.Drawing.Point (13, 349);
      this.SerialPort0Label.Name = "SerialPort0Label";
      this.SerialPort0Label.Size = new System.Drawing.Size (64, 13);
      this.SerialPort0Label.TabIndex = 11;
      this.SerialPort0Label.Text = "Serial Port 0";
      // 
      // SerialPort0
      // 
      this.SerialPort0.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort0.FormattingEnabled = true;
      this.SerialPort0.Items.AddRange (new object[] {
            "BAT",
            "CTD",
            "P-R",
            ""});
      this.SerialPort0.Location = new System.Drawing.Point (16, 365);
      this.SerialPort0.Name = "SerialPort0";
      this.SerialPort0.Size = new System.Drawing.Size (58, 24);
      this.SerialPort0.TabIndex = 20;
      this.SerialPort0.Validating += new System.ComponentModel.CancelEventHandler (this.SerialPort0_Validating);
      // 
      // SerialPort1
      // 
      this.SerialPort1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort1.FormattingEnabled = true;
      this.SerialPort1.Items.AddRange (new object[] {
            "BAT",
            "CTD",
            "P-R",
            ""});
      this.SerialPort1.Location = new System.Drawing.Point (96, 365);
      this.SerialPort1.Name = "SerialPort1";
      this.SerialPort1.Size = new System.Drawing.Size (58, 24);
      this.SerialPort1.TabIndex = 22;
      this.SerialPort1.Validating += new System.ComponentModel.CancelEventHandler (this.SerialPort1_Validating);
      // 
      // SerialPort1Label
      // 
      this.SerialPort1Label.AutoSize = true;
      this.SerialPort1Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort1Label.Location = new System.Drawing.Point (93, 349);
      this.SerialPort1Label.Name = "SerialPort1Label";
      this.SerialPort1Label.Size = new System.Drawing.Size (64, 13);
      this.SerialPort1Label.TabIndex = 13;
      this.SerialPort1Label.Text = "Serial Port 1";
      // 
      // SerialPort2
      // 
      this.SerialPort2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort2.FormattingEnabled = true;
      this.SerialPort2.Items.AddRange (new object[] {
            "BAT",
            "CTD",
            "P-R",
            ""});
      this.SerialPort2.Location = new System.Drawing.Point (176, 365);
      this.SerialPort2.Name = "SerialPort2";
      this.SerialPort2.Size = new System.Drawing.Size (58, 24);
      this.SerialPort2.TabIndex = 24;
      this.SerialPort2.Validating += new System.ComponentModel.CancelEventHandler (this.SerialPort2_Validating);
      // 
      // SerialPort2Label
      // 
      this.SerialPort2Label.AutoSize = true;
      this.SerialPort2Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort2Label.Location = new System.Drawing.Point (173, 349);
      this.SerialPort2Label.Name = "SerialPort2Label";
      this.SerialPort2Label.Size = new System.Drawing.Size (64, 13);
      this.SerialPort2Label.TabIndex = 15;
      this.SerialPort2Label.Text = "Serial Port 2";
      // 
      // SerialPort3
      // 
      this.SerialPort3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort3.FormattingEnabled = true;
      this.SerialPort3.Items.AddRange (new object[] {
            "BAT",
            "CTD",
            "P-R",
            ""});
      this.SerialPort3.Location = new System.Drawing.Point (256, 365);
      this.SerialPort3.Name = "SerialPort3";
      this.SerialPort3.Size = new System.Drawing.Size (58, 24);
      this.SerialPort3.TabIndex = 26;
      this.SerialPort3.Validating += new System.ComponentModel.CancelEventHandler (this.SerialPort3_Validating);
      // 
      // SerialPort3Label
      // 
      this.SerialPort3Label.AutoSize = true;
      this.SerialPort3Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.SerialPort3Label.Location = new System.Drawing.Point (253, 349);
      this.SerialPort3Label.Name = "SerialPort3Label";
      this.SerialPort3Label.Size = new System.Drawing.Size (64, 13);
      this.SerialPort3Label.TabIndex = 17;
      this.SerialPort3Label.Text = "Serial Port 3";
      // 
      // CTDExt0Label
      // 
      this.CTDExt0Label.AutoSize = true;
      this.CTDExt0Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt0Label.Location = new System.Drawing.Point (13, 406);
      this.CTDExt0Label.Name = "CTDExt0Label";
      this.CTDExt0Label.Size = new System.Drawing.Size (56, 13);
      this.CTDExt0Label.TabIndex = 20;
      this.CTDExt0Label.Text = "CTD Ext 0";
      // 
      // CTDExt1Label
      // 
      this.CTDExt1Label.AutoSize = true;
      this.CTDExt1Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt1Label.Location = new System.Drawing.Point (93, 406);
      this.CTDExt1Label.Name = "CTDExt1Label";
      this.CTDExt1Label.Size = new System.Drawing.Size (56, 13);
      this.CTDExt1Label.TabIndex = 21;
      this.CTDExt1Label.Text = "CTD Ext 1";
      // 
      // CTDExt2Label
      // 
      this.CTDExt2Label.AutoSize = true;
      this.CTDExt2Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt2Label.Location = new System.Drawing.Point (173, 406);
      this.CTDExt2Label.Name = "CTDExt2Label";
      this.CTDExt2Label.Size = new System.Drawing.Size (56, 13);
      this.CTDExt2Label.TabIndex = 22;
      this.CTDExt2Label.Text = "CTD Ext 2";
      // 
      // CTDExt3Label
      // 
      this.CTDExt3Label.AutoSize = true;
      this.CTDExt3Label.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt3Label.Location = new System.Drawing.Point (253, 406);
      this.CTDExt3Label.Name = "CTDExt3Label";
      this.CTDExt3Label.Size = new System.Drawing.Size (56, 13);
      this.CTDExt3Label.TabIndex = 23;
      this.CTDExt3Label.Text = "CTD Ext 3";
      // 
      // CTDExt0
      // 
      this.CTDExt0.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt0.FormattingEnabled = true;
      this.CTDExt0.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.CTDExt0.Location = new System.Drawing.Point (16, 422);
      this.CTDExt0.Name = "CTDExt0";
      this.CTDExt0.Size = new System.Drawing.Size (58, 24);
      this.CTDExt0.TabIndex = 30;
      // 
      // CTDExt1
      // 
      this.CTDExt1.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt1.FormattingEnabled = true;
      this.CTDExt1.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.CTDExt1.Location = new System.Drawing.Point (96, 422);
      this.CTDExt1.Name = "CTDExt1";
      this.CTDExt1.Size = new System.Drawing.Size (58, 24);
      this.CTDExt1.TabIndex = 32;
      // 
      // CTDExt2
      // 
      this.CTDExt2.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt2.FormattingEnabled = true;
      this.CTDExt2.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.CTDExt2.Location = new System.Drawing.Point (176, 422);
      this.CTDExt2.Name = "CTDExt2";
      this.CTDExt2.Size = new System.Drawing.Size (58, 24);
      this.CTDExt2.TabIndex = 34;
      // 
      // CTDExt3
      // 
      this.CTDExt3.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.CTDExt3.FormattingEnabled = true;
      this.CTDExt3.Items.AddRange (new object[] {
            "FLO",
            "OXG",
            "TRN",
            "TUR"});
      this.CTDExt3.Location = new System.Drawing.Point (256, 422);
      this.CTDExt3.Name = "CTDExt3";
      this.CTDExt3.Size = new System.Drawing.Size (58, 24);
      this.CTDExt3.TabIndex = 36;
      // 
      // UpdateButton
      // 
      this.UpdateButton.Location = new System.Drawing.Point (16, 526);
      this.UpdateButton.Name = "UpdateButton";
      this.UpdateButton.Size = new System.Drawing.Size (75, 23);
      this.UpdateButton.TabIndex = 28;
      this.UpdateButton.TabStop = false;
      this.UpdateButton.Text = "Update";
      this.UpdateButton.UseVisualStyleBackColor = true;
      this.UpdateButton.Click += new System.EventHandler (this.UpdateButton_Click);
      // 
      // CancelReqButton
      // 
      this.CancelReqButton.Location = new System.Drawing.Point (477, 526);
      this.CancelReqButton.Name = "CancelReqButton";
      this.CancelReqButton.Size = new System.Drawing.Size (75, 23);
      this.CancelReqButton.TabIndex = 29;
      this.CancelReqButton.TabStop = false;
      this.CancelReqButton.Text = "Cancel";
      this.CancelReqButton.UseVisualStyleBackColor = true;
      this.CancelReqButton.Click += new System.EventHandler (this.CancelButton_Click);
      // 
      // errorProvider1
      // 
      this.errorProvider1.ContainerControl = this;
      // 
      // Longitude
      // 
      this.Longitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Longitude.Location = new System.Drawing.Point (181, 230);
      this.Longitude.Name = "Longitude";
      this.Longitude.Size = new System.Drawing.Size (145, 22);
      this.Longitude.TabIndex = 7;
      this.Longitude.Validating += new System.ComponentModel.CancelEventHandler (this.Longitude_Validating);
      // 
      // LongitudeLabel
      // 
      this.LongitudeLabel.AutoSize = true;
      this.LongitudeLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LongitudeLabel.Location = new System.Drawing.Point (178, 215);
      this.LongitudeLabel.Name = "LongitudeLabel";
      this.LongitudeLabel.Size = new System.Drawing.Size (54, 13);
      this.LongitudeLabel.TabIndex = 32;
      this.LongitudeLabel.Text = "Longitude";
      // 
      // Latitude
      // 
      this.Latitude.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Latitude.Location = new System.Drawing.Point (16, 230);
      this.Latitude.Name = "Latitude";
      this.Latitude.Size = new System.Drawing.Size (145, 22);
      this.Latitude.TabIndex = 5;
      this.Latitude.Validating += new System.ComponentModel.CancelEventHandler (this.Latitude_Validating);
      // 
      // DateTimeStartLabel
      // 
      this.DateTimeStartLabel.AutoSize = true;
      this.DateTimeStartLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateTimeStartLabel.Location = new System.Drawing.Point (13, 274);
      this.DateTimeStartLabel.Name = "DateTimeStartLabel";
      this.DateTimeStartLabel.Size = new System.Drawing.Size (55, 13);
      this.DateTimeStartLabel.TabIndex = 34;
      this.DateTimeStartLabel.Text = "Start Time";
      // 
      // DateTimeStart
      // 
      this.DateTimeStart.CustomFormat = "yyyy-MMM-dd (dddd)  HH:mm:ss";
      this.DateTimeStart.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DateTimeStart.Format = System.Windows.Forms.DateTimePickerFormat.Custom;
      this.DateTimeStart.Location = new System.Drawing.Point (16, 290);
      this.DateTimeStart.Name = "DateTimeStart";
      this.DateTimeStart.Size = new System.Drawing.Size (271, 22);
      this.DateTimeStart.TabIndex = 10;
      // 
      // Depth
      // 
      this.Depth.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.Depth.Location = new System.Drawing.Point (447, 422);
      this.Depth.Name = "Depth";
      this.Depth.Size = new System.Drawing.Size (105, 22);
      this.Depth.TabIndex = 52;
      this.Depth.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // DepthLabel
      // 
      this.DepthLabel.AutoSize = true;
      this.DepthLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.DepthLabel.Location = new System.Drawing.Point (444, 406);
      this.DepthLabel.Name = "DepthLabel";
      this.DepthLabel.Size = new System.Drawing.Size (36, 13);
      this.DepthLabel.TabIndex = 52;
      this.DepthLabel.Text = "Depth";
      // 
      // LatitudeLabel
      // 
      this.LatitudeLabel.AutoSize = true;
      this.LatitudeLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.LatitudeLabel.Location = new System.Drawing.Point (13, 215);
      this.LatitudeLabel.Name = "LatitudeLabel";
      this.LatitudeLabel.Size = new System.Drawing.Size (45, 13);
      this.LatitudeLabel.TabIndex = 30;
      this.LatitudeLabel.Text = "Latitude";
      // 
      // ExtractionStatusLabel
      // 
      this.ExtractionStatusLabel.AutoSize = true;
      this.ExtractionStatusLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ExtractionStatusLabel.Location = new System.Drawing.Point (13, 466);
      this.ExtractionStatusLabel.Name = "ExtractionStatusLabel";
      this.ExtractionStatusLabel.Size = new System.Drawing.Size (87, 13);
      this.ExtractionStatusLabel.TabIndex = 53;
      this.ExtractionStatusLabel.Text = "Extraction Status";
      // 
      // ExtractionStatus
      // 
      this.ExtractionStatus.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ExtractionStatus.FormattingEnabled = true;
      this.ExtractionStatus.Items.AddRange (new object[] {
            "Not Extracted",
            "Queued",
            "Started",
            "Completed"});
      this.ExtractionStatus.Location = new System.Drawing.Point (16, 483);
      this.ExtractionStatus.Name = "ExtractionStatus";
      this.ExtractionStatus.Size = new System.Drawing.Size (138, 24);
      this.ExtractionStatus.TabIndex = 54;
      this.ExtractionStatus.Text = "Not Extracted";
      // 
      // ExtractionScanLineStartLable
      // 
      this.ExtractionScanLineStartLable.AutoSize = true;
      this.ExtractionScanLineStartLable.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ExtractionScanLineStartLable.Location = new System.Drawing.Point (173, 466);
      this.ExtractionScanLineStartLable.Name = "ExtractionScanLineStartLable";
      this.ExtractionScanLineStartLable.Size = new System.Drawing.Size (102, 13);
      this.ExtractionScanLineStartLable.TabIndex = 55;
      this.ExtractionScanLineStartLable.Text = "Extraction Start Line";
      // 
      // ExtractionScanLineStart
      // 
      this.ExtractionScanLineStart.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ExtractionScanLineStart.Location = new System.Drawing.Point (176, 483);
      this.ExtractionScanLineStart.Name = "ExtractionScanLineStart";
      this.ExtractionScanLineStart.Size = new System.Drawing.Size (105, 22);
      this.ExtractionScanLineStart.TabIndex = 56;
      this.ExtractionScanLineStart.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ExtractionScanLineEnd
      // 
      this.ExtractionScanLineEnd.Font = new System.Drawing.Font ("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ExtractionScanLineEnd.Location = new System.Drawing.Point (305, 483);
      this.ExtractionScanLineEnd.Name = "ExtractionScanLineEnd";
      this.ExtractionScanLineEnd.Size = new System.Drawing.Size (105, 22);
      this.ExtractionScanLineEnd.TabIndex = 57;
      this.ExtractionScanLineEnd.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
      // 
      // ExtractionScanLineEndLabel
      // 
      this.ExtractionScanLineEndLabel.AutoSize = true;
      this.ExtractionScanLineEndLabel.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.ExtractionScanLineEndLabel.Location = new System.Drawing.Point (302, 467);
      this.ExtractionScanLineEndLabel.Name = "ExtractionScanLineEndLabel";
      this.ExtractionScanLineEndLabel.Size = new System.Drawing.Size (99, 13);
      this.ExtractionScanLineEndLabel.TabIndex = 58;
      this.ExtractionScanLineEndLabel.Text = "Extraction End Line";
      // 
      // SipperFileDialog
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF (6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size (569, 557);
      this.Controls.Add (this.ExtractionScanLineEndLabel);
      this.Controls.Add (this.ExtractionScanLineEnd);
      this.Controls.Add (this.ExtractionScanLineStart);
      this.Controls.Add (this.ExtractionScanLineStartLable);
      this.Controls.Add (this.ExtractionStatus);
      this.Controls.Add (this.ExtractionStatusLabel);
      this.Controls.Add (this.DepthLabel);
      this.Controls.Add (this.Depth);
      this.Controls.Add (this.DateTimeStart);
      this.Controls.Add (this.DateTimeStartLabel);
      this.Controls.Add (this.Longitude);
      this.Controls.Add (this.LongitudeLabel);
      this.Controls.Add (this.Latitude);
      this.Controls.Add (this.LatitudeLabel);
      this.Controls.Add (this.CancelReqButton);
      this.Controls.Add (this.UpdateButton);
      this.Controls.Add (this.CTDExt3);
      this.Controls.Add (this.CTDExt2);
      this.Controls.Add (this.CTDExt1);
      this.Controls.Add (this.CTDExt0);
      this.Controls.Add (this.CTDExt3Label);
      this.Controls.Add (this.CTDExt2Label);
      this.Controls.Add (this.CTDExt1Label);
      this.Controls.Add (this.CTDExt0Label);
      this.Controls.Add (this.SerialPort3);
      this.Controls.Add (this.SerialPort3Label);
      this.Controls.Add (this.SerialPort2);
      this.Controls.Add (this.SerialPort2Label);
      this.Controls.Add (this.SerialPort1);
      this.Controls.Add (this.SerialPort1Label);
      this.Controls.Add (this.SerialPort0);
      this.Controls.Add (this.SerialPort0Label);
      this.Controls.Add (this.ScanRate);
      this.Controls.Add (this.ScanRateLael);
      this.Controls.Add (this.Description);
      this.Controls.Add (this.DescriptionLabel);
      this.Controls.Add (this.NumImages);
      this.Controls.Add (this.NumImagesLabel);
      this.Controls.Add (this.ScanLines);
      this.Controls.Add (this.SipperFileName);
      this.Controls.Add (this.ScanLinesLabel);
      this.Controls.Add (this.SipperFileNameLabel);
      this.Controls.Add (this.TitleLabel);
      this.Font = new System.Drawing.Font ("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
      this.MinimumSize = new System.Drawing.Size (585, 550);
      this.Name = "SipperFileDialog";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
      this.Text = "SipperFileDialog";
      this.Load += new System.EventHandler (this.SipperFileDialog_Load);
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler (this.SipperFileDialog_FormClosing);
      ((System.ComponentModel.ISupportInitialize)(this.errorProvider1)).EndInit ();
      this.ResumeLayout (false);
      this.PerformLayout ();

    }

    #endregion

    private System.Windows.Forms.Label TitleLabel;
    private System.Windows.Forms.Label SipperFileNameLabel;
    private System.Windows.Forms.Label ScanLinesLabel;
    private System.Windows.Forms.TextBox SipperFileName;
    private System.Windows.Forms.TextBox ScanLines;
    private System.Windows.Forms.Label NumImagesLabel;
    private System.Windows.Forms.TextBox NumImages;
    private System.Windows.Forms.Label DescriptionLabel;
    private System.Windows.Forms.TextBox Description;
    private System.Windows.Forms.Label ScanRateLael;
    private System.Windows.Forms.TextBox ScanRate;
    private System.Windows.Forms.Label SerialPort0Label;
    private System.Windows.Forms.ComboBox SerialPort0;
    private System.Windows.Forms.ComboBox SerialPort1;
    private System.Windows.Forms.Label SerialPort1Label;
    private System.Windows.Forms.ComboBox SerialPort2;
    private System.Windows.Forms.Label SerialPort2Label;
    private System.Windows.Forms.ComboBox SerialPort3;
    private System.Windows.Forms.Label SerialPort3Label;
    private System.Windows.Forms.Label CTDExt0Label;
    private System.Windows.Forms.Label CTDExt1Label;
    private System.Windows.Forms.Label CTDExt2Label;
    private System.Windows.Forms.Label CTDExt3Label;
    private System.Windows.Forms.ComboBox CTDExt0;
    private System.Windows.Forms.ComboBox CTDExt1;
    private System.Windows.Forms.ComboBox CTDExt2;
    private System.Windows.Forms.ComboBox CTDExt3;
    private System.Windows.Forms.Button UpdateButton;
    private System.Windows.Forms.Button CancelReqButton;
    private System.Windows.Forms.ErrorProvider errorProvider1;
    private System.Windows.Forms.TextBox Longitude;
    private System.Windows.Forms.Label LongitudeLabel;
    private System.Windows.Forms.TextBox Latitude;
    private System.Windows.Forms.Label DateTimeStartLabel;
    private System.Windows.Forms.DateTimePicker DateTimeStart;
    private System.Windows.Forms.Label DepthLabel;
    private System.Windows.Forms.TextBox Depth;
    private System.Windows.Forms.Label ExtractionStatusLabel;
    private System.Windows.Forms.Label LatitudeLabel;
    private System.Windows.Forms.ComboBox ExtractionStatus;
    private System.Windows.Forms.TextBox ExtractionScanLineStart;
    private System.Windows.Forms.Label ExtractionScanLineStartLable;
    private System.Windows.Forms.TextBox ExtractionScanLineEnd;
    private System.Windows.Forms.Label ExtractionScanLineEndLabel;
  }
}