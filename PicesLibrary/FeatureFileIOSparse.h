#ifndef  _FEATUREFILEIOSPARSE_
#define  _FEATUREFILEIOSPARSE_
/**
  @file  FeatureFileIOSparse.cpp
  ************************************************************************************************
  *  @author  Kurt Kramer                                                                        *
  *  Date:  2009-04-16                                                                           *
  ************************************************************************************************
  */

#include  "FeatureFileIO.h"


namespace MLL
{

  /**
    @brief  Supports the reading and writting of Sparse feature files similar to the ones libSVM use.
    @details
    @code
    * ************************************************************************************************
    * *  FeatureFileIOSparse  Sub-classed from FeatureFileIO.  It supports the reading and writting  *
    * *  of Sparse Files as used by the libSVM.                                                      *
    * *                                                                                              *
    * *  One example per row.                                                                        *
    * *  any row that starts with '//' is ignored.                                                   *
    * *  On each row there will be several fields seperated by the space(' ') character. The first   *
    * *  field will be the class name followed by the attribute data.                                *
    * *                                                                                              *
    * *   - First Field is the Class Name.  This differes from libSVM which assumes that class-names *
    * *     are integers.  We support alpha numeric class names.                                     *
    * *   - Attribute Fields will will be of the following format.  <FeatureNum>:<Feature value>.    *
    * *                                                                                              *
    * *  ex:                                                                                         *
    * *     "0 0:12.12 4:87.1242 12:-65.63 15:0.872"                                                 *
    * *     "1 2:-76.92 12:65.882 26:76"                                                             *
    * *     "1 1:28.74 3:8.452 11:22.72"                                                             *
    * *                                                                                              *
    * ************************************************************************************************
    @endcode
    @see  FeatureFileIO
    */
  class FeatureFileIOSparse: public FeatureFileIO
  {
  public:
    FeatureFileIOSparse ();
    ~FeatureFileIOSparse ();

    typedef  FeatureFileIOSparse*  FeatureFileIOSparsePtr;

    static  FeatureFileIOSparsePtr  Driver ()  {return &driver;}


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
    static  FeatureFileIOSparse  driver;

  };  /* FeatureFileIOSparse */

}  /* namespace MLL */

#endif
