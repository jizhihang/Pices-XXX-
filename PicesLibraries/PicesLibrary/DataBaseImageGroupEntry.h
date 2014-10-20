#if  !defined(_DATABASEIMAGEGROUPENTRY_)
#define  _DATABASEIMAGEGROUPENTRY_


#include  "Str.h"
#include  "KKQueue.h"



namespace MLL 
{


  #if  !defined(_DATABASEIMAGE_)
    class  DataBaseImage;
    typedef  DataBaseImage*  DataBaseImagePtr;
    class  DataBaseImageList;
    typedef  DataBaseImageList*  DataBaseImageListPtr;
  #endif



  class DataBaseImageGroupEntry
  {
  public:
    typedef  DataBaseImageGroupEntry*  DataBaseImageGroupEntryPtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    DataBaseImageGroupEntry (const DataBaseImageGroupEntry&  imageGroupEntry);

    DataBaseImageGroupEntry (int32           _imageGroupId,
                             const KKStr&  _imageFileName
                            );

    ~DataBaseImageGroupEntry ();


    int32           ImageGroupId  ()  const  {return  imageGroupId;}
    const KKStr&  ImageFileName ()  const  {return  imageFileName;}

    void    ImageGroupId  (int32           _imageGroupId)   {imageGroupId  = _imageGroupId;}
    void    ImageFileName (const KKStr&  _imageFileName)  {imageFileName = _imageFileName;}

  private:
    int32    imageGroupId;
    KKStr  imageFileName;
  };  /* DataBaseImageGroupEntry */

  typedef  DataBaseImageGroupEntry::DataBaseImageGroupEntryPtr  DataBaseImageGroupEntryPtr;



  class  DataBaseImageGroupEntryList:  public  KKQueue<DataBaseImageGroupEntry>
  {
  public:
    typedef  DataBaseImageGroupEntryList*  DataBaseImageGroupEntryListPtr;

    DataBaseImageGroupEntryList (bool  _owner);

    DataBaseImageGroupEntryList (const DataBaseImageGroupEntryList&  imageGroupEntries);

    DataBaseImageGroupEntryList (int32                         groupId,
                                 const DataBaseImageListPtr  images
                                );

    ~DataBaseImageGroupEntryList ();
  };

  typedef  DataBaseImageGroupEntryList::DataBaseImageGroupEntryListPtr  DataBaseImageGroupEntryListPtr;

}  /* namespace MLL { */


#endif
