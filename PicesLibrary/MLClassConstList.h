#if  !defined(_IMAGECLASSCONSTLIST_)
#define  _IMAGECLASSCONSTLIST_

/**
 *@class  MLL::MLClass
 *@brief  Represents a "Class" in the Machine Learning Sense.
 *@author  Kurt Kramer
 *@details
 *@code
 **********************************************************************
 **                             MLClass                               *
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
#include "KKQueueConst.h"


namespace MLL
{
#if  !defined(_MLClassDefined_)
  class  MLClass;
  typedef  MLClass*  MLClassPtr;
  typedef  MLClass  const  MLClassConst;
  typedef  MLClassConst*  MLClassConstPtr;
#endif

#if  !defined(_MLClassListDefined_)
  class  MLClassList;
  typedef  MLClassList*  MLClassListPtr;
#endif


  /**
   *@class MLClassList
   *@brief  Maintains a list of MLClass instances.
   *@details  There will be only one instance of each MLClass by name and these instances will 
   *          be owned by MLClass::existingMLClasses.
   */
  class  MLClassConstList:  public KKQueueConst<MLClass>
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;

    typedef  MLClassConstList*  MLClassConstListPtr;

    MLClassConstList ();

    /** @brief Copy constructor;  will copy list but not own the contents. */
    MLClassConstList (const MLClassConstList&  _mlClasses);

    MLClassConstList (const MLClassList&  _mlClasses);

    /** @brief Construct a MLClassList object from the contents of a file. */
    MLClassConstList (const KKStr&  fileName,
                         bool&         successfull
                        );

    virtual
      ~MLClassConstList ();


    virtual
    void   AddMLClass (MLClassConstPtr  _mlClass);


    static
    MLClassConstListPtr  BuildListFromDelimtedStr (const KKStr&  s,
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


    MLClassConstListPtr  ExtractListOfClassesForAGivenHierarchialLevel (int32 level)  const;


    /** @brief  return pointer to instance with '_name';  if none exists, create one and add to list. */
    virtual
    MLClassConstPtr  GetMLClassPtr (const KKStr& _name);

    MLClassConstPtr  GetNoiseClass ()  const;

    /**
     *@brief Return a pointer to the MLClass object that represents the unknown Class in the list. 
     *@details That is the one with the name 'UNKNOWN'.  If none is found then one will be created 
     * and added to the list.
     */
    MLClassConstPtr   GetUnKnownClass ();


    /**
     *@brief  Returns a pointer of MLClass object with name (_name);  if none 
     *        in list will then return NULL.
     *@param[in]  _name  Name of MLClass to search for.
     *@return  Pointer to MLClass or NULL  if not Found.
     */
    virtual
    MLClassConstPtr  LookUpByName (const KKStr&  _name)  const;


    virtual
    MLClassConstPtr  LookUpByClassId (int32  _classId)  const;


    void           Load (const KKStr&  _fileName,
                         bool&         _successfull
                        );

    int32          MemoryConsumedEstimated () const;
      
    static
      MLClassConstListPtr  MergeClassList (const MLClassConstList&  list1,
                                              const MLClassConstList&  list2
                                             );


    KKU::uint16    NumHierarchialLevels ()  const;


    virtual
    MLClassConstPtr  PopFromBack ();

    virtual
    MLClassConstPtr  PopFromFront ();

    virtual
    void           PushOnBack (MLClassConstPtr  mlClass);

    virtual
    void           PushOnFront (MLClassConstPtr  mlClass);


    void           Save (KKStr   _fileName,
                         bool&   _successfull
                        );

    void           SortByName ();

    KKStr          ToString ()  const; 

    KKStr          ToCommaDelimitedStr ()  const;

    KKStr          ToTabDelimitedStr ()  const;

    void           WriteXML (std::ostream&  o)  const;

    bool           operator== (const MLClassConstList&  right)  const;

    bool           operator!= (const MLClassConstList&  right)  const;

                                                                       
    MLClassConstList& operator=  (const MLClassConstList&  right);

    MLClassConstList& operator-= (const MLClassConstList&  right);         /**< remove all classes that in the 'right' parameter  */
    
    MLClassConstList  operator-  (const MLClassConstList&  right)  const;  /**< remove all classes that in the 'right' parameter  */

    MLClassConstList& operator+= (const MLClassConstList&  right);         /**< add all classes that are in the 'right' parameter */


  private:
    void  AddMLClassToNameIndex (MLClassConstPtr  _mlClass);


    typedef  map<KKStr,MLClassConstPtr>   NameIndex;
    NameIndex     nameIndex;

    bool      undefinedLoaded;  /**< Indicates if the class that represents examples that have not beed 
                                 *   classified yet has been loaded.
                                 */

    class  mlClassNameComparison;
  };  /* MLClassConstList */

  typedef  MLClassConstList::MLClassConstListPtr  MLClassConstListPtr;

#define  _MLClassConstListDefined_



  std::ostream&  operator<< (      std::ostream&         os, 
                             const MLClassConstList&  classList
                            );


  KKStr&  operator<< (      KKStr&                str, 
                      const MLClassConstList&  classList
                     );


  typedef  MLClassConstList*  MLClassConstListPtr;

}  /* namespace MLL */


#endif

