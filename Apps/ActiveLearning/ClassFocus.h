#ifndef  _CLASSFOCUS_
#define  _CLASSFOCUS_


#include  "SortOrderType.h"
#include  "TrainingImage.h"


typedef  std::vector<SortOrderType>   VectorSortOrder;

typedef  vector<KKStr>  VectorString;




class  ClassFocus
{
public:
   ClassFocus (int  numOfClasses);
   
   ~ClassFocus ();



   void           AddIn (ClassFocus&  cf);

   void           CalcAvgProb (int     classIDX, 
                               uint&   numOfProbs,
                               float&  avgProbability
                              );

   uint           Count ();

   uint           Count (int  classIDX);

   uint           Correct ();

   //  Info about images added to Training Library for a given randomPass and retraining.
   TrainingImageList&   TrainingImages  ()  {return trainingImages;}

   uint           NumOfProbabilities ();

   uint           NumOfProbabilities (int classIDX);

   uint           NumOfSupportVectors ()     {return  numOfSupportVectors;}

   uint*          Predicted (int knownIDX)   {return  predicted[knownIDX];}

   uint           Predicted (int knownIDX,
                             int predIDX
                            )                {return  predicted[knownIDX][predIDX];}

   SortOrderType  SortMethodUsed   (int classIDX);


   float          TrainingTime ()  {return  trainingTime;}

   float          TestingTime ()   {return  testingTime;}


   void     AddProbability (int            _classIDX,
                            ImageClassPtr  _imageClass,
                            const KKStr&  _imageFileNname,
                            float          _probability,
                            int            _nonStreamPlace,
                            float          _percentile
                           );


   int     GetNumNewImagesInClass (int _classIDX);   // returns the number of images added to the 
                                                     // training library for the given class.


   void    NumOfSupportVectors (int   _numOfSupportVectors,
                                float _trainingTime,
                                float _testingTime
                               );
 
   void    Prediction (int    knownIDX,
                       int    predIDX,
                       float  probability
                      );

   float   Probabilities       (int  classIDX,
                                uint idx
                               );

   void    SetSortMethodUsed   (int            classIDX, 
                                SortOrderType  sortMethod
                               );


private:
   void  ValidateClassIDX (KKStr fn, int  classIDX);


   uint*              counts;
   uint**             predicted;
   
   TrainingImageList  trainingImages;

   VectorFloat**    probabilities;
//   VectorString*    imageFileNames;
//   VectorInt*       nonStreamPlaces;
//   VectorFloat*     percentiles;

   SortOrderType*   sortMethods;
   short            numOfClasses;
   uint             numOfSupportVectors;
   float            trainingTime;
   float            testingTime;
};


typedef  ClassFocus*  ClassFocusPtr;


#endif
