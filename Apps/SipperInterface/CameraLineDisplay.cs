using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;


using PicesInterface;
using SipperFile;



namespace SipperInterface
{
  public partial class  CameraLineDisplay : Form
  {
    private  byte[]                cameraData       = null;
    private  byte[]                cameraDataPrev1  = null;
    private  byte[]                cameraDataPrev2  = null;
    private  SipperConfigRec       configRec        = null;
    private  int                   firstCameraCol   = 0;
    private  int                   horzCoarsness    = 2;
    private  int                   oldScallingRatio = 0;
    private  CircularBuffer        sipperCommunications = null;
    private  int                   vertCoarsness    = 2;
    
    private  Queue<SipperCommand>  executedCmds = null;

    private  bool                  weAreWaitingForCameraLineData = false;
  
    private  bool                  errorLastTimeTryingToRetrieveData = false;
    private  StreamWriter          logFile = null;


    private  Bitmap                chartBuff   = null;
    private  Graphics              chartBuffDC = null;


    public  CameraLineDisplay (CircularBuffer  _sipperCommunications,
                               SipperConfigRec _configRec,
                               StreamWriter    _logFile
                              )
    {
      InitializeComponent ();

      executedCmds     = new Queue<SipperCommand> ();

      sipperCommunications = _sipperCommunications;
      logFile              = _logFile;
      configRec            = _configRec;
      
      oldScallingRatio     = ScallingRatio ();
      firstCameraCol       = 0;
      
      pollingTimer.Enabled = true;
      sipperCommunications.QueueSipperCommand (SipperCommand.CreateGetCammeraLineCmd (executedCmds, sipperCommunications, logFile));
      
      refreshButton.Enabled = false;
      weAreWaitingForCameraLineData = true;

      PaintWaitingForCameraLineData ();
    }




    private  int  ScallingRatio ()
    {
      if  (eightToOne.Checked)
        return 8;

      if  (fourToOne.Checked)
        return 4;
        
      if  (fullSize.Checked)
        return 1;
        
      return 1;
    }  /* ScallingRatio */
    
    
    
