#ifndef  _FEATUREFILEIOPICES_
#define  _FEATUREFILEIOPICES_

#include "FeatureFileIO.h"
#include "MLClassConstList.h"
#include "ImageFeatures.h"


namespace MLL 
{
  /**
    @class  FeatureFileIOPices
    @brief  Supports the reading and writting of feature data from and to Pices feature data files.
    @details
    @code
    * *************************************************************************************************
    * *  The PICES feature data file is a very simple format whre the first few rows contain a        *
    * *  description of the rest of the data file.  Any row that starts with "//" will be treated as  *
    * *  comment line.                                                                                *
    * *  First Row:  Identifies the file as a feature Data File.  An example on th enbext line.       *
    * *              "FEATURE_DATA_FILE Version  51  NumOfFeatures  88  NumOfImages  258"                *
    * *              Each field will be seperated by a tab character.  This line says that the        *
    * *              version number is 51. There are 88 features and 258 examples or feature vectors. *
    * *              The version number gets incremented when ever there is a change in the way       *
    * *              tha feature data is computed.                                                    *
    * *                                                                                               *
    * *  Second Row: List all the fields in the feature data file.  The first fields are featuire     *
    * *              data.  The first non feature field will be: "ClassName".  There are several no   *
    * *              feature data fields.  See the SipperSoftware documentation for details.          *
    * ************************************************************************************************
    @endcode
    @see  FeatureFileIO
    */
  class  FeatureFileIOPices:  public  FeatureFileIO
  {
  public:
    typedef  FeatureFileIOPices*  FeatureFileIOPicesPtr;

    typedef  KKU::uint32   uint32;

    FeatureFileIOPices ();
    virtual ~FeatureFileIOPices ();

    virtual  FileDescPtr  GetFileDesc (const KKStr&            _fileName,
                                       std::istream&           _in,
                                       MLClassConstListPtr  _classes,
                                       int32&                  _estSize,
                                       KKStr&                  _errorMessage,
                                       RunLog&                 _runLog
                                      );


    virtual  ImageFeaturesListPtr  LoadFile (const KKStr&          _fileName,
                                             const FileDescPtr     _fileDesc,
                                             MLClassConstList&  _classes, 
                                             std::istream&         _in,
                                             long                  _maxCount,    // Maximum # images to load.
                                             volatile const bool&  _cancelFlag,
                                             bool&                 _changesMade,
                                             KKStr&                _errorMessage,
                                             RunLog&               _log
                                            );


    virtual  void   SaveFile (FeatureVectorList&      _data,
                              const KKStr&            _fileName,
                              const FeatureNumList&   _selFeatures,
                              std::ostream&           _out,
                              uint32&                 _numExamplesWritten,
                              volatile const bool&    _cancelFlag,
                              bool&                   _successful,
                              KKStr&                  _errorMessage,
                              RunLog&                 _log
                             );


    static  FeatureFileIOPicesPtr  Driver                 ()                  {return &driver;}
    static  FileDescPtr            NewPlanktonFile        (RunLog&  _log);
    static  KKStr                  PlanktonFieldName      (int32  fieldNum);
    static  int32                  PlanktonMaxNumOfFields ();


    /**                       FeatureDataReSink
     * @brief Syncronuzes the contents of a feature data file with a directory of images.
     *        Used with plankton applications to verify that feature file is up-to-date.
     *        Was specifically meant to work with training libraries, to account for
     *        images being added and deleted from training library.  If there are no 
     *        changes, then function will run very quickly.
     * @param[in] dirName,      Directory where source images are loccated.
     * @param[in] fileName,     Feature file that is being syncronized.
     * @param[in] unknownClass, Class to be used when class is unknown
     * @param[in] useDirectoryNameForClassName, if true then class name of each entry
     *            will be set to directory name.
     * @param[in] mlClasses,  list of classes
     * @param[in] log, where to send diagnostic messages to.
     * @param[out] changesMade, If returns as true then there were changes made to the 
     *             feature file 'fileName'.  If set to false, then no changes were made.
     * @param[out] Timestamp of feature file.
     * @return - A ImageFeaturesList container object.  This object will own all the examples loaded
     *
     * A change in featuer file version number would also cause all entries in the feature 
     * file to be recomputed.  The feature file version number gets incremented whenever we change
     * the feature file computation routine.
     */
    static
    ImageFeaturesListPtr  FeatureDataReSink (KKStr                 _dirName, 
                                             const KKStr&          _fileName, 
                                             MLClassConstPtr    _unknownClass,
                                             bool                  _useDirectoryNameForClassName,
                                             MLClassConstList&  _mlClasses,
                                             volatile const bool&  _cancelFlag,    // will be monitored,  if set to True  Load will terminate.
                                             bool&                 _changesMade,
                                             KKU::DateTime&        _timeStamp,
                                             RunLog&               _log
                                            );






    /**                       LoadInSubDirectoryTree
     * @brief Loads in the contents of a sub-directory tree.
     *        Meant to work with SIPPER plankton images, it starts at a specified subdirectory and 
     *        transverses all subdirectories.  It makes use of FeatureDataReSink for each specific
     *        sub-directory.
     * @param[in] rootDir,   Strating directory.
     * @param[in,out] mlClasses, List of classes, any new classes in fileName will be added.
     * @param[in] useDirectoryNameForClassName, if true set class names to sub-directory name.
     *            This happens because the user may manually move images between dorectories using
     *            the sub-directory name as the class name.
     * @param[in] log, where to send diagnostic messages to.
     * @param[in] rewiteRootFeatureFile, If true rewite the feature file in the specified 'rootDir'.  This
     *            feature file will contain all entries from all sub-directories below it.
     * @return - A ImageFeaturesList container object.  This object will own all the examples loaded.
     */
    static
      ImageFeaturesListPtr  LoadInSubDirectoryTree (KKStr                 _rootDir,
                                                    MLClassConstList&  _mlClasses,
                                                    bool                  _useDirectoryNameForClassName,
                                                    volatile const bool&  _cancelFlag,    // will be monitored,  if set to True  Load will terminate.
                                                    bool                  _rewiteRootFeatureFile,
                                                    RunLog&               _log
                                                   );






  private:
    static FeatureFileIOPices  driver;
    static FileDescPtr         planktonFileDesc;
    static int32               MaxNumPlanktonRawFields;
    static const char*         PlanktonRawFeatureDecriptions[];


    typedef  enum  {rfFeatureData,
                    rfClassName,
                    rfMLClassIDX,
                    rfImageFileName, 
                    rfOrigSize,
                    rfNumOfEdgePixels,
                    rfProbability,
                    rfCentroidRow,
                    rfCentroidCol,
                    rfPredictedClass,
                    rfPredictedClassIDX,
                    rfBreakTie,
                    rfSfCentroidRow,
                    rfSfCentroidCol,
                    rfLaditude,
                    rfLongitude,
                    rfDepth,
                    rfUnKnown
                   }  
                   PicesFieldTypes;



    void  Parse_FEATURE_DATA_FILE_Line (KKStr&  line,
                                        int32&  version,
                                        int32&  numOfFields,
                                        int32&  numOfExamples
                                       );


    VectorInt  CreateIndirectionTable (const VectorKKStr&  fields,
                                       int32                 numOfFeatures
                                      );

  };

  typedef  FeatureFileIOPices::FeatureFileIOPicesPtr  FeatureFileIOPicesPtr;

}  /* namespace MLL */


#endif
