#include <stdio.h>
#include "usfcascor.h"



int number_of_classes=-1;
char *filestem=NULL;
char **class_label=NULL;
int line_length=-1;
int  *feature_type=NULL;

long the_random_seed=0;

Boolean use_index_files=False;
Boolean do_predictions=True;
Boolean normalize_features=False;
Boolean resubstitution=False;
Boolean do_training=True;
Boolean save_network_each_trial=False;
Boolean save_network_each_round=False;
Boolean load_weights=False;

/* Allocate global storage */
/***********************************************************************/
/*  Assorted Parameters.                                               */
/*  These parameters and switches control the quickprop learning       */
/*  algorithm used to train the output weights.                        */
/***********************************************************************/

/* hidden unit type can be SIGMOID or GAUSIAN*/
int UnitType=SIGMOID;		   

/* output unit type can be SIGMOID or LINEAR */
int OutputType=SIGMOID;	           

/* Maximum output vaule for sigmoid units. Used */
/* to alter sigmoid range without having to edit */ 
/* training values.  Use the symbols "min" and  */
/* "max" in the input file.  The input routines */ 
/* will translate to the appropriate float values.*/
float SigmoidMax=0.5;

/* Minimum output vaule for sigmoid units.  */
float SigmoidMin=-0.5;  

/* Random-init weights in range [-WR,+WR] */
float WeightRange=1.0;

/* Add to sigmoid-prime to kill flat spots */
float SigmoidPrimeOffset=0.1;  

/* Scale Candidate correlation to get init weight */
float WeightMultiplier=1.0;	   

/* Mu used to quickprop train output weights. */
float OutputMu=2.0;		   

/* Used in computing whether the proposed step is */
/* too large.  Related to OutputMu.               */
float OutputShrinkFactor;

/* Controls the amount of linear gradient descent */
/* to use in updating output weights.             */
float OutputEpsilon=0.35;

/* This factor times the current weight is added  */
/* to the slope at the start of each output epoch */
/* Keeps weights from growing too big.            */
float OutputDecay=0.0001;	  

/* If we go for this many epochs with no real     */
/* change, it's time to stop tuning.  If 0, go on */
/* forever.                                       */
int OutputPatience=8;

/* The error must change by at least this */
/* fraction of its old value to count as a  */
/* significant change.                      */
float OutputChangeThreshold=0.01; 

/* Mu used to quickprop train input weights.  */
float InputMu=2.0;		  

/* Used in computing whether the proposed step is */
/* too large.  Related to InputMu. */
float InputShrinkFactor;
 
/* Controls the amount of linear gradient descent */
/* to use in updating Input weights. */
float InputEpsilon=1.0;	  

/* This factor times the current weight is added  */
/* to the slope at the start of each Input epoch */
/* Keeps weights from growing too big. */
float InputDecay=0.0;	  

/* If we go for this many epochs with no real */
/* change, it's time to stop tuning.  If 0, go on */
/* forever. */
int InputPatience=8;

/* The error must change by at least this */
/* fraction of its old value to count as a  */
/* significant change. */
float InputChangeThreshold=0.03;


/***********************************************************************/
/*  Variables related to error and correlation.                        */
/***********************************************************************/

/* Total output error for one epoch */
float TrueError=0.0;

/* This close to desired value => bit is correct */
float ScoreThreshold=0.35;      


int   ErrorBits=0;           /* Total # bits in epoch that were wrong */
float *SumErrors;	   /* Accumulate the sum of the error values used in */
                           /* the correlation phase. Sum is stored seperately */
			   /* for each output.  Values are converted to */
			   /* average errors before use in ADJUST_CORRELATION */
float *DummySumErrors;	   /* Replace SumErrors with this for test epochs. */
float SumSqError;	   /* Accumulate the sum of the square of the error  */
                           /* values used in the correlation phase. */
float BestCandidateScore=0.0;  /* Best correlation score of all candidate units. */
int   BestCandidate=0;	   /* Index of the candidate unit with best score. */

/***********************************************************************/
/* These variables and switches control the simulation and display.    */
/***********************************************************************/
Boolean UseCache=False;	   /* If TRUE, cache the forward-pass values instead */
                           /* of repeatedly computing them. */
int     Epoch=0;             /* Current epoch number */
Boolean Graphics=False;	   /* If TRUE, print progress after each epoch. */
Boolean NonRandomSeed=False;	   /* TRUE => use 1 as the seed for the random */
 			   /* number generator.  Useful when comparing */
			   /* different parameter settings.  FALSE => use */
			   /* system clock to start random sequence. */
Boolean Test=True;		   /* If TRUE, run a test epoch and print the result */
                           /* after each round of output tuning. */
Boolean SinglePass=False;        /* TRUE => Pause after forward/backward cycle */
Boolean SingleEpoch;       /* TRUE => Pause after each training epoch */
Boolean Step=False;              /* Turned to TRUE after each pause, briefly */
int Trial=0;		   /* Current trial number, used in log outputs */

