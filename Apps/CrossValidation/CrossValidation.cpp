#include  "FirstIncludes.h"

// Kurt Kramer  2012-03-01  
// Added a Second Best Guess Confusion Matrix.  That is a confusion matrix that only reports the
// results using the second best class as the predicted class when the first one wss wrong.
 
#include <string>
#include <ctype.h>
#include <stdio.h> 
#include <time.h>

#ifndef  WIN32
#include  <unistd.h>
#endif

#include <iomanip>
#include <iostream>
#include <fstream>
//#include  <ostream>
#include <iostream>
#include <map>
#include <vector>

#include  "MemoryDebug.h"


#ifdef WIN32
#include  <windows.h>
#else
#endif

using namespace  std;


#include "BasicTypes.h"
#include "StatisticalFunctions.h"
using namespace  KKU;

#include "Instrument.h"
#include "InstrumentDataFileManager.h"
#include "InstrumentDataManager.h"
#include "SipperBuff.h"
using namespace SipperHardware;

#include "BinaryClassParms.h"
#include "Classifier2.h"
#include "CmdLineExpander.h"
#include "CrossValidation.h"
#include "CrossValidationVoting.h"
#include "CrossValidationThreeWay.h"
#include "CrossValidationMxN.h"
#include "DuplicateImages.h"
#include "FeatureFileIO.h"
#include "FeatureFileIOC45.h"
#include "FeatureFileIOPices.h"
#include "FeatureNumList.h"
#include "Orderings.h"
#include "OSservices.h"
#include "SipperCruise.h"
#include "SipperStation.h"
#include "SipperDeployment.h"

#include "TrainingProcess2.h"
using namespace  MLL;



#include  "CrossValidationApp.h"


// 2005-July-13
// K:\v1\Adult
// -c Adult.cfg  -df adult.data  -Report Adult_cv_Report.txt -format c45  -Validation adult.test

// 2005-July-20
// -c test.cfg -df test.data  -format c45  -report TestReport.txt

// -c covtype.cfg  -df covtype.data  -Report covtype_Report.txt  -Validation covtype.test -Format c45

// -c Thesis2Classes.cfg -df TwoClasses_TestData.data -Report TwoClassResults.txt

// -c Thesis9Classes.cfg -df NineClasses_TestData.data -Report  TestRandomSelection.txt  -RandomSets

// -c C:\TrainingApp\DataFiles\TrainingModels\ThirtyFourClasses_Prob.cfg -MissClassed c:\temp\MissClassed  -Report ThirtyFourClasses_Prob.txt


// -Dir C:\TrainingApp\TestTrainingLibarry  -c C:\TrainingApp\DataFiles\TrainingModels\FiveBigOnes.cfg

// -c Spain_Tunned.cfg  -R  Spain_Report.txt -DataFile SpainTraining.data


// -c Trich_Protist_All.cfg  -DataFile Trich_Protist_All.data  -RandomSets 10


// K:\Plankton\Papers\BitReduction\WEB_Data
// -C anonymous-msweb.cfg -DF anonymous-msweb_Data-C45.data -format C45  -Validation anonymous-msweb_Test-C45.data  -Report anonymous-msweb.txt

// C:\users\kkramer\GradSchool\Plankton\Papers\BitReduction\WEB_Data
// -C anonymous-msweb.cfg -DF anonymous-msweb_Data-C45.data -format C45  -Validation anonymous-msweb_Test-C45.data  -Report anonymous-msweb-Using-Sparse-Matrix.txt -TESTBITREDUCTIONOPTIONS


// -c Thesis9Classes.cfg -df NineClasses_TestData.data -Report  BinaryFeatureSelection.txt  -TestBitReduction

// K:\Plankton\Papers\BitReduction\Phoneme
// -C phoneme.cfg -DF phoneme.data -format C45  -Report phoneme_218-PC_Jan-16.txt -TESTBITREDUCTIONOPTIONS
// -C phoneme.cfg -DF phoneme.data -format C45  -Report phoneme_G-1024.txt -TESTBITREDUCTIONOPTIONS
// -C phoneme.cfg -DF phoneme_sparse.data -format sparse  -Report phoneme_sparse.txt

// 2006-Jan-19      For Bit reduction Paper
// K:\Plankton\Papers\BitReduction\DataSets\ForestCover
// -C covtype.cfg -DF CovType_TwoClass.data -Validation CovType_TwoClass.test -format C45  -Report CovType_TwoClass.txt -TESTBITREDUCTIONOPTIONS

// K:\Plankton\Papers\BitReduction\DataSets\Adult
// -C adult.cfg -DF adult.data -Validation adult.test -format C45  -Report adult_218pc.txt 

// 2006-01-25   Reslts for plankton meating today.
// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures\RandomSearch
// -c Thesis9Classes.cfg -df NineClasses_TestData.data -Report  Bes35Features.txt


// 2006-02-08   Bit reduction   Shuttle paper
// K:\Plankton\Papers\BitReduction\DataSets\Shuttle
// -C Shuttle.cfg  -DF Shuttle_Train_c45.data   -format c45 -Validation Shuttle_Test_c45.data  -Report Shuttle_218pc_Linear.txt   -TESTBITREDUCTIONOPTIONS Shuttle


// 2006-02-08
// K:\Plankton\Papers\BitReduction\DataSets\Pendigits
// C:\users\kkramer\GradSchool\Plankton\Papers\BitReduction\DataSets\Pendigits
// -C pendigits.cfg  -DF pendigits_train_sparse.txt   -format sparse -Validation pendigits_test_sparse.txt  -Report pendigits_218pc_Linear.txt   -TESTBITREDUCTIONOPTIONS PenDigits

// 2006-02-09
// K:\Plankton\Papers\BitReduction\DataSets\WEB_Data
// -C anonymous-msweb.cfg -DF anonymous-msweb_Data-C45.data -format C45  -Validation anonymous-msweb_Test-C45.data  -Report anonymous-msweb_218pc_Linear.txt  -TESTBITREDUCTIONOPTIONS WEB


// 2006-02-10
// K:\Plankton\Papers\BitReduction\DataSets\Banana
// C:\users\kkramer\GradSchool\Plankton\Papers\BitReduction\DataSets\Banana
// -C banana.cfg  -DF banana.all.txt -format sparse  -Report banana_218pc_RadomSplits.txt   -TESTBITREDUCTIONOPTIONS  banana
// -C banana.cfg  -DF banana.all.txt -format sparse  -Report banana_KurtsLapTop.txt   -TESTBITREDUCTIONOPTIONS  banana2
// -C banana.cfg  -DF banana.all_80p.data -validation banana.all_20p.data  -format sparse  -Report banana_218pc_80-20.txt   -TESTBITREDUCTIONOPTIONS  banana2

// -C banana.cfg  -DF banana.all_80p.data -validation banana.all_20p.data  -format sparse  -Report Banana_Test_Duplicate.txt   -TESTBITREDUCTIONOPTIONS  banana2


// K:\Plankton\Papers\BitReduction\DataSets\WaveForm
// -c WaveForm.cfg  -df waveform-+noise_80p.data -format C45 -Validation waveform-+noise_20p.data -Report WaveForm_80-20_218PC.txt  -TESTBITREDUCTIONOPTIONS  Wave

// c:\temp
// -C FiveBigest.cfg -df ActiveLearning_MasterTestImages_c45.data  -format C45 

// K:\Plankton\Papers\BitReduction\DataSets\Plankton
// -c Plankton.cfg  -df ActiveLearning_MasterTestImages.data -format Raw -Validation ActiveLearning_ValidationImages.data -Report Plankton_218PC.txt  -TESTBITREDUCTIONOPTIONS  Plankton
// -c Plankton.cfg  -df PlanctionTrain_17.data -format Raw -Validation PlanctionTest_17.data -Report Plankton_218PC.txt  -TESTBITREDUCTIONOPTIONS  Plankton

// K:\Plankton\Papers\BitReduction\DataSets\SatImage
// -c Sat.cfg  -df sat.trn  -Validation sat.tst  -Format  uci  -TESTBITREDUCTIONOPTIONS  SatImage -Report Sat_218pc.txt


// K:\Plankton\Papers\BitReduction\DataSets\Adult
// -c adult.cfg  -df adult.data  -Validation adult.test  -format c45 -TESTBITREDUCTIONOPTIONS adult -Report Adult_218pc.txt


// 2006-02-17
// K:\Plankton\Papers\BitReduction\DataSets\ForestCover
// -C covtype.cfg -DF CovType_TwoClass.data -Validation CovType_TwoClass.test -format C45  -Report Forest_218pc.txt -TESTBITREDUCTIONOPTIONS Forest
// -C covtype.cfg -DF CovType_TwoClass.data -Validation CovType_TwoClass.test -format C45  -Report Forest.txt

// 2006-02-19
// K:\Plankton\Papers\BitReduction\DataSets\Page
// -C ad.cfg -DF ad_80p.data -Validation ad_20p.data -format C45  -Report Page_218PC.txt  -TESTBITREDUCTIONOPTIONS Page

// K:\Plankton\Papers\BitReduction\DataSets\Letter
// -C Letter.cfg -DF letter-c45_80p.data  -Validation letter-c45_20p.data -format C45  -Report Letter_218PC.txt  -TESTBITREDUCTIONOPTIONS Letter

// K:\Plankton\Papers\BitReduction\DataSets\Phoneme
// -C phoneme.cfg -DF phoneme_80p.data  -Validation phoneme_20p.data -format C45  -Report PhomeMe_218PC.txt  -TESTBITREDUCTIONOPTIONS Phone

// 2006-02-21
// K:\Plankton\Papers\BitReduction\DataSets\WEB_Data
// -C anonymous-msweb.cfg -DF anonymous-msweb_Data-C45.data -format C45  -Validation anonymous-msweb_Test-C45.data  -Report anonymous-msweb_218pc_g-067.txt  -TESTBITREDUCTIONOPTIONS WEB

// 2006-02-22
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\ForestCover
// -C covtype.cfg -DF CovType_TwoClass.data -Validation CovType_TwoClass.test -format C45  -Report Forest_Binary_g-0.374075_218pc.txt -TESTBITREDUCTIONOPTIONS Forest

//  2006-02-23
// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Page-Blocks
// -C page-blocks.cfg -DF page-blocks_80p.data -Validation page-blocks_20p.data -format C45  -Report Page-Blocks_218pc.txt -TESTBITREDUCTIONOPTIONS Page
                                                    

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Adult
// -c adult.cfg  -df adult.data  -Validation adult.test  -format c45 -TESTBITREDUCTIONOPTIONS adult -Report Adult_NoEncoding_218pc.txt


// K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements
// C:\users\kkramer\GradSchool\Plankton\ActiveLearning\Results\2004_Sep_Expirements
// -C FiveBigest.cfg -df ActiveLearning_ValidationImages.data  -Validation ActiveLearning_MasterTestImages.data  -format Raw -folds 10 -Report  TestReport.txt


// 2006-04-29
// C:\users\kkramer\GradSchool\Plankton\ActiveLearning\Results\2004_Sep_Expirements
// -c FiveBigest.cfg  -DF ActiveLearning_ValidationImages.data  -folds 10 -Report TestUnBalancedBitReduction.txt

// K:\v1\Thesis\TestSet9Classes\1000_ImagesPerClass\ActiveLearning
// -c Thesis9Classes.cfg  -df NineClasses_shuffled.data  -Folds 10   -Probability 

// C:\bigpine\Plankton\Papers\BitReduction\DataSets\Plankton
// -C Plankton.cfg -df PlanctionTrain_17.data  -Validation PlanctionTest_17.data  -format Raw -folds 10 -Report  TestReport.txt

// F:\Users\kkramer\GradSchool\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures
// -c Thesis9Classes.cfg  -df NineClasses_ValidationData.data  -folds 10 -Report TestReport.txt

// -DataFile C:\users\LI\Data\colon.train.scale -Format Sparse -Config Li.cfg  



// K:\v1\Thesis\TestSet9Classes\300_ImagesPerClass\Using5OldContourFeatures
// -c Thesis9Classes.cfg  -df NineClasses_TestData.data  -Validation NineClasses_ValidationData.data  -format raw  -folds 10 -report  Test.txt


// C:\bigpine\Plankton\Papers\BitReduction\DataSets\WEB_Data
// -c anonymous-msweb.cfg  -df anonymous-msweb_Data-C45.data  -Validation anonymous-msweb_Test-C45.data  -format C45  -report  BoostSamplingTest.txt

// -c Thesis9Classes.cfg -df NineClasses_TestData.data -Validation NineClasses_ValidationData.data  -format raw 

// -c Plankton_17.cfg  -df PlanktonTrain_17.data  -Format raw  -report Plankton_17.txt

// C:\bigpine\LI\Data
// -c C:\users\LI\Data\Li.cfg  -df colon.train_TestData.data  -Validation colon.train_ValidationData.data  -Format sparse  -report test.txt

// -c C:\users\LI\Data\Li.cfg  -DataFile C:\users\LI\Data\colon.train.scale -Format Sparse -report  C:\users\LI\Data\colon.train_10Fole.txt


// -c etpB_training.cfg  -Report WithOutInstrumentData.txt

// -c C:\temp\FeatureSearch_BinaryClasses\etpB_TunedParameters.cfg  -df C:\temp\FeatureSearch_BinaryClasses\etpB_training_shuffled.data  -folds 10 -Report OnVsOneSelection.txt


// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\NineClassesAll
// -c Plankton9Classes_Tuned.cfg  -df NineClasses_TrainData_c45.data  -Validation NineClasses_ValidationData_c45.data  -format c45  -report  NineClasses_AllClasses.txt

// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\NineClassesBinary
// -c Plankton9Classes_Tuned.cfg  -df NineClasses_TrainData_c45.data  -Validation NineClasses_ValidationData_c45.data  -format c45  -report  NineClasses_BinaryClasses_Voting.txt


// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\NineClassesBinary
//-c Plankton9Classes_Tuned_Tuned.cfg   -df NineClasses_TrainData_c45.data  -Validation NineClasses_ValidationData_c45.data  -format c45  -report  NineClasses_BinaryClasses_ParmsTuned_Voting.txt
//-c Plankton9Classes_Tuned_Tuned.cfg   -df NineClasses_TrainData_c45.data  -Validation NineClasses_ValidationData_c45.data  -format c45  -report  NineClasses_BinaryClasses_ParmsTuned_Probability.txt


// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\Letter\AllClasses
// -c Letter_Tuned.cfg  -df letter-TrainAndTest.data  -Validation letter-Validation.data   -format c45  -report  Letter_AllClass-ParmsTuned-AllFeatures.txt

// -c Letter_Tuned-FeaturesSelected.cfg  -df letter-TrainAndTest.data  -Validation letter-Validation.data   -format c45  -report  Letter_Tuned-FeaturesSelected.txt

// -c Letter_Binary-FeatureSelected.cfg  -df letter-TrainAndTest.data  -Validation letter-Validation.data   -format c45  -report  Letter_Binary-FeatureSelected-Voting.txt

// -c Letter_Binary-FeatureSelected-Tuned.cfg  -df letter-TrainAndTest.data  -Validation letter-Validation.data   -format c45  -report  Letter_Binary-FeatureSelected-Tuned-Voting.txt
// -c Letter_Binary-FeatureSelected-Tuned.cfg  -df letter-TrainAndTest.data  -Validation letter-Validation.data   -format c45  -report  Letter_Binary-FeatureSelected-Tuned-Probability.txt

// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\NineClasses\AllClasses
// -c Plankton9Classes_ParmsTunedAndFeaturesSelected.cfg  -df  NineClasses_TrainTestData_c45.data -validation NineClasses_ValidationData_c45.data  -format c45  -report Plankton9Classes_ParmsTunedAndFeaturesSelected.txt

// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\NineClasses\BinaryClasses
// -c Plankton9Classes_Binary-FeatureSelected-Tuned.cfg -df  NineClasses_TrainTestData_c45.data -validation NineClasses_ValidationData_c45.data  -format c45  -report Plankton9Classes_Binary-FeatureSelected-Tuned_Voting.txt


// C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\ForestCover
// -c covtype_AllClasses-Tuned.cfg                      -df  CovType-TrainTest.data -validation CovType-Validation.data  -format c45  -report covtype_AllClasses-Tuned_Probability.cfg
// -c covtype_AllClasses-Tuned-FeaturesSelected.cfg     -df  CovType-TrainTest.data -validation CovType-Validation.data  -format c45  -report covtype_AllClasses-Tuned-FeaturesSelected_Probability.txt
// -c covtype_AllClasses-Tuned-FeaturesSelected.cfg     -df  CovType-TrainTest.data -validation CovType-Validation.data  -format c45  -report covtype_AllClasses-Tuned-FeaturesSelected_Probability.txt
// -c covtype-Tuned-BinaryClasses-FeaturesSelected.cfg  -df  CovType-TrainTest.data -validation CovType-Validation.data  -format c45  -report covtype-Tuned-BinaryClasses-FeaturesSelected_Voting.txt

// -c SimpleModel.cfg  -folds 10 -Report  TestNewSVM

// -dir D:\TrainingApp\etpC_training  -Hierarchy -Report  Hierarchy.txt


// D:\Users\kkramer\GradSchool\Plankton\Papers\BitReduction\DataSets\SatImage
// -DataFile sat.trn  -Validation sat.tst  -C sat.c3fg  -format UCI  -Report Validation_ConfusionMatrix.txt

// -dir C:\PICES\etpC_training -folds 2 -report c:\temp\NewRootDir.txt

// -DataFile  C:\Temp\Viky\svm_d7_30.data  -validation  C:\Temp\Viky\svm_d7_test_30.data  -c C:\Temp\Viky\Viky.cfg  -r C:\Temp\Viky\svm_d7_Report.txt


// -DataFile C:\Temp\MegaLibrary_2009-09-17.data  -c megalibrary_1.cfg  -fold 10  -r c:\Temp\Megalibrary_10Folds_Weighted.txt

// C:\Users\kkramer\SipperProject\Papers\BinaryFeatureSelection\Experiments\SatImage
// C:\Users\kkramer\GradSchool\Papers\BinaryFeatureSelection\Experiments\SatImage
// -DataFile sat.tst  -c SatNew.cfg  -folds 10  -format UCI    -r BaseSVM.txt

