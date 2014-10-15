using System;
using System.Collections.Generic;
using System.Text;
using PicesInterface;

namespace GradeTrainingModel
{


  public  class  Prediction
  {
    public
      Prediction (PicesClass          _knownClass,
                  PicesClass          _predClass,
                  double              _prob,
                  PicesFeatureVector  _fv, 
                  TrainingModel2      _trainModel,
                  bool                _partOfClassifier
                 )
    {
      knowClass        = _knownClass;
      predClass        = _predClass;
      prob             = _prob;
      fv               = _fv;
      trainModel       = _trainModel;
      partOfClassifier = _partOfClassifier;
    }

    public  String  FileName
    {
      get
      {
        if  (fv == null)
          return "";
        else
          return fv.ImageFileName;
      }
    }

    public  PicesClass          knowClass;
    public  PicesClass          predClass;
    public  double              prob;
    public  PicesFeatureVector  fv;
    public  TrainingModel2      trainModel;
    public  bool                partOfClassifier;  /**< Indicates that the known class of the example belongs to the Classifier
                                                   * before ClassName reduction specified by training level.
                                                   */
  }  /* Prediction*/

}