/***********************************************************************/
/* The sets of training inputs and outputs.                            */
/***********************************************************************/
int   NTrainingPatterns=0;    /* !! Not in Lisp version.  Needed here. */
int   NTestPatterns=0;        /* !! Not in Lisp version.  Needed here. */
float **TrainingInputs;
float **TrainingOutputs;
float *Goal;                /* Goal vector for the current training or */
                            /* testing case.                           */
char *T_T_files;            /* Pointer to Training or Test filenames  */
                            /* in input line updated by PROCESS_LINE, */ 
                            /* each time the user needs a file for input */ 
                            /* of training or test data. */

float *example_weight;
/***************************************************************************/
/*  For some benchmarks there is a separate set of values used for testing */
/*  the network's ability to generalize.  These values are not used during */
/*  training.                                                              */
/***************************************************************************/
float **TestInputs;
float **TestOutputs;
/***************************************************************************/
/*                                                                         */
/* Fundamental data structures.                                            */
/*                                                                         */
/* Unit outputs and weights are floats.                                    */
/*                                                                         */
/* Instead of representing each unit by a structure, we represent the      */
/* unit by a int.  This is used to index into various arrays that hold     */
/* per-unit information, such as the activation value of each unit.        */
/*                                                                         */
/* Per-connection information for each connection COMING INTO unit is      */
/* stored in a array of arrays.  The outer array is indexed by the unit    */
/* number, and the inner array is then indexed by connection number.       */
/*                                                                         */
/* Unit 0 is always at a maximum-on value.  Connections from this unit     */
/* supply a bias.  Next come some input units, then some hidden units.     */
/*                                                                         */
/* Output units have their own separate set of data structures, as do      */
/* candidate units whose inputs are currently being trained.               */
/***************************************************************************/
int   MaxUnits=2000;		   /* Maximum number of input values and hidden  */
                           /* in the network. */
int   Nunits;              /* Total number of active units in net */
int   Ninputs=0;             /* Number of input units */
int   Noutputs=0;            /* Number of output units */
int   Ncandidates=8;	   /* Number of candidate units trained at once. */
int   MaxCases;		   /* Maximum number of training cases that can be */
                           /* accommdated by the current data structures.  */
int   Ncases;		   /* Number of training cases currently in use. */
                           /* Assume a contiguous block beginning with   */
int   FirstCase;	   /* Address of the first training case in the     */
                           /* currently active set.  Usually zero, but may  */
                           /* differ if we are training on different chunks */
                           /* of the training set at different times.       */

/***************************************************************************/
/* The following vectors hold values related to hidden units in the active */
/* net and their input weights.                                            */
/***************************************************************************/
float *Values;   	   /* Current activation value for each unit */
float **ValuesCache=NULL;  /* Holds a distinct Values array for each of the */
                           /* MaxCases training cases.                      */
float *ExtraValues;	   /* Extra Values vector to use when no cache. */
int   *Nconnections;       /* # of INCOMING connections per unit */
int   **Connections;       /* C[i][j] lists jth unit projecting to unit i */
float **Weights;           /* W[i][j] holds weight of C[i][j] */

/***************************************************************************/
/* The following arrays of arrays hold values for the outputs of the active*/
/*  network and the output-side weights.                                   */
/***************************************************************************/
float *Outputs;            /* Network output values */
float *Errors;             /* Final error value for each unit */
float **ErrorsCache=NULL;  /* Holds a distinct Errors array for each of the */
                           /* MaxCases training cases.                      */
float *ExtraErrors;	   /* Extra Errors vector to use when no cache. */
float **OutputWeights;	   /* OW[i][j] holds the weight from hidden unit i */
                           /* to output unit j */
float **OutputDeltas;      /* Change between previous OW and current one */
float **OutputSlopes;      /* Partial derivative of TotalError wrt OW[i][j] */
float **OutputPrevSlopes;  /* Previous value of OutputSlopes[i][j] */

/***************************************************************************/
/* The following arrays have one entry for each candidate unit in the      */
/* pool of trainees.                                                       */
/***************************************************************************/
float *CandValues;         /* Current output value of each candidate unit.   */
float *CandSumValues;      /* Output value of each candidate unit, summed    */
                	   /* over an entire training set.                   */
float **CandCor;           /* Correlation between unit & residual error at   */
                           /* each output, computed over a whole epoch.      */
float **CandPrevCor;       /* Holds the CandCor values from last epoch.      */
float **CandWeights;       /* Current input weights for each candidate unit. */
float **CandDeltas;        /* Input weights deltas for each candidate unit.  */
float **CandSlopes;        /* Input weights slopes for each candidate unit.  */
float **CandPrevSlopes;    /* Holds the previous values of CandSlopes.       */

/***************************************************************************/
/* This saves memory if each candidate unit receives a connection from       */
/* each existing unit and input.  That's always true at present, but may     */
/* not be in future.                                                         */
/***************************************************************************/
int *AllConnections;       /* A standard connection that connects a unit to  */
                           /* all previous units, in order, but not to the   */
                           /* bias unit.*/

