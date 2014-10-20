#include <math.h>
#include "usfcascor.h"

void rnormalize()
{
  int test_maxval, test_minval, train_maxval, train_minval;
  float minval, maxval,range;
  int i,s;

  /* Here we have to normalize the features */
  test_maxval=test_minval=train_maxval=train_minval=0;
  for (s=0; s < Ninputs; s++) {
    for (i=0; i < NTrainingPatterns; i++) {
       if ( TrainingInputs[i][s] > TrainingInputs[train_maxval][s] ) 
		train_maxval=i;
       if ( TrainingInputs[i][s] < TrainingInputs[train_minval][s] ) 
		train_minval=i;
    } 

    /* Set the max and min to the max and min of train/test. */
    maxval=TrainingInputs[train_maxval][s];
    minval=TrainingInputs[train_minval][s];

    /* Set the attribute range and normalize */
    range=maxval-minval;
    for (i=0; i < NTrainingPatterns; i++) {
       TrainingInputs[i][s]=(TrainingInputs[i][s]-minval)/range;
    }
    for (i=0; i < NTestPatterns; i++) {
       TestInputs[i][s]=(TestInputs[i][s]-minval)/range;
    } 

   }
    return ;
}



/* Normalize using the logistic function, as done by Jones in
   his paper. The function is

       1
   ---------
    1+e^-x
*/
void lnormalize()
{
  int s,i;
  
  
  for (i=0; i < NTrainingPatterns; i++) {
    for (s=0; s < Ninputs; s++) {
      TrainingInputs[i][s]=1.0/
	                   (1.0+exp(-(TrainingInputs[i][s])));
    }
  }

  for (i=0; i < NTestPatterns; i++) {
    for (s=0; s < Ninputs; s++) {
      TestInputs[i][s]=1.0/
	                   (1.0+exp(-(TestInputs[i][s])));
    }
  }



  return;
}
