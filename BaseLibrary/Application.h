/* Application.h -- Generic Application class.  
 * Copyright (C) 1994-2011 Kurt Kramer
 * For conditions of distribution and use, see copyright notice in KKU.h
 */

#ifndef  _APPLICATION_
#define  _APPLICATION_

#include  "Str.h"
#include  "RunLog.h"

/** 
 *@namespace  KKU
 *@brief KKU is a the namespace where all objects that belong to BaseLibrary are stored.
 *@details The idea is that any objects and functions that are not specific to any particular end
 *         application are placed in this namespace.  Such as Raster for image processing, KKStr for 
 *         string management, OSservices for operating specific function calls.
 *
 *         I started on this library in 1994 and have been adding to it incrementally as
 *         functionality was needed.  For the last 5 years the main driving force has
 *         been my work on the <a href="http://figment.cse.usf.edu/~kkramer/sipper">SIPPER.</a> 
 *         project.
 *@par Author 
 *  Kurt Kramer,  Ph.D. University of South Florida.<br>
 *     <a href="http://figment.cse.usf.edu/~kkramer/">Kurt Kramer.</a><br>
 *    ( mail : <a href="mailto:kurtkramer@gmail.com">kurtkramer@gmail.com</a> )<br>
*/


namespace KKU 
{
  /** 
   *@class Application
   *@brief The base class for all standalone application.
   *@details  This class is meant to be a general class that all standalone applications should be inherited 
   *          from.  It supports command line processing, and logging facilities.
   */

  class  Application
  {
  public:
    /** 
     *@brief  Copy Constructor for Application class.
     *@param[in]  _application  Application instance to copy.
     */
    Application (const Application&  _application);


    /**
     *@brief  Constructor for Application class with no parameters.
     *@details This constructor is not interested in any command line parameters.
     *@param[in]  _log  A reference to a RunLog object.
     */
    Application (RunLog&  _log);


    /** 
     *@brief  Constructor for Application class that expects Command Line Parameters.
     *@details This constructor will scan the command line parameters for the log file options  (-L, -Log, or -LogFile)  and use its
     *        parameter as the LogFile name.  If none is provided it will assume the stdout as the Log File to write to.  It will take
     *        ownership of this log file and delete it in its destructor.  Right after calling this constructor you will need to
     *        call the method ProcessCmdLineParameters.
     *@see  RunLog
     *@see  ProcessCmdLineParameters
     *@param[in]  argc  Number of arguments in argv.
     *@param[in]  argv  List of asciiz strings; one string for each argument.
     */
    Application (int32   argc,
                 char**  argv
                );

    virtual
    ~Application ();

    /**
     *@brief  Returns 'true' if application has been aborted.
     *@details  You would typically call this method after you are done processing the command line to
     *          make sure that the application is to keep on running.
     */
    bool  Abort ()  {return  abort;}

    /** 
     *@brief Used to specify that the application is been aborted.
     *@details If you have a reason to abort the processing of this application you would call this method to set the 'abort' 
     *         flag to true.
     *@param[in] _abort  Abort status to set; if set to true you are telling the application that the program needs to be terminated.
     */
    void  Abort (bool _abort)  {abort = _abort;}

    /** Specify the name of the application */
    virtual 
    const char*  ApplicationName ();

    void         AssignLog (RunLog&  _log);  /**< @brief Replaces the Log file to write to.  */

    KKStr        BuildDate ()  const;


    /**
     *@brief Processes all the command line parameters; will also expand the -CmdFile option.
     *@details This method assumes that the command line consists of pairs of Switches and Operands.  Switches are proceeded by the
     *         dash character("-").  For each pair it will call the 'ProcessCmdLineParameter' method which should be implemented by
     *         the derived class.  Before calling 'ProcessCmdLineParameter' though it will scan the parameters for the "-CmdFile" 
     *         switch.  This switch specifies a text file that is to be read for additional command parameters.
     *@param[in] argc   Number of parameters.
     *@param[in] argv   The actual parameters.
     */
    void         ProcessCmdLineParameters (int32   argc,
                                           char**  argv
                                          );



    /**
     *@brief This method will get called once for each parameter specified in the command line.
     *@details  Derived classes should define this method to intercept parameters that they are interested in.
     *          Parameters are treated as pairs, Switched and Values where switches have a leading dash("-").
     *          The CmdLineExpander class will be used to expand any "-CmdFile" parameters.
     *@param[in] parmSwitchCode  The first character of the switch parameter.
     *@param[in] parmSwitch      The fill switch parameter.
     *@param[in] parmValue       Any value parameter that followed the switch parameter.
     */
    virtual 
    bool         ProcessCmdLineParameter (char    parmSwitchCode, 
                                          KKStr   parmSwitch, 
                                          KKStr   parmValue
                                         );


  private: 
    void        BuildCmdLineParameters (const VectorKKStr&  argv,
                                        VectorKKStr&        expamdedParameters,
                                        VectorKKStr&        cmdFileStack,
                                        bool&               parmsGood
                                       );

  
    void        ExtractParametersFromFile (const KKStr&  cmdFileName, 
                                           VectorKKStr&  cmdFileParameters,
                                           bool&          validFile
                                          );


    bool        ParameterIsASwitch (const KKStr&  parm);

    void        ProcessCmdLineParmeters (int32   argc,
                                         char**  argv
                                        );

    bool        abort;

  public:
    RunLog&     log;  

  private:
    KKStr       logFileName;
    RunLogPtr   ourLog;  // We use this Log file if one is not provided,
  };  /* Application */
}  /* NameSpace KKU */

#endif
