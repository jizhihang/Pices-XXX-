using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Imaging;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;


namespace ChartControls
{
  public partial class TeminalWindow : UserControl
  {
    private bool          active            = false;

    private Font          currentFont       = null;
    private string        curLine           = "";
    private int           curLineCursorPos  = 0;  // Where to insert the next Chrater into curLine.  Normally this will
                                                  // be point to just beyond the end of 'curLine'.
                                                  // Example if 'curLine' == "Set",  'curLineCursorPos' <= '3'.
                                                  
    private int           curLineFirstRow   = 0;  // These two variables will bracket the current line being entered
    private int           curLineLastRow    = 0;  // that is 'curLine'.  Example if the current line starts on the 
                                                  // third row of the terminal window and is 2 lines long:
                                                  //    'curLineFirstRow' <= 2  and  'curLineLastRow' <= 3
    
    private string         lastLine          = "";
    
    private List<string>   userHistory       = null;
    private int            userHistoryNextRowToRetreive    = -1;
    private int            userHistoryMaxSize    = 100;
    
    private  Color         backGroundColor   = Color.White;
    private  SolidBrush    backGroundBrush   = null;
    private  Pen           backGroundPen     = null;
    
    private  Bitmap        buffer            = null;
    private  Graphics      bufferDC          = null;
    
    private  int           charWidth         = 0;  // width of each charater in pixels;  we will be using only fixed spacd fonts.
    private  int           lineHeight        = 0;  // Height of each textline in pixels
    private  Size          charSize;
    
    private  Color         foreGroundColor   = Color.Black;
    private  SolidBrush    foreGroundBrush   = null;
    private  Pen           foreGroundPen     = null;

    private  int           marginLeft        = 7;
    private  int           marginRight       = 5;
    private  int           marginTop         = 5;
    private  int           marginBottom      = 5;
    
    private  Graphics      terminalScreenDC    = null;
    private  int           textMaxNumOfRows    = 0;
    private  int           textCharsPerLine    = 0;  // Max num of charaters per text line.
    private  int           textLastRowInScreen = 0;

    private  Point         blinkingCursorLoc;
    private  Rectangle     blinkingCursorRec;
    private  bool          cursorOn          = false;
    private  int           cursorRow         = 0;  // Row that cursor is currently on.
    private  int           cursorCol         = 0;  // Col that cursor is currently at.
    private  int           cursorColLast     = 0;
    private  int           cursorRowLast     = 0;

    private  Queue<string> pendingCmds           = null;
    private  Queue<string> pendingTextForConsole = null;



    TextFormatFlags flags        = TextFormatFlags.NoPadding;
    Size            proposedSize = new Size (int.MaxValue, int.MaxValue);
    bool            terminalWindowDoneConstructing = false;

    public delegate void  NewCmdEnteredHandler ();
    public delegate void  LineChangedHandler ();



    public TeminalWindow()
    {
      InitializeComponent ();

      pendingCmds           = new Queue<string> ();
      pendingTextForConsole = new Queue<string> ();

      userHistory = new List<string>(userHistoryMaxSize);
      currentFont = TerminalScreen.Font;

      terminalScreenDC = TerminalScreen.CreateGraphics ();
      backGroundColor = Color.FromArgb (255, 255, 255);
      backGroundBrush = new SolidBrush (backGroundColor);
      backGroundPen   = new Pen (backGroundBrush);
      
      foreGroundColor = Color.FromArgb (0, 0, 0);
      foreGroundBrush = new SolidBrush (foreGroundColor);
      foreGroundPen   = new Pen (foreGroundBrush);

      UpdateBufferForPanelSize ();
      
      terminalWindowDoneConstructing = true;
      
      ComputeParameters ();

      blinkingCursorLoc = new Point     (marginLeft, marginTop);
      blinkingCursorRec = new Rectangle (Point.Round (blinkingCursorLoc), new Size (1, lineHeight));
      
      RefreshTimer.Enabled = true;
    }



