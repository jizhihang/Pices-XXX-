/***********************************************************************/
/* Learning Utilities                                                  */
/***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <math.h>
#include "usfcascor.h"


void INTERACTIVE_PARM_UPDATE();
void CHECK_INTERRUPT();
void OUT_EPOCH_USER_INTERFACE();

void Y_OR_N_P();
void INTERACT_GET_WEIGHTS();

void IN_EPOCH_USER_INTERFACE();
void OUT_PASS_OUTPUT();
void IN_EPOCH_OUTPUT();
void OUT_EPOCH_OUTPUT();
void PRINT_SUMMARY();
float ERROR_INDEX();

float STANDARD_DEV();

int ErrorMisclassifications;

/*
 * Given the sum of weighted inputs, compute the unit's activation value.
 * Defined unit types are SIGMOID, VARSIGMOID, and GAUSSIAN.
 */
float ACTIVATION(float sum)
{
  float temp;

  switch(UnitType){
  case SIGMOID: 
    /* Sigmoid function in range -0.5 to 0.5. */
    if (sum < -15.0) 
      return(-0.5);
    else if (sum > 15.0) 
      return(0.5);
    else 
      return (1.0 /(1.0 + exp(-sum)) - 0.5);
  case GAUSSIAN:
    /* Gaussian activation function in range 0.0 to 1.0. */
    temp = -0.5 * sum * sum;
    if (temp < -75.0) 
      return(0.0);
    else 
      return (exp(temp));
  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    if (sum < -15.0) 
      return(0.0);
    else if (sum > 15.0) 
      return(1.0);
    else 
      return (1.0 /(1.0 + exp(-sum)));
  case VARSIGMOID: 
    /* Sigmoid function in range SigmoidMin to SigmoidMax. */
    if (sum < -15.0) 
      return(SigmoidMin);
    else if (sum > 15.0) 
      return(SigmoidMax);
    else 
      return ((SigmoidMax - SigmoidMin)/ (1.0 + exp(-sum)) + SigmoidMin);
  }
  return -1;
}


/*
 * Given the unit's activation value and sum of weighted inputs, compute
 * the derivative of the activation with respect to the sum.  Defined unit
 * types are SIGMOID, VARSIGMOID, and GAUSSIAN.
 *
 * Note: do not use sigmoid prime offset here, as it confuses the
 * correlation machinery.  But do use it in output-prime.
 * 
 */
float ACTIVATION_PRIME(float value, float sum)
{
  switch(UnitType){
  case SIGMOID: 
    /* Symmetrical sigmoid function. */
    return (0.25 -  value*value);
  case GAUSSIAN:
    /* Gaussian activation function. */
    return (sum * (- value));
  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    return (value * (1.0 - value));
  case VARSIGMOID: 
    /* Sigmoid function with range SigmoidMin to SigmoidMax. */
    return ((value - SigmoidMin) * (1.0 - (value - SigmoidMin) / 
				    (SigmoidMax - SigmoidMin)));
  }
  return -1;
}

/* Compute the value of an output, given the weighted sum of incoming values.
 * Defined output types are SIGMOID, ASYMSIGMOID, and LINEAR.
 */
float OUTPUT_FUNCTION(float sum)
{
  switch(OutputType){
  case SIGMOID: 
    /* Symmetrical sigmoid function, used for binary functions. */
    if (sum < -15.0) 
      return(-0.5);
    else if (sum > 15.0) 
      return(0.5);
    else 
      return (1.0 /(1.0 + exp(-sum)) - 0.5);
  case LINEAR:
    /* Linear output function, used for continuous functions. */
    return (sum);
  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    if (sum < -15.0) 
      return(0.0);
    else if (sum > 15.0) 
      return(1.0);
    else 
      return (1.0 /(1.0 + exp(-sum)));
  case VARSIGMOID: 
    /* Sigmoid function in range SigmoidMin to SigmoidMax. */
    if (sum < -15.0) 
      return(SigmoidMin);
    else if (sum > 15.0) 
      return(SigmoidMax);
    else 
      return ((SigmoidMax - SigmoidMin)/ (1.0 + exp(-sum))
	      + SigmoidMin);
  }
  return -1;
}

/* Compute the value of an output, given the weighted sum of incoming values.
 * Defined output types are SIGMOID, ASYMSIGMOID, and LINEAR.
 *
 * Sigmoid_Prime_Offset used to keep the back-prop error value from going to 
 * zero.
 */
