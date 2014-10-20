#ifndef  _FEATUREFILEIO_
#define  _FEATUREFILEIO_


/**
 *@class  MLL::FeatureFileIO
 *@brief  Base class for all FeatureFileIO classes.
 *@details  This is a abstract class.  For each type of FeatureFile you will need to implement
 *          a separate class derivd from this class that supports the specific file format.
 *          You only bneed to implementthe puir virtual functions.
 *
 *          If you create a new FeatureFileIO class you will need to modify 'RegisterAllDrivers'
 *          methiod in FeatureFileIO.cpp.
 */

#include  "FeatureNumList.h"
#include  "FeatureVector.h"
#include  "GoalKeeper.h"
#include  "MLClass.h"
#include  "MLClassConstList.h"
#include  "OSservices.h"
#include  "RunLog.h"
#include  "Str.h"

namespace MLL 
{

  class  FeatureFileIO;
  typedef  FeatureFileIO*  FeatureFileIOPtr;



  #if  !defined(_FeatureVectorDefined_)
  class  FeatureVector;
  typedef  FeatureVector*  FeatureVectorPtr;
  #endif


  #if  !defined(_FeatureVectorListDefined_)
  class  FeatureVectorList;
  typedef  FeatureVectorList*  FeatureVectorListPtr;
  #endif


  class  FeatureFileIO
  {
  public:
    typedef  FeatureFileIO*  FeatureFileIOPtr;

    typedef  KKU::int32    int32;
    typedef  KKU::uint32   uint32;

    FeatureFileIO (const KKStr&  _driverName,
                   bool          _canRead,
                   bool          _canWrite
                  );

    virtual ~FeatureFileIO ();

    bool  CanRead  ()  {return  canRead;}
    bool  CanWrite ()  {return  canWrite;}


    void   AppendToFile (const KKStr&           _fileName,
                         const FeatureNumList&  _selFeatures,
                         FeatureVectorList&     _examples,
                         uint32&                _numExamplesWritten,
                         volatile const bool&   _cancelFlag,
                         bool&                  _successful,
                         RunLog&                log
                        );


    /**
     *@brief Loads the contents of a feature data file and returns a ImageFeaturesList container object.
     *@param[in]  _fileName   Feature file that is being syncronized.
     *@param[in,out] _mlClasses All classes encounterd during the loading of the feature file will be added to this list.
     *@param[in]  _maxCount Maximum number of examples to load, -1 = load all
     *@param[in]  _cancelFlag If this flag turns true the load will terminate and return to caller.
     *@param[out] _successful False will be returned if load failed.
     *@param[out] _changesMade If the routine hatloaded the feature data determined that it neeeded to make
     *            changes this flag will be set 'true'.
     *@param[in]  _log Where to send diagnostic messages to.
     *@return  A ImageFeaturesList container object; this object will own all the examples loaded.
     */
    virtual  
      FeatureVectorListPtr  LoadFeatureFile (const KKStr&          _fileName,
                                             MLClassConstList&  _mlClasses,
                                             long                  _maxCount,
                                             volatile const bool&  _cancelFlag,    // will be monitored,  if set to True  Load will terminate.
                                             bool&                 _successful,
                                             bool&                 _changesMade,
                                             RunLog&               _log
                                            );

      
     /**                       SaveFeatureFile
     *@brief Save examples to 'fileName'
     *@param[in]  _fileName  Name of file top same examples/images to.
     *@param[in]  _selFeatures Specify specific features to save, typically all features.
     *@param[in]  _examples Examples that are to be saved.
     *@param[in]  _numExamplesWritten Will reflect the num examples written, caller will be able to monitor.
     *@param[in]  _cancelFlag If this flag turns true the writting of data will terminate and return to caller.
     *@param[out] _successful False will be returned if the save failed.
     *@param[in]  _log log file to send messages to.
     */
    virtual
      void  SaveFeatureFile (const KKStr&           _fileName, 
                             const FeatureNumList&  _selFeatures,
                             FeatureVectorList&     _examples,
                             uint32&                _numExamplesWritten,  // caller will be able to manitor this variable.
                             volatile const bool&   _cancelFlag,
                             bool&                  _successful,
                             RunLog&                _log
                            );


