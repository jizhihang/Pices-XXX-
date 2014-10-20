#ifndef  _SIPPERVARIABLES_
#define  _SIPPERVARIABLES_

#include  "GoalKeeper.h"
#include  "Str.h"

/** @namespace  SipperHardware
 *  @brief Contains Classes that are specific to SIPPER's physical characteristics such as Instrument Data.
 *  @details Classes that represent Instruments, SIPPER File Formats, Cruises, Stations, and Deployments are grouped here.
 *           The PICES software installation specifics are also dealt within this namespace.
 */
namespace  SipperHardware  
{
  /**
    *@class  SipperVariables
    *@brief Variables that specify where we can locate SIPPER and PICES directories and files are grouped here.
    *@details  All methods and applications are to use these variables to locate files and directories that they need.
    *         This way all the knowledge or where to locate things is kept in this one class.
    */
  class  SipperVariables
  {
  public:
    typedef  KKU::int32   int32;
    typedef  KKU::uint32  uint32;
    typedef  KKU::int64   int64;
    typedef  KKU::uint64  uint64;

    SipperVariables ();
    ~SipperVariables ();

    static  KKU::KKStrPtr       picesHomeDir;
    static  KKU::GoalKeeperPtr  blocker;

    static  void  SetPicesHomeDir (const KKU::KKStr&  _picesHomeDir);
    static  void  FinalCleanUp ();

    static  bool        AllowUpdates                   ();  /**< 'True' indicates that user is allowed to peofrom updates.                           */
    static  KKU::KKStr  InstrumentDataFilesDir         ();  /**< @brief Directory where Instrument Data files are stored.                            */
    static  KKU::KKStr  InstrumentDataReportDir        ();  /**< @brief Directory where Instrument Data Reports are written to.                      */
    static  KKU::KKStr  PicesHomeDir                   ();  /**< @brief Pices Home Directory.  All other directories will be off this directory.     */
    static  KKU::KKStr  PicesConfigurationDirectory    ();  /**< @brief Where application configuration files go;  NOT training models.              */
    static  KKU::KKStr  PicesCmdLineProgDir            ();  /**< @brief Directory where you can Command Line programs such as "CrossValidation.exe"  */
    static  KKU::KKStr  PicesDataFilesDirectory        ();
    static  KKU::KKStr  PicesTempDirectory             ();
    static  KKU::KKStr  SipperConfigrationFileName     ();
    static  KKU::KKStr  PicesReportDir                 ();
    static  KKU::KKStr  PicesDataFileDirectory         ();
    static  KKU::KKStr  SipperDirectoryListFileName    ();  /**< @brief Specifies the file where we can list directories that containing Sipper Files. */
    static  KKU::KKStr  SipperFilesDefaultDir          ();
    static  KKU::KKStr  SipperInterfaceDir             ();
    static  KKU::KKStr  SipperInterfaceLogDir          ();  /**< @brief Where the SipperInterface program will write its log files to.               */
    static  KKU::KKStr  TrainingModelsConfigurationDir ();  /**< @brief Directory where all Training Model Configuration files are stored.           */
    static  KKU::KKStr  TrainingModelsSaveFilesDir     ();  /**< @brief Directory where all built trained models are stored.                         */
    static  KKU::KKStr  TrainingLibrariesDir           ();  /**< @brief Directory where training library directories will be under                   */
    static  KKU::KKStr  ValidationInfoDir              ();  /**< @brief Directory where Validated Image File Names are stored during image extraction. */



    /**
     *@brief  Expands the supplied string by substituting in Environment variable values.
     *@details  Similar to the 'osSubstituteInEvironmentVariables' method defined in 'osServices' except that there is some special
     *          processing for 'PicesHomeDir'.  If  'PicesHomeDir' is not defined then the default location "C:\\Pices" will be used.
     *          If a environment variable other than "PicesHomeDir" is not defined then the original string will be returned.<br />
     *          ex:  "$(PicesHomeDir)\WFS\Copepod"  will expand to: "c:\Pices\WFS\Copepod".
     *
     *@param[in]  src  String that is to be search for environment variables and expanded.
     *@return  The expanded string.
     */
    static  KKU::KKStr  SubstituteInEvironmentVariables (const KKU::KKStr&  src);

    static  KKU::KKStr  LatitudeToStr  (double latitude);

    static  KKU::KKStr  LongitudeToStr (double longitude);

  private:
    static  void   ReadPermissions ();

    static  int32  LocateEnvStrStart (const KKU::KKStr&  str);
    static  bool   ValidPicesHomeDir (const KKU::KKStr&  dir);

    static  bool   allowUpdates;
    static  bool   picesHomeDirInitialized;
  };
}


#endif

