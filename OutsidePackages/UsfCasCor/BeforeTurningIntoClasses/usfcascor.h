#ifndef __USFCASCOR__H_

#include "utils.h"



#ifndef INT_MAX
#define INT_MAX 32767
#endif

#define LINELEN   1024


/* switches used in the interface routines */
#define INT         0
#define FLOAT       1
#define ENUM        2      /* interger values that use #defines */
#define BOOLE       3
#define GETTRAINING 4
#define GETTEST     5
#define GO          6
#define INT_NO      7      /* parameters only good in netfile */
#define FLOAT_NO    8      /* most are used in memory allocation  */
#define ENUM_NO     9      /* and cannot be changed mid-simulation */
#define BOOLE_NO    10
#define VALUE       11
#define GETTRAININGFILE 12
#define GETTESTFILE     13
#define SAVE        14
#define INITFILE    15



#define NEXTLINE    0
#define FAILURE    -1

/* switch constants */
#define SIGMOID        0
#define GAUSSIAN       1
#define LINEAR         2
#define ASYMSIGMOID    3
#define VARSIGMOID     4

#define WIN            20
#define STAGNANT       21
#define TIMEOUT        22
#define LOSE           23

#define BITS           30
#define INDEX          31

#define FATAL           0
#define WARN            1

#define BOMB -99
/* Feature types. There are only three types supported (this is
   a neural network). The logic is ignore,binary otherwise real. */
#define REAL   1
#define IGNORE 2
#define BINARY 3


/*****************************************************************************/
/* Parameter table contains all user settable parameters.  It is used by the */
/* input routines to change the values of the named parameters.  The list of */
/* parameters must be sorted alphabetically so that the search routine will  */
/* work.  Parameter names are used as keywords in the search.  Keywords are  */
/* lower case to speed the comparison process.                               */
/*****************************************************************************/
struct parmentry {char *keyword;    /* variable name in lower case */
          int vartype;      /* can be INT, FLOAT, or ENUM */
          void *varptr;     /* cast to correct type before use */
        };

typedef struct parmentry PARMS;



/* Global variables */
extern int Nparameters;
extern PARMS ParmTable[];
extern char *version;
extern char *release_date;

/* Administrative variables about the dataset */
extern int line_length;
extern char *filestem;
extern char **class_label;
extern int *feature_type;

/* Flags */
extern Boolean use_index_files;
extern Boolean do_predictions;
extern Boolean normalize_features;
extern Boolean resubstitution;
extern Boolean UseCache;
extern Boolean save_network_each_round;
extern Boolean save_network_each_trial;
extern long the_random_seed;
extern Boolean do_training;
extern Boolean load_weights;

extern int NTrainingPatterns;
extern int NTestPatterns;
extern int Ninputs;
extern int Noutputs;
extern int number_of_classes;

extern float SigmoidMin;
extern float SigmoidMax;

extern float **TrainingInputs;
extern float **TrainingOutputs;
extern float **TestInputs;
extern float **TestOutputs;

extern int MaxUnits;
extern int Nunits;

extern float *example_weight;

extern float *Values;             /* Current activation value for each unit          */
extern float **ValuesCache;       /* Holds a distinct Values array for each of the   */
                                  /* MaxCases training cases.                        */
extern float *ExtraValues;        /* Extra Values vector to use when no cache.       */
extern int   *Nconnections;       /* # of INCOMING connections per unit              */
extern int   **Connections;       /* C[i][j] lists jth unit projecting to unit i     */
extern float **Weights;           /* W[i][j] holds weight of C[i][j]                 */
                             
extern float *Outputs;            /* Network output values                           */
extern float *Errors;             /* Final error value for each unit                 */
extern float **ErrorsCache;       /* Holds a distinct Errors array for each of the   */
                                  /* MaxCases training cases.                        */
extern float *ExtraErrors;        /* Extra Errors vector to use when no cache.       */
extern float **OutputWeights;     /* OW[i][j] holds the weight from hidden unit i    */
                                  /* to output unit j                                */
extern float **OutputDeltas;      /* Change between previous OW and current one      */
extern float **OutputSlopes;      /* Partial derivative of TotalError wrt OW[i][j]   */
extern float **OutputPrevSlopes;  /* Previous value of OutputSlopes[i][j]            */
extern int *AllConnections;       /* A standard connection that connects a unit to   */
                                  /* all previous units, in order, but not to the    */
                                  /* bias unit                                       */


extern int UnitType;           /* hidden unit type can be SIGMOID or GAUSIAN*/
extern int OutputType;               /* output unit type can be SIGMOID or LINEAR */

extern float SigmoidPrimeOffset;  /* Add to sigmoid-prime to kill flat spots */
extern float TrueError;           /* Total output error for one epoch */
extern float ScoreThreshold;      /* This close to desired value => bit is correct */
extern int   ErrorBits;           /* Total # bits in epoch that were wrong */
extern float *SumErrors;          /* Accumulate the sum of the error values used in */
                                  /* the correlation phase. Sum is stored seperately */
                                  /* for each output.  Values are converted to */
                                  /* average errors before use in ADJUST_CORRELATION */
extern float *DummySumErrors;     /* Replace SumErrors with this for test epochs. */
extern float SumSqError;          /* Accumulate the sum of the square of the error  */
                                  /* values used in the correlation phase. */