    //****************************************************
    //                  NewCmdEntered                   *
    //****************************************************
    [Category("Terminal Action")]
    [Description("Fires when a new line of text is entered.")]
    public event NewCmdEnteredHandler NewCmdEntered;

    protected virtual void OnNewCmdEntered ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if  (NewCmdEntered != null)
      {
        NewCmdEntered ();  // Notify Subscribers
      }
    }  /* OnNewCmdEntered */




    //****************************************************
    //                CurrentLineChanged                 *
    //****************************************************
    [Category("Terminal Action")]
    [Description("Fires when current line changs.")]
    public event  LineChangedHandler CurrentLineChanged;

    protected virtual  void  OnCurrentLineChanged ()
    {
      // If an event has no subscribers registerd, it will
      // evaluate to null. The test checks that the value is not
      // null, ensuring that there are subsribers before
      // calling the event itself.
      if (CurrentLineChanged != null)
      {
        CurrentLineChanged ();  // Notify Subscribers
      }
    }  /* OnNewCmdEntered */



    [Bindable(true), Category("Terminal Options"),
    DefaultValue(""),
    Description("Last line entered.")]
    protected string  LastLine
    {
      get { return lastLine; }
    }


    [Bindable(true), Category("Terminal Options"),
    DefaultValue(""),
    Description("Current Line being edited.")]
    protected string CurrentLine
    {
      get { return CurrentLine; }
    }



    [Bindable(true), Category("Terminal Options"),
    DefaultValue(""),
    Description("History")]
    public List<string>  History
    {
      get { return userHistory; }
      set
      {
        InitializeHistory (value);
      }
    }




    [Bindable(true), Category("Terminal Options"),
     Description("Terminal Font.")]
    protected Font CurrentFont
    {
      get { return currentFont; }
      set 
      { 
        currentFont = value;
      }
    }  /* CurrentFont */



    private  char[]  pendingCmdsLockBuffer = new char[10];


    [Bindable (true), Category ("Terminal Options"),
     Description ("Number pending comand on queue.")]
    protected int  PendingCmdsCount
    {
      get 
      {
        int pendingCmdsCount = 0;
        lock  (pendingCmdsLockBuffer)
        {
          pendingCmdsCount = pendingCmds.Count;
        }
        return pendingCmdsCount; 
      }
    }  /* PendingCmdsCount */






    private  void  AddPendingCmd (string  cmd)
    {
      lock  (pendingCmdsLockBuffer)
      {
        pendingCmds.Enqueue (cmd);
      }
    }  /* AddPendingCmd */




    public  string  GetNextPendingCmd ()
    {
      string  cmd = "";
      
      lock (pendingCmdsLockBuffer)
      {
        cmd = pendingCmds.Dequeue ();
      }
      
      return  cmd;
    }  /* GetNextPendingCmd */





    private  char[]  pendingTextForConsoleLockBuffer = new char[10];
    
    private  char[]  endOfLineCharacters = {'\n', '\r'};



    public  void  AddTextToConsole (string  txt)
    {
      lock  (pendingTextForConsoleLockBuffer)
      {
        if  (txt == "")
          pendingTextForConsole.Enqueue ("");
        
        else
        {
          if  (txt.EndsWith ("\n\r"))
            txt = txt.Substring (0, txt.Length - 2);
      
          else if  (txt[txt.Length - 1]  == '\r')
            txt = txt.Substring (0, txt.Length - 1);

          string[] lines = txt.Split (endOfLineCharacters);
          for  (int x = 0;  x < lines.Length;  x++)
            pendingTextForConsole.Enqueue (lines[x]);
        }
      }
    }  /* AddTextToConsole */



    private  string  GetPendingTextForConsole ()
    {
      string  textLine = null;
      lock  (pendingTextForConsoleLockBuffer)
      {
        if  (pendingTextForConsole.Count > 0)
          textLine = pendingTextForConsole.Dequeue ();
      }
      
      return  textLine;
    }  /* GetPendingTextForConsole */



