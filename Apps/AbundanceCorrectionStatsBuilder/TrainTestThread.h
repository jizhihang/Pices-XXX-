#if  !defined(_TRAINTESTTHREAD_)
#define  _TRAINTESTTHREAD_

#include "MsgQueue.h"

#include "AbundanceCorrectionMatrix.h"
#include "ConfusionMatrix2.h"

namespace  AbundanceCorrectionApplication
{
  #if  !defined(_ABUNDANCECORRECTIONSTATSBUILDER_)
  class  AbundanceCorrectionStatsBuilder;
  typedef  AbundanceCorrectionStatsBuilder*  AbundanceCorrectionStatsBuilderPtr;
  #endif


  typedef  enum  {tsNotStarted,
                  tsStarting,
                  tsRunning,
                  tsStopping,
                  tsStopped
                 } 
                   ThreadStatus;

  /**
   *@brief   Class that will manage a single thrad that will train and test one fold.
   */
  class  TrainTestThread
  {
  public:
    TrainTestThread (const KKStr&                        _desc,
                     AbundanceCorrectionStatsBuilderPtr  _parent,
                     TrainingConfiguration2Ptr           _config,           /**< Will NOT  take ownership.       */
                     MLClassConstListPtr              _allClasses,
                     FeatureVectorListPtr                _trainData,        /**< Will take ownship of this list. */
                     MLClassConstListPtr              _trainDataClasses,
                     FeatureVectorListPtr                _testData,         /**< Will take ownship of this list. */
                     MLClassConstPtr                  _otherClass,
                     const KKStr&                        _threadName,
                     MsgQueuePtr                         _msgQueue,
                     RunLogPtr                           _runLog
                    );

    virtual ~TrainTestThread ();

    // Thread related accesss methods.
    volatile const bool&   CancelFlag      () const  {return cancelFlag;}
    bool                   Crashed         () const  {return crashed;}
    ThreadStatus           Status          () const  {return status;}
    volatile const bool&   TerminateFlag   () const  {return terminateFlag;}
    const KKStr&           ThreadName      () const  {return threadName;}

    void    Crashed (bool         _crashed)  {crashed = _crashed;}
    void    Status  (ThreadStatus _status)   {status  = _status;}


    const AbundanceCorrectionMatrixPtr  AbundanceCorMatrix () const {return abundanceCorMatrix;}
    const ConfusionMatrix2Ptr           ConfusionMatrix    () const {return confusionMatrix;}
    const KKStr&                        Desc               () const {return desc;}


    AbundanceCorrectionMatrixPtr  AbundanceCorMatrixTakeOwnership ();

    ConfusionMatrix2Ptr           ConfusionMatrixTakeOwnership ();


    void  Run ();
    
    void  CancelThread ();         /**< Call this method to have its thread stop right away and exit. */

    void  TerminateThread ();      /**< Call this method to have its thread finish what ever is in the queue and then exit. */

    void  Start ();                /**<  Call this method to start thread; it will call the method "Run". */
    void  Stop ();

  protected:
    void  AddMsg (KKStrPtr  msg);      /**<  Taking ownership of 'msg' and will append to 'msgQueue'.           */
    void  AddMsg (const KKStr&  msg);  /**<  A copy of the message 'msg' will be added to the end of msgQueue.  */

    bool  CancelOrTerminateRequested ()  {return  (cancelFlag || terminateFlag);}


  private:
    AbundanceCorrectionMatrixPtr         abundanceCorMatrix;

    const
    MLClassConstListPtr               allClasses;     /**<  Will be owned by parent.  */

    volatile bool                        cancelFlag;     /**< Threads need to monotir this flag;  if it goes 'true'  they need 
                                                          * to terminate as quick as possible.
                                                          */

    TrainingConfiguration2Ptr            config;

    ConfusionMatrix2Ptr                  confusionMatrix;

    bool                                 crashed;        /**< Signifies that this thread had to terminate on its own because 
                                                          * of an abnormal situation. 
                                                          */

    KKStr                                desc;

    MsgQueuePtr                          msgQueue;       /**< This MsgQueue instance will be owned by 'ExtractionManager' we will just use
                                                          * it to communicate messeges to the controlling process.
                                                          */
    MLClassConstPtr                   otherClass;

    AbundanceCorrectionStatsBuilderPtr   parent;

    RunLogPtr                            runLog;

    ThreadStatus                         status;

    volatile bool                        terminateFlag;  /**< Threads need to monitor this flag; if it goes thre they are to 
                                                          * complete all processing that is queued up and then terminate.
                                                          */
    FeatureVectorListPtr                 testData;

    KKStr                                threadName;

    FeatureVectorListPtr                 trainData;

    const
    MLClassConstListPtr               trainDataClasses;  /**<  Will be owned by parent.  */


    //  Data structures and methods specific to Windows thread management.
    unsigned long*                       windowsThread;
    DWORD                                windowsThreadId;	

    void   SetPriority (int p);


  };  /* TrainTestThread */


  typedef  TrainTestThread*          TrainTestThreadPtr;
  typedef  KKQueue<TrainTestThread>  TrainTestThreadList;
  typedef  TrainTestThreadList*      TrainTestThreadListPtr;


  // global function called by the thread object.
  // this in turn calls the overridden run()
  extern "C" 
  {
	  unsigned int ThreadStartCallBack(void* param);
  }


}  /* AbundanceCorrectionApplication*/

#endif