    private  Bitmap  BuildImage ()
    {
      if  (cameraData == null)
        return  null;
    
      int numOfCols = cameraData.Length;
      
      int scallingRatio = ScallingRatio ();
      int vertCoarsness = 2;
      int horzCoarsness = 2;
      
      int  imageHeight = 256 * vertCoarsness;
      int  imageWidth  = (numOfCols / scallingRatio) * horzCoarsness;

      int  height = imageHeight - 4;
      int  width  = imageWidth  - 4;
      int  topRow = 2;
      int  botRow = topRow + height - 1;
      
      int  leftCol = 2;
      int  rightCol = leftCol + width - 1;

      Bitmap i = new Bitmap (imageWidth, imageHeight);
      Graphics  g = Graphics.FromImage (i);
      g.Clear (Color.Black);

      {
        // Draw Axises
        Pen p = new Pen (Color.GreenYellow, 1);
        
        int row255 = botRow - 255 * vertCoarsness;
        
        g.DrawLine (p,  leftCol,  botRow,  rightCol,  botRow);
        g.DrawLine (p,  leftCol,  row255,  leftCol,   botRow);
        g.DrawLine (p,  rightCol, row255,  rightCol,  botRow);
        g.DrawLine (p,  leftCol,  row255,  rightCol,  row255);

        p.Color = Color.GreenYellow;

        int row100 = botRow - 100 * vertCoarsness;
        int row200 = botRow - 200 * vertCoarsness;
        
        g.DrawLine (p, leftCol, row100, rightCol, row100);
        g.DrawLine (p, leftCol, row200, rightCol, row200);
      }


     
      Color  c  = Color.Red;
      Color  c2 = Color.Blue;
      Color  c3 = Color.White;

      int  cameraCol = 0;
      int  imageCol  = 0;
      
      while  (imageCol < imageWidth)
      {
        int  x = 0;
        int  minCameraValue = 99999;
        int  maxCameraValue = 0;

        int  minCameraValuePrev1 = 99999;
        int  maxCameraValuePrev1 = 0;

        for (x = 0; x < scallingRatio; x++)
        {
          int  cameraValue = cameraData[cameraCol + x];
          if  (cameraValue < minCameraValue)
            minCameraValue = cameraValue;
            
          if  (cameraValue > maxCameraValue)
            maxCameraValue = cameraValue;

          int  cameraValuePrev1 = cameraDataPrev1[cameraCol + x];
          if  (cameraValuePrev1 < minCameraValuePrev1)
            minCameraValuePrev1 = cameraValuePrev1;
            
          if  (cameraValuePrev1 > maxCameraValuePrev1)
            maxCameraValuePrev1 = cameraValuePrev1;

        }

        int endImageRow   = imageHeight - (minCameraValue * vertCoarsness) - 1;
        int startImageRow = imageHeight - (maxCameraValue * vertCoarsness) - 1;

        int endImageRowPrev1   = imageHeight - (minCameraValuePrev1 * vertCoarsness) - 1;
        int startImageRowPrev1 = imageHeight - (maxCameraValuePrev1 * vertCoarsness) - 1;
      

        /*
        if  (scallingRatio == 1)
        {
          if  (cameraCol < 2048)
          {
            if  ((cameraCol % 2) == 1)
              c = Color.LightBlue;
            else
              c = Color.Lime;
          }
          else
          {
            if ((cameraCol % 2) == 1)
              c = Color.Red;
            else
              c = Color.White;
          }
        }
        */        
        
        for  (int imageRow = startImageRowPrev1;  imageRow <= endImageRowPrev1;  imageRow++)
        {
          for  (int y = 0;  y < horzCoarsness;  y++)
           i.SetPixel (imageCol + y, imageRow, c2);
        }



        for  (int imageRow = startImageRow;  imageRow <= endImageRow;  imageRow++)
        {
          for  (int y = 0;  y < horzCoarsness;  y++)
           i.SetPixel (imageCol + y, imageRow, c);
        }

        cameraCol += scallingRatio;
        imageCol += horzCoarsness;
      }
      
      
      //Bitmap  b = new Bitmap (
      
      return  i;
    }  /* BuildImage */