    private  int  PendingTextForConsoleCount ()
    {
      int  count = 0;
      lock  (pendingTextForConsoleLockBuffer)
      {
        count = pendingTextForConsole.Count;
      }
      return  count;
    }  /* PendingTextForConsoleCount */



    private  void  UpdateBufferForPanelSize ()
    {
      if  (buffer != null)
      {
        bufferDC.Dispose ();
        bufferDC = null;
        buffer.Dispose ();
        buffer = null;
      }
      
      buffer = new Bitmap (TerminalScreen.Width, TerminalScreen.Height);
      bufferDC = Graphics.FromImage (buffer);
      bufferDC.Clear (backGroundColor);
    } /* UpdateBufferForPanelSize */





    private  void  ComputeParameters ()
    {
      if  ((currentFont == null)  ||   (terminalScreenDC == null))
      {
        MessageBox.Show ("ComputeParameters   Was called before TerminalWindow has finished initializing."  + "\n" +
                         "                    will ignore call; but bug programmer to fix this situation.");
        return;
      }
    
    
      if  (buffer == null)
        UpdateBufferForPanelSize ();

      int  h = TerminalScreen.Height;
      int  w = TerminalScreen.Width;
      lineHeight = currentFont.Height;
      textMaxNumOfRows = (int)((h - (marginTop + marginBottom)) / lineHeight);
      textLastRowInScreen = textMaxNumOfRows - 1;

      string  s1 = "W0123456789ABCDEFGHIJKLMNOPQRSTUFWXYZabcdefghijklmnopqrstuvwxyzW";
      string  textStr = s1 + s1 + s1 + s1 + s1;
      float xxx = (float)TextRenderer.MeasureText (terminalScreenDC, textStr, currentFont, proposedSize, flags).Width / (float)textStr.Length;

      string  s = currentFont.ToString ();

      //charWidth = (int)Math.Ceiling (xxx);
      charWidth = (int)Math.Floor (xxx);
      charSize = new Size (charWidth, lineHeight);
      textCharsPerLine = (int)((float)(w - (marginLeft + marginRight)) / (float)charWidth);
    }  /* ComputeParameters */




    private void  UpdateForSizeChange ()
    {
      int  h = Height;
      int  w = Width;
      
      TerminalScreen.Height = (h - StatusBar.Height);
      TerminalScreen.Width  = w;
    
      UpdateBufferForPanelSize ();
      
      if  (terminalWindowDoneConstructing)
        ComputeParameters ();
    }  /* UpdateForSizeChange */




    private  void  InitializeHistory (List<string> _history)
    {
      userHistory = _history;
    }





    private  Point  ComputePointForRowCol (int  textRow,
                                           int  textCol
                                          )
    {
      int row = textRow * lineHeight + marginTop;
      int col = textCol * charWidth  + marginLeft;
      return new Point (col, row);
    }  /* ComputePointForRowCol */





    private  Point  ComputeTextCurRowColPixelValues ()
    {
      return  ComputePointForRowCol (cursorRow, cursorCol);
    }  /* ComputeTextCurRowColPixelValues */
  
  


    private void  ScrollTextRows (Bitmap  b,
                                  int     numTextRows
                                 )
    {
      int numPixelRows = numTextRows * lineHeight;
    
      // GDI+ return format is BGR, NOT RGB. 
      // http://www.bobpowell.net/lockingbits.htm
      
      BitmapData bmData = b.LockBits (new Rectangle (0, 0, b.Width, b.Height),
                                      ImageLockMode.ReadWrite,
                                      PixelFormat.Format24bppRgb
                                     );

      int stride = bmData.Stride;
      System.IntPtr  Scan0 = bmData.Scan0;

      unsafe
      {
        int  nOffset     = stride - b.Width * 3;
        int  bytesPerRow = b.Width * 3 + nOffset;
      
        byte*  destPtr = (byte*)(void*)Scan0;
        byte*  srcPtr =  ((byte*)(void*)Scan0) + bytesPerRow * numPixelRows;

        int nWidth = b.Width * 3;

        for (int y = 0; y < (b.Height - numPixelRows); ++y)
        {
          for (int x = 0; x < nWidth; ++x)
          {
            destPtr[0] = srcPtr[0];
            ++srcPtr;
            ++destPtr;
          }
          
          srcPtr  += nOffset;
          destPtr += nOffset;
        }
      }  /* Unsafe */

      b.UnlockBits (bmData);
       
      {
        // Now blank out area that was scrolled away.
        int topTextRow = textMaxNumOfRows - numTextRows;
        
        Point p = ComputePointForRowCol (topTextRow, 0);
        int   height = buffer.Height - p.Y + 1;
        bufferDC.FillRectangle (backGroundBrush, new Rectangle (p, new Size (buffer.Width, height)));
      }      
    }  /* ScrollTextRows */



