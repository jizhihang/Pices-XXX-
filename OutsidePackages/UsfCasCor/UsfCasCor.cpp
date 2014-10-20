/*
   Cascade Correlation Neural Network

   See notes below for original author, etc.

   Changes made by Steven Eschrich <eschrich@csee.usf.edu>
   Fall, 2001

 - The code was heavily modified in the I/O portion to follow
   C4.5 standard data formats (although currently only real-valued
   attributes are supported). It parses the .names file for classes
   (1 of N representation) and feature values (ignore or real). The
   logic simply treats all non-ignore fields as real so integer labels
   are treated as continuous.

 - All network parameters can be passed via command line arguments,
   try cascor -h for details. Briefly,
     -i #    input epochs
     -o #    output epochs
     -r #    number of new units to add
     -P      make predictions. generates a filestem.pred file with class
       names as predictions (from a .test file).
     -N      normalize features to 0-1
     -f filestem   Filestem for data
     -R      use training set (resubstitution)

   All other parameters that could be specified in the .net file are
   specified as
     -O option=value    (e.g. -O UseCache=True)

 - Defaults are used for parameters, in case someone starts it without
   any...

 - Support for index files (-I option). That is, the following files
   are supposed to exist
     filestem.data
     filestem.idata
     filestem.itest (-P only)
     filestem.names

   The .idata and .itest are text files with the index number of the record
   in .data desired. The .data file **must** be in fixed-width format, each
   line padded to the same length with spaces.
*/
/****************************************************************************/
/* C implementation of the Cascade-Correlation learning algorithm.          */
/*                                                                          */
/*   Written by:          R. Scott Crowder, III                             */
/*                        School of Computer Science                        */
/*                        Carnegie Mellon University                        */
/*                        Pittsburgh, PA 15213-3890                         */
/*                                                                          */
/*                        Phone: (412) 268-8139                             */
/*        Internet:  rsc@cs.cmu.edu                                         */
/*                                                                          */
/*                                                                          */
/*  This code has been placed in the public domain by the author.  As a     */
/*  matter of simple courtesy, anyone using or adapting this code is        */
/*  expected to acknowledge the source.  The author would like to hear      */
/*  about any attempts to use this system, successful or not.               */
/*                                                                          */
/*  This code is a port to C from the original Common Lisp implementation   */
/*  written by Scott E. Fahlman.  (Version dated June 1 1990.)              */
/*                                                                          */
/*  For an explanation of this algorithm and some results, see "The         */
/*  Cascade-Correlation Learning Architecture" by Scott E. Fahlman and      */
/*  Christian Lebiere in D. S. Touretzky (ed.), "Advances in Neural         */
/*  Information Processing Systems 2", Morgan Kaufmann, 1990.  A somewhat   */
/*  longer version is available as CMU Computer Science Tech Report         */
/*  CMU-CS-90-100.  Instructions for Ftping this report are given at the    */
/*  end of this file.                                                       */
/*                                                                          */
/*  An example of the network set up file is provided at the bottom of      */
/*  this file.                                                              */
/*                                                                          */
/*  This code has been successfully compiled on the following machines.     */
/*                                                                          */
/*  DEC Station 3100 using the MIPS compiler version 1.31                   */
/*  Sun 4 using the gcc compiler version 1.23                               */
/*  IBM PC-RT  using the cc compiler                                        */
/*  IBM RS6000 (Model 520) using the xlc compiler                           */
/*  386 machine using the Turbo C 2.0 compiler                              */
/*  The implementation compiles with the ANSI standard.  Some machine       */
/*  specific preprocessor commands are required.  It is assumed that your   */
/*  system will provide the required preprocessor arguments.                */
/*                                                                          */
/****************************************************************************/
/* Change Log                                                               */
/****************************************************************************/
/*                                                                          */
/* Changes from Release 1 dated Jun-12-90 to Version 1.14 Jul-18-90         */
/*                                                                          */
/*  bug fix in TYPE_CONVERT  Thanks to Michael Witbrock for the 1st report  */
/*  bug fix in BUILD_NET     Thanks to Michael Witbrock for the 1st report  */
/*  bug fix in GET_ARRAY_ELEMENT       Thanks to Ken Lang                   */
/*  bug fix in COMPUTE_CORRELATIONS    Thanks to Eric Melz                  */
/*  bug fix in ADJUST_CORRELATIONS     Thanks to Chris Lebiere              */
/*  bug fix in COMPUTE_SLOPES          Thanks to Chris Lebiere              */
/*  removed 2nd call to INIT_GLOBALS   Thanks to Dimitris Michailidis       */
/*  Added UnitType ASYMSIGMOID for users who like sigmoids to go from 0-1   */
/*     all learning utility functions changed with this addition.           */
/*  Added command line argument option type 'cascor1 help' for usage info.  */
/*  Added .net file and on-the-fly parameter adjustment code.  See new      */
/*   samples files at the end of this listing for examples.  Functions      */
/*   main and GET_NETWORK_CONFIGURATION have changed completely.            */
/*  GET_USER_INPUT replaced by Y_OR_N_P                                     */
//*  <signal.h> included to support on-the-fly parameter updating            */
/****************************************************************************/
/*                                                                          */
/* Changes from Version 1.15 Jul-18-90 to  1.16  Oct-24-90                  */
/*                                                                          */
/* bug fix in BUILD_NETWORK, INSTALL_NEW_UNIT, and TRAIN to allow           */
/* NTestPatterns > NTrainingPatterns.  Thanks to William Stevenson          */
/****************************************************************************/
/*                                                                          */
/* Changes from Version 1.16  Oct-24-90  to 1.17  Nov-12-90                 */
/****************************************************************************/
/* bug fix in TRAIN line 1662 change NtrainingPatterns to NTrainingPatterns */
/*  Thanks to Merrill Flood for pointing out the problem.                   */
/****************************************************************************/
/*                                                                          */
/* Changes from Version 1.17  Nov-12-90 to 1.30  Jan-23-91                  */
/****************************************************************************/
/* Added code to allow user to save the weights into and load weights       */
/*   from external files.                                                   */
/* Added code to allow saving of .net files to save any changes to          */
/*   parameters made during interactive learning trials.                    */
/* Added an alternative main routine that can be used to calculate          */
/*   predictions using a previously saved set of weights.  To activate      */
/*   this feature compile the code with the symbol PREDICT_ONLY defined.    */
/* Added code to allow '# comment' lines in the training or test sets.      */
/* Added optional code to calculate the number of multiply-accumulates      */
/*   used during training.  This is useful for comparing                    */
/*   Cascade-correlation to other learning algorithms in a machine          */
/*   independent manner.  To activate this feature define the symbol        */
/*   CONNX at compile time.                                                 */
/* Added code to calculate the Lapedes and Faber Index.  Useful for    */
/*   problems with real-valued outputs.                                     */
/* Added UnitType VARSIGMOID which can have arbitrary output range          */
/*   defined by SigmoidMin and SigmoidMax.  Thanks to Dimitris              */
/*   Michailidis.                                                           */
/* Added code to allow the training and test data to be read from           */
/*   separate files.  Thanks to Carlos Puchol.                              */
/* Added code to save SumError for each output instead of combining it      */
/*   together for all outputs.  This change helps for multiple output       */
/*   problems.  Thanks to Scott Fahlman.                                    */
/* Code added to allow specification of a NonRandomSeed for the random      */
/*   number generator.  Thanks to Dimitris Michailidis.                     */
/* Removed useless setting of Ninputs and Noutputs from BUILD_NET.  Thanks  */
/*   to Dimitris Michailidis.                                               */
/****************************************************************************/
/*                                                                          */
/* Changes from Version 1.30  Jan-23-91 to 1.31 Jan-25-91                   */
/* fixed typo.  include <string.h> not <sting.h> thanks to Peter Hancock.   */
/*                                                                          */
/* Changes from Version 1.31 Jan-25-91 to 1.32 Mar-21-91                    */
/* BUG FIX in INIT_NET.  Thanks to Boris Gokhman                            */
/* BUG FIX in TEST_EPOCH.  Thanks to Boris Gokhman                          */
/*                                                                          */
/* Changes from Version 1.32 Mar-21-91 to 1.33 Apr-16-92                    */
/* Prototype correction for strtok.  Thanks to Brian Ripley                 */
/****************************************************************************/
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <string.h>
//#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

#if  defined(WIN32)
#include <LIMITS.H>
#include <FLOAT.H>
#include <windows.h>
#define _SCL_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#endif



//#ifndef _GNU_SOURCE
//   #include "getopt.h"
//#else
//   #include <getopt.h>
//#endif

//#include "utils.h"

#include "UsfCasCor.h"

// Forward Declarations.
int        GetProcessId ();
long long  GetLocalDateTime ();
char*      GetDsiplayableTime ();
char* index (char* str, int c);

#if  defined(WIN32)
int  strncasecmp (const char*  s1, 
                  const char*  s2, 
                  int          count
                 );
#endif



const char*  _(const char* str)
{
  return str;
}




UsfCasCor::UsfCasCor ():
  progname             (NULL),
  optarg               (NULL),
  optind               (1),
  nextchar             (NULL),
  __getopt_initialized (0),
  opterr               (1),
  optopt               ('?'),
  posixly_correct      (NULL),

  //***************************************************************
  //*                            usfcascor                        *
  //***************************************************************
  in_limit             (500),
  out_limit            (500),
  number_of_trials     (1),
  number_of_rounds     (-1),
  normalization_method (0),
  my_mpi_rank          (0),
  //longopts             (),
  options              ("hf:i:o:r:t:PO:N::IRS:w:sT"),
  debug_level          (0),

  //*********************************************************************
  //                             globals.c                              *
  //*********************************************************************
  number_of_classes       (-1),
  filestem                (NULL),
  class_label             (NULL),
  line_length             (-1),
  feature_type            (NULL),

  the_random_seed         (0),

  use_index_files         (False),
  do_predictions          (True),
  normalize_features      (False),
  resubstitution          (False),
  do_training             (True),
  save_network_each_trial (False),
  save_network_each_round (False),
  load_weights            (False),

  UnitType                (SIGMOID),
  OutputType              (SIGMOID),
  SigmoidMax              (0.5),
  SigmoidMin              (-0.5),
  WeightRange             (1.0),
  SigmoidPrimeOffset      (0.1),
  WeightMultiplier        (1.0),
  OutputMu                (2.0),
  OutputShrinkFactor      (0.0f),
  OutputEpsilon           (0.35f), 
  OutputDecay             (0.0001f),
  OutputPatience          (8),    
  OutputChangeThreshold   (0.01f),
  InputMu                 (2.0f),
  InputShrinkFactor       (0.0f),
  InputEpsilon            (1.0f),
  InputDecay              (0.0f),
  InputPatience           (8),
  InputChangeThreshold    (0.03f),
                                  

  /*  Variables related to error and correlation.   */
  TrueError               (0.0f),
  ScoreThreshold          (0.35f),
  ErrorBits               (0),
  SumErrors               (NULL),
  DummySumErrors          (NULL),
  SumSqError              (0.0f),
  BestCandidateScore      (0.0f),
  BestCandidate           (0),


  /* These variables and switches control the simulation and display.    */
  UseCache                (False),
  Epoch                   (0),        
  Graphics                (False),
  NonRandomSeed           (False),
  Test                    (True),
  SinglePass              (False),
  SingleEpoch             (False),
  Step                    (False),
  Trial                   (0),

  /* The sets of training inputs and outputs. */
  NTrainingPatterns       (0),
  NTestPatterns           (0),
  TrainingInputs          (NULL),
  TrainingOutputs         (NULL),
  Goal                    (NULL),
  T_T_files               (NULL),

  example_weight          (NULL),

  /*  For some benchmarks there is a separate set of values used for testing */
  /*  the network's ability to generalize.  These values are not used during */
  /*  training.                                                              */
  TestInputs              (NULL),
  TestOutputs             (NULL),

  MaxUnits                (2000),
                  
  Nunits                  (0),
  Ninputs                 (0),
  Noutputs                (0),
  Ncandidates             (8),
  MaxCases                (0),
  Ncases                  (0),
  FirstCase               (0),
                    
  /***************************************************************************/
  /* The following vectors hold values related to hidden units in the active */
  /* net and their input weights.                                            */
  /***************************************************************************/
  Values                  (NULL),
  ValuesCache             (NULL),
  ExtraValues             (NULL),
  Nconnections            (NULL),
  Connections             (NULL),
  Weights                 (NULL),

  /***************************************************************************/
  /* The following arrays of arrays hold values for the outputs of the active*/
  /*  network and the output-side weights.                                   */
  /***************************************************************************/
  Outputs                 (NULL),
  Errors                  (NULL),
  ErrorsCache             (NULL),
  ExtraErrors             (NULL),
  OutputWeights           (NULL),
  OutputDeltas            (NULL),
  OutputSlopes            (NULL),
  OutputPrevSlopes        (NULL),

  /***************************************************************************/
  /* The following arrays have one entry for each candidate unit in the      */
  /* pool of trainees.                                                       */
  /***************************************************************************/
  CandValues              (NULL),
  CandSumValues           (NULL),
  CandCor                 (NULL),
  CandPrevCor             (NULL),
  CandWeights             (NULL),
  CandDeltas              (NULL),
  CandSlopes              (NULL),
  CandPrevSlopes          (NULL),

  /***************************************************************************/
  /* This saves memory if each candidate unit receives a connection from       */
  /* each existing unit and input.  That's always true at present, but may     */
  /* not be in future.                                                         */
  /***************************************************************************/
  AllConnections          (NULL),

  /***************************************************************************/
  /* ErrorIndex specific globals.  Not in release Lisp version               */
  /***************************************************************************/
  NtrainingOutputValues   (0),
  NtestOutputValues       (0),
  TrainingStdDev          (1.0f),
  TestStdDev              (1.0f),
  ErrorIndex              (0.0f),
  ErrorIndexThreshold     (0.2f),
  ErrorMeasure            (BITS),


  /***************************************************************************/
  /* Save and plot file related varibles                                     */
  /***************************************************************************/
  DumpWeights             (False),
  WeightFile              (NULL),
  InterruptPending        (False),
  Nparameters             (0)

{
  ConstructParmTable ();
  //longopts = new option*[9];
  longopts[0] = option ("help",                   no_argument,       0, 'h');
  longopts[1] = option ("version",                no_argument,       0, 'v');
  longopts[2] = option ("filestem",               required_argument, 0, 'f');
  longopts[3] = option ("normalize",              optional_argument, 0, 'N');
  longopts[4] = option ("no-normalize",           no_argument,       (int*)&normalize_features, False);
  longopts[5] = option ("use-example-weights",    no_argument,       (int*)&load_weights,       True);
  longopts[6] = option ("no-use-example-weights", no_argument,       (int*)&load_weights,       False);
  longopts[7] = option ("mpi",                    required_argument, 0, option_mpi);
  longopts[8] = option (0,0,0,0);
}





UsfCasCor::~UsfCasCor ()
{
  delete  progname;  progname = NULL;
}


UsfCasCor::int32  UsfCasCor::MemoryConsumedEstimated ()  const
{
  int32  memoryConsumedEstimated = sizeof (*this);


/*
  static  char*  version;
  char*   progname;

  char*   options;
  char*   optarg;
  char*   nextchar;
  char*    filestem;
  char**   class_label;
  int*     feature_type;
  float*   SumErrors;       
  float*   DummySumErrors;  
  float**  TrainingInputs;
  float**  TrainingOutputs;
  float*   Goal;            
  char*    T_T_files;       
  float*   example_weight;
  float**  TestInputs;
  float**  TestOutputs;
  float*   Values;   	      
  float**  ValuesCache;     
  float*   ExtraValues;	    
  int*     Nconnections;    
  int**    Connections;     
  float**  Weights;         

  float*   Outputs;         
  float*   Errors;          
  float**  ErrorsCache;     
  float*   ExtraErrors;     
  float**  OutputWeights;   
  float**  OutputDeltas;    
  float**  OutputSlopes;    
  float**  OutputPrevSlopes; 

  float*   CandValues;         
  float*   CandSumValues;      
  float**  CandCor;            
  float**  CandPrevCor;        
  float**  CandWeights;        
  float**  CandDeltas;         
  float**  CandSlopes;         
  float**  CandPrevSlopes;     

   int*     AllConnections;    


  FILE*    WeightFile;	       

*/
  return  memoryConsumedEstimated;
}  /* MemoryConsumedEstimated */






/* Administrative variables */
char*  UsfCasCor::version="5.0";
char*  UsfCasCor::release_date="07-07-2012";

