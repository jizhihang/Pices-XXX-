#ifndef  _RESULTLINE_
#define  _RESULTLINE_


#include  "ConfusionMatrix2.h"
#include  "CrossValidation.h"
#include  "FeatureNumList.h"
#include  "FileDesc.h"
#include  "MLClass.h"
#include  "RBTree.h"
#include  "RunLog.h"


using namespace MLL;


class  ResultLine;
typedef  ResultLine* ResultLinePtr;
class  ResultLineList;
typedef  ResultLineList*  ResultLineListPtr;


class  ResultLineTree;
typedef  ResultLineTree*  ResultLineTreePtr;


//**********************************************************************************
//  Author:     Kurt Kramer                                                        *
//  Date        July-16-2005                                                       *
//---------------------------------------------------------------------------------*
// Three major classes defined in this header file.                                *
//                                                                                 *
//    ResultLine     - Represents the evaluation of one combination of features.   *
//                                                                                 *
//    ResultLineTree - A tree structure that conatins instances of 'ResultLine'    *
//                     where 'id' from ResultLine is used as index.                *
//                                                                                 *
//    ResultLineList - A structure used to contain lists of 'ResultLine' instances.*
//                     Used by several methods. Sub-Classed from 'KKQueue<>'       *
//**********************************************************************************





//********************************** ResultLine ************************************
//*  Represents the results of one feature combination.  Every time RandomSearch   *
//*  evaluates a feature set, it will create an instance of this object and add it *
//*  to 'results' a 'ResultLineTree' structure.                                    *
//**********************************************************************************
class ResultLine
{
public:
  ResultLine (int                 _id,
              ResultLinePtr       _parent1,
              ResultLinePtr       _parent2,
              ResultLinePtr       _family,
              CrossValidation&    _cv,
              FeatureNumListPtr   _features,
              RunLog&             log
             );

  ResultLine (KKStr           txt,
              ResultLineTree&  results
             );

  ~ResultLine(void);


  float                    Accuracy () const {return accuracy;}

  float                    AccuracyWeighted () const  {return accuracyWeighted;}

  float                    ClassTime () const {return classTime;}

  bool                     Expanded () const {return expanded;}

  ResultLinePtr            Family   () const {return family;}

  int                      FamilyId () const {return family ? family->Id () : id;}

  const FeatureNumListPtr  Features () const {return features;}

  int                      Id       () const {return id;}

  int                      NumOfFeatures () const  {return features->NumOfFeatures ();}

  ResultLinePtr            Parent1   () const {return parent1;}
  ResultLinePtr            Parent2   () const {return parent2;}

  int                      Parent1Id () const {return parent1 ? parent1->Id () : -1;}
  int                      Parent2Id () const {return parent2 ? parent2->Id () : -1;}

  static KKStr             TitleLine (MLClassList&  mlClasses);

  KKStr                    ToString ();

  float                    TrainTime () const {return trainTime;}

  bool                     Verified  () const  {return verified;}



  void     Accuracy         (float _accuracy)          {accuracy         = _accuracy;}
  void     AccuracyWeighted (float _accuracyWeighted)  {accuracyWeighted = _accuracyWeighted;}
  void     Expanded         (bool  _expanded)          {expanded         = _expanded;}
  void     Verified         (bool  _verified)          {verified         = _verified;}


  void  BuildExtendedFamilyList (int           deapth,
                                 vector<int>&  families
                                );



private:

  ResultLinePtr  TrackDownFamily ();


  static  FileDescPtr     fileDesc;
  static  MLClassListPtr  mlClasses;
  static  int             numOfClasses;
  static  RunLog&         log;

  static
  void  InitializeMLClasses (FileDescPtr   _fileDesc,
                                MLClassList&  _mlClasses,
                                RunLog&       _log
                               );

  friend class ResultLineTree;

  float              accuracy;
  float              accuracyWeighted;   // Weightest such that each class has equal weight
  float*             classAccuracies;
  float              classTime;

  bool               expanded;           // If set to true, meens that this ResultLine
                                         // has been trimmed(Expanded).

  ResultLinePtr      family;             // Pointer to result line that this 'ResultLine'
                                         // ultimatly decends from. This could be either 
                                         // an originally RandomLy selected set of 
                                         // features, or the union of two other 'ResultLine'

