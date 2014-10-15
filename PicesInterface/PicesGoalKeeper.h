#ifndef  _PICESGOALKEEPER_
#define  _PICESGOALKEEPER_


namespace PicesInterface 
{

  /*!
   \class PicesGoalKeeper
   \brief  This class is used to managed sycronization between threads in the same application.  
   */
  public ref class PicesGoalKeeper
  {
  public:
    PicesGoalKeeper (String^  _name);

   
      /*!
       \brief  Initiates a Block so that no other thread can lock this instance of PicesGoalKeeper.
       \details  If this instance of PicesGoalKeeper is already locked (blocked = false)  then will in this function sleeping until 
                 it is no longer blocked(blocked = false).  If we are the thread that holds the block then we wil lbe allowed to
                 proceed.  For each time a thread calls this method in a row the variable 'blockerDepth' will be incrementd. The
                 block will not be released until the thread that has the lock has called the method 'EndBlock' enough times
                 to reduce 'blockerDepth' to zero.
       */
      void   StartBlock ();

      void   EndBlock ();

      property  bool  Blocked {bool  get ()  {return  blocked;}}
      property  int   NumBlockedThreads  {int  get ()  {return numBlockedThreads;}}  /*!< Indicates the number of threads waiting in 'StartBlock' */



    private:
      bool  blocked;           // 'true' = Curently Blocked.
  
      int   blockerThreadId;   // Managed ThreadId of thread that currently holds the Block  -1 indcates no Block

      int   blockerDepth;      // Indicates how many times the thread that curentky holds the black has 
                               // called  "StartBlock".  For every time the thread that holds the Block calls 
                               // "StartBlock"  it wil have to call "EndBlock"  before the block is actualy 
                               // released.
    
      String^  name;           // Name that the instance of PicesGoalKeeper will be known as.
  
  
      int      numBlockedThreads;
  
  };  /* PicesGoalKeeper */

}

#endif
