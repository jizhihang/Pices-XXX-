#include "StdAfx.h"
#include "FirstIncludes.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

#include "MemoryDebug.h"
#include "BasicTypes.h"

#include "..\BaseLibrary\GoalKeeper.h"
#include "Raster.h"
#include "OSservices.h"
using namespace KKU;


#include "PicesClassList.h"
#include "PicesKKStr.h"
#include "PicesPoint.h"
#include "PicesRaster.h"

#include "PicesDataBaseImage.h"

using namespace  PicesInterface;

using namespace System;
using namespace System::Windows::Forms;



PicesDataBaseImage::PicesDataBaseImage ():
   image (NULL)
{
}




PicesDataBaseImage::PicesDataBaseImage (DataBaseImagePtr  _image):
   image (_image)
{
}




PicesDataBaseImage::~PicesDataBaseImage ()
{
  this->!PicesDataBaseImage ();
}



PicesDataBaseImage::!PicesDataBaseImage ()
{
  delete  image;
  image = NULL;
}



PicesPointList^  PicesDataBaseImage::SizeCoordinates::get ()
{
  if  (image->SizeCoordinates () == NULL)
    return gcnew PicesPointList ();
  else
    return gcnew PicesPointList (*(image->SizeCoordinates ()));
}


void  PicesDataBaseImage::SizeCoordinates::set (PicesPointList^ _sizeCoordinates)
{
  PointListPtr  coordinates = _sizeCoordinates->ToUnmanaged ();
  image->SizeCoordinates (coordinates);
  coordinates = NULL;
}



float   PicesDataBaseImage::ImageSize::get ()
{
  return image->ImageSize ();
}


  
void  PicesDataBaseImage::ImageSize::set (float _imageSize)
{
  image->ImageSize (_imageSize);
}




Image^  PicesDataBaseImage::Thumbnail ()
{
  if  (!image->EncodedThumbnail ())
     return  nullptr;

  // See "SimpleCompressor" 

  PicesRaster^  pr = nullptr;
  try
  {
    pr = gcnew PicesRaster (RasterSipper::FromSimpleCompression (image->EncodedThumbnail (), image->EncodedThumbnailLen ()));
  }
  catch  (Exception^  e)
  {
    System::Windows::Forms::MessageBox::Show ("Exception creating Thumbnail Image.\n\n" +
                                              "ImageFileName[" + ImageFileName+ "]  Height[" + Height + "]  Width[" + Width + "]\n\n" +
                                              e->ToString (),
                                              "PicesDataBaseImage::Thumbnail"
                                             );
    pr = gcnew PicesRaster (50, 50);
  }

  Image^ thumbNail = pr->BuildBitmap ();
  delete  pr;
  pr = nullptr;

  return  thumbNail;
}





String^   PicesDataBaseImage::Class1Name::get ()
{
  return  PicesKKStr::KKStrToSystenStr (image->Class1Name ());
}



String^   PicesDataBaseImage::Class2Name::get ()
{
  return  PicesKKStr::KKStrToSystenStr (image->Class2Name ());
}



PicesClass^  PicesDataBaseImage::ValidatedClass::get ()
{
  if  ((image == NULL)  ||  (image->ValidatedClass () == NULL))
    return  nullptr;
  else
    return  PicesClassList::GetUniqueClass (image->ValidatedClass ());
}



void  PicesDataBaseImage::ValidatedClass::set (PicesClass^ _validatedCass)
{
  if  (_validatedCass == nullptr)
    image->ValidatedClass (NULL);
  else
    image->ValidatedClass (_validatedCass->UnmanagedMLClass ());
}



String^  PicesDataBaseImage::ValidatedClassName::get ()
{
  return  PicesKKStr::KKStrToSystenStr (image->ValidatedClassName ());
}



String^   PicesDataBaseImage::ImageFileName::get ()
{
  return  PicesKKStr::KKStrToSystenStr (image->ImageFileName ());
}



PicesRaster^  PicesDataBaseImage::GetImageFromSipperFile (PicesRunLog^  runLog)   // Will return Original image from SipperFile using coordinates from database.  "TopLeftRow, TopLeftRow. Height, Width"
{
  if  (image == NULL)
    return nullptr;

  RasterSipperPtr  r = image->GetOrigImageFromSipperFile (runLog->Log ());
  if  (r == NULL)
    return nullptr;

  return gcnew PicesRaster (r);
}  /* GetImageFromSipperFile */



