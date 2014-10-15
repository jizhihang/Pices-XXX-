#ifndef  _FEATUREFILEIOUCI_
#define  _FEATUREFILEIOUCI
/**
  ************************************************************************************************
  *  @author  Kurt Krame                                                                         *
  *  Date:  2009-04-16                                                                           *
  ************************************************************************************************
  */

#include  "FeatureFileIO.h"

namespace MLL
{
  /**
    @class FeatureFileIOUCI
    @brief  Supports the reading and writting of Feature data from a file format commonly used by many datasets in the UCI repository.
    @details
    @code
    * ************************************************************************************************
    * *  FeatureFileIOUCI  Sub-classed from FeatureFileIO.  It supports the reading and writting     *
    * *  of many of the files found in the UCI repository.                                           *
    * *                                                                                              *
    * *  1) One example per row.                                                                     *
    * *  2) Each field is seperated by comma.                                                        *
    * *  3) Each row has the same number of fields.                                                  *
    * *  4) The last field is the ClassName.                                                         *
    * *  5) Each field is numeric except the last one which is the ClassName                         *
    * ************************************************************************************************
    @endcode
    @see  FeatureFileIO
    */
  class  FeatureFileIOUCI: public FeatureFileIO
  {
  public:
    FeatureFileIOUCI ();

    ~FeatureFileIOUCI ();

    typedef  FeatureFileIOUCI*  FeatureFileIOUCIPtr;

    static  FeatureFileIOUCIPtr  Driver ()  {return &driver;}

    virtual  FileDescPtr  GetFileDesc (const KKStr&            _fileName,
                                       istream&                _in,
                                       MLClassConstListPtr  _classList,
                                       int32&                  _estSize,
                                       KKStr&                  _errorMessage,
                                       RunLog&                 _log
                                      );


    virtual  FeatureVectorListPtr  LoadFile (const KKStr&          _fileName,
                                             const FileDescPtr     _fileDesc,
                                             MLClassConstList&  _classes, 
                                             istream&              _in,
                                             long                  _maxCount,    // Maximum # images to load.
                                             volatile const bool&  _cancelFlag,
                                             bool&                 _changesMade,
                                             KKStr&                _errorMessage,
                                             RunLog&               _log
                                            );


    virtual  void   SaveFile (FeatureVectorList&     _data,
                              const KKStr&           _fileName,
                              const FeatureNumList&  _selFeatures,
                              ostream&               _out,
                              uint32&                _numExamplesWritten,
                              volatile const bool&   _cancelFlag,
                              bool&                  _successful,
                              KKStr&                 _errorMessage,
                              RunLog&                _log
                             );



  private:
    static  FeatureFileIOUCI  driver;

  };

}  /* namespace MLL */

#endif
