using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

using PicesInterface;



namespace GradeTrainingModel
{
  public partial class ConfusionMatrix : Form
  {
    PicesClassList        classes           = null;
    PicesClassList        classesPredLevel  = null;

    PicesClassList        classifierClasses = null;   /**< A list of classes that belong to the Classifier, 
                                                       *This list should be the class list before reduction do
                                                       *to sleected training level.
                                                       */

    PicesClass            otherClass        = null;   /**< Class labeld as other in Classifier.  This is
                                                       * where Dual classifers send predictions that don't 
                                                       * agree.
                                                       */

    int[]                 knownClassCounts  = null;
    int                   numPredClasses    = -1;     // -1 = Data Struxctures need to be iniailized.
    int[]                 predClassCounts   = null;
    int[][]               predictionCounts  = null;
    List<Prediction>      predictions       = new List<Prediction> ();

    int                   totalCount         = 0;
    float                 totalCorrect       = 0.0f;
    int                   totalCorrectNPC    = 0;
    float                 accuracyAll        = 0.0f;
    float                 accuracyAllNPC     = 0.0f;
    int[]                 totalByCorectLevel = new int[8];


    int                   totalPartOfClassifier = 0;
    float                 totalPartOfClassifierCorrect = 0.0f;
    int                   totalPartOfClassifierCorrectNPC = 0;
    int[]                 totalPartOfClassifierByCorectLevel = new int[8];
    float                 accuracyPartOfClass     = 0.0f;
    float                 accuracyPartOfClassNPC  = 0.0f;

    int                   totalNoise            = 0;
    float                 totalNoiseCorrect     = 0.0f;
    int                   totalNoiseCorrectNPC  = 0;
    float                 accuracyNoise         = 0.0f;
    float                 accuracyNoiseNPC      = 0.0f;

    int                   totalNonNoise         = 0;
    float                 totalNonNoiseCorrect  = 0.0f;
    int                   totalNonNoiseCorrectNPC = 0;
    float                 accuracyNonNoise      = 0.0f;
    float                 accuracyNonNoiseNPC   = 0.0f;

    float                 percentPartOfClass    = 0.0f;
    float                 percentNotPartOfClass = 0.0f;
    float                 percentNoise          = 0.0f;
    float                 percentNonNoise       = 0.0f;


    int                   totalNotPartOfClassifier = 0;
    Dictionary<String, Prediction>[][]  imagesByPred      = null;

    uint                  trainLevel        = 1;
    uint                  predLevel         = 1;


    // Matrix Display Parameters
    bool                        displayPercentages = false;
    int                         maxProbability     = 100;
    int                         minProbability     = 0;

    int                         lastWidth  = 0;
    int                         lastHeight = 0;

    PicesTrainingConfiguration  config;
    TrainingModel2              trainingModel  = null;
    String                      groundTruthRootDir = null;

    PicesClassList              matrixRowClassAssigned = null;



    enum PartialCreditIdx: int  {None        = 0,
                                 Whole       = 1,
                                 OneHalf     = 2, 
                                 OneThird    = 3,
                                 TwoThirds   = 4, 
                                 OneForth    = 5,
                                 TwoForths   = 6, 
                                 ThreeForths = 7
                                };




    public ConfusionMatrix (PicesTrainingConfiguration  _config,
                            PicesClassList              _classifierClasses,  /**< List of classes in origianl Classifier before reduction due to training level. */
                            TrainingModel2              _trainingModel,
                            String                      _modelName,
                            String                      _groundTruthRootDir,
                            uint                        _trainLevel,
                            uint                        _predLevel
                           )
    {
      config             = _config;
      trainingModel      = _trainingModel;
      groundTruthRootDir = _groundTruthRootDir;
      trainLevel         = _trainLevel;
      predLevel          = _predLevel;
      classifierClasses  = _classifierClasses;
      
      if  (config != null)
        otherClass = config.OtherClass ();

      InitializeComponent();
      classes = new PicesClassList ();
      classes.Clear ();

      ModelName.Text   = _modelName;
      GroundTruth.Text = _groundTruthRootDir;
    }





    public  void  SetTrainingModel (TrainingModel2  _trainingModel)
    {
      trainingModel = _trainingModel;

      if  (otherClass == null)
        otherClass = PicesClassList.GetUniqueClass ("Other","");
    }


    private  PicesClassList  SortIntoDisplayOrder (PicesClassList  l)
    {
      PicesClassList  knownClasses = new PicesClassList ();
      PicesClassList  unKnownClasses = new PicesClassList ();
      foreach  (PicesClass pc in l)
      {
        if  (ClassInClassifierClasses (pc))
          knownClasses.Add (pc);
        else
          unKnownClasses.Add (pc);
      }

      knownClasses.SortByName ();
      unKnownClasses.SortByName ();
    
      foreach (PicesClass  pc in unKnownClasses)
        knownClasses.Add (pc);
      return  knownClasses;
    }


    private  void  InitilizeDataStructures ()
    {
      int  x, y;
      
      classesPredLevel = classes.ExtractListOfClassesForAGivenHierarchialLevel (predLevel);
      //classesPredLevel.SortByName ();
      classesPredLevel = SortIntoDisplayOrder (classesPredLevel);

      numPredClasses = classesPredLevel.Count;

      totalCount                   = 0;
      totalCorrect                 = 0.0f;
      totalCorrectNPC              = 0;
      for  (x = 0;  x < 8;   ++x)
      {
        totalByCorectLevel[x] = 0;
        totalPartOfClassifierByCorectLevel[x] = 0;
      }

      totalPartOfClassifier        = 0;
      totalPartOfClassifierCorrect = 0.0f;
      totalPartOfClassifierCorrectNPC = 0;
      
      totalNoise                   = 0;
      totalNoiseCorrect            = 0.0f;
      totalNoiseCorrectNPC         = 0;

      totalNonNoise                = 0;
      totalNonNoiseCorrect         = 0.0f;
      totalNonNoiseCorrectNPC      = 0;

      totalNotPartOfClassifier = 0;
      knownClassCounts = new int[numPredClasses];
      predictionCounts = new int[numPredClasses][];
      predClassCounts  = new int[numPredClasses];

      imagesByPred = new Dictionary<String, Prediction>[numPredClasses][];

      for  (x = 0;  x < numPredClasses;  x++)
      {
        knownClassCounts[x] = 0;
        predClassCounts [x] = 0;
        predictionCounts[x] = new int[numPredClasses];
        imagesByPred[x]     = new Dictionary<String, Prediction>[numPredClasses];
        
        for  (y = 0;  y < numPredClasses;  y++)
        {
          predictionCounts[x][y] = 0;
          imagesByPred[x][y] = new Dictionary<String, Prediction> ();
        }
      }
    }   /* InitilizeDataStructures */