char*  UsfCasCor::Usage = 
"Usage\n"
   "%s -f filestem\n"
      "-h | --help             help\n"
      "-f | --filestem=s       filestem\n"
      "-i n                    max number of input epochs\n"
      "-o n                    max number of output epochs\n"
      "-r n                    max number of new units\n"
      "-t n                    number of trials\n"
      "-P                      generate predictions from the test set\n"
      "-O option=value         Set an option, as in net file (UseCache=TRUE) \n"
      "-N[l|s]                 set normalization (default=s)\n"
      "-I           use index files\n"
      "-R                      use resubstitution (test the training set)\n"
      "-w file                 use file for network weights/config\n"
      "-s                      save network state every round and trial.\n"
      "-S #                    use # as the random seed\n"
      "-T                      do not train\n"
      "\n"      
      "Flags\n"
      "*****\n"
      "--use-example-weights   Load example weights from filestem.weights\n"
        "--no-use-example-weights\n"
      "--normalize=[logistic]  Use normalization\n"
      "--no-normalize          \n"
      "--mpi=n    \n";







int  UsfCasCor::main (int argc, char *argv[])
{
  char*  network_file=NULL;
  char*  old_filestem;
  int    c;
  int    option_index;
  /***************/

  #ifdef USE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_mpi_rank);
  #endif

  progname = strdup (argv[0]);
  info ("Cascade Correlation: v%s\n", version);
  INITIALIZE_GLOBALS ();

  /* process arguments one line at a time */
  while  ((c = getopt_long (argc, argv, options, longopts, &option_index)) != EOF) 
  {
    switch (c) {
    case 0:
      printf ("  Set option: %s.\n", longopts[option_index].name);
      break;

    case 'h':
      fprintf(stderr, Usage, progname);
      exit(1);

    case 'v':
      fprintf (stdout, "%s\n", version);
      exit (1);

    case 'f':                        /* Filestem */
      filestem=strdup(optarg);
      break;

    case option_mpi:
      my_mpi_rank+=atoi(optarg);
      break;

    case 'i':
      in_limit=atoi(optarg);
      break;

    case 'o':
      out_limit = atoi(optarg);
      break;

    case 'r':
      number_of_rounds = atoi(optarg);
      break;

    case 't':
      number_of_trials = atoi(optarg);
      break;

    case 'N':
      normalization_method = 0;
      normalize_features = True;

      if  (optarg != NULL)
      {
        if  (*optarg == 'l')
          normalization_method = 1;
        else if  (*optarg == 's')
          normalization_method = 0;
      }

      printf (" Using normalization method %c\n", (normalization_method == 0) ? 'l' : 's');
      break;


    case 'I':
      use_index_files=True;
      printf(" Using index files\n");
      break;


    case 'P':                        
      do_predictions=True;
      printf(" Using predictions\n");
      break;


      /* Here, -o parm=value. PROCESS_LINE handles this */
    case 'O':
      process_line (optarg);
      break;


    case 'R':
      resubstitution = True;
      break;


    case 'S':
      the_random_seed = atol (optarg);
      break;


    case 's':
      save_network_each_trial = True;
      save_network_each_round = True;
      break;


    case 'w':
      network_file = strdup (optarg);
      break;


    case 'T':
      do_training=False;
      break;

    default:
      break;
    }
  } 

  if  (filestem == NULL) 
  {
    fprintf (stderr, Usage, progname);
    exit (1);
  }


  #ifdef USE_MPI
   old_filestem = filestem;
   filestem = calloc (strlen (old_filestem) + my_mpi_rank / 10 + 5, sizeof(char));
   sprintf (filestem, "%s-%d", old_filestem, my_mpi_rank);
   printf ("\tUsing filestem %s\n", filestem);
   free (old_filestem);
  #endif


  /* First, load the data and configuration */
  setup_network ();


  /* If specified, load network weights/parameters from file */
  if  (network_file)
    load_network (network_file);


  /* Then train the network */
  if  (do_training)
    train_network ();

  
  /* Test the sucker. */
  if ( do_predictions )
    PREDICT (ScoreThreshold);

  char buffer[1024];
  cin >> buffer;
  

  #ifdef USE_MPI
     MPI_Finalize();
  #endif

  return(0);
}  /* main */









void  UsfCasCor::Initialize (int32  _in_limit,
                             int32  _out_limit,
                             int32  _number_of_rounds,
                             int32  _number_of_trials,
                             int64  _the_random_seed
                            )
{
  progname = strdup ("UsfCasCor");
  info ("Cascade Correlation: v%s\n", version);
  INITIALIZE_GLOBALS ();

  filestem=strdup("");

  in_limit         = _in_limit;
  out_limit        = _out_limit;
  number_of_rounds = _number_of_rounds;
  number_of_trials = _number_of_trials;

  /** Features will already be normalized. */
  normalization_method = 0;
  normalize_features = False; 
  the_random_seed =_the_random_seed;

  /* First, load the data and configuration */
  setup_network ();

  /* If specified, load network weights/parameters from file */
//  if  (network_file)
//    load_network (network_file);

  /* Then train the network */
  if  (do_training)
    train_network ();

}  /* SetParameters */







/*
 *  Get and initialize a network. 
 */
void  UsfCasCor::setup_network()
{
  /* 
     There are some required variables, like NInputs,etc
     that can be taken from the training/testing files. 
  */
  info ("Dataset: %s\n", filestem);
  load_namesfile (filestem);


  /* At this point, it looks like the MaxUnits parameter is
     simply the sum of NInputs+1 and the max. number of units
     to add. Set this manually, since it doesn't seem to be
     set elsewhere.
  */


  if  (number_of_rounds == -1)
     number_of_rounds=15;

//(Ninputs+Noutputs)/2;
  MaxUnits = Ninputs + 1 + number_of_rounds;

  /* Once all arguments have been read and what parameters we
     have, we have -- then, build the network and load the data.
  */
  allocate_network ();
  load_data ();


  /* Randomization. If not specified on command line and NonRandomSeed
     is not True then seed with time (truly random) */
  if  (NonRandomSeed) 
     the_random_seed = 1;

  if (the_random_seed <= 0)
     the_random_seed = time(NULL);

  the_random_seed += GetProcessId () + my_mpi_rank;

  srand (the_random_seed);
  info (" Starting at %s seed: %ld\n",(NonRandomSeed)?"fixed":"random",
        the_random_seed);

  /* Initialize the network variables */
  initialize_network ();

  return;
}  /* setup_network */




void UsfCasCor::train_network ()
{
  int nhidden;      /* number of hidden units used in run  */
  int vics, defs, i;
  long total_epochs, total_units, total_trials;
  long min_units, max_units, min_epochs, max_epochs;

  /* initialize testing parms */
  total_epochs = 0;
  total_units = 0;
  min_units = INT_MAX;
  min_epochs = INT_MAX;
  max_units = 0;
  max_epochs = 0;
  total_trials = 0;
  vics = 0;
  defs = 0;


  /* Start the main processing loop */
  info ("\nStarting run for %s, Ilim %d, Olim %d, MaxUnits %d, Trials %d.\n",
        filestem, 
        in_limit, 
        out_limit, 
        number_of_rounds, 
        number_of_trials
       );

  for  (i = 0; i < number_of_trials; i++)
  {
    Trial = i + 1;
    
    if  (number_of_trials > 1) 
      info ("Trial %d:\n", Trial);

    switch (TRAIN (out_limit, in_limit, number_of_rounds))
    {
     case WIN:
          vics++;
          break;

     case LOSE:
          defs++;
           break;
    }

    /* how did we do? */
    if  (Test)
      TEST_EPOCH (ScoreThreshold);

#ifdef CONNX
    printf (" Connection Crossings: %d\n\n", conx);
#endif

    /* collect trail stats */
    nhidden = Nunits - Ninputs - 1;  /* don't count inputs or bias unit */
    total_epochs += Epoch;
    total_units += nhidden;
    total_trials++;
    min_epochs = (Epoch < min_epochs) ? Epoch : min_epochs;
    max_epochs = (Epoch > max_epochs) ? Epoch : max_epochs;
    min_units = (nhidden < min_units) ? nhidden : min_units;
    max_units = (nhidden > max_units) ? nhidden : max_units;
    
    if  (save_network_each_trial )
      save_network (Trial, -1);

  } /* End trial loop */
  
  /* print out loop stats */
  info ("TRAINING STATS\n");
  LIST_PARAMETERS ();
  info ("Victories: %d, Defeats: %d, \n", vics, defs);
  info ("   Training Epochs - Min: %ld, Avg: %ld,  Max: %ld,\n", 
    min_epochs, (total_epochs / total_trials), max_epochs);
  info ("   Hidden Units -    Min: %ld, Avg: %4.1f,  Max: %ld,\n", 
    min_units,((float)total_units /total_trials), max_units);

  return;
}  /* train_network */



/* Create the network data structures, given the number of input and output
 * units.  Get the MaxUnits value from a variable.
 */
void  UsfCasCor::allocate_network (void)
{
  int i;
/***************/

  if  (NTrainingPatterns > NTestPatterns)
    MaxCases = NTrainingPatterns;
  else
    MaxCases = NTestPatterns;

  Ncases    = NTrainingPatterns;
  FirstCase = 0;
  Nunits    = 1 + Ninputs;


  /* setup for ErrorIndex */
  NtrainingOutputValues = Noutputs * NTrainingPatterns; 
  NtestOutputValues = Noutputs * NTestPatterns;
  if(Nunits>MaxUnits)
    die( "MaxUnits must be greater than Ninputs.");


  /* allocate memory for outer arrays */
  ExtraValues = (float *)CALLOC(MaxUnits, sizeof(float));
  Values = ExtraValues;

  Nconnections = (int *)CALLOC(MaxUnits, sizeof(int));
  Connections = (int **)CALLOC(MaxUnits, sizeof(int *));
  Weights = (float **)CALLOC(MaxUnits, sizeof(float *));

  ExtraErrors    = (float *)CALLOC(Noutputs, sizeof(float));
  SumErrors      = (float *)CALLOC(Noutputs, sizeof(float));
  DummySumErrors = (float *)CALLOC(Noutputs, sizeof(float));
  Errors         = ExtraErrors;
  
  Outputs          = (float *)CALLOC(Noutputs, sizeof(float));
  OutputWeights    = (float **)CALLOC(Noutputs, sizeof(float *));
  OutputDeltas     = (float **)CALLOC(Noutputs, sizeof(float *));
  OutputSlopes     = (float **)CALLOC(Noutputs, sizeof(float *));
  OutputPrevSlopes = (float **)CALLOC(Noutputs, sizeof(float *));

  CandValues     = (float *) CALLOC(Ncandidates, sizeof(float));
  CandSumValues  = (float *) CALLOC(Ncandidates, sizeof(float));
  CandCor        = (float **)CALLOC(Ncandidates, sizeof(float *));
  CandPrevCor    = (float **)CALLOC(Ncandidates, sizeof(float *));
  CandWeights    = (float **)CALLOC(Ncandidates, sizeof(float *));
  CandDeltas     = (float **)CALLOC(Ncandidates, sizeof(float *));
  CandSlopes     = (float **)CALLOC(Ncandidates, sizeof(float *));
  CandPrevSlopes = (float **)CALLOC(Ncandidates, sizeof(float *));
 
  TrainingInputs  = (float **)CALLOC(NTrainingPatterns, sizeof(float *));
  TrainingOutputs = (float **)CALLOC(NTrainingPatterns, sizeof(float *));

  if  (NTestPatterns){
    TestInputs  = (float **)CALLOC(NTestPatterns, sizeof(float *));
    TestOutputs = (float **)CALLOC(NTestPatterns, sizeof(float *));
  }

  else
  {       /* no test patterns so just point at training set */
    TestInputs  = TrainingInputs;
    TestOutputs = TrainingOutputs;
  }



/* Only create the caches if UseCache is on -- may not always have room. */
  if  (UseCache)
  {
    ValuesCache = (float **)CALLOC(MaxCases, sizeof(float *));
    ErrorsCache = (float **)CALLOC(MaxCases, sizeof(float *));
    for  (i = 0; i < MaxCases; i++)
    {
      ValuesCache[i] = (float *)CALLOC(MaxUnits, sizeof(float));
      ErrorsCache[i] = (float *)CALLOC(Noutputs, sizeof(float));
    }
  }



  /* Allocate per unit data arrays */
  for  (i = 0; i < Noutputs; i++)
  {
    OutputWeights[i]    = (float *)CALLOC(MaxUnits, sizeof(float));
    OutputDeltas[i]     = (float *)CALLOC(MaxUnits, sizeof(float));
    OutputSlopes[i]     = (float *)CALLOC(MaxUnits, sizeof(float));
    OutputPrevSlopes[i] = (float *)CALLOC(MaxUnits, sizeof(float));
  }

  for  (i = 0; i < Ncandidates; i++)
  {
    CandCor[i]        = (float *)CALLOC(Noutputs, sizeof (float));
    CandPrevCor[i]    = (float *)CALLOC(Noutputs, sizeof (float));
    CandWeights[i]    = (float *)CALLOC(MaxUnits, sizeof (float));
    CandDeltas[i]     = (float *)CALLOC(MaxUnits, sizeof (float));
    CandSlopes[i]     = (float *)CALLOC(MaxUnits, sizeof (float));
    CandPrevSlopes[i] = (float *)CALLOC(MaxUnits, sizeof (float));
  }

  /* Allocate per case data arrays */
  for  (i = 0; i < NTrainingPatterns; i++)
  {
    TrainingInputs[i]  = (float *)CALLOC(Ninputs,  sizeof (float));
    TrainingOutputs[i] = (float *)CALLOC(Noutputs, sizeof (float));
  }

  for  (i = 0; i < NTestPatterns; i++)
  {
    TestInputs[i]  = (float *)CALLOC (Ninputs,  sizeof (float));
    TestOutputs[i] = (float *)CALLOC (Noutputs, sizeof (float));
  }

  /* Allocate generic connection vector */
  AllConnections = (int *)CALLOC(MaxUnits, sizeof(int));

  return;
}  /* allocate_network */

  





/********************************************************************/
/*   interrupt handling routines                                    */
/*   Thanks to Dimitris Michailidis for this code.                  */
/********************************************************************/

/* allow user to change parameters if they have hit Control-C 
 */
void CHECK_INTERRUPT (void)
{
  /*   if (InterruptPending){
    printf("  Simulation interrupted at epoch %d\n", Epoch);

  }
  */
}



/* Record an interrupt whenever the user presses Control-C 
 */
void TRAP_CONTROL_C(int sig)
{
  /* 
  InterruptPending = True;
  signal(SIGINT, TRAP_CONTROL_C);
  */
}








//****************************************************************
//*                            'parms.c'                         *   
//****************************************************************


#define EOL '\0'


Boolean _boolean_convert();
int     _type_convert();

//char parm_string[LINELEN];

/* Save the settable parameters to a file for later use. */
char*  UsfCasCor::parm_to_string(int k)
{
  switch(ParmTable[k].vartype)
  { 
  case INT: case INT_NO:
    sprintf(parm_string,"%s=%d\n", ParmTable[k].keyword,
      *(int *)ParmTable[k].varptr);
    break;

  case FLOAT: case FLOAT_NO:
    sprintf(parm_string, "%s=%f\n", ParmTable[k].keyword, 
      *(float *)ParmTable[k].varptr);
    break;

  case ENUM: case ENUM_NO:
    sprintf(parm_string, "%s=%s\n", ParmTable[k].keyword, 
      type_to_string(*(int *)ParmTable[k].varptr));
    break;

  case BOOLE: case BOOLE_NO:
    sprintf(parm_string, "%s=%s\n", ParmTable[k].keyword, 
      boolean_to_string(*(Boolean *)ParmTable[k].varptr));
    break;

  default:
    parm_string[0]=0;
    break;      /* skip anything else */
  }

  return parm_string;
}


char*  UsfCasCor::type_strings[]={"SIGMOID","GAUSSIAN", "LINEAR","ASYMSIGMOID","VARSIGMOID","WIN","STAGNANT","TIMEOUT","LOSE","BITS","INDEX","Bad Type"};


/* Input of the type variables and return a string showing its value.  This
 * is only used as a output routine for the user's convenience. 
 */
char*  UsfCasCor::type_to_string (int var)
{
  switch (var) 
  {
    case SIGMOID:     return(type_strings[0]);
    case GAUSSIAN:    return(type_strings[1]);
    case LINEAR:      return(type_strings[2]);
    case ASYMSIGMOID: return(type_strings[3]);
    case VARSIGMOID:  return(type_strings[4]);
    case WIN:         return(type_strings[5]);
    case STAGNANT:    return(type_strings[6]);
    case TIMEOUT:     return(type_strings[7]);
    case LOSE:        return(type_strings[8]);
    case BITS:        return(type_strings[9]);
    case INDEX:       return(type_strings[10]);

    default:          return(type_strings[11]);
  }
}  /* type_to_string */



char*  UsfCasCor::boolean_strings[2]={"False","True"};