    private  void  ScrollCurrentLineDownOneRow ()
    {
      if  (curLineLastRow >= (textMaxNumOfRows - 1))
      {
        // the Current Line being entered (curLine) is alredy at the bottom of the screen
        return;
      }
      
      Point p = ComputePointForRowCol (curLineFirstRow, 0);
      int  numLinesInCurLine = 1 + curLineLastRow - curLineFirstRow;
      int  numLinesToBotttonOfScreen = (textMaxNumOfRows - curLineFirstRow);
      
      Rectangle  r = new Rectangle (p, new Size (textCharsPerLine * charWidth, numLinesToBotttonOfScreen * lineHeight));
      terminalScreenDC.FillRectangle (backGroundBrush, r);
      bufferDC.FillRectangle         (backGroundBrush, r);
      
      // Adjust current line info to reflect the move down 1 row,
      curLineFirstRow++;
      curLineLastRow++;
      cursorRow++;
      
      // Lets repain the current line.
      string  wrkStr = curLine;
      int  row = curLineFirstRow;
      
      while  (wrkStr.Length > 0)
      {
        string  textThisRow = "";
        if  (wrkStr.Length > textCharsPerLine)
        {
          textThisRow = wrkStr.Substring (0, textCharsPerLine - 1);
          wrkStr = wrkStr.Substring (textCharsPerLine);
        }
        else
        {
          textThisRow = wrkStr;
          wrkStr = "";
        }
        
        p = ComputePointForRowCol (0, row);
        TextRenderer.DrawText (terminalScreenDC, textThisRow, currentFont, p, foreGroundColor, backGroundColor, flags);
        TextRenderer.DrawText (bufferDC,         textThisRow, currentFont, p, foreGroundColor, backGroundColor, flags);
      }
    }  /* ScrollCurrentLineDownOneRow */
    



    private  void  ScrollEveryThingAboveCurrentLineUpOneRow ()
    {
      if  (curLineFirstRow == 0)
      {
        // There is no place for us to scroll.
        return;
      }


      if  ((curLineFirstRow < 0)  ||  (curLineFirstRow >= textMaxNumOfRows))
      {
        // This is not good;  something had gone very wrong with this termnal window.
        throw new Exception ("TerminalWindow.ScrollEveryThingAboveCurrentLineUpOneRow   'curLineFirstRow'[" + curLineFirstRow.ToString () + "] is out of screen bounds.");
      }

      int  pixelRowsDelta = lineHeight;
      int  heightOfScrollingArea = ComputePointForRowCol (curLineFirstRow, 0).Y;

      // GDI+ return format is BGR, NOT RGB. 
      // http://www.bobpowell.net/lockingbits.htm

      BitmapData bmData = buffer.LockBits (new Rectangle (0, 0, buffer.Width, buffer.Height),
                                           ImageLockMode.ReadWrite,
                                           PixelFormat.Format24bppRgb
                                          );

      int stride = bmData.Stride;
      System.IntPtr Scan0 = bmData.Scan0;

      unsafe
      {
        int nOffset     = stride - buffer.Width * 3;
        int bytesPerRow = buffer.Width * 3 + nOffset;

        byte* destPtr = (byte*)(void*)Scan0;
        byte* srcPtr =  ((byte*)(void*)Scan0) + bytesPerRow * pixelRowsDelta;

        int nWidth = buffer.Width * 3;

        for (int y = 0; y < (heightOfScrollingArea - pixelRowsDelta); ++y)
        {
          for (int x = 0; x < nWidth; ++x)
          {
            destPtr[0] = srcPtr[0];
            ++srcPtr;
            ++destPtr;
          }

          srcPtr  += nOffset;
          destPtr += nOffset;
        }
      }  /* Unsafe */

      buffer.UnlockBits (bmData);

      {
        // Now blank out area that was scrolled away.
        int  topTextRow = curLineFirstRow - 1;
        Point p = ComputePointForRowCol (topTextRow, 0);
        bufferDC.FillRectangle (backGroundBrush, new Rectangle (p, new Size (buffer.Width, lineHeight)));
      }
      
      terminalScreenDC.DrawImageUnscaled (buffer, 0, 0);
    }  /* ScrollEveryThingAboveCurrentLineUpOneRow */



