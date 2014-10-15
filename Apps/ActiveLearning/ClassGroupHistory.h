#ifndef  _CLASSGROUPHISTORY_
#define  _CLASSGROUPHISTORY_


//**************************************************************************************
//*                                 ClassGroupHistory                                  *
//*                                                                                    *
//*   There are several classes defined in this module,  there purpose is to keep      *
//* track of the activity that is occurig in  the ClassGroupTotals object.  The idea   *
//* is that the ClassGroupTotals object will use one instance of a ClassGroupHistory   *
//* derived object for each eace of information that ClassGroupTotals is tracking.     *
//*                                                                                    *
//* As active learning records data in ClassGroupTotals and oinstance of a History     *
//* object will be created.  These instances will later be written to a history file   *
//* for later processing.  This way ActiveLearninging can be ran as mutiple modules    *
//* where each one records its results in a seperate history file to be processed      *
//* later.  When all the different instances of ActiveLearning are dome a single pro-  *
//* gram can load all the data in the history files and combine there results for a    *
//* final report.                                                                      *
//*                                                                                    *
//*    HistoryBit - A abstract class that all history objects are derived from.        *
//*                                                                                    *
//*    PredictionHistoryBit - Records the results of one prediction against the test   *
//*                           data.                                                    *
//*                                                                                    *
//*    ProbabilityHistoryBit - For each test image that is selected by ActiveLearning  *
//*                            to be added to the training library.                    *
//*                                                                                    *
//*    SortMethodHistoryBit  - Specifies which Sortmethod was used for active learning *
//*                            As of 2004-09-02 it does not appear to be used.         *
//*                            Beleive the original concept was taht we might try      *
//*                            to mix the sort methods in the same run.                *
//*                                                                                    *
//*    SupportVectorsHistoryBit - We record the number of support points are created   *
//*                            when building the models for a given Retraining pass.   *
//*                                                                                    *
//**************************************************************************************


#include "MLClass.h"
using namespace MLL;



#define  OURFLOAT  float

typedef  enum  {NULLhist,
                PredictionHist, 
                PredictionHist2,  // This is a replacement for PredictionHist,  it summarizes rather that stores each seperate predictio.
                ProbabilityHist,
                SortMethodHist,
                SupportVectorsHist
               }                 HistoryType;


#include  "SortOrderType.h"



#ifndef  _CLASSGROUPTOTALS_
class  ClassGroupTotals;
typedef  ClassGroupTotals*  ClassGroupTotalsPtr;
#endif




class  HistoryBit
{
public:
  HistoryBit ();

  virtual KKStr  ToString () = 0;

  virtual HistoryType  TypeHistory () = 0;

  virtual void  UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                   int                  group,
                                   int                  pass)  = 0;

};


typedef  HistoryBit*  HistoryBitPtr;



class  PredictionHistoryBit:  public  HistoryBit
{
public:
   PredictionHistoryBit (MLClassPtr  _knownClass,
                         MLClassPtr  _predClass,
                         OURFLOAT       _probability
                        ):
              knownClass  (_knownClass),
              predClass   (_predClass),
              probability (_probability)
     {}

   PredictionHistoryBit (MLClassList&  imageClasses, 
                         KKStr           l);

   MLClassPtr  KnownClass ()   {return  knownClass;}

   MLClassPtr  PredClass  ()   {return  predClass;}

   OURFLOAT       Probability ()  {return  probability;}
   

   virtual KKStr  ToString ();
      
   virtual HistoryType  TypeHistory () {return  PredictionHist;}


   virtual void  UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                    int                  group,
                                    int                  pass);

   int  Compare (PredictionHistoryBit* r);




private:
  MLClassPtr  knownClass;
  MLClassPtr  predClass;
  OURFLOAT       probability;
};


typedef  PredictionHistoryBit*  PredictionHistoryBitPtr;




class  ProbabilityHistoryBit:  public  HistoryBit
{
public:
   ProbabilityHistoryBit (MLClassPtr  _mlClass,
                          OURFLOAT       _probability,
                          const KKStr&  _imageFileName,
                          int            _position,
                          float          _percentile
                         ):
              mlClass    (_mlClass),
              probability   (_probability),
              imageFileName (_imageFileName),
              position      (_position),
              percentile    (_percentile)
     {}


   const KKStr&  ImageFileName ()   {return  imageFileName;}


   ProbabilityHistoryBit (MLClassList&  imageClasses, 
                          KKStr           l
                         );


   virtual
   KKStr  ToString ();
      
   virtual  
   HistoryType  TypeHistory () {return  ProbabilityHist;}



   virtual void  UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                    int                  group,
                                    int                  pass
                                   );

   int  Compare (ProbabilityHistoryBit* r);

private:
  MLClassPtr  mlClass;
  OURFLOAT       probability;
  KKStr         imageFileName;    // Name of Image that is being added to Training Library
  int            position;         // Used when doing stream learning,  reflects the relative
                                   // position tthis image would have if not using streaming
  float          percentile;       // Where positrion was located, by percentage
};


typedef  ProbabilityHistoryBit*  ProbabilityHistoryBitPtr;







class  SortMethodHistoryBit:  public  HistoryBit
{
public:
   SortMethodHistoryBit (MLClassPtr   _mlClass,
                         SortOrderType   _sortMethod
                        );

   SortMethodHistoryBit (MLClassList&  imageClasses, 
                         KKStr           l
                        );
      
   virtual
   KKStr  ToString ();
      
   virtual  
   HistoryType  TypeHistory () {return  SortMethodHist;}

   virtual void  UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                    int                  group,
                                    int                  pass
                                   );

   int  Compare (SortMethodHistoryBit* r);


private:
  MLClassPtr  mlClass;
  SortOrderType  sortMethod;
};


typedef  SortMethodHistoryBit*   SortMethodHistoryBitPtr;






class  SupportVectorsHistoryBit:  public  HistoryBit
{
public:
   SupportVectorsHistoryBit (int   _group,
                             int   _pass,
                             int   _numSupportVectors,
                             float _trainingTime,
                             float _testingTime
                            ):
             group             (_group),
             pass              (_pass),
             numSupportVectors (_numSupportVectors),
             trainingTime      (_trainingTime),
             testingTime       (_testingTime)
       {}

   SupportVectorsHistoryBit (KKStr  l);

   virtual KKStr  ToString ();

   virtual HistoryType  TypeHistory () {return  SupportVectorsHist;}

   virtual void  UpdateClassTotals (ClassGroupTotalsPtr  totals,
                                    int                  group,
                                    int                  pass);


   int  Compare (SupportVectorsHistoryBit*  r);



private:
  int    group;
  int    pass;
  uint   numSupportVectors;
  float  trainingTime;
  float  testingTime;

};


typedef  SupportVectorsHistoryBit*  SupportVectorsHistoryBitPtr;





class  HistoryBitList:  public KKQueue<HistoryBit>
{
public:
  HistoryBitList (bool  _owner,  int  _size):
     KKQueue<HistoryBit> (_owner, _size)
     {}

  void  WriteOutContents (ostream&  o);


  void  HistoryBitList::SortHistory ();

};


typedef  HistoryBitList*  HistoryBitListPtr;
#endif