float OUTPUT_PRIME(float output)
{
  switch(OutputType){
  case SIGMOID: 
    /* Symmetrical sigmoid function, used for binary functions. */
    return (SigmoidPrimeOffset + 0.25 -  output*output);
  case LINEAR:
    /* Linear output function, used for continuous functions. */
    return (1.0);
  case ASYMSIGMOID: 
    /* asymmetrical sigmoid function in range 0.0 to 1.0. */
    return (SigmoidPrimeOffset + output * (1.0 - output));
  case VARSIGMOID: 
    /* Sigmoid function with range SigmoidMin to SigmoidMax. */
    return (SigmoidPrimeOffset + 
	    (output - SigmoidMin) * (1.0 - (output - SigmoidMin) / 
				     (SigmoidMax - SigmoidMin)));
  }

  return -1;
}


/* The basic routine for doing quickprop-style update of weights, given a
 * pair of slopes and a delta.
 *
 * Given arrays holding weights, deltas, slopes, and previous slopes,
 * and an index i, update weight[i] and delta[i] appropriately.  Move
 * slope[i] to prev[i] and zero out slope[i].  Add weight decay term to
 * each slope before doing the update.
 */
void QUICKPROP_UPDATE(int i, float weights[], float deltas[], float slopes[], 
		      float prevs[], float epsilon, float decay, float mu, 
		      float shrink_factor)
{
  float w,d,s,p, next_step;
  /********/

  w = weights[i];
  d = deltas[i];
  s = slopes[i] +  decay * w;
  p = prevs[i];
  next_step = 0.0;

  /* The step must always be in direction opposite to the slope. */

  if(d < 0.0){			
    /* If last step was negative...  */  
    if(s > 0.0)	  
      /*  Add in linear term if current slope is still positive.*/
      next_step -= epsilon * s;
    /*If current slope is close to or larger than prev slope...  */
    if(s >= (shrink_factor*p)) 
      next_step += mu * d;	/* Take maximum size negative step. */
    else
      next_step += d * s / (p - s); /* Else, use quadratic estimate. */
  }
  else if(d > 0.0){
    /* If last step was positive...  */
    if(s < 0.0)	  
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
}
/* Set up all the inputs from the INPUT vector as the first few entries in
   in the values vector.
*/
void SETUP_INPUTS(float inputs[])
{

  int i;
/*********/

  Values[0]  = 1.0;		/* bias unit */
  for(i=0; i<Ninputs; i++)
    Values[i+1] = inputs[i];
}

/* Assume the values vector has been set up.  Just compute the output
   values.
*/
void OUTPUT_FORWARD_PASS(void)
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

}

/* Assume that values vector has been set up for units with index less
   than J.  Compute and record the value for unit J.
*/
void COMPUTE_UNIT_VALUE(int j)
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
}


/* Set up the inputs from the INPUT vector, then propagate activation values
   forward through all hidden units and output units.
*/
void  FULL_FORWARD_PASS(float input[])
{
  int j;
/********/

  SETUP_INPUTS (input);

  /* Unit values must be calculated in order because the activations */
  /* cascade down through the hidden layers */

  for(j= 1+Ninputs; j<Nunits; j++) /* For each hidden unit J, compute the */
    COMPUTE_UNIT_VALUE (j);	     /* activation value. */

  OUTPUT_FORWARD_PASS ();	/* Now compute outputs. */
}


/*  Goal is a vector of desired values for the output units.  Compute and
 *  record the output errors for the current training case.  Record error
 *  values and related statistics.  If output_slopesp is TRUE, then use errors
 *  to compute slopes for output weights.  If statsp is TRUE, accumulate error
 *  statistics. 
 */
void COMPUTE_ERRORS(float goal[], Boolean output_slopesp, Boolean statsp, int xw)
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
}

/* Update the output weights, using the pre-computed slopes, prev-slopes,
 * and delta values.
 */