    private  void  UpdateScreenWithPendingConsoleText (string  text)
    {
      int  numRowsNeeded = (int)Math.Ceiling ((float)text.Length / (float)textCharsPerLine);
      
      int  numRowsMadeAvailable = 0;  
      
      while  (numRowsMadeAvailable < numRowsNeeded)
      {
        if  (curLineLastRow < textLastRowInScreen)
        {
          ScrollCurrentLineDownOneRow ();
          numRowsMadeAvailable++;
        }
        else
        {
          ScrollEveryThingAboveCurrentLineUpOneRow ();
          numRowsMadeAvailable++;
        }
      }


      // Now that we have made space we can display text
      // Lets repaint the current line.
      string wrkStr = text;
      int row = curLineFirstRow - numRowsNeeded;

      while (wrkStr.Length > 0)
      {
        string textThisRow = "";
        if (wrkStr.Length > textCharsPerLine)
        {
          textThisRow = wrkStr.Substring (0, textCharsPerLine - 1);
          wrkStr = wrkStr.Substring (textCharsPerLine);
        }
        else
        {
          textThisRow = wrkStr;
          wrkStr = "";
        }

        Point p = ComputePointForRowCol (row, 0);
        TextRenderer.DrawText (terminalScreenDC, textThisRow, currentFont, p, foreGroundColor, backGroundColor, flags);
        TextRenderer.DrawText (bufferDC,         textThisRow, currentFont, p, foreGroundColor, backGroundColor, flags);
        
        row++;
      }
    } /* UpdateScreenWithPendingConsoleText */





    private  void  ScrollOneTextRow ()
    {
      ScrollTextRows (buffer, 1);
      terminalScreenDC.DrawImageUnscaled (buffer, 0, 0);
      cursorRow--;
      curLineFirstRow--;
      curLineLastRow--;
    }



    private void  StrobeCursor ()
    {
      if  (!active)
        return;

      if ((cursorColLast!= cursorCol) ||  (cursorRowLast!= cursorRow))
      {
        blinkingCursorLoc = ComputeTextCurRowColPixelValues ();
        blinkingCursorLoc.X = blinkingCursorLoc.X;
        blinkingCursorRec = new Rectangle (Point.Round (blinkingCursorLoc), new Size (1, lineHeight));
        cursorColLast = cursorCol;
        cursorRowLast = cursorRow;
      }
    
      if  (cursorOn)
      {
        terminalScreenDC.FillRectangle (backGroundBrush, blinkingCursorRec);
        cursorOn = false;
      }
      else
      {
        terminalScreenDC.FillRectangle (foreGroundBrush, blinkingCursorRec);
        cursorOn = true;
      }

    }  /* StrobeCursor */





    private  void  ClearCursorOnScreen ()
    {
      if  (cursorOn)
        terminalScreenDC.FillRectangle (backGroundBrush, blinkingCursorRec);
      cursorOn = false;
    }  /* ClearCursorOnScreen */




