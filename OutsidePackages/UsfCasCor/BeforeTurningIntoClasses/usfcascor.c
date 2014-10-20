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
/*			  Internet:  rsc@cs.cmu.edu                         */
/*                                                                          */
/*                                                                          */
/*  This code has been placed in the public domain by the author.  As a     */
/*  matter of simple courtesy, anyone using or adapting this code is        */
/*  expected to acknowledge the source.  The author would like to hear      */
/*  about any attempts to use this system, successful or not.               */
/*                                                                          */
/*  This code is a port to C from the original Common Lisp implementation   */
/*  written by Scott E. Fahlman.  (Version dated June 1 1990.)		    */
/*									    */
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
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#ifndef _GNU_SOURCE
   #include "getopt.h"
#else
   #include <getopt.h>
#endif
#ifdef USE_MPI
   #include <mpi.h>
#endif
#include "utils.h"
#include "usfcascor.h"


/* Administrative variables */
char *version="2.0";
char *release_date="06-06-2002";
char *progname;

char Usage[]="Usage
   %s -f filestem
      -h | --help             help
      -f | --filestem=s       filestem
      -i n                    max number of input epochs
      -o n                    max number of output epochs
      -r n                    max number of new units
      -t n                    number of trials
      -P                      generate predictions from the test set
      -O option=value         Set an option, as in net file (UseCache=TRUE) 
      -N[l|s]	     	      set normalization (default=s)
      -I 		      use index files
      -R                      use resubstitution (test the training set)
      -w file                 use file for network weights/config
      -s                      save network state every round and trial.
      -S #                    use # as the random seed
      -T                      do not train
      
      Flags
      *****
      --use-example-weights   Load example weights from filestem.weights
        --no-use-example-weights
      --normalize=[logistic]  Use normalization
      --no-normalize          
      --mpi=n    
";

extern char *optarg;
extern int optind;
char *options="hf:i:o:r:t:PO:N::IRS:w:sT";
enum {no_option,option_mpi};

struct option longopts[] = {
  {"help",                no_argument      , 0, 'h'},
  {"version",             no_argument      , 0, 'v'},
  {"filestem",            required_argument, 0, 'f'},
  {"normalize",           optional_argument,0,'N'},
  {"no-normalize",        no_argument, (int *)&normalize_features,False},
  {"use-example-weights",no_argument, (int *)&load_weights, True},
  {"no-use-example-weights",no_argument, (int *)&load_weights, False},
  {"mpi", required_argument,0,option_mpi},

  /* Last element must be all zeroes */
  {0,0,0,0}
};


int in_limit=500;
int out_limit=500;
int number_of_trials=1;
int number_of_rounds=-1;
int normalization_method=0;

int my_mpi_rank=0;

void setup_network();
void train_network();
void allocate_network();


int main (int argc, char *argv[])
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

  progname=argv[0];
  info("Cascade Correlation: v%s\n", version);
  INITIALIZE_GLOBALS();

  /* process arguments one line at a time */
  while  ((c = getopt_long (argc, argv, options, longopts, &option_index)) != EOF) 
  {
    switch (c) {
    case 0:
      printf("  Set option: %s.\n", longopts[option_index].name);
      break;

    case 'h':
      fprintf(stderr,Usage,progname);
      exit(1);

    case 'v':
      fprintf (stdout,"%s\n",version);
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
      if ( *optarg == 'l' ) 
        normalization_method=1;
      else                  
        normalization_method=0;

      printf (" Using normalization method %c\n",*optarg);
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
  

  #ifdef USE_MPI
     MPI_Finalize();
  #endif

  return(0);
}





/*
 *  Get and initialize a network. 
 */
void setup_network()
{
  /* 
     There are some required variables, like NInputs,etc
     that can be taken from the training/testing files. 
  */
  info ("Dataset: %s\n",filestem);
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

  the_random_seed += getpid () + my_mpi_rank;

  srand (the_random_seed);
  info (" Starting at %s seed: %ld\n",(NonRandomSeed)?"fixed":"random",
        the_random_seed);

  /* Initialize the network variables */
  initialize_network ();

  return;
}  /* setup_network */




void train_network ()
{
  int nhidden;			/* number of hidden units used in run  */
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
  info("\nStarting run for %s, Ilim %d, Olim %d, MaxUnits %d, Trials %d.\n",
	 filestem, in_limit, out_limit, number_of_rounds, number_of_trials);

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
    nhidden = Nunits - Ninputs - 1;	/* don't count inputs or bias unit */
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
}



/* Create the network data structures, given the number of input and output
 * units.  Get the MaxUnits value from a variable.
 */
void allocate_network(void)
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
  {	     /* no test patterns so just point at training set */
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
}

  





/********************************************************************/
/*   interrupt handling routines                                    */
/*   Thanks to Dimitris Michailidis for this code.                  */
/********************************************************************/

/* allow user to change parameters if they have hit Control-C 
 */
void CHECK_INTERRUPT(void)
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