    private  String  MatrixFieldStr (int count, int total)
    {
      String  field = "";
      if  (displayPercentages)
      {
        if  (total == 0)
          field = "0.00%";
        else
        {
          float percentage = (100.0f * (float)count / (float)total);
          field = percentage.ToString ("##0.00") + "%";
        }
      }
      else
      {
        field = count.ToString ("##,###,##0");
      }

      return  field;
    }  /* MatrixFieldStr */



    private  void  ComputeSumaryAccuracies ()
    {
      accuracyAll = 0.0f;
      accuracyAllNPC = 0.0f;
      if  (totalCount > 0)
      {
        accuracyAll    = totalCorrect / (float)totalCount;
        accuracyAllNPC        = (float)totalCorrectNPC               / (float)totalCount;
        percentNotPartOfClass = (float)totalNotPartOfClassifier      / (float)totalCount;
        percentPartOfClass    = (float)totalPartOfClassifier         / (float)totalCount;
        percentNonNoise       = (float)totalNonNoise                 / (float)totalCount;
        percentNoise          = (float)totalNoise                    / (float)totalCount;
      }

      accuracyPartOfClass = 0.0f;
      accuracyPartOfClassNPC = 0.0f;
      if  (totalPartOfClassifier > 0)
      {
        accuracyPartOfClass    = totalPartOfClassifierCorrect    / (float)totalPartOfClassifier;
        accuracyPartOfClassNPC = totalPartOfClassifierCorrectNPC / (float)totalPartOfClassifier;
      }

      accuracyNoise = 0.0f;
      accuracyNoiseNPC = 0.0f;
      if  (totalNoise > 0)
      {
        accuracyNoise     = totalNoiseCorrect           / (float)totalNoise;
        accuracyNoiseNPC  = (float)totalNoiseCorrectNPC / (float)totalNoise;
      }

      accuracyNonNoise = 0.0f;
      accuracyNonNoiseNPC = 0.0f;
      if  (totalNonNoise > 0)
      {
        accuracyNonNoise    = totalNonNoiseCorrect / (float)totalNonNoise;
        accuracyNonNoiseNPC = totalNonNoiseCorrectNPC / (float)totalNonNoise;
      }
    }



    private  void  PopulateMatrix ()
    {
      int  x, y;
      
      Font  normFont = new Font ("Courier",  8, FontStyle.Regular);
      Font  boldFont = new Font ("Courier", 10, FontStyle.Bold);

      Matrix.Items.Clear ();
      Matrix.Columns.Clear ();

      Matrix.Columns.Add ("ClassName", 135, HorizontalAlignment.Left);
      Matrix.Columns.Add ("Count",      60, HorizontalAlignment.Center);

      matrixRowClassAssigned = new PicesClassList ();
      
      foreach (PicesClass c in classesPredLevel)
      {
        Size textSize = TextRenderer.MeasureText(c.Name, Matrix.Font);
        int  width = Math.Max (65, (textSize.Width + 6));
        Matrix.Columns.Add (c.Name, width, HorizontalAlignment.Center);
      }
      for  (x = 0;  x < numPredClasses;  x++)
      {
        PicesClass c = classesPredLevel[x];

        bool  okToDisplayThisRow = false;
        if  (knownClassCounts[x] != 0)
        {
          okToDisplayThisRow = true;
        }
        else
        {
          for  (y = 0;  y < numPredClasses;  y++)
          {
            if  (predictionCounts[x][y] != 0)
            {
              okToDisplayThisRow = true;
              break;
            }
          }
        }

        if  (okToDisplayThisRow)
        {
          // Matrix.I
          ListViewItem lvi = new ListViewItem (c.Name);
          //lvi.SubItems.Add (c.Name);

          lvi.SubItems.Add (MatrixFieldStr (knownClassCounts[x], totalCount));

          for  (y = 0;  y < numPredClasses;  y++)
          {
            String  field = MatrixFieldStr (predictionCounts[x][y], knownClassCounts[x]);
            if  (x == y)
            {
              if  (ClassInClassifierClasses (c))
                lvi.SubItems.Add (field, Color.Green,   Color.White, boldFont);
              else
                lvi.SubItems.Add (field + 'v', Color.Olive,   Color.White, boldFont);
            }
            else
            {
              if  (predictionCounts[x][y] == 0)
              {
                lvi.SubItems.Add ("",    Color.Black, Color.White, normFont);
              }
              else
              {
                lvi.SubItems.Add (field, Color.Black, Color.White, normFont);
              }
            }
          }

          lvi.UseItemStyleForSubItems = false;
          Matrix.Items.Add (lvi);
          matrixRowClassAssigned.Add (c);
        }
      }

      {
        // Lets print a Total Line.
        // Matrix.I
        ListViewItem lvi = new ListViewItem ("Totals");
        //lvi.SubItems.Add (c.Name);

        lvi.SubItems.Add (MatrixFieldStr (totalCount, totalCount));

        for  (y = 0;  y < numPredClasses;  y++)
        {
          String  field = MatrixFieldStr (predClassCounts [y], totalCount);
          lvi.SubItems.Add (field, Color.Black, Color.White, boldFont);
        }

        lvi.UseItemStyleForSubItems = false;
        Matrix.Items.Add (lvi);
      }

      ComputeSumaryAccuracies ();

      TotalAll.Text        = totalCount.ToString      ("###,##0");
      CorrectAll.Text      = totalCorrect.ToString    ("###,##0.00");
      AccuracyAll.Text     = accuracyAll.ToString     ("P2");
      AccuracyAll_NPC.Text = accuracyAllNPC.ToString  ("P2");

      TotalPartOfClassifier.Text        = totalPartOfClassifier.ToString        ("###,##0");
      CorrectPartOfClassifier.Text      = totalPartOfClassifierCorrect.ToString ("###,##0.00");
      AccuracyPartOfClassifier.Text     = accuracyPartOfClass.ToString          ("P2");
      AccuracyPartOfClassifier_NPC.Text = accuracyPartOfClassNPC.ToString       ("P2");
     
      TotalNoise2.Text        = totalNoise.ToString           ("###,##0");
      CorrectNoise2.Text      = totalNoiseCorrect.ToString    ("###,##0.00");
      AccuracyNoise2.Text     = accuracyNoise.ToString        ("P2");
      AccuracyNoise2_NPC.Text = accuracyNoiseNPC.ToString     ("P2");
      
      TotalNonNoise2.Text       = totalNonNoise.ToString        ("###,##0");
      CorrectNonNoise.Text      = totalNonNoiseCorrect.ToString ("###,##0.00");
      AccuracyNonNoise.Text     = accuracyNonNoise.ToString     ("P2");
      AccuracyNonNoise_NPC.Text = accuracyNonNoiseNPC.ToString  ("P2");
    }  /* PopulateMatrix */