    private  void  ClearCurLineFromScreen ()
    {
      int  numRows = 1 + curLineLastRow - curLineFirstRow;
      
      Point  p = ComputePointForRowCol (curLineFirstRow, 0);
      
      int  lineWidthInPixels = (textCharsPerLine * charWidth);

      Rectangle r = new Rectangle (p, new Size (lineWidthInPixels, numRows * lineHeight));
      terminalScreenDC.FillRectangle (backGroundBrush, r);
      bufferDC.FillRectangle (backGroundBrush, r);
      
      cursorRow        = curLineFirstRow;
      cursorCol        = 0;
      curLineLastRow   = curLineFirstRow;
      curLineCursorPos = 0;
      curLine          = "";
    }  /* ClearCurLineFromScreen */




    private  void  TypeString (string s)
    {
      ClearCursorOnScreen ();

      // We will have to clear the rest of the row from the cursor position to end.
      // then we can redraw the line starting from the cursor position.

      curLine = curLine.Insert (curLineCursorPos, s);
      
      string wrkStr = curLine.Substring (curLineCursorPos);

      int  row = cursorRow;
      int  col = cursorCol;
      
      while  (wrkStr.Length > 0)
      {
        curLineLastRow = row;

        if  (row >= textMaxNumOfRows)
        {
          // We have to scroll the screen
          ScrollOneTextRow ();
          row--;
        }
        
        // Lets draw what we can fit on the rest of this row
        
        string  textThisRow = "";

        int  colsLeftOnRow = textCharsPerLine - col;
        if  (colsLeftOnRow >= wrkStr.Length)
        {
          textThisRow = wrkStr;
          wrkStr = "";
        }
        else
        {
          textThisRow = wrkStr.Substring (0, colsLeftOnRow);
          wrkStr = wrkStr.Substring (colsLeftOnRow);
        }
      
        Point  p = ComputePointForRowCol (row, col);
        int lineWidthInPixels = textThisRow.Length * charWidth;
        Rectangle r = new Rectangle (p, new Size (lineWidthInPixels, lineHeight));

        terminalScreenDC.FillRectangle (backGroundBrush, r);
        bufferDC.FillRectangle (backGroundBrush, r);
        
        //terminalScreenDC.DrawString (textThisRow, currentFont, foreGroundBrush, p, strFmt);
        //bufferDC.DrawString (textThisRow, currentFont, foreGroundBrush, p, strFmt);


        TextRenderer.DrawText (terminalScreenDC, textThisRow, currentFont, p, foreGroundColor, backGroundColor, flags);
        TextRenderer.DrawText (bufferDC,         textThisRow, currentFont, p, foreGroundColor, backGroundColor, flags);

        row++;
        col = 0;
      }

      OnCurrentLineChanged ();

      cursorCol += s.Length;
      while  (cursorCol >=  textCharsPerLine)
      {
        cursorCol = cursorCol - textCharsPerLine;
        cursorRow++;
        if  (cursorRow >= textMaxNumOfRows)
          ScrollOneTextRow ();
      }
      
      curLineCursorPos += s.Length;
      
      //terminalScreenDC.DrawImageUnscaled (buffer, 0, 0);
    }  /* TypeString */




    private  void  EnterKey ()
    {
      curLine = curLine.Trim ();
      if  (curLine != "")
      {
        userHistory.Add (curLine);
        userHistoryNextRowToRetreive = userHistory.Count - 1;
      }
      
      lastLine = curLine;
      AddPendingCmd (curLine);
      curLine = "";

      OnNewCmdEntered ();
      
      cursorRow++;
      cursorCol = 0;
      curLineCursorPos = 0;
      
      if  (cursorRow >= textMaxNumOfRows)
      {
        ScrollOneTextRow ();
      }
      
      curLineFirstRow = curLineLastRow = cursorRow;
    }  /* EnterKey */