    /**
     *@brief Saves the feature file in multiple parts with no one single part larger that 64k examples.
     *@details Same as 'SaveFeatureFile', if more than 64k examples will break into multiple files.
     *         If there are more than 64k examples, will save all images into 'fileName', but also
     *         a second copy of them into files with same name plust seq num with max of 64k eamples
     *         in each one.
     *
     *@param[in]  _fileName  Name of file top same examples/images to.
     *@param[in]  _selFeatures Specify specific features to save, typically all features.
     *@param[in]  _examples Examples that are to be saved.
     *@param[in]  _cancelFlag If this flag turns true the writting of data will terminate and return to caller.
     *@param[out] _successful False will be returned if the save failed.
     *@param[in]  _log log file to send messages to.
     */
    void  SaveFeatureFileMultipleParts (const KKStr&           _fileName, 
                                        const FeatureNumList&  _selFeatures,
                                        FeatureVectorList&     _examples,
                                        volatile const bool&   _cancelFlag,
                                        bool&                  _successful,
                                        RunLog&                _log
                                       );





    //***************************************************************************
    //*    The following routines need to be implemented by derived classes.   *
    //***************************************************************************

    /**
     *@brief  Create a FileDesc object from the input stream '_in'.
     *@details  All derived classes must implement this method.  It is called by 'LoadFeatureFile'
     *          before it starts reading in the feature data.  
     *@param[in]  _fileName  Name of file to read top get FileDesc data from.  Ex  in c45 this wiould be the names file.
     *@param[in]  _in        Input Stream t read from.
     *@param[out] _classes   Must be pointing to a valid MLClassList object.  As class names are encountered add them to this list.
     *@param[out] _estSize   If you cabn drive the number of examples in the feature file populate this parameter.
     *@param[out] _errorMessage  If a error in processing occurs; place a description of th eerror in this parameter.
     *@param      _log
     */
    virtual  FileDescPtr  GetFileDesc (const KKStr&            _fileName,
                                       std::istream&           _in,
                                       MLClassConstListPtr  _classes,
                                       int32&                  _estSize,
                                       KKStr&                  _errorMessage,
                                       RunLog&                 _log
                                      ) = 0;


    /**
     *@brief To be implemented by derived classes; loads the contents of a feature data file and returns a ImageFeaturesList container object.
     *@param[in]  _fileName   Feature file that is being loaded.
     *@param[in]  _fileDesc  Description of featyuire data that is to be loaded.
     *@param[in,out] _classes All classes encounterd during the loading of the feature file will be added to this list.
     *@param[in]  _in  input stream that feature data is to be loaded/read from.
     *@param[in]  _maxCount Maximum number of examples to load, -1 = load all
     *@param[in]  _cancelFlag If this flag turns true the load will terminate and return to caller.
     *@param[out] _changesMade If the routine hatloaded the feature data determined that it neeeded to make
     *            changes this flag will be set 'true'.
     *@param[out] _errorMessage If an error occurs during the loading a description iof this error will be placed here.
     *@param[in]  _log Where to send diagnostic messages to.
     *@return  A ImageFeaturesList container object; this object will own all the examples loaded;  if an error occurs NULL will be returned.
     */
    virtual  FeatureVectorListPtr  LoadFile (const KKStr&          _fileName,
                                             const FileDescPtr     _fileDesc,
                                             MLClassConstList&  _classes, 
                                             std::istream&         _in,
                                             long                  _maxCount,    /**< Maximum # images to load. */
                                             volatile const bool&  _cancelFlag,
                                             bool&                 _changesMade,
                                             KKStr&                _errorMessage,
                                             RunLog&               _log
                                            ) = 0;


