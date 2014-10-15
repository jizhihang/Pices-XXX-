using System;
using System.Collections.Generic;
using System.Text;

using PicesInterface;

namespace PicesCommander
{
  public class  PredictionResult
  {
    public  PredictionResult (String  _imageRootName,
                              int     _class1Id,
                              double  _class1Prob,
                              int     _class2Id,
                              double  _class2Prob
                             )
    {
      imageRootName = _imageRootName;
      class1Id      = _class1Id;
      class1Prob    = _class1Prob;
      class2Id      = _class2Id;
      class2Prob    = _class2Prob;
    }

    public  string  imageRootName;
    public  int     class1Id;
    public  double  class1Prob;
    public  int     class2Id;
    public  double  class2Prob;
  };  /* PredictionResult */
}
