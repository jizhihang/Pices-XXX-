#if  !defined(_MLCLASS_)
#define  _MLCLASS_

/**
 *@class  MLL::MLClass
 *@brief  Represents a "Class" in the Machine Learnining Sense.
 *@author  Kurt Kramer
 *@details
 *@code
 **********************************************************************
 **                            MLClass                                *
 **                                                                   *
 **  Each instance of this class represents a single Class as used in *
 **  Machine Learning sense.  Each instance of 'FeatureVector' class  *
 **  will point to an instance of this class.  There can only be one  *
 **  instance of each Class in memory.  Specifically the 'name' field *
 **  will be unique.  This is enforced by making the constructor and  *
 **  destructor private.  The only way to create a new instance of a  *
 **  'MLClass' object is to call one of the the static methods of     *
 **  'CreateNewMLClass'.  These methods will look for a instance   *
 **  of 'MLClass' that already exists.  If one does not they will     *
 **  then create a new one.                                           *
 **                                                                   *
 **  Please refer to MLClassList at bottom of file.  That class is    *
 **  the object you will most be using when dealing with Images.      *
 **  classes.                                                         *
 **                                                                   *
 **  PicesClass  
 **  There is a special relationship between this class  and a class  *
 **  called 'PicesClass' in the 'PicesInterface' library'.  PicesClass*
 **  is a managed c++ version of this class.  There is a one-to-one   *
 **  correspondence between the two classes.  When ever a instance    *
 **  of 'PicesClass' gets created it will automatically call the      *
 **  the static method 'MLClass::CreateNewMLClass' to get the      *
 **  unmanaged version of the class.                                  *
 **                                                                   *
 **                                                                   *
 **  classId   - A unique number that is assigned by the Pices        *
 **              DataBase when a new class is added to it.            *
 **                                                                   *
 **  name      - Name of class; This name will be unique for the      *
 **              process. Underscore characters have special meaning  *
 **              to several methods in PicesLibrary.  They help       *
 **              separate the name into multiple class levels.        *
 **                                                                   *
 **  num       - Used by SVMModel when creating Training Models and   *
 **              Classifying an instance of a imageFeatures           *
 **                                                                   *
 **  parent    - Pointer to a instance of 'MLClass' that is the       *
 **              parent to this instance. And example would be a      *
 **              class called 'Copepod_Calanoid' will have a parent   *
 **              called 'Copepod'.  The parent class to all classes   *
 **              is "AllClasses"                                      *
 **                                                                   *
 **  unDefined - When set to true indicates that this class is not    *
 **              a Plankton or has not yet been classified as a       *
 **              Plankton.  This is set automatically when name is    *
 **              set. Names such as NOISE, UNKNOWN, UNDEFINED, NOISY, *
 **              and NONPLANKTON will be set to true  otherwise       *
 **              false.  The names are NOT case sensitive so for      *
 **              example  Noise, UnKnown, and noisy  would set  it    *
 **              false.                                               *
 **********************************************************************
 *@endcode
 *@see MLL::MLClassList, MLL::FeatureVector, PicesInterface::PicesClass
 *@see MLL::DataBase::MLClassLoad, MLL::DataBase::ImageInsert
 */


#include "GoalKeeper.h"
#include "RunLog.h"
#include "Str.h"
#include "KKQueue.h"


namespace MLL
{
#if  !defined(_MLClassConstListDefined_)
  class  MLClassConstList;
  typedef  MLClassConstList*  MLClassConstListPtr;
#endif

  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  typedef  MLClass  const  MLClassConst;
  typedef  MLClassConst*  MLClassConstPtr;

  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;

  class  MLClass 
  {
  private:
    static  MLClassListPtr   existingMLClasses;
    static  map<MLClassListPtr,MLClassListPtr>  existingClassLists;

    static  void  AddImageClassList    (MLClassListPtr  list);
    static  void  DeleteImageClassList (MLClassListPtr  list);


  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    static  MLClassListPtr  GlobalClassList ();

