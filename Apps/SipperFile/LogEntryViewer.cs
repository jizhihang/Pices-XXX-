using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

using PicesInterface;

namespace SipperFile
{
  public partial class LogEntryViewer : Form
  {
    PicesDataBaseLogEntry  logEntry = null;

    public LogEntryViewer(PicesDataBaseLogEntry  _logEntry)
    {
      logEntry = _logEntry;
      InitializeComponent();
      PopulateForm ();
    }

    private void PopulateForm ()
    {
      AddEntryToFieldTable ("LogEntryId",       logEntry.LogEntryId.ToString ());
      AddEntryToFieldTable ("ProgCode",         logEntry.ProgCode);
      AddEntryToFieldTable ("ProgName",         logEntry.ProgName);
      AddEntryToFieldTable ("DateTimeCompiled", logEntry.DateTimeCompiled.ToString ());
      AddEntryToFieldTable ("CompName",         logEntry.CompName);
      AddEntryToFieldTable ("DataBaseUserName", logEntry.DataBaseUserName);
      AddEntryToFieldTable ("CompUserName",     logEntry.CompUserName);
      AddEntryToFieldTable ("DateTimeStart",    logEntry.DateTimeStart.ToString ());
      AddEntryToFieldTable ("DateTimeStartUtc", logEntry.DateTimeStartUtc.ToString ());
      AddEntryToFieldTable ("DateTimeEnd",      logEntry.DateTimeEnd.ToString ());
      AddEntryToFieldTable ("CpuTimeUsed",      logEntry.CpuTimeUsed.ToString ());
      AddEntryToFieldTable ("CompletionStatus", logEntry.CompletionStatus);

      CmdLine.Text = logEntry.CmdLine;
    }



    private void  AddEntryToFieldTable (String fieldName, 
                                        String fieldValue
                                       )
    {
      object[]  row = new object[2];
      row[0] = fieldName;
      row[1] = fieldValue;
      FieldsGridView.Rows.Add (row);
    }

    private void CloseButton_Click (object sender, EventArgs e)
    {
      Close ();
    }
  }
}
