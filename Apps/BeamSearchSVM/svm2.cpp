#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <string>
#include <stdarg.h>
#include <vector>
#include <assert.h>
#include <iostream>
#include "svm.h"








svm_model *svm_train (const svm_problem    *prob, 
                      const svm_parameter  *param
                     )
{
  svm_model *model = new svm_model();
  return model;
}  /* svm_train */




double  svm_predict (const svm_model       *model, 
                     const svm_node        *x, 
                     std::vector<double>&  dist,
                     std::vector<int>&     winners
                    )
{
  return  rand () % 2;
}  /* svm_predict */












void  svm_destroy_model  (svm_model*  model)
{
  delete model;	
}