    private  void  BackSpace ()
    {
      if  (curLineCursorPos <= 0)
        return;
    
      curLineCursorPos--;
      cursorCol--;
      
      if  (cursorCol < 0)
      {
        cursorCol = textCharsPerLine - 1;
        cursorRow--;
      }
      
      curLine = curLine.Remove (curLineCursorPos, 1);

      int  col = cursorCol;
      int  row = cursorRow;
      string  strLeftToDisplay = curLine.Substring (curLineCursorPos);
      
      while  (strLeftToDisplay.Length > 0)
      {
        curLineLastRow = row;

        // Lets draw what we can fit on the rest of this row

        string textThisRow = "";

        int colsLeftOnRow = textCharsPerLine - col;
        if (colsLeftOnRow >= strLeftToDisplay.Length)
        {
          textThisRow = strLeftToDisplay;
          strLeftToDisplay = "";
        }
        else
        {
          textThisRow = strLeftToDisplay.Substring (0, colsLeftOnRow);
          strLeftToDisplay = strLeftToDisplay.Substring (colsLeftOnRow);
        }

        Point  p = ComputePointForRowCol (row, col);
        int lineWidthInPixels = textThisRow.Length * charWidth;
        Rectangle r = new Rectangle (Point.Round (p), new Size (lineWidthInPixels, lineHeight));

        terminalScreenDC.FillRectangle (backGroundBrush, r);
        bufferDC.FillRectangle (backGroundBrush, r);

        //terminalScreenDC.DrawString (textThisRow, currentFont, foreGroundBrush, p, strFmt);
        //bufferDC.DrawString (textThisRow, currentFont, foreGroundBrush, p, strFmt);

        TextRenderer.DrawText (terminalScreenDC, textThisRow, currentFont, Point.Round (p), foreGroundColor, backGroundColor, flags);
        TextRenderer.DrawText (bufferDC,         textThisRow, currentFont, Point.Round (p), foreGroundColor, backGroundColor, flags);

        col = col + textThisRow.Length;
        if  (col >= textCharsPerLine)
        {
          row++;
          col = 0;
        }
      }

      
      {
        //  Blank out the last character.
        Point  p = ComputePointForRowCol (row, col);
        Rectangle r = new Rectangle (p, charSize);
        terminalScreenDC.FillRectangle (backGroundBrush, r);
        bufferDC.FillRectangle         (backGroundBrush, r);
      }

      OnCurrentLineChanged ();

    }  /* BackSpace */




    private void  UpArrowPressed ()
    {
      if  (userHistory.Count < 1)
      {
        userHistoryNextRowToRetreive = -1;
        return;
      }

      if (userHistoryNextRowToRetreive >= userHistory.Count)
        userHistoryNextRowToRetreive = 0;
      else if (userHistoryNextRowToRetreive < 0)
        userHistoryNextRowToRetreive = userHistory.Count - 1;

      string  newCurLine = "";

      newCurLine = userHistory[userHistoryNextRowToRetreive];
      userHistoryNextRowToRetreive--;
      if  (userHistoryNextRowToRetreive < 0)
        userHistoryNextRowToRetreive = userHistory.Count - 1;

      ClearCurLineFromScreen ();
      
      TypeString (newCurLine);
    }  /* UpArrowPressed */




    private void DownArrowPressed ()
    {
      if  (userHistory.Count < 1)
      {
        userHistoryNextRowToRetreive = -1;
        return;
      }
      
      if  (userHistoryNextRowToRetreive >= userHistory.Count)
        userHistoryNextRowToRetreive = 0;
      else if  (userHistoryNextRowToRetreive < 0)
        userHistoryNextRowToRetreive = userHistory.Count - 1;

      string newCurLine = "";

      newCurLine = userHistory[userHistoryNextRowToRetreive];
      userHistoryNextRowToRetreive++;
      if (userHistoryNextRowToRetreive >=  userHistory.Count)
        userHistoryNextRowToRetreive = 0;

      ClearCurLineFromScreen ();

      TypeString (newCurLine);
    }  /* DownArrowPressed */




    private void LeftArrowKey ()
    {
      if  (curLineCursorPos <= 0)
        return;
        
      cursorCol--;
      if  (cursorCol < 0)
      {
        cursorRow--;
        cursorCol = textCharsPerLine - 1;
      }
    
      curLineCursorPos--;
    }  /* LeftArrowKey */


