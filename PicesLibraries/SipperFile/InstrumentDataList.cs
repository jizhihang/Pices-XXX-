using System;
using System.Collections.Generic;
using System.IO;
using System.Collections;
using System.Windows.Forms;
using PicesInterface;



namespace SipperFile
{
	/// <summary>
	/// Summary description for DataRowList.
	/// </summary>
	public class InstrumentDataList: ArrayList
	{
    public enum  DataOrder
    {
      NoOrder,
      TimeOrder,
      DepthOrder
    }
 
    private  DataOrder      currentOrder = DataOrder.NoOrder;
    private  InstrumentData emptyDataRow = null;
    private  int[]          fieldIndexes = null;
    private  string         fileName     = null;
    private  float[]        minValues    = null;
    private  float[]        maxValues    = null;
    private  StreamWriter   writer       = null;

    public float[]  MinValues ()  {return minValues;}
    public float[]  MaxValues ()  {return maxValues;}
    
    // MinMax  (time & depth)  values for all data in list.
    float               depthMin = 99999;
    float               depthMax = 0;
    
    DateTime            timeMin  = new DateTime (2299,12,31);
    DateTime            timeMax  = new DateTime (1900,1,1);


    public float     DepthMin ()  {return depthMin;}
    public float     DepthMax ()  {return depthMax;}
    
    public DateTime  TimeMin ()   {return timeMin;}
    public DateTime  TimeMax ()   {return timeMax;}



    public InstrumentDataList ()
		{
			//
			// TODO: Add constructor logic here
			//
		}



    /// <summary>
    /// Will cunstruct a InstrumentDataList from a PicesInstrumenDataBase.  Keep in kind that
    /// PicesInstrumentData is a managed wrapper class for the unmanaged class 'InstrumentData' 
    /// from the 'SipperInstruments' library.
    /// </summary>
    /// <param name="dbInstData"></param>
    public  InstrumentDataList (PicesInstrumentDataList  dbInstData)
    {
      if  (dbInstData == null)
        return;

      foreach  (PicesInstrumentData  id in dbInstData)
      {
        Add (new InstrumentData (id));
      }
    }



    public  InstrumentDataList (string        _fileName,
                                StreamWriter  logFile
                               )
    {
      LoadFromDataFile (_fileName, logFile);
    }



    /*

    public  InstrumentDataList  (string           sipperFileName,
                                 StreamWriter     logFile
                                )
    {
      string instDataFileName = ConstructInstrumentFileName (sipperFileName);

      LoadFromDataFile (instDataFileName, logFile);

      return;
    }  /*  DataRowList (sipperConfigRec) */

    


    public  void  StartNewDataFile (string  sipperFileName)
    {
      lock  (this)
      {
        fileName = ConstructInstrumentFileName (sipperFileName);
        OpenForWriteInstrumentDataFile ();

        this.Clear ();
      }
    }


    private  string  ConstructInstrumentFileName (string  sipperFileName)
    {
      String  instDataDir = OSservices.AddSlash (PicesSipperVariables.SipperInterfaceDir ()) + "InstrumentData";

      if  (instDataDir == "")
        instDataDir = OSservices.AddSlash (PicesSipperVariables.SipperInterfaceDir ()) + "InstrumentData";
      OSservices.CreateDirectory (instDataDir);
      
      string  instDataRootName = sipperFileName;
      if  (instDataRootName == "")
        instDataRootName = "InstrumentData";
      else
        instDataRootName = instDataRootName + "_InstrumentData";

      string instDataFileName = OSservices.AddSlash (instDataDir) + instDataRootName + ".txt";

      return  instDataFileName;
    }  /* ConstructInstrumentFileName */


    public  DataOrder  CurrentOrder ()  {return currentOrder;}