int  PicesDataBaseImage::CompareClass2 (PicesDataBaseImage^  right)
{
  DataBaseImagePtr   rightImage = right->UnManagedDataBaseImage ();

  if  (image->ValidatedClass () != NULL)
  {
    if  (rightImage->ValidatedClass () == NULL)
      return -1;
  }
  else if  (rightImage->ValidatedClass () != NULL)
  {
    return 1;
  }

  int  result = image->Class2Name ().CompareTo (rightImage->Class2Name ());
  if  (result == 0)
  {
    if  (image->Class2Prob () > rightImage->Class2Prob ())
      result = -1;
    else if  (image->Class2Prob () < rightImage->Class2Prob ())
      result = 1;
    else
      result = 0;
  }

  return  result;
}  /* CompareClass2 */



int  PicesDataBaseImage::CompareBreakTie (PicesDataBaseImage^  right)
{
  DataBaseImagePtr   rightImage = right->UnManagedDataBaseImage ();
  if  (image->BreakTie () < rightImage->BreakTie ())
    return -1;
  else if  (image->BreakTie () > rightImage->BreakTie ())
    return 1;
  else
    return  CompareSpatially (right);
}


int  PicesDataBaseImage::CompareDepth (PicesDataBaseImage^  right)
{
  DataBaseImagePtr   rightImage = right->UnManagedDataBaseImage ();
  if  (image->Depth () < rightImage->Depth ())
    return -1;
  else if  (image->Depth () > rightImage->Depth ())
    return 1;
  else
    return  CompareSpatially (right);
}  /* CompareDepth */




int  PicesDataBaseImage::CompareSpatially (PicesDataBaseImage^  right)
{
  DataBaseImagePtr   rightImage = right->UnManagedDataBaseImage ();
  return  image->ImageFileName ().CompareTo (rightImage->ImageFileName ());
}



PicesDataBaseImageList::PicesDataBaseImageList ()
{
}



PicesDataBaseImageList::PicesDataBaseImageList (DataBaseImageListPtr  unmanagedImages)
{
  if  (unmanagedImages == NULL)
    return;

  DataBaseImageList::iterator  idx;
  for  (idx = unmanagedImages->begin ();  idx != unmanagedImages->end ();  idx++)
  {
    DataBaseImagePtr  i = *idx;
    PicesDataBaseImage^ pi = gcnew PicesDataBaseImage (i);
    Add (pi);
  }
  unmanagedImages->Owner (false);
}



PicesDataBaseImageList::~PicesDataBaseImageList ()
{
  CleanUpMemory ();
}


PicesDataBaseImageList::!PicesDataBaseImageList ()
{
  CleanUpMemory ();
}



void  PicesDataBaseImageList::CleanUpMemory ()
{
  Clear ();
}



void  PicesDataBaseImageList::AddList (PicesDataBaseImageList^ list)
{
  for each  (PicesDataBaseImage^ i in list)
  {
    Add (i);
  }
}  /* AddList */



PicesClassList^  PicesDataBaseImageList::ExtractListOfClasses ()
{
  PicesClassList^  result = gcnew PicesClassList ();

  for each  (PicesDataBaseImage^ i in *this)
  {
    if  (result->LookUpIndex (i->Class1) < 0)
      result->Add (i->Class1);
  }
  return  result;
}  /* ExtractListOfClasses */



void  PicesDataBaseImageList::RandomizeOrder ()
{
  Random^ rng = gcnew System::Random ();
  int  i, j;
  int  numEntries = Count;
  if  (numEntries < 2)
    return;

  i = numEntries - 1;
  while (true)  
  {
    j = rng->Next (0, i);
    PicesDataBaseImage^ temp = this[i];
    this[i] = this[j];
    this[j] = temp;
    if  (i < 1)
      break;
    i--;
  }
}  /* RandomizeOrder */



PicesDataBaseImageList^  PicesDataBaseImageList::ExtractImagesForAGivenClass (PicesClass^  _class)
{
  PicesDataBaseImageList^  result = gcnew PicesDataBaseImageList ();
  for each  (PicesDataBaseImage^ i in *this)
  {
    if  (Object::ReferenceEquals (i->Class1, _class))
      result->Add (i);
  }
  return  result;
}  /* ExtractImagesForAGivenClass  */




PicesDataBaseImageList::PicesDataBaseImageComparer::PicesDataBaseImageComparer 
           (SortOrderType  _sortOrder, 
            bool           _reverseSort
           ):
      sortOrder   (_sortOrder),
      reverseSort (_reverseSort)
  {}
                                   