extern float BestCandidateScore;  /* Best correlation score of all candidate units. */
extern int   BestCandidate;       /* Index of the candidate unit with best score. */


extern float OutputMu;                 /* Mu used to quickprop train output weights.      */
extern float OutputShrinkFactor;       /* Used in computing whether the proposed step is  */
                                       /* too large.  Related to OutputMu.                */
extern float OutputEpsilon;            /* Controls the amount of linear gradient descent  */
                                       /* to use in updating output weights.              */
extern float OutputDecay;              /* This factor times the current weight is added   */
                                       /* to the slope at the start of each output epoch  */
                                       /* Keeps weights from growing too big.             */
extern int OutputPatience;             /* If we go for this many epochs with no real      */
                                       /* change, it's time to stop tuning.  If 0, go on  */
                                       /* forever.                                        */
extern float OutputChangeThreshold;    /* The error must change by at least this          */
                                       /* fraction of its old value to count as a         */
                                       /* significant change.                             */
extern float InputMu;                  /* Mu used to quickprop train input weights.       */
extern float InputShrinkFactor;        /* Used in computing whether the proposed step is  */
                                       /* too large.  Related to InputMu.                 */
extern float InputEpsilon;             /* Controls the amount of linear gradient descent  */
                                       /* to use in updating Input weights.               */
extern float InputDecay;               /* This factor times the current weight is added   */
                                       /* to the slope at the start of each Input epoch   */
                                       /* Keeps weights from growing too big.             */
extern int InputPatience;              /* If we go for this many epochs with no real      */
                                       /* change, it's time to stop tuning.  If 0, go on  */
                                       /* forever.                                        */
extern float InputChangeThreshold;     /* The error must change by at least this          */
                                       /* fraction of its old value to count as a         */
                                       /* significant change.                             */
extern int   MaxCases;                 /* Maximum number of training cases that can be    */
                                       /* accommdated by the current data structures.     */
extern int   Ncases;                   /* Number of training cases currently in use.      */
                                       /* Assume a contiguous block beginning with        */
extern int   FirstCase;                /* Address of the first training case in the       */
                                       /* currently active set.  Usually zero, but may    */
                                       /* differ if we are training on different chunks   */
                                       /* of the training set at different times.         */
extern float *Goal;                    /* Goal vector for the current training or testing */ 
                                       /* case.                                           */

extern int     Ncandidates;            /* Number of candidate units trained at once.      */
extern float*  CandValues;             /* Current output value of each candidate unit.    */
extern float*  CandSumValues;          /* Output value of each candidate unit, summed     */
                                       /* over an entire training set.                    */
extern float** CandCor;                /* Correlation between unit & residual error at    */
                                       /* each output, computed over a whole epoch.       */
extern float** CandPrevCor;            /* Holds the CandCor values from last epoch.       */
extern float** CandWeights;            /* Current input weights for each candidate unit.  */
extern float** CandDeltas;             /* Input weights deltas for each candidate unit.   */
extern float** CandSlopes;             /* Input weights slopes for each candidate unit.   */
extern float** CandPrevSlopes;         /* Holds the previous values of CandSlopes.        */
extern int     NtrainingOutputValues;  /* Number of outputs in the training set.          */
extern int     NtestOutputValues;      /* Number of outputs in the test set.              */
extern float   TrainingStdDev;         /* Std Dev of entire training set.  Used to        */
                                       /* normalize the ErrorIndex.                       */
extern float   TestStdDev;
extern float   ErrorIndex;             /* Normalized error function for continuos         */
                                       /* output training sets.                           */
extern float   ErrorIndexThreshold;    /* Stop training when ErrorIndex is < EIT.         */
extern int     ErrorMeasure;           /* Set to BITS for using ErrorBits to stop         */
                                       /* of INDEX to use ErrorIndex to stop.             */
                                       /* of repeatedly computing them.                   */
extern int     Epoch;                  /* Current epoch number                            */
extern Boolean Graphics;               /* If TRUE, print progress after each epoch.       */
extern Boolean NonRandomSeed;          /* TRUE => use 1 as the seed for the random        */
                                       /* number generator.  Useful when comparing        */
                                       /* different parameter settings.  FALSE => use     */
                                       /* system clock to start random sequence.          */
extern Boolean Test;                   /* If TRUE, run a test epoch and print the result  */
                                       /* after each round of output tuning.              */
extern Boolean SinglePass;             /* TRUE => Pause after forward/backward cycle      */
extern Boolean SingleEpoch;            /* TRUE => Pause after each training epoch         */
extern Boolean Step;                   /* Turned to TRUE after each pause, briefly        */
extern int     Trial;                  /* Current trial number, used in log outputs       */


extern float WeightMultiplier;        /* Scale Candidate correlation to get init weight */

/* Functions */
char *parm_to_string();
char *boolean_to_string();
char *type_to_string();
int process_line();

void save_network();
void rnormalize();
void lnormalize();
/* Utilities */
float random_weight();
char *trim();
int   class_index();

void load_network();
void load_namesfile();
void load_data();
void initialize_network();

void COMPUTE_UNIT_VALUE();
int TRAIN();
void TEST_EPOCH();
void LIST_PARAMETERS();
void PREDICT(float);
void INITIALIZE_GLOBALS();
#endif
