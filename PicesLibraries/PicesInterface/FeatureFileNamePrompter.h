#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include  "OSservices.h"
using namespace  KKU;

#include  "FeatureFileIO.h"
#include  "FeatureFileIOPices.h"
using namespace  MLL;

#include  "PicesKKStr.h"
#include  "PicesSipperVariables.h"



namespace PicesInterface 
{
	/// <summary>
	/// Summary for FeatureFileNamePrompter
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FeatureFileNamePrompter : public System::Windows::Forms::Form
	{
  private:
    String^            configFileName;
    FeatureFileIOPtr   fileFormat;
    bool               savingAFile;
    System::Windows::Forms::DialogResult result;


	public:
    FeatureFileNamePrompter (bool _savingAFile):
        configFileName   (nullptr),
        fileFormat       (nullptr),
        savingAFile      (_savingAFile)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

  public:
    property  System::Windows::Forms::DialogResult   Result  
    {
      System::Windows::Forms::DialogResult  get ()  {return  result;};
    }

    property  String^  Directory  
    {
      String^  get  ()  {return  DirectoryField->Text;}
      void     set  (String^  _directory)  {DirectoryField->Text = _directory;}
    }

    property  String^  FileName  
    {
      String^  get  ()  {return  FileNameField->Text;}
      void     set  (String^  _fileName)  {FileNameField->Text = _fileName;}
    }

    property  String^  Format
    {
      String^  get  ()  {return  ValidatedFormat ();}
      void     set  (String^ _format)  {FormatField->Text = _format;}
    }


  public:
    property String^  FullFileName
    {
      String^  get ()
      {
        if  (String::IsNullOrEmpty (DirectoryField->Text))
          return  FileNameField->Text;
        else
          return  OSservices::AddSlash (DirectoryField->Text) + FileNameField->Text;
      };
    }


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FeatureFileNamePrompter()
		{
			if (components)
			{
				delete components;
			}
		}



  private: System::Windows::Forms::Label^  DirectoryLabel;

  private: System::Windows::Forms::Button^  BrowseButton;


  private: System::Windows::Forms::Label^  FormatLabel;
  private: System::Windows::Forms::ComboBox^  FormatField;



  private: System::Windows::Forms::Button^  OkButton;
  private: System::Windows::Forms::Button^  CancelButton;
  private: System::Windows::Forms::Label^  FileNameLabel;
  private: System::Windows::Forms::TextBox^  FileNameField;

  private: System::Windows::Forms::ComboBox^  DirectoryField;



  protected: 

  protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
      this->DirectoryLabel = (gcnew System::Windows::Forms::Label());
      this->BrowseButton = (gcnew System::Windows::Forms::Button());
      this->FormatLabel = (gcnew System::Windows::Forms::Label());
      this->FormatField = (gcnew System::Windows::Forms::ComboBox());
      this->OkButton = (gcnew System::Windows::Forms::Button());
      this->CancelButton = (gcnew System::Windows::Forms::Button());
      this->FileNameLabel = (gcnew System::Windows::Forms::Label());
      this->FileNameField = (gcnew System::Windows::Forms::TextBox());
      this->DirectoryField = (gcnew System::Windows::Forms::ComboBox());
      this->SuspendLayout();
      // 
      // DirectoryLabel
      // 
      this->DirectoryLabel->AutoSize = true;
      this->DirectoryLabel->Location = System::Drawing::Point(12, 14);
      this->DirectoryLabel->Name = L"DirectoryLabel";
      this->DirectoryLabel->Size = System::Drawing::Size(49, 13);
      this->DirectoryLabel->TabIndex = 0;
      this->DirectoryLabel->Text = L"Directory";
      // 
      // BrowseButton
      // 
      this->BrowseButton->Location = System::Drawing::Point(559, 9);
      this->BrowseButton->Name = L"BrowseButton";
      this->BrowseButton->Size = System::Drawing::Size(53, 23);
      this->BrowseButton->TabIndex = 2;
      this->BrowseButton->Text = L"Browse";
      this->BrowseButton->UseVisualStyleBackColor = true;
      this->BrowseButton->Click += gcnew System::EventHandler(this, &FeatureFileNamePrompter::BrowseButton_Click);
      // 
      // FormatLabel
      // 
      this->FormatLabel->AutoSize = true;
      this->FormatLabel->Location = System::Drawing::Point(12, 82);
      this->FormatLabel->Name = L"FormatLabel";
      this->FormatLabel->Size = System::Drawing::Size(39, 13);
      this->FormatLabel->TabIndex = 3;
      this->FormatLabel->Text = L"Format";
      // 
      // FormatField
      // 
      this->FormatField->FormattingEnabled = true;
      this->FormatField->Location = System::Drawing::Point(66, 77);
      this->FormatField->Name = L"FormatField";
      this->FormatField->Size = System::Drawing::Size(305, 21);
      this->FormatField->TabIndex = 4;
      // 
      // OkButton
      // 
      this->OkButton->Location = System::Drawing::Point(559, 43);
      this->OkButton->Name = L"OkButton";
      this->OkButton->Size = System::Drawing::Size(53, 23);
      this->OkButton->TabIndex = 5;
      this->OkButton->Text = L"OK";
      this->OkButton->UseVisualStyleBackColor = true;
      this->OkButton->Click += gcnew System::EventHandler(this, &FeatureFileNamePrompter::OkButton_Click);
      // 
      // CancelButton
      // 
      this->CancelButton->Location = System::Drawing::Point(559, 77);
      this->CancelButton->Name = L"CancelButton";
      this->CancelButton->Size = System::Drawing::Size(53, 23);
      this->CancelButton->TabIndex = 6;
      this->CancelButton->Text = L"Cancel";
      this->CancelButton->UseVisualStyleBackColor = true;
      this->CancelButton->Click += gcnew System::EventHandler(this, &FeatureFileNamePrompter::CancelButton_Click);
      // 
      // FileNameLabel
      // 
      this->FileNameLabel->AutoSize = true;
      this->FileNameLabel->Location = System::Drawing::Point(12, 48);
      this->FileNameLabel->Name = L"FileNameLabel";
      this->FileNameLabel->Size = System::Drawing::Size(54, 13);
      this->FileNameLabel->TabIndex = 7;
      this->FileNameLabel->Text = L"File Name";
      // 
      // FileNameField
      // 
      this->FileNameField->Location = System::Drawing::Point(66, 44);
      this->FileNameField->Name = L"FileNameField";
      this->FileNameField->Size = System::Drawing::Size(304, 20);
      this->FileNameField->TabIndex = 8;
      // 
      // DirectoryField
      // 
      this->DirectoryField->FormattingEnabled = true;
      this->DirectoryField->Location = System::Drawing::Point(66, 10);
      this->DirectoryField->Name = L"DirectoryField";
      this->DirectoryField->Size = System::Drawing::Size(480, 21);
      this->DirectoryField->TabIndex = 9;
      // 
      // FeatureFileNamePrompter
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(628, 111);
      this->Controls->Add(this->DirectoryField);
      this->Controls->Add(this->FileNameField);
      this->Controls->Add(this->FileNameLabel);
      this->Controls->Add(this->CancelButton);
      this->Controls->Add(this->OkButton);
      this->Controls->Add(this->FormatField);
      this->Controls->Add(this->FormatLabel);
      this->Controls->Add(this->BrowseButton);
      this->Controls->Add(this->DirectoryLabel);
      this->Name = L"FeatureFileNamePrompter";
      this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
      this->Text = L"Feature File Name Prompter";
      this->Load += gcnew System::EventHandler(this, &FeatureFileNamePrompter::FeatureFileNamePrompter_Load);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private: System::Void FeatureFileNamePrompter_Load(System::Object^  sender, System::EventArgs^  e) 
           {
             SetUpVariables ();
           }



    void  SetUpVariables ()
    {
      DirectoryField->Items->Clear ();
      
      FormatField->Items->Clear ();
      VectorKKStr  driverNames = FeatureFileIO::RegisteredDriverNames (!savingAFile,  //  Specifies wheather the driver must be able to read.
                                                                       savingAFile    //  Specifies wheather the drive mustbe able to write.
                                                                      );
      VectorKKStr::iterator  idx;
      for  (idx = driverNames.begin ();  idx != driverNames.end ();  idx++)
        FormatField->Items->Add (PicesKKStr::KKStrToSystenStr (*idx));


      configFileName = OSservices::AddSlash (PicesSipperVariables::PicesConfigurationDirectory ()) + "FeatureFileNamePrompter.cfg";
      ConfigurationLoad ();
    }



    void  ConfigurationLoad ()
    {
      DirectoryField->Items->Clear ();
      fileFormat = FeatureFileIOPices::Driver ();

      System::IO::StreamReader^  i = nullptr;
      try  {i = gcnew System::IO::StreamReader (configFileName);}  catch (Exception^) {i = nullptr;}
      if  (i == nullptr)
        return;

      String^  nextLine = nullptr;

      while  (true)
      {
        try  {nextLine = i->ReadLine ();}  catch (Exception^) {break;}
        if  (nextLine == nullptr)
          break;

        nextLine = nextLine->Trim ();
        
        if  ((nextLine->Length < 3)  ||  (nextLine->Substring (0, 2) == "//"))
          continue;

        array<String^>^ fields = nextLine->Split ('\t');
        if  (fields->Length != 2)
          continue;

        String^  fieldName  = fields[0];
        String^  fieldValue = fields[1];

        if  (fieldName == "DirectoryHistory")
          DirectoryField->Items->Add (fieldValue);

        else if  (fieldName == "LastFileFormat")
        {
          FeatureFileIOPtr  driver = FeatureFileIO::FileFormatFromStr (PicesKKStr::SystemStringToKKStr (fieldValue));
          if  (driver)
            FormatField->Text = PicesKKStr::KKStrToSystenStr (driver->DriverName ());
          else
            FormatField->Text = "";
        }
      }
      i->Close ();


      if  (String::IsNullOrEmpty (FormatField->Text))
        FormatField->Text = PicesKKStr::KKStrToSystenStr (FeatureFileIOPices::Driver ()->DriverName ());
    }  /* ConfigurationLoad */





    private: void  ConfigurationSave ()
    {
      System::IO::StreamWriter^  o = nullptr;
      try  {o = gcnew System::IO::StreamWriter (configFileName);}  catch (Exception^) {o = nullptr;}
      if  (o == nullptr)
        return;

      System::DateTime  curTime = System::DateTime::Now;

      o->WriteLine ("//Configuration File   for   FeatureFileNamePrompter");
      o->WriteLine ("//");
      o->WriteLine ("//    Date Written[" + curTime.ToString ("G"));
      o->WriteLine ("");

      for  each  (String^  histLine in DirectoryField->Items)
        o->WriteLine ("DirectoryHistory"    "\t"  + histLine);

      o->WriteLine ("LastFileFormat"        "\t"  + FormatField->Text);


      o->Close ();
    }  /* ConfigurationSave */



    private:  String^  ValidatedFormat ()
              {
                if  (String::IsNullOrEmpty (FormatField->Text))
                  return "";
                FeatureFileIOPtr  driver = FeatureFileIO::FileFormatFromStr (PicesKKStr::SystemStringToKKStr (FormatField->Text));
                if  (driver == NULL)
                  return "";

                return  PicesKKStr::KKStrToSystenStr (driver->DriverName ());
              }


    private:  bool  ValidateScreen ()
              {
                return  true;
              }


    private:   void  AddToHistory  (String^  s)
               {
                 if  (DirectoryField->Items->Count > 0)
                   if  (DirectoryField->Items[DirectoryField->Items->Count - 1] == s)
                     return;

                 DirectoryField->Items->Add (s);
               }  /* AddToHistory */



  private: System::Void BrowseButton_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             if  (savingAFile)
             {
               System::Windows::Forms::FolderBrowserDialog^  dialog = gcnew System::Windows::Forms::FolderBrowserDialog ();
               dialog->SelectedPath  = DirectoryField->Text;
               dialog->Description   = "Select Directory";
               System::Windows::Forms::DialogResult result = dialog->ShowDialog();
               if  (result == System::Windows::Forms::DialogResult::OK) 
               {
                 DirectoryField->Text = dialog->SelectedPath;
                 AddToHistory (DirectoryField->Text);
               }
             }
             else
             {
               System::Windows::Forms::OpenFileDialog^  dialog = gcnew System::Windows::Forms::OpenFileDialog ();
               dialog->CheckPathExists = true;
               dialog->CheckFileExists = true;
               dialog->FileName = FileNameField->Text;
               dialog->InitialDirectory = DirectoryField->Text;
               dialog->Title = "Feature File Select";

               System::Windows::Forms::DialogResult result = dialog->ShowDialog();
               if  (result == System::Windows::Forms::DialogResult::OK) 
               {
                 String^  dirPathSpecified  = dialog->InitialDirectory;
                 String^  fileNameSpecified = dialog->FileName;

                 String^  pathPartOfFileName  = OSservices::GetPathPartOfFile (fileNameSpecified);
                 String^  rootFileNameWithExt = OSservices::GetRootNameWithExtension (fileNameSpecified);

                 if  (String::IsNullOrEmpty (dirPathSpecified))
                 {
                   DirectoryField->Text = pathPartOfFileName;
                   FileNameField->Text  = rootFileNameWithExt;
                 }
                 else
                 {
                   if  (String::IsNullOrEmpty (pathPartOfFileName))
                   {
                     DirectoryField->Text = dirPathSpecified;
                     FileNameField->Text  = fileNameSpecified;
                   }
                   else
                   {
                     DirectoryField->Text = OSservices::AddSlash (dirPathSpecified) + pathPartOfFileName;
                     FileNameField->Text  = rootFileNameWithExt;
                   }
                 }

                 AddToHistory (DirectoryField->Text);
               }
             }
           }


  private: System::Void OkButton_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             if  (!ValidateScreen ())
               return;

             result = System::Windows::Forms::DialogResult::OK;
             Close ();
           }


private: System::Void CancelButton_Click(System::Object^  sender, System::EventArgs^  e) 
         {
           result = System::Windows::Forms::DialogResult::Cancel;
           Close ();
         }
  };  /* FeatureFileNamePrompter */
}