void UPDATE_OUTPUT_WEIGHTS(void)
{
  int i,j;
  float eps;			/* epsilon scaled by fan-in */
/********/

  eps = OutputEpsilon / Ncases;

  for(j=0; j<Noutputs; j++)
    for(i=0; i<Nunits; i++)
      QUICKPROP_UPDATE(i, OutputWeights[j], OutputDeltas[j],
		       OutputSlopes[j], OutputPrevSlopes[j], eps,
		       OutputDecay, OutputMu, OutputShrinkFactor);

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
void TRAIN_OUTPUTS_EPOCH(void)
{
  int i;
/********/

  /* zero error accumulators */
  ErrorBits = 0;
  TrueError = 0.0;
  ErrorMisclassifications=0;
  for(i=0; i<Noutputs; i++){
    SumErrors[i] = 0.0;
  }
  SumSqError = 0.0;

  
  /* User may have changed mu between epochs, so fix shrink-factor. */
  OutputShrinkFactor = OutputMu / (1.0 + OutputMu);

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
    
}

/* Train the output weights.  If we exhaust max_epochs, stop with value
 * TIMEOUT.  If there are zero error bits, stop with value WIN.  Else,
 * keep going until the true error has changed by a significant amount,
 * and then until it does not change significantly for Patience epochs.
 * Then return STAGNANT.  If Patience is zero, we do not stop until victory
 * or until max_epochs is used up.
 */

int TRAIN_OUTPUTS(int max_epochs)
{
  int i;
  int retval = TIMEOUT;	  /* will be reset within loop for other conditions */
  float last_error = 0.0;
  int quit_epoch = Epoch + OutputPatience;
  Boolean first_time = True;
/********/

  for(i=0; i<max_epochs; i++){
    TRAIN_OUTPUTS_EPOCH();

    if((ErrorMeasure == BITS) && 
       (ErrorBits == 0)){
	retval = WIN;
	break;
      }
    else if((ErrorMeasure == INDEX) &&
	    (ErrorIndex <= ErrorIndexThreshold)){
	retval = WIN;
	break;
      }
    else if(OutputPatience == 0)
      continue;			/* continue training until victory */
    else if(first_time){
      first_time = False;
      last_error = TrueError;
    }
    else if(fabs(TrueError - last_error) > /* still getting better */
	    (last_error * OutputChangeThreshold)){
      last_error = TrueError;
      quit_epoch = Epoch + OutputPatience;
    }
    else if(Epoch >= quit_epoch){ /* haven't gotten better for a while */
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

}

/***********************************************************************/
/*                                                                     */
/*  Machinery for Training and selecting candidate units.              */
/*                                                                     */
/***********************************************************************/

/* Give new random weights to all of the candidate units.  Zero the other
 * candidate-unit statistics.
 */
void INIT_CANDIDATES(void)
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
}

/* Add the candidate-unit with the best correlation score to the active
 * network.  Then reinitialize the candidate pool.
 */
void INSTALL_NEW_UNIT(void)
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
  if(UseCache)
    for(i=0; i<NTrainingPatterns; i++){
      Values = ValuesCache[i];
      COMPUTE_UNIT_VALUE(Nunits);
    }

  /* Reinitialize candidate units with random weights.                  */
  Nunits++;
  INIT_CANDIDATES();    

}


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
void COMPUTE_CORRELATIONS(void)
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
}


/* NORMALIZE each accumulated correlation value, and stuff the normalized
 * form into the CandPrevCor data structure.  Then zero CandCor to
 * prepare for the next round.  Note the unit with the best total
 * correlation score.
 */
void ADJUST_CORRELATIONS(void)
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
}


/* After the correlations have been computed, we do a second pass over
 * the training set and adjust the input weights of all candidate units.
 */
void COMPUTE_SLOPES(void)
{
  int i,o,u;
  float sum, value, actprime, direction, error, change;
/*********/

  for(u=0; u<Ncandidates; u++){
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
      direction = (CandPrevCor[u][o] < 0.0) ? -1.0 : 1.0;
      change -= direction * actprime *((error -SumErrors[o])/SumSqError);
      CandCor[u][o] += error * value;
    }
    for(i=0; i<Nunits; i++)
       CandSlopes[u][i] += change * Values[i];
  }
}

/* Update the input weights, using the pre-computed slopes, prev-slopes,
 * and delta values.
 */
void UPDATE_INPUT_WEIGHTS(void)
{
  int i,u;
  float eps;
  float *cw, *cd, *cs, *cp;
/*********/

  eps = InputEpsilon / (float)(Ncases * Nunits);
  for(u=0; u<Ncandidates; u++){
    cw = CandWeights[u];
    cd = CandDeltas[u];
    cs = CandSlopes[u];
    cp = CandPrevSlopes[u];
    for(i=0; i<Nunits; i++)
      QUICKPROP_UPDATE(i, cw, cd, cs, cp, eps, InputDecay, InputMu, 
		       InputShrinkFactor);
  }
}