    /**
     *@brief  To be implmenetd by derived classes; save examples to output stream '_out'.
     *@param[in]  _data  Examples that are to be written to saved to the output stream.
     *@param[in]  _fileName  Name of file top same examples/images to.
     *@param[in]  _selFeatures Specify specific features to save, typically all features.
     *@param[out] _out  Output stream to save feature data to.
     *@param[out] _numExamplesWritten Will reflect the num examples written, caller will be able to monitor.
     *@param[in]  _cancelFlag If this flag turns true the writting of data will terminate and return to caller.
     *@param[out] _successful False will be returned if the save failed.
     *@param[out] _errorMessage If the save fails (_successful == false)  then a description of the error will be placed here.
     *@param[in]  _log log file to send messages to.
     */
    virtual  void   SaveFile (FeatureVectorList&      _data,
                              const KKStr&            _fileName,
                              const FeatureNumList&   _selFeatures,
                              std::ostream&           _out,
                              uint32&                 _numExamplesWritten,
                              volatile const bool&    _cancelFlag,
                              bool&                   _successful,
                              KKStr&                  _errorMessage,
                              RunLog&                 _log
                             ) = 0;



    const  KKStr&   DriverName ()  {return  driverName;}



    static  FeatureFileIOPtr   FileFormatFromStr   (const KKStr&  _fileFormatStr);

    static  FeatureFileIOPtr   FileFormatFromStr   (const KKStr&  _fileFormatStr,
                                                    bool          _canRead,
                                                    bool          _canWrite
                                                   );

    static  KKStr              FileFormatsReadOptionsStr ();

    static  KKStr              FileFormatsWrittenOptionsStr ();

    static  KKStr              FileFormatsReadAndWriteOptionsStr ();

    static  VectorKKStr        RegisteredDriverNames (bool  canRead,
                                                      bool  canWrite
                                                     );

    static  void               FinalCleanUp ();
      

  protected:
    /**
     *@brief Will retrieve the next token from the input stream. 
     *@details Leading and trailing blank charaters will be skipped. A token will be seperated 
     *         by any charater in '_delimiters' or 'EndOfLine', or 'EndOfFile'.  If a 'EndOfLine' 
     *         or 'EndOfFile' occur while reading in a token the respective flags  '_eol'  and 
     *         '_eof'  will be set to false but the following call to this function will set the 
     *         resepective flag to true and return a empty token.
     *
     *@param[in]  _in          Stream to read from,
     *@param[in]  _delimiters  List of valid delimiter characters.
     *@param[out] _token       token extracted from '_in'.  If either '_eof' or '_eol'
     *                         are set to true; then token will be empty.
     *@param[out] _eof         Set true if at end of file;
     *@param[out] _eol         Set true if at end of line.  
     */
     void  GetToken (std::istream&  _in,
                     const char*    _delimiters,
                     KKStr&         _token,
                     bool&          _eof, 
                     bool&          _eol
                    );


     void  GetLine (std::istream&  _in,
                    KKStr&         _line,
                    bool&          _eof
                   );


  private:
    bool    canRead;
    bool    canWrite;
    KKStr   driverName;
    KKStr   driverNameLower;

    void  RegisterDriver (FeatureFileIOPtr  driver);
    static void  RegisterAllDrivers ();
    static GoalKeeperPtr  featureFileIOGoalKeeper;



  static
    std::vector<FeatureFileIOPtr>*  registeredDrivers;

  static  std::vector<FeatureFileIOPtr>*  RegisteredDrivers  ();

  static
    FeatureFileIOPtr  LookUpDriver (const KKStr&  _driverName);
  };  /* FeatureFileIO */



  typedef  FeatureFileIO::FeatureFileIOPtr   FeatureFileIOPtr;

  #define  _FEATUREFILEIODEFINED_


}  /* namespace MLL  */



#endif