char*  UsfCasCor::boolean_to_string (Boolean var)
{
  switch (var) 
  {
    case False:  return(boolean_strings[0]);
    case True:   return(boolean_strings[1]);

    default: die("Bad boolean type\n");
 }
}  /* boolean_to_string */





/* Convert '\0' terminated sting to all lower case characters.  This routine
 * is destructive.
 */
void  UsfCasCor::strdncase (char *s)
{
  int i;
  for  (i = 0;  s[i] != EOL;  i++)
  {
    if  (isupper(s[i]))
      s[i] = tolower(s[i]);  /* tolower only guaranteed on upper case */
    else
      s[i] = s[i];
  }
}



/* Given a keyword string, return the index into the keyword table.
 * Assumes that the keys are in alphabetacal order.  Keyword comparison
 * is all lower case.  Return FAILURE when not found.
 */
int  UsfCasCor::find_key (char *searchkey)
{
  int lower = 0;
  int upper = Nparameters - 1; 
  int m, dif;
  /************/

  strdncase (searchkey);    /* convert case for comparison */

  while(lower <= upper)
  {
    m = (upper + lower) / 2;
    dif = strcmp (searchkey, ParmTable[m].keyword);
    if  (dif < 0)
      upper = m - 1;    /* look in lower half */

    else if  (dif == 0)
      return  (m);    /* found it */

    else if  (dif > 0)
      lower = m + 1;    /* look in upper half */
  }
  
  /* search failed */
  return(FAILURE);
}  /* find_key */



/* Parse a line of input into keyword value pairs and reset the given 
 * parameters to given values.  Comment lines start with the character
 * '#' and are ignored.  If a bad keyword is given a message is printed,
 * but processing continues.  The routine returns a value telling the 
 * calling routine whether to grap another line, read in  the training, 
 * or read in testing data.  The special keywords "Training", and 
 * "Testing" signal the changes in status.
 */
int   UsfCasCor::process_line (char *line)
{
  int k = 0;      /* location in ParmTable */
  char *keytok;      /* token pointer */
  char *valtok;      /* token pointer */
  static char *seperators = "= \t\v\f\r\n,"; /* white space plus comma  */
  /*************/

  /* check for comment character */
  if(line[0] == '#' || line[0] == '\n')
    return(NEXTLINE);    /* skip comment and blank lines */
  else
  {
    keytok = strtok(line, seperators); /* get first token */
    while(keytok != NULL)
    {
      k = find_key(keytok);

      if  (k != FAILURE)
      {
        /* get value token for this parameter */
        valtok = strtok(NULL, seperators);

        /* read value in correct format */
        switch(ParmTable[k].vartype)
        { 
          case INT: case INT_NO:
            *(int *)ParmTable[k].varptr = atoi(valtok);
            break;
          case FLOAT: case FLOAT_NO:
            *(float *)ParmTable[k].varptr = (float)atof(valtok);
            break;
          case ENUM: case ENUM_NO: 
            *(int *)ParmTable[k].varptr = _type_convert(valtok);
            break;
          case BOOLE: case BOOLE_NO:
            *(Boolean *)ParmTable[k].varptr= _boolean_convert(valtok);
            break;
          case INITFILE:
            warn("initfile option no longer accepted.");
            break;
          case GO: case VALUE: case SAVE:
            warn("%s keyword only legal in interactive mode.", keytok);
            break;
          default:
            warn("%d: bad vartype for parameter %s.  No update performed.", 
              ParmTable[k].vartype, keytok);
            break;
          }
      }
      else      /* bad key */
        warn("%s: not in parameter table.  No update performed.", keytok);

      /* get next keyword token */
      keytok = strtok(NULL, seperators);  
    }        /* end while still keytok */

    return(NEXTLINE);
  }        /* end if comment */
}  /* process_line*/




/**********************************************************
 Functions needed only in this file. 
***********************************************************/

int  UsfCasCor::_type_convert (char *input)
{
  strdncase(input);
  
  if  (!strcmp (input,"true"))
    return (1);

  else if  (!strcmp (input, "1")) /* allow backward compatiple input */
    return (1);

  else if  (!strcmp (input, "false"))
    return (0);

  else if  (!strcmp(input, "0"))  /* allow backward compatiple input */
    return (0);

  else if  (!strcmp (input, "sigmoid"))
    return (SIGMOID);

  else if  (!strcmp (input, "gaussian"))
    return  (GAUSSIAN);

  else if  (!strcmp (input, "linear"))
    return  (LINEAR);

  else if  (!strcmp (input, "asymsigmoid"))
    return  (ASYMSIGMOID);

  else if  (!strcmp (input, "varsigmoid"))
    return  (VARSIGMOID);

  else if  (!strcmp (input, "bits"))
    return  (BITS);

  else if  (!strcmp (input, "index"))
    return  (INDEX);

  else 
    die ("Bad string sent to TYPE_CONVERT %s", input);
}  /* _type_convert */




Boolean  UsfCasCor::_boolean_convert (char *input)
{
  strdncase (input);
  if  (!strcmp (input,"true") || !strcmp(input,"1"))
    return True;

  if  (!strcmp (input,"false") || !strcmp(input,"0"))
    return False;

  die("Bad string sent to boolean_convert: %s", input);
}  /* _boolean_convert */
 








//****************************************************************
//*                            getopt.c'                         *   
//****************************************************************




/* Describe how to deal with options that follow non-option ARGV-elements.

   If the caller did not specify anything,
   the default is REQUIRE_ORDER if the environment variable
   POSIXLY_CORRECT is defined, PERMUTE otherwise.

   REQUIRE_ORDER means don't recognize them as options;
   stop option processing when the first non-option is seen.
   This is what Unix does.
   This mode of operation is selected by either setting the environment
   variable POSIXLY_CORRECT, or using `+' as the first character
   of the list of option characters.

   PERMUTE is the default.  We permute the contents of ARGV as we scan,
   so that eventually all the non-options are at the end.  This allows options
   to be given in any order, even with programs that were not written to
   expect this.

   RETURN_IN_ORDER is an option available to programs that were written
   to expect options and other ARGV-elements in any order and that care about
   the ordering of the two.  We describe each non-option ARGV-element
   as if it were the argument of an option with character code 1.
   Using `-' as the first character of the list of option characters
   selects this mode of operation.

   The special argument `--' forces an end of option-scanning regardless
   of the value of `ordering'.  In the case of RETURN_IN_ORDER, only
   `--' can cause `getopt' to return -1 with `optind' != ARGC.  */


char*  UsfCasCor::my_index (const char *str,
                            int        chr
                           )
{
  while (*str)
  {
    if  (*str == chr)
      return (char *) str;
    str++;
  }
  return 0;
}



const char*  UsfCasCor::strmy_index (const char*  str, 
                                     int          chr
                                    )
{
  while (*str)
  {
    if  (*str == chr)
      return (char *)str;
    str++;
  }
  return 0;
}  /* strmy_index */



#define SWAP_FLAGS(ch1, ch2)



/* Exchange two adjacent subsequences of ARGV.
   One subsequence is elements [first_nonopt,last_nonopt)
   which contains all the non-options that have been skipped so far.
   The other is elements [last_nonopt,optind), which contains all
   the options processed since those non-options were skipped.

   `first_nonopt' and `last_nonopt' are relocated so that they describe
   the new indices of the non-options in ARGV after they are moved.  */


void  UsfCasCor::exchange (char**  argv)
{
  int bottom = first_nonopt;
  int middle = last_nonopt;
  int top = optind;
  char *tem;

  /* Exchange the shorter segment with the far end of the longer segment.
     That puts the shorter segment into the right place.
     It leaves the longer segment in the right place overall,
     but it consists of two parts that need to be swapped next.  */


  while (top > middle  &&  middle > bottom)
  {
    if  (top - middle > middle - bottom)
    {
      /* Bottom segment is the short one.  */
      int len = middle - bottom;
      register int i;

      /* Swap it with the top part of the top segment.  */
      for  (i = 0; i < len; i++)
      {
        tem = argv[bottom + i];
        argv[bottom + i] = argv[top - (middle - bottom) + i];
        argv[top - (middle - bottom) + i] = tem;
        SWAP_FLAGS (bottom + i, top - (middle - bottom) + i);
      }
      /* Exclude the moved bottom segment from further swapping.  */
      top -= len;
    }
    else
    {
      /* Top segment is the short one.  */
      int len = top - middle;
      register int i;

      /* Swap it with the bottom part of the bottom segment.  */
      for (i = 0; i < len; i++)
      {
        tem = argv[bottom + i];
        argv[bottom + i] = argv[middle + i];
        argv[middle + i] = tem;
        SWAP_FLAGS (bottom + i, middle + i);
      }
      /* Exclude the moved top segment from further swapping.  */
      bottom += len;
    }
  }

  /* Update records for the slots the non-options now occupy.  */

  first_nonopt += (optind - last_nonopt);
  last_nonopt = optind;
}  /* exchange */



/* Initialize the internal data when the first call is made.  */

const char*  UsfCasCor::_getopt_initialize (int         argc,
                                            char* const *argv,
                                            const char  *optstring
                                           )
{
  /* Start processing options with ARGV-element 1 (since ARGV-element 0
     is the program name); the sequence of previously skipped
     non-option ARGV-elements is empty.  */

  first_nonopt = last_nonopt = optind;

  nextchar = NULL;

  posixly_correct = getenv ("POSIXLY_CORRECT");

  /* Determine how to handle the ordering of options and nonoptions.  */

  if  (optstring[0] == '-')
  {
    ordering = RETURN_IN_ORDER;
    ++optstring;
  }
  
  else if  (optstring[0] == '+')
  {
    ordering = REQUIRE_ORDER;
    ++optstring;
  }
  
  else if  (posixly_correct != NULL)
    ordering = REQUIRE_ORDER;

  else
    ordering = PERMUTE;

  return optstring;
}  /* _getopt_initialize */







/* Scan elements of ARGV (whose length is ARGC) for option characters
   given in OPTSTRING.

   If an element of ARGV starts with '-', and is not exactly "-" or "--",
   then it is an option element.  The characters of this element
   (aside from the initial '-') are option characters.  If `getopt'
   is called repeatedly, it returns successively each of the option characters
   from each of the option elements.

   If `getopt' finds another option character, it returns that character,
   updating `optind' and `nextchar' so that the next call to `getopt' can
   resume the scan with the following option character or ARGV-element.

   If there are no more option characters, `getopt' returns -1.
   Then `optind' is the index in ARGV of the first ARGV-element
   that is not an option.  (The ARGV-elements have been permuted
   so that those that are not options now come last.)

   OPTSTRING is a string containing the legitimate option characters.
   If an option character is seen that is not listed in OPTSTRING,
   return '?' after printing an error message.  If you set `opterr' to
   zero, the error message is suppressed but we still return '?'.

   If a char in OPTSTRING is followed by a colon, that means it wants an arg,
   so the following text in the same ARGV-element, or the text of the following
   ARGV-element, is returned in `optarg'.  Two colons mean an option that
   wants an optional arg; if there is text in the current ARGV-element,
   it is returned in `optarg', otherwise `optarg' is set to zero.

   If OPTSTRING starts with `-' or `+', it requests different methods of
   handling the non-option ARGV-elements.
   See the comments about RETURN_IN_ORDER and REQUIRE_ORDER, above.

   Long-named options begin with `--' instead of `-'.
   Their names may be abbreviated as long as the abbreviation is unique
   or is an exact match for some defined option.  If they have an
   argument, it follows the option name in the same ARGV-element, separated
   from the option name by a `=', or else the in next ARGV-element.
   When `getopt' finds a long-named option, it returns 0 if that option's
   `flag' field is nonzero, the value of the option's `val' field
   if the `flag' field is zero.

   The elements of ARGV aren't really const, because we permute them.
   But we pretend they're const in the prototype to be compatible
   with other systems.

   LONGOPTS is a vector of `struct option' terminated by an
   element containing a name which is zero.

   LONGIND returns the index in LONGOPT of the long-named option found.
   It is only valid when a long-named option has been found by the most
   recent call.

   If LONG_ONLY is nonzero, '-' as well as '--' can introduce
   long-named options.  */

int  UsfCasCor::_getopt_internal (int           argc,
                                  char* const   *argv,
                                  const char*   optstring,
                                  option*       longopts,
                                  int*          longind,
                                  int           long_only
                                 )

