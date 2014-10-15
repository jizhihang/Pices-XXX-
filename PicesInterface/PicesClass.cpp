#include  "StdAfx.h"
#include  "FirstIncludes.h"

#include  <stdio.h>
#include  <math.h>
#include  <ctype.h>
#include  <time.h>
#include  <fstream>
#include  <iostream>
#include  <map>
#include  <ostream>
#include  <string>
#include  <vector>
using namespace std;

#include  "MemoryDebug.h"
#include  "BasicTypes.h"

#include  "..\BaseLibrary\GoalKeeper.h"
using namespace KKU;

#include "MLClass.h"

//#include "PicesKKStr.h"
#include  "OSservices.h"

#include  "PicesClass.h"
#include  "PicesClassList.h"
#include  "PicesKKStr.h"


using namespace  PicesInterface;



using namespace System;
using namespace System::Windows::Forms;


namespace  PicesInterface
{

  String^  PicesClass::GetClassNameFromDirName (String^  subDir)
  {
    if  (String::IsNullOrEmpty (subDir))
      return  String::Empty;
    return  PicesKKStr::KKStrToSystenStr (MLClass::GetClassNameFromDirName (PicesKKStr::SystemStringToKKStr (subDir)));
  }



  void  PicesClass::ChangeNameOfClass (PicesClass^  mlClass, 
                                       String^      newName,
                                       bool%        changeSuccessful
                                      )
  {
    bool  successful = false;
    MLClass::ChangeNameOfClass (mlClass->UnmanagedMLClass (),
                                PicesKKStr::SystemStringToKKStr (newName),
                                successful
                               );
    changeSuccessful = successful;

    if  (successful)
    {
      mlClass->name = newName;
      mlClass->nameUpper = newName->ToUpper ();
    }
  }



  PicesClass::PicesClass (String^       _name,
                          PicesClass^   _parent
                         ):
       children            (gcnew List<PicesClass^> ()),
       description         (String::Empty),
       unmanagedMLClass (NULL),
       name                (_name),
       nameUpper           (_name->ToUpper ()),
       parent              (_parent),
       unDefined           (false)
  {
    unmanagedMLClass = MLClass::CreateNewMLClass (PicesKKStr::SystemStringToKKStr (name));
    if  (_parent != nullptr)
    {
      MLClassConstPtr  unmanagedParentClass = _parent->UnmanagedMLClass ();
      unmanagedMLClass->Parent (unmanagedParentClass);
    }
    unDefined = unmanagedMLClass->UnDefined ();
  }



  PicesClass::PicesClass (String^  _name,
                          String^  _parentName
                         ):

       children            (gcnew List<PicesClass^> ()),
       description         (String::Empty),
       unmanagedMLClass (NULL),
       name                (_name),
       nameUpper           (_name->ToUpper ()),
       parent              (nullptr),
       unDefined           (false)
  {
    unmanagedMLClass = MLClass::CreateNewMLClass (PicesKKStr::SystemStringToKKStr (name));
    if  (!String::IsNullOrEmpty (_parentName))
    {
      parent = PicesClassList::GetUniqueClass (_parentName, "");
      unmanagedMLClass->Parent (parent->UnmanagedMLClass ());
    }
    unDefined = unmanagedMLClass->UnDefined ();
  }



  MLClassPtr  PicesClass::UnmanagedMLClass ()
  {
    if  (unmanagedMLClass)
      return  unmanagedMLClass;

    unmanagedMLClass = MLClass::CreateNewMLClass (PicesKKStr::SystemStringToKKStr (name));
    return  unmanagedMLClass;
  }



  int  PicesClass::ClassId::get () 
  {
    return UnmanagedMLClass ()->ClassId ();
  }





  void  PicesClass::Description::set (String^ _description)
  {
    description = _description;
    if  (unmanagedMLClass)
      unmanagedMLClass->Description (PicesKKStr::SystemStringToKKStr (description));
  }



  uint16  PicesClass::NumHierarchialLevels::get ()
  {
    if  (unmanagedMLClass)
      return unmanagedMLClass->NumHierarchialLevels ();
    else
      return 0;
  }