// C:\Users\kkramer\GradSchool\Papers\BinaryFeatureSelection\Experiments\ETP08_Station1_Harvest
// -dataFile ETP08_station1_harvest_Train.data  -Validation ETP08_station1_harvest_Validation.data  -c ETP08_station1_harvest.cfg  -r Test.txt

// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\ForestCover_1500_Acc\Merge1Best
// -dataFile  ../CovType-TrainTest.data  -Validation ../CovType-Validation.data -c covtype_BinaryClasses-FeaturesSelTuned.cfg  -r covtype_BinaryClasses-FeaturesSelTuned.txt

// -c SixClasses.cfg -Folds 2 -r c:\Temp\SixClasses.txt
// -c SMP751001_34_35_37.cfg -dataFile I:\Pices\may_BP_2010\features_34_35_37.data -folds 10 -r I:\Pices\may_BP_2010\crossValidation_report_34_35_37.txt

// -c experiment02-08-11.cfg -dataFile  c:\temp\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -r  c:\temp\OilExperiment\cross_validation_02-08-11.txt -Randomize 

// 02/21/2011: -Validation I:\Pices\TrainingLibraries\temp\all_but_34_35_37.data -DF I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -Config c:\Pices\DataFiles\TrainingModels\experiment02-08-11.cfg -R I:\Pices\OilExperiment\2011-02-22-Report.data
// 02/21/2011: -Validation I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -DF I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -Config c:\Pices\DataFiles\TrainingModels\experiment02-08-11.cfg -R I:\Pices\OilExperiment\2011-02-22-SameDataReport.data
// 02/21/2011: -Validation I:\Pices\TrainingLibraries\temp2\all_but_34_35_37.data -DF I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -Config c:\Pices\DataFiles\TrainingModels\experiment02-08-11.cfg -R I:\Pices\OilExperiment\2011-02-22-ReducedDataReport.data -MissClassed I:\Pices\OilExperiment\Missclassified
// 03/01/2011: -Validation I:\Pices\TrainingLibraries\binary_testset\binary_all_but_34_35_37.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-01-BinaryDataReport.data -MissClassed I:\Pices\OilExperiment\MissclassifiedBinary
// 03/01/2011: -Validation I:\Pices\TrainingLibraries\36ClassSMPNot34_35_37.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-01-36ClassesDataReport.data -MissClassed I:\Pices\OilExperiment\Missclassified36Classes
// 03/01/2011: -Validation I:\Pices\TrainingLibraries\44ClassNOT_34_35_37.data -DF I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -Config c:\Pices\DataFiles\TrainingModels\experiment02-08-11.cfg -R I:\Pices\OilExperiment\2011-03-01-44ClassesReducedDataReport.data -MissClassed I:\Pices\OilExperiment\Missclassified44Classes
// 03/06/2011: -Validation I:\Pices\TrainingLibraries\44ClassNOT_34_35_37.data -DF I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -Config c:\Pices\DataFiles\TrainingModels\experiment02-08-11.cfg -R I:\Pices\OilExperiment\2011-03-01-44ClassesReducedDataReport.data -MissClassed I:\Pices\OilExperiment\Missclassified44Classes -PredictionReport I:\Pices\OilExperiment\Prediction44Classes.data
// 03/07/2011: -DF I:\Pices\OilExperiment\34_35_37_lessThan10BACKUPTXT.data -Config c:\Pices\DataFiles\TrainingModels\experiment02-08-11.cfg -R I:\Pices\OilExperiment\2011-03-07-44ClassesDataReport.data -PredictionReport I:\Pices\OilExperiment\Prediction44ClassesCF.data
// 03/08/2011: -DF I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-08-oil_nonoil_hard_trainset.data -MissClassed I:\Pices\OilExperiment\MissclassifiedHard-03-08-2011 -Randomize -save I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_STRATIFIED.data

// 03/15/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-BinaryDataReport-1stStage.data -MissClassed I:\Pices\OilExperiment\MissclassifiedBinary-1stStage-03-15-11

// 03/15/2011: -Validation I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -DF I:\Pices\TrainingLibraries\binary_testset\binary_all_but_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\MissclassifiedBinary-TrainedOn2nd_Teston1st.data -MissClassed I:\Pices\OilExperiment\MissclassifiedBinary-TrainedOn2nd_Teston1st
// 03/15/2011: -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-BinaryDataReport-1stStageNONRANDOMIZED.data 

// 03/15/2011: -Validation I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -DF I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_STRATIFIED.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-MissclassifiedBinary-TrainedOn2nd_Teston1st.data -MissClassed I:\Pices\OilExperiment\MissclassifiedBinary-TrainedOn2nd_Teston1st
// 03/15/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-BinaryDataReport-1stStage.data -MissClassed I:\Pices\OilExperiment\MissclassifiedBinary-1stStage-03-15-11

// 03/15/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-two-stageEXP-1stStage.data -MissClassed I:\Pices\OilExperiment\2011-03-15-two-stageEXP-1stStage

// 03/15/2011: -Validation I:\Pices\TrainingLibraries\2011-03-15-two-stageEXP-1stStage-missclassified_other.data -DF I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_STRATIFIED.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-two-stageEXP-2ndStage.data -MissClassed I:\Pices\OilExperiment\2011-03-15-two-stageEXP-2ndStage
// 03/15/2011: -Randomize -DF I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_expanded.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-oil_nonoil_hard_trainset_expanded.data 

// 03/15/2011: -Validation I:\Pices\TrainingLibraries\2011-03-15-two-stageEXP-1stStage-missclassified_other.data -DF I:\Pices\TrainingLibraries\oil_nonoil_hard_trainset_expanded.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-01.cfg -R I:\Pices\OilExperiment\2011-03-15-two-stageEXP-2ndStage-after_expansion.data 

// 03/22/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22.cfg -R I:\Pices\OilExperiment\2011-03-22-BinaryDataReport-1stStage.data -MissClassed I:\Pices\OilExperiment\MissclassifiedBinary-1stStage-2011-03-22
// 03/22/2011: -Randomize -DF I:\Pices\TrainingLibraries\2ndStage-2011-03-22.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22.cfg -R I:\Pices\OilExperiment\2011-03-22-2ndStage-2011-03-22.data 
// 03/22/2011: -Randomize -Validation I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -DF I:\Pices\TrainingLibraries\2ndStage-2011-03-22.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22.cfg -R I:\Pices\OilExperiment\2011-03-22-MissclassifiedBinary-TrainedOn2nd_Teston1st.data 
// 03/22/2011: -Randomize -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22.cfg -R I:\Pices\OilExperiment\2011-03-22-two-stageEXP-1stStage.data

// 03/22/2011: -Randomize -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22.cfg -R I:\Pices\OilExperiment\2011-03-22-two-stageEXP-1stStage.data

// 03/22/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22-temp.cfg -R I:\Pices\OilExperiment\2011-03-22-BinaryDataReport-1stStage-temp.data
// 03/22/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22-temp_no_solinity_no_oxygen.cfg -R I:\Pices\OilExperiment\2011-03-22-BinaryDataReport-1stStage_temp_no_solinity_no_oxygen.data
// 03/22/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22-temp_no_solinity_no_oxygen.cfg -R I:\Pices\OilExperiment\2011-03-22-two-stageEXP-1stStage-W_TEMP_WOUT_SALINITY_OXYGEN.data

// 03/28/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22-temp_no_solinity_no_oxygen_no_florecence.cfg -R I:\Pices\OilExperiment\2011-03-28-two-stageEXP-1stStage-W_TEMP_WOUT_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22-temp_solinity_oxygen_florecence.cfg -R I:\Pices\OilExperiment\2011-03-28-two-stageEXP-1stStage-W_TEMP_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-22-temp_no_solinity_no_oxygen_no_florecence.cfg -R I:\Pices\OilExperiment\2011-03-28-two-stageEXP-1stStage-W_TEMP_WOUT_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-28-no_temp_no_solinity_no_oxygen_no_florecence.cfg -R I:\Pices\OilExperiment\2011-03-28-two-stageEXP-1stStage-WOUT_TEMP_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-28-temp_solinity_oxygen_florecence.cfg -R I:\Pices\OilExperiment\2011-03-28-two-stageEXP-1stStage-W_TEMP_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-03-28-no_temp_W_solinity_oxygen_florecence.cfg -R I:\Pices\OilExperiment\2011-03-28-two-stageEXP-1stStage-WOUT_TEMP_W_SALINITY_OXYGEN_FLORECENCE.data

// 03/28/2011: -Randomize -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-28_W_TEMPERATURE_NO_SALINITY_OXYGEN_FLORECENCE.cfg -R I:\Pices\OilExperiment\2011-03-28-MultiClassification36Classes_W_TEMPERATURE_NO_SALINITY_OXYGEN_FLORECENCE.cfg.data
// 03/28/2011: -Randomize -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-28_W_TEMPERATURE_SALINITY_OXYGEN_FLORECENCE.cfg -R I:\Pices\OilExperiment\2011-03-28-MultiClassification36Classes_W_TEMPERATURE_SALINITY_OXYGEN_FLORECENCE.cfg.data

// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES_WITH_TEMPERATURE.data -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-28_NO_TEMPERATURE_SALINITY_OXYGEN_FLORECENCE.cfg -R I:\Pices\OilExperiment\MultiClassification36Classes2011-03-28_NO_TEMPERATURE_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES_WITH_TEMPERATURE.data -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-28_W_TEMPERATURE_NO_SALINITY_OXYGEN_FLORECENCE.cfg -R I:\Pices\OilExperiment\MultiClassification36Classes2011-03-28_W_TEMPERATURE_NO_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES_WITH_TEMPERATURE.data -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-28_W_TEMPERATURE_SALINITY_OXYGEN_FLORECENCE.cfg -R I:\Pices\OilExperiment\MultiClassification36Classes2011-03-28_W_TEMPERATURE_SALINITY_OXYGEN_FLORECENCE.data
// 03/28/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES_WITH_TEMPERATURE.data -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-03-28_W_TEMPERATURE_WOUT_SALINITY_OXYGEN_FLORECENCE.cfg -R I:\Pices\OilExperiment\MultiClassification36Classes2011-03-28_W_TEMPERATURE_WOUT_SALINITY_OXYGEN_FLORECENCE.data


// 04/02/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-04-02-NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_BINARY_CROSSVALIDATION.data
// 04/02/2011: -Randomize -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_MULTICLASS_CROSSVALIDATION.data
// 04/02/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_36CLASSES_WITH_TEMPERATURE.data -DF I:\Pices\TrainingLibraries\SMP751001_SMP_Multiclass34_35_37_MoreThan20_WITH_TEMPERATURE.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_MULTICLASS_TESTSET.data
// 04/02/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-04-02-NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_BINARY_TESTSET.data

// 04/02/2011: -Randomize -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-04-02-NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_BINARY_CROSSVALIDATION.data -MissClassed I:\Pices\TrainingLibraries\BINARY_34_35_37_MISSCLASSIFIED

// 04/02/2011: -Randomize -DF I:\Pices\TrainingLibraries\2ndStage-2011-04-05.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-04-02-NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_2ndStage-BINARY_CROSSVALIDATION.data 
// 04/02/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY_temperature.data -DF I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-04-02-NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_BINARY_TESTSET.data -MissClassed I:\Pices\TrainingLibraries\BINARY_NON751001_SMP_MISSCLASSIFIED
// 04/02/2011: -Validation I:\Pices\TrainingLibraries\2ndStage-TESTSET-2011-04-05.data -DF I:\Pices\TrainingLibraries\2ndStage-2011-04-05.data -Config c:\Pices\DataFiles\TrainingModels\binaryClassification2011-04-02-NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-05_BINARY_TESTSET_2ND_STAGE.data 

// 04/12/2011: -Randomize -DF I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification35Classes2011-04-12_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-04-12_Gulf_2010-05_RandomSamplingMoreThan10.data 
// 04/12/2011: -Randomize -DF I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingBinary.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12-binaryClassification.cfg -R I:\Pices\OilExperiment\2011-04-12_Gulf_2010-05_RandomSamplingBinary.data 
// 04/12/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-04-12_RANDOMSAMPLING_MULTICLASS35_TESTSET.data 
// 04/12/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_BINARY.data -DF I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingBinary.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12-binaryClassification.cfg -R I:\Pices\OilExperiment\2011-04-12_RANDOMSAMPLING_BINARY_TESTSET.data 

// 04/26/2011: -Validation I:\Pices\TrainingLibraries\binary_trainset\binary_all_34_35_37_temp.data -DF I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingBinary.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12-binaryClassification.cfg -R I:\Pices\OilExperiment\2011-04-26_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2.data 

// 05/03/2011: -Randomize -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12-binaryClassification.cfg -R I:\Pices\OilExperiment\2011-05-03_RandomSampling_SMOTE_BINARY.data 
// 05/03/2011: -Randomize -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RandomSampling_SMOTE_MULTICLASS.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20.data -DF I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_NO_SMOTE.data 

// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_90percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_90PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_80percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_80PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_70percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_70PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_60percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_60PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_50percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_50PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_40percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_40PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_30percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_30PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_20percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_20PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_10percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_10PERCENT.data 
// 05/03/2011: -Validation I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_00percent.data -DF I:\Pices\TrainingLibraries\Gulf_RandomSampling_SMOTE.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-03_RANDOMSAMPLING_BINARY_OIL1_ON_OIL2_00PERCENT.data 


// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_00percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_00PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_10percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_10PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_20percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_20PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_30percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_30PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_40percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_40PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_50percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_50PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_60percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_60PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_70percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_70PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_80percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_80PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_90percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_90PERCENT_ON_RANDOM_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_100percent.data -Config c:\Pices\DataFiles\TrainingModels\2011-04-12_MultiClassification35Classes.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_100PERCENT_ON_RANDOM_OIL.data 

// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_100percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_100PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_90percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_90PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_80percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_80PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_70percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_70PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_60percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_60PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_50percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_50PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_40percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_40PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_30percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_30PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_20percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_20PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_10percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_10PERCENT_ON_NON_OIL.data 
// 05/10/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\36Class34_35_37_MoreThan20_00percent.data -Config c:\Pices\DataFiles\TrainingModels\MultiClassification36Classes2011-04-02_NO_INSTRUMENTS.cfg -R I:\Pices\OilExperiment\2011-05-10_RANDOMSAMPLING_BINARY_ORIGINAL_OIL_00PERCENT_ON_NON_OIL.data 


//05/17/2011: -Randomize -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_MFS.cfg -R I:\Pices\OilExperiment\2011-05-17_OriginalSet_CrossValidation_MFS.data
//05/17/2011: -Randomize -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-17_OriginalSet_CrossValidation_BFS.data
//05/17/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-17_RandomSet_Validation_BFS.data
//05/17/2011: -Validation I:\Pices\TrainingLibraries\Gulf_2010-05_RandomSamplingMoreThan10.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_MFS.cfg -R I:\Pices\OilExperiment\2011-05-17_RandomSet_Validation_MFS.data
//05/17/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-17_NonOilSet_Validation_BFS.data
//05/17/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_35CLASSES.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_MFS.cfg -R I:\Pices\OilExperiment\2011-05-17_NonOilSet_Validation_MFS.data


//05/22/2011: -Validation I:\Pices\TrainingLibraries\RandomSampling27-36Classes.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-22_RandomSet_Validation_BFS.data
//05/22/2011: -Validation I:\Pices\TrainingLibraries\RandomSampling27-36Classes.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_MFS.cfg -R I:\Pices\OilExperiment\2011-05-22_RandomSet_Validation_MFS.data
//05/22/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_24-36CLASSES.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-22_NonOilSet_Validation_BFS.data
//05/22/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_24-36CLASSES.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_MFS.cfg -R I:\Pices\OilExperiment\2011-05-22_NonOilSet_Validation_MFS.data

//05/22/2011: -Validation I:\Pices\TrainingLibraries\RandomSampling27-36Classes.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-22_RandomSet_Validation_BFS.data -MissClassed I:\Pices\TrainingLibraries\MissclassRandomSampling-05-22

//05/23/2011: -Randomize -DF I:\Pices\TrainingLibraries\RandomSampling2ndStage.data -Config c:\Pices\DataFiles\TrainingModels\05-23-11-7CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-23_RandomSet_2ndStage_CrossValidation_BFS.data 
//05/23/2011: -Validation I:\Pices\TrainingLibraries\NON_SMP751001_21-36CLASSES.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-22_NonOilSet_Validation_BFS.data -MissClassed I:\Pices\TrainingLibraries\MissclassNonOil-05-23

//05/23/2011: -Validation I:\Pices\TrainingLibraries\MissclassNonOil.data -DF I:\Pices\TrainingLibraries\RandomSampling2ndStage.data -Config c:\Pices\DataFiles\TrainingModels\05-23-11-7CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-23_NonOilSet_2ndStage_CrossValidation_BFS.data 
//05/23/2011: -Validation I:\Pices\TrainingLibraries\MissclassNonOil.data -DF I:\Pices\TrainingLibraries\RandomSampling2ndStage.data -Config c:\Pices\DataFiles\TrainingModels\05-23-11-7CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-23_NonOilSet_2ndStage_CrossValidation_BFS.data 

//05/30/2011: -Validation I:\Pices\TrainingLibraries\MissclassNonOil.data -DF I:\Pices\TrainingLibraries\RandomSampling2ndStage.data -Config c:\Pices\DataFiles\TrainingModels\05-23-11-7CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-05-30_NonOilSet_2ndStage_CrossValidation_BFS.data -Probability I:\Pices\OilExperiment\2011-05-30_NonOilSet_2ndStage_CrossValidation_BFS_PROBABILITIES.data -PredictionReport I:\Pices\OilExperiment\2011-05-30_NonOilSet_2ndStage_CrossValidation_BFS_PREDICTION.data
//05/30/2011: -Validation I:\Pices\TrainingLibraries\MissclassNonOil.data -DF I:\Pices\TrainingLibraries\RandomSampling2ndStage.data -Config c:\Pices\DataFiles\TrainingModels\05-30-11-7CLASSES_MFS.cfg -R I:\Pices\OilExperiment\2011-05-30_NonOilSet_2ndStage_CrossValidation_MFS.data -Probability I:\Pices\OilExperiment\2011-05-30_NonOilSet_2ndStage_CrossValidation_MFS_PROBABILITIES.data -PredictionReport I:\Pices\OilExperiment\2011-05-30_NonOilSet_2ndStage_CrossValidation_MFS_PREDICTION.data