    private  void  LoadFromDataFile (string        _fileName,
                                     StreamWriter  logFile
                                    )
    {
      if  (logFile != null)
      {
        lock  (logFile)
        {
          logFile.WriteLine ("//DataRowList::LoadFromDataFile   fileName[" + _fileName + "]");
          logFile.Flush ();
        }
      }

      DataRowListLoadStatus statusDisplay = null;
      bool valid = false;

      fieldIndexes = InstrumentData.CreateCurrentFieldIndexList();

      fileName = _fileName;
      bool fileOpened = false;
      StreamReader f = null;

      try
      {
        f = new StreamReader (_fileName);
        fileOpened = true;
      }
      catch (Exception  openException)
      {
        fileOpened = false;
        if  (logFile != null)
        {
          lock  (logFile)
          {
            logFile.WriteLine ("");
            logFile.WriteLine ("//LoadFromDataFile   Error opening file, Error[" + openException.ToString () + "].");
            logFile.Flush ();
          }
        }
      }

      if (fileOpened)
      {
        while (true)
        {
          string txtLine = f.ReadLine ();
          if (f.EndOfStream)
            break;

          if (txtLine.Length < 3)
            continue;

          if (txtLine.Substring (0, 2) == "//")
            continue;

          string[] fields = txtLine.Split ('\t');

          if (fields[0].ToLower () == "fieldsnamelist")
          {
            // Next Text line will be list of fild names that are used to determine the order
            // of the collumns is following rows.
            txtLine = f.ReadLine ();
            if (f.EndOfStream)
              break;

            fields = txtLine.Split ('\t');

            fieldIndexes = InstrumentData.BuildFieldNameList (fields);
            continue;
          }
          else
          {
            InstrumentData r = new InstrumentData(txtLine, fieldIndexes, ref valid);
            if (valid)
            {
              Add (r);
              if ((Count % 100) == 0)
              {
                if (statusDisplay == null)
                {
                  statusDisplay = new DataRowListLoadStatus (_fileName);
                  statusDisplay.Show ();
                }

                statusDisplay.Update (r.Time, Count);
              }
            }
          }
        }

        if (statusDisplay != null)
        {
          statusDisplay.Close ();
          statusDisplay.Dispose ();
          statusDisplay = null;
        }

        f.Close ();
      }

      OpenForWriteInstrumentDataFile ();
      
      if  (logFile != null)
      {
        lock (logFile)
        {
          logFile.WriteLine ("");
          logFile.WriteLine ("//LoadFromDataFile   Records Loaded[" + Count + "]");
        }
      }
    }  /* LoadFromDataFile */



    private  void  OpenForWriteInstrumentDataFile ()
    {
      if  (writer != null)
      {
        writer.Close ();
        writer = null;
      }

      writer = new StreamWriter (fileName,
                                 true      // true = append.
                                );

      writer.WriteLine ("//");
      writer.WriteLine ("//");
      writer.WriteLine ("//");
      writer.WriteLine ("//Starting SIPPER Interface " + DateTime.Now.ToString ());
      writer.WriteLine ("//");
      InstrumentData.WriteFieldList (writer);
      writer.Flush ();
    }




    public  int  GetByTimeGreaterOrEqual (DateTime  t)
    {
      // Find the first row that has a time equal or greater than t
      // otherwise  return -1;


      if  (Count == 0)
        return -1;
      
      if  (this.currentOrder != DataOrder.TimeOrder)
        SortByTime ();

      // Check and see if last row is less than the time we are looking for ('t').
      InstrumentData r = (InstrumentData)(this[Count - 1]);
      int i = r.Time.CompareTo (t);
      if  (i < 0)
      {
        // All rows have a time less than 't'.
        return -1;
      }

      int   minIdx = 0;
      int   maxIdx = Count;
      bool  found = false;
      int   idx = -1;
      
      while  ((!found)  &&  (minIdx <= maxIdx))
      {
        idx = (minIdx + maxIdx) / 2;
        r = (InstrumentData)(this[idx]);

        i = r.Time.CompareTo (t);
        if  (i < 0)
          minIdx = idx + 1;

        else if  (i > 0)
          maxIdx = idx - 1;

        else
          found = true;
      }

      if  (idx >= Count)
        idx = Count - 1;

      if  (idx < 0)
        idx = 0;

      // Lets back up to we find the first row that has a time equal to 't';
      while  (idx > 0)
      {
        int  nextIdx = idx - 1;
        r = (InstrumentData)(this[nextIdx]);
        i = r.Time.CompareTo (t);
        if  (i >= 0)
          idx = nextIdx;
        else
          break;
      }
      
      return  idx;
    } /* GetByTimeGreaterOrEqual */



    public  void  Save (string _fileName)
    {
      if  (fieldIndexes == null)
        fieldIndexes = InstrumentData.CreateCurrentFieldIndexList ();
    
      StreamWriter w = null;
      try  
      {
        w = new StreamWriter (_fileName);
      }
      catch  (Exception  e)
      {
        MessageBox.Show ("Exception Opening File[" + _fileName + "]  [" + e.ToString () + "]");
        return;
      }
     
      w.WriteLine ("// Instrument Data Log");
      w.WriteLine ("// DateTime" + "\t" + DateTime.Now.ToString ());
      w.WriteLine ("// Entries"  + "\t" + Count);

      InstrumentData.WriteFieldList(w);
      
      for  (int x = 0;  x < this.Count;  x++)
      {
        InstrumentData r = (InstrumentData)(this)[x];
        r.WriteLine (w, fieldIndexes);
      }
      
      w.Close ();
    }  /* Save */