    private  void  DetermineKnownIdxAndPredIdx (int      mouseRow, 
                                                int      mouseCol,
                                                ref int  matrixRowIdx,
                                                ref int  predIDX
                                               )
    {
      // First lets detemine which cell in the matrix that was clicked.
      
      ListViewItem lvi = null;

      for  (matrixRowIdx = 0;  matrixRowIdx < Matrix.Items.Count;  matrixRowIdx++)
      {
        lvi = Matrix.Items[matrixRowIdx];
        if  ((lvi.Bounds.Top    < mouseRow)  &&
             (lvi.Bounds.Bottom > mouseRow)
            )
        {
          // We found the correct row
          break;
        }
      }

      if  (matrixRowIdx >= Matrix.Items.Count)
      {
        matrixRowIdx = -1;
        predIDX  = -1;
        return;
      }

      // Determine the Column we are are interested in.
      ListViewItem.ListViewSubItem lvsi = null;
      for  (predIDX = 1;  predIDX < lvi.SubItems.Count;  predIDX++)
      {
        lvsi = lvi.SubItems[predIDX] ;
        int  colRight = lvsi.Bounds.Left + lvsi.Bounds.Width;
        if  ((lvsi.Bounds.Left < mouseCol)  &&  (colRight > mouseCol))
          break;
      }

      if  (predIDX >= lvi.SubItems.Count)
      {
        matrixRowIdx = -1;
        predIDX  = -1;
        return;
      }

      predIDX = predIDX - 2;  // The first two columns are for ClassName and Count.
      if  (predIDX < 0)
      {
        matrixRowIdx = -1;
        predIDX  = -1;
        return;
      }
    }  /* DetermineKnownIdxAndPredIdx */

    
    
    
    
    private  void  ExploreMatrixCell (int  mouseRow, 
                                      int  mouseCol
                                     )
    {
      int  knownIDX = 0;
      int  predIDX  = 0;
      int  matrixRowIdx = 0;

      if  (matrixRowClassAssigned == null)
        return;

      DetermineKnownIdxAndPredIdx (mouseRow, mouseCol, ref matrixRowIdx, ref predIDX);
      if  ((knownIDX < 0)  ||  (matrixRowIdx >= matrixRowClassAssigned.Count)  ||  (predIDX < 0)  ||  (predIDX >= numPredClasses))
        return;

      PicesClass  knownClass = matrixRowClassAssigned[matrixRowIdx];
      knownIDX = classesPredLevel.IndexOf (knownClass);
      if  (knownIDX < 0)
        return;

      ImageListDisplay  imagesDisplay = new ImageListDisplay (groundTruthRootDir,
                                                              imagesByPred[knownIDX][predIDX],
                                                              trainingModel
                                                             );
      imagesDisplay.ShowDialog ();
    }  /* ExploreMatrixCell */





    public  void  MakePrediction (PicesClass          knownClass,
                                  PicesClass          predictedClass,
                                  double              predProb,
                                  PicesFeatureVector  fv,
                                  TrainingModel2      trainModel
                                 )
    {
      bool  partOfOriginalClassifier = ClassInClassifierClasses (knownClass);
      if  (!partOfOriginalClassifier)
      {
        String  notPartClassName = "(V)" + knownClass.Name;
        knownClass = PicesClassList.GetUniqueClass (notPartClassName, "");
      }

      int  x = classes.LookUpIndex (knownClass);
      if  (x < 0)
        classes.Add (knownClass);

      x = classes.LookUpIndex (predictedClass);
      if  (x < 0)
        classes.Add (predictedClass);

      Prediction p = new Prediction (knownClass, predictedClass, predProb, fv, trainModel, partOfOriginalClassifier);
      predictions.Add (p);
    }  /* featureVector */



    private  void  ReCalcMatrix ()
    {
      TallyPredictions ();
    }  /* ReCalcMatrx */



    public  void  TallyPredictions ()
    {
      InitilizeDataStructures ();

      float  minProbFloat = (float)minProbability / 100.0f;
      float  maxProbFloat = (float)maxProbability / 100.0f;

      foreach  (Prediction p in predictions)
      {
        if  ((p.prob >= minProbFloat)  &&  (p.prob <= maxProbFloat))
          MakePrediction (p);

        else if  (p.prob < minProbFloat)
        {
          FailedProbabilityPrediction (p);
        }
      }

      PopulateMatrix ();
    }  /* TallyPredictions */