    /**
     *@brief  Static method used to create a new instance of a MLClass object.
     *@details
     *@code
     ************************************************************************
     ** Used to get pointer to existing MLClass object that already      *
     ** exists in 'existingMLClasses'.  If one does not exist then a new *
     ** MLClass object with this name will be created.                   *
     **                                                                     *
     ** This is the only method that can actually create a MLClass       *
     ** instance.  The idea is that there is only one MLClass object     *
     ** ever created for each class.  All MLClassList container objects  *
     ** will point to instances that are in 'existingMLClasses.          *
     ************************************************************************
     *@endcode
     *@param[in]  _name   Name of class to be created.
     *@return  Pointer to an instance of 'MLClass' that will have the name '_name'.
     */
    static  MLClassPtr       CreateNewMLClass (const KKStr&  _name,
                                                  int32         _classId = -1
                                                 );

    static  MLClassConstPtr  GetUnKnownClassStatic ();

    static  MLClassConstPtr  GetByClassId (int32  _classId);

    /**  
     *@brief  Changes the name of an existing class verifying that a duplicate does not get created.
     *@details Since the class name can not be duplicated and there are nameIndex structures maintained by each
     * 'MLClassList' instances we need to make sure the name is not already in use by another instance of 
     * 'mlClass'.  This is done by first trying to update the 'nameIndex' in 'existingMLClasses' (list
     * of all 'inageClass' instances in existance); if tjs is sucessful will then change the name in 'mlClass'
     * and update all existing 'MLClassList' instances 'nameIndex' structures.
     *
     *@param[in,out] mlClass Class having its name changed; upon entry should contain its original name; if
     *                          no other class already has its name it will be updated to the value in 
     *                          'newName'.
     *@param[in] newName  The new name that 'mlClass' is to receive.
     *@param[in] successful Returns 'true' if name was changed; if set to false then the 'name' field in 'mlClass'
     *                       will not be changed.
     */
    static  void  ChangeNameOfClass (MLClassPtr  mlClass, 
                                     const KKStr&   newName,
                                     bool&          successful
                                    );

    static  void  ResetAllParentsToAllClasses ();


    /** @brief Call this as the last thing the application does, will delete all existing instances of 'MLClass'. */
    static  void  FinalCleanUp ();


  private:
    static void  CreateBlocker ();

    static GoalKeeperPtr  blocker;
    static bool  needToRunFinalCleanUp;

    friend class KKQueue<MLClass>;
    friend class MLClassList;

    MLClass (const KKStr&  _name);

    MLClass (const MLClass&  mlClass);

    ~MLClass ();

    void  Name (const KKStr&  _name);


  public:
    static KKStr    GetClassNameFromDirName (const KKStr&  subDir);

    static MLClassConstListPtr  BuildListOfDecendents (MLClassConstPtr  parent);
   

    int32               ClassId ()  const  {return classId;}
    void                ClassId (int32 _classId)  {classId = _classId;}

    void                Description (const KKStr&  _description)  {description = _description;}

    const KKStr&        Description ()  const {return description;}

    MLClassPtr       MLClassForGivenHierarchialLevel (KKU::uint16 level)  const;

    bool                IsAnAncestor (MLClassConstPtr  c)  const;          /**< @brief Returns true if 'c' is an ancestor */

    KKU::uint16         NumHierarchialLevels ()  const;

    const  KKStr&       Name ()      const {return  name;}
    const  KKStr&       UpperName () const {return  upperName;}  /**< @brief Returns name capitalized. */

    MLClassConstPtr  Parent () const {return parent;}
    void                Parent (MLClassConstPtr  _parent)  {parent = _parent;}
    const KKStr&        ParentName ()  const;

    void                ProcessRawData (KKStr&  data);  /**< @brief Parses 'data' and populates this instance
                                                         *   @details Extracts name of class from first field in 'data' using whitespace
                                                         *   ',', ' ', '\n', '\r', or '\t' as delimiter.  Data will have this name removed from
                                                         *   the beginning of it.
                                                         */

    bool                StoredOnDataBase () const  {return  storedOnDataBase;}

    void                StoredOnDataBase (bool _storedOnDataBase)  {storedOnDataBase = _storedOnDataBase;}

    KKStr               ToString ()  const;  /**< @brief Returns a KKStr representing this instance.
                                              *   @details This string will later be written to a file.
                                              */
                    

    bool                UnDefined ()  const  {return  unDefined;}

    void                UnDefined (bool _unDefined)  {unDefined = _unDefined;}

    void                WriteXML (std::ostream& o)  const;
    
private:
    int32               classId;      /**< @brief From MySQL table  Classes, '-1' indicates that not loaded from table        */