    private  void  PaintImage ()
    {
      if  (errorLastTimeTryingToRetrieveData)
      {
        PaintErrorRetreivingCammeraLineData ();
        return;
      }
        
      if  ((cameraData == null)  &&  weAreWaitingForCameraLineData)
      {
        PaintWaitingForCameraLineData ();
        return;        
      }
    
      if  (cameraData == null)
        return;

      // firstCameraCol = 0;
      
      if  (firstCameraCol < 0)
        firstCameraCol = 0;
        
      int  scallingRatio = ScallingRatio ();

      firstCameraCol = ((int)(firstCameraCol / scallingRatio)) * scallingRatio;

      if  (chartBuff != null)
      {
        chartBuffDC.Dispose ();
        chartBuff.Dispose ();
      }

      chartBuff = new Bitmap (cameraLineWindow.Width, cameraLineWindow.Height);
      chartBuffDC = Graphics.FromImage (chartBuff);

      int  height = cameraLineWindow.Height - 4;
      int  width  = cameraLineWindow.Width  - 4;
      int  topRow = 2;
      int  botRow = topRow + height - 1;
      
      int  leftCol = 2;
      int  rightCol = leftCol + width - 1;
      
      int  numCameraCols = cameraData.Length;
      
      int  numDisplableCameraCols = numCameraCols / scallingRatio;

      vertCoarsness = height / 256;
      if  (vertCoarsness < 2)
        vertCoarsness = 2;
        
      horzCoarsness = 2;
      if  (scallingRatio == 8)
      {
        horzCoarsness = width / numDisplableCameraCols;
        if  (horzCoarsness < 2)
          horzCoarsness = 2;
      }

      else if  (scallingRatio == 4)
      {
        horzCoarsness = width / (numDisplableCameraCols / 2);
        if (horzCoarsness < 2)
          horzCoarsness = 2;
      }

      else if (scallingRatio == 2)
      {
        horzCoarsness = width / (numDisplableCameraCols / 4);
        if (horzCoarsness < 2)
          horzCoarsness = 2;
      }

      else if (scallingRatio == 1)
      {
        horzCoarsness = width / (numDisplableCameraCols / 8);
        if (horzCoarsness < 2)
          horzCoarsness = 2;
      }
      
      Graphics  g = cameraLineWindow.CreateGraphics ();
      g.Clear (cameraLineWindow.BackColor);
      chartBuffDC.Clear (cameraLineWindow.BackColor);
     
      int  availLogicalCols = width / horzCoarsness;
      int  availLogicalRows = height / vertCoarsness;

      int  numCameraColsThatCouldFit = availLogicalCols * scallingRatio;
      numCameraColsThatCouldFit = ((int)(numCameraColsThatCouldFit / scallingRatio)) * scallingRatio;

      int  numCamColsLeftToDisplay = (numCameraCols - firstCameraCol);
      if  (numCamColsLeftToDisplay < numCameraColsThatCouldFit)
        firstCameraCol = Math.Max (0, numCameraCols - numCameraColsThatCouldFit);

      int lastCameraCol = firstCameraCol + availLogicalCols * scallingRatio;
      if  (lastCameraCol >= numCameraCols)
        lastCameraCol = numCameraCols - 1;

      {
        // Draw Axises
        Pen p = new Pen (Color.GreenYellow, 1);
        
        int row255 = botRow - 255 * vertCoarsness;
        
        g.DrawLine (p,  leftCol,  botRow,  rightCol,  botRow);
        g.DrawLine (p,  leftCol,  row255,  leftCol,   botRow);
        g.DrawLine (p,  rightCol, row255,  rightCol,  botRow);
        g.DrawLine (p,  leftCol,  row255,  rightCol,  row255);

        chartBuffDC.DrawLine (p,  leftCol,  botRow,  rightCol,  botRow);
        chartBuffDC.DrawLine (p,  leftCol,  row255,  leftCol,   botRow);
        chartBuffDC.DrawLine (p,  rightCol, row255,  rightCol,  botRow);
        chartBuffDC.DrawLine (p,  leftCol,  row255,  rightCol,  row255);

        p.Color = Color.GreenYellow;

        int row100 = botRow - 100 * vertCoarsness;
        int row200 = botRow - 200 * vertCoarsness;
        
        g.DrawLine (p, leftCol, row100, rightCol, row100);
        g.DrawLine (p, leftCol, row200, rightCol, row200);

        chartBuffDC.DrawLine (p, leftCol, row100, rightCol, row100);
        chartBuffDC.DrawLine (p, leftCol, row200, rightCol, row200);
      }

        
      Brush b  = new SolidBrush (Color.Red);
      Brush b2 = new SolidBrush (Color.Blue);

      int  logicalCol = 0;
      int  imageCol = leftCol;
      int  cameraCol = firstCameraCol;
      for  (logicalCol = 0;  logicalCol < availLogicalCols;  logicalCol++)
      {
        if  (cameraCol > lastCameraCol)
          break;

        int x = 0;
        int minCameraValue = 99999;
        int maxCameraValue = 0;

        int minCameraValuePrev1 = 99999;
        int maxCameraValuePrev1 = 0;
        
        
        for (x = 0; x < scallingRatio; x++)
        {
          int cameraValue = cameraData[cameraCol + x];
          if (cameraValue < minCameraValue)
            minCameraValue = cameraValue;

          if (cameraValue > maxCameraValue)
            maxCameraValue = cameraValue;

        
          if  (cameraDataPrev1 != null)
          {
            cameraValue = cameraDataPrev1[cameraCol + x];
            if (cameraValue < minCameraValuePrev1)
              minCameraValuePrev1 = cameraValue;

            if (cameraValue > maxCameraValuePrev1)
              maxCameraValuePrev1 = cameraValue;
          }
        }

        int endImageRow   = botRow - (minCameraValue * vertCoarsness);
        int startImageRow = botRow - (maxCameraValue * vertCoarsness);

        int endImageRowPrev1   = botRow - (minCameraValuePrev1 * vertCoarsness);
        int startImageRowPrev1 = botRow - (maxCameraValuePrev1 * vertCoarsness);

        if  (cameraDataPrev1 != null)
        {
          g.FillRectangle           (b2, imageCol, startImageRowPrev1, horzCoarsness, (1 + endImageRowPrev1 - startImageRowPrev1));
          chartBuffDC.FillRectangle (b2, imageCol, startImageRowPrev1, horzCoarsness, (1 + endImageRowPrev1 - startImageRowPrev1));
        }
        
        g.FillRectangle           (b, imageCol, startImageRow, horzCoarsness, (1 + endImageRow - startImageRow));
        chartBuffDC.FillRectangle (b, imageCol, startImageRow, horzCoarsness, (1 + endImageRow - startImageRow));
        
        cameraCol += scallingRatio;
        imageCol  += horzCoarsness;
      }
      
      pictureScrollBar.LargeChange = availLogicalCols * scallingRatio;
      pictureScrollBar.SmallChange = scallingRatio;
      pictureScrollBar.Value = firstCameraCol;
    }  /* PaintImage */




