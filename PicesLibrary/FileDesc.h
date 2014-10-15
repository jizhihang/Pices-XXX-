#if  !defined(_FILEDESC_)
#define  _FILEDESC_

/**
 @file  FileDesc.h
 @author  Kurt Kramer
 @details
 @code
 **************************************************************************************
 **                                                                                  *
 **  Kurt Kramer                                                                     *
 **                                                                                  *
 **  Describes the different fields of a dataset. There will be one instance of this *
 **  class for each tyope of Dataset that you have in an application.  From this     *
 **  object you can get information such as number of attributes, Attribute types,   *
 **  weather they are nomainal, continuous.  If nomainal what are the accepted       *
 **  values.                                                                         *
 **                                                                                  *
 **                                                                                  *
 **  Never delete an instance of a FileDesc object.                                  *
 **                                                                                  *
 **  Only one FileDesc object can exist for any Dataset.  Example the Forest Cover   *
 **  dataset.  You can split the data into many files and manage them seperately but *
 **  you will only have one instance of a FileDesc object tha they will all refer    *
 **  to.  See "GetExistingFileDesc" method below.  You would initially create an     *
 **  instance of FileDesc abd then use "GetExistingFileDesc" to make sure that it is *
 **  unique. This typically happens in the FeatureFileIO derived classes.            *
 **                                                                                  *
 *************************************************************************************
 @endcode
 */

#ifdef  WIN32
#include "..\\BaseLibrary\\GoalKeeper.h"
#include "..\\BaseLibrary\\RunLog.h"
#include "..\\BaseLibrary\\Str.h"
#else
#include "../BaseLibrary/GoalKeeper.h"
#include "../BaseLibrary/RunLog.h"
#include "../BaseLibrary/Str.h"
#endif

#include "Attribute.h"
#include "MLClass.h"
#include "MLClassConstList.h"




namespace MLL 
{
  class  FileDescList;
  typedef  FileDescList*  FileDescListPtr;

  #if  !defined(_FEATUREFILEIODEFINED_)
  class  FeatureFileIO;
  typedef  FeatureFileIO* FeatureFileIOPtr;
  #endif


  /**
    *@class  FileDesc
    *@brief  Provides a detailed description of the attributes of a dataset.
    *@author Kurt Kramer
    *@details
    *@code
    * ***********************************************************************************
    * * Describes the different fields of a dataset. There will be one instance of this *
    * * class for each tyope of Dataset that you have in an application.  From this     *
    * * object you can get information such as number of attributes, Attribute types,   *
    * * weather they are nomainal, continuous.  If nomainal what are the accepted       *
    * * values.                                                                         *
    * *                                                                                 *
    * *  Never delete an instance of a FileDesc object.                                 *
    * *                                                                                 *
    * * Only one FileDesc object can exist for any Dataset.  Example the Forest Cover   *
    * * dataset.  You can split the data into many files and manage them seperately but *
    * * you will only have one instance of a FileDesc object that they will all refer   *
    * * to.  See "GetExistingFileDesc" method below.  You would initially create an     *
    * * instance of FileDesc and then use "GetExistingFileDesc" to make sure that it is *
    * * unique. This typically happens in the FeatureFileIO derived classes.            *
    *************************************************************************************
    *@endcode
    *@see GetExistingFileDesc
    *@see FeatureVectorList
    *@see FeatureFileIo
    */
  class  FileDesc
  {
  public:
    typedef  FileDesc*  FileDescPtr;

    typedef  KKU::ulong   ulong;
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    /**
     @brief  Clean up function,  call just vefore exiting the application.
     @details
     @code
     * *******************************************************************************
     * * Before you terminate your application you should call this method.  It will *
     * * clean up the FileDesc objects that were created during the runtime of your  *
     * * application.                                                                *
     *********************************************************************************
     @endcode
     */
    static  void  FinalCleanUp ();

    static  bool  FinalCleanUpRanAlready ()  {return finalCleanUpRanAlready;}

    /**
     @brief  Creates a simple FileDesc that consists of continuous data only.
     @details
     @code
     * ***************************************************************************
     * * Creates a file description that will consist of continuous fields only. *
     * * The vector '_fieldNames' will provide the list of ffield names.         *
     * ***************************************************************************
     @endcode
     @param[in]  _log   Logging fille to use.
     @param[in]  _fieldNames Name of fields;  one entry for each field.
     */
    static
      FileDescPtr   NewContinuousDataOnly (RunLog&       _log,        
                                           VectorKKStr&  _fieldNames
                                          );

      FileDesc ();

  protected:
    ~FileDesc ();

  public:
    friend class KKQueue<FileDesc>;

  public:
    void                      AddAAttribute (const KKU::KKStr&   _name,
                                             MLL::AttributeType  _type,
                                             bool&               alreadyExists
                                            );
                            
    void                      AddAAttribute (const MLL::Attribute&  attribute);

    void                      AddClasses (const MLClassConstList&  classesToAdd);
                            
    bool                      AllFieldsAreNumeric ()  const;

    const
    MLL::AttributeList&       Attributes ()  const  {return  attributes;};

    const 
    AttributeTypeVector&      AttributeVector ()  const;

    int32                     Cardinality (int32    fieldNum,
                                           RunLog&  log
                                          )  const;

    const VectorInt32&        CardinalityVector  ()  const   {return cardinalityVector;}

    /** ClassNameAttribute added to support dstWeb  data files. */
    const KKStr&              ClassNameAttribute ()  const   {return classNameAttribute;}

    const 
      MLL::AttributePtr*      CreateAAttributeTable ()  const;  /**< Caller will be responsable for deleteing  */

