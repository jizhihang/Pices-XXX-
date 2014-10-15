#ifndef  _ORDERINGS_
#define  _ORDERINGS_


/**
 *@class  MLL::Orderings
 *@author  Kurt Kramer
 *@brief  Used to maintain multiple orderings of a single list of FeatureVector objects.
 *@details
 * Will maintain multiple orderings of a single FeatureVectorList.  These orderings will be saved in 
 * a text file for recall later. This will allow the user to beable to repeat experiments using the 
 * same ordering of data again. The Idea is that the 1st time this orderings is created the order 
 * will be randomly driven with Stratification by Class.  This ordering will then be saved in data 
 * Files for future recall.  More than one order can be maintained for a single list. An example of 
 * a good use of this is RandomSplits. 
 */



#include  "FileDesc.h"
#include  "RunLog.h"
#include  "Str.h"


namespace  MLL  {


#ifndef  _FeatureVectorDefined_
class  FeatureVector;
typedef  FeatureVector*  FeatureVectorPtr;
#endif


#ifndef  _FeatureVectorListDefined_
class  FeatureVectorList;
typedef  FeatureVectorList*  FeatureVectorListPtr;
#endif


#ifndef  _MLCLASS_
class  MLClass;
typedef  MLClass*  MLClassPtr;
typedef  MLClass const  MLClassConst;
typedef  MLClassConst*  MLClassConstPtr;
class  MLClassList;
typedef  MLClassList*  MLClassListPtr;
#endif


#if  !defined(_MLClassConstListDefined_)
  class  MLClassConstList;
  typedef  MLClassConstList*  MLClassConstListPtr;
#endif




#ifndef  _FILEDESC_
class  FileDesc;
typedef  FileDesc*  FileDescPtr;
class  FileDescList;
typedef  FileDescList*  FileDescListPtr;
#endif



class  Orderings
{
public:

  typedef  Orderings*  OrderingsPtr;

  /**
   *@brief Constructs Orderings object from ImageFeatursList object.
   *@details Use this when an existing list does not exist.  Will create 'numOfOrderings' 
   *         seperate lists of 'data' that are randomly ordered and stratified by 
   *         'numOfFolds'.
   *
   *@param[in] _data  ImagFeaturesList object, 
   *@param[in] _numOfOrderings  Number of seperate orderings of data neeed.
   *@param[in] _numOfFolds  Used to help stratify data in each fold.
   */
  Orderings (const FeatureVectorListPtr _data,
             uint32                     _numOfOrderings,
             uint32                     _numOfFolds
            );


  /**
   *@brief Constructs Orderings of a FeatureVectorList from a previous construction that was saved
   *       in a data file.
   *@details Will load object from the Feature File '_featureFileName'  and retrieve the 
   *         different orderings from a separate index file whos name will be 
   *         osExtention (FeatureFileName) + ".idx".  The load routine will validate that all 
   *         imageFeatures objects are accounted for in each ordering.  If the index file does not 
   *         exist it then the  'successful' flag will bet to false.  
   *
   *@param[in] _featureFileName  File to load FeatureVector' objects from.  This will be used as 
   *                             master list for 'Orderings'.
   *@param[in] _driver           Feature File driver toutilize.
   *@param[in] _log              Log file to write messages to.
   *@param[in] v                 If flag tturns to 'TRUE' then will terminate the load process and return to caller.
   */
  Orderings (const KKStr&      _featureFileName,
             FeatureFileIOPtr  _driver,
             RunLog&           _log,
             bool&             cancelFlag
            );


  /**
   *@brief Constructs a Orderings object from a FeatureLectorList object.  
   *@details  Will use 'data' as master list of FeatureVector objects.The orderings will be loaded 
   *          from 'indexFileName'.  It is expected that the size of the files will match.  The 
   *          load routine will validate that all FeatureVector objects are accounted for in each 
   *          ordering.
   *
   *@param[in] _data  Master List of Imagefeatures  objects.
   *@param[in] _indexFileName  File where orderings of 'data' are to be loaded from.
   *@param[in] _numOfOrderings
   *@param[in] _numOfFolds
   */
  Orderings (const FeatureVectorListPtr  _data,
             const KKStr&                _indexFileName,
             uint32                      _numOfOrderings,
             uint32                      _numOfFolds
            );


  /**
   *@brief   Constructs a Orderings objects for a specified FeatureVectorList using a previously 
   *         built Orderings data index file.
   *@details Will use FileName from "data" parameter to derive both 'featureFileName' and 
   *        'indexFileName' using the 'FileName' method from FeatureVectorList.  It is expected 
   *        that a seperate index file by the name osDeletExtention (FeatureFileName) + ".idx" 
   *        will exist.  The orderings will be loaded from that file.
   *@param[in]  _data  FeatureVectorList that we want different orderings of.
   */
  Orderings (FeatureVectorListPtr  _data);

  ~Orderings ();



  /**
   *@brief   Constructs a Orderings object for a specified FeatureVectorList.
   *@details Will use FileName from "_data" parameter to derive both 'featureFileName' and 
   *        'indexFileName' using the 'FileName' method from FeatureVectorList.  If a separate 
   *        Index file does not exist it will randomly xcreate orderings and save the orderings 
   *        in a new Index file.
   *@param[in]  _data  FeatureVectorList that we want different orderings of.
   *@param[in]  _numOfOrderings  Expected number of orderings.
   *@param[in]  _numOfFolds  Number of folds each ordering should be straitified by.
   */
  static
  OrderingsPtr  CreateOrderingsObjFromFileIfAvaliable (const FeatureVectorListPtr  _data,
                                                       uint32                      _numOfOrderings,
                                                       uint32                      _numOfFolds
                                                      );


  /***************************************************************************/
  /*                            Access Methods                               */
  /***************************************************************************/
  FeatureVectorListPtr          Data            ()  const  {return data;}
  const KKStr&                  FeatureFileName ()  const  {return featureFileName;}
  const FileDescPtr             FileDesc        ()  const  {return fileDesc;}
  const MLClassConstListPtr  MLClasses    ()  const  {return mlClasses;}
  const KKStr&                  IndexFileName   ()  const  {return indexFileName;}
  RunLog&                       Log             ()         {return log;}
  uint32                        NumOfFolds      ()  const  {return numOfFolds;}
  uint32                        NumOfOrderings  ()  const  {return numOfOrderings;}
  uint32                        Size            ()  const  {return (uint32)orderings.size ();}
  bool                          Valid           ()  const  {return valid;}




  const
  FeatureVectorListPtr  Ordering (uint32  orderingIdx)  const;




  void  Save ();
 
  void  Save (const KKStr&  _indexFileName);


private:
  void  CreateOrderings ();
  void  DeleteOrderings ();
  void  Load ();

  void  Load (const KKStr&  _indexFileName,
              bool&          successful
             );


  FeatureVectorListPtr           data;
  KKStr                          featureFileName;
  FileDescPtr                    fileDesc;
  MLClassConstListPtr         mlClasses;
  KKStr                          indexFileName;
  RunLog&                        log;
  uint32                         numOfFolds;
  uint32                         numOfOrderings;
  vector<FeatureVectorListPtr>   orderings;
  bool                           valid;
};


typedef  Orderings::OrderingsPtr  OrderingsPtr;

}  /* namespace  MLL  */

#endif