//06/01/2011: -Validation I:\Pices\TrainingLibraries\RandomSampling27-36Classes.data -DF I:\Pices\TrainingLibraries\Multiclass_set_original.data -Config c:\Pices\DataFiles\TrainingModels\05-17-11-36CLASSES_BFS.cfg -R I:\Pices\OilExperiment\2011-06-01_RandomSet_Validation_BFS.data -Probability I:\Pices\OilExperiment\2011-06-01_RandomSet_Validation_BFS_VOTES.data

// 2011-09-07 -config etp_08_MFS.cfg -folds 10  -Report c:\Temp\etp_08_MFS_OneVsOne.txt

// D:\Users\kkramer\GradSchool\SipperProject\Papers\BinaryFeatureSelection\Experiments\MNIST
// -format C45 -Validation MNIST_TEST_10000_C45.data -DF MNIST_TRAIN_50000_C45.data  -Config 01-08-12-10DIGITS_MFS.cfg -R MNIST_Report.txt


// -dir C:\Pices\TrainingLibraries\etp_08  -c etp_08_BFS.cfg  -folds 10  -report C:\Temp\SecondBestGuessReport.txt

// -dir E:\Pices\TrainingLibraries\OilDrop-vs-FishEggs  -c OilDrop-vs-FishEggs  -folds 10  -r E:\Pices\TrainingLibraries\OilDrop-vs-FishEggs\CV_Report.txt  -pr E:\Pices\TrainingLibraries\OilDrop-vs-FishEggs\Predictions.txt

// 2012-07-15  -c SixClasses_UsfCasCor.cfg -folds 10 -r c:\Temp\SixClasses_UsfCasCor.txt


// -c USF_MFS.cfg

CrossValidationApp::CrossValidationApp ()
  :
    cancelFlag                       (false),
    config                           (NULL),
    copyMissClassedImages            (false),
    directory                        (),
    doRandomSplits                   (false),
    featuresAreNormalizedAlready     (false),
    fileDesc                         (NULL),
    examples                         (NULL),
    generateBiasMatrix               (false),
    mlClasses                     (NULL),
    inputFormat                      (FeatureFileIOPices::Driver ()),
    numOfFolds                       (10),
    numRandomSplits                  (30),
    predictionReport                 (NULL),
    probReport                       (NULL),
    punishmentFactor                 (0.10f),
    randomizeInputData               (false),
    reduction_time                   (0.0),
    report                           (NULL),
    reportFileStream                 (NULL),
    runLog                           (),
    splitPercentage                  (0.30f),
    validationData                   (NULL)

{
  InstrumentDataFileManager::Initialize ();
  mlClasses = new MLClassConstList ();
}  /* CrossValidationApp */





CrossValidationApp::~CrossValidationApp ()
{
  delete  validationData;     validationData   = NULL;
  delete  examples;           examples         = NULL;
  delete  mlClasses;       mlClasses     = NULL;
  delete  config;             config           = NULL;
  delete  reportFileStream;   reportFileStream = NULL;
  delete  probReport;         probReport       = NULL;
}





void  CrossValidationApp::RemoveDuplicateImages (FeatureVectorList&  examples)
{
  runLog.Level (10) << "RemoveDuplicateImages" << endl;

  KKStrListPtr   duplicateImages = examples.ExtractDuplicatesByImageFileName ();

  KKStrPtr  dupName = NULL;
  //StringListIterator dIDX (*duplicateImages);
  KKStrList::iterator  dIDX;

  FeatureVectorPtr  dupImage = NULL;

  //for  (dIDX.Reset ();  (dupName = dIDX.CurPtr ());  ++dIDX)
  for  (dIDX = duplicateImages->begin ();  dIDX != duplicateImages->end ();  ++dIDX)
  {
    dupName = *dIDX;
    dupImage = examples.LookUpByImageFileName (*dupName);
    while  (dupImage)
    {
      runLog.Level (10) << "RemoveDuplicateImages  Deleting Duplicate Image[" << *dupName << "]  "
        << "Class[" << dupImage->ClassName () << "]." << endl;

      examples.DeleteEntry (dupImage);
      delete  dupImage;  dupImage = NULL;
      dupImage = examples.LookUpByImageFileName (*dupName);
    }
  }

  delete  duplicateImages;  duplicateImages = NULL;

  runLog.Level (10) << "RemoveDuplicateImages  Exiting" << endl;
}  /* RemoveDuplicateImages */




void  CrossValidationApp::DistributesImagesRandomlyWithInFolds (int    imagesPerClass,
                                                                bool&  successful
                                                                )
{
  runLog.Level (10) << "DistributesImagesRandomlyWithInFolds" << endl;

  successful = true;

  FeatureVectorListPtr  newImages = examples->StratifyAmoungstClasses (mlClasses, imagesPerClass, numOfFolds);
  examples->Owner (false);
  delete examples;
  examples = newImages;
  examples->Owner (true);
  newImages = NULL;
}  /* DistributesImagesRandomlyWithInFolds */




void  CrossValidationApp::CmdLineHelp ()
{
  cout  << std::endl;
  cout  <<  "CrossValidation"                                                                << endl;
  cout  <<  "    -BiasMatrix      Generate a Bias Matrix to be used by PicesCommader to"     << endl;
  cout  <<  "                     adjust Classificartion results for learner Bias."          << endl;
  cout  <<  "    -Config          <Configuration File>"                                      << endl;
  cout  <<  "    -DF              <Data File>            Overides Config File"               << endl;
  cout  <<  "    -Dir             <Source Directory>"                                        << endl;
  cout  <<  "    -Folds           <Num of Folds>         defaults to 10"                     << endl;
  cout  <<  "    -Format          <" << FeatureFileIO::FileFormatsReadOptionsStr () << ">"   << endl;
  cout  <<  "    -Hierarchy"                                                                 << endl;
  cout  <<  "    -Log             <Log File Name>"                                           << endl;
  cout  <<  "    -MissClassed     <Directory where to copy Misclassified Images to>"         << endl;
  cout  <<  "    -Normalized                          Data has Already beeen Normalized"     << endl;
  cout  <<  "    -Probability     <File Name>         Generat Probability Predictions"       << endl;
  cout  <<  "    -Randomize                           Randomize order of Input Data"         << endl;
  cout  <<  "    -Report          <Report File Name>"                                        << endl;
  cout  <<  "    -SplitPercentage <Percentage to use as Training Data>"                      << endl;
  cout  <<  "    -Save             <Save File Name>"                                         << endl;
  cout  <<  "    -Validation       <Validation Data>"                                        << endl;
  cout  <<  "   -PredictionReport <filename>"                                                << endl;
  cout  << std::endl;
  cout  << std::endl;
}

// -c E:\Pices\Reports\FeatureData\ETP2008_Filtered_Norm.cfg  -format c45 -df E:\Pices\Reports\FeatureData\ETP2008_Filtered_Norm.data  -Validation E:\Pices\Reports\FeatureData\f18.m1.7clusters.data -pr E:\Pices\Reports\FeatureData\f18.m1.7clusters.Predictions.txt  -r E:\Pices\Reports\FeatureData\f18.m1.7clusters.Report.txt  -Normalized 
// -c E:\Pices\Reports\FeatureData\ETP2008_Filtered_Norm.cfg  -format c45 -df E:\Pices\Reports\FeatureData\ETP2008_Filtered_Norm.data  -Validation E:\Pices\Reports\FeatureData\f24.m1.7clusters.data -pr E:\Pices\Reports\FeatureData\f24.m1.7clusters.Predictions.txt  -r E:\Pices\Reports\FeatureData\f24.m1.7clusters.Report.txt  -Normalized 

// -c K:\Plankton\StatusReports\2004-06-20\BeamSearch.cfg   -df K:\v1\Plankton\TempBeamSearch\2004_March_02\2004_03_02_TestData.data  -r K:\Plankton\StatusReports\2004-06-20\NormalMomentFeatures.txt 

// 2005-01-12       Running Validation of Active Learning data used for jmlr paper ,  
//                  using entire pool of data as training data,  and validation data to test.
// K:\v1\Plankton\ActiveLearning\Results\2004_Sep_Expirements
//  -config FiveBigest.cfg   -format raw  -df ActiveLearning_MasterTestImages.data  -Validation ActiveLearning_ValidationImages.data  -R  TestAllMasterData.txt