    MLL::AttributeTypeVector  CreateAttributeTypeTable ()  const;
    
    VectorInt32               CreateCardinalityTable ()  const;

    const KKStr&              FieldName (int32  fieldNum)  const;

    const KKStr&              FileName ()   const {return  fileName;}

    const MLL::Attribute&     GetAAttribute (int32 fieldNum) const;

    const  
    KKStr&                    GetNominalValue (int32  fieldNum, 
                                               int32  code
                                              ) const;

    MLClassConstPtr           GetMLClassPtr (const KKStr&  className);

    int32                     GetFieldNumFromAttributeName (const KKStr&  attributeName)  const;

    //RunLog&                   Log () const {return log;}

    int32                     LookUpNominalCode (int32         fieldNum,
                                                 const KKStr&  nominalValue
                                                )  const;

    MLClassConstPtr           LookUpMLClassByName (const KKStr&  className);

    MLClassConstPtr           LookUpUnKnownMLClass ();
    
    int32                     MemoryConsumedEstimated ()  const;

    uint32                    NumOfFields () const  {return (uint32)attributes.size ();}

    bool                      SameExceptForSymbolicData (const FileDesc&  otherFd,
                                                         RunLog&          log
                                                        )  const;

    void                      SparseMinFeatureNum (int32  _sparseMinFeatureNum)  {sparseMinFeatureNum = _sparseMinFeatureNum;}
    int32                     SparseMinFeatureNum () const {return sparseMinFeatureNum;}

    MLL::AttributeType        Type (int32 fieldNum)  const;

    KKStr                     TypeStr (int32 fieldNum)  const;

    short                     Version ()     const  {return version;}

    void                      Version (short _version)   {version = _version;}


    /**
     @brief Returns a pointer to an existing instance of 'fileDesc' if it exists, otherwise will use one being passed in.
     @details
     @code
     * ******************************* GetExistingFileDesc ******************************
     * *  Will look for a existing FileDesc that is the same as the 'fileDesc' being    *
     * *  passed in.                                                                    *
     * *  if  one is found   them                                                       *
     * *    fileDesc is deleted                                                         *
     * *    exiting one will be retuned.                                                *
     * *  else                                                                          *
     * *    fileDesc will be added to existinList (exisitingDescriptions) and returned. *
     * *    and returned.                                                               *
     * **********************************************************************************
     @endcode
     @param[in] fileDesc Pointer to a FileDesc object that you want to look and see if one that is identical already exists.
     @return pointer to the 'FileDesc' instance that the caller should be using.
     */
    static
      FileDescPtr     GetExistingFileDesc (FileDescPtr  fileDesc);

    const
      MLL::AttributePtr    LookUpByName (const KKStr&  attributeName)  const;  


    void  DisplayAttributeMappings ();

    /** 
     *@brief Returns true if file description on the right size is identicle. 
     *@details Both FileDesc instances must have the same number of fields, the fields
     *  must have the same names(NOT case sensative), and the fields must have 
     *  matching types(ex numericle, ordinal, etc).
     */
    bool  operator== (const FileDesc&  rightSize)  const;


    /** 
     *@brief Returns true if file description on the right size is NOT identicle.
     *@details If both FileDesc instances have differnet number of fields, or any one 
     *  of the fields has a different name(NOT case sensative), or one of the fields
     *  is of a different type.
     */
    bool  operator!= (const FileDesc&  rightSize)  const;


    /**
     *@brief  Mergs the Symbolic fields of two different 'FileDesc' instances
     * producing a new instance of a 'FileDesc'.  This method will only work
     * if both instances have the same number of fields, their names must be
     * the same(NOT case sensative), and each field in both instances must be
     * the same type.  If all these constions are not 'true' will retirn NULL.
     * The fields that are of 'SymbolicAttribute' will have their values merged 
     * together.  
     *@see MLL:Attribute
     */
    static FileDescPtr  MergeSymbolicFields (const FileDesc&  left,
                                             const FileDesc&  right,
                                             RunLog&          log
                                            );
  public:
    void  AddANominalValue (int32         fieldNum,
                            const KKStr&  nominalValue,
                            bool&         alreadyExist,
                            RunLog&       log
                           );


    void  AddANominalValue (const KKStr&  nominalValue,
                            bool&         alreadyExist,
                            RunLog&       log
                           );


    void  AddANominalValue (const KKStr&  attributeName,
                            const KKStr&  nominalValue,
                            bool&         alreadyExist,
                            RunLog&       log
                           );

  private:
    static void  CreateBlocker ();

    int32   NumOfAttributes ()  {return attributes.QueueSize ();}


    void  ValidateFieldNum (int32        fieldNum,
                            const char*  funcName
                           )  const;

    MLL::AttributeList    attributes;
    AttributeTypeVector   attributeVector;
    VectorInt32           cardinalityVector;
    MLClassConstList      classes;
    KKStr                 classNameAttribute;   /**< Added to support DstWeb files.  The name of the attribute that specifies the className */
    MLL::AttributePtr     curAttribute;
    KKStr                 fileName;
    //RunLog&             log;
    int32                 sparseMinFeatureNum;  /**< Used specifically for sparse files.  */
    short                 version;

    static
      KKU::GoalKeeperPtr  blocker;

    static
      FileDescListPtr     exisitingDescriptions;  /**< Will keep a list of all FileDesc instancs instatiated. */

    static
      bool                finalCleanUpRanAlready;

  };  /* FileDesc */


  typedef  FileDesc::FileDescPtr  FileDescPtr;  
}  /* namespace MLL  */

#define  _FileDescDefined_



#endif

