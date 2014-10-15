#ifndef _PICESCLASS_
#define _PICESCLASS_


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;


#include  "MLClass.h"
#include  "ImageFeatures.h"
#include  "PicesRunLog.h"
using namespace  KKU;
using namespace  MLL;


namespace PicesInterface 
{
  /*!
   \author  Kurt A. Kramer
   \class  PicesClass
   \brief  Represents a single 'Class' in the machine learning world.
   \details 
    Machine Learning Class.  Keep in mind that when we use the word 'class' in these 
    comments we are referring the Machine Learning concept of 'class' and not the c++ 
    or c# idea.  The idea is that for any image or example(more general) a class can 
    be assigned to them.  This class is to indicate a grouping which will allow us to 
    group like images/examples together.

    PicesClass is the Managed Version of the unmanaged class 'MLL::MLClass'.  For 
    every instance of this class there will be a matching instance of a 'MLClass' 
    object.  Also just like 'MLClass' there can only be one instance of a class 
    with the same name.  This helps simplify other programming because we can compare 
    pointers or handles directly to know if we are talking about the same class.  To 
    accomplish this, the creation of new Classes is done through a static method in 
    the PicesClassList object called PicesClassList::GetUniqueClass.
   */

  public ref  class PicesClass
  {
  public:
    PicesClass (String^       _name,
                PicesClass^   _parent
               );

    PicesClass (String^  _name,
                String^  _parentName
               );

    property  int                 ClassId              {int          get ();}
    property  String^             Description          {String^      get () {return description;};  void set (String^ _description);}
    property  String^             Name                 {String^      get () {return name;}}
    property  String^             NameUpper            {String^      get () {return nameUpper;}}
    property  uint16              NumHierarchialLevels {uint16       get ();}
    property  PicesClass^         Parent               {PicesClass^  get () {return parent;}        void set (PicesClass^ _parent);}
    property  String^             ParentName           {String^      get ();}
    property  List<PicesClass^>^  Children             {List<PicesClass^>^ get ();}
    property  bool                StoredOnDataBase     {bool         get ();                        void set (bool _StoredOnDataBase);}  
    property  bool                UnDefined            {bool         get () {return unDefined;}}                                           /**< Inidicates that class undefined or Noise */


    void    AddAChild  (PicesClass^  child);

    bool    ChangeNameOfClass (String^ newName);  /**< Will only change the name if no other instance of 'MLClass' is using the new name */

    void    CreateAUnManagedClass ();

    void    DeleteFromDisk (String^  directory);

    PicesClass^  MLClassForGivenHierarchialLevel (uint level);

    bool    IsEqual (PicesClass^ c);

    bool    IsInMySubTree (PicesClass^  candidate);

    /**
     @brief  Determines the number of generations between this class and the candidate class.  
     @details We search all our children to determine if Candidate is one our decendents. If 
              it is not we return -1 otherwise we return the number of generations below us.
              Our imediate children will be considered 1 generation apart.
     @param[in] Candidate Class that we are searching for amongst out children.
     @return The number of generations between us and the candidate or -1 if not one of our decendents.
     */
    int     NumGenerations (PicesClass^  candidate);

    int     NumOfChildren ();

    bool    ParentLoopCheck (PicesClass^ startingClass);

    void    RemoveAChild (PicesClass^ child);
      
    void    Save (String^  directory);

    PicesClass^  SearchTree (String^  _nameUpper);

    PicesClass^  SearchFamilyTree (String^      _nameUpper,
                                   PicesClass^  _nodeSearchingFrom
                                  );

    /**
     @brief Returns a pointer to the unmanaged version of this Class.
     */
    MLClassPtr  UnmanagedMLClass ();


    static String^  GetClassNameFromDirName (String^  subDir);

    /**
     *@brief Will change the name of an existing class verifying that a duplicate does not get created.
     *@details Will make sure that the new name is not in use by another class     *
     *@param[in]  mlClass  Pointer to existing MLClass instance that you wish to rename.
     *@param[in]  newName  New name that you with to give instance of 'MLClass'
     *@param[out] changeSuccessful Will return'True' if change was successful,  only reason it would not work is the name is already used.
     */
    static  void  ChangeNameOfClass (PicesClass^  mlClass, 
                                     String^      newName,
                                     bool%        changeSuccessful
                                    );


  private:
    String^       description;
    String^       name;
    String^       nameUpper;
    PicesClass^   parent;
    MLClassPtr    unmanagedMLClass;  // Our link to the unmanaged version of this class;
    bool          unDefined;

    List<PicesClass^>^  children;
  
  };  /* PicesClass */

}  /* PicesInterface */

#endif