  bool  PicesClass::ChangeNameOfClass (String^ _newName)  
  {
    KKStr  newName = PicesKKStr::SystemStringToKKStr (_newName);
    bool  changeSuccessful = false;
    MLClass::ChangeNameOfClass (unmanagedMLClass, newName, changeSuccessful);
    if  (changeSuccessful)
    {
      name = _newName;
      nameUpper = name->ToUpper ();
    }

    return  changeSuccessful;
  }  /* ChangeNameOfClass */





  String^  PicesClass::ParentName::get ()
  {
    if   (parent != nullptr)
      return  parent->Name;
    else
      return  String::Empty;
  }



  void  PicesClass::CreateAUnManagedClass ()
  {
    if  (!unmanagedMLClass)
    {
      unmanagedMLClass = MLClass::CreateNewMLClass (PicesKKStr::SystemStringToKKStr (name));
      unmanagedMLClass->Description (PicesKKStr::SystemStringToKKStr (description));
      if  (parent != nullptr)
        unmanagedMLClass->Parent (parent->UnmanagedMLClass ());
    }
  }  /* CreateAUnManagedClass*/




  void  PicesClass::Parent::set (PicesClass^ _parent)  
  {
    if  (_parent == parent)
      return;

    CreateAUnManagedClass ();

    if  (parent != nullptr)
      parent->RemoveAChild (this);
    parent = _parent;
  
    if  (parent != nullptr)
      parent->AddAChild (this);

    if  (parent == nullptr)
      unmanagedMLClass->Parent (NULL);
    else
      unmanagedMLClass->Parent (parent->UnmanagedMLClass ());
  }  /* Parent::set */




  List<PicesInterface::PicesClass^>^  PicesInterface::PicesClass::Children::get ()
  {
    return  children;
  }



  bool    PicesClass::StoredOnDataBase::get ()
  {
    if  (unmanagedMLClass)
      return  unmanagedMLClass->StoredOnDataBase ();
    else
      return  false;
  }



  void  PicesClass::StoredOnDataBase::set (bool _storedOnDataBase)
  {
    if  (!unmanagedMLClass)
      CreateAUnManagedClass ();
    unmanagedMLClass->StoredOnDataBase (_storedOnDataBase);
  }




  bool  PicesClass::IsEqual (PicesClass^ c)
  {
    return  ((name        == c->name)        &&
             (nameUpper   == c->nameUpper)   &&
             (description == c->description)
            );
  }



  int  PicesClass::NumOfChildren ()
  {
    if  (children == nullptr)
       return 0;

    return  children->Count;
  }



  void    PicesClass::AddAChild  (PicesClass^  child)
  {
    if  (child->Name == "AllClasses")
    {
      // This can not happen;  "AllClasses" must always be a root class.
      return;
    }

    if  (children == nullptr)
      children = gcnew List<PicesClass^> ();
    child->Parent = this;
        
    // Make sure we are not already in the children list.
    bool  alreadyInChildrenList = false;
    for  (int x = 0;  x < children->Count;  x++)
    {
      PicesClass^ c = (*children)[x];
      if  (c == child)
      {
        alreadyInChildrenList = true;
        break;
      }
    }
        
    if  (!alreadyInChildrenList)
      children->Add (child);
  }  /* AddAChild */





  void  PicesClass::RemoveAChild (PicesClass^ child)
  {
    if  (children == nullptr)
      return;
          
    int  x = 0;
    for  (x = 0;  x < children->Count;  x++)
    {
      if  ((*children)[x] == child)
      {
        children->RemoveAt (x);
        break;
      }  
    }
        
    if  (children->Count == 0)
    {
      children = nullptr;
    }
  }  /* RemoveAChild */




  bool  PicesClass::ParentLoopCheck (PicesClass^ startingClass)
  {

    if  (parent == this)
      return false;

    if  (startingClass == this)
      return true;
          
    if  (parent == nullptr)
      return false;

    return  parent->ParentLoopCheck (startingClass);
  }




  PicesClass^  PicesClass::SearchTree (String^  _nameUpper)
  { 
    if  (_nameUpper == nameUpper)
      return this;
        
    if  (children == nullptr)
      return nullptr;
          
    for  (int x = 0;  x < children->Count;  x++)
    {
      PicesClass^  c = (*children)[x];
      PicesClass^  aDecendentNode = c->SearchTree (_nameUpper);
      if  (aDecendentNode != nullptr)
        return  aDecendentNode;
    }
        
    return  nullptr;
  }  /* SearchTree */