/***************************************************************************/
/* ErrorIndex specific globals.  Not in release Lisp version               */
/***************************************************************************/
int NtrainingOutputValues=0;	/* Number of outputs in the training set.  */
int NtestOutputValues=0;		/* Number of outputs in the test set.      */
float TrainingStdDev=1.0;		/* Std Dev of entire training set.  Used to*/
                                /* normalize the ErrorIndex. */
float TestStdDev=1.0;
float ErrorIndex=0.0;		/* Normalized error function for continuos  */
                                /* output training sets. */
float ErrorIndexThreshold=0.2;	/* Stop training when ErrorIndex is < EIT. */
int ErrorMeasure=BITS;		/* Set to BITS for using ErrorBits to stop */
                                /* of INDEX to use ErrorIndex to stop.  */

/* ParmTable is at the end of the file */


/***************************************************************************/
/* Save and plot file related varibles                                     */
/***************************************************************************/
Boolean DumpWeights=False;		/* Are we dumping weights into a file. */
char DumpFileRoot[LINELEN+1];	/* Root of the names for the files */
FILE *WeightFile;		/* Contains weights from the current net. */

Boolean InterruptPending;	/* TRUE => user has pressed Control-C */


/******************** end of global storage allocation  **********************/
#ifdef CONNX
long conx;
#endif







/*********************************************************************/
/* keyword table used for updating the simulation parameters without */
/* recompilation. They must be in alphabetical order by keyword.     */
/*********************************************************************/
PARMS ParmTable[] = {
  {"errorindexthreshold",   FLOAT, (void *)&ErrorIndexThreshold},
  {"errormeasure",        ENUM_NO, (void *)&ErrorMeasure},
  {"go",                    GO,    (void *)NULL}, /* special keyword */
  {"graphics",             BOOLE,  (void *)&Graphics},
  {"inputchangethreshold",  FLOAT, (void *)&InputChangeThreshold},
  {"inputdecay",            FLOAT, (void *)&InputDecay},
  {"inputepsilon",          FLOAT, (void *)&InputEpsilon},
  {"inputmu",               FLOAT, (void *)&InputMu},
  {"inputpatience",         INT,   (void *)&InputPatience},
  {"maxunits",             INT_NO, (void *)&MaxUnits},
  {"ncandidates",          INT_NO, (void *)&Ncandidates},
  {"ninputs",              INT_NO, (void *)&Ninputs},
  {"nonrandomseed",         BOOLE, (void *)&NonRandomSeed},
  {"noutputs",             INT_NO, (void *)&Noutputs},
  //{"ntestpatterns",        INT_NO, (void *)&NTestPatterns},
  //{"ntrainingpatterns",    INT_NO, (void *)&NTrainingPatterns},
  {"outputchangethreshold", FLOAT, (void *)&OutputChangeThreshold},
  {"outputdecay",           FLOAT, (void *)&OutputDecay},
  {"outputepsilon",         FLOAT, (void *)&OutputEpsilon},
  {"outputmu",              FLOAT, (void *)&OutputMu},
  {"outputpatience",        INT,   (void *)&OutputPatience},
  {"outputtype",            ENUM,  (void *)&OutputType},
  {"quit",                  BOMB,  (void *)NULL}, /* special keyword */
  {"save",                  SAVE, (void *)NULL}, /* special keyword */
  {"scorethreshold",        FLOAT, (void *)&ScoreThreshold},
  {"sigmoidmax",         FLOAT_NO, (void *)&SigmoidMax},
  {"sigmoidmin",         FLOAT_NO, (void *)&SigmoidMin},
  {"sigmoidprimeoffset",    FLOAT, (void *)&SigmoidPrimeOffset},
  {"singleepoch",          BOOLE,  (void *)&SingleEpoch},
  {"singlepass",           BOOLE,  (void *)&SinglePass},
  {"test",                 BOOLE,  (void *)&Test},
  {"testing",            GETTEST,  (void *)NULL}, /* special keyword */
  {"training",       GETTRAINING,  (void *)NULL}, /* special keyword */
  {"unittype",            ENUM_NO, (void *)&UnitType},
  {"usecache",           BOOLE_NO, (void *)&UseCache},
  {"values",                VALUE, (void *)NULL},      /* special keyword */
  {"weightfile",          INITFILE,  (void *)NULL}, /* special keyword */
  {"weightmultiplier",      FLOAT, (void *)&WeightMultiplier},
  {"weightrange",           FLOAT, (void *)&WeightRange}
};
int Nparameters = 		/* Number of entries in ParmTable */
  sizeof(ParmTable)/sizeof(PARMS);


/* Initialize all globals that are not problem dependent.  Put this function 
 * in a seperate file to make changing parameters less painful.
 */
void INITIALIZE_GLOBALS(void)
{

  OutputShrinkFactor=OutputMu/(1.0 + OutputMu); 
  InputShrinkFactor=InputMu /(1.0 + InputMu);




  //signal(SIGINT, TRAP_CONTROL_C); /* initialize interrupt handler */
  //InterruptPending = FALSE;
}