{
  int  print_errors = opterr;
  if  (optstring[0] == ':')
    print_errors = 0;

  if (argc < 1)
    return -1;

  optarg = NULL;

  if  (optind == 0 || !__getopt_initialized)
  {
    if  (optind == 0)
      optind = 1;  /* Don't scan ARGV[0], the program name.  */
    optstring = _getopt_initialize (argc, argv, optstring);
    __getopt_initialized = 1;
  }

  /* Test whether ARGV[optind] points to a non-option argument.
     Either it does not have option syntax, or there is an environment flag
     from the shell indicating it is not an option.  The later information
     is only used when the used in the GNU libc.  */
# define NONOPTION_P (argv[optind][0] != '-' || argv[optind][1] == '\0')

  if  (nextchar == NULL || *nextchar == '\0')
  {
    /* Advance to the next ARGV-element.  */

    /* Give FIRST_NONOPT & LAST_NONOPT rational values if OPTIND has been moved back by the user (who may also have changed the arguments).  */
    if  (last_nonopt > optind)
      last_nonopt = optind;

    if  (first_nonopt > optind)
      first_nonopt = optind;

    if  (ordering == PERMUTE)
    {
      /* If we have just processed some options following some non-options, exchange them so that the options come first.  */

      if  (first_nonopt != last_nonopt  &&  last_nonopt != optind)
        exchange ((char **) argv);

      else if  (last_nonopt != optind)
        first_nonopt = optind;

      /* Skip any additional non-options and extend the range of non-options previously skipped.  */

      while (optind < argc && NONOPTION_P)
        optind++;
      last_nonopt = optind;
    }

    /* The special ARGV-element `--' means premature end of options. Skip it like a null option,
     * then exchange with previous non-options as if it were an option, then skip everything else 
     * like a non-option.  
     */

    if (optind != argc && !strcmp (argv[optind], "--"))
    {
      optind++;

      if  (first_nonopt != last_nonopt  &&  last_nonopt != optind)
        exchange ((char **) argv);

      else if  (first_nonopt == last_nonopt)
        first_nonopt = optind;

      last_nonopt = argc;

      optind = argc;
    }

    /* If we have done all the ARGV-elements, stop the scan and back over any non-options that we skipped and permuted.  */

    if  (optind == argc)
    {
      /* Set the next-arg-index to point at the non-options
         that we previously skipped, so the caller will digest them.  */
      if (first_nonopt != last_nonopt)
        optind = first_nonopt;
      return -1;
    }

    /* If we have come to a non-option and did not permute it,  either stop the scan or describe it to the caller and pass it by.  */
    if  (NONOPTION_P)
    {
      if  (ordering == REQUIRE_ORDER)
        return -1;
      optarg = argv[optind++];
      return 1;
    }

    /* We have found another option-ARGV-element. Skip the initial punctuation.  */

    nextchar = (argv[optind] + 1
    + (longopts != NULL && argv[optind][1] == '-'));
  }

  /* Decode the current option-ARGV-element.  */

  /* Check whether the ARGV-element is a long option.

     If long_only and the ARGV-element has the form "-f", where f is
     a valid short option, don't consider it an abbreviated form of
     a long option that starts with f.  Otherwise there would be no
     way to give the -f short option.

     On the other hand, if there's a long option "fubar" and
     the ARGV-element is "-fu", do consider that an abbreviation of
     the long option, just like "--fu", and not "-f" with arg "u".

     This distinction seems to be the most useful approach.  */

  if (longopts != NULL   &&  (argv[optind][1] == '-' || (long_only && (argv[optind][2] || !my_index (optstring, argv[optind][1])))))
  {
    char *nameend;
    option* p;
    const struct option *pfound = NULL;
    int exact = 0;
    int ambig = 0;
    int indfound = -1;
    int option_index;

    for   (nameend = nextchar; *nameend && *nameend != '='; nameend++)
       /* Do nothing.  */ ;

    /* Test all long options for either exact match or abbreviated matches.  */
    for  (p = longopts, option_index = 0;  p->name;  p++, option_index++)
      if  (!strncmp (p->name, nextchar, nameend - nextchar))
      {
        if  ((unsigned int) (nameend - nextchar) == (unsigned int) strlen (p->name))
        {
          /* Exact match found.  */
          pfound = p;
          indfound = option_index;
          exact = 1;
          break;
        }
        else if  (pfound == NULL)
        {
          /* First nonexact match found.  */
          pfound = p;
          indfound = option_index;
        }
        else if  (long_only  || pfound->has_arg != p->has_arg  || pfound->flag != p->flag  || pfound->val != p->val)
        {
          /* Second or later nonexact match found.  */
          ambig = 1;
        }
      }

    if  (ambig && !exact)
    {
      if (print_errors)
        fprintf (stderr, _("%s: option `%s' is ambiguous\n"), argv[0], argv[optind]);
      nextchar += strlen (nextchar);
      optind++;
      optopt = 0;
      return '?';
    }

    if (pfound != NULL)
    {
      option_index = indfound;
      optind++;
      if ( *nameend)
      {
        /* Don't test has_arg with >, because some C compilers don't allow it to be used on enums.  */
        if  (pfound->has_arg)
          optarg = nameend + 1;
        else
        {
          if (print_errors)
          {
            if  (argv[optind - 1][1] == '-')
            {
               /* --option */
               fprintf (stderr, 
                        _("%s: option `--%s' doesn't allow an argument\n"),
                        argv[0], 
                        pfound->name
                       );
            }
            else
            {
              /* +option or -option */
              fprintf (stderr,
                       _("%s: option `%c%s' doesn't allow an argument\n"),
                       argv[0], argv[optind - 1][0], pfound->name);
            }
          }

          nextchar += strlen (nextchar);

          optopt = pfound->val;
          return '?';
        }
      }

      else  if (pfound->has_arg == 1)
      {
        if  (optind < argc)
          optarg = argv[optind++];
        else
        {
          if  (print_errors)
             fprintf (stderr,
                      _("%s: option `%s' requires an argument\n"),
                      argv[0], 
                      argv[optind - 1]
                     );
          nextchar += strlen (nextchar);
          optopt = pfound->val;
          return optstring[0] == ':' ? ':' : '?';
        }
      }
      nextchar += strlen (nextchar);
      if  (longind != NULL)
        *longind = option_index;
      if (pfound->flag)
      {
        *(pfound->flag) = pfound->val;
        return 0;
      }
      return pfound->val;
    }

    /* Can't find it as a long option.  If this is not getopt_long_only, or the option starts with '--' or is not a valid short
     * option, then it's an error. Otherwise interpret it as a short option.  */
    if (!long_only  ||  argv[optind][1] == '-'  || my_index (optstring, *nextchar) == NULL)
    {
      if (print_errors)
      {
        if (argv[optind][1] == '-')
        {
          /* --option */
          fprintf (stderr, 
                   _("%s: unrecognized option `--%s'\n"),
                   argv[0], 
                   nextchar
                  );
        }
        else
        {
          /* +option or -option */
          fprintf (stderr, 
                   _("%s: unrecognized option `%c%s'\n"),
                   argv[0], 
                   argv[optind][0], 
                   nextchar
                  );
        }
      }
      nextchar = (char *) "";
      optind++;
      optopt = 0;
      return '?';
    }
  }

  /* Look at and handle the next short option-character.  */

  {
    char c = *nextchar++;
    char *temp = my_index (optstring, c);

    /* Increment `optind' when we start to process its last character.  */
    if (*nextchar == '\0')
      ++optind;

    if (temp == NULL || c == ':')
    {
      if  (print_errors)
      {
        if (posixly_correct)
        {
          /* 1003.2 specifies the format of this message.  */
          fprintf (stderr, 
                   _("%s: illegal option -- %c\n"),
                   argv[0], 
                   c
                  );
        }
        else
        {
          fprintf (stderr, 
                   _("%s: invalid option -- %c\n"),
                   argv[0], 
                   c
                  );
        }
      }
      optopt = c;
      return '?';
    }

    /* Convenience. Treat POSIX -W foo same as long option --foo */
    if  (temp[0] == 'W' && temp[1] == ';')
    {
      char *nameend;
      const option*  p;
      const option*  pfound = NULL;
      int exact = 0;
      int ambig = 0;
      int indfound = 0;
      int option_index;

      /* This is an option that requires an argument.  */
      if  (*nextchar != '\0')
      {
        optarg = nextchar;
        /* If we end this ARGV-element by taking the rest as an arg, we must advance to the next element now.  */
        optind++;
      }
      else if (optind == argc)
      {
        if (print_errors)
        {
          /* 1003.2 specifies the format of this message.  */
          fprintf (stderr, 
                   _("%s: option requires an argument -- %c\n"),
                   argv[0], 
                   c
                  );
        }
        optopt = c;
        if  (optstring[0] == ':')
          c = ':';
        else
          c = '?';
        return c;
      }
      else
      {
        /* We already incremented `optind' once; increment it again when taking next ARGV-elt as argument.  */
        optarg = argv[optind++];
      }

      /* optarg is now the argument, see if it's in the table of longopts.  */

      for  (nextchar = nameend = optarg; *nameend && *nameend != '=';  nameend++)
        /* Do nothing.  */ ;

      /* Test all long options for either exact match or abbreviated matches.  */
      for  (p = longopts, option_index = 0;  p->name;  p++, option_index++)
        if (!strncmp (p->name, nextchar, nameend - nextchar))
        {
          if ((unsigned int) (nameend - nextchar) == strlen (p->name))
          {
            /* Exact match found.  */
            pfound = p;
            indfound = option_index;
            exact = 1;
            break;
          }

          else if (pfound == NULL)
          {
            /* First nonexact match found.  */
            pfound = p;
            indfound = option_index;
          }
          
          else
          {
            /* Second or later nonexact match found.  */
            ambig = 1;
          }
        }

      if  (ambig && !exact)
      {
        if (print_errors)
          fprintf (stderr, _("%s: option `-W %s' is ambiguous\n"),
             argv[0], argv[optind]);
        nextchar += strlen (nextchar);
        optind++;
        return '?';
      }

      if  (pfound != NULL)
      {
        option_index = indfound;
        if  (*nameend)
        {
          /* Don't test has_arg with >, because some C compilers don't allow it to be used on enums.  */
          if  (pfound->has_arg)
            optarg = nameend + 1;
          else
          {
            if  (print_errors)
            {
              fprintf (stderr, 
                       _("%s: option `-W %s' doesn't allow an argument\n"),
                       argv[0], 
                       pfound->name
                      );
            }
            nextchar += strlen (nextchar);
            return '?';
          }
        }
        else if  (pfound->has_arg == 1)
        {
          if  (optind < argc)
          {
            optarg = argv[optind++];
          }
          else
          {
            if  (print_errors)
            {
              fprintf (stderr,
                       _("%s: option `%s' requires an argument\n"),
                       argv[0], 
                       argv[optind - 1]
                      );
            }
            nextchar += strlen (nextchar);
            return optstring[0] == ':' ? ':' : '?';
          }
        }

        nextchar += strlen (nextchar);
        if  (longind != NULL)
          *longind = option_index;
        if  (pfound->flag)
        {
          *(pfound->flag) = pfound->val;
          return 0;
        }
        return pfound->val;
      }
      nextchar = NULL;
      return 'W';  /* Let the application handle it.   */
    }

    if  (temp[1] == ':')
    {
      if  (temp[2] == ':')
      {
        /* This is an option that accepts an argument optionally.  */
        if (*nextchar != '\0')
        {
          optarg = nextchar;
          optind++;
        }
        else
          optarg = NULL;
        nextchar = NULL;
      }
      else
      {
        /* This is an option that requires an argument.  */
        if (*nextchar != '\0')
        {
          optarg = nextchar;
          /* If we end this ARGV-element by taking the rest as an arg, we must advance to the next element now.  */
          optind++;
        }

        else if (optind == argc)
        {
          if (print_errors)
          {
            /* 1003.2 specifies the format of this message.  */
            fprintf (stderr,
                     _("%s: option requires an argument -- %c\n"),
                     argv[0], 
                     c
                    );
          }
          optopt = c;
          if  (optstring[0] == ':')
            c = ':';
          else
            c = '?';
        }
        else
        {
          /* We already incremented `optind' once; increment it again when taking next ARGV-elt as argument.  */
          optarg = argv[optind++];
        }
        nextchar = NULL;
      }
    }
    return c;
  }
}  /* _getopt_internal */





int  UsfCasCor::getopt (int          argc,
                        char* const  *argv,
                        const char*  optstring
                       )
{
  return _getopt_internal (argc, 
                           argv, 
                           optstring,
                           (option*)0,
                           (int*)0,
                           0
                          );
}


int  UsfCasCor::getopt_long (int          argc,
                             char* const  *argv,
                             const char*  options,
                             option*      long_options,
                             int*         opt_index
                            )
{
  return _getopt_internal (argc, argv, options, long_options, opt_index, 0);
}






//****************************************************************************
//*                                utils.c                                   *
//****************************************************************************

void  UsfCasCor::info(char *msg, ...)
{
   va_list ap;
  
   va_start(ap, msg);
   vfprintf(stderr, msg, ap);
   
   return;
}


void  UsfCasCor::warn(char *msg, ...)
{
   va_list ap;
   va_start(ap,msg);

   vfprintf(stderr, msg, ap);
   return;

}

void  UsfCasCor::debug (int level, char *msg, ...)
{
   va_list ap;
   va_start(ap,msg);
   if ( debug_level >= level ) 
      vfprintf(stderr,msg, ap);

   return;
}


void  UsfCasCor::die (char *msg,...)
{
   va_list ap;
   va_start(ap,msg);
 
   vfprintf(stderr, msg, ap); 

   char buffer[1024];
   cin >> buffer;
   
}


void*  UsfCasCor::MALLOC (int size)
{
  register void *p; 

  if ( (p=(void *)malloc(size)) == 0 ) { 
    fprintf(stderr, "error allocating memory\n");
    exit(1); 
  }
  
  return p;
}



void*  UsfCasCor::CALLOC (int n, int size)
{
  register void *p;

  if ( (p=(void *)calloc(n,size)) == 0 ) { 
    fprintf(stderr, "error allocating memory\n");
    exit(1);
  }
  return p; 
}



void*  UsfCasCor::REALLOC (void *q, int n)
{
  register void *p; 
  if ( (p=(void *)realloc(q,n)) == 0 ) {
    fprintf(stderr, "error re-allocating memory\n");
    exit(1); 
  }
  return  p;  
}



FILE*  UsfCasCor::FOPEN(char *filename, char *mode)
{
  FILE *fp; 

  if ( (fp=fopen(filename, mode)) == 0 ) {
    fprintf(stderr,"Error opening %s (mode %s): %s\n",
	    filename, mode, strerror(errno));
    exit(1);
  }

  return fp;
}



void  UsfCasCor::chomp (char *q)
{
  register char *p=q; 
  register int plen=strlen(p); 
  if  ( p != 0 && plen > 0 ) 
    p[plen-1]=0; 

  return;
}



//**********************************************************************************
//*                                 util.c                                         *
//**********************************************************************************



float  UsfCasCor::random_weight ()
{
  return ( (float) (WeightRange * (rand()%1000 / 500.0)) - WeightRange);
}



char*  UsfCasCor::trim (char *p)
{
  char *q,*r;

  /* Trim leading spaces */
  for (q=p; *q != 0 && isspace(*q); q++);
  /* Strip off trailing spaces */
  for (r=q+strlen(q)-1; r >= q && isspace(*r); r--) *r=0;
  /* And check for the last character a . */
  if ( r >= q && *r=='.') *r=0;
  return q;
}


/* Additional functions */
int  UsfCasCor::class_index (char *label)
{
  int i;
  int endoflabel;
  
  if ( label == NULL || (endoflabel=strlen(label)-1) < 0) 
    die("Class label not correct");

  if ( label[endoflabel] == '.') label[endoflabel]=0;

  for (i=0; i < number_of_classes;i++) {
    if (strcmp(label, class_label[i]) == 0 )
      return i;
  }

  die("Class label %s not found\n", label);

  return -1;
}  /* class_index */







//**********************************************************************************
//*                                 netio.c                                        *
//**********************************************************************************

#define BEGIN_PARAMETER_STRING "# Parameters\n"
#define END_PARAMETER_STRING   "# End Parameters\n"
#define BEGIN_CONNECTION_STRING "# Connections\n"
#define END_CONNECTION_STRING   "# End Connections\n"
#define BEGIN_INPUT_WEIGHTS_STRING "# Input Weights\n"
#define END_INPUT_WEIGHTS_STRING "# End Input Weights\n"
#define BEGIN_OUTPUT_WEIGHTS_STRING "# Output Weights\n"
#define END_OUTPUT_WEIGHTS_STRING "# End Output Weights\n"





/* 
   Save all of the parameters needed to a file, so that the entire 
   network state can be restored later.
*/
void  UsfCasCor::save_network (int current_trial, 
                               int current_round
                              )
{
  FILE *nfp;
  long long tt;
  char fname[LINELEN];
  int i,j;
  /***************/

  /* The default filename is filestem-trial-rounds.net */
  if  (current_round == -1)
    sprintf(fname,"%s-%d.network",filestem,current_trial);
  else
    sprintf(fname,"%s-%d-%d.network",filestem,current_trial,current_round);

  nfp=FOPEN(fname,"w");
  tt = GetLocalDateTime ();

  fprintf(nfp,"# Net file saved from Cascor Version %s\n", version);
  fprintf(nfp,"# Saved at %s#\n", ctime(&tt));


  /* Dump Architecture */
  fprintf(nfp,"# Architecture\n");
  fprintf(nfp, "number_of_units=%d\nnumber_of_outputs=%d\n",Nunits,Noutputs);



  /* Save parameters */
  fputs(BEGIN_PARAMETER_STRING,nfp);
  for (i=0; i<Nparameters; i++)
    fputs(parm_to_string(i),nfp);
  fputs(END_PARAMETER_STRING,nfp);


  /* Dump connections */
  fputs(BEGIN_CONNECTION_STRING,nfp);
  for(i=0; i<Nunits; i++)
    fprintf(nfp, "connections[%d]=%d\n",i, Nconnections[i]);
  fputs(END_CONNECTION_STRING,nfp);

  /* Dump Input Weights */
  fputs(BEGIN_INPUT_WEIGHTS_STRING,nfp);
  for(i=0; i<Nunits; i++)
    if(Nconnections[i]){
      for(j=0; j<Nconnections[i]; j++)
	fprintf(nfp, "input_weight[%d][%d]=%17.15g\n", i,j,Weights[i][j]);
    }
  fputs(END_INPUT_WEIGHTS_STRING,nfp);

  /* Dump Output Weights */
  fputs(BEGIN_OUTPUT_WEIGHTS_STRING,nfp);
  for(i=0; i<Noutputs; i++){
    for(j=0; j<Nunits; j++)
      fprintf(nfp, "output_weight[%d][%d]=%17.15g\n", i,j,OutputWeights[i][j]);
  }
  fputs(END_OUTPUT_WEIGHTS_STRING,nfp);

  /* All done */
  fclose(nfp);

  return;
}  /* save_network */






/* Read a .wgt file and do limited consistency checking with the input
 * from the .net file.  Space must be allocated for the Weight vectors in 
 * this routine.  During normal training the space is allocated in 
 * INSTALL_NEW_UNIT.
 */