  PicesClass^  PicesClass::SearchFamilyTree (String^      _nameUpper,
                                             PicesClass^  _nodeSearchingFrom
                                            )
  {
    if  (_nameUpper == nameUpper)
      return  this;
          
    if  ((parent != nullptr)  &&  (parent != _nodeSearchingFrom))
    {
      PicesClass^  relatedToParent = parent->SearchFamilyTree (_nameUpper, this);
      if  (relatedToParent != nullptr)
        return  relatedToParent;
    }
        
    for  (int x = 0;  x < children->Count;  x++)
    {
      PicesClass^  child = (*children)[x];
      if  (child  != _nodeSearchingFrom)
      {
        PicesClass^  relatedToChild = child->SearchFamilyTree (_nameUpper, this);
        if  (relatedToChild != nullptr)
          return  relatedToChild;
      }
    }
        
    return  nullptr;
  }  /* SearchFamilyTree */




  bool  PicesClass::IsInMySubTree (PicesClass^  candidate)
  {
    if  (candidate->nameUpper == nameUpper)
      return  true;
      
    if  (children == nullptr)
      return false;
          
    bool  isInMySubTree = false;
    for  (int x = 0;  x < children->Count;  x++)
    {
      PicesClass^ c = (*children)[x];
      if  (c->IsInMySubTree (candidate))
      {
        isInMySubTree = true;
        break;
      }
    }
        
    return  isInMySubTree;
  }  /* IsInMySubTree */
      




  // Will return the number of generations between this class and the candidate 
  // class.  If 'candidate' is not a decendent then will return -1.
  int  PicesClass::NumGenerations (PicesClass^  candidate)
  {
    for each  (PicesClass^ pc in children)
    {
      if  (pc == candidate)
        return 1;

      int  zed = pc->NumGenerations (candidate);
      if  (zed >= 0)
        return  zed + 1;
    }

    return  -1;
  }  /* NumGenerations */




      
      
  void  PicesClass::Save (String^  directory)
  {
    String^  fileName = OSservices::AddSlash (directory) + name + ".txt";

    StreamWriter^  sw = nullptr;

    try  {StreamWriter^  sw = gcnew StreamWriter (fileName, false);}
    catch  (Exception^ e)  
    {
      sw = nullptr;

      MessageBox::Show ("Error occured trying to create PicesClass Description file[" + fileName + "]" + "\n\n" +
                        e->ToString (),
                        "PicesClass.Save[" + fileName + "]",
                        MessageBoxButtons::OK
                       );
     }
        
     if  (sw != nullptr)
     {
       sw->WriteLine ("[IMAGE_CLASS]");
       sw->WriteLine ("Name="         + name);
       sw->WriteLine ("Description="  + description);
       if  (parent != nullptr)
         sw->WriteLine ("Parent="     + parent->Name);
       sw->Close ();
       delete  sw;
       sw = nullptr;
     }
  }  /* Save */




  void  PicesClass::DeleteFromDisk (String^  directory)
  {
    String^  fileName = OSservices::AddSlash (directory) + name + ".txt";
    FileInfo^  fi = nullptr;

    try  {fi = gcnew FileInfo (fileName);}  catch (Exception^)  {fi = nullptr;}
    if  (fi != nullptr)
    {
      fi->Delete ();
      delete  fi;
      fi= nullptr;
    }
  }  /* DeleteFromDisk */



  PicesClass^  PicesClass::MLClassForGivenHierarchialLevel (uint   level)
  {
    array<String^>^  levelNames = name->Split ('_');
    if  (level < 1)
      level = 1;

    String^  fullLevelName = "";

    int  curLevel = 0;
    while  ((curLevel < (int)level)  &&  (curLevel < levelNames->Length))
    {
      if  (curLevel < 1)
        fullLevelName = levelNames->GetValue (curLevel)->ToString ();
      else
      {
        fullLevelName += "_";
        fullLevelName += levelNames->GetValue (curLevel)->ToString ();
      }

      curLevel++;
    }

    return  PicesClassList::GetUniqueClass (fullLevelName, "");
  }


}  /* PicesInterface */
