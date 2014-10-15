#if  !defined(_ABUNDANCECORRECTIONMATRIX_)
#define  _ABUNDANCECORRECTIONMATRIX_

#include "BasicTypes.h"
#include "DateTime.h"
#include "RunLog.h"
#include "MLClass.h"
#include "MLClassConstList.h"

namespace MLL 
{
  #if !defined(_CLASSSTATISTIC_)
  class  ClassStatistic;
  typedef  ClassStatistic*  ClassStatisticPtr;
  class  ClassStatisticList;
  typedef  ClassStatisticList*  ClassStatisticListPtr;
  #endif

  /**
   *@brief  Confusion matrix used to compute adjustment vectors used to make adjustments to classification results.
   *@details
   *@code
   *  From Paper: "Accurate automatic quantification of taxa-specific plankton abundance using dual classification with correction"
   *          by: Qiao Hu, Cabell Davis
   *@endcode
   *    
   */
  class  AbundanceCorrectionMatrix
  {
  public:
    /**
     *@brief  Constructor.
     *@details  The matrix will contain one row and one column for each entry in '_classes'  and one additional row 
     * and column for '_otherClass'.
     *@param[in] _classes    Classes that are to be part of matrix but should not include '_otherClass'; will create a 
     *                       local instance.
     *@param[in] _otherClass This is the class that predictions to any class that is not in '_otherClass' will go to.
     */
    AbundanceCorrectionMatrix (MLClassConstList&  _classes,
                               MLClassConstPtr    _otherClass,
                               RunLog&               _log
                              );
    
    AbundanceCorrectionMatrix (const KKStr&  configFileName,
                               RunLog&       _log
                              );
    
    ~AbundanceCorrectionMatrix ();


    const KKU::DateTime&  DateTimeComputed () const {return dateTimeComputed;}
    const VectorFloat&    ProbOfDetection  () const {return probOfDetection;}
    const VectorFloat&    ProbOfFalseAlarm () const {return probOfFalseAlarm;} 
    bool                  Valid            () const {return valid;}


    void  AddIn (const AbundanceCorrectionMatrix&  matrix,
                 RunLog&                           _log
                );

    /**
     *@brief Compute and return abundance adjusted counts.
     *@details Will treat all classes from 'inputCounts' that are not in the internal 'classes' or 'otherClass' as a 'otherClass'.
     *  The adjusted counts will be computed using the stats earlier computed and saved in 'probOfDetection' and 'probOfFalseAlarm'.
     *  AdjustedCount = InputCount * probOfFalseAlarm / probOfDetection;
     *@param[in] inputCounts List of counts retuned from the classifier; each entry will consist of the classification and its 
     *                       respective count.
     */
    ClassStatisticListPtr   AdjustClassificationCounts (const ClassStatisticList&  inputCounts,
                                                        RunLog&                    log
                                                       );

    void  ComputeStatistics (); /**< computing  probOfDetection  and  probOfFalseAlarm(Specifiity) */


    /**@brief Will lump all counts in 'inputCounts' into classes that match this instance 'classList' and 'otherClass' grouping. */
    ClassStatisticListPtr   LumpCounts (const ClassStatisticList&  inputCounts);

    void  Prediction (MLClassConstPtr  knownClass,
                      MLClassConstPtr  predClass,
                      RunLog&             log
                     );

    void  PrintConfusionMatrixTabDelimited (ostream&  outFile);

    void  ReadXml (istream&  i,
                   RunLog&   log
                  );

    void  WriteXml (ostream&  o);

    void  SaveForConfigFileName (const KKStr&  configFileName);



  private:
    void  AllocateMemory ();

    void  AllocatePredictionsTable ();

    template<typename T>
    void   AllocateVector (vector<T>&  v,  uint32 len);

    uint32*   AllocateUint32Array (uint32 len);

    void  CleanUp ();
    
    int32  LookUpClassIdx (MLClassConstPtr c);

    void  ReadForConfigFileName (const KKStr&  configFileName,
                                 RunLog&       log
                                );

    template<typename T>
    istream&  ReadVector (vector<T>&  v,
                          istream&    i
                         );

    template<typename T>
    KKStr  VectorToStr (vector<T>&  v);

    template<typename T>
    KKStr  ArrayToStr (T* a,  uint32 len);


    template<typename T>
    void  ReadArray (uint32    len,
                     T*&       a,
                     istream&  i
                    );


    MLClassConstList  classes;
    KKU::DateTime        dateTimeComputed;
    uint32*              knownByClass;
    uint32               numClasses;    /**< Number of classes in 'classes' plus one for 'otherClass'. */
    uint32               numCorrect;
    uint32               numPredictions;
    MLClassConstPtr   otherClass;
    int32                otherClassIdx; /**< Index into 'classes' for 'otherClass'.                    */

    /**
     * A two dimensional matrix where each column will represent known class.  The predictions of the known class
     * will be distributed across rows.  The last row and last column will represent the other class.  The first 
     * N collumns and rows will reprsent the classes listed in "classes".  The concept is that a Dual classifoer
     * will be trained on the classes specified in "classes"  but the test data will come from all classes; 
     * that is the classes listed in "classes" and "otherClass".
     */
    uint32**         predictions;
    uint32*          predByClass;

    VectorFloat      probOfDetection;
    VectorFloat      probOfFalseAlarm;

    bool             valid;
  };  /* AbundanceCorrectionMatrix */

  typedef  AbundanceCorrectionMatrix*  AbundanceCorrectionMatrixPtr;
}  /* MLL */


#endif