void  UsfCasCor::load_network(char *netfile)
{
  int i,j, noutputs, nunits;
  int tmpi, tmpj;
  int tmp;
  float *w=NULL;			/* temporary weight array */
  char junk[LINELEN];
  FILE *nfp;
  /***************/

  info("Loading configuration from %s\n", netfile);

  /* open network configuration file */
  nfp=FOPEN(netfile,"r");

  /* First, we need to get both the particular parameters we need,
     as well as all of the general-purpose parameters */
  while ( fgets(junk,LINELEN,nfp) != NULL ) {
    if ( sscanf(junk,"number_of_units=%d", &nunits) > 0 ) continue;
    if ( sscanf(junk,"number_of_outputs=%d", &noutputs) > 0 ) continue;
    /* We should have a marker that denotes when parameters start
       and end. We stop when parameters end.
    */
    if ( !strcmp(END_PARAMETER_STRING, junk) ) break;
    if ( junk[0] == '#' ) continue;
    process_line(junk);
  }


  /* Sanity checks before reading in weight values */
  /* check to see if this matches the .net file inputs. */
  if(noutputs != Noutputs){
    warn("noutputs in net file does not match Noutputs.\n  Training will start from scratch.");
    return;
  }
  if(nunits > MaxUnits){
    die("nunits(%d) in .wgt file is greater than MaxUnits in .net\n  Please change MaxUnits and try again.", nunits);
  }
    
  /* Set globals from input. */
  Nunits = nunits;
  Noutputs = noutputs;
    

  /* Get Nconnections */
  while ( fgets(junk,LINELEN,nfp) != NULL ) 
    if ( !strcmp(BEGIN_CONNECTION_STRING,junk) ) break;
  for(i=0; i<Nunits; i++) {
    fscanf(nfp, "connections[%d]=%d\n",&tmp, &Nconnections[i]);
    if ( tmp != i ) die("Error parsing net file\n");
  }

  /* Get Input Weights */
  while (fgets(junk,LINELEN,nfp) != NULL )
    if ( !strcmp(BEGIN_INPUT_WEIGHTS_STRING,junk) ) break;
  for(i=0; i<Nunits; i++) {
    if(Nconnections[i]){
      w = (float *)CALLOC(Nconnections[i], sizeof(float));   
      for(j=0; j<Nconnections[i]; j++) {
	fscanf(nfp, "input_weight[%d][%d]=%f\n", &tmpi, &tmpj, &w[j]);
	if ( tmpi != i || tmpj != j ) die("Error parsing net file\n");
      }
      Weights[i] = w;
      Connections[i] = AllConnections;
    }
  }

  /* Get Output Weights */
  while (fgets(junk,LINELEN, nfp) != NULL )
    if ( !strcmp(BEGIN_OUTPUT_WEIGHTS_STRING,junk) ) break;

  for(i=0; i<Noutputs; i++) {
    for(j=0; j<Nunits; j++) {
      fscanf(nfp, "output_weight[%d][%d]=%f\n",&tmpi, &tmpj,
	     &OutputWeights[i][j]);
      if ( tmpi != i || tmpj != j ) die("Error parsing net file\n");
    }
  }

  fclose(nfp);			/* close the weight file. */
    
  /* If using cache, run an epoch to compute hidden units' values.  */
  if(UseCache) {

     /* Odd error check. If usecache was specified in file, but not on
        command line, then Cache was not allocated. We look for NULL
        value and allocate storage here. */
     if(ValuesCache == NULL ){
       ValuesCache = (float **)CALLOC(MaxCases, sizeof(float *));
       ErrorsCache = (float **)CALLOC(MaxCases, sizeof(float *));

       for(i=0; i<MaxCases; i++){
         ValuesCache[i] = (float *)CALLOC(MaxUnits, sizeof(float));
         ErrorsCache[i] = (float *)CALLOC(Noutputs, sizeof(float));
       }
     }


    for(i=0; i<NTrainingPatterns; i++){
      Values = ValuesCache[i];
      
      /* Unit values must be calculated in order because the activations */
      /* cascade down through the hidden layers */
      for(j= 1+Ninputs; j<Nunits; j++) 
	COMPUTE_UNIT_VALUE(j);
    }
  }

  return;
}  /* load_network */





//******************************************************************************************
//*                                   load_namesfile.c                                     *
//******************************************************************************************
#define N 1024
#define ALLOC_INCR 100



/* The guts of the transformation to C4.5-like format. 
   We have to read in both the data and test files, to get the number of
   patterns from each.
   We also have to open the names file and figure out:
    1) Class names (assign indices to them)
    2) number of features
*/



void  UsfCasCor::load_namesfile (char *filestem)
{
  /* First, ensure the necessary variables are reset */
  NTrainingPatterns = -1;
  NTestPatterns     = -1;
  line_length       = -1;

  /* It is possible to get everything out of the names file. */
  process_namesfile (filestem);

  /* If not everything was included in the names file, we have 
     to get extra information */
  get_example_counts (filestem);

  return;
}






void  UsfCasCor::process_namesfile (char *filestem) 
{
  char filename[LINELEN+1];
  FILE *fp;
  char s[N], *p, *q, *r;
  Boolean seen_classes=False;
  int feature_count=0;
  int i;

  Ninputs=0;

  sprintf  (filename, "%s.names", filestem);
  fp = FOPEN (filename,"r");

  /* First useful line will be the class information */
  while ((p=fgets (s, N, fp)) != NULL) 
  {
    /* All of this strips leading/trailing spaces */
    p = trim (s);
    if  (strlen (p) <= 0 ) 
      continue;

    /* Check for various meta-data in the form of |@ comments */
    if  (!strncmp (p, "|@LINELENGTH=",13)) 
    {
      line_length=atoi(p+13);
      continue;
    } 

    else if  (!strncmp (p, "|@TRAINEXAMPLES=", 16))
    {
      NTrainingPatterns=atoi (p+16);
      continue;
    } 
    
    else if  (!strncmp(p, "|@TESTEXAMPLES=", 15)) 
    {
      NTestPatterns=atoi (p + 15);
      continue;
    }
    
    else if  (!strncmp (p, "|@DESCRIPTION=",14)) 
    {
      info ("-- %s\n", p + 14);
      continue;
    }

    /* Ignore any other comments */
    if  (*p == '|') 
       continue;

    /* Now we have a fairly clean line, figure out what to do with it */
    if  (!seen_classes) 
    {   
      /* This is the classes line                                          */
      /* First, go through the string and find the number of commas. This  */
	  /* is the number of classes-1                                        */
      number_of_classes = 1;
      for  (q = p; *q != 0; q++) 
         if  (*q == ',') 
            number_of_classes++;

      class_label = (char **)CALLOC (number_of_classes, sizeof (char *));

      for (i = 0; i < number_of_classes; i++) 
      {
        if  ((q = index (p, ','))  !=  NULL) 
        {
          *q=0;
          q++;
        }

        class_label[i] = strdup (trim (p));
        p = q;
      }

      seen_classes=True;
    } 
    
    else 
    {
      /* Else this is a feature description line of the type  */
	  /* feature name: type.                                  */

      if  ((q = index (p, ':')) != NULL)
      {
        /* Make sure feature_type has enough examples */
        if  (feature_count % ALLOC_INCR == 0)
           feature_type = (int*)REALLOC (feature_type, sizeof (int) * (feature_count + ALLOC_INCR));

        /* Find a clean description of the feature */
        q++;
        r = trim (q);

        /* If it's ignore, note that */
        if  (!strncasecmp("uccIGNORE", r, 6)) 
        {
          feature_type[feature_count]=uccIGNORE;
        } 

        else if  (!strncasecmp ("BINARY", r, 6)) 
        {
	      /* In the case of binary, allocate that many floats */
          int n,k;
          n = atoi (r + 6);
          feature_type = (int*)REALLOC (feature_type, sizeof (int) * (feature_count + n));
          for (k = 0; k < n; k++) 
          {
            feature_type[feature_count] = BINARY;
            Ninputs++;
            feature_count++;
          }

	      feature_count--; // Kludge for below
        } 

        else 
        {
          feature_type[feature_count] = REAL;
          Ninputs++;
        }

        feature_count++;
      }
    }
  }

  fclose (fp);

  /* Setup nn variables */
  Noutputs = number_of_classes;

  return;
}




void  UsfCasCor::get_example_counts (char *filestem)
{
  char filename[LINELEN];
  FILE *fp;
  int num_lines,c;


  if  (NTrainingPatterns == -1) 
  {
    /* Easy stuff first, get the number of lines from data and test,
       they correspond to the number of patterns */
    if  (use_index_files) 
      sprintf (filename, "%s.idata", filestem);
    else 
      sprintf (filename, "%s.data",  filestem);
  
    fp = FOPEN (filename, "r");
    
    num_lines = 0;
    while ((c = fgetc(fp)) != EOF ) 
    {
      if  (c == '\n' && (c = fgetc (fp)) != '\n') 
        num_lines++;
    }

    fclose (fp);
    NTrainingPatterns = num_lines;
  }


  /* This needs to handle resubstitution better */
  if  (NTestPatterns == -1) 
  {
    if  (use_index_files) 
      sprintf (filename, "%s.itest", filestem);
    else 
      sprintf (filename, "%s.test",  filestem);

    fp = FOPEN (filename,"r");
    
    num_lines = 0;
    while  ((c = fgetc (fp)) != EOF) 
    {
      if (c == '\n' && (c = fgetc(fp)) != '\n') 
	    num_lines++;
    }

    fclose (fp);
    NTestPatterns = num_lines;
  }


  /* Here we peek to get the length of a line */
  if  (use_index_files  &&  line_length == -1) 
  {
    sprintf (filename,"%s.data", filestem);
    fp = FOPEN (filename,"r");
    
    for  (line_length=1; (c=fgetc(fp)) && c != '\n'; line_length++);

    fclose(fp);
  }

  return;
}  /* get_example_counts */









/***********************************************************************/
/*                             learn.c                                 */
/***********************************************************************/


/*
 * Given the sum of weighted inputs, compute the unit's activation value.
 * Defined unit types are SIGMOID, VARSIGMOID, and GAUSSIAN.
 */
float  UsfCasCor::ACTIVATION (float sum)
{
  float temp;

  switch(UnitType)
  {
  case SIGMOID: 
    /* Sigmoid function in range -0.5 to 0.5. */
    if      (sum < -15.0)  
      return(-0.5f);
    
    else if (sum >  15.0)
      return(0.5f);

    else
      return (1.0f / (1.0f + exp (-sum)) - 0.5f);

  case GAUSSIAN:
    /* Gaussian activation function in range 0.0 to 1.0. */
    temp = -0.5f * sum * sum;
    if  (temp < -75.0f)
      return  (0.0f);
    else 
      return (exp (temp));

  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    if  (sum < -15.0f) 
      return (0.0f);
    else if  (sum > 15.0f) 
      return (1.0f);
    else 
      return (1.0f  / (1.0f + exp(-sum)));


  case VARSIGMOID: 
    /* Sigmoid function in range SigmoidMin to SigmoidMax. */
    if  (sum < -15.0) 
      return (SigmoidMin);

    else if   (sum > 15.0f) 
      return (SigmoidMax);
    
    else 
      return ((SigmoidMax - SigmoidMin)/ (1.0f + exp(-sum)) + SigmoidMin);
  }
  return -1;
}  /* ACTIVATION */




/*
 * Given the unit's activation value and sum of weighted inputs, compute
 * the derivative of the activation with respect to the sum.  Defined unit
 * types are SIGMOID, VARSIGMOID, and GAUSSIAN.
 *
 * Note: do not use sigmoid prime offset here, as it confuses the
 * correlation machinery.  But do use it in output-prime.
 * 
 */
float  UsfCasCor::ACTIVATION_PRIME (float value, 
                                    float sum
                                   )
{
  switch(UnitType)
  {
  case SIGMOID: 
    /* Symmetrical sigmoid function. */
    return (0.25f -  value * value);

  case GAUSSIAN:
    /* Gaussian activation function. */
    return (sum * (- value));

  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    return (value * (1.0f - value));

  case VARSIGMOID: 
    /* Sigmoid function with range SigmoidMin to SigmoidMax. */
    return ((value - SigmoidMin) * (1.0f - (value - SigmoidMin) / 
				    (SigmoidMax - SigmoidMin)));

  }
  return -1.0f;
}  /* ACTIVATION_PRIME */




/* Compute the value of an output, given the weighted sum of incoming values.
 * Defined output types are SIGMOID, ASYMSIGMOID, and LINEAR.
 */
float  UsfCasCor::OUTPUT_FUNCTION (float sum)
{
  switch(OutputType)
  {
  case SIGMOID: 
    /* Symmetrical sigmoid function, used for binary functions. */
    if  (sum < -15.0) 
      return (-0.5f);

    else if  (sum > 15.0f) 
      return (0.5f);
    
    else 
      return (1.0f / (1.0f + exp (-sum)) - 0.5f);

  case LINEAR:
    /* Linear output function, used for continuous functions. */
    return (sum);

  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    if  (sum < -15.0f) 
      return (0.0f);

    else if  (sum > 15.0f) 
      return (1.0f);

    else 
      return (1.0f / (1.0f + exp (-sum)));

  case VARSIGMOID:
    /* Sigmoid function in range SigmoidMin to SigmoidMax. */
    if  (sum < -15.0f) 
      return (SigmoidMin);

    else if  (sum > 15.0f) 
      return(SigmoidMax);

    else 
      return ((SigmoidMax - SigmoidMin) / (1.0f + exp (-sum)) + SigmoidMin);
  }
  return -1.0f;
}  /* OUTPUT_FUNCTION */




/* Compute the value of an output, given the weighted sum of incoming values.
 * Defined output types are SIGMOID, ASYMSIGMOID, and LINEAR.
 *
 * Sigmoid_Prime_Offset used to keep the back-prop error value from going to 
 * zero.
 */
float  UsfCasCor::OUTPUT_PRIME (float output)
{
  switch(OutputType)
  {
  case SIGMOID: 
    /* Symmetrical sigmoid function, used for binary functions. */
    return (SigmoidPrimeOffset + 0.25f -  output * output);

  case LINEAR:
    /* Linear output function, used for continuous functions. */
    return (1.0);

  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    return (SigmoidPrimeOffset + output * (1.0f - output));

  case VARSIGMOID: 
    /* Sigmoid function with range SigmoidMin to SigmoidMax. */
    return (SigmoidPrimeOffset + 
	    (output - SigmoidMin) * (1.0f - (output - SigmoidMin) / 
				     (SigmoidMax - SigmoidMin)));
  }

  return -1.0f;
}  /* OUTPUT_PRIME */




/* The basic routine for doing quickprop-style update of weights, given a
 * pair of slopes and a delta.
 *
 * Given arrays holding weights, deltas, slopes, and previous slopes,
 * and an index i, update weight[i] and delta[i] appropriately.  Move
 * slope[i] to prev[i] and zero out slope[i].  Add weight decay term to
 * each slope before doing the update.
 */
void  UsfCasCor::QUICKPROP_UPDATE (int i, 
                                   float weights[], 
                                   float deltas[], 
                                   float slopes[], 
		                               float prevs[], 
                                   float epsilon,
                                   float decay,
                                   float mu,
                                   float shrink_factor
                                  )
{
  float w,d,s,p, next_step;
  /********/

  w = weights[i];
  d = deltas[i];
  s = slopes[i] +  decay * w;
  p = prevs[i];
  next_step = 0.0f;

  /* The step must always be in direction opposite to the slope. */

  if(d < 0.0f){			
    /* If last step was negative...  */  
    if(s > 0.0f)	  
      /*  Add in linear term if current slope is still positive.*/
      next_step -= epsilon * s;
    /*If current slope is close to or larger than prev slope...  */
    if(s >= (shrink_factor*p)) 
      next_step += mu * d;	/* Take maximum size negative step. */
    else
      next_step += d * s / (p - s); /* Else, use quadratic estimate. */
  }
  else if(d > 0.0f){
    /* If last step was positive...  */
    if(s < 0.0f)	  
      /*  Add in linear term if current slope is still negative.*/
      next_step -= epsilon * s;
    /* If current slope is close to or more neg than prev slope... */
    if(s <= (shrink_factor*p)) 
      next_step += mu * d;	/* Take maximum size negative step. */
    else
      next_step += d * s / (p - s); /* Else, use quadratic estimate. */
  }
  else
    /* Last step was zero, so use only linear term. */
    next_step -= epsilon * s;
  
  /* update global data arrays */
  deltas[i] = next_step;
  weights[i] = w + next_step;
  prevs[i] = s;
  slopes[i] = 0.0;
}  /* QUICKPROP_UPDATE */




/* Set up all the inputs from the INPUT vector as the first few entries in
   in the values vector.
*/
void  UsfCasCor::SETUP_INPUTS (float inputs[])
{
  int i;
  /*********/

  Values[0]  = 1.0;		/* bias unit */
  for(i=0;  i < Ninputs;  i++)
    Values[i+1] = inputs[i];
}




/* Assume the values vector has been set up.  Just compute the output
   values.
*/
void  UsfCasCor::OUTPUT_FORWARD_PASS ()
{
  int i,j;
  float sum;
  float *ow;
/********/

  for(j=0; j<Noutputs; j++){
    sum = 0.0;
    ow  = OutputWeights[j];

    for(i=0; i<Nunits; i++)
      sum += Values[i] * ow[i];

#ifdef CONNX
      conx += Nunits;
#endif

    Outputs[j] = OUTPUT_FUNCTION(sum);
  }

}  /* OUTPUT_FORWARD_PASS */




/* Assume that values vector has been set up for units with index less
   than J.  Compute and record the value for unit J.
*/
void  UsfCasCor::COMPUTE_UNIT_VALUE (int j)
{
  int i;
  int   *c;		/* pointer to unit's connections array */
  float *w,		/* pointer to unit's weights array*/
        sum = 0.0;
/********/

  c = Connections[j];
  w = Weights[j];

  for(i=0; i<Nconnections[j]; i++)
    sum += Values[c[i]] * w[i];

#ifdef CONNX
    conx += Nconnections[j];
#endif

  Values[j] = ACTIVATION(sum);
}  /* COMPUTE_UNIT_VALUE */




/* Set up the inputs from the INPUT vector, then propagate activation values
   forward through all hidden units and output units.
*/
void  UsfCasCor::FULL_FORWARD_PASS (float input[])
{
  int j;
  /********/

  SETUP_INPUTS (input);

  /* Unit values must be calculated in order because the activations */
  /* cascade down through the hidden layers */

  for  (j = 1 + Ninputs;  j < Nunits;  j++) /* For each hidden unit J, compute the */
    COMPUTE_UNIT_VALUE (j);	     /* activation value. */

  OUTPUT_FORWARD_PASS ();	/* Now compute outputs. */
}  /* FULL_FORWARD_PASS */