    private  void  FailedProbabilityPrediction (Prediction  p)
    {

    }  /* FailedProbabilityPrediction */



    private  PartialCreditIdx  DerivePartialCreditIdx (float grade)
    {
      if  (grade <= 0.0f)
        return PartialCreditIdx.None;

      if  (grade >= 1.0f)
        return PartialCreditIdx.Whole;

      if  (grade == (1.0f / 2.0f))
        return PartialCreditIdx.OneHalf;

      if  (grade == (1.0f / 3.0f))
        return PartialCreditIdx.OneThird;

      if  (grade == (2.0f / 3.0f))
        return PartialCreditIdx.TwoThirds;

      if  (grade == (1.0f / 4.0f))
        return PartialCreditIdx.OneForth;

      if  (grade == (2.0f / 4.0f))
        return PartialCreditIdx.TwoForths;

      if  (grade == (3.0f / 4.0f))
        return PartialCreditIdx.ThreeForths;

      return PartialCreditIdx.Whole;
    }


    private  String  PartialCreditIdxToStr (PartialCreditIdx  idx)
    {
      String  s = "None";
      switch  (idx)
      {
        case  PartialCreditIdx.None:        s = "None";        break;
        case  PartialCreditIdx.Whole:       s = "Whole";       break;
        case  PartialCreditIdx.OneHalf:     s = "OneHalf";     break;
        case  PartialCreditIdx.OneThird:    s = "OneThird";    break;
        case  PartialCreditIdx.TwoThirds:   s = "TwoThirds";   break;
        case  PartialCreditIdx.OneForth:    s = "OneForth";    break;
        case  PartialCreditIdx.TwoForths:   s = "TwoForths";   break;
        case  PartialCreditIdx.ThreeForths: s = "ThreeForths"; break;
      }

      return  s;
    }
    


    private  float  FractionThatIsAMatch (PicesClass knownClass,
                                          PicesClass predClass
                                         )
    {
      String[]  knownLevels = knownClass.NameUpper.Split ('_');
      String[]  predLevels  = predClass.NameUpper.Split ('_');

      if (knownLevels.Length < 1)
        return 0;

      int  zed = Math.Min (knownLevels.Length, predLevels.Length);
      int  numMatchedLevels = 0;
      for  (int x = 0;  x < zed;  ++x)
      {
        if  (knownLevels[x] == predLevels[x])
          numMatchedLevels = x;
        else
          break;
      }
      return (float)numMatchedLevels / (float)knownLevels.Length;
    }  /* FractionThatIsAMatch */




    /**
     *@details  All levels of the predicted class must match to get partial credit.
     */
    private  float  FractionThatIsAMatch2 (PicesClass knownClass,
                                           PicesClass predClass
                                          )
    {
      String[]  knownLevels = knownClass.NameUpper.Split ('_');
      String[]  predLevels  = predClass.NameUpper.Split ('_');

      if (knownLevels.Length < 1)
        return 0;

      int  zed = Math.Min (knownLevels.Length, predLevels.Length);
      int  numMatchedLevels = 0;
      for  (int x = 0;  x < zed;  ++x)
      {
        if  (knownLevels[x] == predLevels[x])
          ++numMatchedLevels;
        else
          break;
      }

      if  (numMatchedLevels < predLevels.Length)
        return 0.0f;
      else
        return (float)numMatchedLevels / (float)knownLevels.Length;
    }  /* FractionThatIsAMatch2 */



    private  bool  ClassInClassifierClasses (PicesClass c)
    {
      if  (c == otherClass)
        return true;

      bool  included = false;
      foreach  (PicesClass pc in classifierClasses)
      {
        if  ((c == pc)  ||  (pc.NameUpper.StartsWith (c.NameUpper)))
        {
          included = true;
          break;
        }
      }

      return included;
    }




    private  void  MakePrediction (Prediction p)
    {
      String  rootName = OSservices.GetRootName (p.FileName);

      PicesClass knownClassPredLevel = p.knowClass.MLClassForGivenHierarchialLevel (predLevel);
      PicesClass predClassPredLevel  = p.predClass.MLClassForGivenHierarchialLevel (predLevel);

      int  knowClassIdx = classesPredLevel.LookUpIndex (knownClassPredLevel);
      int  predClassIdx = classesPredLevel.LookUpIndex (predClassPredLevel);

      int  exactMatch = 0;
      float  partialCredit = 0.0f;
      if  (knowClassIdx == predClassIdx)
      {
        partialCredit = 1.0f;
        exactMatch = 1;
      }
      else
      {
        partialCredit = FractionThatIsAMatch2 (knownClassPredLevel, predClassPredLevel);
      }

      PartialCreditIdx  pcIdx = DerivePartialCreditIdx (partialCredit);

      totalByCorectLevel[(int)pcIdx]++;
      totalCount++;
      totalCorrect    += partialCredit;
      totalCorrectNPC += exactMatch;

      if  (!p.partOfClassifier)
      {
        totalNotPartOfClassifier++;
      }
      else
      {
        totalPartOfClassifier++;
        totalPartOfClassifierByCorectLevel[(int)pcIdx]++;
        totalPartOfClassifierCorrect += partialCredit;
        totalPartOfClassifierCorrectNPC += exactMatch;
       
        if  (p.knowClass.UnDefined)
        {
          totalNoise++;
          totalNoiseCorrect += partialCredit;
          totalNoiseCorrectNPC += exactMatch;
        }
        else
        {
          totalNonNoise++;
          totalNonNoiseCorrect += partialCredit;
          totalNonNoiseCorrectNPC += exactMatch;
        }
      }

      if  (knowClassIdx < 0)
      {
        MessageBox.Show ("Invalid Classs Specified[" + p.knowClass.Name + "]");
        return;
      }

      if  (predClassIdx < 0)
      {

        MessageBox.Show ("Invalid Classs Specified[" + p.predClass.Name + "]");
        return;
      }

      knownClassCounts[knowClassIdx]++;
      predClassCounts [predClassIdx]++;
      predictionCounts[knowClassIdx][predClassIdx]++;
      
      try {imagesByPred[knowClassIdx][predClassIdx].Add (rootName, p);}
      catch (Exception e)
      {
        MessageBox.Show (this, "Exception occured adding example[" + rootName + "]" + "\n" +
                               "Known Class[" + p.knowClass.Name + "] Pred Class[" + p.predClass.Name + "]" + "\n" +
                               "Exception [" + e.ToString () + "]" + "\n" +
                               e.StackTrace,
                         "Confusion Matrix",
                         MessageBoxButtons.OK
                        );
      }

    }  /* MakePrediction */




