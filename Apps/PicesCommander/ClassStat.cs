using System;
using System.Collections.Generic;
using System.Text;

using PicesInterface;

namespace PicesCommander
{
  public  class  ClassStat
  {
    public  ClassStat (PicesClass  _mlClass, uint  _count)
    {
      mlClass = _mlClass;
      count      = _count;
    }

    public  PicesClass  mlClass;
    public  uint        count;

    public override string ToString()
    {
      return mlClass.Name;
    }
  }  /* ClassStat */



  public class  ClassStatSortedList: SortedList<String,ClassStat>
  {
    public  ClassStatSortedList ()
    {
    }


    public  void  AddOne (PicesClass  pc)
    {
      int index = IndexOfKey (pc.Name);
      if  (index >= 0)
      {
        this[pc.Name].count += 1;
      }
      else
      {
        Add (pc.Name, new ClassStat (pc, 1));
      }
    }  /* AddOne */


    public  void  AddCount (PicesClass  _pc,
                            uint        _count
                           )
    {
      int index = IndexOfKey (_pc.Name);
      if  (index >= 0)
      {
        this[_pc.Name].count += _count;
      }
      else
      {
        Add (_pc.Name, new ClassStat (_pc, _count));
      }
    }


    public  PicesClassStatisticList  ToPicesClassStatisticList ()
    {
      PicesClassStatisticList result = new PicesClassStatisticList ();
      IList<ClassStat>  stats = this.Values;

      foreach  (ClassStat cs in stats)
      {
        result.Add (new PicesClassStatistic (cs.mlClass, cs.count));
      }

      return  result;
    }  /* ToPicesClassStatisticList */

  }  /* ClassStatSortedList */



}