    private void  CheckForScallingRatioChange ()
    {
      int  newScallingRatio = ScallingRatio ();
      if  (newScallingRatio != oldScallingRatio)
      {
        PaintImage ();
        oldScallingRatio = newScallingRatio;
      }

    }  /* CheckForScallingRatioChange */



    private void eightToOne_CheckedChanged (object sender, EventArgs e)
    {
      CheckForScallingRatioChange ();
    }


    private void fourToOne_CheckedChanged (object sender, EventArgs e)
    {
      CheckForScallingRatioChange ();
    }



    private void fullSize_CheckedChanged (object sender, EventArgs e)
    {
      CheckForScallingRatioChange ();
    }



    private void exitButton_Click (object sender, EventArgs e)
    {
      this.Close ();
    }


    private void refreshButton_Click (object sender, EventArgs e)
    {
      errorLastTimeTryingToRetrieveData = false;
      weAreWaitingForCameraLineData = true;
      refreshButton.Enabled = false;
      sipperCommunications.QueueSipperCommand (SipperCommand.CreateGetCammeraLineCmd (executedCmds, sipperCommunications, logFile)); 
    }


    private void cameraLineWindow_Resize (object sender, EventArgs e)
    {
      PaintImage ();
    }


    private void pictureScrollBar_ValueChanged (object sender, EventArgs e)
    {
      firstCameraCol = pictureScrollBar.Value;
      PaintImage ();
    }


    private  void  ResizeMainWindow ()
    {
      int newTop;
      int newLeft;

      newTop = Height - scalingBox.Height - 36;
      scalingBox.Top = newTop;

      newTop = Height - refreshButton.Height - 36;
      refreshButton.Top = newTop;
      newLeft = (Width - refreshButton.Width) / 2;
      refreshButton.Left = newLeft;

      newTop = Height - exitButton.Height - 36;
      exitButton.Top = newTop;
      newLeft = Width - exitButton.Width - 12;
      exitButton.Left = newLeft;

      // ScrollBar
      pictureScrollBar.Width = Width - 14;
      newTop = refreshButton.Top - pictureScrollBar.Height - 26;
      pictureScrollBar.Top = newTop;

      // cameraLineWindow
      cameraLineWindow.Width = Width - 14;
      cameraLineWindow.Height = pictureScrollBar.Top - cameraLineWindow.Top;

      PaintImage ();
    }  /* ResizeMainWindow */