    private  void  PopulateProbabilityComboBoxes ()
    {
      maxProbability = Math.Min (maxProbability, 100);
      maxProbability = Math.Max (maxProbability, 1);

      minProbability = Math.Min (maxProbability - 1, minProbability);
      minProbability = Math.Max (minProbability, 0);
      
      MinProbability.Items.Clear ();
      int x = 0;
      while  (x < maxProbability)
      {
        MinProbability.Items.Add (x.ToString ());
        x += 10;
      }

      MaxProbability.Items.Clear ();
      x = 10 * ((minProbability / 10) + 1);
      while  (x <= 100)
      {
        MaxProbability.Items.Add (x.ToString ());
        x += 10;
      }

      MinProbability.Text = minProbability.ToString ();
      MaxProbability.Text = maxProbability.ToString ();
    }  /* PopulateProbabilityComboBoxes */




    private void ConfusionMatrix_Load(object sender, EventArgs e)
    {
      lastWidth   = Width;
      lastHeight  = Height;

      PopulateProbabilityComboBoxes ();
      TrainingLevel.Text = trainLevel.ToString ();

      PredictionLevel.Items.Clear ();
      for  (uint x = 1;  x <= trainLevel;  x++)
        PredictionLevel.Items.Add (x.ToString ());
      PredictionLevel.Text = predLevel.ToString ();

      TallyPredictions ();
    }

    
    private void ConfusionMatrix_SizeChanged(object sender, EventArgs e)
    {
      int  widthChange  = Width  - lastWidth;
      int  heightChange = Height - lastHeight;
 
      lastWidth  = Width;
      lastHeight = Height;

      Matrix.Width  += widthChange;
      Matrix.Height += heightChange;

      Summary.Top += heightChange;

      SaveButton.Top  += heightChange;
      SaveButton.Left += widthChange;
    }


    private void PercentagesButton_Click(object sender, EventArgs e)
    {
      displayPercentages = !displayPercentages;
      if  (displayPercentages)
        PercentagesButton.Text = "Counts";
      else
        PercentagesButton.Text = "Percentages";

      PopulateMatrix ();
    }



    private  bool  minProbabilityBeingValidated = false;

    private  void  ValidateMinProbability ()
    {
      if  (minProbabilityBeingValidated)
        return;

      minProbabilityBeingValidated = true;

      int minUpperLimit = maxProbability - 1;
      int zed = PicesKKStr.StrToInt (MinProbability.Text);
      if  ((zed < 0)  ||  (zed > minUpperLimit))
      {
        MessageBox.Show ("Minimum probability must be in range of [0 - " + minUpperLimit.ToString () + "]", "Min Probability");
        MinProbability.Text = minProbability.ToString ();
      }
      else
      {
        if  (zed != minProbability)
        {
          minProbability = zed;
          PopulateProbabilityComboBoxes ();
        }
      }

      minProbabilityBeingValidated = false;
    }  /* ValidateMinProbability */




    private  bool  maxProbabilityBeingValidated = false;

    private  void  ValidateMaxProbability ()
    {
      if  (maxProbabilityBeingValidated)
        return;

      maxProbabilityBeingValidated = true;

      int maxLowerLimit = minProbability + 1;
      int zed = PicesKKStr.StrToInt (MaxProbability.Text);
      if  ((zed < maxLowerLimit)  ||  (zed > 100))
      {
        MessageBox.Show ("Maximum probability must be in range of [" + maxLowerLimit.ToString () + " - 100]", "Max Probability");
        MaxProbability.Text = maxProbability.ToString ();
      }
      else
      {
        if  (zed != maxProbability)
        {
          maxProbability = zed;
          PopulateProbabilityComboBoxes ();
        }
      }

      maxProbabilityBeingValidated = false;
    }  /* ValidateMaxProbability */


    private  String  FormatNameForHTML (String  n)
    {
      String[] parts = n.Split ('_');
      if  (parts.Length == 0)
        return String.Empty;

      String  htmlName = parts[0];
      for (int x = 1;  x < parts.Length;  x++)
        htmlName += ("<br />" + parts[x]);
      return  htmlName;
    }