    KKStr               name;         /**< @brief Name of Class.                                                              */
    KKStr               upperName;    /**< @brief Upper case version of name;  Used by LookUpByName to assist in performance. */

    bool                unDefined;    /**< @brief A class whos name is "", "UnKnown", "UnDefined", or starts with "Noise_" */

    MLClassConstPtr  parent;       /**< @brief Supports the concept of Parent/Child classes as part of a hierarchy.
                                       *@details Adding this field to help support the PicesInterface version of this class.
                                       */

    bool                storedOnDataBase;   
                                      /**< @brief because we have no control over classes that users unilaterally create
                                       * it would be useful to know which ones are stored in the PICES database
                                       * table "Classes".
                                       */

    KKStr               description;
  };  /* MLClass */



#define  _MLClassDefined_



  /**
   *@class MLClassList
   *@brief  Maintains a list of MLClass instances.
   *@details  There will be only one instance of each MLClass by name and these instances will 
   *          be owned by MLClass::existingMLClasses.
   */
  class  MLClassList:  public KKQueue<MLClass>
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    MLClassList ();

    /** @brief Copy constructor;  will copy list but not own the contents. */
    MLClassList (const MLClassList&  _mlClasses);

    /** @brief Construct a MLClassList object from the contents of a file. */
    MLClassList (const KKStr&  fileName,
                    bool&         successfull
                   );

    virtual
      ~MLClassList ();


    virtual
    void   AddMLClass (MLClassPtr  _mlClass);


    static
    MLClassListPtr  BuildListFromDelimtedStr (const KKStr&  s,
                                                 char          delimiter
                                                );


    /** @brief  Clears the contens of this list and updates nameIndex structure. */
    virtual
      void  Clear ();
      

    /**
     *@brief  Using the class names create two title lines where we split
     *        names by "_" characters between the two lines.
     */
    void  ExtractTwoTitleLines (KKStr&  titleLine1,
                                KKStr&  titleLine2 
                               ) const;


    /** 
     *@brief Using the class names create three title lines where we split names 
     *       by "_" characters between the three lines.
     */
    void  ExtractThreeTitleLines (KKStr&  titleLine1,
                                  KKStr&  titleLine2, 
                                  KKStr&  titleLine3 
                                 ) const;

    void  ExtractThreeTitleLines (KKStr&  titleLine1,
                                  KKStr&  titleLine2, 
                                  KKStr&  titleLine3,
                                  int32   fieldWidth
                                 ) const;

    /**
     *@brief Will generate a HTML formated string that can be used in a HTML table.
     *@details Using the class names create one header line for a HTML table. The 
     *         underscore character ("_") will be used to separate levels
     */
    KKStr   ExtractHTMLTableHeader () const;


    MLClassListPtr  ExtractListOfClassesForAGivenHierarchialLevel (int32 level);


    /** @brief  return pointer to instance with '_name';  if none exists, create one and add to list. */
    virtual
    MLClassPtr  GetMLClassPtr (const KKStr& _name);

    MLClassPtr  GetNoiseClass ()  const;

    /**
     *@brief Return a pointer to the MLClass object that represents the unknown Class in the list. 
     *@details That is the one with the name 'UNKNOWN'.  If none is found then one will be created 
     * and added to the list.
     */
    MLClassPtr   GetUnKnownClass ();


    /**
     *@brief  Returns a pointer of MLClass object with name (_name);  if none 
     *        in list will then return NULL.
     *@param[in]  _name  Name of MLClass to search for.
     *@return  Pointer to MLClass or NULL  if not Found.
     */
    virtual
    MLClassPtr  LookUpByName (const KKStr&  _name)  const;


    virtual
    MLClassPtr  LookUpByClassId (int32  _classId)  const;


    void           Load (const KKStr&  _fileName,
                         bool&         _successfull
                        );

    int32          MemoryConsumedEstimated () const;
      
    static
      MLClassListPtr  MergeClassList (const MLClassList&  list1,
                                         const MLClassList&  list2
                                        );


    KKU::uint16    NumHierarchialLevels ()  const;


    virtual
    MLClassPtr  PopFromBack ();

    virtual
    MLClassPtr  PopFromFront ();

    virtual
    void           PushOnBack (MLClassPtr  mlClass);