int  PicesDataBaseImageList::PicesDataBaseImageComparer::Compare 
     (PicesDataBaseImage^ x, 
      PicesDataBaseImage^ y
     )
{
  int  result = 0;

  if  (x == nullptr)
    result = (y == nullptr) ? 0 : -1;

  else if  (y == nullptr)
    result = 1;

  else
  {
    switch  (sortOrder)
    {
    case  SortOrderType::soNULL:                result = 0;                                                         break;
    case  SortOrderType::soBreakTie:            result = x->CompareBreakTie       (y);                              break;
    case  SortOrderType::soClassName:           result = x->Class1Name->CompareTo (y->Class1Name);                  break;
    case  SortOrderType::soDepth:               result = x->CompareDepth          (y);                              break;
    case  SortOrderType::soProbability:         result = x->Class1Prob.CompareTo  (y->Class1Prob);                  break;
    case  SortOrderType::soClass2:              result = x->CompareClass2         (y);                              break;
    case  SortOrderType::soSize:                result = x->PixelCount.CompareTo  (y->PixelCount);                  break;
    case  SortOrderType::soSpatial:             result = x->CompareSpatially      (y);                              break;
    case  SortOrderType::soValidatedClassName:  result = x->ValidatedClassName->CompareTo (y->ValidatedClassName);  break;
    }
  }

  if  (result == 0)
  {
    // Since we are still tied;  we will compre in temporal order.
    result = x->SipperFileName->CompareTo (y->SipperFileName);
    if  (result == 0)
      result = x->TopLeftRow.CompareTo (y->TopLeftRow);
  }

  if  (reverseSort)
    result = 0 - result;  // Revese the result of th ecomparision.

  return  result;
}  /* Compare */




void  PicesDataBaseImageList::Sort (SortOrderType  _sortOrder, 
                                    bool           _reverseSort
                                   )
{
  PicesDataBaseImageComparer^  comparer = gcnew PicesDataBaseImageComparer (_sortOrder, _reverseSort);

  List<PicesDataBaseImage^>::Sort (comparer);
}  /* Sort */




String^  PicesDataBaseImageList::SortOrderTypeToStr (SortOrderType  so)
{
  String^  result = nullptr;

  switch (so)
  {
  case  SortOrderType::soNULL:                result = "Null";         break;
  case  SortOrderType::soBreakTie:            result = "BreakTie";     break;
  case  SortOrderType::soClassName:           result = "ClassName";    break;
  case  SortOrderType::soDepth:               result = "Depth";        break;
  case  SortOrderType::soProbability:         result = "Probability";  break;
  case  SortOrderType::soClass2:              result = "Class2";       break;
  case  SortOrderType::soSize:                result = "Size";         break;
  case  SortOrderType::soSpatial:             result = "Spatial";      break;
  case  SortOrderType::soValidatedClassName:  result = "ValidatedName";    break;
   
  }
  return  result;
}  /* SortOrderTypeToStr */



PicesDataBaseImageList::SortOrderType  PicesDataBaseImageList::SortOrderTypeFromStr (String^  s)
{
  s = s->ToUpper ();

  if  (s == "NULL")           return  SortOrderType::soNULL;
  if  (s == "BREAKTIE")       return  SortOrderType::soBreakTie;
  if  (s == "CLASSNAME")      return  SortOrderType::soClassName;
  if  (s == "DEPTH")          return  SortOrderType::soDepth;
  if  (s == "PROBABILITY")    return  SortOrderType::soProbability;
  if  (s == "CLASS2")         return  SortOrderType::soClass2;
  if  (s == "SIZE")           return  SortOrderType::soSize;
  if  (s == "SPATIAL")        return  SortOrderType::soSpatial;
  if  (s == "VALIDATEDNAME")  return  SortOrderType::soValidatedClassName;

  return  SortOrderType::soNULL;
}  /* SortOrderTypeFromStr */



array<PicesDataBaseImageList::SortOrderType>^    PicesDataBaseImageList::SortOrderTypeList ()
{
  array<SortOrderType>^  sortOrderTypeList = gcnew array<SortOrderType> (9);
  sortOrderTypeList[0] = SortOrderType::soNULL;
  sortOrderTypeList[1] = SortOrderType::soBreakTie;
  sortOrderTypeList[2] = SortOrderType::soClassName;
  sortOrderTypeList[3] = SortOrderType::soDepth;
  sortOrderTypeList[4] = SortOrderType::soProbability;
  sortOrderTypeList[5] = SortOrderType::soClass2;
  sortOrderTypeList[6] = SortOrderType::soSize;
  sortOrderTypeList[7] = SortOrderType::soSpatial;
  sortOrderTypeList[8] = SortOrderType::soValidatedClassName;

  return  sortOrderTypeList;
}  /* SortOrderTypeList */



void  PicesDataBaseImageList::PopulateComboBoxWithSortOrderOptions (ComboBox^  cb)
{
  cb->Items->Clear ();
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soSpatial));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soBreakTie));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soClassName));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soValidatedClassName));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soDepth));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soSize));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soProbability));
  cb->Items->Add (SortOrderTypeToStr (SortOrderType::soClass2));
}
