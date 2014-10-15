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
  public partial class SipperFileDialog : Form
  {
    private  bool               addMode     = false;
    private  PicesDataBase      dbConn      = null;
    private  bool               weOwnDbConn = false;
    private  PicesRunLog        runLog      = null;
    private  PicesSipperFile    sipperFile  = null;
    private  bool               validationErrorsFound = false;
    private  String[]           ctdExtrnlPortCodes = null;  // will be populated from dtabse first time needed.
    private  String[]           ctdExtrnlPortDesc  = null;

    private  double             latitude  = 0.0;
    private  double             longitude = 0.0;

    private  bool               allowUpdates = false;


    private  PicesDataBase  DbConn ()
    {
      if  (dbConn == null)
      {
        weOwnDbConn = true;
        dbConn = PicesDataBase.GetGlobalDatabaseManagerNewInstance (runLog);
      }
      return  dbConn;
    }


    public SipperFileDialog (String       _siperFileName,
                             PicesRunLog  _runLog
                            )
    {
      runLog = _runLog;
      sipperFile = DbConn ().SipperFileRecLoad (_siperFileName);
      if  (sipperFile == null)
      {
        sipperFile = new PicesSipperFile (_siperFileName);
        sipperFile.SipperFileName = _siperFileName;
        addMode = true;
      }

      InitializeComponent();

      allowUpdates = PicesSipperVariables.AllowUpdates ();

      PopulateScreen ();
    }


    public SipperFileDialog(PicesSipperFile  _sipperFile,
                            PicesRunLog      _runLog
                           )
    {
      runLog = _runLog;

      if  (runLog == null)
        runLog = new PicesRunLog ();

      sipperFile = _sipperFile;

      if  (sipperFile != null)
      {
        // Retrieve an up-to-date version of SipperFile from database.
        PicesSipperFile  tempSipperFile = DbConn ().SipperFileRecLoad (sipperFile.SipperFileName);
        if  (tempSipperFile != null)
          sipperFile.ReFresh (tempSipperFile);
      }

      addMode = false;

      InitializeComponent();
    }



    private void SipperFileDialog_Load(object sender, EventArgs e)
    {
      allowUpdates = PicesSipperVariables.AllowUpdates ();

      if  (runLog == null)
        runLog = new PicesRunLog ();

      LoadValidCTDExtrnlPorts ();

      String[]  fieldNames = {"NumScanLines", "DateTimeStart", "NumImages"};
      String[][] results = DbConn ().QueryStatement ("call SipperFilesCalcStats(\"" + sipperFile.SipperFileName + "\")",
                                                     fieldNames
                                                    );

      if  ((results == null)  ||  (results.Length < 1))
        return;

      uint  numScanLines = PicesKKStr.StrToUint (results[0][0]);
      uint  numImages    = PicesKKStr.StrToUint (results[0][2]);

      if  (numScanLines > 0)
        ScanLines.Text = numScanLines.ToString ("###,###,##0");

      if  (numImages > 0)
        NumImages.Text = numImages.ToString ("###,###,##0");

      latitude  = sipperFile.Latitude;
      longitude = sipperFile.Longitude;

      PopulateScreen ();
    }  /* SipperFileDialog_Load */



    private  String  ExtractionStatusCodeToStr (char  code)
    {
      String  desc = null;

      switch  (code)
      {
        case '0': desc = "Not Extracted";  break;
        case '1': desc = "Queued";         break;
        case '2': desc = "Started";        break;
        case '3': desc = "Completed";      break;
        default:  desc = code.ToString () + " UNKNOWN";  break;
      }
      return  desc;
    }  /* ExtractionStatusCodeToStr */



    private  char  ExtractionStatusCodeFromStr (String  desc)
    {
      char  code = '0';

      desc = desc.ToLower ();

      if  (desc == "not extracted")
        code = '0';
      else if  (desc == "queued")
        code = '1';
      else if  (desc == "started")
        code = '2';
      else if  (desc == "completed")
        code = '3';
      else
        code = ' ';
      return  code;
    }  /* ExtractionStatusCodeFromStr */


    
    private  void  PopulateScreen ()
    {
      SipperFileName.Text  = sipperFile.SipperFileName;
      Description.Text     = sipperFile.Description;
      Latitude.Text        = PicesKKStr.LatitudeToStr  (sipperFile.Latitude);
      Longitude.Text       = PicesKKStr.LongitudeToStr (sipperFile.Longitude);

      DateTimeStart.Value  = sipperFile.DateTimeStart;

      ScanRate.Text        = sipperFile.ScanRate.ToString ("###,##0.00");
      Depth.Text           = sipperFile.Depth.ToString    ("#,##0.00");

      SerialPort0.Text     = sipperFile.Sp0;
      SerialPort1.Text     = sipperFile.Sp1;
      SerialPort2.Text     = sipperFile.Sp2;
      SerialPort3.Text     = sipperFile.Sp3;

      CTDExt0.Text         = sipperFile.CtdExt0;
      CTDExt1.Text         = sipperFile.CtdExt1;
      CTDExt2.Text         = sipperFile.CtdExt2;
      CTDExt3.Text         = sipperFile.CtdExt3;

      ExtractionStatus.Text = ExtractionStatusCodeToStr ((char)sipperFile.ExtractionStatus);
      ExtractionScanLineStart.Text = sipperFile.ExtractionScanLineStart.ToString ("###,###,##0");
      ExtractionScanLineEnd.Text   = sipperFile.ExtractionScanLineEnd.ToString   ("###,###,##0");
    }  /* PopulateScreen */




    private  void  UpdateFromScreen ()
    {
      sipperFile.SipperFileName = SipperFileName.Text;
      sipperFile.Description    = Description.Text;
      sipperFile.Latitude       = PicesKKStr.StrToLatitude  (Latitude.Text);
      sipperFile.Longitude      = PicesKKStr.StrToLongitude (Longitude.Text);
      sipperFile.DateTimeStart  = DateTimeStart.Value;
      sipperFile.ScanRate       = PicesKKStr.StrToFloat     (ScanRate.Text);
      sipperFile.Depth          = PicesKKStr.StrToFloat     (Depth.Text);
      sipperFile.Sp0            = SerialPort0.Text;
      sipperFile.Sp1            = SerialPort1.Text;
      sipperFile.Sp2            = SerialPort2.Text;
      sipperFile.Sp3            = SerialPort3.Text;
      sipperFile.CtdExt0        = CTDExt0.Text;
      sipperFile.CtdExt1        = CTDExt1.Text;
      sipperFile.CtdExt2        = CTDExt2.Text;
      sipperFile.CtdExt3        = CTDExt3.Text;

      sipperFile.ExtractionStatus = (sbyte)ExtractionStatusCodeFromStr (ExtractionStatus.Text);
      sipperFile.ExtractionScanLineStart = PicesKKStr.StrToUint (ExtractionScanLineStart.Text);
      sipperFile.ExtractionScanLineEnd   = PicesKKStr.StrToUint (ExtractionScanLineEnd.Text);
    }  /* PopulateScreen */



    private  bool  ChangesMade ()
    {
      float  newScanRate = PicesKKStr.StrToFloat (ScanRate.Text);
      float  newDepth    = PicesKKStr.StrToFloat (Depth.Text);
      String  newDepthStr = newDepth.ToString ("#,##0.00");
      String  oldDepthStr = sipperFile.Depth.ToString ("#,##0.00");

      char  extractionStatus        = ExtractionStatusCodeFromStr (ExtractionStatus.Text);
      uint  extractionScanLineStart = PicesKKStr.StrToUint (ExtractionScanLineStart.Text);
      uint  extractionScanLineEnd   = PicesKKStr.StrToUint (ExtractionScanLineEnd.Text);
      
      bool  changesMade = (sipperFile.SipperFileName != SipperFileName.Text)  ||
                          (sipperFile.Description    != Description.Text)     || 
                          (sipperFile.ScanRate       != newScanRate)          ||
                          (oldDepthStr               != newDepthStr)          ||
                          (sipperFile.Sp0            != SerialPort0.Text)     ||
                          (sipperFile.Sp1            != SerialPort1.Text)     ||
                          (sipperFile.Sp2            != SerialPort2.Text)     ||
                          (sipperFile.Sp3            != SerialPort3.Text)     ||
                          (sipperFile.CtdExt0        != CTDExt0.Text)         ||
                          (sipperFile.CtdExt1        != CTDExt1.Text)         ||
                          (sipperFile.CtdExt2        != CTDExt2.Text)         ||
                          (sipperFile.CtdExt3        != CTDExt3.Text)         ||
                          (sipperFile.DateTimeStart  != DateTimeStart.Value)  ||
                          (sipperFile.Latitude       != latitude)             ||
                          (sipperFile.Longitude      != longitude)            ||
                          (sipperFile.ExtractionStatus        != extractionStatus)        ||
                          (sipperFile.ExtractionScanLineStart != extractionScanLineStart) ||
                          (sipperFile.ExtractionScanLineEnd   != extractionScanLineEnd);

      return  changesMade;
    }  /* ChangesMade */



    private  void  LoadValidCTDExtrnlPorts ()
    {
      ctdExtrnlPortCodes = null;
      ctdExtrnlPortDesc  = null;
      String  sqlStr = "select * from  CTDExternalSensors";
      String[]   cols = {"SensorName", "Description"};
      String[][] results = DbConn ().QueryStatement (sqlStr, cols);
      if  ((results == null)  ||  (results.Length < 2))
      {
        ctdExtrnlPortCodes = new String[5];
        ctdExtrnlPortDesc  = new String[5];

        ctdExtrnlPortCodes[0] = "CDM";
        ctdExtrnlPortCodes[1] = "FLO";
        ctdExtrnlPortCodes[2] = "OXG";
        ctdExtrnlPortCodes[3] = "TRN";
        ctdExtrnlPortCodes[4] = "TUR";

        ctdExtrnlPortDesc[0] = "CDOM";
        ctdExtrnlPortDesc[1] = "Fluorometer";
        ctdExtrnlPortDesc[2] = "Oxygen";
        ctdExtrnlPortDesc[3] = "Transmisivity";
        ctdExtrnlPortDesc[4] = "Turbidity";
      }
      else
      {
        ctdExtrnlPortCodes = new String[results.Length];
        ctdExtrnlPortDesc  = new String[results.Length];
        for  (int x = 0;  x < results.Length;  x++)
        {
          ctdExtrnlPortCodes[x] = results[x][0];
          ctdExtrnlPortDesc[x]  = results[x][1];
        }
      }

      CTDExt0.Items.Clear ();
      CTDExt1.Items.Clear ();
      CTDExt2.Items.Clear ();
      CTDExt3.Items.Clear ();

      CTDExt0.Items.Add ("");
      CTDExt1.Items.Add ("");
      CTDExt2.Items.Add ("");
      CTDExt3.Items.Add ("");

      for  (int x = 0;  x < ctdExtrnlPortCodes.Length;  x++)
      {
        CTDExt0.Items.Add (ctdExtrnlPortCodes[x]);
        CTDExt1.Items.Add (ctdExtrnlPortCodes[x]);
        CTDExt2.Items.Add (ctdExtrnlPortCodes[x]);
        CTDExt3.Items.Add (ctdExtrnlPortCodes[x]);
      }
    }  /* LoadValidCTDExtrnlPorts */





    private  void  ValidateScanRate ()
    {
      errorProvider1.SetError (ScanRate, null);
      float  newScanRate = PicesKKStr.StrToFloat (ScanRate.Text);
      if  (newScanRate < 0.0f)
      {
        errorProvider1.SetError (ScanRate, "ScanRate must be greater than 0.0");
        validationErrorsFound = true;
        return;
      }
      
      ScanRate.Text = newScanRate.ToString ("###,##0.00");
    }



    private  void  ValidateDepth ()
    {
      errorProvider1.SetError (Depth, null);
      float  newDepth = PicesKKStr.StrToFloat (Depth.Text);
      if  (newDepth < 0.0f)
      {
        errorProvider1.SetError (ScanRate, "Depth must be greater than 0.0");
        validationErrorsFound = true;
        return;
      }
      
      Depth.Text = newDepth.ToString ("###,##0.00");
    }


    

    private  void  ValidateSerialPort (ComboBox  cb,
                                       int       spNum
                                      )
    {
      errorProvider1.SetError (cb, null); 
      if  (!String.IsNullOrEmpty (cb.Text))
      {
        Instrument i = Instrument.LookUpByShortName (cb.Text);
        if  (i == null)
        {
          errorProvider1.SetError (cb, "Invalid Instrument[" + cb.Text + "]");
          validationErrorsFound = true;
          return;
        }
      }
    }  /* ValidateSerialPort */



    private  String  LookUpCTDExtrnlCode (String extrnlCode)
    {
      if  (ctdExtrnlPortCodes == null)
        return null;
      int x = 0;
      for  (x = 0;  x < ctdExtrnlPortCodes.Length;  x++)
      {
        if  (extrnlCode.CompareTo (ctdExtrnlPortCodes[x]) == 0)
        {
          return  ctdExtrnlPortDesc[x];
        }
      }
      return null;
    }



    private  void  ValidateExternalPort (ComboBox  cb,
                                         int       extrnlPortNum
                                        )
    {
      errorProvider1.SetError (cb, null); 
      if  (!String.IsNullOrEmpty (cb.Text))
      {
        if  (LookUpCTDExtrnlCode (cb.Text) == null)
        {
          errorProvider1.SetError (cb, "Invalid External[" + cb.Text + "]");
          validationErrorsFound = true;
          return;
        }
      }
    }  /* ValidateExternalPort */




    private  void  ValidateExtractionStatus()
    {
      char code = ExtractionStatusCodeFromStr (ExtractionStatus.Text);
      if  (code == ' ')
      {
        errorProvider1.SetError (ExtractionStatus, "Invalid Extraction Status");
        validationErrorsFound = true;
        return;
      }
    }


    private  void  ValidateExtractionScanLineEnd ()
    {
      uint extractionScanLineStart = PicesKKStr.StrToUint (ExtractionScanLineStart.Text);
      uint extractionScanLineEnd   = PicesKKStr.StrToUint (ExtractionScanLineEnd.Text);

      ExtractionScanLineStart.Text = extractionScanLineStart.ToString ("###,###,##0");
      ExtractionScanLineEnd.Text   = extractionScanLineEnd.ToString   ("###,###,##0");

      if  ((extractionScanLineEnd > 0)  &&  (extractionScanLineEnd < extractionScanLineStart))
      {
        errorProvider1.SetError (ExtractionScanLineEnd, "End Scan line must be greater thah start scan line or '0'.");
        validationErrorsFound = true;
        return;
      }
    }


    private  void  ValidateAllFields ()
    {
      validationErrorsFound = false;
      ValidateScanRate ();
      ValidateSerialPort (SerialPort0, 0);
      ValidateSerialPort (SerialPort1, 1);
      ValidateSerialPort (SerialPort2, 2);
      ValidateSerialPort (SerialPort3, 3);

      ValidateExternalPort (CTDExt0, 0);
      ValidateExternalPort (CTDExt1, 1);
      ValidateExternalPort (CTDExt2, 2);
      ValidateExternalPort (CTDExt3, 3);

      ValidateExtractionStatus ();
      ValidateExtractionScanLineEnd ();
    }  /* ValidateAllFields */


    
    private  void  PerformUpdate ()
    {
      if  (!allowUpdates)
      {
        PicesSipperVariables.UpdatesNotAllowed ("Sipper File Dialog");
        return;
      }


      UpdateFromScreen ();

      if  (addMode)
        DbConn ().SipperFileInsert (sipperFile);
      else
        DbConn ().SipperFileUpdate (sipperFile);

      if  (DbConn ().Valid ())
      {
       Close ();
      }
      else
      {
        MessageBox.Show (this,  "Update of databse failed.\n\n" + DbConn ().LastErrorDesc (),  "Update Failed", MessageBoxButtons.OK);
      }
    }



    private void SerialPort0_Validating(object sender, CancelEventArgs e)
    {
      ValidateSerialPort (SerialPort0, 0);
    }


    private void SerialPort1_Validating(object sender, CancelEventArgs e)
    {
      ValidateSerialPort (SerialPort1, 1);
    }

    private void SerialPort2_Validating(object sender, CancelEventArgs e)
    {
      ValidateSerialPort (SerialPort2, 2);
    }

    private void SerialPort3_Validating(object sender, CancelEventArgs e)
    {
      ValidateSerialPort (SerialPort3, 3);
    }

    private void UpdateButton_Click(object sender, EventArgs e)
    {
      if  (!allowUpdates)
        return;

      ValidateAllFields ();
      if  (validationErrorsFound)
        return;

      if  (!ChangesMade ())
      {
        MessageBox.Show ("No Changes Made");
        return;
      }

      PerformUpdate ();
    }



    private  bool  userRequestedToCancel = false;


    private void CancelButton_Click(object sender, EventArgs e)
    {
      if  (!ChangesMade ())
      {
        Close ();
      }
      else
      {
        DialogResult dr = MessageBox.Show (this, 
                                           "Changes have been made; are you sure that you want to exit ?", 
                                           "Cancel Sipper File Dialog", 
                                           MessageBoxButtons.YesNo
                                          );
        if  (dr == DialogResult.Yes)
        {
          userRequestedToCancel = true;
          Close ();
        }
      }
    }


    private void SipperFileDialog_FormClosing(object sender, FormClosingEventArgs e)
    {
      if  (!userRequestedToCancel)
      {
        if  (ChangesMade ())
        {
          DialogResult dr = MessageBox.Show (this, "Changes have been made;  are you sure you want to exit ?", "Exiting Sipper File Dialog", MessageBoxButtons.YesNo);
          if  (dr == DialogResult.No)
          {
            e.Cancel = true;
            return;
          }
        }
      }

      if  (weOwnDbConn)
      {
        dbConn.Close ();
        dbConn = null;
        GC.Collect();
        weOwnDbConn = false;
      }
    }


    private void Latitude_Validating(object sender, CancelEventArgs e)
    {
      latitude = PicesKKStr.StrToLatitude (Latitude.Text);
      Latitude.Text = PicesKKStr.LatitudeToStr (latitude);
    }


    private void Longitude_Validating(object sender, CancelEventArgs e)
    {
      longitude = PicesKKStr.StrToLongitude (Longitude.Text);
      Longitude.Text = PicesKKStr.LongitudeToStr (longitude);
    }
  }
}