    private void CameraLineDisplay_SizeChanged (object sender, EventArgs e)
    {
      int  newTop;
      int  newLeft;
      
      newTop = Height - scalingBox.Height - 36;
      scalingBox.Top = newTop;
      
      newTop = Height - refreshButton.Height - 36;
      refreshButton.Top = newTop;
      newLeft = (Width - refreshButton.Width) / 2;
      refreshButton.Left = newLeft;
      
      newTop = Height - exitButton.Height - 36;
      exitButton.Top = newTop;
      newLeft = Width - exitButton.Width - 12;
      exitButton.Left = newLeft;
      
      // ScrollBar
      pictureScrollBar.Width = Width - 14;
      newTop =  refreshButton.Top - pictureScrollBar.Height - 26;
      pictureScrollBar.Top = newTop;

      // cameraLineWindow
      cameraLineWindow.Width  = Width - 14;
      cameraLineWindow.Height = pictureScrollBar.Top - cameraLineWindow.Top;
      
      PaintImage ();
    }  /* CameraLineDisplay_SizeChanged */




    private  void  PaintWaitingForCameraLineData ()
    {
      Graphics dc = cameraLineWindow.CreateGraphics ();
      Font  font = new Font ("Arial", 24);
      Brush  b = new SolidBrush (Color.Red);
      
      string  msg = "Waiting For Camera Line Data";
      Size  spaceNeeded = TextRenderer.MeasureText (msg, font);
      
      int  h = cameraLineWindow.Height;
      int  w = cameraLineWindow.Width;
      
      int  row = (h - spaceNeeded.Height) / 2;
      int  col = (w - spaceNeeded.Width)  / 2;
      
      dc.Clear (Color.Black);
      TextRenderer.DrawText (dc, msg, font, new Point (col, row), Color.Red, Color.Black);
    }  /* PaintWaitingForCameraLineData */
 


    private  void  PaintErrorRetreivingCammeraLineData ()
    {
      Graphics dc = cameraLineWindow.CreateGraphics ();
      Font  font = new Font ("Arial", 24);
      Brush  b = new SolidBrush (Color.Red);
      
      string  msg = "Error Retreiving Cammera Line Data";
      Size  spaceNeeded = TextRenderer.MeasureText (msg, font);
      
      int  h = cameraLineWindow.Height;
      int  w = cameraLineWindow.Width;
      
      
      int  row = (h - spaceNeeded.Height) / 2;
      int  col = (w - spaceNeeded.Width)  / 2;
      
      dc.Clear (Color.Black);
      TextRenderer.DrawText (dc, msg, font, new Point (col, row), Color.Red, Color.Black);
    }  /* PaintErrorRetreivingCammeraLineData   */





    private void  SaveImageInLogDirectory ()
    {
      if  (chartBuff != null)
      {
        DateTime  d = DateTime.Now;
        string logFileDir = OSservices.AddSlash (PicesSipperVariables.SipperInterfaceLogDir ()) + "CameraLines";
        string logFileName = OSservices.AddSlash (logFileDir) + "CameraLine_" + d.ToString ("yyyy-MM-dd_HHmmss") + ".jpg";
        OSservices.CreateDirectory (logFileDir);
        chartBuff.Save (logFileName);
      }
    }  /* SaveImageInLogDirectory */



    private void cameraLineWindow_Paint (object sender, PaintEventArgs e)
    {
      PaintImage ();
    }

    
    
    private void  pollingTimer_Tick (object sender, EventArgs e)
    {
      SipperCommand  cmd = null;
      
      lock  (executedCmds)
      {
        if  (executedCmds.Count > 0)
        {
          cmd = executedCmds.Dequeue ();
        }
      }
      
      if  (cmd == null)
        return;
        
      if  (cmd.ExecutedOk ())
      {
        errorLastTimeTryingToRetrieveData = false;
        cameraDataPrev2 = cameraDataPrev1;
        cameraDataPrev1 = cameraData;
        cameraData = cmd.CameraLine ();
        PaintImage ();
        SaveImageInLogDirectory ();
      }
      else
      {
        errorLastTimeTryingToRetrieveData = true;
        PaintErrorRetreivingCammeraLineData (); 
      }
      
      refreshButton.Enabled = true;
      weAreWaitingForCameraLineData = false;
    }
  }
}