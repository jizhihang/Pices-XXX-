#pragma once


namespace PicesInterface 
{
  public  ref class PicesFeatureFileIO
  {
  public:
    PicesFeatureFileIO (String^  driverName);

  private:
    ~PicesFeatureFileIO ();

  protected:
    !PicesFeatureFileIO ();

  public:
    static  array<String^>^  RegisteredDriverNames (bool  canRead,    /**< if True driver must be able to Read a Feature File. */
                                                    bool  canWrite    /**< if True driver must be able to Wrire a Feature File. */
                                                   );

    static  bool             ValidDriverName (String^ _driverName);

    property  bool     CancelLoad         {bool     get ();   void set (bool _cancelLoad); }
    property  bool     CanRead            {bool     get ();}
    property  bool     CanWrite           {bool     get ();}
    property  bool     ChangesMade        {bool     get ();}
    property  String^  DriverName         {String^  get ();} 
    property  uint     NumExamplesWritten {uint     get ();}
    property  bool     Successful         {bool     get ();}
    property  bool     Valid              {bool     get ();}


    void  SaveFeatureFile (String^                  _fileName, 
                           PicesFeatureVectorList^  _examples,
                           PicesRunLog^             _runLog
                          );

    void  SaveFeatureFileNormalized (String^                  _fileName, 
                                     PicesFeatureVectorList^  _examples,
                                     PicesRunLog^             _runLog
                                    );

    PicesFeatureVectorList^   LoadFeatureFile (String^          _fileName, 
                                               PicesClassList^  _classes, 
                                               PicesRunLog^     _runLog
                                              );


  private:
    bool*             cancelLoad;
    bool*             changesMade;
    FeatureFileIOPtr  driver;
    String^           lastError;
    uint*             numExamplesWritten;
    bool*             successful;
  };

}  /* PicesInterface */
