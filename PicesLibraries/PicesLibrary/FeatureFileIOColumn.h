#ifndef  _FEATUREFILEIOCOLUMN_
#define  _FEATUREFILEIOCOLUMN_
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
    @class FeatureFileIOColumn
    @brief  Supports a simple Feature File format where each collumn represents a example and each row a feature value.
    @details
    @code
    ************************************************************************************************
    *  FeatureFileIOColumn    Sub-classed from FeatureFileIO.  A simple format where each collumn  *
    *  represents a  example and each row an attribute.  The First line has the Classs Names.      *
    ************************************************************************************************
    @endcode
    @see  FeatureFileIO
    */
  class FeatureFileIOColumn:  public FeatureFileIO
  {
  public:
    FeatureFileIOColumn ();
    ~FeatureFileIOColumn ();

    typedef  FeatureFileIOColumn*  FeatureFileIOColumnPtr;

    static   FeatureFileIOColumnPtr  Driver ()  {return &driver;}

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

    virtual  void   SaveFile (FeatureVectorList&      _data,
                              const KKStr&            _fileName,
                              const FeatureNumList&   _selFeatures,
                              ostream&                _out,
                              uint32&                 _numExamplesWritten,
                              volatile const bool&    _cancelFlag,
                              bool&                   _successful,
                              KKStr&                  _errorMessage,
                              RunLog&                 _log
                             );

  private:
    static  FeatureFileIOColumn  driver;
  };

}  /* namespace MLL */


#endif

