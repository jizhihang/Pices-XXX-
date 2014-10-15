using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;

namespace SipperSimulator
{
  class BatteryPack
  {
    private float[] volts = new float[5];
    private bool[]  statuses = new bool[5];
    private int     activeBattery = 1;
    private bool    autoSwitchMode = false;

    private Thread  monitor             = null;
    private bool    monitorRunning      = false;
    private bool    terminateMonitoring = false;

    private bool    batteryReqSwitch     = false;
    private int     batteryReqIdx        = 0;
    private int     batteryReqTicsToWait = 0;

    public BatteryPack()
    {
      int x = 0;
      
      volts    [0] = 0.0f;   // Batteries are from 1 - 4;  
      statuses [0] = true;   // Battery  '0'  indicates automatic selection
      
      for (x = 1; x < 5; x++)
      {
        volts[x] = 26;
        statuses[x] = true;
      }

      activeBattery = 1;
      autoSwitchMode = false;
      monitor = new Thread(new ThreadStart(Monitor));
      monitor.Name = "SipperBatteryPack";
      monitor.Start ();
    }


    public  void  Dispose ()
    {
      TerminateMonitorThreadReq ();

      int x = 0;
      while ((monitorRunning) && (x < 10))
      {
        Thread.Sleep(50);
        x++;
      }
      
      monitor.Abort ();
      monitor = null;
    }


    public  void  TerminateMonitorThreadReq ()
    {
      lock (this)
      {
        terminateMonitoring = true;
      }
    }  /* TerminateMonitorThread */



    private void Monitor()
    {
      monitorRunning = true;

      while  (!terminateMonitoring)
      {
        lock (this)
        {
          volts[activeBattery] = volts[activeBattery] - 0.001f;

          if  (batteryReqSwitch)
          {          
            if  (batteryReqTicsToWait == 0)
            {
              activeBattery    = batteryReqIdx;
              batteryReqSwitch = false;
            }
            else
            {
              batteryReqTicsToWait--;
            }
          }
          
          else if  (autoSwitchMode)
          {
            if (volts[activeBattery] < 18.0)
            {
              statuses[activeBattery] = false;
              AutoSwitch();
            }
          
            else if  (volts[activeBattery] < 20.0)
              AutoSwitch ();
          }
          else
          {
            if (volts[activeBattery] < 16.0)
            {
              statuses[activeBattery] = false;
              autoSwitchMode = true;
              AutoSwitch();
            }
          }
        
        }      
        Thread.Sleep (100);
      }
      
      monitorRunning = false;
    } /* Monitor */




    private  void  SwitchBatteries (int  batteryIdx)
    {
      if  ((batteryIdx < 0)  ||  (batteryIdx > 4))
        return;
    
      if  (!statuses[batteryIdx])
        return;
        
      batteryReqSwitch = true;
      batteryReqIdx    = batteryIdx;
      batteryReqTicsToWait = 10;
    }  /* SwitchBatteries */




    public void RequestToSwitchBatteries(int batteryIdx)
    {
      // Same as 'SwitchBatteries' except available to public.  Had to do this way so
      // that a lock could be made to insure integrety.

      lock (this)
      {
        if  (batteryIdx == 0)
        {
          // We want to switch to auto battery select mode.
          autoSwitchMode= true;
          AutoSwitch ();
        }
        else
        {
          SwitchBatteries (batteryIdx);
        }
      }
    }  /* RequestToSwitchBatteries */
    



    private  void  AutoSwitch ()
    {
      int  x = 1;
      for  (x = 1;  x < 5;  x++)
      {
        if  (!statuses[x])
          continue;
          
        if  (volts[x] > 21)
        {
          SwitchBatteries (x);
          break;
        }
      }
    }  /* AutoSwitch */



    public  string  StatusStr ()
    {
      string  statusStr = "";
      
      lock  (this)
      {
        string  aliveStatusStr = "";
      
        int  x;
        
        for  (x = 1;  x < 5;  x++)
        {
          if  (statuses[x])
            aliveStatusStr += "L";
          else
            aliveStatusStr += "D";
        }

        statusStr = activeBattery.ToString() + ", " +
                    volts[1].ToString ("##0.00")   + ", "  +
                    volts[2].ToString ("##0.00")   + ", "  +
                    volts[3].ToString ("##0.00")   + ", "  +
                    volts[4].ToString ("##0.00")   + ", "  +
                    aliveStatusStr;
      }
      
      return  statusStr;
    }  /* StatusStr */
  }  /* BatteryPack */
}