    public  void  Dispose ()
    {
      if  (writer != null)
      {
        writer.Close ();
        writer = null;
      }

      for  (int x = 0;  x < this.Count;  x++)
      {
        ((InstrumentData)((this)[x])).Dispose();
        (this)[x] = null;
      }

      Clear ();

      fileName = null;
      
    }  /* Dispose */



    public virtual  int  Add (InstrumentData row)
    {
      if  (writer != null)
      {
        row.WriteLine (writer, fieldIndexes);
      }

      float d = row.Depth ();
      if  (d < depthMin)
        depthMin = d;
        
      if  (d > depthMax)
        depthMax = d;

      if  (row.Time.Year > 2000)
      {
        if  (row.Time.CompareTo (timeMin) < 0)
          timeMin = row.Time;
      }
        
      if  (row.Time.CompareTo (timeMax) > 0)
        timeMax = row.Time;

      return  base.Add (row);
    }




    public  InstrumentData  CurrentDataRow()
   {
     if  (Count == 0)
     {
       if  (emptyDataRow == null)
         emptyDataRow = new InstrumentData();
       return emptyDataRow;
     }

     return (InstrumentData)((this)[Count - 1]);
   }  /* CurrentDataRow */




    public  void   CalcMinMaxValues (DateTime  startTime,  DateTime endTime)
    {
      minValues = new float[InstrumentData.NumDataFields()];
      maxValues = new float[InstrumentData.NumDataFields()];
      int  x = 0;

      if  (Count < 1)
      {
        for (x = 0; x < minValues.Length; x++)
        {
          minValues[x] = -10;
          maxValues[x] =  10;
        }
        
        return;
      }

      for  (x = 0;  x < minValues.Length;  x++)
      {
        minValues[x] =  999999.99f;
        maxValues[x] = -999999.99f;
      }

      foreach (InstrumentData r in this)
      {
        if  ((r.Time.CompareTo (startTime) < 0)  ||  (r.Time.CompareTo (endTime) > 0))
          continue;
      
        float[]  data = r.Data ();
        for  (x = 0;  x < minValues.Length;  x++)
        {
          if  (data[x] < minValues[x])
            minValues[x] = data[x];
            
          if  (data[x] > maxValues[x])
            maxValues[x] = data[x];
        }
      }
    }  /* CalcMinMaxValues */
    
    
    
    

    private class  TimeComparator:  IComparer  
    {
      // Calls CaseInsensitiveComparer.Compare with the parameters reversed.
      int  IComparer.Compare  
               (object  x, 
                object  y
               )  
      {
        return ((InstrumentData)x).Time.CompareTo(((InstrumentData)y).Time);
      }
    }


    private class  DepthComparator : IComparer  
    {
      // Calls CaseInsensitiveComparer.Compare with the parameters reversed.
      int  IComparer.Compare  
        (object  x, 
         object  y
        )  
      {
        float delta = ((InstrumentData)x).Depth() - ((InstrumentData)y).Depth();

        if  (delta < 0.0f)
          return -1;
       
        if  (delta > 0.0f)
          return 1;

        return (((InstrumentData)x).Time.CompareTo(((InstrumentData)y).Time));
      }
    }

    

    public  void  SortByTime ()
    {
      IComparer  c = new TimeComparator ();
      Sort (c);

      this.currentOrder = DataOrder.TimeOrder;
    }  /* SortByTime */

  

    public  void  SortByDepth ()
    {
      IComparer  c = new DepthComparator ();
      Sort (c);

      this.currentOrder = DataOrder.DepthOrder;
    }  /* SortByTime */



    public  InstrumentData  FindClosestEntryByTime (DateTime  date)
    {
      if  (Count < 1)
        return null;

      InstrumentData  nearestData = null;

      int  x = 0;

      for  (x = 0;  x < Count;  x++)
      {
        InstrumentData id = (InstrumentData)(this[x]);
        if  (id.Time.CompareTo (date) >= 0)
        {
          nearestData = id;
          break;
        }
      }

      if  (nearestData == null)
        return (InstrumentData)(this[Count - 1]);

      if  (x == 0)
        return  nearestData;

      TimeSpan ts1 = nearestData.Time - date;
      TimeSpan ts2 = date - ((InstrumentData)(this[x - 1])).Time;

      if  (ts1.Seconds < ts2.Seconds)
        return nearestData;
      else
        return (InstrumentData)(this[x - 1]);
    }  /* FindClosestEntryByTime */

  }  /* InstrumentDataList */

}