    virtual
    void           PushOnFront (MLClassPtr  mlClass);


    void           Save (KKStr   _fileName,
                         bool&   _successfull
                        );

    void           SortByName ();

    KKStr          ToString ()  const; 

    KKStr          ToCommaDelimitedStr ()  const;

    KKStr          ToTabDelimitedStr ()  const;

    void           WriteXML (std::ostream&  o)  const;

    bool           operator== (const MLClassList&  right)  const;

    bool           operator!= (const MLClassList&  right)  const;

                                                                       
    MLClassList& operator=  (const MLClassList&  right);

    MLClassList& operator-= (const MLClassList&  right);         /**< remove all classes that in the 'right' parameter  */
    
    MLClassList  operator-  (const MLClassList&  right)  const;  /**< remove all classes that in the 'right' parameter  */

    MLClassList& operator+= (const MLClassList&  right);         /**< add all classes that are in the 'right' parameter */


  private:
    friend class MLClass;

    void  AddMLClassToNameIndex (MLClassPtr  _mlClass);


    /** @brief  Should only be called from "MLClass::ChangeNameOfClass". */
    void  ChangeNameOfClass (MLClassPtr  mlClass, 
                             const KKStr&   oldName,
                             const KKStr&   newName,
                             bool&          successful
                            );

    typedef  map<KKStr,MLClassPtr>     NameIndex;
    NameIndex     nameIndex;

    /**
     *@brief  Set the owner flag.
     *@details Forcing Owner to be private to make sure that no list can own any 
     * MLClass objects, to prevent accidental deletion of a 'MLClass'
     * object.  Only 'MLClass::existingMLClasses' may own the contents 
     * of its list.
     */
    void      Owner (bool _owner)  {KKQueue<MLClass>::Owner (_owner);}

    bool      undefinedLoaded;  /**< Indicates if the class that represents examples that have not beed 
                                 *   classified yet has been loaded.
                                 */

    class  mlClassNameComparison;
  };  /* MLClassList */

#define  _MLClassListDefined_





  std::ostream&  operator<< (      std::ostream&    os, 
                             const MLClassList&  classList
                            );


  KKStr&  operator<< (      KKStr&           str, 
                      const MLClassList&  classList
                     );


  typedef  MLClassList*  MLClassListPtr;






  /**
   *@brief  Maintains a list of classes and their associated integer index.  
   *@details 
   *  Multiple Classes can have the same index but a class may only appear once in the list.
   *  The primary purpose of this class is to allow the quick access to classes by numerical
   *  indexes.  This comes in useful when communicating with another library that does not
   *  recognize alpha numeric strings for class names such libSVM which only uses integers
   *  class names.
   *@see MLL::Model
   *@see MLL::FeatureEncoder2::compress
   */
  class  ClassIndexList: public  map<MLClassConstPtr, short>
  {
  public:
    typedef  ClassIndexList*  ClassIndexListPtr;

    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    ClassIndexList ();
    ClassIndexList (const ClassIndexList&       _list);
    ClassIndexList (const MLClassList&       _classes);
    ClassIndexList (const MLClassConstList&  _classes);

    virtual
      void  Clear ();

    void  AddClass (MLClassConstPtr  _ic,
                    bool&               _dupEntry
                   );


    void  AddClassIndexAssignment (MLClassConstPtr  _ic,
                                   short               _classIndex,
                                   bool&               _dupEntry
                                  );

    /** @brief  Returns the corresponding index to the class 'c';  if not in list will return -1. */
    short  GetClassIndex (MLClassConstPtr  c);


    /**
     *@brief  Locates the MLClass that was assigned classIndex.
     *@details If not found then returns NULL.  If more than one class has the same classIndex will return the first one added.
     */
    MLClassConstPtr  GetMLClass (short classIndex);

    int32  MemoryConsumedEstimated ()  const;

    void   ParseClassIndexList (const KKStr&  s);

    KKStr  ToCommaDelString ();


  private:
    map<short, MLClassConstPtr>  shortIdx;
    short                           largestIndex;   // largest index used so far.
  };  /* ClassIndexList */

#define  _ClassIndexListDefined_

  typedef  ClassIndexList::ClassIndexListPtr  ClassIndexListPtr;


  extern  MLClassList  globalClassList;

}  /* namespace MLL */


#endif