  FeatureNumListPtr  features;           // ResulLine will own this member.

  int                id;                 // Unique number, no other instance of this 
                                         // object will have the same id.


  //  The two members below  'parent1' and 'parent2' are used to
  // keep track of lineage.
  //  
  //  parent1  parent2  Description
  //    NULL     NULL   Created by random selection of features.
  //    xxxx     NULL   An imedeiate decendent as a result of trimming.
  //    xxxx     xxxx   The result of combining two other ResultLine.

  ResultLinePtr      parent1;            
  ResultLinePtr      parent2;

  float              trainTime;

  bool               verified;
};


typedef  ResultLine*            ResultLinePtr;
typedef  vector<ResultLinePtr>  ResultLineVector;
typedef  pair<ResultLinePtr, ResultLinePtr> ResultLinePair;
typedef  vector<ResultLinePair>  ResultLinePairVector;


ResultLineVector  operator+ (ResultLineVector&  left,  
                             ResultLineVector&  right
                            );


class Parents
{
public:
  Parents  (int  _p1,  int  _p2);

  int  P1 () const {return p1;}
  int  P2 () const {return p2;}

  bool  operator== (const Parents&  right)  const;
  bool  operator!= (const Parents&  right)  const;
  bool  operator<  (const Parents&  right)  const;
  bool  operator>  (const Parents&  right)  const;

private:
  int  p1;
  int  p2;
};




class  CompareResultLine 
{
public:
  const FeatureNumList& ExtractKey (const ResultLinePtr  resultLine)
  {
    return (*(resultLine->Features ()));
  }
};  /* CompareResultLine */






//******************************** ResultLineTree **********************************
//*                                                                                *
//*  used to keep track of 'ResultLine' objects.  It is subclassed from 'RBTree'   *
//*  with Features form 'ResultLine' being used as index.  'RandomSearch will      *
//*  have one instance of this object (results).  This will allow the RandomSearch *
//*  to determine is a given feature combination was used before.  There are also  *
//*  two other indexes  'Id' and 'ParentId' using stl::map<> template.             *
//*                                                                                *
//*                                                                                *
//**********************************************************************************
class  ResultLineTree: public  RBTree<ResultLine, CompareResultLine, FeatureNumList>
{
public:
  ResultLineTree (bool               _owner,
                  RunLog&            _log
                 );

  ResultLineTree (KKStr             _fileName,
                  FileDescPtr        _fileDesc,
                  MLClassList&    _mlClasses,
                  bool&              _successful,
                  RunLog&            _log
                 );

  float   HighestAccuracy         () const  {return highestAccuracy;}
  float   HighestAccuracyWeighted () const  {return highestAccuracyWeighted;}
  int     NextId                  () const  {return nextId;}

  ResultLinePtr  LookUpById (int id);

  ResultLinePtr  LookUpByParents (const ResultLinePtr p1,
                                  const ResultLinePtr p2
                                 );

  ResultLineListPtr  GetChildrenOfAParent (ResultLinePtr  parent);


  void  Save (const KKStr& fileName,
              bool&         successful
             );

  RBTree<ResultLine, CompareResultLine, FeatureNumList>::NodePtr
      RBInsert (ResultLinePtr  result);



private:
  void  Load (const KKStr& fileName,
              bool&         successful
             );

  float                             highestAccuracy;
  float                             highestAccuracyWeighted;
  RunLog&                           log;
  int                               nextId;
  map<int, ResultLinePtr>           idIndex;
  multimap<Parents, ResultLinePtr>  parentIndex;
  ResultLinePtr                     resultWithHighestAccuracy;
};


typedef  ResultLineTree*  ResultLineTreePtr;

typedef  Iterator<ResultLine, CompareResultLine, FeatureNumList>  ResultLineTreeIterator;









class  ResultLineList: public KKQueue<ResultLine>
{
public:
  ResultLineList ();

  ResultLineList (bool _bool,
                  int  _size
                 );

  ResultLineList (ResultLineTree*  resultLineTree);

  ResultLineList (ResultLineTree*  resultLineTree,
                  int              maxSize,
                  int              maxId = -1
                 );


  void  SortByAccuracy (bool reversed,
                        bool weighted
                       );

private:
};

typedef  ResultLineList*  ResultLineListPtr;




#endif