/*  Goal is a vector of desired values for the output units.  Compute and
 *  record the output errors for the current training case.  Record error
 *  values and related statistics.  If output_slopesp is TRUE, then use errors
 *  to compute slopes for output weights.  If statsp is TRUE, accumulate error
 *  statistics. 
 */
void  UsfCasCor::COMPUTE_ERRORS (float    goal[], 
                                 Boolean  output_slopesp, 
                                 Boolean  statsp, 
                                 int      xw
                                )
{
  int i,j;
  float out = 0.0,
        dif = 0.0,
        err_prime = 0.0;
  float *os;		/* pointer to unit's output slopes array */
  int goal_winner,output_winner;
/********/

  
  goal_winner=0;output_winner=0;
  for (i=1; i < Noutputs; i++) {
    if ( Outputs[output_winner] < Outputs[i] ) output_winner=i;
    if ( goal[goal_winner] < goal[i] ) goal_winner=i;
  }
  //    printf("Goal is %d: %f  %d: %f\n", goalWinner, goal[goalWinner],
  //                               outputWinner, Outputs[outputWinner]);
  if ( goal_winner != output_winner ) ErrorMisclassifications++;


  for(j=0; j<Noutputs; j++){
    out = Outputs[j];
    dif = out - goal[j];
    if ( load_weights && xw >= 0 && example_weight[xw] != 1.0 ) 
       dif *= example_weight[xw];
    err_prime = dif * OUTPUT_PRIME(out);
    os = OutputSlopes[j];

    Errors[j] = err_prime;

    if (statsp){
      if (fabs(dif) > ScoreThreshold) ErrorBits++;
      TrueError += dif * dif;
      SumErrors[j] += err_prime;
      SumSqError += err_prime * err_prime;
    }

    if (output_slopesp)
      for(i=0; i<Nunits; i++)
	os[i] += err_prime * Values[i];

  }				/* end for unit j */
}  /* COMPUTE_ERRORS */




/* Update the output weights, using the pre-computed slopes, prev-slopes,
 * and delta values.
 */
void  UsfCasCor::UPDATE_OUTPUT_WEIGHTS ()
{
  int i,j;
  float eps;			/* epsilon scaled by fan-in */
/********/

  eps = OutputEpsilon / Ncases;

  for  (j = 0;  j < Noutputs;  j++)
    for  (i = 0;  i < Nunits;  i++)
      QUICKPROP_UPDATE (i, 
                        OutputWeights[j], 
                        OutputDeltas[j],
                        OutputSlopes[j], 
                        OutputPrevSlopes[j], 
                        eps,
                        OutputDecay, 
                        OutputMu, 
                        OutputShrinkFactor
                       );

}




/***********************************************************************/
/*                                                                     */
/* The outer loops for training output weights.                        */
/*                                                                     */
/***********************************************************************/


/* Perform forward propagation once for each set of weights in the
 * training vectors, computing errors and slopes.  Then update the output
 * weights.
 */
void  UsfCasCor::TRAIN_OUTPUTS_EPOCH ()
{
  int i;
/********/

  /* zero error accumulators */
  ErrorBits = 0;
  TrueError = 0.0;
  ErrorMisclassifications = 0;
  for(i=0; i<Noutputs; i++){
    SumErrors[i] = 0.0;
  }
  SumSqError = 0.0;

  /* User may have changed mu between epochs, so fix shrink-factor. */
  OutputShrinkFactor = OutputMu / (1.0f + OutputMu);

  for(i= FirstCase; i<(FirstCase+Ncases); i++){
    Goal = TrainingOutputs[i];

    if(UseCache){
      Values = ValuesCache[i];
      Errors = ErrorsCache[i];
      OUTPUT_FORWARD_PASS();
    }
    else{
      Values = ExtraValues;
      Errors = ExtraErrors;
      FULL_FORWARD_PASS(TrainingInputs[i]);
    }
    COMPUTE_ERRORS(Goal, True, True, i);
  }
 
  switch (ErrorMeasure){
  case BITS:
    /* Do not change weights or count epoch if this run was a winner. */
    if(ErrorBits > 0){
      UPDATE_OUTPUT_WEIGHTS();
      Epoch++;
    }
    break;
  case INDEX:
    /* Compute index and don't change weights if we have a winner. */
    ErrorIndex = ERROR_INDEX(TrainingStdDev, NtrainingOutputValues);
    if(ErrorIndex > ErrorIndexThreshold){
      UPDATE_OUTPUT_WEIGHTS();
      Epoch++;
    }
    break;
  }
   
}  /* TRAIN_OUTPUTS_EPOCH */





/* Train the output weights.  If we exhaust max_epochs, stop with value
 * TIMEOUT.  If there are zero error bits, stop with value WIN.  Else,
 * keep going until the true error has changed by a significant amount,
 * and then until it does not change significantly for Patience epochs.
 * Then return STAGNANT.  If Patience is zero, we do not stop until victory
 * or until max_epochs is used up.
 */

int  UsfCasCor::TRAIN_OUTPUTS (int max_epochs)
{
  int i;
  int retval = TIMEOUT;	  /* will be reset within loop for other conditions */
  float last_error = 0.0;
  int quit_epoch = Epoch + OutputPatience;
  Boolean first_time = True;
/********/

  for(i=0; i<max_epochs; i++)
  {
    TRAIN_OUTPUTS_EPOCH();

    if  ((ErrorMeasure == BITS) &&  (ErrorBits == 0))
    {
      retval = WIN;
      break;
    }
    
    else if  ((ErrorMeasure == INDEX) && (ErrorIndex <= ErrorIndexThreshold))
    {
      retval = WIN;
      break;
    }
    
    else if  (OutputPatience == 0)
      continue;			/* continue training until victory */
    
    else if  (first_time)
    {
      first_time = False;
      last_error = TrueError;
    }

    else if  (fabs(TrueError - last_error) > (last_error * OutputChangeThreshold))
    {
       /* still getting better */
      last_error = TrueError;
      quit_epoch = Epoch + OutputPatience;
    }

    else if  (Epoch >= quit_epoch)
    { 
      /* haven't gotten better for a while */
      retval = STAGNANT;
      break;
    }
  }

  /* tell user about the output weights of new unit */
  /*for(o=0; o<Noutputs; o++){
    fprintf(stderr,"  Output %d Weights: ", o);
    for(i=0; i<Nunits; i++)
      fprintf(stderr,"%6f ", OutputWeights[o][i]);
    fprintf(stderr,"\n");
  }
  */

  /* return result,  will be TIMEOUT unless reset in loop */
  return(retval);
}  /* TRAIN_OUTPUTS */






/***********************************************************************/
/*                                                                     */
/*  Machinery for Training and selecting candidate units.              */
/*                                                                     */
/***********************************************************************/

/* Give new random weights to all of the candidate units.  Zero the other
 * candidate-unit statistics.
 */
void  UsfCasCor::INIT_CANDIDATES ()
{
  int i,j,o;
/********/

  for(i=0; i<Ncandidates; i++){
    CandValues[i] = 0.0;
    CandSumValues[i] = 0.0;
    for(j=0; j<Nunits; j++){
      CandWeights[i][j] = random_weight();
      CandDeltas[i][j] = 0.0;
      CandSlopes[i][j] = 0.0;
      CandPrevSlopes[i][j] = 0.0;
    }
    for(o=0; o<Noutputs; o++){
      CandCor[i][o] = 0.0;
      CandPrevCor[i][o] = 0.0;
    }
  }
}  /* INIT_CANDIDATES */



 
/* Add the candidate-unit with the best correlation score to the active
 * network.  Then reinitialize the candidate pool.
 */
void  UsfCasCor::INSTALL_NEW_UNIT ()
{
  int i,o;
  float wm;			/* temporary weight multiplier */
  float *w;			/* temporary weight array */
  float *cw;
/********/

  if(Nunits >= MaxUnits)
    die("Cannot add any more units.\n");

  Nconnections[Nunits] = Nunits;
  Connections[Nunits] = AllConnections;
  /* Set up the weight vector for the new unit. */
  w = (float *)CALLOC(Nunits, sizeof(float));
  cw = CandWeights[BestCandidate];
  for(i=0; i<Nunits; i++)
    w[i] = cw[i];
  Weights[Nunits] = w;

  /* Tell user about the new unit. */
  //for(i=0; i<Nunits; i++)
  //  fprintf(stderr,"%6f ", Weights[Nunits][i]);
  //fprintf(stderr,"\n");

  /* Fix up output weights for candidate unit.  Use minus the           */
  /* correlation times the WeightMultiplier as an initial guess.        */

  if(ErrorMeasure == BITS)
    wm = WeightMultiplier;
  else				/* ErrorMeasure == INDEX */
    wm = WeightMultiplier / (float)Nunits;

  for(o=0; o<Noutputs; o++)
    OutputWeights[o][Nunits] = -CandPrevCor[BestCandidate][o] * wm;

  /* If using cache, run an epoch to compute this unit's values.        */
  if  (UseCache)
    for(i=0; i<NTrainingPatterns; i++){
      Values = ValuesCache[i];
      COMPUTE_UNIT_VALUE(Nunits);
    }

  /* Reinitialize candidate units with random weights.                  */
  Nunits++;
  INIT_CANDIDATES();    
}  /* INSTALL_NEW_UNIT*/







/* Note: Ideally, after each adjustment of the candidate weights, we would  */
/* run two epochs.  The first would just determine the correlations         */
/* between the candidate unit outputs and the residual error.  Then, in a   */
/* second pass, we would adjust each candidate's input weights so as to     */
/* maximize the absolute value of the correlation.  We need to know the     */
/* direction to tune the input weights.                                     */
/*                                                                          */
/* Since this ideal method doubles the number of epochs required for        */
/* training candidates, we cheat slightly and use the correlation values    */
/* computed BEFORE the most recent weight update.  This combines the two    */
/* epochs, saving us almost a factor of two.  To bootstrap the process, we  */
/* begin with a single epoch that computes only the correlation.            */
/*                                                                          */
/* Since we look only at the sign of the correlation after the first ideal  */
/* epoch and since that sign should change very infrequently, this probably */
/* is OK.  But keep a lookout for pathological situations in which this     */
/* might cause oscillation.                                                 */

/* For the current training pattern, compute the value of each candidate
 * unit and begin to compute the correlation between that unit's value and
 * the error at each output.  We have already done a forward-prop and
 * computed the error values for active units.
 */
void  UsfCasCor::COMPUTE_CORRELATIONS ()
{
  int i,o,u;
  float sum=0.0;
  float v=0.0;
/*********/

  for(u=0; u<Ncandidates; u++){
    sum = 0.0;
    v = 0.0;
    /* Determine activation value of each candidate unit. */
    for(i=0; i<Nunits; i++)
      sum += CandWeights[u][i] * Values[i];
#ifdef CONNX
    conx += Nunits;
#endif
    v = ACTIVATION(sum);
    CandValues[u] = v;
    CandSumValues[u] += v;
    /* Accumulate value of each unit times error at each output. */
    for(o=0; o<Noutputs; o++)
      CandCor[u][o] += v * Errors[o];
  }
}  /* COMPUTE_CORRELATIONS */





/* NORMALIZE each accumulated correlation value, and stuff the normalized
 * form into the CandPrevCor data structure.  Then zero CandCor to
 * prepare for the next round.  Note the unit with the best total
 * correlation score.
 */
void  UsfCasCor::ADJUST_CORRELATIONS ()
{
  int o,u;
  float cor, offset, score, csv;
  float *cc, *cpc;
  float avg_value;
/*********/

  BestCandidate = 0;
  BestCandidateScore = 0.0;
  for(u=0; u<Ncandidates; u++){
    avg_value = CandSumValues[u] / Ncases;
    cor = 0.0;
    score = 0.0;
    cc = CandCor[u];
    cpc = CandPrevCor[u];
    for(o=0; o<Noutputs; o++){
      cor = (cc[o] - avg_value * SumErrors[o]) / SumSqError;
      cpc[o] = cor;
      cc[o] = 0.0;
      score += fabs(cor);
    }

    /* zero CandSumValues for next epoch */
    CandSumValues[u] = 0.0;
    /* Keep track of the candidate with the best overall correlation. */
    if(score > BestCandidateScore){
      BestCandidateScore = score;
      BestCandidate = u;
    }
  }
}  /* ADJUST_CORRELATIONS */





/* After the correlations have been computed, we do a second pass over
 * the training set and adjust the input weights of all candidate units.
 */
void  UsfCasCor::COMPUTE_SLOPES ()
{
  int i,o,u;
  float sum, value, actprime, direction, error, change;
/*********/

  for  (u=0; u<Ncandidates; u++)
  {
    sum = 0.0;
    value = 0.0;
    actprime = 0.0;
    direction = 0.0;
    change = 0.0;
    /* Forward pass through each candidate unit to compute activation-prime. */
    for(i=0; i<Nunits; i++)
      sum += CandWeights[u][i] * Values[i];
#ifdef CONNX
    conx += Nunits;
#endif
    value = ACTIVATION(sum);
    actprime = ACTIVATION_PRIME(value, sum);
    CandSumValues[u] += value;
    /* Now try to adjust the inputs so as to maximize the absolute value */
    /* of the correlation. */
    for(o=0; o<Noutputs; o++){
      error = Errors[o];
      direction = (CandPrevCor[u][o] < 0.0f) ? -1.0f : 1.0f;
      change -= direction * actprime *((error -SumErrors[o])/SumSqError);
      CandCor[u][o] += error * value;
    }
    for(i=0; i<Nunits; i++)
       CandSlopes[u][i] += change * Values[i];
  }
}  /* COMPUTE_SLOPES */




/* Update the input weights, using the pre-computed slopes, prev-slopes,
 * and delta values.
 */
void  UsfCasCor::UPDATE_INPUT_WEIGHTS ()
{
  int i,u;
  float eps;
  float *cw, *cd, *cs, *cp;
/*********/

  eps = InputEpsilon / (float)(Ncases * Nunits);
  for(u=0; u<Ncandidates; u++)
  {
    cw = CandWeights[u];
    cd = CandDeltas[u];
    cs = CandSlopes[u];
    cp = CandPrevSlopes[u];
    for(i=0; i<Nunits; i++)
      QUICKPROP_UPDATE(i, cw, cd, cs, cp, eps, InputDecay, InputMu, 
		       InputShrinkFactor);
  }
}  /* UPDATE_INPUT_WEIGHTS */




/* For each training pattern, perform a forward pass and compute correlations.
 * Then perform a second forward pass and compute input slopes for the 
 * candidate units.  Finally, use quickprop update to adjust the input weights.
 */

void  UsfCasCor::TRAIN_INPUTS_EPOCH ()
{
  int i;
/********/

  for(i=FirstCase; i<(Ncases+FirstCase); i++){
    Goal = TrainingOutputs[i];
    if(UseCache){
      Values = ValuesCache[i];
      Errors = ErrorsCache[i];
    }
    else {
      Values = ExtraValues;
      Errors = ExtraErrors;
      FULL_FORWARD_PASS(TrainingInputs[i]);
      COMPUTE_ERRORS(Goal, False, False, i);
     }
    COMPUTE_SLOPES();
  }
  /*  User may have changed mu between epochs, so fix shrink-factor.*/
  InputShrinkFactor = InputMu / (1.0f + InputMu);

  /* Now tweak the candidate unit input weights. */
  UPDATE_INPUT_WEIGHTS();

  /*  Fix up the correlation values for the next epoch.*/
  ADJUST_CORRELATIONS();
  Epoch++;
}




/* Do an epoch through all active training patterns just to compute the
 * correlations.  After this one pass, we will update the correlations as we
 * train.
 */
void  UsfCasCor::CORRELATIONS_EPOCH ()
{
  int i;
/********/

  for  (i=FirstCase; i<(Ncases+FirstCase); i++)
  {
    Goal = TrainingOutputs[i];
    if  (UseCache)
    {
      Values = ValuesCache[i];
      Errors = ErrorsCache[i];
    }
    else 
    {
      Values = ExtraValues;
      Errors = ExtraErrors;
      FULL_FORWARD_PASS(TrainingInputs[i]);
      COMPUTE_ERRORS(Goal, False, False, i);
    }
    COMPUTE_CORRELATIONS();
  }
  /*  Fix up the correlation values for the next epoch. */
  ADJUST_CORRELATIONS();
  Epoch++;
}  /* CORRELATIONS_EPOCH */