    private  void  ControlLeftArrowKey ()
    {
    }


    private void RightArrowKey ()
    {
      if (curLineCursorPos >= curLine.Length)
        return;

      cursorCol++;
      if (cursorCol >= textCharsPerLine)
      {
        cursorRow++;
        cursorCol = 0;
      }

      curLineCursorPos++;
    }  /* RightArrowKey */




    private void  HomeKey ()
    {
      cursorRow = curLineFirstRow;
      cursorCol = 0;
      curLineCursorPos = 0;
    }  /* HomeKey */




    private void  EndKey ()
    {
      cursorRow = curLineLastRow;
      cursorCol = curLine.Length;
      
      int  numOfRows = 1 + curLineLastRow - curLineFirstRow;
      cursorCol = cursorCol - textCharsPerLine * (numOfRows - 1);
      
      curLineCursorPos = curLine.Length;
    }  /* EndKey */
    



    private void TeminalWindow_SizeChanged (object sender, EventArgs e)
    {
      UpdateForSizeChange ();
    }



    private void TerminalScreen_MouseClick (object sender, MouseEventArgs e)
    {
      this.Select ();
    }


    private void TeminalWindow_KeyPress(object sender, KeyPressEventArgs e)
    {
      char ch = e.KeyChar;

      ClearCursorOnScreen ();

      TypeString (ch.ToString ());
    }  /* TeminalWindow_KeyPress */





    private void TerminalScreen_Paint (object sender, PaintEventArgs e)
    {
      terminalScreenDC.DrawImageUnscaled (buffer, 0, 0);
    }




    protected override bool  ProcessCmdKey (ref Message msg, Keys keyData)
    {
      const int WM_KEYDOWN    = 0x100;
      const int WM_SYSKEYDOWN = 0x104;

      if  ((msg.Msg == WM_KEYDOWN)  ||  (msg.Msg == WM_SYSKEYDOWN))
      {

        ClearCursorOnScreen ();
        
        switch (keyData)
        {
          case Keys.Down:
            DownArrowPressed ();
            return  true;

          case Keys.Up:
            UpArrowPressed ();
            return  true;
            
          case Keys.Left:
            LeftArrowKey ();
            return true;
            
          case Keys.Right:
            RightArrowKey ();
            return true;
            
          case Keys.Home:
            HomeKey ();
            return  true;
            
          case Keys.End:
            EndKey ();
            return  true;
            
          case Keys.Back:
            BackSpace ();
            return true;
            
          case Keys.Enter:
            EnterKey ();
            return  true;
            
          case Keys.LControlKey:
            ControlLeftArrowKey ();
            return true;
        }
      }

      return base.ProcessCmdKey(ref msg, keyData);
    }  /* ProcessCmdKey */
    
    

    private void TeminalWindow_Enter (object sender, EventArgs e)
    {
      active = true;
      //RefreshTimer.Enabled = true;
    }



    private void TeminalWindow_Leave(object sender, EventArgs e)
    {
      ClearCursorOnScreen ();
      active = false;
      //RefreshTimer.Enabled = false;
    }  /* TeminalWindow_Leave */




    int  miliSecsSinceLastStrobe = 0;

    private void RefreshTimer_Tick (object sender, EventArgs e)
    {
      miliSecsSinceLastStrobe += RefreshTimer.Interval;
    
      if  (miliSecsSinceLastStrobe > 333)
      {
        StrobeCursor ();
        miliSecsSinceLastStrobe = 0;
      }

      int  numPendingText =  PendingTextForConsoleCount ();
      int  x = 0;

      while  ((numPendingText > 0)  &&  (x < 5))
      {
        string  nextLine = GetPendingTextForConsole ();
        if  (nextLine != null)
          UpdateScreenWithPendingConsoleText (nextLine);
        x++;
        numPendingText--;
      }
      
      if  (numPendingText < 1)
        RefreshTimer.Interval = 111;
      else
        RefreshTimer.Interval = 5;
      
    }  /* RefreshTimer_Tick */


  }  /* TeminalWindow */
}