void  CrossValidationApp::ProcessCmdLineParameter (KKStr  parmSwitch, 
                                                   KKStr  parmValue 
                                                  )
{
  parmSwitch.Upper ();


  if  ((parmSwitch == "-BM")  ||  (parmSwitch == "-BIASMATRIX"))
  {
    generateBiasMatrix = true;
  }


  else if  ((parmSwitch == "-C")  ||  (parmSwitch == "-CONFIG"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << "ProcessCmdLineParameter *** ERROR Missing Configuration Parameter ***"  << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    configFileName = parmValue;
  }


  else if  ((parmSwitch == "-DF")         ||  
            (parmSwitch == "-DATAFILE")   ||  
            (parmSwitch == "-DATA_FILE")  ||
            (parmSwitch == "-FEATUREFILE")
            )
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) 
        << endl
        << "CrossValidation::ProcessCmdLineParameter - Must provide File Name with -U option." 
        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    loadFileName = parmValue;
  }

  else if  ((parmSwitch == "-DIR")  ||  (parmSwitch == "-DIRECTORY"))
  {
    // The user wants the rogram to build a feature file from the examples in 
    // the directory structure pointed to by parValue.  The sub-dir names will
    // be used to specify the class name.
    directory = parmValue;
    if  (!osValidDirectory (directory))
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameter - Dir[" << directory << "]  Invaild Directory." << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }

  else if  ((parmSwitch == "-F")  ||  (parmSwitch == "-FORMAT"))
  {
    parmValue.Upper ();
    inputFormat = FeatureFileIO::FileFormatFromStr (parmValue, true, false);
    if  (inputFormat == NULL)
    {
      cerr << endl
           << endl
           << "*** ERROR ***       Invalid File Format[" << parmValue << "]"       << endl
           << "                    Valid Formats " << FeatureFileIO::FileFormatsReadOptionsStr () << endl
           << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    } 
  }

  else if  (parmSwitch == "-FOLDS")
  {
    parmValue.Upper ();

    if  (parmValue.Empty ())
    {
      cerr << "*** ERROR *** Must supply the Number of Folds to process" << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    numOfFolds = atoi (parmValue.Str ());

    if  (numOfFolds < 1)
    {
      cerr << endl << "*** ERROR *** numOfFolds["  << numOfFolds << "] must be greater than 0." << endl << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    } 
  }
  
  else if  ((parmSwitch == "-L")  ||  (parmSwitch == "-LOG"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << endl
                        << "CrossValidation::ProcessCmdLineParameter - Must provide File Name with -L option." 
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    runLog.AttachFile (parmValue);
  }


  else if  ((parmSwitch == "-MC")  ||  (parmSwitch == "-MISSEDCLASSED")  ||  (parmSwitch == "-MISSCLASSED"))
  {
    copyMissClassedImages = true;
    if  (parmValue.Empty ())
    {
      // Since no directory was specified will default to:
      missedClassedDir = "c:\\Temp\\MissClassifiedImages";
    }
    else
    {
      missedClassedDir = parmValue;
    }
  }


  else if  ((parmSwitch == "-N")  ||  (parmSwitch == "-NORMALIZED"))
  {
    featuresAreNormalizedAlready = true;
  }


  else if  ((parmSwitch == "-P")             ||  
            (parmSwitch == "-PROBABILITY")   ||
            (parmSwitch == "-PROBABILITIES") ||
            (parmSwitch == "-PROBREPORT")
    )
  {
    if  (parmValue.Empty ())
      probReportName = "Predictions.txt";
    else
      probReportName = parmValue;
  }

  else if  ((parmSwitch == "-PREDICTIONREPORT")  ||  (parmSwitch == "-PR"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << endl
                        << "*** Error ***  Must supply a Prediction Report Filename  with  the '-PR (-PredictionReport)' option." << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    predictionReportName = parmValue;
  }

  else if  ((parmSwitch == "-RANDOM")  ||  (parmSwitch == "-RANDOMIZE"))
  {
    randomizeInputData = true;
  }

  else if  ((parmSwitch == "-R")  ||  (parmSwitch == "-REPORT"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << "CrossValidation::ProcessCmdLineParameter - Must provide File Name with -R option." << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
    else
    {
      reportFileName = parmValue;
    }
  }

  else if  ((parmSwitch == "-S")  ||  (parmSwitch == "-SAVE"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << endl
                        << "CrossValidation::ProcessCmdLineParameter - Must provide File Name with -S option."  << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    saveFileName = parmValue;
  }

  else if  ((parmSwitch == "-SP")  ||  (parmSwitch == "-SPLITPERCENTAGE"))
  {
    splitPercentage = parmValue.ToPercentage ();
    if  ((splitPercentage <= 0.0f)  ||  (splitPercentage >= 100.0f))
    {
      runLog.Level (-1) << endl
                        << "*** Error ***  Split Percentage must be greater than 0.0% and less than 100.0%" << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }
  }

  else if  ((parmSwitch == "-V")  ||  (parmSwitch == "-VALIDATION")  ||  (parmSwitch == "-VALIDATE"))
  {
    if  (parmValue.Empty ())
    {
      runLog.Level (-1) << endl
                        << "*** Error ***  Must supply a Validation Data Filename  with  the '-validate' option." << endl
                        << endl;
      CmdLineHelp ();
      osWaitForEnter ();
      exit (-1);
    }

    validationFileName = parmValue;
  }

  else
  {
    runLog.Level (-1) << endl
                      << "CrossValidation::ProcessCmdLineParameter   Invalid Switch["  <<  parmSwitch << "]." 
                      << endl;
    CmdLineHelp ();
    osWaitForEnter ();
    exit (-1);
  }

  return;
}  /* ProcessCmdLineParameter */





void  CrossValidationApp::ReportCmdLineParameters ()
{
  *report << endl;
  *report << "Configuration File   [" << configFileName                                << "]" << endl;
  *report << "Input Format         [" << inputFormat->DriverName ()                    << "]" << endl;

  if  (!directory.Empty ())
    *report << "Directory            [" << directory                                   << "]" << endl;

  *report << "Log File             [" << runLog.FileName ()                            << "]" << endl;
  *report << "LoadFile             [" << loadFileName                                  << "]" << endl;
  *report << "NumOfFolds           [" << numOfFolds                                    << "]" << endl;
  *report << "ReportFileName       [" << reportFileName                                << "]" << endl;
  *report << "PredictionReportName [" << predictionReportName                          << "]" << endl;
  if  (!probReportName.Empty ())
    *report << "ProbReportName       [" << probReportName                                << "]" << endl;
  *report << "SaveFileName         [" << saveFileName                                  << "]" << endl;
  *report << "Already Normalized   [" << (featuresAreNormalizedAlready ? "Yes" : "No") << "]" << endl;
  *report << "Validation File Name [" << validationFileName                            << "]" << endl;

  *report << endl;
}  /* ReportCmdLineParameters */





void  CrossValidationApp::ProcessCmdLineParameters (int     argcXXX, 
                                                    char**  argvXXX
                                                   )
{
  CmdLineExpander  cmdLineExpander (KKStr (argvXXX[0]), runLog, argcXXX, argvXXX);
  if  (!cmdLineExpander.ParmsGood ())
  {
    runLog.Level (-1) << endl
                      << "ProcessCmdLineParameters    *** ERROR ***" << endl
                      << endl
                      << "Could not expand command line parameters" << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }

  const VectorKKStr&  parameters = cmdLineExpander.ExpandedParameters ();

  uint  x = 0;

  for  (x = 0; x < parameters.size ();  x++)
  {
    KKStr  parmValue ("");
    KKStr  parmSwitch ("");

    if  (parameters[x][0] == '-')  
    {
      parmSwitch = parameters[x];
      if  (parameters.size () > (x + 1))  
      {
        if  (parameters[x + 1][0] != '-')
        {
          x++;
          parmValue = parameters[x];
        }
      }
      ProcessCmdLineParameter (parmSwitch, parmValue);
    }
  }  /*  end of for loop  */



  if  ((!directory.Empty ())  &&  (!loadFileName.Empty ()))
  {
    runLog.Level (-1) << endl
                      << "ProcessCmdLineParameters    *** ERROR ***" << endl
                      << endl
                      << "Can not specify Directory and Data file both." << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }



  if  ((configFileName.Empty ())  &&  (directory.Empty ())  &&  (loadFileName.Empty ()))
  {
    // At the very least we need a configuration file or Feature File, otherwise there
    // is nothing we can do.
    runLog.Level (-1) << endl
                      << "ProcessCmdLineParameters **** Configuration File not specified."
                      << endl;
    CmdLineHelp ();
    osWaitForEnter ();
    exit (-1);
  }


  if  ((configFileName.Empty ())  &&  (generateBiasMatrix))
  {
    // User wants to generate a BiasMatrix but did not specify a Classifer.
    runLog.Level (-1) << endl
                      << "ProcessCmdLineParameters ***ERROR***   Must specify a configuration file to generate a BiasMatrix."
                      << endl;
    CmdLineHelp ();
    osWaitForEnter ();
    exit (-1);
  }


  bool  successful;
  bool  changesMade = false;


  if  ((!configFileName.Empty ())  &&  (directory.Empty ())  &&  (loadFileName.Empty ()))
  {
    // We are going to load data from configuration file.
    runLog.Level (10) << " ProcessCmdLineParameters::Loading Feature Data using Cinfig File[" << configFileName << "]." << endl;

    bool  successful = false;
    KKStr  errorMessage;

    fileDesc = FeatureFileIOPices::NewPlanktonFile (runLog);

    config = new TrainingConfiguration2 (fileDesc,
                                        configFileName, 
                                        runLog, 
                                        true  // true = validate directories
                                       );
      
    if  (!config->FormatGood ())
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameters   *** ERROR ***" << endl
                        << endl
                        << "Error loading config file.  ErrorMessage[" << errorMessage << "]" << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    delete  mlClasses;  mlClasses = NULL;
    mlClasses = config->ExtractClassList ();

    DateTime  latestImageTimeStamp;
    bool      changesMadeToTrainingLibraries;

    examples = config->LoadFeatureDataFromTrainingLibraries (latestImageTimeStamp, changesMadeToTrainingLibraries, cancelFlag);

    if  (!examples)
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameters   *** ERROR ***" << endl
                        << endl
                        << "Error loading feature data using configuration file[" << configFileName << "]" << endl
                        << "ErrorMessage[" << errorMessage << "]" << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    delete  mlClasses;
    mlClasses = examples->ExtractMLClassConstList ();
    DistributesImagesRandomlyWithInFolds (-1, successful);
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameters    *** ERROR ***" << endl
                        << endl
                        << "Could not redistribute feature data amongst classes" << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }

  else if  (!directory.Empty ())
  {
    runLog.Level (10) << "ProcessCmdLineParameters::Loading Feature Data from Directory[" << directory << "]." << endl;

    bool  successful = false;
    KKStr  errorMessage;
    
    fileDesc = FeatureFileIOPices::NewPlanktonFile (runLog);

    if  (configFileName.Empty ())
    {
      config = TrainingConfiguration2::CreateFromDirectoryStructure 
                                        (fileDesc,
                                         configFileName,
                                         directory,
                                         runLog,
                                         successful,
                                         errorMessage
                                        );
     
    }
    else
    {
      config = new TrainingConfiguration2 (fileDesc,
                                          configFileName, 
                                          runLog, 
                                          true  // true = validate directories
                                         );    
      successful = config->FormatGood ();
    }
      
    if  ((!config) || (!successful))
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameters   *** ERROR ***" << endl
                        << endl
                        << "Failed to build ConfigFile from Directory[" << directory << "]" << endl
                        << "ErrorMessage[" << errorMessage << "]" << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    delete  mlClasses;
    mlClasses = config->ExtractClassList ();

    delete  examples;
    examples = FeatureFileIOPices::LoadInSubDirectoryTree 
                                    (directory, 
                                     *mlClasses, 
                                     true,           // use Directory name for className
                                     cancelFlag,
                                     true,            // reWrite Featutre Files.
                                     runLog
                                    );
  
    RemoveDuplicateImages (*examples);


    delete  mlClasses;
    mlClasses = examples->ExtractMLClassConstList ();

    mlClasses->SortByName ();

    DistributesImagesRandomlyWithInFolds (-1, successful);
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "ProcessCmdLineParameters    *** ERROR ***" << endl
                        << endl
                        << "Could not redistribute feature data amongst classes" << endl
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }
  }

  else if  (loadFileName.Empty ())
  {
    runLog.Level (-1) << endl
                      << "ProcessCmdLineParameters    *** ERROR ***" << endl
                      << endl
                      << "No data file or directory specified." << endl
                      << endl;
    osWaitForEnter ();
    exit (-1);
  }

  else
  {
    // User has specified a file with feature data to use rathar than extrcating data
    // from examples.  ( -U command line option)
    runLog.Level (10) << "ProcessCmdLineParameters::Loading Feature Data from File[" << loadFileName << "]." << endl;

    delete  examples;
    examples = inputFormat->LoadFeatureFile (loadFileName, *mlClasses, -1, cancelFlag, successful, changesMade, runLog);
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "main::Error Loading feature Data["
                        << loadFileName
                        << "],  Cross Validation Terminated."
                        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    // Since for right now we do not know how to handle missing data, 
    // will remove from data loaded from file;
    examples->RemoveEntriesWithMissingFeatures ();

    fileDesc = examples->FileDesc ();

    if  (configFileName.Empty ())
    {
      configFileName = osRemoveExtension (loadFileName) + ".cfg";
      if  (!osFileExists (configFileName))
      {
        config = new TrainingConfiguration2 (fileDesc, 
                                            mlClasses, 
                                            "-m 200 -s 0 -n 0.11 -t 2 -g 0.1  -c 1  -u 100  -up  -sm Voting  -mt OneVsOne",
                                            runLog            
                                           );
        config->Save (configFileName);
        delete  config;  config = NULL;
      }
    }

    {
      config = new TrainingConfiguration2 (fileDesc, configFileName, runLog, false);
      if  (!config->FormatGood ())
      {
        runLog.Level (-1) << endl
                          << "ProcessCmdLineParameters **** Invalid Configuration File[" << configFileName << "] ****" << endl
                          << endl;
        CmdLineHelp ();
        osWaitForEnter ();
        exit (-1);
      }
      MLClassConstListPtr  configMLClasses = config->ExtractClassList ();

      FeatureVectorListPtr  filteredList = new FeatureVectorList (*configMLClasses, *examples, runLog);
      if  (filteredList->QueueSize () == examples->QueueSize ())
      {
        delete  filteredList;  
        filteredList = NULL;
      }
      else
      {
        FeatureVectorListPtr  dupedFilteredExamples = new FeatureVectorList (*filteredList, true);
        delete  filteredList;  filteredList = NULL;
        delete  examples;      examples     = NULL;
        examples = dupedFilteredExamples;
        delete  mlClasses;
        mlClasses = examples->ExtractMLClassConstList ();
      }

      delete  configMLClasses;  configMLClasses= NULL;
    }
  }


  if  (!validationFileName.Empty ())
  {
    validationData = inputFormat->LoadFeatureFile 
                                     (validationFileName, 
                                      *mlClasses,
                                      -1,
                                      cancelFlag,
                                      successful,
                                      changesMade,
                                      runLog
                                     );
    if  (!successful)
    {
      runLog.Level (-1)
        << endl
        << "ProcessCmdLineParameters   *** ERROR ***"  << endl
        << endl
        << "                    Error loading Validation Data File[" << validationFileName << "]" << endl
        << endl;
      osWaitForEnter ();
      exit (-1);
    }

    // Since for right now we do not know how to handle missing data, 
    // will remove from data loaded from file;
    validationData->RemoveEntriesWithMissingFeatures ();
  }
}  /* ProcessCmdLineParameters */





#define  thresholdSmall   500
#define  thresholdMed    1000


KKStr  CrossValidationApp::LocateImageFileUsingConfigurationFile (const  KKStr&        fileName,
                                                                  MLClassConstPtr   knownClass
                                                                 )
{
  const 
  TrainingClassList& trainClasses = config->TrainingClasses ();

  KKStr  dirWhereFileIs;


  TrainingClassList::const_iterator  tclIDX;

  TrainingClassPtr trainingClass = NULL;

  for  (tclIDX = trainClasses.begin ();  tclIDX != trainClasses.end (); tclIDX++)
  {
    trainingClass = *tclIDX;

    if  (knownClass)
    {
      if  (trainingClass->MLClass ()->UpperName () == knownClass->UpperName ())
      {
        // We look in this specific directory
        dirWhereFileIs = osLookForFile (fileName, trainingClass->ExpandedDirectory (config->RootDir ()));
        break;
      }
    }
    else
    {
      dirWhereFileIs = osLookForFile (fileName, trainingClass->ExpandedDirectory (config->RootDir ()));
      if  (!dirWhereFileIs.Empty ())
        break;
    }
  }

  return  dirWhereFileIs;
}  /* LocateImageFileUsingConfigurationFile */





void  CrossValidationApp::PostMisclassifiedImage (KKStr               fileName,
                                                  MLClassConstPtr  knownClass,
                                                  MLClassConstPtr  predictedClass
                                                 )
{
  KKStr  newDir;
  KKStr  resultDir;

  resultDir << osAddSlash (missedClassedDir) 
            << knownClass->Name () << DS
            << predictedClass->Name ();
  osCreateDirectoryPath (resultDir);

  bool copyGood;

  int LastOccurence=fileName.LocateLastOccurrence("\\");
  KKStr subDir;
  if (LastOccurence!=-1)
  {
    subDir=fileName.SubStrPart(0,LastOccurence);
    fileName=fileName.SubStrPart(LastOccurence+1);
    
  }

  //KKStr  dirWhereFileIs = LocateImageFileUsingConfigurationFile (fileName, knownClass);
  KKStr dirWhereFileIs="I:\\Pices\\TrainingLibraries\\NON_SMP751001_21-36CLASSES\\"+subDir;

  if  (dirWhereFileIs.Empty ())
  {
    dirWhereFileIs = osLookForFile (fileName, "I:\\Pices\\TrainingLibraries\\NON_SMP751001_21-36CLASSES\\");
  }

  if  (dirWhereFileIs.Empty ())
  {
    cerr << "PostMisclassifiedImage   *** ERROR ***   Could not find example[" << fileName << "]" << endl;
  }
  else
  {
    copyGood = osCopyFileBetweenDirectories (fileName, dirWhereFileIs, resultDir);
  }
  cout << knownClass->Name () << " - " << predictedClass->Name () << endl;
}  /* PostMisclassifiedImage */



void  CrossValidationApp::CrossValidate 
                    (FeatureVectorListPtr   testImages, 
                     FeatureVectorListPtr   trainingImages,
                     ConfusionMatrix2&      cm,
                     ConfusionMatrix2&      cm2cdGuess,
                     ConfusionMatrix2&      cmSmall,
                     ConfusionMatrix2&      cmMed,
                     ConfusionMatrix2&      cmLarge,
                     int&                   foldCount,
                     double&                foldAccuracy,
                     int                    foldNum,
                     double&                trainingTime,
                     double&                classificationTime,
                     int&                   reductionPreImageCount,
                     int&                   reductionPostImageCount,
                     double&                reductionTime,
                     int&                   supportPoints,
                     KKStr&                 bitReductionByFeature
                    )

{
  runLog.Level (10) << "CrossValidate   FoldNum[" << foldNum  << "]." << endl;

  runLog.Level (20) << "CrossValidate   Creating Training Models" << endl;

  bool     cancelFlag = false;
  KKStr    statusMessage;

  TrainingProcess2  trainer (config, 
                             trainingImages, 
                             report,
                             fileDesc,
                             runLog,
                             featuresAreNormalizedAlready,
                             cancelFlag,
                             statusMessage
                            );

  //double  startTrainingTime = osGetSystemTimeUsed ();
  trainer.CreateModelsFromTrainingData ();
  //double  endTrainingTime   = osGetSystemTimeUsed ();
  //trainingTime = (endTrainingTime - startTrainingTime);

  supportPoints           = trainer.NumOfSupportVectors ();

  trainingTime            = trainer.TrainingTime ();
  reductionPreImageCount  = trainer.ReductionPreExampleCount  ();
  reductionPostImageCount = trainer.ReductionPostExampleCount ();
  reductionTime           = trainer.ReductionTime ();
  bitReductionByFeature   = trainer.BitReductionByFeature ();

  runLog.Level (20) << "CrossValidate   Creating Classification Object" << endl;

  Classifier2  classifier (&trainer, runLog);

  noiseMLClass = mlClasses->GetNoiseClass ();

  //FeatureVectorListIterator  imageIDX (*testImages);
  FeatureVectorList::iterator  imageIDX;

  int  foldCorrect = 0;
  foldCount = 0;

  if  (!probReportName.Empty ())
  {
    if  (!probReport)
    {

      probReport = new ofstream (probReportName.Str ());
      *probReport << endl << endl << endl << endl;

      *probReport << "Probability Predictions by Class"  << endl << endl;

      *probReport << "ImageFileName" << "\t" << "KnownClass"    << "\t" << "PredClass";

      for  (int  classIDX = 0;  classIDX < mlClasses->QueueSize ();  classIDX++)
      {
        *probReport << "\t" << mlClasses->IdxToPtr (classIDX)->Name ();
      }

      *probReport << endl;
    }
  }

  runLog.Level (20) << "CrossValidate   Classifying Test Images." << endl;

  FeatureVectorPtr  example = NULL;
  double  startClassificationTime = osGetSystemTimeUsed ();

  //for  (imageIDX.Reset (); (example = imageIDX.CurPtr ()); ++imageIDX)
  for  (imageIDX = testImages->begin ();  imageIDX != testImages->end (); ++imageIDX)
  {
    example = *imageIDX;

    MLClassConstPtr  knownClass = example->MLClass ();

    MLClassConstPtr  predClass1       = NULL;
    MLClassConstPtr  predClass2       = NULL;
    int32               predClass1Votes  = 0;
    int32               predClass2Votes  = 0;
    double              knownClassProb   = 0.0;
    double              predClass1Prob   = 0.0;
    double              predClass2Prob   = 0.0;
    int32               numOfWinners     = 0;
    double              breakTie         = 0.0;
    double              compact          = 0.0;

    classifier.ClassifyAImage (*example,
                               predClass1,       predClass2,
                               predClass1Votes,  predClass2Votes,
                               knownClassProb,   
                               predClass1Prob,   predClass2Prob,
                               numOfWinners,     breakTie,
                               compact
                              );
    
    // Becuase unfortunatly 'ClassifyAImage'  updates the MLClass field in the 'FeatureVector' 
    // instance to the predicted class we need to set the class back to the original class.
    example->MLClass (knownClass);

    cm.Increment (knownClass,
                  predClass1,
                  (int)example->OrigSize (),
                  predClass1Prob,
                  runLog
                 );
    if  (knownClass != predClass1)
    {
      cm2cdGuess.Increment (knownClass,
                            predClass2,
                            (int)example->OrigSize (),
                            predClass2Prob,
                            runLog
                           );
    }

    if  (predictionReport)
    {
      *predictionReport << osGetRootName (example->ImageFileName ()) << "\t"
                        << (int)example->OrigSize () << "\t"
                        << knownClass->Name ()       << "\t"
                        << predClass1->Name ()       << "\t"
                        << predClass1Prob            << "\t"
                        << predClass2->Name ()       << "\t"
                        << predClass2Prob
                        << endl;
    }

    if  (probReport)
    {
      double*  probs = new double[mlClasses->QueueSize ()];
      int*     votes = new int   [mlClasses->QueueSize ()];
      classifier.ProbabilitiesByClass (*mlClasses, example, votes, probs);

      int  classIDX = 0;

      *probReport 
        << osGetRootName (example->ImageFileName ())  << "\t"
        << knownClass->Name ()                        << "\t"
        << predClass1->Name ();

      int*  probsIDX = new int[mlClasses->QueueSize ()];
      for  (classIDX = 0;  classIDX < mlClasses->QueueSize ();  classIDX++)
      {
        *probReport << "\t" << StrFormatDouble ((probs[classIDX] * 100), "zzz,zz0.000") << "%";
        probsIDX[classIDX] = classIDX;
      }

      *probReport << endl;
      delete[]  probs;  probs = NULL;
      delete[]  votes;  votes = NULL;
    }

    if  (knownClass == predClass1)
    {
      foldCorrect++;
    }

    runLog.Level (50) << "CrossValidate - Known Class["      << knownClass->Name () << "]  "
                      <<                 "Predicted Class["  << predClass1->Name () << "]."
                      << endl;


    if  (copyMissClassedImages)
    {
      if  (knownClass != predClass1)
        PostMisclassifiedImage ((*imageIDX)->ImageFileName (),
                                knownClass, 
                                predClass1
                               );
    }



    if  ((int)(*imageIDX)->OrigSize () < thresholdSmall)
    {
      cmSmall.Increment (knownClass, 
                         predClass1, 
                         (int)(*imageIDX)->OrigSize (), 
                         predClass1Prob,
                         runLog
                        );
    }

    else if  ((int)(*imageIDX)->OrigSize () < thresholdMed)
    {
      cmMed.Increment (knownClass, 
                       predClass1, 
                       (int)(*imageIDX)->OrigSize (), 
                       predClass1Prob,
                       runLog
                      );
    }

    else
    {
      cmLarge.Increment (knownClass, 
                         predClass1, 
                         (int)(*imageIDX)->OrigSize (), 
                         predClass1Prob,
                         runLog
                        );
    }

    foldCount++;
  }

  double  endClassificationTime = osGetSystemTimeUsed ();

 classificationTime = endClassificationTime - startClassificationTime;

 if  (foldCount == 0)
   foldAccuracy = 0.0;
 else
   foldAccuracy = 100.0 * (double)foldCorrect / (double)foldCount;

 runLog.Level (20) << "CrossValidate - Done." << endl;

 reduction_time += trainer.ReductionTime();

}  /* CrossValidate */






void  CrossValidationApp::ValidationProcess ()
{
  runLog.Level (10) << "ValidationProcess" << endl;

  double time_before = 0.0, time_after = 0.0;
  double classification_time = 0.0;
  double training_time = 0.0;
  double reduction_time = 0.0;

  if  (!validationData)
  {
    runLog.Level (-1)
      << endl
      << "ValidationProcess   *** ERROR ***"  << endl
      << endl
      << "              *** No Validation Data ***" << endl
      << endl;
    osWaitForEnter ();
    exit (-1);
  }

  bool     cancelFlag    = false;
  KKStr   statusMessage = "";

  TrainingProcess2  trainer (config, 
                             examples,
                             report,
                             fileDesc, 
                             runLog,
                             featuresAreNormalizedAlready,
                             cancelFlag,
                             statusMessage
                            );

  // train the SVM
  time_before = osGetSystemTimeUsed();
  trainer.CreateModelsFromTrainingData ();
  time_after = osGetSystemTimeUsed();
  training_time = time_after - time_before;

  int  numOfSupportVectors  =  trainer.NumOfSupportVectors ();

  runLog.Level (10) << "ValidationProcess   Support Vectors Found[" << numOfSupportVectors << "]" << endl;

  // create the classification object
  runLog.Level (20) << "ValidationProcess   Creating Classification Object" << endl;
  Classifier2 classifier (&trainer, runLog);

  int  numOfClasses = mlClasses->QueueSize ();
  ConfusionMatrix2   cm         (*mlClasses);
  ConfusionMatrix2   cm2cdGuess (*mlClasses);
  ConfusionMatrix2   cmSmall    (*mlClasses);
  noiseMLClass = mlClasses->GetNoiseClass ();

  FeatureVectorPtr  example = NULL;

  //FeatureVectorListIterator  imageIDX (*validationData);
  FeatureVectorList::iterator  imageIDX;

  runLog.Level (20) << "ValidationProcess   Classifying Validation Images." << endl;

  int  numPredicted = 0;


  *report << "Image"  << "\t" << "Known" << "\t" << "Predicted"  << "\t" << "Predicted"   << endl;
  *report << "Name"   << "\t" << "Class" << "\t" << "Class"      << "\t" << "Probability" << endl;
  // perform the classification
  time_before = osGetSystemTimeUsed();

  if  (!probReportName.Empty ())
  {
    if  (!probReport)
    {
      probReport = new ofstream (probReportName.Str ());
      *probReport << endl << endl << endl << endl;

      *probReport << "Probability Predictions by Class"  << endl << endl;

      *probReport << "ImageFileName" << "\t" << "KnownClass"    << "\t" << "PredClass";

      for  (int  classIDX = 0;  classIDX < mlClasses->QueueSize ();  classIDX++)
      {
        *probReport << "\t" << mlClasses->IdxToPtr (classIDX)->Name ();
      }

      *probReport << endl;
    }
  }

  //for  (imageIDX.Reset (); (example = imageIDX.CurPtr ()); ++imageIDX)
  for  (imageIDX = validationData->begin ();  imageIDX != validationData->end (); ++imageIDX)
  {
    example = *imageIDX;
    MLClassConstPtr  knownClass = example->MLClass ();

    MLClassConstPtr  predClass1       = NULL;
    MLClassConstPtr  predClass2       = NULL;
    int32               predClass1Votes  = 0;
    int32               predClass2Votes  = 0;
    double              knownClassProb   = 0.0;
    double              predClass1Prob   = 0.0;
    double              predClass2Prob   = 0.0;
    int32               numOfWinners     = 0;
    double              breakTie         = 0.0;
    double              compact          = 0.0;

    classifier.ClassifyAImage (*example,
                               predClass1,      predClass2,
                               predClass1Votes, predClass2Votes,
                               knownClassProb, 
                               predClass1Prob,  predClass2Prob,
                               numOfWinners,    breakTie,
                               compact
                              );
    // Becuase unfortunatly 'ClassifyAImage'  updates the MLClass field in the 'FeatureVector' 
    // instance to the predicted class we need to set the class back to the original class.
    example->MLClass (knownClass);

    cm.Increment (knownClass,
                  predClass1,
                  (int)example->OrigSize (),
                  predClass1Prob,
                  runLog
                 );
    if  (knownClass != predClass1)
    {
      cm2cdGuess.Increment (knownClass,
                            predClass2,
                            (int)example->OrigSize (),
                            predClass2Prob,
                            runLog
                          );
    }

    //*report << example->ImageFileName ()  << "\t" 
    //        << knownClass->Name     ()  << "\t" 
    //        << predictedClass->Name ()  << "\t"
    //        << (probability * 100.0) 
    //        << endl;

    if  (copyMissClassedImages)
    {
      if  (knownClass != predClass1)
      {
        PostMisclassifiedImage (example->ImageFileName (), knownClass, predClass1);
      }
    }

    if  (predictionReport)
    {
      *predictionReport << osGetRootName (example->ImageFileName ()) << "\t"
                        << (int)example->OrigSize () << "\t"
                        << knownClass->Name ()       << "\t"
                        << predClass1->Name ()       << "\t"
                        << predClass1Prob            << "\t"
                        << predClass2->Name ()       << "\t"
                        << predClass2Prob
                        << endl;
    }

    if  (probReport)
    {
      double*  probs = new double[mlClasses->QueueSize ()];
      int*     votes = new int   [mlClasses->QueueSize ()];
      classifier.ProbabilitiesByClass (*mlClasses, example, votes, probs);

      int  classIDX = 0;

      *probReport 
        << osGetRootName (example->ImageFileName ())  << "\t"
        << knownClass->Name ()                        << "\t"
        << predClass1->Name ();

      int*  probsIDX = new int[mlClasses->QueueSize ()];
      for  (classIDX = 0;  classIDX < mlClasses->QueueSize ();  classIDX++)
      {
        *probReport << "\t" << votes[classIDX] ;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! changed probs to votes
        probsIDX[classIDX] = classIDX;
      }

      *probReport << endl;
      delete[]  probs;  probs = NULL;
      delete[]  votes;  votes = NULL;
    }


    numPredicted++;
    if  ((numPredicted % 1000) == 0)
      runLog.Level (10) << "   Num Predicted[" << numPredicted << "] of [" << validationData->QueueSize () << "]" << endl;
  }

  time_after = osGetSystemTimeUsed();
  classification_time = time_after - time_before;


  *report << "All Validation Data."  << endl;
  cm.PrintConfusionMatrixTabDelimited (*report);
  *report << endl << endl << endl;

  *report << "All Validation Data  -  Second Guess Performance."  << endl;
  cm2cdGuess.PrintConfusionMatrixTabDelimited (*report);
  *report << endl << endl << endl;

  *report << "Error by Probability."  << endl;
  cm.PrintErrorByProb (*report);
  *report << endl << endl << endl;

  endTime = osGetLocalDateTime();
  DateTime  elapsedTime = endTime - startTime;

  reduction_time = trainer.ReductionTime ( );
  long int num_images_before_reduction = trainer.ReductionPreExampleCount ( );
  long int num_images_after_reduction  = trainer.ReductionPostExampleCount ( );
  float reduction_ratio = trainer.ReductionRatio ( );
  training_time -= reduction_time;


  *report << "****************************************************************************************"         << endl;
  *report << "CPU Run Time Statistics"                                                                          << endl;
  *report << "Start          Time [" << startTime      << "]."                                                  << endl;
  *report << "End            Time [" << endTime        << "]."                                                  << endl;
  *report << "Elapsed        Time [" << elapsedTime    << "],  secs [" << elapsedTime.Seconds () << "]."        << endl;
  *report << "Training       Time [" << StrFormatDouble (training_time,       "zzz,zz0.000")     << "] secs."   << endl;
  *report << "Classification Time [" << StrFormatDouble (classification_time, "zzz,zz0.000")     << "] secs."   << endl;
  *report << "Reduction      Time [" << StrFormatDouble (reduction_time, "zzz,zz0.000")          << "] secs."    << endl;
  *report << "Pre Reduction       ["  << num_images_before_reduction                             << "] examples," << endl;
  *report << "Post Reduction      ["  << num_images_after_reduction                              << "] examples,"  << endl;
  *report << "Reduction Ratio     ["  << StrFormatDouble (reduction_ratio, "zzz,zz0.000")        << "]"          << endl;
  *report << "Support Vectors     ["  << numOfSupportVectors                                     << "]"         << endl;
  *report << "****************************************************************************************"         << endl;
  *report << endl << endl;

  delete  validationData;         validationData     = NULL;

  runLog.Level (10) << "ValidationProcess - Done." << endl;
}  /* ValidationProcess */




void  CrossValidationApp::PrintSizeHistogram ()
{
  int  x;

  ConfusionMatrix2  cm (*mlClasses);

  for  (x = 0; x < examples->QueueSize (); x++)
  {
    FeatureVectorPtr example = examples->IdxToPtr (x);
    cm.Increment (example->MLClass (), 
                  example->MLClass (), 
                  (int)example->OrigSize (), 
                  0,
                  runLog
                 );
  }    

  cm.PrintErrorBySizeByRows (*report);

  cm.PrintErrorBySize (*report);
} /* PrintSizeHistogram */



void  CrossValidationApp::ReportConfigurationInfo ()
{
  *report << "Modeling Method[" << config->ModelingMethod () << "]."
              << endl;

  KKStr  parameterStr = "";
  if  (config->ModelParameters ())
    parameterStr = config->ModelParameters ()->ToCmdLineStr ();

  *report << "Parameter Settings[" << parameterStr << "]."
              << endl;

  *report << "Features Selected[" << config->GetFeatureNums ().ToString () << "]."
              << endl;

  *report << std::endl << std::endl
          << "Root Directory[" << config->RootDir () << "]" << std::endl
          << std::endl
          << "Training"  << "\t" << "Class" << "\t" << ""       << std::endl
          << "Class Dir" << "\t" << "Name"  << "\t" << "Weight" << std::endl;

  TrainingClassList::const_iterator  idx;
  for  (idx = config->TrainingClasses ().begin ();  idx != config->TrainingClasses ().end ();  idx++)
  {
    TrainingClassPtr tc = *idx;
    *report <<  tc->Directory () << "\t" << tc->Name () << "\t" << tc->Weight () << std::endl;
  }

  *report << endl;
}  /* ReportConfigurationInfo */






class  CrossValidationRun
{
public:
  CrossValidationRun (int                 _id,
                      CrossValidationPtr  _cv,
                      FeatureNumListPtr   _features
                     ):
     id       (_id),
     cv       (_cv),
     features (_features)
  {}


  ~CrossValidationRun ()
  {
    delete  cv;
    delete  features;
  }

  int                 id;
  CrossValidationPtr  cv;
  FeatureNumListPtr   features;
};

typedef  CrossValidationRun*  CrossValidationRunPtr;





class  CrossValidationSortByTrainTime
{
public:
   bool  operator () (CrossValidationRunPtr  p1,  
                      CrossValidationRunPtr  p2
                     )
   {
     return  p1->cv->TrainTimeTotal () < p2->cv->TrainTimeTotal ();
   }
}; /* CrossValidationSortByTrainTime */




class  CrossValidationSortBySupportPoints
{
public:
   bool  operator () (CrossValidationRunPtr  p1,  
                      CrossValidationRunPtr  p2
                     )
   {
     return  p1->cv->NumOfSupportVectors () < p2->cv->NumOfSupportVectors ();
   }
}; /* CrossValidationSortByTrainTime */



class  CrossValidationSortByAccuracy
{
public:
   bool  operator () (CrossValidationRunPtr  p1,  
                      CrossValidationRunPtr  p2
                     )
   {
     return  p1->cv->Accuracy () > p2->cv->Accuracy ();
   }
}; /* CrossValidationSortByTrainTime */






void  CrossValidationApp::RandomlySelectedResults (vector<CrossValidationRunPtr>&  crossValidations,
                                                   KKStr                          order
                                                  )
{
  *report  << "Order"      << "\t"
               << "Operation"  << "\t"
               << "Num"        << "\t"
               << "ID"         << "\t"
               << "FC"         << "\t"
               << "Features"   << "\t"
               << "Accuracy"   << "\t"
               << "TrainTime"  << "\t"
               << "ClassTime"  << "\t"
               << "SupportVectors"
               << endl;

  uint  randomPass = 0;

  for  (randomPass = 0;  randomPass < crossValidations.size ();  randomPass++)
  {
    CrossValidationRunPtr run = crossValidations[randomPass];
    CrossValidationPtr  cv       = run->cv;
    FeatureNumListPtr   features = run->features;

    *report  << order                      << "\t"
                 << " "                        << "\t"
                 << " "                        << "\t"
                 << run->id                    << "\t"
                 << features->NumOfFeatures () << "\t"
                 << features->ToString ()      << "\t"
                 << cv->Accuracy ()            << "\t"
                 << cv->TrainTimeTotal ()      << "\t"
                 << cv->TestTimeTotal ()       << "\t"
                 << cv->NumOfSupportVectors ()
                 << endl;
  }

  *report << endl;
}  /* RandomlySelectedResults */







void  CrossValidationApp::RandomlySelectedProcessResults (vector<CrossValidationRunPtr>&  crossValidations,
                                                          int                             numToCombine,
                                                          KKStr                          order
                                                         )

{
  // Perform Union and Voting of First numToCombine
  KKStr  idStr;

  FeatureNumList  unionOfFeatures (fileDesc);
  FeatureNumList  intersectOfFeatures (fileDesc);

  TrainingConfiguration2List  bestConfigs (true);

  int x;
  int  numVotingFeatures = 0;

  for  (x = 0;  x < numToCombine; x++)
  {
    CrossValidationRunPtr run = crossValidations[x];
    unionOfFeatures = unionOfFeatures + *(run->features);

    if  (intersectOfFeatures.NumOfFeatures () < 1)
      intersectOfFeatures = *(run->features);
    else
      intersectOfFeatures = intersectOfFeatures * (*(run->features));

    TrainingConfiguration2Ptr  c = new TrainingConfiguration2 (*config);
    c->SetFeatureNums (*(run->features));
    bestConfigs.PushOnBack (c);
    numVotingFeatures += run->features->NumOfFeatures ();

    if  (!idStr.Empty ())  idStr << ",";
    idStr << run->id;
  }

  config->SetFeatureNums (unionOfFeatures);

  *report  << endl << endl ;
}  /* RandomlySelectedProcessResults */









FeatureVectorListPtr  CrossValidationApp::RandomlySampleData (const   FeatureVectorListPtr data, 
                                                              float   percentToKeep, 
                                                              double& samplingTime
                                                             )
{
  double  startTime = osGetSystemTimeUsed ();

  int  numToKeep = int (float (data->QueueSize ()) * percentToKeep + 0.5);
  data->RandomizeOrder ();
  FeatureVectorListPtr  sample = new FeatureVectorList (fileDesc, false, runLog, numToKeep);

  int x;
  for  (x = 0;  x < numToKeep;  x++)
  {
    FeatureVectorPtr  i = data->IdxToPtr (x);
    sample->PushOnBack (i);
  }

  double  endTime = osGetSystemTimeUsed ();

  samplingTime = endTime - startTime;

  return sample;
}  /* RandomlySampleData */





void  CrossValidationApp::CheckForDuplicates ()
{
  bool  duplicatesWereFound = false;
  {
    DuplicateImages dups (examples, runLog);
    if  (dups.DuplicatesFound ())
    {
      duplicatesWereFound = true;

      *report << endl
                  << endl
                  << "*****************************************************************" << endl
                  << "*                  DUPLPLICATE IMAGES DETECTED                  *" << endl
                  << "*****************************************************************" << endl
                  << endl;

      *report << "File[" << examples->FileName () << "]" << endl
                  << endl
                  << "Count[" << dups.DuplicateCount () << "]" << endl
                  << endl;

      dups.ReportDuplicates (*report);

      *report << endl 
                  << endl;

      cout << endl << endl 
           << "Duplicates detected in data." << endl;

      dups.ReportDuplicates (cout);
    }
  }


  if  (validationData)
  {
    DuplicateImages  dups (validationData, runLog);
    if  (dups.DuplicatesFound ())
    {
      duplicatesWereFound = true;

      *report << endl
                  << endl
                  << "*****************************************************************" << endl
                  << "*                  DUPLPLICATE IMAGES DETECTED                  *" << endl
                  << "*****************************************************************" << endl
                  << endl;

      *report << "File[" << validationData->FileName () << "]" << endl
                  << endl;
      dups.ReportDuplicates (*report);

      *report << endl 
                  << endl;

      cout << endl << endl 
           << "Duplicates detected in validation data." << endl;

      dups.ReportDuplicates (cout);
    }
  }

  if  ((examples)  &&  (validationData))
  {
    FeatureVectorList allImages (*examples, false);
    allImages.AddQueue (*validationData);

    DuplicateImages  dups (&allImages, runLog);
    if  (dups.DuplicatesFound ())
    {
      duplicatesWereFound = true;

      *report << endl
                  << endl
                  << "*****************************************************************" << endl
                  << "*     Dupicate Images between training and test data sets       *" << endl
                  << "*****************************************************************" << endl
                  << endl;

      *report << "File[" << validationData->FileName () << "]" << endl
                  << endl;
      dups.ReportDuplicates (*report);

      *report << endl 
                  << endl;
    }
  }


  if  (duplicatesWereFound)
  {
    osWaitForEnter ();
  }

}  /* CheckForDuplicates */




void  CrossValidationApp::RandomSplitsIntoTrainAndTest (FeatureVectorListPtr   splitData, 
                                                        FeatureVectorListPtr&  splitDataTrain, 
                                                        FeatureVectorListPtr&  splitDataTest
                                                       )
{
  runLog.Level (20) << "RandomSplitsIntoTrainAndTest   Seperating into Train and Test datasets." << endl;

  splitDataTrain = new FeatureVectorList (splitData->FileDesc (), false, runLog);
  splitDataTest  = new FeatureVectorList (splitData->FileDesc (), false, runLog);

  MLClassConstListPtr  classes = splitData->ExtractMLClassConstList ();
  MLClassConstList::iterator  idx;
  for  (idx = classes->begin ();  idx != classes->end ();  idx++)
  {
    MLClassConstPtr  c = *idx;
    FeatureVectorListPtr  examplesThisClass = splitData->ExtractImagesForAGivenClass (c);
    int  numTrainExamples = (int)(0.5f + ((float)(examplesThisClass->QueueSize ()) * splitPercentage / 100.0f));
    if  (numTrainExamples < 1)
      numTrainExamples = 1;

    FeatureVectorList::iterator  idx2;
    for  (idx2 = splitData->begin ();  idx2 < splitData->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;
      if  (splitDataTrain->QueueSize () < numTrainExamples)
        splitDataTrain->PushOnBack (example);
      else
        splitDataTest->PushOnBack (example);
    }
  }
}  /* RandomSplitsIntoTrainAndTest */






void  CrossValidationApp::WriteABiasMatrix (ConfusionMatrix2&  cm)
{
  if  (configFileName.Empty ())
  {
    runLog.Level (-1) << endl
                      << "CrossValidationApp::WriteABiasMatrix   ***ERROR***   Can not generate a BiasMatrix without a ConfigurationFile name." << endl
                      << endl;
    return;
  }

  KKStr  biasFileName = osRemoveExtension (TrainingConfiguration2::GetEffectiveConfigFileName (configFileName)) + ".BiasMatrix.txt";
  ofstream f (biasFileName.Str (), ios_base::ate);

  if  (!f.is_open ())
  {
    runLog.Level (-1) << endl
                      << "CrossValidationApp::WriteABiasMatrix   ***ERROR***   Error opening BiasMatrix file[" << biasFileName << "]" << endl
                      << endl;
    return;
  }

  
  f << "<BiasMatrix>"                                                             << endl
    << "FileName"            << "\t" << biasFileName                              << endl
    << "DateTime"            << "\t" << osGetLocalDateTime ()                     << endl
    << "ConfigFileName"      << "\t" << config->FileName   ()                     << endl
    << "ConfigFileDateTime"  << "\t" << osGetFileDateTime  (config->FileName ())  << endl
    << endl;

  cm.WriteSimpleConfusionMatrix (f);

  f << "</BiasMatrix>" << endl;

  f.close ();
}  /* WriteABiasMatrix */





//**************************************************************************
//*                           The Main Module                              *
//*                                                                        *
//**************************************************************************

int  CrossValidationApp::Main (int  argc,  char** argv)
{
  ProcessCmdLineParameters (argc, argv);

  bool  successful = false;

  if  (!fileDesc)
  {
    runLog.Level (-1) << endl
      << endl
      << "main          *** ERROR ***   FileDesc == NULL" << endl
      << endl;
    osWaitForEnter ();
    return  -1; 
  }

  if  (reportFileName.Empty ())
  {
    report = &cout;
  }
  else
  {
    reportFileStream = new ofstream (reportFileName.Str ());
    if  (!reportFileStream->is_open ())
    {
      runLog.Level (-1) << "CrossValidation - Error Opening Report File["
        << reportFileName << "]."
        << endl;
      osWaitForEnter ();
      return  -1;
    }

    report = reportFileStream;
  }
    
  if  (!predictionReportName.Empty ())
  {
   
    predictionReport = new ofstream (predictionReportName.Str ());
    if  (!predictionReport->is_open ())
    {
      runLog.Level (-1) << "CrossValidation - Error Opening Preidiction File["
        << predictionReportName << "]."
        << endl;
      osWaitForEnter ();
      return  -1;
    }

    *predictionReport << "ImageFileName"  << "\t"
                      << "ImageSize"      << "\t"
                      << "KnownClass"     << "\t"
                      << "Pred1Class"     << "\t"
                      << "Pred1Prob"      << "\t"
                      << "Pred2Class"     << "\t"
                      << "Pred2Prob"
                      << endl;
  }
  
  ReportCmdLineParameters ();

  ReportConfigurationInfo ();


  startTime = osGetLocalDateTime ();

  *report << "Start Time[" << startTime << "]." << endl << endl;

  CheckForDuplicates ();

  if  (randomizeInputData)
  {
    examples->RandomizeOrder ();
    bool successful = true;
    int  imagesPerClass = config->ImagesPerClass ();
    DistributesImagesRandomlyWithInFolds (imagesPerClass, successful);
    if  (!successful)
    {
      runLog.Level (-1) << endl
                        << "Could not redistribute feature data amongst classes" << endl
                        << endl;
      osWaitForEnter ();
      return  -1;
    }
  }

  if  (false)
  {
    // Eliminate duplicates
    DuplicateImages  dupDetector (examples, runLog);
    if  (dupDetector.DuplicatesFound ())
    {
      *report << endl << endl
              << "Duplicates were found" << endl
              << endl;

      dupDetector.ReportDuplicates (*report);
      FeatureVectorListPtr  examplesToDelete = dupDetector.ListOfExamplesToDelete ();
      FeatureVectorList::iterator  idx;
      for  (idx = examplesToDelete->begin ();  idx != examplesToDelete->end ();  idx++)
      {
        FeatureVectorPtr  fv = *idx;
        examples->DeleteEntry (fv);
        if  (examples->Owner ())
          delete  fv;
      }
    }
  }

  *report << endl
          << endl
          << "Class Statistics." << endl
          << endl;

  *report << examples->ClassStatisticsStr ();

  if  (!saveFileName.Empty ())
  {
    // Save copy of Extracted data in file Specified with -s Option
    uint numExamplesWritten = 0;
    inputFormat->SaveFeatureFile (saveFileName, examples->AllFeatures (), *examples, numExamplesWritten, cancelFlag, successful, runLog);
  }

  // We are going to do A validation Run,  no need to perform CrossValidation
  if  (!validationFileName.Empty ())
  {
    *report << endl
            << endl
            << "Class Statistics for validation data." << endl
            << endl;

  *report << this->validationData->ClassStatisticsStr () << endl << endl;




    ValidationProcess ();
    return  -1;
  }

  PrintSizeHistogram ();

  int  numOfClasses = mlClasses->QueueSize ();

  int  imageCount = examples->QueueSize ();

  int  numImagesPerFold = (imageCount + numOfFolds - 1) / numOfFolds;

  int  firstInGroup = 0;

  ConfusionMatrix2  cm         (*mlClasses);  // From beginning To End.
  ConfusionMatrix2  cm2cdGuess (*mlClasses);  // From beginning To End.

  ConfusionMatrix2  cmSmall (*mlClasses);
  ConfusionMatrix2  cmMed   (*mlClasses);
  ConfusionMatrix2  cmLarge (*mlClasses);

  double*  foldAccuracies  = new double[numOfFolds];
  int*     foldCounts      = new int[numOfFolds];

  double*  foldClassTimes  = new double[numOfFolds];
  double*  foldTrainTimes  = new double[numOfFolds];

  int*     reductionPostImageCounts = new int[numOfFolds];
  int*     reductionPreImageCounts  = new int[numOfFolds];
  double*  foldReductionTimes       = new double [numOfFolds];
  int*     supportPoints            = new int[numOfFolds];

    VectorKKStr  bitReductionsByFeature (numOfFolds, "");

  int  foldNum;

  double  totalTrainingTime            = 0.0;
  double  totalClassificationTime      = 0.0;
  double  totalReductionTime           = 0.0;
  int     totalReductionPostImageCount = 0;
  int     totalReductionPreImageCount  = 0;
  int     totalSupportPoints           = 0;

  //  thirdClass = new FeatureVectorList ("C:\\TrainingApp\\TrainingImages\\trich_tufts\\Trichodesmium_tufts.data",
  //                                     RawFormat, *mlClasses, runLog, successful, true);

  for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    int  lastInGroup;

    // If We are doing the last Fold Make sure that we are including all the examples 
    // that have not been tested.
    if  (foldNum == (numOfFolds - 1))
      lastInGroup = imageCount;
    else
      lastInGroup = firstInGroup + numImagesPerFold - 1;


    *report << endl;
    *report << "Fold [" << (foldNum + 1) << "]  of  [" << numOfFolds << "]" << endl;

    FeatureVectorListPtr  trainingImages = new FeatureVectorList (fileDesc, true, runLog);

    FeatureVectorListPtr  testImages     = new FeatureVectorList (fileDesc, true, runLog);

    runLog.Level (10) << "Fold Num["        << foldNum        << "]   "
                      << "FirstTestImage["  << firstInGroup   << "]   "
                      << "LastInGroup["     << lastInGroup    << "]."
                      << endl;

    for  (int  x = 0;  x < imageCount;  x++)
    {
      FeatureVectorPtr  newImage = new FeatureVector (*(examples->IdxToPtr (x)));

      if  ((x >= firstInGroup)  &&  (x <= lastInGroup))
      {
        testImages->PushOnBack (newImage);
      }
      else
      {
        trainingImages->PushOnBack (newImage);
      }
    }

    *report << "Number Of Training Images : " << trainingImages->QueueSize () << endl;
    *report << "Number Of Test Images     : " << testImages->QueueSize ()     << endl;

    foldClassTimes [foldNum] = 0.0;
    foldTrainTimes [foldNum] = 0.0;
    reductionPostImageCounts[foldNum] = 0;
    reductionPreImageCounts[foldNum]  = 0;
    foldReductionTimes [foldNum] = 0.0;

    CrossValidate (testImages, 
                   trainingImages, 
                   cm, 
                   cm2cdGuess,
                   cmSmall, 
                   cmMed,
                   cmLarge,
                   foldCounts               [foldNum],
                   foldAccuracies           [foldNum],
                   foldNum,
                   foldTrainTimes           [foldNum],
                   foldClassTimes           [foldNum],
                   reductionPreImageCounts  [foldNum],
                   reductionPostImageCounts [foldNum],
                   foldReductionTimes       [foldNum],
                   supportPoints            [foldNum],
                   bitReductionsByFeature   [foldNum]
                  );

    totalTrainingTime            += foldTrainTimes           [foldNum];
    totalClassificationTime      += foldClassTimes           [foldNum];
    totalReductionPostImageCount += reductionPostImageCounts [foldNum];
    totalReductionPreImageCount  += reductionPreImageCounts  [foldNum];
    totalReductionTime           += foldReductionTimes       [foldNum];
    totalSupportPoints           += supportPoints            [foldNum];

    runLog.Level (10) << "TraingingTime[" << foldTrainTimes     [foldNum]       << "] "
                      << "ClassTime["     << foldClassTimes     [foldNum]       << "] "
                      << "ReductionTime[" << foldReductionTimes [foldNum]       << "] "
                      << "InitialSize["   << reductionPreImageCounts  [foldNum] << "]"
                      << "ReducedSize["   << reductionPostImageCounts [foldNum] << "]"
                      << endl;

    delete  trainingImages;    trainingImages = NULL;
    delete  testImages;        testImages     = NULL;

    firstInGroup = firstInGroup + numImagesPerFold;
  }

  float  reductionRatio = 100.0f * (float)totalReductionPostImageCount / (float)totalReductionPreImageCount;

  *report << "Cross Validation" << endl;
  *report << endl;
  *report << endl;

  *report << "Fold Accuracies" << endl;
  *report << endl;
  *report << "FoldNum"           << "\t" 
              << "Count"             << "\t" 
              << "Accuracy"          << "\t" 
              << "TrainTime"         << "\t" 
              << "ClassTime"         << "\t"
              << "ReductionTime"     << "\t"
              << "InitalImageCount"  << "\t"
              << "ReducedImageCount" << "\t"
              << "ReductionRatio"    << "\t"
              << "SupportPoints"
              << endl;

  for  (foldNum = 0;  foldNum < numOfFolds;  foldNum++)
  {
    float  reductionRatio = 100.0f * (float)reductionPostImageCounts [foldNum] / (float)reductionPreImageCounts [foldNum];

    *report << foldNum                                               << "\t"
                << foldCounts               [foldNum]                    << "\t" 
                << StrFormatDouble (foldAccuracies[foldNum], "zzz0.000") << "\t" 
                << foldTrainTimes           [foldNum]                    << "\t"
                << foldClassTimes           [foldNum]                    << "\t"
                << foldReductionTimes       [foldNum]                    << "\t"
                << reductionPreImageCounts  [foldNum]                    << "\t"
                << reductionPostImageCounts [foldNum]                    << "\t"
                << StrFormatDouble (reductionRatio, "ZZ0.00") << "%"     << "\t"
                << supportPoints            [foldNum]                    << "\t"
                << bitReductionsByFeature   [foldNum]  
                << endl;
  }

  *report << "Totals" <<                     "\t"
              <<                                 "\t"
              <<                                 "\t"
              << totalTrainingTime            << "\t"
              << totalClassificationTime      << "\t" 
              << totalReductionTime           << "\t"
              << totalReductionPreImageCount  << "\t"
              << totalReductionPostImageCount << "\t"
              << StrFormatDouble (reductionRatio, "ZZ0.00") << "%"  << "\t"
              << totalSupportPoints
              << endl;

  *report << endl;
  *report << endl;

  *report << "From Beginning To End."  << endl;
  *report << "Inclusive of all Feature Data."  << endl;
  cm.PrintConfusionMatrixTabDelimited (*report);
  *report << std::endl;
  *report << "Reduced Image Count" << "\t" << totalReductionPostImageCount << std::endl;
  *report << std::endl;

  *report <<  std::endl << std::endl << std::endl
    <<  "Confusion Matrix indicating performance of second best guess." << std::endl
    << std::endl;
  cm2cdGuess.PrintConfusionMatrixTabDelimited (*report);


  *report << endl;
  *report << "Latex Formated Confusion matrix." << endl;
  cm.PrintConfusionMatrixLatexTable (*report);
  *report << endl;
  *report << endl;
  *report << endl;

  /*
  *report << "From Beginning To End    Small (Size < " << thresholdSmall << ")." << endl;
  cmSmall.PrintConfusionMatrixTabDelimited (*report);
  *report << endl << endl;

  *report << "From Beginning To End    Medium (" << thresholdSmall << " <= Size < "<< thresholdMed << ")." << endl;
  cmMed.PrintConfusionMatrixTabDelimited (*report);
  *report << endl << endl;

  *report << "From Beginning To End    Large (Size >= "  << thresholdMed << ")." << endl;
  cmLarge.PrintConfusionMatrixTabDelimited (*report);
  *report << endl << endl;
  */


  cm.PrintErrorBySizeByRows  (*report);

  *report << endl << endl;

  cm.PrintErrorBySizeReduced (*report);

  *report << endl << endl;

  cm.PrintErrorByProbByRows  (*report);

  if  (generateBiasMatrix)
    WriteABiasMatrix (cm);

  *report << endl << endl;

  DateTime  endTime     = osGetLocalDateTime ();
  DateTime  elapsedTime = endTime - startTime;

  double   userTimeUsed   = osGetUserTimeUsed   ();
  double   kernalTimeUsed = osGetKernalTimeUsed ();

  *report << "****************************************************************************************"        << endl;
  *report << "CPU Run Time Statistics"                                                                         << endl;
  *report << "Start          Time [" << startTime      << "]."                                                 << endl;
  *report << "End            Time [" << endTime        << "]."                                                 << endl;
  *report << "Elapsed        Time [" << elapsedTime    << "],  secs [" << elapsedTime.Seconds () << "]."       << endl;
  *report << "User           Time [" << userTimeUsed   << "] secs."                                            << endl;
  *report << "Kernal         Time [" << kernalTimeUsed << "] secs."                                            << endl;
  *report << "Training       Time [" << StrFormatDouble (totalTrainingTime,       "zzz,zz0.000")  << "] secs." << endl;
  *report << "Classification Time [" << StrFormatDouble (totalClassificationTime, "zzz,zz0.000")  << "] secs." << endl;
  *report << "Reduction      Time [" << StrFormatDouble (reduction_time, "zzz,zz0.000")  << "] secs."          << endl;
  *report << "Reduction Ratio     [" << StrFormatDouble (reductionRatio, "zz0.00")<< "%"        << "]"          << endl;
  *report << "Support Points      [" << totalSupportPoints << "]" << endl;
  *report                                                                                                      << endl;
  *report << "****************************************************************************************"        << endl;
  *report << endl 
          << endl;

  if  (reportFileStream)
  {
    reportFileStream->close ();
    delete  reportFileStream;  reportFileStream = NULL;
  }

  //osWaitForEnter();

  delete[]  foldCounts;                  foldCounts               = NULL;
  delete[]  foldAccuracies;              foldAccuracies           = NULL;
  delete[]  foldClassTimes;              foldClassTimes           = NULL;
  delete[]  foldTrainTimes;              foldTrainTimes           = NULL;
  delete[]  foldReductionTimes;          foldReductionTimes       = NULL;
  delete[]  supportPoints;               supportPoints            = NULL;
  delete[]  reductionPreImageCounts;     reductionPreImageCounts  = NULL;
  delete[]  reductionPostImageCounts;    reductionPostImageCounts = NULL;

  //osWaitForEnter();
  return  0;

} /* Main */




//C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\Letter\AllClasses



void  SetUpJMLRDataSet ()
{
  RunLog  log;
  MLClassConstList  classes;

  KKStr  fileName = "ActiveLearning_MasterTestImages_c45.data";
  
  bool  successful  = false;
  bool  changesMade = false;

  bool  cancelFlag = false;

  FeatureVectorListPtr  file = FeatureFileIOC45::Driver ()->LoadFeatureFile (fileName,  classes, 999999, cancelFlag, successful, changesMade, log);
  if  (!successful)
  {
    cout << "Load of file[" << fileName << "]  Failed." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  file->RandomizeOrder ();

  FeatureVectorListPtr  testData   = new FeatureVectorList (file->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  trainData  = new FeatureVectorList (file->FileDesc (), false, log, 20000);

  int  count = 0;

  MLClassConstList::iterator  idx;
  for  (idx = classes.begin ();   idx != classes.end ();  idx++)
  {
    MLClassConstPtr  mlClass = *idx;
    FeatureVectorListPtr  imageThisClass = file->ExtractImagesForAGivenClass (mlClass, -1, -1);
    imageThisClass->RandomizeOrder ();

    int  numExamplesThisClass = 0;


    FeatureVectorList::iterator idx2;
    for (idx2 = imageThisClass->begin ();  idx2 != imageThisClass->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;

      if  (numExamplesThisClass < 1000)
         trainData->PushOnBack (example);
      else
         testData->PushOnBack (example);

      count++;
      numExamplesThisClass++;
    }

    delete  imageThisClass;  imageThisClass = NULL;
  }

  FeatureVectorListPtr    trainDataStratified = trainData->StratifyAmoungstClasses (&classes, 999999, 10);

  KKU::uint  numExamplesWritten = 0;
  FeatureFileIOC45::Driver ()->SaveFeatureFile ("JMLR-Train.data",  trainData->AllFeatures (), *trainDataStratified, numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile ("JMLR-Test.data",   testData->AllFeatures  (), *testData,            numExamplesWritten, cancelFlag, successful, log);


  TrainingConfiguration2Ptr  config 
      = TrainingConfiguration2::CreateFromFeatureVectorList (*trainData, "", log);

  config->Save ("JMLR.cfg");

  delete  trainDataStratified;
  delete  trainData;
  delete  testData;
  delete  file;
}  /* SetUpJMLRDataSet */






void  SplitUpETPdataSet ()
{
  RunLog  log;
  MLClassConstList  classes;

  KKStr  fileName = "ETP.data";
  
  bool  successful  = false;
  bool  changesMade = false;
  bool  cancelFlag  = false;

  FeatureVectorListPtr  file = FeatureFileIOPices::Driver ()->LoadFeatureFile (fileName, classes, 999999, cancelFlag, successful, changesMade, log);
  if  (!successful)
  {
    cout << "Load of file[" << fileName << "]  Failed." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  file->RandomizeOrder ();

  FeatureVectorListPtr  testData        = new FeatureVectorList (file->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  trainData       = new FeatureVectorList (file->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  validationData  = new FeatureVectorList (file->FileDesc (), false, log, 20000);

  int  count = 0;

  MLClassConstList::iterator  idx;
  for  (idx = classes.begin ();   idx != classes.end ();  idx++)
  {
    MLClassConstPtr  mlClass = *idx;
    FeatureVectorListPtr  imageThisClass = file->ExtractImagesForAGivenClass (mlClass, -1, -1);
    imageThisClass->RandomizeOrder ();


    int  totalExamples = imageThisClass->QueueSize ();

    if  (totalExamples < 100)
    {
      delete  imageThisClass;
      imageThisClass = NULL;
      continue;
    }

    int numTrainExamples      = Min (500, (int)((float)totalExamples * 0.50f + 0.5f));  // Train will consist of 50% of avaialbale examples or 500
    int numTestExamples       = Min (250, (int)((float)totalExamples * 0.25f + 0.5f));  // Test  will consist of 25% of avaialbale examples or 250
    int numTrainTestExamples  = numTrainExamples + numTestExamples;
    int numValidationExamples = totalExamples - numTrainTestExamples;

    imageThisClass->RandomizeOrder ();

    int  numExamplesThisClass = 0;

    FeatureVectorList::iterator idx2;
    for (idx2 = imageThisClass->begin ();  idx2 != imageThisClass->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;

      if  (numExamplesThisClass < numTrainExamples)
         trainData->PushOnBack (example);

      else if  (numExamplesThisClass < numTrainTestExamples)
         testData->PushOnBack (example);

      else
         validationData->PushOnBack (example);

      count++;
      numExamplesThisClass++;
    }

    delete  imageThisClass;  imageThisClass = NULL;
  }

  MLClassConstListPtr  classesThatAreLeft = trainData->ExtractMLClassConstList ();

  FeatureVectorListPtr    trainDataStratified = trainData->StratifyAmoungstClasses (classesThatAreLeft, 999999, 10);

  KKU::uint  numExamplesWritten = 0;
  FeatureFileIOPices::Driver ()->SaveFeatureFile ("ETP-Train.data",        trainData->AllFeatures       (), *trainDataStratified, numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOPices::Driver ()->SaveFeatureFile ("ETP-Test.data",         testData->AllFeatures        (), *testData,            numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOPices::Driver ()->SaveFeatureFile ("ETP-Validation.data",   validationData->AllFeatures  (), *validationData,      numExamplesWritten, cancelFlag, successful, log);

  TrainingConfiguration2Ptr  config 
      = TrainingConfiguration2::CreateFromFeatureVectorList (*trainData, "", log);

  config->Save ("ETP.cfg");

  delete  classesThatAreLeft;
  delete  trainDataStratified;
  delete  trainData;
  delete  testData;
  delete  file;
}  /* SplitUpETPdataSet */





void  SetUpLetterDataSet ()
{
  RunLog  log;
  MLClassConstList  classes;

  KKStr  file1Name = "letter-c45_80p.data";
  KKStr  file2Name = "letter-c45_20p.data";
  
  bool  successful  = false;
  bool  cancelFlag  = false;
  bool  changesMade = false;

  FeatureVectorListPtr  file1 = FeatureFileIOC45::Driver ()->LoadFeatureFile (file1Name, classes, 999999, cancelFlag, successful, changesMade, log);
  FeatureVectorListPtr  file2 = FeatureFileIOC45::Driver ()->LoadFeatureFile (file2Name, classes, 999999, cancelFlag, successful, changesMade, log);

  FeatureVectorListPtr  allExamples = new FeatureVectorList (*file1, false);
  allExamples->AddQueue (*file2);
  allExamples->RandomizeOrder ();
  
  KKU::uint  numExamplesWritten = 0;
  FeatureFileIOC45::Driver ()->SaveFeatureFile ("letter-All_c45", allExamples->AllFeatures (), *allExamples, numExamplesWritten, cancelFlag, successful, log); 
}  /* SetUpLetterDataSet */








void  SetNineClassesTrainAndTest ()
{
  RunLog  log;
  MLClassConstList  classes;

  KKStr  rootName  = "NineClasses";
  KKStr  file1Name = "NineClasses_TrainData_c45.data";
  KKStr  file2Name = "NineClasses_TestData_c45.data";

  KKStr  trainName      = rootName + "_Train.data";
  KKStr  testName       = rootName + "_Test.data";
  KKStr  trainTestName  = rootName + "_TrainTest.data";
  KKStr  validationName = rootName + "_Validation.data";
  
  bool  successful  = false;
  bool  cancelFlag  = false;
  bool  changesMade = false;

  FeatureVectorListPtr  file1 = FeatureFileIOC45::Driver ()->LoadFeatureFile (file1Name, classes, 999999, cancelFlag, successful, changesMade, log);
  FeatureVectorListPtr  file2 = FeatureFileIOC45::Driver ()->LoadFeatureFile (file2Name, classes, 999999, cancelFlag, successful, changesMade, log);

  FeatureVectorListPtr  allExamples = new FeatureVectorList (*file1, false);
  allExamples->AddQueue (*file2);
  allExamples->RandomizeOrder ();

  FeatureVectorListPtr  testData       = new FeatureVectorList (allExamples->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  trainData      = new FeatureVectorList (allExamples->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  trainTestData  = new FeatureVectorList (allExamples->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  validationData = new FeatureVectorList (allExamples->FileDesc (), false, log, 20000);

  int  count = 0;

  MLClassConstList::iterator  idx;
  for  (idx = classes.begin ();   idx != classes.end ();  idx++)
  {
    MLClassConstPtr  mlClass = *idx;
    FeatureVectorListPtr  imageThisClass = allExamples->ExtractImagesForAGivenClass (mlClass, -1, -1);

    int  numTrainImages = (int)(0.5f + (float)imageThisClass->QueueSize () / 2.0f);
    int  numTestImages  = (int)(0.5f + (float)imageThisClass->QueueSize () / 4.0f);
    int  numTrainTestImages = numTrainImages + numTestImages;

    int imagesThisClass = 0;
    FeatureVectorList::iterator idx2;
    for (idx2 = imageThisClass->begin ();  idx2 != imageThisClass->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;

      if  (imagesThisClass > numTrainTestImages)
      {
        validationData->PushOnBack (example);
      }

      else 
      {
        trainTestData->PushOnBack (example);
        if  (imagesThisClass > numTrainImages)
          testData->PushOnBack (example);
        else
          trainData->PushOnBack (example);
      }

      if  ((count % 1000) == 0)
      {
        cout << count << "\t"
             << trainData->QueueSize      () << "\t"
             << testData->QueueSize       () << "\t"
             << trainTestData->QueueSize  () << "\t"
             << validationData->QueueSize ()
             << endl;
      }

      imagesThisClass++;
      count++;
    }

    delete  imageThisClass;  imageThisClass = NULL;
  }

  FeatureVectorListPtr    trainDataStratified = trainData->StratifyAmoungstClasses (&classes, 999999, 10);

  KKU::uint  numExamplesWritten = 0;
  FeatureFileIOC45::Driver ()->SaveFeatureFile (trainName,      trainData->AllFeatures      (), *trainDataStratified, numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile (testName,       testData->AllFeatures       (), *testData,            numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile (trainTestName,  trainTestData->AllFeatures  (), *trainTestData,       numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile (validationName, validationData->AllFeatures (), *validationData,      numExamplesWritten, cancelFlag, successful, log);

  delete  trainDataStratified;
  delete  trainData;
  delete  testData;
  delete  validationData;
  delete  trainTestData;
  delete  allExamples;
  delete  file1;
  delete  file2;
}  /* SetNineClassesTrainAndTest */








void  SplitUpForestData ()
{
  // C:\bigpine\Plankton\Papers\BinaryFeatureSelection\Experiments\ForestCover

  RunLog  log;
  MLClassConstList  classes;

  KKStr  fileName       = "covtype.data";
  KKStr  trainName      = "CovType-Train.data";
  KKStr  testName       = "CovType-Test.data";
  KKStr  trainTestName  = "CovType-TrainTest.data";
  KKStr  validationName = "CovType-Validation.data";
  
  bool  successful  = false;
  bool  cancelFlag  = false;
  bool  changesMade = false;

  FeatureVectorListPtr  file = FeatureFileIOC45::Driver ()->LoadFeatureFile (fileName, classes, 999999, cancelFlag, successful, changesMade, log);
  if  (!successful)
  {
    cout << "Load of file[" << fileName << "]  Failed." << endl;
    osWaitForEnter ();
    exit (-1);
  }

  file->RandomizeOrder ();

  FeatureVectorListPtr  testData       = new FeatureVectorList (file->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  trainData      = new FeatureVectorList (file->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  trainTestData  = new FeatureVectorList (file->FileDesc (), false, log, 20000);
  FeatureVectorListPtr  validationData = new FeatureVectorList (file->FileDesc (), false, log, 20000);

  int  count = 0;

  MLClassConstList::iterator  idx;
  for  (idx = classes.begin ();   idx != classes.end ();  idx++)
  {
    MLClassConstPtr  mlClass = *idx;
    FeatureVectorListPtr  imageThisClass = file->ExtractImagesForAGivenClass (mlClass, -1, -1);

    int imagesThisClass = 0;
    FeatureVectorList::iterator idx2;
    for (idx2 = imageThisClass->begin ();  idx2 != imageThisClass->end ();  idx2++)
    {
      FeatureVectorPtr  example = *idx2;

      if  (imagesThisClass > 1499)
      {
        validationData->PushOnBack (example);
      }
      else
      {
        trainTestData->PushOnBack (example);
        if  (imagesThisClass < 1000)
          trainData->PushOnBack (example);
        else
          testData->PushOnBack (example);
      }

      if  ((count % 2000) == 0)
      {
        cout << count << "\t"
             << trainData->QueueSize      () << "\t"
             << testData->QueueSize       () << "\t"
             << trainTestData->QueueSize  () << "\t"
             << validationData->QueueSize ()
             << endl;
      }

      imagesThisClass++;
      count++;
    }

    delete  imageThisClass;  imageThisClass = NULL;
  }

  FeatureVectorListPtr    trainDataStratified = trainData->StratifyAmoungstClasses (&classes, 999999, 10);

  KKU::uint  numExamplesWritten = 0;
  FeatureFileIOC45::Driver ()->SaveFeatureFile (trainName,      trainData->AllFeatures      (), *trainDataStratified, numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile (testName,       testData->AllFeatures       (), *testData,            numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile (trainTestName,  trainTestData->AllFeatures  (), *trainTestData,       numExamplesWritten, cancelFlag, successful, log);
  FeatureFileIOC45::Driver ()->SaveFeatureFile (validationName, validationData->AllFeatures (), *validationData,      numExamplesWritten, cancelFlag, successful, log);

  delete  trainDataStratified;
  delete  trainData;
  delete  testData;
  delete  validationData;
  delete  trainTestData;
  delete  file;
}   /* SplitUpForestData */



#include  "ModelParamDual.h"

void  CreateBfsFromMfs (const KKStr&  mfsFileName,
                        const KKStr&  srcBfs1Name,
                        const KKStr&  srcBfs2Name,
                        const KKStr&  srcBfs3Name,
                        const KKStr&  srcBfs4Name,
                        const KKStr&  srcBfs5Name
                       )
{
  RunLog  log;

  FileDescPtr   fileDesc = FeatureFileIOPices::NewPlanktonFile (log);
  TrainingConfiguration2Ptr  mfsConfig 
    = new TrainingConfiguration2 (fileDesc, mfsFileName, log, false);

  TrainingConfiguration2Ptr  newBfsConfig = new TrainingConfiguration2 (*mfsConfig);

  TrainingConfiguration2Ptr  srcBfs1Config = new TrainingConfiguration2 (fileDesc, srcBfs1Name, log, false);
  TrainingConfiguration2Ptr  srcBfs2Config = new TrainingConfiguration2 (fileDesc, srcBfs2Name, log, false);
  TrainingConfiguration2Ptr  srcBfs3Config = new TrainingConfiguration2 (fileDesc, srcBfs3Name, log, false);
  TrainingConfiguration2Ptr  srcBfs4Config = new TrainingConfiguration2 (fileDesc, srcBfs4Name, log, false);
  TrainingConfiguration2Ptr  srcBfs5Config = new TrainingConfiguration2 (fileDesc, srcBfs5Name, log, false);

  MLClassConstListPtr  classes = mfsConfig->ExtractClassList ();
  classes->SortByName ();

  MLClassConstListPtr  classes1 = srcBfs1Config->ExtractClassList ();
  classes1->SortByName ();

  MLClassConstListPtr  classes2 = srcBfs2Config->ExtractClassList ();
  classes2->SortByName ();

  MLClassConstListPtr  classes3 = srcBfs3Config->ExtractClassList ();
  classes3->SortByName ();

  MLClassConstListPtr  classes4 = srcBfs4Config->ExtractClassList ();
  classes4->SortByName ();

  MLClassConstListPtr  classes5 = srcBfs4Config->ExtractClassList ();
  classes5->SortByName ();

  ofstream   missingClasses ("C:\\Temp\\MissingClasses.txt");
  missingClasses << endl << endl;
  missingClasses << "============================================================================" << endl;
  for  (int32  idx1 = 0; idx1 < (classes->QueueSize () - 1);  ++idx1)
  {
    MLClassConstPtr  c1 = classes->IdxToPtr (idx1);

    if  ((classes1->PtrToIdx (c1) < 0)  &&  (classes2->PtrToIdx (c1) < 0)  &&  (classes3->PtrToIdx (c1) < 0)  &&  (classes4->PtrToIdx (c1) < 0)  )
    {
      missingClasses << c1->Name ();
    }

    for  (int32  idx2 = idx1 + 1; idx2 < classes->QueueSize ();  ++idx2)
    {
      MLClassConstPtr c2 = classes->IdxToPtr (idx2);

      BinaryClassParmsPtr  bcp = srcBfs1Config->GetBinaryClassParms (c1, c2);
      if  (bcp == NULL)
      {
        bcp = srcBfs2Config->GetBinaryClassParms (c1, c2);
        if  (bcp == NULL)
        {
           bcp = srcBfs3Config->GetBinaryClassParms (c1, c2);
           if  (bcp == NULL)
           {
             bcp = srcBfs4Config->GetBinaryClassParms (c1, c2);
             if  (bcp == NULL)
             {
               bcp = srcBfs5Config->GetBinaryClassParms (c1, c2);
             }
           }
        }
      }

      if  (bcp)
      {
        newBfsConfig->SetBinaryClassFields (c1, c2, bcp->Param (), bcp->SelectedFeatures (), bcp->Weight ());
      }
      else
      {
        missingClasses << "Pair Missing" << "\t" << c1->Name () << "\t" << c2->Name () << endl;
      }
    }
  }

  bool  valid = false;
  newBfsConfig->MachineType (BinaryCombos);
  newBfsConfig->SelectionMethod (SelectByProbability);
  newBfsConfig->SetFeatureNums (FeatureNumList (fileDesc, "0-84", valid));

  KKStr  baseName = osRemoveExtension (mfsFileName);
  KKStr  tail = mfsFileName.Tail (8);
  if  (tail.EqualIgnoreCase ("_MFS.cfg"))
    baseName = mfsFileName.SubStrPart (0, mfsFileName.Len () - 9);

  KKStr  newBfsFileName  = baseName + "_BFS.cfg";
  KKStr  newDualFileName = baseName + "_Dual.cfg";

  newBfsConfig->Save (newBfsFileName);

  ModelParamDualPtr dualModelParms = new ModelParamDual (fileDesc, osGetRootNameWithExtension (mfsFileName), osGetRootNameWithExtension (newBfsFileName), true, log);
  dualModelParms->OtherClass (MLClass::CreateNewMLClass ("Other", -1));
  
  TrainingConfiguration2Ptr  newDualConfig = new TrainingConfiguration2 (fileDesc, classes, dualModelParms, log);
  newDualConfig->SetFeatureNums (FeatureNumList (fileDesc, "0-84", valid));
  newDualConfig->ExamplesPerClass (mfsConfig->ExamplesPerClass ());
  newDualConfig->RootDir (mfsConfig->RootDir ());
  newDualConfig->Save (newDualFileName);

  delete  newBfsConfig;   newBfsConfig = NULL;
  delete  mfsConfig;      mfsConfig    = NULL;
  delete  srcBfs1Config;  srcBfs1Config = NULL;
  delete  srcBfs2Config;  srcBfs2Config = NULL;
  delete  srcBfs3Config;  srcBfs3Config = NULL;
  delete  srcBfs4Config;  srcBfs4Config = NULL;
  delete  srcBfs5Config;  srcBfs5Config = NULL;
} /* CreateBfsFromMfs */
     


#include  "DataBase.h"




void  MarineSnowReportDeployment (SipperDeploymentPtr  deployment,
                                  DataBase&            db
                                 )
{
  if  (!deployment)
    return;

  cout << deployment->CruiseName () << "\t" << deployment->StationName () << "\t" << deployment->DeploymentNum () << endl;


  // 1)Determines Midpoint of deploymet from Instruments table.
  KKStr  sqlStr = "Call ImagesSizeDataByDepthSipper8(" + deployment->CruiseName ().QuotedStr () + "," + deployment->StationName ().QuotedStr () + "," + deployment->DeploymentNum ().QuotedStr () +",1.0)";

  VectorKKStr  columnNames;
  KKStrMatrixPtr results = db.QueryStatementReturnAllColumns (sqlStr.Str (), sqlStr.Len (), columnNames);
  if  (results == NULL)
  {
    cout << "No results returend" << endl;
    return;
  }

  int32  numRows = results->NumRows ();
  int32  numCols = results->NumCols ();
  int32  numCountCols = numCols - 8;

  if  (numRows == 0)
  {
    delete  results;
    results = NULL;
    return;
  }

  VectorKKStr  countHeader (numCountCols);
  for  (int32 x = 0;  x < numCountCols;  ++x)
  {
    countHeader[x] = columnNames[8 + x];
  }

  bool  cancelFlag = false;
  InstrumentDataMeansListPtr  instData = db.InstrumentDataBinByMeterDepth (deployment->CruiseName (), deployment->StationName (), deployment->DeploymentNum (), 1.0f, cancelFlag);
  if  (!instData)
  {
    cout << "no Instrument data." << endl << endl;
    return;
  }


  KKStr  reportRootName = deployment->CruiseName () + "-" + deployment->StationName () + "-" + deployment->DeploymentNum ();
  KKStr  reportFileRootName1 = "C:\\Temp\\MarineSnow\\" + reportRootName + ".txt";

  ofstream  r1 (reportFileRootName1.Str ());

  r1 << "Cruise"     << "\t" << deployment->CruiseName    () << endl
     << "Station"    << "\t" << deployment->StationName   () << endl
     << "Deployment" << "\t" << deployment->DeploymentNum () << endl
     << "DateTime"   << "\t" << osGetLocalDateTime ()        << endl
     << "DataBase"   << "\t" << db.ServerDescription ()      << endl
     << "ProgName"   << "\t" << osGetProgName ()             << endl
     << "HostName"   << "\t" << osGetHostName ()             << endl
     << "UserName"   << "\t" << osGetUserName ()             << endl
     << endl
     << endl;

  KKStr  h1 (1024);
  KKStr  h2 (1024);

  h1 << ""        << "\t" << "Depth"  << "\t" << "VolumeSampled" << "\t" << "Image"     << "\t"  << "Pixel";
  h2 << "Down/Up" << "\t" << "(m)"    << "\t" << "m^3"           << "\t" << "Abundance" << "\t"  << "Count";

  for  (int32 c = 0;  c < numCountCols;  ++c)
  {
    h1 << "\t" << "";
    h2 << "\t" << countHeader[c];
  }

  KKStr  temperatureUOM   = InstrumentData::TemperatureUnit   ();
  KKStr  salinityUOM      = InstrumentData::SalinityUnit      ();
  KKStr  denisityUOM      = InstrumentData::DensityUnit       ();
  KKStr  fluorescenceUOM  = InstrumentData::FluorescenceUnit  ();
  KKStr  oxygenUOM        = InstrumentData::OxygenUnit        ();
  KKStr  transmisivityUOM = InstrumentData::TransmisivityUnit ();
  KKStr  turbidityUOM     = InstrumentData::TurbidityUnit     ();

  h1 << "\t" << "Temperature"   << "\t" << "Salinity"  << "\t" << "Denisity"  << "\t" << "Fluorescence"  << "\t" << "Fluorescence-Sensor" << "\t" << "Oxygen"  << "\t" << "Oxygen"  << "\t" << "transmisivity"  << "\t" << "turbidity";
  h2 << "\t" << temperatureUOM  << "\t" << salinityUOM << "\t" << denisityUOM << "\t" << fluorescenceUOM << "\t" << "Volts"               << "\t" << oxygenUOM << "\t" << "umol/kg" << "\t" << transmisivityUOM << "\t" << turbidityUOM;

  r1 << endl << endl
     << "Abundance / m^3" << endl
     << endl
     << h1 << endl
     << h2 << endl;

  for  (int32 rowIdx = 0;  rowIdx < numRows;  ++rowIdx)
  {
    KKStrList&  row = (*results)[rowIdx];

    bool  downCast     = row[3].ToBool  ();
    int32  bucketIdx   = row[4].ToInt32 ();
    float  bucketDepth = row[5].ToFloat ();
    int32  imageCount  = row[6].ToInt32 ();
    int32  pixelCount  = row[7].ToInt32 ();

    VectorInt32  counts (numCountCols, 0);
    for  (int32 x = 0;  x < numCountCols;  ++x)
      counts[x] = row[8 + x].ToInt32 ();

    float  volumeSampled = 0.0f;
    InstrumentDataMeansPtr  idm = instData->LookUp (downCast, bucketDepth);
    if  (idm)
      volumeSampled = idm->volumeSampled;

    KKStr  begOfLine (128);
    begOfLine << (downCast ? "Down" : "Up") << "\t" << bucketDepth << "\t" << volumeSampled << "\t" << imageCount << "\t" << pixelCount;

    r1 << begOfLine;

    for  (int32 x = 0;  x < numCountCols;  ++x)
    {
      float  density = 0.0f;
      if  (volumeSampled != 0.0)
        density = (float)counts[x] / volumeSampled;
      r1 << "\t" << density;
    }

    if  (idm)
    {
      KKStr  instDataStr (256);

      instDataStr << "\t" << idm->temperatureMean
                  << "\t" << idm->salinityMean 
                  << "\t" << idm->denisityMean
                  << "\t" << idm->fluorescenceMean
                  << "\t" << idm->fluorescenceSensorMean
                  << "\t" << idm->oxygenMean
                  << "\t" << idm->Oxygen_molPerKg_Mean ()
                  << "\t" << idm->transmisivityMean 
                  << "\t" << idm->turbidityMean;

      r1 << instDataStr;
    }

    r1 << endl;
  }


  r1 << endl << endl << endl
     << "Abundance" << endl
     << endl;

  r1 << h1 << endl;
  r1 << h2 << endl;

  for  (int32 rowIdx = 0;  rowIdx < numRows;  ++rowIdx)
  {
    KKStrList&  row = (*results)[rowIdx];

    bool  downCast     = row[3].ToBool  ();
    int32  bucketIdx   = row[4].ToInt32 ();
    float  bucketDepth = row[5].ToFloat ();
    int32  imageCount  = row[6].ToInt32 ();
    int32  pixelCount  = row[7].ToInt32 ();

    VectorInt32  counts (numCountCols, 0);
    for  (int32 x = 0;  x < numCountCols;  ++x)
      counts[x] = row[8 + x].ToInt32 ();

    float  volumeSampled = 0.0f;
    InstrumentDataMeansPtr  idm = instData->LookUp (downCast, bucketDepth);
    if  (idm)
      volumeSampled = idm->volumeSampled;

    KKStr  begOfLine (128);
    begOfLine << (downCast ? "Down" : "Up") << "\t" << bucketDepth << "\t" << volumeSampled << "\t" << imageCount << "\t" << pixelCount;

    r1 << begOfLine;

    for  (int32 x = 0;  x < numCountCols;  ++x)
      r1 << "\t" << counts[x];

    if  (idm)
    {
      KKStr  instDataStr (256);

      instDataStr << "\t" << idm->temperatureMean
                  << "\t" << idm->salinityMean 
                  << "\t" << idm->denisityMean
                  << "\t" << idm->fluorescenceMean
                  << "\t" << idm->fluorescenceSensorMean
                  << "\t" << idm->oxygenMean
                  << "\t" << idm->Oxygen_molPerKg_Mean ()
                  << "\t" << idm->transmisivityMean 
                  << "\t" << idm->turbidityMean;

      r1 << instDataStr;
    }

    r1 << endl;
  }


  delete  instData;  instData = NULL;
  delete  results;   results  = NULL; 
}  /* MarineSnowReportDeployment */




void  MarineSnowReport ()
{
  RunLog  runLog;
  DataBasePtr  db = new DataBase (runLog);

  SipperDeploymentListPtr  deployments = db->SipperDeploymentLoad ("","");

  SipperDeploymentList::const_iterator  idx;
  for  (idx = deployments->begin ();  idx != deployments->end ();  ++idx)
  {
    MarineSnowReportDeployment (*idx, *db);
  }

  delete  db;
  db = NULL;
}  /* MarineSnowReport */




void  DetermineCropSettingsForDeployment (RunLog&              runLog,
                                          DataBasePtr          db, 
                                          SipperDeploymentPtr  deployment
                                         )
{
  KKStr  cruiseName    = deployment->CruiseName    ();
  KKStr  stationName   = deployment->StationName   ();
  KKStr  deploymentNum = deployment->DeploymentNum ();
  runLog.Level (10) << endl
    << "Cruise: " << cruiseName << "   Station: " << stationName << "    DeploymentNum: " << deploymentNum << endl;
  
  SipperFileListPtr  sipperFiles = db->SipperFileLoad (cruiseName, stationName,  deploymentNum);
  if  ((sipperFiles == NULL)  ||  (sipperFiles->size () < 1))
  {
    runLog.Level (-1) 
      << endl
      << "DetermineCropSettingsForDeployment     Cruise: " << cruiseName << "  Station: " << stationName << "  Deployment: " << deploymentNum 
      << "   No Sippler Files defined in database." << endl
      << endl;
    delete  sipperFiles;
    sipperFiles = NULL;
    return;
  }

  KKStr  reportDir = "C:\\Temp\\DetermineCtropSettings";

  KKU::osCreateDirectoryPath (reportDir);

  vector<uint32>  cropLefts;
  vector<uint32>  cropRights;

  SipperFileList::iterator  idx;
  for  (idx =  sipperFiles->begin ();  idx != sipperFiles->end ();  ++idx)
  {
    uint32  cropLeft = 0;
    uint32  cropRight = 4095;

    SipperFilePtr  sipperFile = *idx;

    KKStr  sipperFileRootName = sipperFile->SipperFileName ();
    KKStr  sipperFileName = InstrumentDataFileManager::GetFullSipperFileName (sipperFileRootName);

    if  (sipperFileName.Empty ())
      continue;

    InstrumentDataManagerPtr  instrumentDataManager = new InstrumentDataManager (sipperFileName, sipperFile, reportDir, runLog);

    SipperBuffPtr  sb = SipperBuff::CreateSipperBuff (sipperFileName, 0, instrumentDataManager, runLog);

    if  (sb)
    {
      sb->DetermineCropSettings (cropLeft, cropRight);
      runLog.Level (10) << sipperFile->SipperFileName () << "\t" << cropLeft << "\t" << cropRight << endl;
      cropLefts.push_back (cropLeft);
      cropRights.push_back (cropRight);
    }

    delete  sb;                     sb                    = NULL;
    delete  instrumentDataManager;  instrumentDataManager = NULL;
  }

  if  (cropLefts.size () > 0)
  {
    int  midPoint = cropLefts.size () / 2;
    sort (cropLefts.begin  (), cropLefts.end  ());
    sort (cropRights.begin (), cropRights.end ());

    deployment->CropLeft  (cropLefts[midPoint]);
    deployment->CropRight (cropRights[midPoint]);

    db->SipperDeploymentUpdate (*deployment);
  }

  delete  sipperFiles;
  sipperFiles = NULL;
}  /* DetermineCropSettingsForDeployment */




void  DetermineCropSettings ()
{
  RunLog  runLog;
  DataBasePtr  db = new DataBase (runLog);

  SipperCruiseListPtr   cruises = db->SipperCruiseLoadAllCruises ();
  if  (cruises == NULL)
  {
    runLog.Level (-1) 
      << endl
      << "DetermineCropSettings   ***ERROR***    No Cruise Entries found in database." << endl
      << endl;
    exit (-1);
  }

  ofstream  cropSettings ("C:\\Temp\\CropSettings.txt");

  SipperCruiseList::iterator  idx;
  for  (idx = cruises->begin ();  idx != cruises->end ();  ++idx)
  {
    SipperCruisePtr  cruise = *idx;
    KKStr  cruiseName = cruise->CruiseName ();

    runLog.Level (10) << endl
      << "Cruise: " << cruiseName << endl;

    SipperStationListPtr stations = db->SipperStationLoad (cruiseName);

    if  (stations != NULL)
    {
      SipperStationList::iterator  idx2;
      for  (idx2 = stations->begin ();  idx2 != stations->end ();  ++idx2)
      {
        SipperStationPtr  station = *idx2;
        KKStr  stationName = station->StationName ();

        runLog.Level (10) << endl
          << "Cruise: " << cruiseName << "   Station: " << stationName << endl;

        SipperDeploymentListPtr  deployments = db->SipperDeploymentLoad (cruiseName, stationName);
        if  (deployments != NULL)
        {
          SipperDeploymentList::iterator  idx3;
          for  (idx3 = deployments->begin ();  idx3 != deployments->end ();  ++idx3)
          {
            SipperDeploymentPtr  deployment = *idx3;
            DetermineCropSettingsForDeployment (runLog, db, deployment);
            int32  cropWidth = 1 + deployment->CropRight () - deployment->CropLeft ();
            cropSettings << cruiseName                   << "\t" 
                         << stationName                  << "\t" 
                         << deployment->DeploymentNum () << "\t" 
                         << deployment->DateTimeStart () << "\t" 
                         << deployment->CropLeft      () << "\t" 
                         << deployment->CropRight     () << "\t" 
                         << cropWidth << endl;
          }
          delete  deployments;
          deployments = NULL;
        }
      }

      delete  stations;
      stations = NULL;
    }
  }

  delete  cruises;
  cruises = NULL;


  delete  db;
  db = NULL;

}  /* DetermineCropSettings */




int  main (int    argc, 
           char** argv
          )
{

  #if  defined (WIN32)  &&  defined (_DEBUG)  &&  !defined(_NO_MEMORY_LEAK_CHECK_)
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
  #endif

  if  (false)
  {
    MarineSnowReport ();
    exit (-1);
  }


  if  (false)
  {
    DetermineCropSettings ();
    exit (-1);
  }


  if  (false)
  {
    //CreateBfsFromMfs ("E:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_Discreate5_MFS.cfg",
    //                  "E:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_Discreate_BFS.cfg",
    //                  "E:\\Pices\\DataFiles\\TrainingModels\\NRDA_small_BFS.cfg",
    //                  "E:\\Pices\\DataFiles\\TrainingModels\\Gulf_2011_BFS.cfg"
    //                 );

    //CreateBfsFromMfs ("D:\\Users\\kkramer\\DropBox\\Dropbox\\Sipper\\TrainModelsToCreateBfsFor\\GulfOilBroad_130314_MFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_Discreate_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\NRDA_small_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_Discreate5_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\Gulf_2011_BFS.cfg"
    //                 );

    //CreateBfsFromMfs ("D:\\Users\\kkramer\\DropBox\\Dropbox\\Sipper\\TrainModelsToCreateBfsFor\\2013-03-18_TwoNewBFSToUtilizeRecentlyTuunedParms\\NRDA_SMP751001_37_MFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_Discreate_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\NRDA_small_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad2_Discreate5_BFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\Gulf_2011_BFS.cfg"
    //                 );

    //CreateBfsFromMfs ("E:\\Users\\kkramer\\Dropbox\\Sipper\\TrainModelsToCreateBfsFor\\2013-03-18_TwoNewBFSToUtilizeRecentlyTuunedParms\\NRDA_GG1002_37_MFS.cfg",
    //                  "E:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
    //                  "",
    //                  "",
    //                  "",
    //                  ""
    //                 );

    //CreateBfsFromMfs ("D:\\Users\\kkramer\\DropBox\\Dropbox\\Sipper\\TrainModelsToCreateBfsFor\\2013-03-22\\NRDA_SD0102_MFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
    //                  "GulfOilBroad2_Discreate_OrigTuned_BFS.cfg",
    //                  "",
    //                  "",
    //                  ""
    //                 );


    //2014-05-14  kurt kramer
    // At andrews request creating mfs, bfs and dual classifier for specied list of classes.
    //CreateBfsFromMfs ("E:\\Pices\\DataFiles\\TrainingModels\\GulfBroad_2014-05-16_MFS.cfg",
    //                  "E:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
    //                  "GulfOilBroad2_Discreate_OrigTuned_BFS.cfg",
    //                  "Oil_BFS",
    //                  "",
    //                  ""
    //                 );


    //2014-05-14  kurt kramer
    // At andrews request creating mfs, bfs and dual classifier for specied list of classes.
    //CreateBfsFromMfs ("D:\\Pices\\DataFiles\\TrainingModels\\GulfBroad_2014-06-27_MFS.cfg",
    //                  "D:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
    //                  "Oil_BFS",
    //                  "",
    //                  ""
    //                 );



    //CreateBfsFromMfs ("E:\\Pices\\DataFiles\\TrainingModels\\GulfBroad_2014-06-10_MFS.cfg",
    //                  "E:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
    //                  "GulfOilBroad2_Discreate_OrigTuned_BFS.cfg",
    //                  "Oil_BFS",
    //                  "",
    //                  ""
    //                 );


    // 2014-07-06
    // Meanth to be master model containg all classes;  andrew will reduce from this point.
    CreateBfsFromMfs ("E:\\Pices\\DataFiles\\TrainingModels\\USF_MFS.cfg",
                      "E:\\Pices\\DataFiles\\TrainingModels\\GulfOilBroad_130314_BFS.cfg",
                      "GulfOilBroad2_Discreate_OrigTuned_BFS.cfg",
                      "Oil_BFS",
                      "",
                      ""
                     );

    exit (-1);
  }


  //SplitUpForestData ();
  //  SetUpJMLRDataSet ();
  //  exit (-1);
  //   SplitUpETPdataSet ();
  //   exit (-1);

  //Used the following code to help debug 'LoadFeatureDataFromTrainingLibraries'
  //RunLog  runLog;
  //FileDescPtr  fd = FileDesc::NewPlanktonFile (runLog);
  //bool  validateDirs = true;
  //TrainingConfiguration2Ptr  config = new TrainingConfiguration2 (fd, "WFS.cfg", runLog, true);
  //DateTime   dt;
  //bool       cm = false;
  //bool       cf = false;
  //FeatureVectorListPtr  fvl = config->LoadFeatureDataFromTrainingLibraries (dt, cm, cf);

  CrossValidationAppPtr  crossValidationApp = new CrossValidationApp ();
  crossValidationApp->Main (argc, argv);
  delete  crossValidationApp;
  crossValidationApp = NULL;

  //InstrumentDataFileManager::FinalCleanUp ();
  //FeatureFileIO::FinalCleanUp ();
  //FileDesc::FinalCleanUp ();
  //Instrument::FinalCleanUp ();
  //MLClass::FinalCleanUp ();

  return 0;
}