/* Train the input weights of all candidates.  If we exhaust max_epochs,
 * stop with value TIMEOUT.  Else, keep going until the best candidate unit's
 * score has changed by a significant amount, and then
 * until it does not change significantly for Patience epochs.  Then return
 * STAGNANT.  If Patience is zero, we do not stop until victory or until
 * max_epochs is used up.
 */
int  UsfCasCor::TRAIN_INPUTS (int max_epochs)
{
  int i;
  float last_score = 0.0;
  int quit = max_epochs;
  Boolean first_time = True;
/**********/

  for(i=0; i<Noutputs; i++)	/* Convert to the average error for use in */
    SumErrors[i]  /=  Ncases;	/* calculation of the correlation. */

  CORRELATIONS_EPOCH();

  for(i=0; i<max_epochs; i++){
    TRAIN_INPUTS_EPOCH();

    if(InputPatience == 0)
      continue;			/* continue training until victory */
    else if(first_time){
      first_time = False;
      last_score = BestCandidateScore;
    }
    else if(fabs(BestCandidateScore - last_score) > /* still getting better */
	    (last_score * InputChangeThreshold)){
      last_score = BestCandidateScore;
      quit = i + InputPatience;
    }
    else if(i >= quit) /* haven't gotten better for a while */
      return(STAGNANT);
  }

  /* didn't return within the loop, so must have run out of time. */
  return(TIMEOUT);

}  /* TRAIN_INPUTS */





/**********************************************************************/
/*                                                                    */
/*  The outer loop routines                                           */
/*                                                                    */
/**********************************************************************/


void  UsfCasCor::LIST_PARAMETERS ()
{
#ifdef __STDC__			/* does is compiler conform to the standard? */
  //fprintf(stderr,"\nCascor.c Version: %5.2f %s   Compiled: %s  %s\n", 
	 //VERSION, REL_DATE, __DATE__, __TIME__);
#else
  //fprintf(stderr,"\nCascor.c Version: %5.2f  %s\n", VERSION, REL_DATE);
#endif
  /*fprintf(stderr,"Trial Number %d Parameters\n", Trial);
  fprintf(stderr,"SigOff %4.2f, WtRng %4.2f, WtMul %4.2f\n",
	 SigmoidPrimeOffset, WeightRange, WeightMultiplier);
  fprintf(stderr,"OMu %4.2f, OEps %4.2f, ODcy %7.5f, OPat %d, OChange %4.2f\n",
	  OutputMu, OutputEpsilon, OutputDecay, OutputPatience,
	  OutputChangeThreshold);
  fprintf(stderr,"IMu %4.2f, IEps %4.2f, IDcy %7.5f, IPat %d, IChange %4.2f\n",
	  InputMu, InputEpsilon, InputDecay, InputPatience,
	  InputChangeThreshold);
  fprintf(stderr,"Utype: %s, Otype: %s, Pool %d\n",
	  TYPE_STRING(UnitType), TYPE_STRING(OutputType), Ncandidates);
  fprintf(stderr,"ErrMeas: %s, ErrThres: %5.3f\n",
	 TYPE_STRING(ErrorMeasure), ErrorIndexThreshold);
*/
}



/* Train the output weights until stagnation or victory is reached.  Then
 * train the input weights to stagnation or victory.  Then install the best
 * candidate unit and repeat.  OUTLIMIT and INLIMIT are upper limits on the
 * number of cycles in the output and input phases.  ROUNDS is an upper
 * limit on the number of unit-installation cycles.
 */
int  UsfCasCor::TRAIN (int  outlimit, 
                       int  inlimit, 
                       int  rounds
                      )
{
  int i,r;

/***********/

  initialize_network();
  LIST_PARAMETERS();

  if  (UseCache)
    for(i=0; i<NTrainingPatterns; i++)
    {
      Values = ValuesCache[i];
      SETUP_INPUTS(TrainingInputs[i]);
    }


  for  (r = 0;  r < rounds;  r++)
  {
    fprintf(stderr,"Round %d\n",r);
    switch(TRAIN_OUTPUTS(outlimit)){
    case WIN:
      LIST_PARAMETERS();
      fprintf(stderr,
        "Victory at %d epochs, %d units, %d hidden, Error %6.4f EI %6.4f.\n",
	     Epoch, Nunits, (Nunits - Ninputs - 1), TrueError, ErrorIndex);
      return(WIN);

    case TIMEOUT:      
      fprintf(stderr," -Output: Epoch %d Timeout\n   train:",Epoch);
      PRINT_SUMMARY(NTrainingPatterns); 
      fprintf(stderr,"\n");
      
      break;
    case STAGNANT:
      fprintf(stderr," +Output: Epoch %d Stagnant\n  train:",Epoch);
      PRINT_SUMMARY(NTrainingPatterns); 
      fprintf(stderr,"\n");
      break;

    default:
      fprintf(stderr,"Bad return from TRAIN_OUTPUTS");
      break;
    }

    /* DumpWeightsFileforROundx */
    if  (save_network_each_round) save_network(Trial,r);

    if(Test)TEST_EPOCH(0.49);	 /* how are we doing? */

    switch(TRAIN_INPUTS(inlimit)){
    case TIMEOUT:      
      fprintf(stderr," -Input : Epoch %d Timeout  (Correlation %6.4f)\n",
	     Epoch, BestCandidateScore);
      break;
    case STAGNANT:
      fprintf(stderr," +Input : Epoch %d Stagnant  (Correlation %6.4f)\n",
	     Epoch, BestCandidateScore);
      break;
    default:
      fprintf(stderr,"Bad return from TRAIN_INPUTS");
      break;
    }

    INSTALL_NEW_UNIT(); 
    fprintf(stderr," ADDED UNIT: %d\n", r+1);
  }

  LIST_PARAMETERS();

  switch(TRAIN_OUTPUTS(outlimit)){
    case WIN:
      fprintf(stderr,"Victory at %d epochs, %d units, %d hidden, Error %6.4f EI %6.4f.\n",
	     Epoch, Nunits, (Nunits - Ninputs - 1), TrueError, ErrorIndex);
      return(WIN);

    case TIMEOUT: case STAGNANT:      
      fprintf(stderr,"Defeat at %d units, ", Nunits);
      PRINT_SUMMARY(NTrainingPatterns);
      fprintf(stderr,"\n");
      return(LOSE);

    default:
      fprintf(stderr,"Bad return from TRAIN_OUTPUTS");
      break;
    }

  return -1;
}  /* TRAIN */






/* Perform forward propagation once for each set of weights in the
 * testing vectors, computing errors.  Do not change any weights.
 */
void  UsfCasCor::TEST_EPOCH (double test_threshold)
{
  int i;

  /* Globals must be saved from the last training phase. If they are not  */
  /* saved then the next unit will be training to correlate with the test */
  /* set error. */
  Boolean old_UC = UseCache;	/* temporarily turn off cache */
  float old_ST = ScoreThreshold; /* save global */
  float old_TE = TrueError;	/* save global */
  float *old_SE = SumErrors;	/* save global */
  float old_SSE = SumSqError;	/* save global */

  if  ((test_threshold > FLT_MAX)  || (test_threshold < FLT_MIN))
    cerr << endl << "UsfCasCor::TEST_EPOCH    test_threshold[" << test_threshold << "]  has exceeded capacity of a float variabnle." << endl << endl;

  ScoreThreshold = (float)test_threshold;
  UseCache = False;

  Values = ExtraValues;
  Errors = ExtraErrors;
  /* If no separate test inputs, use training inputs. */
  if(NTestPatterns == 0){
    TestInputs = TrainingInputs;    
    TestOutputs = TrainingOutputs;
    NTestPatterns = NTrainingPatterns;
  }

  /* Zero some accumulators. */
  ErrorBits = 0;
  ErrorMisclassifications=0;
  TrueError = 0.0;
  SumErrors = DummySumErrors;
  SumSqError = 0.0;

  /* Now run all test patterns and report the results. */
  for(i=0; i<NTestPatterns; i++){
    Goal = TestOutputs[i];
    FULL_FORWARD_PASS(TestInputs[i]);
    
    COMPUTE_ERRORS(Goal, False, True, -1);
  } 

  if(ErrorMeasure == INDEX)
    ErrorIndex = ERROR_INDEX(TestStdDev, NtestOutputValues);
  fprintf(stderr,"  test :");
  PRINT_SUMMARY(NTestPatterns);
  fprintf(stderr,"\n");

  /* restore globals */
  UseCache = old_UC;		
  ScoreThreshold = old_ST;
  TrueError = old_TE;	
  SumErrors = old_SE;	
  SumSqError = old_SSE;
}  /* TEST_EPOCH */





/* Perform forward propagation once for each set of weights in the
 * testing vectors, computing errors.  Do not change any weights.
 */

void  UsfCasCor::PREDICT (float test_threshold)
{
  int i;

  /* Globals must be saved from the last training phase. If they are not  */
  /* saved then the next unit will be training to correlate with the test */
  /* set error. */
  Boolean old_UC = UseCache;	/* temporarily turn off cache */
  float old_ST = ScoreThreshold; /* save global */
  float old_TE = TrueError;	/* save global */
  float *old_SE = SumErrors;	/* save global */
  float old_SSE = SumSqError;	/* save global */
  int test_examples_misclassified=0;

  /* Open the prediction file for output */
  int max_pred,max_true,j;
  FILE *predfile;
  FILE *wpredfile;
  char predfilename[LINELEN+1];

  ErrorMisclassifications=0;
  sprintf(predfilename,"%s.pred", filestem);
  predfile=FOPEN(predfilename,"w");
  sprintf(predfilename,"%s.weighted_pred",filestem);
  wpredfile=FOPEN(predfilename,"w"); 

  fputc ('#',wpredfile);
  for  (i = 0; i < number_of_classes; i++) 
  {
    fprintf(wpredfile,"%s",class_label[i]);
    if  (i < number_of_classes-1) 
      fputc (',',wpredfile);
    else                          
      fputc ('\n',wpredfile);
  }

  ScoreThreshold = test_threshold;
  UseCache = False;

  Values = ExtraValues;
  Errors = ExtraErrors;

  /* If no separate test inputs, use training inputs. */
  if  (NTestPatterns == 0 || resubstitution )
  {
    TestInputs  = TrainingInputs;    
    TestOutputs = TrainingOutputs;
    NTestPatterns = NTrainingPatterns;
  }

  /* Zero some accumulators. */
  ErrorBits = 0;
  TrueError = 0.0;
  SumErrors = DummySumErrors;
  SumSqError = 0.0;

  /* Now run all test patterns and report the results. */
  for  (i = 0; i < NTestPatterns; i++)
  {
    Goal = TestOutputs[i];
    FULL_FORWARD_PASS (TestInputs[i]);
    
    /* Find max. output (predicted class) */
    max_pred = 0;
    max_true = 0;
    for (j = 0; j < Noutputs; j++)
    {
      if (Outputs[max_pred] < Outputs[j]) 
         max_pred = j;

      if (TestOutputs[i][max_true] < TestOutputs[i][j]) 
         max_true = j;

      fprintf (wpredfile,"%f", Outputs[j]);

      if (j < Noutputs - 1) 
        fputc(',', wpredfile);
      else                 
        fputc('\n', wpredfile);
    }

    if  (max_pred != max_true) 
      test_examples_misclassified++;

    fprintf  (predfile,"%s\n", class_label[max_pred]);
    

    COMPUTE_ERRORS (Goal, False, True, -1);
  } 


  fclose  (predfile);

  if  (ErrorMeasure == INDEX)
    ErrorIndex = ERROR_INDEX(TestStdDev, NtestOutputValues);

  fprintf(stderr,"  Test set:");
  
  PRINT_SUMMARY (NTestPatterns);

  fprintf (stderr, "\nAccuracy: %5.2f%% (%d/%d)\n", 
           100.0 * (NTestPatterns - test_examples_misclassified) / (float)NTestPatterns, 
           test_examples_misclassified, 
           NTestPatterns
          );

  /* restore globals */
  UseCache = old_UC;		
  ScoreThreshold = old_ST;
  TrueError = old_TE;	
  SumErrors = old_SE;	
  SumSqError = old_SSE;

  return;
}  /* PREDICT */




/* print out the things interesting after a pass.
 */
void  UsfCasCor::OUT_PASS_OUTPUT ()
{
  int i;

  fprintf (stderr," Outputs: ");
  for  (i = 0;  i < Noutputs;  i++)
    fprintf(stderr,"%6.4f ", Outputs[i]);

  fprintf (stderr,"\n Errors: ");
  for  (i = 0;  i < Noutputs;  i++)
    fprintf(stderr,"%6.4f ", Errors[i]);

  fprintf(stderr,"\n Values: ");
  for(i=0;i<Nunits;i++)
    fprintf(stderr,"%6.4f ", Values[i]);

  fprintf(stderr,"\n\n");
}  /* OUT_PASS_OUTPUT */



/* Print the summary statistics based on the value of ErrorMeasure.
 */
void  UsfCasCor::PRINT_SUMMARY (int n)
{
  switch(ErrorMeasure){
  case BITS:
    fprintf(stderr,"errbits %d\terror %6.4f\t",
	   ErrorBits, TrueError);
    break;
  case INDEX:
    fprintf(stderr," ErrorIndex %6.4f, TrueError %6.4f, ",
	   ErrorIndex, TrueError);
    break;
  }

  fprintf(stderr,"accuracy %6.2f%% (%d/%d).",100.0-(100.0*ErrorMisclassifications)/n,ErrorMisclassifications,n);
}  /* PRINT_SUMMARY */




/* Set up the network for a learning problem.  Clean up all the data
 * structures.  Initialize the output weights to random values controlled by
 * WeightRange.
 */
void  UsfCasCor::initialize_network ()
{
  int i,j;
/**********/

  /* Set up the AllConnections vector. */
  for(i=0; i<MaxUnits; i++)
    AllConnections[i] = i;

  /* Initialize the active unit data structures. */
  for(i=0; i<MaxUnits; i++){
    ExtraValues[i] = 0.0;
    Nconnections[i] = 0;
    Connections[i] = NULL;
    Weights[i] = NULL;
  }
  /* Initialize the per-output data structures. */
  for(i=0; i<Noutputs; i++){
    Outputs[i] = 0.0;
    ExtraErrors[i] = 0.0;
    for(j=0; j<MaxUnits; j++){
      OutputWeights[i][j] = 0.0;
      OutputDeltas[i][j] = 0.0;
      OutputSlopes[i][j] = 0.0;
      OutputPrevSlopes[i][j] = 0.0;
    }
    /* Set up initial random weights for the input-to-output connections. */
    for(j=0; j<(Ninputs+1); j++)
      OutputWeights[i][j] = random_weight();
  }

  /* Initialize the caches if they are in use. */
  if(UseCache)
    for(j=0; j<MaxCases; j++){
      for(i=0; i<MaxUnits; i++)
	ValuesCache[j][i] = 0.0;
      for(i=0; i<Noutputs; i++)
	ErrorsCache[j][i] = 0.0;
    }

  /* Candidate units get initialized in a separate routine. */
  INIT_CANDIDATES();

  ExtraValues[0] = 1.0;		/* bias unit */
  Epoch = 0;
  Nunits = Ninputs + 1;  
  ErrorBits = 0;
  TrueError = 0.0;
  for(i=0; i<Noutputs; i++){
    SumErrors[i] = 0.0;
    DummySumErrors[i] = 0.0;
  }
  SumSqError = 0.0;
  BestCandidateScore = 0.0;
  BestCandidate = 0;
#ifdef CONNX
  conx = 0l;
#endif

  if(ErrorMeasure == INDEX){
    /* ErrorIndex initialization */
    ErrorIndex = 0.0;
    TrainingStdDev = STANDARD_DEV(TrainingOutputs, NTrainingPatterns, 
				  NtrainingOutputValues);
    if(NTestPatterns)
      TestStdDev = STANDARD_DEV(TestOutputs, NTestPatterns, 
				NtestOutputValues);
  }
}  /* initialize_network */




/* Calculate the standard deviation of an entire output set.
 */
float  UsfCasCor::STANDARD_DEV (float** outputs, 
                                int     npatterns, 
                                int     nvalues
                               )
{
  int i,j;
  float sum_o_sqs = 0.0;
  float sum = 0.0;
  float cur = 0.0;
  float fnum = (float)nvalues;
/**************/

  for(i=0;i<npatterns;i++)
    for(j=0;j<Noutputs;j++){
      cur = outputs[i][j];
      sum += cur;
      sum_o_sqs += cur * cur;
    }

  return (sqrt((fnum * sum_o_sqs - sum * sum)/
	       (fnum * (fnum - 1.0f))));
}  /* STANDARD_DEV */




/* ErrorIndex is the rms TrueError normalized by the standard deviation of the 
 * goal set.
 */
float  UsfCasCor::ERROR_INDEX (double std_dev, 
                               int    num
                              )
{
  return (sqrt( TrueError / (float)num) / (float)std_dev);
}






