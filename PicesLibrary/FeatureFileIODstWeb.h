#ifndef  _FEATUREFILEIODSTWEB_
#define  _FEATUREFILEIODSTWEB_
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
    ************************************************************************************************
    *  FeatureFileIODstWeb    Sub-classed from FeatureFileIO.  It supports thee readinf of the     *
    *  "dst" file format used by for the Anonymous web data from www.microsoft.com.                *
    *  for detailed desciption go to http://kdd.ics.uci.edu/databases/msweb/msweb.data.html        *
    *  In addition to the layout described above you will need to add a line at the top of the     *
    *  file of the format "ClassName = xxxx"  whre xxxx = the name of the attribute that will be   *
    *  used as the Class-Name.                                                                     *
    ************************************************************************************************
    * @see  FeatureFileIO
    */
  class FeatureFileIODstWeb: public FeatureFileIO
  {
  public:
    FeatureFileIODstWeb ();
    ~FeatureFileIODstWeb ();

    typedef  FeatureFileIODstWeb*  FeatureFileIODstWebPtr;

    static   FeatureFileIODstWebPtr  Driver ()  {return &driver;}

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
    static  FeatureFileIODstWeb  driver;

    class  AttrDescLine;
    typedef  AttrDescLine*  AttrDescLinePtr;
    class  AttrDescLineComparator;

  };  /* FeatureFileIODstWeb */


}  /* namespace MLL */


#endif



