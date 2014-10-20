#if  !defined(_DATABASEIMAGEENTRY_)
#define  _DATABASEIMAGEENTRY_

#include  "Str.h"

//**********************************************************************************************************
//*  Created to help support Harvester.cs.  The idea is that we want to Transmit a minimal amount of data  *
//**********************************************************************************************************


namespace MLL
{

  class DataBaseImageEntry
  {
  public:
    typedef  DataBaseImageEntry*  DataBaseImageEntryPtr;

    DataBaseImageEntry (const KKStr&  _imageFileName,
                        float         _depth
                       );


    ~DataBaseImageEntry ();


    const KKStr&   ImageFileName ()  const  {return imageFileName;}
    float          Depth         ()  const  {return depth;}


  private:
    KKStr  imageFileName;
    float  depth;
  };

  typedef  DataBaseImageEntry::DataBaseImageEntryPtr  DataBaseImageEntryPtr;




  class  DataBaseImageEntryList: public  KKQueue<DataBaseImageEntry>
  {
  public:
    typedef  DataBaseImageEntryList*  DataBaseImageEntryListPtr;

    DataBaseImageEntryList (bool  _owner = true);
    ~DataBaseImageEntryList ();
  };  /* DataBaseImageList */


  typedef  DataBaseImageEntryList::DataBaseImageEntryListPtr  DataBaseImageEntryListPtr;
}  /* namespace MLL */


#endif