//**************************************************************
//*                        normalize.c                         *
//**************************************************************



void  UsfCasCor::rnormalize ()
{
  int test_maxval, test_minval, train_maxval, train_minval;
  float minval, maxval,range;
  int i,s;

  /* Here we have to normalize the features */
  test_maxval=test_minval=train_maxval=train_minval=0;
  for (s=0; s < Ninputs; s++) 
  {
    for (i=0; i < NTrainingPatterns; i++) 
    {
       if  (TrainingInputs[i][s] > TrainingInputs[train_maxval][s])
         train_maxval = i;

       if  (TrainingInputs[i][s] < TrainingInputs[train_minval][s])
         train_minval = i;
    } 

    /* Set the max and min to the max and min of train/test. */
    maxval = TrainingInputs[train_maxval][s];
    minval = TrainingInputs[train_minval][s];

    /* Set the attribute range and normalize */
    range = maxval - minval;
    for  (i = 0;  i < NTrainingPatterns;  i++)
    {
      TrainingInputs[i][s] = (TrainingInputs[i][s] - minval) / range;
    }

    for  (i = 0;  i < NTestPatterns;  i++)
    {
      TestInputs[i][s] = (TestInputs[i][s] - minval) / range;
    } 

   }
    return ;
}  /* rnormalize */





/* Normalize using the logistic function, as done by Jones in
   his paper. The function is

       1
   ---------
    1+e^-x
*/
void  UsfCasCor::lnormalize()
{
  int s, i;
    
  for (i = 0;  i < NTrainingPatterns;  i++)
  {
    for (s = 0;  s < Ninputs;  s++)
    {
      TrainingInputs[i][s] = 1.0f / (1.0f + exp (-(TrainingInputs[i][s])));
    }
  }

  for  (i = 0;  i < NTestPatterns;  i++) 
  {
    for  (s = 0;  s < Ninputs;  s++)
    {
      TestInputs[i][s] = 1.0f / (1.0f + exp (-(TestInputs[i][s])));
    }
  }

  return;
}  /* lnormalize */







//******************************************************************************
//                            Utility Functions.                               *
//******************************************************************************


int  GetProcessId ()
{
#ifdef  WIN32
  DWORD WINAPI  processId = GetCurrentProcessId ();
  return  processId;
#else
  pid_t processID = getpid ();
  return  processID;
#endif
}



int daysYTDforMonth[] = 
   {0,  31,  59,  90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
   //  Jan  Feb  Mar  Apr  may  jun  jul  aug  sep  oct  nov  dec



long long  GetLocalDateTime ()
{
#ifdef  WIN32
  SYSTEMTIME  sysTime;
  GetLocalTime(&sysTime);

  int  year   = (short)sysTime.wYear;
  int  month  = sysTime.wMonth;
  int  day    = sysTime.wDay;
  int  hour   = sysTime.wHour;
  int  minute = sysTime.wMinute;
  int  second = sysTime.wSecond;

  int  years = year - 1970;
  int  numLeapYears = 0;
  int  y =1972;
  while  (y < year)  
  {
    y += 4; 
    ++numLeapYears;
  }

  if  (((year % 4) == 0)  &&  (month > 2))
    ++numLeapYears;

  int  ytdDays = day;
  if  ((month > 0)  &&  (month < 13))
    ytdDays = daysYTDforMonth[month - 1];


  long long  result = years * 365 * 24 * 60 * 60  + 
                      numLeapYears * 24 * 60 * 60 +
                      ytdDays      * 24 * 60 * 60 +
                      hour         * 60 * 60      +
                      minute       * 60           +
                      second;
  return  result;
#else
  time_t   long_time;
  return  long_time;
#endif

}  /* GetCurrentDateTime */


char*  GetDsiplayableTime ()
{
#if  defined(WIN32)
   __time64_t ltime;
   _time64( &ltime );
   return  strdup (_ctime64 (&ltime));
#else
  struct tm  *curTime;
  time_t     long_time;
  time (&long_time);
  return  strdup (ctime (long_time));
#endif
}  /* GetDsiplayableTime */


char*   index (char* str, int c)
{
  return strchr (str, c);
}


#if  defined(WIN32)
int  strncasecmp (const char*  s1, 
                  const char*  s2, 
                  int          count
                 )
{
  if  (s1 == NULL)
  {
    if  (s2 == NULL)
      return 0;
    else
      return -1;
  }
  else if  (s2 == NULL)
  {
    return 1;
  }

  const char*  c1 = s1;   
  const char*  c2 = s2;
  int    c = 0;
  while  (c < count)
  {
    int  x = tolower (*c1) - tolower(*c2);
    if  (x < 0)
      return -1;

    else if  (x > 0)
      return 1;

    if  (*c1 == 0)
      break;

    ++c1;
    ++c2;
    ++c;
  }

  return 0;
}  /* srtncasecmp */
#endif




//******************************************************************************
//                                globals.c                                    *
//******************************************************************************

/*********************************************************************/
/* keyword table used for updating the simulation parameters without */
/* recompilation. They must be in alphabetical order by keyword.     */
/*********************************************************************/

void  UsfCasCor::ConstructParmTable ()
{
  //ParmTable = 
  //{
  ParmTable[ 0] = parmentry ("errorindexthreshold",   FLOAT,      (void *)&ErrorIndexThreshold);
  ParmTable[ 1] = parmentry ("errormeasure",          ENUM_NO,    (void *)&ErrorMeasure);
  ParmTable[ 2] = parmentry ("go",                    GO,         (void *)NULL); /* special keyword */
  ParmTable[ 3] = parmentry ("graphics",              BOOLE,      (void *)&Graphics);
  ParmTable[ 4] = parmentry ("inputchangethreshold",  FLOAT,      (void *)&InputChangeThreshold);
  ParmTable[ 5] = parmentry ("inputdecay",            FLOAT,      (void *)&InputDecay);
  ParmTable[ 6] = parmentry ("inputepsilon",          FLOAT,      (void *)&InputEpsilon);
  ParmTable[ 7] = parmentry ("inputmu",               FLOAT,      (void *)&InputMu);
  ParmTable[ 8] = parmentry ("inputpatience",         INT,        (void *)&InputPatience);
  ParmTable[ 9] = parmentry ("maxunits",              INT_NO,     (void *)&MaxUnits);
  ParmTable[10] = parmentry ("ncandidates",           INT_NO,     (void *)&Ncandidates);
  ParmTable[11] = parmentry ("ninputs",               INT_NO,     (void *)&Ninputs);
  ParmTable[12] = parmentry ("nonrandomseed",         BOOLE,      (void *)&NonRandomSeed);
  ParmTable[13] = parmentry ("noutputs",              INT_NO,     (void *)&Noutputs);
    //{"ntestpatterns",        INT_NO, (void *)&NTestPatterns},
    //{"ntrainingpatterns",    INT_NO, (void *)&NTrainingPatterns},
  ParmTable[14] = parmentry ("outputchangethreshold", FLOAT,      (void *)&OutputChangeThreshold);
  ParmTable[15] = parmentry ("outputdecay",           FLOAT,      (void *)&OutputDecay);
  ParmTable[16] = parmentry ("outputepsilon",         FLOAT,      (void *)&OutputEpsilon);
  ParmTable[17] = parmentry ("outputmu",              FLOAT,      (void *)&OutputMu);
  ParmTable[18] = parmentry ("outputpatience",        INT,        (void *)&OutputPatience);
  ParmTable[19] = parmentry ("outputtype",            ENUM,       (void *)&OutputType);
  ParmTable[20] = parmentry ("quit",                  BOMB,       (void *)NULL);    /* special keyword */
  ParmTable[21] = parmentry ("save",                  SAVE,       (void *)NULL);    /* special keyword */
  ParmTable[22] = parmentry ("scorethreshold",        FLOAT,      (void *)&ScoreThreshold);
  ParmTable[23] = parmentry ("sigmoidmax",            FLOAT_NO,   (void *)&SigmoidMax);
  ParmTable[24] = parmentry ("sigmoidmin",            FLOAT_NO,   (void *)&SigmoidMin);
  ParmTable[25] = parmentry ("sigmoidprimeoffset",    FLOAT,      (void *)&SigmoidPrimeOffset);
  ParmTable[26] = parmentry ("singleepoch",           BOOLE,      (void *)&SingleEpoch);
  ParmTable[27] = parmentry ("singlepass",            BOOLE,      (void *)&SinglePass);
  ParmTable[28] = parmentry ("test",                  BOOLE,      (void *)&Test);
  ParmTable[29] = parmentry ("testing",               GETTEST,    (void *)NULL);      /* special keyword */
  ParmTable[30] = parmentry ("training",              GETTRAINING,(void *)NULL);      /* special keyword */
  ParmTable[31] = parmentry ("unittype",              ENUM_NO,    (void *)&UnitType);
  ParmTable[32] = parmentry ("usecache",              BOOLE_NO,   (void *)&UseCache);
  ParmTable[33] = parmentry ("values",                VALUE,      (void *)NULL);      /* special keyword */
  ParmTable[34] = parmentry ("weightfile",            INITFILE,   (void *)NULL);      /* special keyword */
  ParmTable[35] = parmentry ("weightmultiplier",      FLOAT,      (void *)&WeightMultiplier);
  ParmTable[36] = parmentry ("weightrange",           FLOAT,      (void *)&WeightRange);
  //}
  // 4160 - 4122 + 1 - 2 = 37 entries
  Nparameters = 		/* Number of entries in ParmTable */
      sizeof(ParmTable)/sizeof(PARMS);
}  /* ConstructParmTable */


/* Initialize all globals that are not problem dependent.  Put this function 
 * in a seperate file to make changing parameters less painful.
 */
void UsfCasCor::INITIALIZE_GLOBALS ()
{
  OutputShrinkFactor=OutputMu / (1.0f + OutputMu); 
  InputShrinkFactor=InputMu / (1.0f + InputMu);
  //signal(SIGINT, TRAP_CONTROL_C); /* initialize interrupt handler */
  //InterruptPending = FALSE;
}






//******************************************************************
//*                       load_data.c                              *
//******************************************************************


void  UsfCasCor::load_data ()
{
  _load_training_data();
  if  (!resubstitution )
    _load_test_data();

  info("Read in %d training patterns, %d test patterns of dimension %d\n", 
       NTrainingPatterns, 
       NTestPatterns, 
       Ninputs
      );
  
  if  (normalize_features) 
  {
    if  (normalization_method == 0)
      rnormalize ();
    else                         
      lnormalize ();
  }

  if  (load_weights) 
    _load_example_weights();

  return;
}  /* load_data */



/*****************************************************************
  private functions
******************************************************************/
void  UsfCasCor::_load_training_data ()
{
  FILE  *datafile, *indexfile;
  char realfname[LINELEN+1];
  int rec_num,i;
  /********/

  /* open training data file */
  sprintf (realfname, "%s.data", filestem);
  datafile=FOPEN(realfname,"r");

  /* If index files are being used, we read the index number from the
     .idata file and seek to the right place  in the data file */
  if ( use_index_files ) {
    sprintf (realfname, "%s.idata", filestem);
    indexfile=FOPEN(realfname,"r");
    
    i=0;
    while ( fscanf(indexfile, "%d", &rec_num) > 0 ) {
      fseek(datafile, rec_num*line_length,0);
      _load_training_example(datafile, i++);
    }
    fclose(indexfile);
  } else {
    
    /* Else read them in one line at a time. */
    for (i=0; i < NTrainingPatterns; i++) 
      _load_training_example(datafile,i);
    
  } 
  fclose(datafile);
}  /* _load_training_data */



/* Read the next NTrainingPattern number of lines into the TrainingInput and
 * TrainingOutput data structures.
 */
void  UsfCasCor::_load_training_example (FILE *infile, int i)
{
  int j, k;
  char classlabel[LINELEN+1];
  /**************/

  /* Get features. If a feature_type is set to ignore, skip over it */
  for  (j = 0, k = 0;  j < Ninputs;  k++ )
  {
    /* Some features are to be ignored, do so here. */
    if  (feature_type[k] == IGNORE )
    {
      while (fgetc(infile) != ',');
    } 

    else if  (feature_type[k] == BINARY )
    {
      /* For binary features, we have a hex string that represents the values, so we have to read all of them in */
      int bit_num=0;
      int bn;
      unsigned int cb;
      unsigned int bitmask[8] = {128,64,32,16,8,4,2,1};
      fscanf (infile, "0x");
      while  (j < Ninputs  &&  feature_type[k] == BINARY ) 
      {
        bn = bit_num % 8;
        if  (bn == 0)
        {
          cb = 0;
	        fscanf(infile,"%02x", &cb);
        }
        TrainingInputs[i][j] = ((cb & bitmask[bn]) > 0);
        bit_num++;
        j++;
        k++;
      }
      while (fgetc(infile) != ',');
    } 
    else
    {
      fscanf (infile, "%f,", &TrainingInputs[i][j]);
      j++;
    }
  }
  
  fscanf (infile, "%s", classlabel);
  while  (fgetc(infile) != '\n');
  k = class_index (classlabel);

  for  (j = 0;  j < Noutputs;  j++)
  {
    if  (j == k) 
      TrainingOutputs[i][j] = SigmoidMax;
    else
      TrainingOutputs[i][j] = SigmoidMin; 
  }
}  /* _load_training_example */




/* Get the test data from a seperate file.  Open the file
 */
void  UsfCasCor::_load_test_data ()
{
  FILE  *indexfile, *datafile;
  char realfname[LINELEN+1];
  int rec_num;
  int i;
  /********/

  if  (use_index_files)
  {
    /* open test data file */
    sprintf (realfname, "%s.data", filestem);
    datafile = FOPEN (realfname, "r");

    /* open test index file */
    sprintf (realfname, "%s.itest", filestem);
    indexfile = FOPEN (realfname, "r");

    i = 0;
    while  (fscanf (indexfile, "%d", &rec_num) > 0 )
    {
      fseek (datafile, rec_num*line_length,0);
      _load_test_example (datafile, i++);
    }

    fclose (indexfile);
    fclose (datafile);
  } 
  else 
  {
    /* open test data file */
    sprintf (realfname, "%s.test", filestem);
    datafile = FOPEN (realfname, "r");
    for (i = 0;  i < NTestPatterns;  i++) 
       _load_test_example (datafile, i);
    fclose (datafile);
  }

  return;
}  /* _load_test_data */



/* Read the next NTestPattern number of lines into the TestInput and
 * TestOutput data structures.
 */
void  UsfCasCor::_load_test_example (FILE* infile, 
                                     int   i
                                    )
{
  int j,k;
  char classlabel[LINELEN+1];
  /**************/


  /* Get features. If a feature_type is set to ignore, skip over it */
  for  (j = 0, k = 0;  j < Ninputs;  k++ )
  {
    /* Some features are to be ignored, do so here. */
    if  (feature_type[k] == IGNORE )
    {
      while (fgetc(infile) != ',');
    }
    else if ( feature_type[k] == BINARY )
    {
      /* For binary features, we have a hex string that represents the values, so we have to read all of them in */
      int bit_num=0;
      int bn;
      unsigned int cb;
      unsigned int bitmask[8]={128,64,32,16,8,4,2,1};
      fscanf (infile, "0x");
      while  (j < Ninputs && feature_type[k] == BINARY)
      {
        bn = bit_num % 8;
        if ( bn == 0 )
        {
          cb = 0;
          fscanf (infile,"%02x", &cb);
        }

        TestInputs[i][j] = ((cb & bitmask[bn]) > 0);
        bit_num++;
        j++;
        k++;
      }
      while (fgetc(infile) != ',');
    }
    else 
    {
      fscanf (infile, "%f,", &TestInputs[i][j]);
      j++;
    }
  }

  fscanf (infile, "%s", classlabel);
  while  (fgetc (infile) != '\n');
  k = class_index (classlabel);
  for  (j = 0;  j < Noutputs;  j++)
  {
    if  (j == k) 
      TestOutputs[i][j] = SigmoidMax;
    else
      TestOutputs[i][j] = SigmoidMin;
  }

}  /* _load_test_example */



/* Open filestem.weights and read in all of the floating point weights. */
void  UsfCasCor::_load_example_weights ()
{
  int i;
  char filenamebuf[1024];
  FILE *fp;

  example_weight = (float *)malloc (NTrainingPatterns * sizeof (float));
  sprintf(filenamebuf, "%s.weights", filestem);
  if  ((fp=fopen (filenamebuf, "r")) == NULL )
  {
    fprintf (stderr,"Cannot open weight file\n");
    exit (1);
  }
  for  (i = 0;  i < NTrainingPatterns;  i++) 
    fscanf (fp, "%f", &(example_weight[i]));
  fclose(fp);

  return;
}  /* _load_example_weights */