    private  void  SaveAsHTML (System.IO.StreamWriter  sw)
    {
      String  configFileName = trainingModel.ConfigFileName;
      String  svmParamStr    = trainingModel.ParameterStr;


      sw.WriteLine ("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">");
      sw.WriteLine ("<html  xmlns=\"http://www.w3.org/1999/xhtml\">");
      sw.WriteLine ("<head>");
      sw.WriteLine ("<title>Grading Report</title>");
      sw.WriteLine ("</head>");
      sw.WriteLine ("<body style=\"text-align:center\">");
      sw.WriteLine ();

      sw.WriteLine ("<h1>Run Time Parameters</h1>");
      sw.WriteLine ("<table align=\"center\" border=\"2\" cellpadding=\"4\">");
      sw.WriteLine ("<caption>Run Time Parameters</caption>");
      sw.WriteLine ("<tr><td>Run Time</td>"                 +  "<td>" + DateTime.Now.ToString ()              + "</td></tr>");
      sw.WriteLine ("<tr><td>Ground Truch Directory</td>"   +  "<td>" + groundTruthRootDir                    + "</td></tr>");
      sw.WriteLine ("<tr><td>Config File Name</td>"         +  "<td>" + configFileName                        + "</td></tr>");
      sw.WriteLine ("<tr><td>SVM Parameters</td>"           +  "<td>" + svmParamStr                           + "</td></tr>");
      sw.WriteLine ("<tr><td>Training Level</td>"           +  "<td>" + trainLevel.ToString ()                + "</td></tr>");
      sw.WriteLine ("<tr><td>Prediction Level</td>"         +  "<td>" + predLevel.ToString ()                 + "</td></tr>");
      sw.WriteLine ("<tr><td>Minimum Probaility</td>"       +  "<td>" + minProbability.ToString ()            + "</td></tr>");
      sw.WriteLine ("<tr><td>Maximum Probaility</td>"       +  "<td>" + maxProbability.ToString ()            + "</td></tr>");
      sw.WriteLine ("</table>");
      sw.WriteLine ("<br>");

      sw.WriteLine ("<table align=\"center\" border=\"2\" cellpadding=\"4\">");
      sw.WriteLine ("<tr><td>Total Images</td>"             +  "<td>" + totalCount.ToString ()                + "</td></tr>");
      sw.WriteLine ("<tr><td>Total Part of Class</td>"      +  "<td>" + totalPartOfClassifier.ToString ()     + "</td></tr>");
      sw.WriteLine ("<tr><td>Total Not Part of Class</td>"  +  "<td>" + totalNotPartOfClassifier.ToString ()  + "</td></tr>");
      sw.WriteLine ("<tr><td>Total Non Noise</td>"          +  "<td>" + totalNonNoise.ToString ()             + "</td></tr>");
      sw.WriteLine ("<tr><td>Total Noise</td>"              +  "<td>" + totalNoise.ToString ()                + "</td></tr>");
      sw.WriteLine ("<tr><td>Part of Classifier %</td>"     +  "<td>" + percentPartOfClass.ToString ("P2")    + "</td></tr>");
      sw.WriteLine ("<tr><td>Not Part of Classifier %</td>" +  "<td>" + percentNotPartOfClass.ToString ("P2") + "</td></tr>");
      sw.WriteLine ("<tr><td>Non Noise %</td>"              +  "<td>" + percentNonNoise.ToString ("P2")       + "</td></tr>");
      sw.WriteLine ("<tr><td>Noise %</td>"                  +  "<td>" + percentNoise.ToString ("P2")          + "</td></tr>");
      sw.WriteLine ("</table>");
      sw.WriteLine ("<br>");

      sw.WriteLine ("<table align=\"center\" border=\"2\" cellpadding=\"4\">");
      sw.WriteLine ("<tr><td></td>"                         +  "<td>" + "With Partial Credit"                 + "</td><td>" + "NO Partial Credit"                    + "</td></tr>");
      sw.WriteLine ("<tr><td>Accuracy All</td>"             +  "<td>" + accuracyAll.ToString ("P2")           + "</td><td>" + accuracyAllNPC.ToString ("P2")         + "</td></tr>");
      sw.WriteLine ("<tr><td>Accuracy Part of Class</td>"   +  "<td>" + accuracyPartOfClass.ToString ("P2")   + "</td><td>" + accuracyPartOfClassNPC.ToString ("P2") + "</td></tr>");
      sw.WriteLine ("<tr><td>Accuracy Non Noise</td>"       +  "<td>" + accuracyNonNoise.ToString ("P2")      + "</td><td>" + accuracyNonNoiseNPC.ToString ("P2")    + "</td></tr>");
      sw.WriteLine ("<tr><td>Accuracy Noise</td>"           +  "<td>" + accuracyNoise.ToString ("P2")         + "</td><td>" + accuracyNoiseNPC.ToString ("P2")       + "</td></tr>");

      //sw.WriteLine ("<tr><td>Accuracy All - NPC</td>"           +  "<td>" + accuracyAllNPC.ToString ("P2")           + "</td></tr>");
      //sw.WriteLine ("<tr><td>Accuracy Part of Class - NPC</td>" +  "<td>" + accuracyPartOfClassNPC.ToString ("P2")   + "</td></tr>");
      //sw.WriteLine ("<tr><td>Accuracy Non Noise - NPC</td>"     +  "<td>" + accuracyNonNoiseNPC.ToString ("P2")      + "</td></tr>");
      //sw.WriteLine ("<tr><td>Accuracy Noise - NPC</td>"         +  "<td>" + accuracyNoiseNPC.ToString ("P2")         + "</td></tr>");

      sw.WriteLine ("</table>");

      sw.WriteLine ("<h1>Confusion Matrix.</h1>");
      sw.WriteLine ("<table  align=\"center\" border=\"2\" cellpadding=\"3\" cellspacing=\"0\" frame=\"box\"  summary=\"Confusion Matrix\">");
      sw.WriteLine ("  <thead>");
      sw.Write     ("    <tr><th>Class<br />Name</th><th>Count</th>");
      foreach  (PicesClass c in classesPredLevel)
        sw.Write ("<th>" + FormatNameForHTML (c.Name) + "</th>");

      sw.WriteLine ("</tr>");
      sw.WriteLine ("</thead>");
      sw.WriteLine ("  <tbody style=\"font-weight:normal; text-align:center; font-family:Courier\">");

      for  (int x = 0;  x < numPredClasses;  x++)
      {
        PicesClass  knowClass = classesPredLevel[x];
        
        sw.Write ("    <tr><td style=\"text-align:left; font-family:Arial\">" +  knowClass.Name + "</td>" + 
                           "<td>" + MatrixFieldStr (knownClassCounts[x], totalCount)            + "</td>");

        for  (int y = 0;  y < numPredClasses;  y++)
        {
          if  (x == y)
          {
            sw.Write ("<td style=\"font-weight:bold\">");
            if  (ClassInClassifierClasses (knowClass))
              sw.Write (MatrixFieldStr (predictionCounts[x][y], knownClassCounts[x]) + "</td>");
            else
              sw.Write (MatrixFieldStr (predictionCounts[x][y], knownClassCounts[x]) + "v" + "</td>");
          }
          else
          {
            sw.Write ("<td>");
            sw.Write (MatrixFieldStr (predictionCounts[x][y], knownClassCounts[x]) + "</td>");
          }
        }
        sw.WriteLine ("</tr>");
      }

      sw.Write ("<tr><td>Totals</td><td>" + MatrixFieldStr (totalCount, totalCount) + "</td>");
      for  (int y = 0;  y < numPredClasses;  y++)
      {
        sw.Write ("<td>" + MatrixFieldStr (predClassCounts [y], totalCount) + "</td>");
      }
      sw.WriteLine ("</tr>");

      sw.WriteLine ("</tbody>");
      sw.WriteLine ("</table>");
    }  /* SaveAsHTML */


    private  void  SaveAsText (System.IO.StreamWriter  sw)
    {
      String  configFileName = trainingModel.ConfigFileName;
      String  svmParamStr    = trainingModel.ParameterStr;

      sw.WriteLine ("Run Time Parameters");
      sw.WriteLine ("Run Time"                 +  "\t" + DateTime.Now.ToString ());
      sw.WriteLine ("Ground Truch Directory"   +  "\t" + groundTruthRootDir);
      sw.WriteLine ("Config File Name"         +  "\t" + configFileName);
      sw.WriteLine ("SVM Parameters"           +  "\t" + svmParamStr);
      sw.WriteLine ("Training Level"           +  "\t" + trainLevel.ToString ());
      sw.WriteLine ("Prediction Level"         +  "\t" + predLevel.ToString ());
      sw.WriteLine ();
      sw.WriteLine ("Total Images"             +  "\t" + totalCount.ToString ()                );
      sw.WriteLine ("Total Part of Class"      +  "\t" + totalPartOfClassifier.ToString ()     );
      sw.WriteLine ("Total Not Part of Class"  +  "\t" + totalNotPartOfClassifier.ToString ()  );
      sw.WriteLine ("Total Non Noise"          +  "\t" + totalNonNoise.ToString ()             );
      sw.WriteLine ("Total Noise"              +  "\t" + totalNoise.ToString ()                );
      sw.WriteLine ();
      sw.WriteLine ("Part of Classifier %"     +  "\t" + percentPartOfClass.ToString ("P2")    );
      sw.WriteLine ("Not Part of Classifier %" +  "\t" + percentNotPartOfClass.ToString ("P2") );
      sw.WriteLine ("Non Noise %"              +  "\t" + percentNonNoise.ToString ("P2")       );
      sw.WriteLine ("Noise %"                  +  "\t" + percentNoise.ToString ("P2")          );
      sw.WriteLine ();
      sw.WriteLine ("Accuracy All"             +  "\t" + accuracyAll.ToString ("P2")           );
      sw.WriteLine ("Accuracy Part of Class"   +  "\t" + accuracyPartOfClass.ToString ("P2")   );
      sw.WriteLine ("Accuracy Non Noise"       +  "\t" + accuracyNonNoise.ToString ("P2")      );
      sw.WriteLine ("Accuracy Noise"           +  "\t" + accuracyNoise.ToString ("P2")         );
      sw.WriteLine ();
      sw.WriteLine ("Accuracy All - NPC"           +  "\t" + accuracyAllNPC.ToString         ("P2"));
      sw.WriteLine ("Accuracy Part of Class - NPC" +  "\t" + accuracyPartOfClassNPC.ToString ("P2"));
      sw.WriteLine ("Accuracy Non Noise - NPC"     +  "\t" + accuracyNonNoiseNPC.ToString    ("P2"));
      sw.WriteLine ("Accuracy Noise - NPC"         +  "\t" + accuracyNoiseNPC.ToString       ("P2"));
      sw.WriteLine ();
      sw.WriteLine ();
      sw.WriteLine ("Confusion Matrix");
      sw.WriteLine ();
      sw.Write     ("Class_Name" + "\t" + "Count");
      foreach  (PicesClass c in classesPredLevel)
        sw.Write ("\t" + c.Name);
      sw.WriteLine ();

      for  (int x = 0;  x < numPredClasses;  x++)
      {
        PicesClass  knowClass = classesPredLevel[x];
        sw.Write (knowClass.Name + "\t" + MatrixFieldStr (knownClassCounts[x], totalCount));

        for  (int y = 0;  y < numPredClasses;  y++)
        {
          sw.Write ("\t" + MatrixFieldStr (predictionCounts[x][y], knownClassCounts[x]));
          if  (x == y)
          {
            if  (!ClassInClassifierClasses (knowClass))
              sw.Write ("V");
          }
        }

        sw.WriteLine ();
      }

      sw.Write ("Totals" + "\t" + MatrixFieldStr (totalCount, totalCount));
      for  (int y = 0;  y < numPredClasses;  y++)
        sw.Write ("\t" + MatrixFieldStr (predClassCounts [y], totalCount));
      sw.WriteLine ();
      sw.WriteLine ("Accuracy All"       +  "\t" + accuracyAll.ToString ("P2"));
      sw.WriteLine ("Accuracy All - NPC" +  "\t" + accuracyAllNPC.ToString ("P2"));

      sw.WriteLine ();
      sw.WriteLine ();
      sw.WriteLine ();
      sw.WriteLine ();
      sw.WriteLine ("Non Noise Confusion Matrix");
      sw.WriteLine ();
      sw.Write     ("Class_Name" + "\t" + "Count");
      foreach  (PicesClass c in classesPredLevel)
        sw.Write ("\t" + c.Name);
      sw.WriteLine ();

      for  (int x = 0;  x < numPredClasses;  x++)
      {
        PicesClass  knowClass = classesPredLevel[x];
        if  (!ClassInClassifierClasses (knowClass))
          continue;

        if  (knowClass.UnDefined)
          continue;

        sw.Write (knowClass.Name + "\t" + MatrixFieldStr (knownClassCounts[x], totalNonNoise));

        for  (int y = 0;  y < numPredClasses;  y++)
        {
          sw.Write ("\t" + MatrixFieldStr (predictionCounts[x][y], knownClassCounts[x]));
          if  (x == y)
          {
            if  (!ClassInClassifierClasses (knowClass))
              sw.Write ("V");
          }
        }

        sw.WriteLine ();
      }

      sw.Write ("Totals" + "\t" + MatrixFieldStr (totalNonNoise, totalNonNoise));
      for  (int y = 0;  y < numPredClasses;  y++)
        sw.Write ("\t" + MatrixFieldStr (predClassCounts [y], totalNonNoise));
      sw.WriteLine ();

      sw.WriteLine ("Non Noise Accuracy" + "\t" + accuracyNonNoise.ToString ("P2"));
      sw.WriteLine ("\n\n\n\n");
      sw.WriteLine ("Variable Definitions");
      sw.WriteLine ("" + "\t" + "");
      sw.WriteLine ("Total-Examples"             + "\t" + "Total number of predictions; same as \"Total-Images\"");
      sw.WriteLine ("Part-of-Classifier"         + "\t" + "True if:  Known class is (Defined as Other Class)  or  (any class in the classifier starts with the same name).");
      sw.WriteLine ("Total-Part-of-Classifier"   + "\t" + "Total number of examples that are “Part-of-Classifier”.");
      sw.WriteLine ("Noise-Class"                + "\t" + "Class considered noise if its highest level is one of the following: \"UNKNOWN\", \"UNDEFINED\",  \"NONPLANKTON\",  \"NOISE\";  ex:  “Noise-Lines” would be considered a noise class.");
      sw.WriteLine ("Known-Class"                + "\t" + "The class that an example was ground-truthed as.");
      sw.WriteLine ("Percent-Part-of-Classifier" + "\t" + "Total number of predictions that are considered ‘Part-of-Classifier’ divided by ‘Total-num-of-Predictions’.");
      sw.WriteLine ("Total-Noise"                + "\t" + "Total number of examples where the \"Known-Class\" was considered \"Part-of-Classifier\" and \"Noise-Class\".");
      sw.WriteLine ("Percent-Noise"              + "\t" + "Fraction of All-Examples that are of a \"Known-Class\" that is flagged as \"Noise-Class\".");
      sw.WriteLine ("Total-Non-Noise"            + "\t" + "Total number of examples where the \"Known-Class\" was considered \"Part-of-Classifier\" and NOT \"Class-Considered-Noise\".");
      sw.WriteLine ("Percent-Non-Noise"          + "\t" + "Fraction of All-Examples that are of a Known Class that is NOT flagged as \"Noise-Class\".");
      sw.WriteLine ();
      sw.WriteLine ("        Note:   \"Total-Noise\"   + \"Total-Non-Noise\"    =  \"Total-Part-of-Classifier\"");
      sw.WriteLine ("        Note:   \"Percent-Noise\" + \"Percent-Non-Noise\"  =  \"Percent-Part-of-Classifier\"");
      sw.WriteLine ();
      sw.WriteLine ("Total-Correct"               + "\t" + "The number of predictions that were correct;  note that when utilizing Dual classifier there can be partial credit when only part of the predicted class equals part of the known class.");
      sw.WriteLine ("Accuracy-All"                + "\t" + "Total-Correct / Total Predicted.");
      sw.WriteLine ("Accuracy-Part-of-Classifier" + "\t" + "Of the examples where known class was \"Part-of-Classifier\"  the number that were predicted correctly / total number that were considered \"Part-of-Classifier\".");
      sw.WriteLine ("Total-Non-Noise-Correct"     + "\t" + "Number examples that are NOT a \"Noise-Class\" and \"Part-of-Classifier\".");
      sw.WriteLine ("Total-Noise-Correct"         + "\t" + "Number examples that are \"Noise-Class\" and \"Part-of-Classifier\".");
      sw.WriteLine ("Accuracy-Non-Noise"          + "\t" + "\"Total-Non-Noise-Correct\" / \"Total-Part-of-Classifier\"");
      sw.WriteLine ("Accuracy-Noise"              + "\t" + "\"Total-Noise-Correct\" / \"Total-Noise\"");
    }  /* SaveAsText */



    private void MinProbability_Leave(object sender, EventArgs e)
    {
      ValidateMinProbability ();
    }


    private void MaxProbability_Leave(object sender, EventArgs e)
    {
      ValidateMaxProbability ();
    }


    private void ReTallyButton_Click(object sender, EventArgs e)
    {
      uint zed = PicesKKStr.StrToUint (PredictionLevel.Text);
      if  (zed < 1)
        predLevel = 1;

      else if (zed > trainLevel)
        predLevel = trainLevel;

      else
        predLevel = zed;

      PredictionLevel.Text = predLevel.ToString ();

      ReCalcMatrix ();
    }



    private void Matrix_MouseDoubleClick(object sender, MouseEventArgs e)
    {
      int  mouseRow = e.Y;
      int  mouseCol = e.X;

      ExploreMatrixCell (mouseRow, mouseCol);
    }



    private void SaveButton_Click (object sender, EventArgs e)
    {
      SaveFileDialog  sfd = new SaveFileDialog ();
      sfd.CheckPathExists = true;
      sfd.Filter = "txt file (*.txt)|*.txt|html (*.html)|*.html";
      sfd.Title  = "Save Confusion Matrix";
      sfd.DefaultExt = "txt";
      sfd.AddExtension = true;
      sfd.FileName = OSservices.GetRootName (trainingModel.ConfigFileName);
      DialogResult dr = sfd.ShowDialog (this);
      if  (dr == DialogResult.OK)
      {
        String  extension = OSservices.GetFileExtension (sfd.FileName).ToUpper ();
        StreamWriter  sr = null;
        try  {sr = new StreamWriter (sfd.FileName);} catch (Exception) {sr = null;}
        if  ((extension == @"HTML")  ||  (extension == @"HTM"))
          SaveAsHTML (sr);
        else
          SaveAsText (sr);
        
        sr.Close ();
        sr.Dispose ();
        sr = null;
      }
    }

  }  /* ConfusionMatrix */
}