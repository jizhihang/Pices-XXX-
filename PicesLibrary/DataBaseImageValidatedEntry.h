#if  !defined(_DATABASEIMAGEVALIDATEDENTRY_)
#define  _DATABASEIMAGEVALIDATEDENTRY_

#include "Point.h"
#include "Str.h"

//*********************************************************************************************************
//*  Keeps track of Validated Images;  used by ImageExtraction to save ValidationInfo for a specific      *
//*  Sipper file.  Image Extraction will:                                                                 *
//*   1) Load existing list of validated class info from text file.                                       *
//*   2) Retrieve from Images table current Validated calsses and append to end of list.                  *
//*   3) Erase existing Images for Sipper File.                                                           *
//*   4) After extraction foir the sipper file is completed it will update the Validation Info from the   *
//*      previously saved file.                                                                           *
//*********************************************************************************************************


namespace MLL
{
#if  !defined(_MLCLASS_)
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
#endif

  class DataBaseImageValidatedEntry
  {
  public:
    typedef  DataBaseImageValidatedEntry*  DataBaseImageValidatedEntryPtr;

    DataBaseImageValidatedEntry (const KKStr&        _imageFileName,
                                 PointListPtr        _sizeCoordinates, /**< Will take ownership of point list */
                                 MLClassConstPtr  _validatedClass
                                );


    DataBaseImageValidatedEntry (const KKStr&        _imageFileName,
                                 const KKStr&        _sizeCoordinatesDelStr, /**< Will take ownership of point list */
                                 MLClassConstPtr  _validatedClass
                                );


    ~DataBaseImageValidatedEntry ();


    const KKStr&        ImageFileName         () const  {return imageFileName;}
    const PointListPtr  SizeCoordinates       () const  {return sizeCoordinates;}
    KKStr               SizeCoordinatesDelStr () const;
    MLClassConstPtr  ValidatedClass        () const  {return validatedClass;}
    const KKStr&        ValidatedClassName    () const;

    float  ImageSize (float  heightFactor,
                      float  widthFactor
                     ) const;


  private:
    KKStr               imageFileName;
    PointListPtr        sizeCoordinates;
    MLClassConstPtr  validatedClass;
  };

  typedef  DataBaseImageValidatedEntry::DataBaseImageValidatedEntryPtr  DataBaseImageValidatedEntryPtr;




  class  DataBaseImageValidatedEntryList: public KKQueue<DataBaseImageValidatedEntry>
  {
  public:
    typedef  DataBaseImageValidatedEntryList*  DataBaseImageValidatedEntryListPtr;

    DataBaseImageValidatedEntryList (bool  _owner = true);

    DataBaseImageValidatedEntryList (const KKStr&  _fileName,
                                     RunLog&       log
                                    );
    
    ~DataBaseImageValidatedEntryList ();


    void  Load (const KKStr&  _fileName,
                RunLog&       log
               );

    void  Save (const KKStr&  _fileName,
                RunLog&       log
               );

  private:
    KKU::DateTime  dateWritten;
    KKStr          fileName;
    KKStr          hostName;
    PointListPtr   sizeCoordinates;

    /*
    class  Order
    {
      bool  operator() (DataBaseImageValidatedEntryPtr p1,  DataBaseImageValidatedEntryPtr p2);
    };

    typedef  map<DataBaseImageValidatedEntryPtr,DataBaseImageValidatedEntryPtr,Order>  IndexType;
    typedef  pair<DataBaseImageValidatedEntryPtr,DataBaseImageValidatedEntryPtr>       IndexPair;

    IndexType  index;
    IndexType::iterator  indexIdx;
    */
  };  /* DataBaseImageValidatedEntryList */


  typedef  DataBaseImageValidatedEntryList::DataBaseImageValidatedEntryListPtr  DataBaseImageValidatedEntryListPtr;
}  /* namespace MLL */


#endif