/* For each training pattern, perform a forward pass and compute correlations.
 * Then perform a second forward pass and compute input slopes for the 
 * candidate units.  Finally, use quickprop update to adjust the input weights.
 */

void TRAIN_INPUTS_EPOCH(void)
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
  InputShrinkFactor = InputMu / (1.0 + InputMu);

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
void CORRELATIONS_EPOCH(void)
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
    COMPUTE_CORRELATIONS();
  }
  /*  Fix up the correlation values for the next epoch. */
  ADJUST_CORRELATIONS();
  Epoch++;

}

/* Train the input weights of all candidates.  If we exhaust max_epochs,
 * stop with value TIMEOUT.  Else, keep going until the best candidate unit's
 * score has changed by a significant amount, and then
 * until it does not change significantly for Patience epochs.  Then return
 * STAGNANT.  If Patience is zero, we do not stop until victory or until
 * max_epochs is used up.
 */
int TRAIN_INPUTS(int max_epochs)
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

}
/**********************************************************************/
/*                                                                    */
/*  The outer loop routines                                           */
/*                                                                    */
/**********************************************************************/


void LIST_PARAMETERS(void)
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
int TRAIN(int outlimit, int inlimit, int  rounds)
{
  int i,r;

/***********/

  initialize_network();
  LIST_PARAMETERS();

  if(UseCache)
    for(i=0; i<NTrainingPatterns; i++){
      Values = ValuesCache[i];
      SETUP_INPUTS(TrainingInputs[i]);
    }


  for(r=0; r<rounds; r++){
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
    if ( save_network_each_round ) save_network(Trial,r);

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
 
}


/* Perform forward propagation once for each set of weights in the
 * testing vectors, computing errors.  Do not change any weights.
 */
void TEST_EPOCH(double test_threshold)
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

  ScoreThreshold = test_threshold;
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

}





/* Perform forward propagation once for each set of weights in the
 * testing vectors, computing errors.  Do not change any weights.
 */

void PREDICT(float test_threshold)
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

  if(ErrorMeasure == INDEX)
    ErrorIndex = ERROR_INDEX(TestStdDev, NtestOutputValues);
  fprintf(stderr,"  Test set:");
  
  PRINT_SUMMARY (NTestPatterns);

  fprintf (stderr, "\nAccuracy: %5.2f%% (%d/%d)\n", 
             100.0 * (NTestPatterns - test_examples_misclassified) / (float)NTestPatterns, 
             test_examples_misclassified, 
             NTestPatterns);

  /* restore globals */
  UseCache = old_UC;		
  ScoreThreshold = old_ST;
  TrueError = old_TE;	
  SumErrors = old_SE;	
  SumSqError = old_SSE;

  return;
}


/* print out the things interesting after a pass.
 */
void OUT_PASS_OUTPUT(void)
{
  int i;

  fprintf(stderr," Outputs: ");
  for(i=0;  i<Noutputs; i++)
    fprintf(stderr,"%6.4f ", Outputs[i]);

  fprintf(stderr,"\n Errors: ");
  for(i=0;i<Noutputs;i++)
    fprintf(stderr,"%6.4f ", Errors[i]);

  fprintf(stderr,"\n Values: ");
  for(i=0;i<Nunits;i++)
    fprintf(stderr,"%6.4f ", Values[i]);

  fprintf(stderr,"\n\n");
}



/* Print the summary statistics based on the value of ErrorMeasure.
 */
void PRINT_SUMMARY(int n)
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
}




/* Set up the network for a learning problem.  Clean up all the data
 * structures.  Initialize the output weights to random values controlled by
 * WeightRange.
 */
void initialize_network(void)
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
}




/* Calculate the standard deviation of an entire output set.
 */
float STANDARD_DEV(float **outputs, int npatterns, int nvalues)
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

  return(sqrt((fnum * sum_o_sqs - sum * sum)/
	      (fnum * (fnum - 1.0))));
}


/* ErrorIndex is the rms TrueError normalized by the standard deviation of the 
 * goal set.
 */
float ERROR_INDEX(double std_dev, int num)
{
  return(sqrt( TrueError / (float)num) / std_dev);
}








