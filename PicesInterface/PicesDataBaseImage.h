#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::Drawing;
using namespace System::Windows::Forms;

#include "DataBaseImage.h"

#include "PicesRunLog.h"
#include "PicesKKStr.h"
#include "PicesClass.h"
#include "PicesClassList.h"
#include "PicesPoint.h"
#include "PicesRaster.h"


namespace PicesInterface 
{
  public ref class PicesDataBaseImage
  {
  public:
    PicesDataBaseImage ();
    PicesDataBaseImage (DataBaseImagePtr  _image);

  protected:
    !PicesDataBaseImage ();

  private:
    ~PicesDataBaseImage ();

  public:
    property  float            BreakTie             {float            get () {return  image->BreakTie ();}}
    property  uint64           ByteOffset           {uint64           get () {return  image->ByteOffset ();}}
    property  PicesClass^      Class1               {PicesClass^      get () {return  PicesClassList::GetUniqueClass (image->Class1 ());}}
    property  String^          Class1Name           {String^          get ();}
    property  float            Class1Prob           {float            get () {return  image->Class1Prob ();}}
    property  PicesClass^      Class2               {PicesClass^      get () {return  PicesClassList::GetUniqueClass (image->Class2 ());}}
    property  String^          Class2Name           {String^          get ();}
    property  float            Class2Prob           {float            get () {return  image->Class2Prob           ();}}
    property  uint             ClassLogEntryId      {uint             get () {return  image->ClassLogEntryId      ();}}
    property  uchar            ConnectedPixelDist   {uchar            get () {return  image->ConnectedPixelDist   ();}}
    property  float            Depth                {float            get () {return  image->Depth                ();}}
    property  uint             ExtractionLogEntryId {uint             get () {return  image->ExtractionLogEntryId ();}}
    property  uint             Height               {uint             get () {return  image->Height ();}}
    property  String^          ImageFileName        {String^          get ();}
    property  uint             ImageId              {uint             get () {return  image->ImageId    ();}}
    property  float            ImageSize            {float            get ();  void  set (float _imageSize);}
    property  uint             TopLeftRow           {uint             get () {return  image->TopLeftRow ();}}
    property  uint             TopLeftCol           {uint             get () {return  image->TopLeftCol ();}}
    property  uint             PixelCount           {uint             get () {return  image->PixelCount ();}}
    property  String^          SipperFileName       {String^          get () {return  PicesKKStr::KKStrToSystenStr (image->SipperFileName ());}}

    property  PicesPointList^  SizeCoordinates      {PicesPointList^  get ();  void set (PicesPointList^  _sizeCoordinates);}

    property  PicesClass^      ValidatedClass       {PicesClass^      get ();  void set (PicesClass^ _validatedCass);}
    property  String^          ValidatedClassName   {String^          get ();}
    property  uint             Width                {uint             get () {return  image->Width ();}}



    Image^    Thumbnail ();

    PicesRaster^  GetImageFromSipperFile (PicesRunLog^  runLog);   // Will return Original image from SipperFile using coordinates from database.  "TopLeftRow, TopLeftRow. Height, Width"

    DataBaseImagePtr  UnManagedDataBaseImage ()   {return  image;}

    int  CompareBreakTie  (PicesDataBaseImage^  right);
    int  CompareClass2    (PicesDataBaseImage^  right);
    int  CompareDepth     (PicesDataBaseImage^  right);
    int  CompareSpatially (PicesDataBaseImage^  right);


  private:
    DataBaseImagePtr   image;
  };  /* PicesDataBaseImage */



  public  ref  class  PicesDataBaseImageList:  List<PicesDataBaseImage^>
  {
  public:
    PicesDataBaseImageList ();

    PicesDataBaseImageList (DataBaseImageListPtr  unmanagedImages);

    ~PicesDataBaseImageList ();
    !PicesDataBaseImageList ();

    enum  class  SortOrderType {soNULL, soBreakTie, soClassName, soDepth, soProbability, soClass2, soSize, soSpatial, soValidatedClassName};

    static  String^                SortOrderTypeToStr   (SortOrderType  so);
    static  SortOrderType          SortOrderTypeFromStr (String^  s);
    static  array<SortOrderType>^  SortOrderTypeList    ();
    static  void                   PopulateComboBoxWithSortOrderOptions (ComboBox^  cb);

    void  AddList (PicesDataBaseImageList^ list);

    void  CleanUpMemory ();

    PicesDataBaseImageList^  ExtractImagesForAGivenClass (PicesClass^  _class);

    PicesClassList^  ExtractListOfClasses ();

    void  RandomizeOrder ();

    void  Sort (SortOrderType  _sortOrder, 
                bool           _reverseSort
               );

  private:
    ref class PicesDataBaseImageComparer: System::Collections::Generic::IComparer<PicesDataBaseImage^>
    {
    public:
      PicesDataBaseImageComparer (SortOrderType  _sortOrder, 
                                  bool           _reverseSort
                                 );

      virtual int Compare (PicesDataBaseImage^ x, PicesDataBaseImage^ y);

    private:
      SortOrderType  sortOrder;
      bool           reverseSort;
    };  /* PicesDataBaseImageComparer */

  };  /* PicesDataBaseImageList */
};
 