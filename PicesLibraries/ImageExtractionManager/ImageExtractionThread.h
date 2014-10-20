#if  !defined(_IEM_IMAGEEXTRACTIONTHREAD_)
#define  _IEM_IMAGEEXTRACTIONTHREAD_

#include "MsgQueue.h"

#include "PreDefinedClasses.h"
#include "ExtractionParms.h"


namespace  ImageExtractionManager
{
  typedef  enum  {tsNotStarted,
                  tsStarting,
                  tsRunning,
                  tsStopping,
                  tsStopped
                 } 
                   ThreadStatus;

  typedef  enum  {ttFrameExtractor,
                  ttFrameProcessor,
                  ttDataBAseUpdater
                 }  
                   ThreadTypes;

  /**
   *@brief   The base class to be used by all Extraction Manager threads.
   *@details  This will be the base class for each one of the different types of processing that 
   * Sipper Image processing will be done.  Soem of threads will Extract logical frames from Sipper 
   * files,  
   */
  class  ImageExtractionThread
  {
  public:
    ImageExtractionThread (ExtractionParms&      _parms,
                           ExtractionManagerPtr  _extractionManager,
                           const KKStr&          _threadName,
                           MsgQueuePtr           _msgQueue
                          );

    virtual ~ImageExtractionThread ();

    virtual  ThreadTypes   ThreadType () = 0;

    volatile const bool&     CancelFlag    ()  const  {return cancelFlag;}
    bool                     Crashed       ()  const  {return crashed;}
    ThreadStatus             Status        ()  const  {return status;}
    volatile const bool&     TerminateFlag ()  const  {return terminateFlag;}
    const KKStr&             ThreadName    ()  const  {return threadName;}

    void    Crashed (bool         _crashed)  {crashed = _crashed;}
    void    Status  (ThreadStatus _status)   {status  = _status;}

    virtual  void  Run () = 0;
    
    void  CancelThread ();         /**< Call this method to have its thread stop right away and exit. */

    void  TerminateThread ();      /**< Call this method to have its thread finish what ever is in the queue and then exit. */

    void  Start ();  /**<  Call this method to start thread; it will call the method "Run". */
    void  Stop ();

  protected:
    void  AddMsg (KKStrPtr  msg);      /**<  Taking ownership of 'msg' and will append to 'msgQueue'.           */
    void  AddMsg (const KKStr&  msg);  /**<  A copy of the message 'msg' will be added to the end of msgQueue.  */

    bool  CancelOrTerminateRequested ()  {return  (cancelFlag || terminateFlag);}

    ExtractionManagerPtr   extractionManager;  /**< Pointer to instance that manges all the threads.  */

    ExtractionParms&       parms;


  private:
    volatile bool          cancelFlag;         /**< Threads need to monotir this flag;  if it goes 'true'  they need 
                                                * to terminate as quick as possible.
                                                */

    bool                   crashed;            /**< Signifies that this thread had to terminate on its own because 
                                                * of an abnormal situation. 
                                                */


    MsgQueuePtr            msgQueue;           /**< This MsgQueue instance will be owned by 'ExtractionManager' we will just use
                                                * it to communicate messeges to the controlling process.
                                                */
    ThreadStatus           status;

    volatile bool          terminateFlag;      /**< Threads need to monitor this flag; if it goes thre they are to 
                                                * complete all processing that is queued up and then terminate.
                                                */
    KKStr                  threadName;

    //  Data structures and methods specific to Windows thread management.
    unsigned long*         windowsThread;
    DWORD                  windowsThreadId;	

    void   SetPriority (int p);


  };  /* ImageExtractionThread */


  typedef  ImageExtractionThread*          ImageExtractionThreadPtr;
  typedef  KKQueue<ImageExtractionThread>  ImageExtractionThreadList;
  typedef  ImageExtractionThreadList*      ImageExtractionThreadListPtr;


  // global function called by the thread object.
  // this in turn calls the overridden run()
  extern "C" 
  {
	  unsigned int ThreadStartCallBack(void* param);
  }


}  /* ImageExtractionManager*/

#endif
