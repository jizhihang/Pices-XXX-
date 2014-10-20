#include  "FirstIncludes.h"

// Downloaded this code from  'http://www.csie.ntu.edu.tw/~cjlin/libsvm/'  on 2009-09-24
// Initial mods are nothing more than to Space out Code of interest to make more readable

#include <ctype.h>
#include <float.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdarg.h>
#include <vector>


#include  "MemoryDebug.h"
using namespace  std;


#include  "OSservices.h"
#include  "Str.h"
using namespace  KKU;


#include  "FeatureVector.h"
using namespace  MLL;

#include "svm2.h"
using  namespace  SVM289_MFS;


#pragma warning(disable : 4996)

//int32 libsvm_version = LIBSVM_VERSION;




namespace  SVM289_MFS
{
  void  sigmoid_train (int32            l, 
                       const double*  dec_values, 
                       const double*  labels, 
                       double&        A,
                       double&        B
                      );

  decision_function  svm_train_one (const svm_problem&    prob, 
                                    const svm_parameter&  param,
                                    double                Cp, 
                                    double                Cn,
                                    RunLog&               _log
                                   );

  double  sigmoid_predict (double  decision_value, 
                           double  A, 
                           double  B
                          );

  void  multiclass_probability (int32       k, /**< Number of Classes.      */
                                double**  r, /**< Pairwise Probabilites.  */
                                double*   p  /**< Class Probability       */
                               );

  void  svm_load_model_XML_SupportVectorSection (istream&     in,
                                                 FileDescPtr  fileDesc,
                                                 svm_model&   model,
                                                 bool&        valid,
                                                 RunLog&      log
                                                );

  // Stratified cross validation
  void  svm_cross_validation (const svm_problem&    prob, 
                              const svm_parameter&  param, 
                              int32                   nr_fold, 
                              double*               target,
                              RunLog&               log
                             );

  void  readline (FILE*    input,
                  char*&   buff,
                  int32&   buffLen,
                  bool&    eof
                 );

  template<class T> 
      inline T* GrowAllocation (T*   src, 
                                int32  origSize,
                                int32  newSize
                               )
  {
    int32  zed = 0;
    T*  dest = new T[newSize];
    while  (zed < origSize)    {dest[zed] = src[zed];  zed++;}
    while  (zed < newSize)     {dest[zed] = (T)0;      zed++;}
    delete  src;
    return  dest;
  }  /* GrowAllocation */


  double Min (double  x,  double   y) {return (x < y) ? x : y;}
  int32    Min (int32     x,  int32      y) {return (x < y) ? x : y;}
  long   Min (long    x,  long     y) {return (x < y) ? x : y;}
  float  Min (float   x,  float    y) {return (x < y) ? x : y;}
  uint32 Min (uint32  x,  uint32   y) {return (x < y) ? x : y;}

  int32    Max (int32     x,  int32      y) {return (x > y) ? x : y;}
  long   Max (long    x,  long     y) {return (x > y) ? x : y;}
  double Max (double  x,  double   y) {return (x > y) ? x : y;}
  float  Max (float   x,  float    y) {return (x > y) ? x : y;}
  uint32 Max (uint32  x,  uint32   y) {return (x > y) ? x : y;}


  inline double  powi (double base, int32 times)
  {
    double tmp = base, ret = 1.0;

    for  (int32 t = times;  t > 0;  t /= 2)
    {
      if  ((t % 2) == 1) 
       ret *= tmp;
      tmp = tmp * tmp;
    }
    return ret;
  }
}  /* SVM289_MFS */



#define INF HUGE_VAL
#define TAU 1e-12
//#define Malloc(type,n) (type *)malloc((n)*sizeof(type))




SVM289_MFS::svm_problem::svm_problem (const svm_problem&  _prob):
      l                  (_prob.l),
      selFeatures        (_prob.selFeatures),
      x                  (_prob.x, false),
      y                  (NULL)

{
  clone (y, _prob.y, l);
}




SVM289_MFS::svm_problem::svm_problem (const FeatureVectorList&  _x,
                                      const float*              _y,
                                      const FeatureNumList&     _selFeatures
                                     ):
  selFeatures (_selFeatures), 
  x           (_x, false),
  y           (NULL)
{
  l = _x.QueueSize ();

  y = new double[l];
  int32  idx = 0;
  for  (idx = 0;  idx < l;  idx++)
    y[idx] = _y[idx];
}



SVM289_MFS::svm_problem::svm_problem (const FeatureNumList&  _selFeatures,
                                      RunLog&                _log
                                     ):
  l           (0),
  selFeatures (_selFeatures), 
  x           (_selFeatures.FileDesc (), false, _log),
  y           (NULL)
{
  int32  zed = 87989;
}


SVM289_MFS::svm_problem::~svm_problem ()
{
  delete y;
  y = NULL;
}


    
RunLog&   SVM289_MFS::svm_problem::Log ()
{
  return  x.Log ();
}



FileDescPtr  SVM289_MFS::svm_problem::FileDesc ()  const
{
  return  x.FileDesc ();
}  



SVM289_MFS::svm_parameter::svm_parameter ():
  svm_type     (C_SVC),
  kernel_type  (RBF),
  degree       (3),
  gamma        (0.0),
  coef0        (0.0),
  cache_size   (40.0),
  eps          (1e-3),
  C            (1),
  nr_weight    (0),
  weight_label (NULL),
  weight       (NULL),
  nu           (0.5),
  p            (0.1),
  shrinking    (1),
  probability  (0),
  probParam    (0.0)
{
}


SVM289_MFS::svm_parameter::svm_parameter (const svm_parameter&  _param):
  svm_type     (_param.svm_type),
  kernel_type  (_param.kernel_type),
  degree       (_param.degree),
  gamma        (_param.gamma),
  coef0        (_param.coef0),
  cache_size   (_param.cache_size),
  eps          (_param.eps),
  C            (_param.C),
  nr_weight    (_param.nr_weight),
  weight_label (NULL),
  weight       (NULL),
  nu           (_param.nu),
  p            (_param.p),
  shrinking    (_param.shrinking),
  probability  (_param.probability),
  probParam    (_param.probParam)
{
  if  (_param.weight_label)
  {
    weight_label = new int32[nr_weight];
    for  (int32 x = 0;  x < nr_weight;  x++)
      weight_label[x] = _param.weight_label[x];
  }

  if  (_param.weight)
  {
    weight = new double[nr_weight];
    for  (int32 x = 0;  x < nr_weight;  x++)
      weight[x] = _param.weight[x];
  }
}



SVM289_MFS::svm_parameter::svm_parameter (KKStr&  paramStr):
  svm_type     (C_SVC),
  kernel_type  (RBF),
  degree       (3),
  gamma        (0.0),
  coef0        (0.0),
  cache_size   (40.0),
  eps          (1e-3),
  C            (1),
  nr_weight    (0),
  weight_label (NULL),
  weight       (NULL),
  nu           (0.5),
  p            (0.1),
  shrinking    (1),
  probability  (0),
  probParam    (0.0)
{
  cerr << endl << "VM289::svm_parameter::svm_parameter   Not Doing anything with 'paramStr'" << endl << endl;
}



SVM289_MFS::svm_parameter::~svm_parameter ()
{
  delete  weight_label;   weight_label = NULL;
  delete  weight;         weight       = NULL;
}



svm_parameter&  SVM289_MFS::svm_parameter::operator= (const svm_parameter& right)
{
  svm_type     = right.svm_type;
  kernel_type  = right.kernel_type;
  degree       = right.degree;
  gamma        = right.gamma;
  coef0        = right.coef0;
  cache_size   = right.cache_size;
  eps          = right.eps;
  C            = right.C;
  nr_weight    = right.nr_weight;
  weight_label = NULL;
  weight       = NULL;
  nu           = right.nu;
  p            = right.p;
  shrinking    = right.shrinking;
  probability  = right.probability;
  probParam    = right.probParam;
  
  if  (right.weight_label)
  {
    weight_label = new int32[nr_weight];
    for  (int32 x = 0;  x < nr_weight;  x++)
      weight_label[x] = right.weight_label[x];
  }

  if  (right.weight)
  {
    weight = new double[nr_weight];
    for  (int32 x = 0;  x < nr_weight;  x++)
      weight[x] = right.weight[x];
  }

  return  *this;
}



KKStr   SVM289_MFS::svm_parameter::ToCmdLineStr ()  const
{
  KKStr cmdStr (200); // Initialized char* allocation to 200

  cmdStr << "-CalcProb " << ((probability == 1) ? "Yes" : "No")  << "  "
         << "-c " << C              << "  "
         << "-d " << degree         << "  "
         << "-e " << eps            << "  "
         << "-g " << gamma          << "  "
         << "-h " << shrinking      << "  "
         << "-m " << cache_size     << "  "
         << "-n " << nu             << "  "
         << "-p " << p              << "  ";

  if  (probParam > 0.0)
    cmdStr << "-ProbParam " << probParam << "  ";
         
  cmdStr << "-r " << coef0          << "  "
         << "-s " << svm_type       << "  "
         << "-t " << kernel_type    << "  ";

  return  cmdStr;
}





void  SVM289_MFS::svm_parameter::ProcessSvmParameter (const KKStr&  cmd,
                                                      const KKStr&  value,
                                                      bool&         parmUsed
                                                     )
{
  parmUsed = true;

  double  valueNum = value.ToDouble ();

  if  (cmd.EqualIgnoreCase ("-B")         ||  
       cmd.EqualIgnoreCase ("-CP")        ||
       cmd.EqualIgnoreCase ("-CalcProb")  ||
       cmd.EqualIgnoreCase ("-CalcProbability")
      )
  {
    if  (value.ToBool ())
      probability = 1;
    else
      probability = 0;
  }

  else if  (cmd.EqualIgnoreCase ("-C")  ||  cmd.EqualIgnoreCase ("-Cost"))
    C  = valueNum;

  else if  (cmd.EqualIgnoreCase ("-D"))
    degree = (int32)valueNum;

  else if  (cmd.EqualIgnoreCase ("-E"))
    eps  = valueNum;

  else if  (cmd.EqualIgnoreCase ("-G")  ||  cmd.EqualIgnoreCase ("-GAMMA"))
    gamma = valueNum;

  else if  (cmd.EqualIgnoreCase ("-H"))
    shrinking = (int32)valueNum;

  else if  (cmd.EqualIgnoreCase ("-M"))
    cache_size = valueNum;

  else if  (cmd.EqualIgnoreCase ("-N"))
    nu = valueNum;
    
  else if  (cmd.EqualIgnoreCase ("-P"))
    p = valueNum;

  else if  (cmd.EqualIgnoreCase ("-R"))
    coef0 = valueNum;
    
  else if  (cmd.EqualIgnoreCase ("-S"))
    svm_type = SVM_Type_FromStr (value);

  else if  (cmd.EqualIgnoreCase ("-T"))
    kernel_type = SVM289_MFS::Kernel_Type_FromStr (value);

  else if  (cmd.EqualIgnoreCase ("-ProbParam")  ||  cmd.EqualIgnoreCase ("-PP"))
    probParam = valueNum;
    
  else if  (cmd.EqualIgnoreCase ("-W"))
  {
    ++nr_weight;
    weight_label = (int32 *) realloc (weight_label, sizeof (int32) * nr_weight);
    weight = (double *) realloc (weight, sizeof (double) * nr_weight);
    weight_label[nr_weight - 1] = atoi (cmd.SubStrPart (2).Str ());
    weight[nr_weight - 1]       = valueNum;
  }
  else
  {
    parmUsed = false;
  }
}  /* ProcessSvmParameter */






KKStr   SVM289_MFS::svm_parameter::ToTabDelStr ()  const
{
  KKStr  result (256);

  int32  x = 0;

  result << "svm_type"    << "\t"  << SVM_Type_ToStr    (svm_type)    << "\t"
         << "kernel_type" << "\t"  << Kernel_Type_ToStr (kernel_type) << "\t"
         << "degree"      << "\t"  << degree                          << "\t"
         << "gamma"       << "\t"  << gamma                           << "\t"
         << "coef0"       << "\t"  << coef0                           << "\t"
         << "cache_size"  << "\t"  << cache_size                      << "\t"
         << "eps"         << "\t"  << eps                             << "\t"
         << "C"           << "\t"  << C                               << "\t"
         << "nr_weight"   << "\t"  << nr_weight                       << "\t";

  if  (nr_weight > 0)
  {
    for  (x = 0;  x < nr_weight;  x++)
    {
      if  (x > 0)  result << ",";
      result << weight_label[x];
    }
    result << "\t";

    for  (x = 0;  x < nr_weight;  x++)
    {
      if  (x > 0)  result << ",";
      result << weight[x];
    }
    result << "\t";
  }

  result << "nu"          << "\t" << nu         << "\t"
         << "p"           << "\t" << p          << "\t"
         << "shrinking"   << "\t" << shrinking  << "\t"
         << "probability" << "\t" << probability;

  if  (probParam > 0.0)
    result << "\t" << "ProbParam" << "\t" << probParam;

  return  result;
}  /* ToTabDelStr */



void    SVM289_MFS::svm_parameter::ParseTabDelStr (const KKStr&  _str)
{
  KKStr  str = _str;
  int32  x;

  while  (!str.Empty ())
  {
    KKStr  field = str.ExtractToken2 ("\t");
    KKStr  value = str.ExtractToken2 ("\t");
    int32     valueI = value.ToInt    ();
    double  valueD = value.ToDouble ();
    float   valueF = value.ToFloat  ();

    if  (field == "svm_type")
      svm_type = SVM_Type_FromStr (value);

    else if  (field == "kernel_type")
      kernel_type = Kernel_Type_FromStr (value);

    else if  (field == "degree")      
      degree = valueI;

    else if  (field == "gamma")
      gamma = valueD;

    else if  (field == "coef0")
      coef0 = valueD;
    else if  (field == "cache_size")
      cache_size = valueD;

    else if  (field == "eps")
      eps = valueD;

    else if  (field == "C")
      C = valueD;

    else if  (field == "nr_weight")
    {
      nr_weight = valueI;
      if  (nr_weight > 0)
      {
        delete[]  weight_label;
        weight_label = new int32[nr_weight];

        // value = weight label.
        for  (x = 0;  x < nr_weight;  x++)
        {
          weight_label[x] = value.ExtractTokenInt (",");
        }

        delete[]  weight;
        weight = new double [nr_weight];
        KKStr  weightStr = str.ExtractToken2 ("\t");
        for  (x = 0;  x < nr_weight;  x++)
        {
          weight[x] = weightStr.ExtractTokenDouble (",");
        }
      }
    }

    else if  (field == "nu")
      nu = valueD;

    else if  (field == "p")
      p = valueD;

    else if  (field == "shrinking")
      shrinking = valueI;

    else if  (field == "probability")
      probability = valueI;

    else if  (field.EqualIgnoreCase ("probparam")  ||  field.EqualIgnoreCase ("pp"))
      probParam = valueD;
  }
}  /* ParseTabDelStr */






SVM_Type  SVM289_MFS::SVM_Type_FromStr (KKStr  s)
{
  s.Upper ();

  if  ((s.EqualIgnoreCase ("C_SVC"))       ||  (s == "0"))
    return C_SVC;

  if  ((s.EqualIgnoreCase ("NU_SVC"))       ||  (s == "1"))
    return NU_SVC;

  if  ((s.EqualIgnoreCase ("ONE_CLASS"))    ||  (s == "2"))
    return  ONE_CLASS;

  if  ((s.EqualIgnoreCase ("EPSILON_SVR"))  ||  (s == "3"))
    return  EPSILON_SVR;

  if  ((s.EqualIgnoreCase ("NU_SVR"))       ||  (s == "4"))
    return  NU_SVR;

  return  SVM_NULL;
}



KKStr  SVM289_MFS::SVM_Type_ToStr (SVM_Type  svmType)
{
  switch  (svmType)
  {
  case  C_SVC:        return  "c_svc";
  case  NU_SVC:       return  "nu_svc";
  case  ONE_CLASS:    return  "one_class";
  case  EPSILON_SVR:  return  "epsilon_svr";
  case  NU_SVR:       return  "nu_svr";
  }
  return  SVM_NULL;
}



 Kernel_Type  SVM289_MFS::Kernel_Type_FromStr (KKStr  s)
 {
   s.Upper ();
   if  ((s.EqualIgnoreCase ("LINEAR"))       ||  (s == "0"))
     return  LINEAR;

   if  ((s.EqualIgnoreCase ("POLYNOMIAL"))   ||  (s.EqualIgnoreCase ("POLY"))  ||  (s == "1"))
     return  POLY;

   if  ((s.EqualIgnoreCase ("RBF"))          ||  (s == "2"))
     return  RBF;

   if  ((s.EqualIgnoreCase ("SIGMOID"))      ||  (s == "3"))
     return  SIGMOID;

   if  ((s.EqualIgnoreCase ("PRECOMPUTED"))  ||  (s == "4"))
     return  PRECOMPUTED;

   return  Kernel_NULL;
 }

 
 


KKStr  SVM289_MFS::Kernel_Type_ToStr (Kernel_Type   kernelType)
{
  switch  (kernelType)
  {
  case  LINEAR:       return  "linear";
  case  POLY:         return  "polynomial";
  case  RBF:          return  "rbf";
  case  SIGMOID:      return  "sigmoid";
  case  PRECOMPUTED:  return  "precomputed";
  } 

  return  "";
}




static void print_string_stdout (const char *s)
{
  fputs(s,stdout);
  fflush(stdout);
}


void (*SVM289_MFS::svm_print_string) (const char *) = &print_string_stdout;
#if 1
static void info(const char *fmt,...)
{
  char buf[BUFSIZ];
  va_list ap;
  va_start(ap,fmt);

#if  defined(USE_SECURE_FUNCS)
  vsprintf_s(buf, BUFSIZ, fmt, ap);
#else
  vsprintf(buf,fmt,ap);
#endif

  va_end(ap);
  (*SVM289_MFS::svm_print_string)(buf);
}
#else
static void info(const char *fmt,...) {}
#endif



//
// Kernel Cache
//
// l is the number of total data items
// size is the cache size limit in bytes
//
class  SVM289_MFS::Cache
{
public:
  Cache (int32 l,
         int32 size
        );

  ~Cache();


  // request data [0,len)
  // return some position p where [p,len) need to be filled
  // (p >= len if nothing needs to be filled)
  int32  get_data(const int32  index, 
                Qfloat**   data, 
                int32        len
               );

  void swap_index (int32 i, int32 j);  

private:
  int32  l;
  int32  size;

  struct  head_t
  {
    head_t *prev, *next;  // a circular list
    Qfloat *data;
    int32    len;           // data[0,len) is cached in this entry
  };

  head_t*     head;
  head_t      lru_head;

  void lru_delete (head_t * h);
  void lru_insert (head_t * h);
};



SVM289_MFS::Cache::Cache (int32  l_,
                          int32  size_
                         ):
      l    (l_),
      size (size_)
{
  head = (head_t *)calloc (l, sizeof (head_t));  // initialized to 0
  size /= sizeof (Qfloat);
  size -= l * sizeof (head_t) / sizeof (Qfloat);
  size = Max (size, 2 * (int32) l);  // cache must be large enough for two columns
  lru_head.next = lru_head.prev = &lru_head;
}



SVM289_MFS::Cache::~Cache()
{
  for  (head_t* h = lru_head.next;  h != &lru_head;  h = h->next)
  {delete (h->data);  h->data = NULL;}
  delete head;  head = NULL;
}



void Cache::lru_delete (head_t *h)
{
  // delete from current location
  h->prev->next = h->next;
  h->next->prev = h->prev;
}



void Cache::lru_insert (head_t *h)
{
  // insert to last position
  h->next = &lru_head;
  h->prev = lru_head.prev;
  h->prev->next = h;
  h->next->prev = h;
}



int32  Cache::get_data (const int32  index, 
                      Qfloat**   data, 
                      int32        len
                     )
{
  head_t*  h = &head[index];
  if  (h->len) 
    lru_delete (h);

  int32  more = len - h->len;

  if  (more > 0)
  {
    // free old space
    while  (size < more)
    {
      head_t*  old = lru_head.next;
      lru_delete (old);
      delete old->data;  old->data = NULL;
      size += old->len;
      old->data = 0;
      old->len  = 0;
    }

    // allocate new space
    h->data = (Qfloat *)realloc(h->data, sizeof (Qfloat) * len);
    size -= more;
    SVM289_MFS::swap (h->len, len);
  }

  lru_insert (h);
  *data = h->data;
  return len;
}



void Cache::swap_index (int32 i, int32 j)
{
  if  ( i == j) 
    return;

  if  (head[i].len) 
    lru_delete (&head[i]);

  if  (head[j].len) 
    lru_delete(&head[j]);


  SVM289_MFS::swap (head[i].data, head[j].data);

  SVM289_MFS::swap (head[i].len,  head[j].len);

  if  (head[i].len) 
    lru_insert(&head[i]);

  if  (head[j].len) 
    lru_insert(&head[j]);

  if  (i > j) 
    SVM289_MFS::swap(i, j);


  for  (head_t*  h = lru_head.next;  h != &lru_head;  h = h->next)
  {
    if  (h->len > i)
    {
      if  (h->len > j)
      {
        SVM289_MFS::swap (h->data[i], h->data[j]);
      }
      else
      {
        // give up
        lru_delete (h);
        delete h->data;  h->data = NULL;
        size += h->len;
        h->data = 0;
        h->len  = 0;
      } 
    }
  }
}  /* swap_index */








//
// Kernel evaluation
//
// the static method k_function is for doing single kernel evaluation
// the constructor of Kernel prepares to calculate the l*l kernel matrix
// the member function get_Q is for getting one column from the Q Matrix
//
class  SVM289_MFS::QMatrix 
{
public:
  virtual Qfloat*  get_Q  (int32 column, int32 len) const = 0;
  virtual Qfloat*  get_QD () const = 0;
  virtual void swap_index (int32 i, int32 j)  = 0;
  virtual ~QMatrix() {}
};





class  SVM289_MFS::Kernel: public QMatrix 
{
public:
  Kernel (const FeatureVectorList&  _x,
          const FeatureNumList&     _selFeatures, 
          const svm_parameter&      _param,
          RunLog&                   _log
         );

  virtual ~Kernel();

  static double k_function (const FeatureVector&   x, 
                            const FeatureVector&   y,
                            const svm_parameter&   param,
                            const FeatureNumList&  selFeatures
                           );

  static double  DotStatic (const FeatureVector&   px, 
                            const FeatureVector&   py,
                            const FeatureNumList&  selFeatures
                           );

  virtual Qfloat*  get_Q      (int32 column, int32 len) const = 0;
  virtual Qfloat*  get_QD     () const = 0;
  virtual void     swap_index (int32 i, int32 j)   // no so const...
  {
    //swap (x[i], x[j]);
    x->SwapIndexes (i, j);
    if  (x_square) 
      swap (x_square[i], x_square[j]);
  }

protected:
  double (Kernel::*kernel_function) (int32 i, int32 j) const;

private:
  int32                l;
  int32                numSelFeatures;
  int32*               selFeatures;
  FeatureVectorListPtr  x;
  double*            x_square;

  float**            preComputed;

  // svm_parameter
  const int32         kernel_type;
  const int32         degree;
  const double      gamma;
  const double      coef0;

  double  dot (const FeatureVector&  px, 
               const FeatureVector&  py
              )  const;


  double kernel_linear (int32 i, int32 j) const
  {
    return dot ((*x)[i], (*x)[j]);
  }


  double  kernel_poly (int32 i, int32 j) const
  {
    return  powi (gamma * dot((*x)[i], (*x)[j]) + coef0, degree);
  }


  double  kernel_rbf (int32 i, int32 j) const
  {
    return exp (-gamma * (x_square[i] + x_square[j] - 2 * dot ((*x)[i], (*x)[j])));
  }


  double kernel_sigmoid (int32 i, int32 j) const
  {
    return tanh (gamma * dot ((*x)[i], (*x)[j]) + coef0);
  }


  double kernel_precomputed (int32 i, int32 j) const
  {
    if  (preComputed)
      return  preComputed[i][j];
    else
      return 0.0f;
  }
};  /* Kernel */




SVM289_MFS::Kernel::Kernel (const FeatureVectorList&  _x,
                        const FeatureNumList&     _selFeatures, 
                        const svm_parameter&      _param,
                        RunLog&                   _log
                       ):

   coef0          (_param.coef0),
   degree         (_param.degree),
   gamma          (_param.gamma), 
   kernel_type    (_param.kernel_type), 
   l              (_x.QueueSize ()),
   numSelFeatures (0),
   preComputed    (NULL),
   selFeatures    (NULL),
   x              (NULL)

{
  x = new FeatureVectorList (_x, false);

  numSelFeatures = _selFeatures.NumSelFeatures ();
  selFeatures = new int32[numSelFeatures];
  for  (int32 zed = 0;  zed < numSelFeatures;  zed++)
    selFeatures[zed] = _selFeatures[zed];

  switch  (kernel_type)
  {
    case LINEAR:
      kernel_function = &Kernel::kernel_linear;
      break;

    case POLY:
      kernel_function = &Kernel::kernel_poly;
      break;

    case RBF:
      kernel_function = &Kernel::kernel_rbf;
      break;

    case SIGMOID:
      kernel_function = &Kernel::kernel_sigmoid;
      break;

    case PRECOMPUTED:
      {
        int32  z1 = 0;
        int32  z2 = 0;
        kernel_function = &Kernel::kernel_precomputed;
        preComputed = new float*[l];
        for  (z1 = 0;  z1 < l;  z1++)
        {
          preComputed[z1] = new float[l];
          for  (z2 = 0;  z2 < l;  z2++)
            preComputed[z1][z2] = 0.0f;
        }
      }
      break;
  }

  if  (kernel_type == RBF)
  {
    x_square = new double[l];
    for  (int32 i = 0;  i < l;  i++)
      x_square[i] = dot ((*x)[i], (*x)[i]);
  }

  else
  {
    x_square = 0;
  }
}




SVM289_MFS::Kernel::~Kernel()
{
  delete[] selFeatures;    selFeatures = NULL;
  delete[] x_square;       x_square    = NULL;

  if  (preComputed)
  {
    int32  n = x->QueueSize ();
    int32 z1 = 0;
    for  (z1 = 0;  z1 < l;  z1++)
      delete  preComputed[z1];
    delete  preComputed;
    preComputed = NULL;
  }

  delete  x;
  x = NULL;
}




double  SVM289_MFS::Kernel::dot (const FeatureVector&  px, 
                             const FeatureVector&  py
                            )  const
{
  double  sum = 0;
  int32  fn = 0;
  int32  idx = 0;

  const float*  fvX  = px.FeatureData ();
  const float*  fvY  = py.FeatureData ();

  for  (idx = 0;  idx < numSelFeatures;  idx++)
  {
    fn = selFeatures[idx];
    sum += fvX[fn] * fvY[fn];
  }

  return sum;
}  /* dot */



double  SVM289_MFS::Kernel::DotStatic (const FeatureVector&   px, 
                                   const FeatureVector&   py,
                                   const FeatureNumList&  selFeatures
                                  ) 
{
  int32  numFeatures = selFeatures.NumSelFeatures ();

  double  sum = 0;
  int32  fn = 0;
  int32  idx = 0;

  const float*  fvX  = px.FeatureData ();
  const float*  fvY  = py.FeatureData ();

  for  (idx = 0;  idx < numFeatures;  idx++)
  {
    fn = selFeatures[idx];
    sum += fvX[fn] * fvY[fn];
  }

  return sum;
}  /* dot */







double  SVM289_MFS::Kernel::k_function  (const FeatureVector&   x, 
                                     const FeatureVector&   y,
                                     const svm_parameter&   param,
                                     const FeatureNumList&  selFeatures
                                    )
{
  switch  (param.kernel_type)
  {
    case LINEAR:
      return DotStatic(x, y, selFeatures);

    case POLY:
      return powi (param.gamma * DotStatic (x, y, selFeatures) + param.coef0,param.degree);

    case RBF:
    {
      int32  numSelFeatures = selFeatures.NumSelFeatures ();
      int32  fn  = 0;
      int32  idx = 0;
      const float*  fvX = x.FeatureData ();
      const float*  fvY = y.FeatureData ();

      double sum = 0;
      for  (idx = 0;  idx < numSelFeatures;  idx++)
      {
        fn = selFeatures[idx];
        double d = fvX[fn] - fvY[fn];
        sum += d * d;
      }
      
      return exp (-param.gamma * sum);
    }

    case SIGMOID:
      return tanh(param.gamma * DotStatic (x, y, selFeatures) + param.coef0);

    case PRECOMPUTED:  //x: test (validation), y: SV
      {
        cerr << endl
             << "SVM289_MFS::Kernel::k_function   ***ERROR*** does not support 'PRECOMPUTED'." << endl
             << endl;
        return 0;
      }

    default:
      return 0;  // Unreachable 
  }
}  /* k_function */





// An SMO algorithm in Fan et al., JMLR 6(2005), p. 1889--1918
// Solves:
//
//  Min 0.5(\alpha^T Q \alpha) + p^T \alpha
//
//    y^T \alpha = \delta
//    y_i = +1 or -1
//    0 <= alpha_i <= Cp for y_i = 1
//    0 <= alpha_i <= Cn for y_i = -1
//
// Given:
//
//  Q, p, y, Cp, Cn, and an initial feasible point \alpha
//  l is the size of vectors and matrices
//  eps is the stopping tolerance
//
// solution will be put in \alpha, objective value will be put in obj
//
class  SVM289_MFS::Solver {
public:
  Solver() {};
  virtual ~Solver() {};

  struct SolutionInfo {
    double obj;
    double rho;
    double upper_bound_p;
    double upper_bound_n;
    double r;  // for Solver_NU
  };

  void  Solve (int32            l, 
               QMatrix&       Q, 
               const double*  p_, 
               const schar*   y_,
               double*        alpha_, 
               double         Cp, 
               double         Cn, 
               double         eps,
               SolutionInfo*  si, 
               int32            shrinking
              );

protected:
  int32             active_size;
  schar*          y;
  double*         G;   // gradient of objective function
  enum  {LOWER_BOUND, UPPER_BOUND, FREE};
  char*           alpha_status;  // LOWER_BOUND, UPPER_BOUND, FREE
  double*         alpha;
  QMatrix*        Q;
  const Qfloat*   QD;
  double          eps;
  double          Cp;
  double          Cn;
  double*         p;
  int32*            active_set;
  double*         G_bar;     // gradient, if we treat free variables as 0
  int32             l;
  bool            unshrink;  // XXX


  double get_C (int32 i)
  {
    return (y[i] > 0) ? Cp : Cn;
  }


  void  update_alpha_status (int32 i)
  {
    if  (alpha[i] >= get_C(i))
      alpha_status[i] = UPPER_BOUND;

    else  if(alpha[i] <= 0)
      alpha_status[i] = LOWER_BOUND;

    else alpha_status[i] = FREE;
  }


  bool is_upper_bound (int32 i) {return alpha_status[i] == UPPER_BOUND;}
  bool is_lower_bound (int32 i) {return alpha_status[i] == LOWER_BOUND;}
  bool is_free        (int32 i) {return alpha_status[i] == FREE;}

  void swap_index (int32 i, int32 j);

  void reconstruct_gradient ();

  virtual int32    select_working_set (int32&  i, int32&  j);

  virtual double calculate_rho ();

  virtual void   do_shrinking ();


private:
  bool  be_shrunk (int32    i, 
                   double Gmax1, 
                   double Gmax2
                  );  
};  /* Solver */




void  SVM289_MFS::Solver::swap_index  (int32 i, int32 j)
{
  Q->swap_index (i, j);
  SVM289_MFS::swap (y[i], y[j]);
  SVM289_MFS::swap (G[i], G[j]);
  SVM289_MFS::swap (alpha_status[i], alpha_status[j]);
  SVM289_MFS::swap (alpha[i], alpha[j]);
  SVM289_MFS::swap (p[i], p[j]);
  SVM289_MFS::swap (active_set[i],active_set[j]);
  SVM289_MFS::swap (G_bar[i], G_bar[j]);
}  /* swap_index */



void  SVM289_MFS::Solver::reconstruct_gradient ()
{
  // reconstruct inactive elements of G from G_bar and free variables

  if  (active_size == l) 
    return;

  int32 i,j;
  int32 nr_free = 0;

  for  (j = active_size;  j < l;  j++)
    G[j] = G_bar[j] + p[j];

  for  (j = 0;  j < active_size;  j++)
  {
    if  (is_free(j))
      nr_free++;
  }

  if  (2 * nr_free < active_size)
    info("\nWarning: using -h 0 may be faster\n");

  if  (nr_free*l > 2 * active_size * (l - active_size))
  {
    for  (i = active_size;  i < l;  i++)
    {
      Qfloat *Q_i = Q->get_Q (i, active_size);
      for  (j = 0;  j < active_size;  j++)
      {
        if  (is_free (j))
          G[i] += alpha[j] * Q_i[j];
      }
    }
  }
  else
  {
    for  (i = 0;  i < active_size;  i++)
    {
      if  (is_free (i))
      {
        Qfloat*  Q_i = Q->get_Q (i,l);
        double alpha_i = alpha[i];
        for  (j = active_size;  j < l;  j++)
          G[j] += alpha_i * Q_i[j];
      }
    }
  }
}  /* reconstruct_gradient */





void  SVM289_MFS::Solver::Solve (int32              l, 
                             QMatrix&         Q, 
                             const double*    p_, 
                             const schar*     y_,
                             double*          alpha_, 
                             double           Cp, 
                             double           Cn, 
                             double           eps,
                             SolutionInfo*    si, 
                             int32              shrinking
                            )
{
  this->l = l;
  this->Q = &Q;
  QD=Q.get_QD();
  clone(p, p_,l);
  clone(y, y_,l);
  clone(alpha,alpha_,l);
  this->Cp = Cp;
  this->Cn = Cn;
  this->eps = eps;
  unshrink = false;

  // initialize alpha_status
  {
    alpha_status = new char[l];
    for  (int32 i = 0;  i < l;  i++)
      update_alpha_status (i);
  }

  // initialize active set (for shrinking)
  {
    active_set = new int32[l];
    for  (int32 i = 0;  i < l;  i++)
      active_set[i] = i;
    active_size = l;
  }

  // initialize gradient
  {
    G     = new double[l];
    G_bar = new double[l];
    int32  i;
    for  (i = 0;  i < l;  i++)
    {
      G[i]     = p[i];
      G_bar[i] = 0;
    }

    for(i=0;i<l;i++)
    {
      if  (!is_lower_bound (i))
      {
        Qfloat *Q_i = Q.get_Q(i,l);
        double alpha_i = alpha[i];
        int32  j;
        for  (j = 0;  j < l;  j++)
          G[j] += alpha_i*Q_i[j];

        if  (is_upper_bound (i))
        {
          for  (j = 0;  j < l;  j++)
            G_bar[j] += get_C(i) * Q_i[j];
        }
      }
    }
  }

  // optimization step

  int32 iter = 0;
  int32 counter = Min (l, 1000) + 1;

  while(1)
  {
    // show progress and do shrinking

    if (--counter == 0)
    {
      counter = Min (l, 1000);
      if  (shrinking) 
        do_shrinking();
      info(".");
    }

    int32 i, j;
    if  (select_working_set (i, j) != 0)   // 'select_working_set' == 1 if already optimal otherwise 0.
    {
      // reconstruct the whole gradient
      reconstruct_gradient ();
      // reset active set size and check
      active_size = l;
      info ("*");
      if  (select_working_set (i, j) != 0)
        break;
      else
        counter = 1;  // do shrinking next iteration
    }
    
    ++iter;

    // update alpha[i] and alpha[j], handle bounds carefully
    
    const Qfloat *Q_i = Q.get_Q(i,active_size);
    const Qfloat *Q_j = Q.get_Q(j,active_size);

    double C_i = get_C(i);
    double C_j = get_C(j);

    double old_alpha_i = alpha[i];
    double old_alpha_j = alpha[j];

    if  (y[i] != y[j])
    {
      double  quad_coef = Q_i[i] + Q_j[j] +2 *Q_i[j];
      if  (quad_coef <= 0)
        quad_coef = TAU;

      double delta = (-G[i] - G[j]) / quad_coef;
      double diff = alpha[i] - alpha[j];
      alpha[i] += delta;
      alpha[j] += delta;
      
      if  (diff > 0)
      {
        if  (alpha[j] < 0)
        {
          alpha[j] = 0;
          alpha[i] = diff;
        }
      }
      else
      {
        if  (alpha[i] < 0)
        {
          alpha[i] = 0;
          alpha[j] = -diff;
        }
      }
      if  (diff > C_i - C_j)
      {
        if(alpha[i] > C_i)
        {
          alpha[i] = C_i;
          alpha[j] = C_i - diff;
        }
      }
      else
      {
        if  (alpha[j] > C_j)
        {
          alpha[j] = C_j;
          alpha[i] = C_j + diff;
        }
      }
    }
    else
    {
      double quad_coef = Q_i[i] + Q_j[j] - 2 * Q_i[j];

      if  (quad_coef <= 0)
        quad_coef = TAU;

      double delta = (G[i] - G[j]) / quad_coef;
      double sum = alpha[i] + alpha[j];
      alpha[i] -= delta;
      alpha[j] += delta;

      if  (sum > C_i)
      {
        if  (alpha[i] > C_i)
        {
          alpha[i] = C_i;
          alpha[j] = sum - C_i;
        }
      }
      else
      {
        if  (alpha[j] < 0)
        {
          alpha[j] = 0;
          alpha[i] = sum;
        }
      }
      if  (sum > C_j)
      {
        if  (alpha[j] > C_j)
        {
          alpha[j] = C_j;
          alpha[i] = sum - C_j;
        }
      }
      else
      {
        if  (alpha[i] < 0)
        {
          alpha[i] = 0;
          alpha[j] = sum;
        }
      }
    }

    // update G

    double delta_alpha_i = alpha[i] - old_alpha_i;
    double delta_alpha_j = alpha[j] - old_alpha_j;
    
    for  (int32 k = 0;  k < active_size;  k++)
    {
      G[k] += Q_i[k] * delta_alpha_i + Q_j[k] * delta_alpha_j;
    }

    // update alpha_status and G_bar

    {
      bool ui = is_upper_bound(i);
      bool uj = is_upper_bound(j);
      update_alpha_status (i);
      update_alpha_status (j);
      int32 k;
      if  (ui != is_upper_bound (i))
      {
        Q_i = Q.get_Q (i,l);
        if  (ui)
        {
          for  (k = 0;  k < l;  k++)
            G_bar[k] -= C_i * Q_i[k];
        }
        else
        {
          for  (k = 0;  k < l;  k++)
            G_bar[k] += C_i * Q_i[k];
        }
      }

      if(uj != is_upper_bound(j))
      {
        Q_j = Q.get_Q(j,l);
        if  (uj)
        {
          for  (k = 0;  k < l;  k++)
            G_bar[k] -= C_j * Q_j[k];
        }
        else
        {
          for  (k = 0;  k < l;  k++)
            G_bar[k] += C_j * Q_j[k];
        }
      }
    }
  }

  // calculate rho
  si->rho = calculate_rho();

  // calculate objective value
  {
    double v = 0;
    int32 i;
    for  (i = 0;  i < l;  i++)
      v += alpha[i] * (G[i] + p[i]);

    si->obj = v/2;
  }

  // put back the solution
  {
    for  (int32 i = 0;  i < l;  i++)
    {
      alpha_[active_set[i]] = alpha[i];
    }
  }

  // juggle everything back
  /*{
    for(int32 i=0;i<l;i++)
      while(active_set[i] != i)
        swap_index(i,active_set[i]);
        // or Q.swap_index(i,active_set[i]);
  }*/

  si->upper_bound_p = Cp;
  si->upper_bound_n = Cn;

  //info("\noptimization finished, #iter = %d\n",iter);

  delete[] p;
  delete[] y;
  delete[] alpha;
  delete[] alpha_status;
  delete[] active_set;
  delete[] G;
  delete[] G_bar;
}  /* Solve */




// return 1 if already optimal, return 0 otherwise
int32  SVM289_MFS::Solver::select_working_set (int32&  out_i, 
                                               int32&  out_j
                                              )
{
  // return i,j such that
  // i: maximizes -y_i * grad(f)_i, i in I_up(\alpha)
  // j: minimizes the decrease of obj value
  //    (if quadratic coefficeint <= 0, replace it with tau)
  //    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(\alpha)
  
  double  Gmax         = -INF;
  double  Gmax2        = -INF;
  int32     Gmax_idx     = -1;
  int32     Gmin_idx     = -1;
  double  obj_diff_min = INF;

  for  (int32 t=0;t<active_size;t++)
  {
    if  (y[t] == +1)
    {
      if  (!is_upper_bound (t))
      {
        if  (-G[t] >= Gmax)
        {
          Gmax     = -G[t];
          Gmax_idx = t;
        }
      }
    }
    else
    {
      if  (!is_lower_bound (t))
      {
        if  (G[t] >= Gmax)
        {
          Gmax = G[t];
          Gmax_idx = t;
        }
      }
    }
  }


  int32 i = Gmax_idx;
  const Qfloat *Q_i = NULL;
  if  (i != -1) // NULL Q_i not accessed: Gmax=-INF if i=-1
    Q_i = Q->get_Q(i,active_size);

  for(int32 j=0;j<active_size;j++)
  {
    if(y[j]==+1)
    {
      if (!is_lower_bound(j))
      {
        double grad_diff=Gmax+G[j];
        if (G[j] >= Gmax2)
          Gmax2 = G[j];

        if  (grad_diff > 0)
        {
          double obj_diff; 
          double quad_coef = Q_i[i] + QD[j] - 2.0 * y[i] * Q_i[j];

          if  (quad_coef > 0)
            obj_diff = -(grad_diff*grad_diff)/quad_coef;
          else
            obj_diff = -(grad_diff*grad_diff)/TAU;

          if  (obj_diff <= obj_diff_min)
          {
            Gmin_idx=j;
            obj_diff_min = obj_diff;
          }
        }
      }
    }
    else
    {
      if  (!is_upper_bound(j))
      {
        double grad_diff= Gmax-G[j];
        if (-G[j] >= Gmax2)
          Gmax2 = -G[j];
        if (grad_diff > 0)
        {
          double obj_diff; 
          double quad_coef=Q_i[i]+QD[j]+2.0*y[i]*Q_i[j];
          if (quad_coef > 0)
            obj_diff = -(grad_diff*grad_diff)/quad_coef;
          else
            obj_diff = -(grad_diff*grad_diff)/TAU;

          if (obj_diff <= obj_diff_min)
          {
            Gmin_idx=j;
            obj_diff_min = obj_diff;
          }
        }
      }
    }
  }

  if  (Gmax + Gmax2  <  eps)
    return 1;

  out_i = Gmax_idx;
  out_j = Gmin_idx;
  return 0;
}  /* select_working_set */





bool  SVM289_MFS::Solver::be_shrunk(int32 i, double Gmax1, double Gmax2)
{
  if  (is_upper_bound(i))
  {
    if  (y[i] == +1)
      return (-G[i] > Gmax1);
    else
      return (-G[i] > Gmax2);
  }

  else if  (is_lower_bound(i))
  {
    if  (y[i] == +1)
      return (G[i] > Gmax2);
    else  
      return (G[i] > Gmax1);
  }

  else
  {
    return(false);
  }
}  /* be_shrunk */




void   Solver::do_shrinking()
{
  int32 i;
  double  Gmax1 = -INF;    // Max { -y_i * grad(f)_i | i in I_up(\alpha) }
  double  Gmax2 = -INF;    // Max { y_i * grad(f)_i | i in I_low(\alpha) }

  // find maximal violating pair first
  for  (i = 0;  i < active_size;  i++)
  {
    if  (y[i] == +1)
    {
      if  (!is_upper_bound (i))  
      {
        if  (-G[i] >= Gmax1)
          Gmax1 = -G[i];
      }
      if  (!is_lower_bound (i))  
      {
        if  (G[i] >= Gmax2)
          Gmax2 = G[i];
      }
    }

    else  
    {
      if  (!is_upper_bound (i))
      {
        if  (-G[i] >= Gmax2)
          Gmax2 = -G[i];
      }

      if  (!is_lower_bound (i))
      {
        if  (G[i] >= Gmax1)
          Gmax1 = G[i];
      }
    }
  }

  if  ((unshrink == false) && ((Gmax1 + Gmax2) <= (eps * 10)))
  {
    unshrink = true;
    reconstruct_gradient();
    active_size = l;
    info ("*");
  }

  for  (i = 0;  i < active_size;  i++)
  {
    if  (be_shrunk(i, Gmax1, Gmax2))
    {
      active_size--;
      while  (active_size > i)
      {
        if  (!be_shrunk (active_size, Gmax1, Gmax2))
        {
          swap_index (i, active_size);
          break;
        }
        active_size--;
      }
    }
  }
}  /* do_shrinking */




double  SVM289_MFS::Solver::calculate_rho ()
{
  double  r;
  int32     nr_free  = 0;
  double  ub       = INF;
  double  lb       = -INF;
  double  sum_free = 0;

  for  (int32 i = 0;  i < active_size;  i++)
  {
    double yG = y[i] * G[i];

    if  (is_upper_bound(i))
    {
      if  (y[i] == -1)
        ub = Min (ub, yG);
      else
        lb = Max (lb, yG);
    }

    else if  (is_lower_bound(i))
    {
      if  (y[i] == +1)
        ub = Min (ub,yG);
      else
        lb = Max (lb,yG);
    }
    else
    {
      ++nr_free;
      sum_free += yG;
    }
  }

  if  (nr_free > 0)
    r = sum_free / nr_free;
  else
    r = (ub + lb) / 2;

  return r;
}  /* calculate_rho */





//
// Solver for nu-svm classification and regression
//
// additional constraint: e^T \alpha = constant
//
class  SVM289_MFS::Solver_NU : public SVM289_MFS::Solver
{
public:
  Solver_NU() {}

  void  Solve (int32             l, 
               QMatrix&        Q, 
               const double*   p, 
               const schar*    y,
               double*         alpha, 
               double          Cp, 
               double          Cn, 
               double          eps,
               SolutionInfo*   si, 
               int32             shrinking
              )
  {
    this->si = si;
    Solver::Solve (l, Q, p, y, alpha, Cp, Cn, eps, si, shrinking);
  }

private:
  SolutionInfo*  si;

  int32  select_working_set (int32 &i, int32 &j);

  double calculate_rho ();

  bool  be_shrunk (int32     i, 
                   double  Gmax1, 
                   double  Gmax2, 
                   double  Gmax3, 
                   double  Gmax4
                  );

  void do_shrinking ();
};  /* Solver_NU */



// return 1 if already optimal, return 0 otherwise
int32  SVM289_MFS::Solver_NU::select_working_set (int32&  out_i, 
                                                  int32&  out_j
                                                 )
{
  // return i,j such that y_i = y_j and
  // i: maximizes -y_i * grad(f)_i, i in I_up(\alpha)
  // j: minimizes the decrease of obj value
  //    (if quadratic coefficeint <= 0, replace it with tau)
  //    -y_j*grad(f)_j < -y_i*grad(f)_i, j in I_low(\alpha)

  double  Gmaxp     = -INF;
  double  Gmaxp2    = -INF;
  int32     Gmaxp_idx = -1;

  double Gmaxn     = -INF;
  double Gmaxn2    = -INF;
  int32    Gmaxn_idx = -1;

  int32    Gmin_idx     = -1;
  double obj_diff_min = INF;

  for  (int32 t=0;t<active_size;t++)
  {
    if  (y[t] == +1)
    {
      if  (!is_upper_bound (t))
      {
        if  (-G[t] >= Gmaxp)
        {
          Gmaxp     = -G[t];
          Gmaxp_idx = t;
        }
      }
    }
    else
    {
      if  (!is_lower_bound (t))
      {
        if  (G[t] >= Gmaxn)
        {
          Gmaxn     = G[t];
          Gmaxn_idx = t;
        }
      }
    }
  }


  int32 ip = Gmaxp_idx;
  int32 in = Gmaxn_idx;
  const Qfloat *Q_ip = NULL;
  const Qfloat *Q_in = NULL;
  if(ip != -1) // NULL Q_ip not accessed: Gmaxp=-INF if ip=-1
    Q_ip = Q->get_Q(ip,active_size);
  if(in != -1)
    Q_in = Q->get_Q(in,active_size);

  for  (int32 j = 0;  j < active_size;  j++)
  {
    if(y[j]==+1)
    {
      if (!is_lower_bound(j))  
      {
        double grad_diff=Gmaxp+G[j];
        if (G[j] >= Gmaxp2)
          Gmaxp2 = G[j];
        if (grad_diff > 0)
        {
          double obj_diff; 
          double quad_coef = Q_ip[ip]+QD[j]-2*Q_ip[j];
          if (quad_coef > 0)
            obj_diff = -(grad_diff*grad_diff)/quad_coef;
          else
            obj_diff = -(grad_diff*grad_diff)/TAU;

          if (obj_diff <= obj_diff_min)
          {
            Gmin_idx=j;
            obj_diff_min = obj_diff;
          }
        }
      }
    }
    else
    {
      if (!is_upper_bound(j))
      {
        double grad_diff=Gmaxn-G[j];
        if (-G[j] >= Gmaxn2)
          Gmaxn2 = -G[j];
        if (grad_diff > 0)
        {
          double obj_diff; 
          double quad_coef = Q_in[in]+QD[j]-2*Q_in[j];
          if (quad_coef > 0)
            obj_diff = -(grad_diff*grad_diff)/quad_coef;
          else
            obj_diff = -(grad_diff*grad_diff)/TAU;

          if (obj_diff <= obj_diff_min)
          {
            Gmin_idx=j;
            obj_diff_min = obj_diff;
          }
        }
      }
    }
  }

  if  (Max (Gmaxp + Gmaxp2, Gmaxn + Gmaxn2) < eps)
    return 1;

  if (y[Gmin_idx] == +1)
    out_i = Gmaxp_idx;
  else
    out_i = Gmaxn_idx;

  out_j = Gmin_idx;

  return 0;
}  /* select_working_set */



bool Solver_NU::be_shrunk (int32     i, 
                           double  Gmax1, 
                           double  Gmax2, 
                           double  Gmax3, 
                           double  Gmax4
                          )
{
  if  (is_upper_bound(i))
  {
    if  (y[i]==+1)
      return (-G[i] > Gmax1);
    else  
      return (-G[i] > Gmax4);
  }

  else if  (is_lower_bound(i))
  {
    if  (y[i]==+1)
      return (G[i] > Gmax2);
    else  
      return (G[i] > Gmax3);
  }

  else
  {
    return (false);
  }
}  /* Solver_NU::be_shrunk */



void  SVM289_MFS::Solver_NU::do_shrinking ()
{
  double Gmax1 = -INF;  // Max { -y_i * grad(f)_i | y_i = +1, i in I_up(\alpha) }
  double Gmax2 = -INF;  // Max { y_i * grad(f)_i | y_i = +1, i in I_low(\alpha) }
  double Gmax3 = -INF;  // Max { -y_i * grad(f)_i | y_i = -1, i in I_up(\alpha) }
  double Gmax4 = -INF;  // Max { y_i * grad(f)_i | y_i = -1, i in I_low(\alpha) }

  // find maximal violating pair first
  int32 i;
  for  (i = 0;  i < active_size;  i++)
  {
    if  (!is_upper_bound (i))
    {
      if  (y[i] == +1)
      {
        if  (-G[i] > Gmax1) 
          Gmax1 = -G[i];
      }
      else if(-G[i] > Gmax4) Gmax4 = -G[i];
    }

    if  (!is_lower_bound (i))
    {
      if  (y[i]==+1)
      {  
        if  (G[i] > Gmax2) Gmax2 = G[i];
      }
      else if  (G[i] > Gmax3) 
      {
        Gmax3 = G[i];
      }
    }
  }

  if  (unshrink == false && Max (Gmax1 + Gmax2, Gmax3 + Gmax4) <= eps * 10) 
  {
    unshrink = true;
    reconstruct_gradient();
    active_size = l;
  }

  for(i=0;i<active_size;i++)
  {
    if (be_shrunk (i, Gmax1, Gmax2, Gmax3, Gmax4))
    {
      active_size--;
      while (active_size > i)
      {
        if (!be_shrunk (active_size, Gmax1, Gmax2, Gmax3, Gmax4))
        {
          swap_index (i, active_size);
          break;
        }
        active_size--;
      }
    }
  }
}  /* Solver_NU::do_shrinking */



double  SVM289_MFS::Solver_NU::calculate_rho ()
{
  int32     nr_free1  = 0;
  int32     nr_free2  = 0;
  double  ub1       = INF;
  double  ub2       = INF;
  double  lb1       = -INF;
  double  lb2       = -INF;
  double  sum_free1 = 0;
  double  sum_free2 = 0;

  for  (int32 i = 0;  i < active_size;  i++)
  {
    if(  y[i]==+1)
    {
      if  (is_upper_bound (i))
        lb1 = Max (lb1, G[i]);

      else if  (is_lower_bound (i))
        ub1 = Min (ub1, G[i]);

      else
      {
        ++nr_free1;
        sum_free1 += G[i];
      }
    }
    else
    {
      if  (is_upper_bound (i))
        lb2 = Max (lb2, G[i]);

      else if  (is_lower_bound (i))
        ub2 = Min (ub2, G[i]);

      else
      {
        ++nr_free2;
        sum_free2 += G[i];
      }
    }
  }

  double r1,r2;
  if  (nr_free1 > 0)
    r1 = sum_free1 / nr_free1;
  else
    r1 = (ub1+lb1)/2;
  
  if  (nr_free2 > 0)
    r2 = sum_free2 / nr_free2;
  else
    r2 = (ub2+lb2) / 2;
  
  si->r = (r1+r2) / 2;
  return (r1 - r2) / 2;
}  /* Solver_NU::calculate_rho */



//
// Q matrices for various formulations
//
class  SVM289_MFS::SVC_Q: public SVM289_MFS::Kernel
{ 
public:
  SVC_Q (const svm_problem&    prob, 
         const svm_parameter&  param, 
         const schar*          y_,
         RunLog&               _log
        ):

  Kernel (prob.x, prob.selFeatures, param, _log)

  {
    clone (y, y_, prob.l);
    cache = new Cache (prob.l, (int32)(param.cache_size * (1 << 20)));
    QD = new Qfloat[prob.l];

    for  (int32 i = 0;  i < prob.l;  i++)
      QD[i] = (Qfloat)(this->*kernel_function)(i, i);
  }

  
  Qfloat*  get_Q (int32 i, int32 len) const
  {
    Qfloat *data;
    int32 start, j;
    if  ((start = cache->get_data(i,&data,len)) < len)
    {
      for  (j = start;  j < len;  j++)
        data[j] = (Qfloat)(y[i] * y[j] * (this->*kernel_function)(i, j));
    }
    return data;
  }


  Qfloat*  get_QD() const
  {
    return QD;
  }



  void  swap_index (int32 i, int32 j)
  {
    cache->swap_index(i,j);
    Kernel::swap_index(i,j);
    SVM289_MFS::swap (y[i],y[j]);
    SVM289_MFS::swap (QD[i],QD[j]);
  }


  ~SVC_Q()
  {
    delete[] y;
    delete cache;
    delete[] QD;
  }

private:
  schar*  y;
  Cache*  cache;
  Qfloat* QD;
};  /* SVC_Q */




class  SVM289_MFS::ONE_CLASS_Q: public SVM289_MFS::Kernel
{
public:
  ONE_CLASS_Q (const svm_problem&    prob, 
               const svm_parameter&  param,
               RunLog&               _log
               ):
    Kernel (prob.x, prob.selFeatures, param, _log)

  {
    cache = new Cache (prob.l, (int32)(param.cache_size * (1<<20)));
    QD = new Qfloat[prob.l];
    for  (int32 i = 0;  i < prob.l;  i++)
      QD[i]= (Qfloat)(this->*kernel_function)(i, i);
  }
  

  Qfloat *get_Q (int32 i, int32 len) const
  {
    Qfloat *data;
    int32 start, j;
    if  ((start = cache->get_data(i,&data,len)) < len)
    {
      for  (j=start;  j < len;  j++)
        data[j] = (Qfloat)(this->*kernel_function)(i,j);
    }
    return data;
  }


  Qfloat *get_QD() const
  {
    return QD;
  }


  void swap_index (int32 i, int32 j)
  {
    cache->swap_index(i,j);
    Kernel::swap_index(i,j);
    SVM289_MFS::swap(QD[i],QD[j]);
  }


  ~ONE_CLASS_Q()
  {
    delete cache;
    delete[] QD;
  }

private:
  Cache *cache;
  Qfloat *QD;

};  /* ONE_CLASS_Q */




class  SVM289_MFS::SVR_Q: public SVM289_MFS::Kernel
{ 
public:
  SVR_Q (const svm_problem&    prob, 
         const svm_parameter&  param,
         RunLog&               _log
         ):
    Kernel (prob.x, prob.selFeatures, param, _log)
  {
    l = prob.l;
    cache = new Cache (l, (int32)(param.cache_size * (1 << 20)));
    QD    = new Qfloat[2 * l];
    sign  = new schar [2 * l];
    index = new int32   [2 * l];

    for  (int32 k = 0;  k < l;  k++)
    {
      sign  [k]      = 1;
      sign  [k + l]  = -1;
      index [k]      = k;
      index [k + l]  = k;

      QD[k] = (Qfloat)(this->*kernel_function)(k, k);
      QD[k + l] = QD[k];
    }

    buffer [0] = new Qfloat [2 * l];
    buffer [1] = new Qfloat [2 * l];
    next_buffer = 0;
  }


  void swap_index (int32 i, int32 j)
  {
    SVM289_MFS::swap (sign  [i], sign  [j]);
    SVM289_MFS::swap (index [i], index [j]);
    SVM289_MFS::swap (QD    [i], QD    [j]);
  }
  


  Qfloat *get_Q (int32 i, int32 len) const
  {
    Qfloat *data;
    int32 j, real_i = index[i];

    if  (cache->get_data (real_i, &data, l) < l)
    {
      for  (j = 0;  j < l;  j++)
        data[j] = (Qfloat)(this->*kernel_function)(real_i, j);
    }

    // reorder and copy
    Qfloat *buf = buffer [next_buffer];
    next_buffer = 1 - next_buffer;
    schar si = sign[i];
    for  (j = 0;  j < len;  j++)
      buf[j] = (Qfloat) si * (Qfloat) sign[j] * data[index[j]];
    return buf;
  }



  Qfloat *get_QD () const
  {
    return QD;
  }


  ~SVR_Q()
  {
    delete cache;
    delete[] sign;
    delete[] index;
    delete[] buffer[0];
    delete[] buffer[1];
    delete[] QD;
  }


private:
  int32          l;
  Cache*       cache;
  schar*       sign;
  int32*         index;
  mutable int32  next_buffer;
  Qfloat*      buffer[2];
  Qfloat*      QD;
};   /* SVR_Q */



namespace  SVM289_MFS
{
  void  solve_c_svc (const svm_problem*     prob, 
                     const svm_parameter*   param,
                     double*                alpha, 
                     Solver::SolutionInfo*  si, 
                     double                 Cp, 
                     double                 Cn,
                     RunLog&                _log
                    );

  void  solve_nu_svc (const svm_problem*     prob, 
                      const svm_parameter*   param,
                      double*                alpha, 
                      Solver::SolutionInfo*  si,
                      RunLog&                _log
                     );

  void  solve_epsilon_svr (const svm_problem*    prob, 
                           const svm_parameter*  param,
                           double*               alpha, 
                           Solver::SolutionInfo* si,
                           RunLog&               _log
                          );

  void  solve_one_class (const svm_problem*    prob, 
                         const svm_parameter*  param,
                         double*               alpha, 
                         Solver::SolutionInfo* si,
                         RunLog&               _log
                        );
}



//
// construct and solve various formulations
//
void  SVM289_MFS::solve_c_svc (const svm_problem*     prob, 
                           const svm_parameter*   param,
                           double*                alpha, 
                           Solver::SolutionInfo*  si, 
                           double                 Cp, 
                           double                 Cn,
                           RunLog&                _log
                          )
{
  
  int32      l          = prob->l;
  double*  minus_ones = new double[l];
  schar*   y          = new schar[l];

  int32 i;

  for  (i = 0;  i < l;  i++)
  {
    alpha[i]      = 0;
    minus_ones[i] = -1;
    if  (prob->y[i] > 0) 
      y[i] = +1; 
    else 
      y[i] = -1;
  }

  Solver  s;

  SVC_Q*  jester = new SVC_Q (*prob, *param, y, _log);

  s.Solve (l, 
           *jester, 
           minus_ones, 
           y,
           alpha, 
           Cp, 
           Cn, 
           param->eps, 
           si, 
           param->shrinking
          );
  delete  jester;
  jester = NULL;

  double  sum_alpha =0;

  for  (i = 0;  i < l;  i++)
    sum_alpha += alpha[i];

  //if  (Cp == Cn)
  //  info ("nu = %f\n", sum_alpha / (Cp * prob->l));

  for  (i = 0;  i < l;  i++)
    alpha[i] *= y[i];

  delete[] minus_ones;
  delete[] y;
}  /* solve_c_svc */




void  SVM289_MFS::solve_nu_svc (const svm_problem*     prob, 
                            const svm_parameter*   param,
                            double*                alpha, 
                            Solver::SolutionInfo*  si,
                            RunLog&                _log
                           )
{
  int32 i;
  int32    l  = prob->l;
  double nu = param->nu;

  schar *y = new schar[l];

  for  (i = 0;  i < l;  i++)
  {
    if  (prob->y[i] > 0)
      y[i] = +1;
    else
      y[i] = -1;
  }


  double sum_pos = nu * l / 2;
  double sum_neg = nu * l / 2;

  for  (i = 0;  i < l;  i++)
  {
    if  (y[i] == +1)
    {
      alpha[i] = Min(1.0, sum_pos);
      sum_pos -= alpha[i];
    }
    else
    {
      alpha[i] = Min(1.0,sum_neg);
      sum_neg -= alpha[i];
    }
  }

  double *zeros = new double[l];

  for  (i = 0;  i < l;  i++)
    zeros[i] = 0;

  SVM289_MFS::Solver_NU  s;

  SVC_Q*  jester = new SVC_Q (*prob, *param, y, _log);

  s.Solve (l, 
           *jester, 
           zeros, 
           y,
           alpha, 
           1.0, 
           1.0, 
           param->eps, 
           si,  
           param->shrinking
          );

  delete  jester;
  jester = NULL;

  double r = si->r;

  info ("C = %f\n",1/r);

  for  (i = 0;  i < l;  i++)
    alpha[i] *= y[i] / r;

  si->rho /= r;
  si->obj /= (r * r);
  si->upper_bound_p = 1 / r;
  si->upper_bound_n = 1 / r;

  delete[] y;
  delete[] zeros;
}  /* solve_nu_svc */




void  SVM289_MFS::solve_one_class (const svm_problem*    prob, 
                               const svm_parameter*  param,
                               double*               alpha, 
                               Solver::SolutionInfo* si,
                               RunLog&               _log
                              )
{
  int32 l = prob->l;

  double*   zeros = new double[l];
  schar*    ones  = new schar[l];
  int32 i;

  int32 n = (int32)(param->nu * prob->l);  // # of alpha's at upper bound

  for  (i = 0;  i < n;  i++)
    alpha[i] = 1;

  if  (n < prob->l)
    alpha[n] = param->nu * prob->l - n;

  for  (i = n + 1;  i < l;  i++)
    alpha[i] = 0;

  for  (i = 0;  i < l;  i++)
  {
    zeros[i] = 0;
    ones [i] = 1;
  }

  ONE_CLASS_Q*  jester = new ONE_CLASS_Q (*prob, *param, _log);

  Solver s;
  s.Solve (l, 
           *jester, 
           zeros, 
           ones,
           alpha, 
           1.0, 
           1.0, 
           param->eps, 
           si, 
           param->shrinking
          );

  delete  jester;  
  jester = NULL;

  delete[] zeros;
  delete[] ones;
}  /* solve_one_class */




void  SVM289_MFS::solve_epsilon_svr (const svm_problem*    prob, 
                                 const svm_parameter*  param,
                                 double*               alpha, 
                                 Solver::SolutionInfo* si,
                                 RunLog&               _log
                                )
{
  int32 l = prob->l;
  double*  alpha2       = new double [2 * l];
  double*  linear_term  = new double [2 * l];
  schar*   y = new schar[2*l];
  int32 i;

  for  (i = 0;  i < l;  i++)
  {
    alpha2[i] = 0;
    linear_term[i] = param->p - prob->y[i];
    y[i] = 1;

    alpha2      [i + l] = 0;
    linear_term [i + l] = param->p + prob->y[i];
    y           [i + l] = -1;
  }


  SVR_Q*  jester = new SVR_Q (*prob, *param, _log);
  Solver s;
  s.Solve (2 * l, 
           *jester, 
           linear_term, 
           y,
           alpha2, 
           param->C, 
           param->C, 
           param->eps, 
           si, 
           param->shrinking
          );

  delete  jester;
  jester = NULL;

  double sum_alpha = 0;
  for  (i = 0;  i < l;  i++)
  {
    alpha[i] = alpha2[i] - alpha2[i+l];
    sum_alpha += fabs (alpha[i]);
  }

  info ("nu = %f\n", sum_alpha / (param->C * l));

  delete[] alpha2;
  delete[] linear_term;
  delete[] y;
}  /* solve_epsilon_svr */




static void solve_nu_svr (const svm_problem*    prob, 
                          const svm_parameter*  param,
                          double*               alpha, 
                          Solver::SolutionInfo* si,
                          RunLog&               _log
                         ) 
{
  int32      l = prob->l;
  double   C = param->C;

  double*  alpha2      = new double [2 * l];
  double*  linear_term = new double [2 * l];
  schar*   y           = new schar  [2 * l];
  int32 i;

  double sum = C * param->nu * l / 2;

  for  (i = 0;  i < l;  i++)
  {
    alpha2[i] = alpha2[i + l] = Min (sum, C);
    sum -= alpha2[i];

    linear_term[i] = - prob->y[i];
    y          [i] = 1;

    linear_term[i + l] = prob->y[i];
    y          [i + l] = -1;
  }


  SVR_Q*  jester = new SVR_Q (*prob, *param, _log);
  Solver_NU  s;
  s.Solve (2 * l, 
           *jester, 
           linear_term, 
           y,
           alpha2, 
           C, 
           C, 
           param->eps, 
           si, 
           param->shrinking
          );
  delete  jester;
  jester = NULL;

  info ("epsilon = %f\n", -si->r);

  for  (i = 0;  i < l;  i++)
    alpha[i] = alpha2[i] - alpha2[i + l];

  delete[] alpha2;
  delete[] linear_term;
  delete[] y;
}  /* solve_nu_svr */




//
// decision_function
//
struct SVM289_MFS::decision_function
{
  double *alpha;
  double rho;  
};





decision_function  SVM289_MFS::svm_train_one (const svm_problem&    prob, 
                                          const svm_parameter&  param,
                                          double                Cp, 
                                          double                Cn,
                                          RunLog&               _log
                                         )
{
  double*  alpha = new double [prob.l];
  Solver::SolutionInfo  si;

  switch  (param.svm_type)
  {
    case C_SVC:
      solve_c_svc (&prob, &param, alpha, &si, Cp, Cn, _log);
      break;

    case NU_SVC:
      solve_nu_svc (&prob, &param, alpha, &si, _log);
      break;

    case ONE_CLASS:
      solve_one_class (&prob, &param, alpha, &si, _log);
      break;

    case EPSILON_SVR:
      solve_epsilon_svr (&prob, &param, alpha, &si, _log);
      break;

    case NU_SVR:
      solve_nu_svr (&prob, &param, alpha, &si, _log);
      break;

    default:
      {
        KKStr errMsg = "SVM289_MFS::svm_train_one   ***ERROR***   Invalid Solver Defined.";
        errMsg << "   Solver[" << param.svm_type << "]";
        _log.Level (-1) << endl << endl << errMsg << endl << endl;
        throw KKStrException (errMsg);
      }
  }

  //info ("obj = %f, rho = %f\n", si.obj, si.rho);

  // output SVs

  std::vector<int32> SVIndex;     // Normalize by Margin Width(NMW).

  int32 nSV   = 0;
  int32 nBSV  = 0;
  for  (int32 i = 0;  i < prob.l;  i++)
  {
    if  (fabs (alpha[i]) > 0)
    {
      ++nSV;
      SVIndex.push_back (i);    // NMW
      if  (prob.y[i] > 0)
      {
        if  (fabs (alpha[i]) >= si.upper_bound_p)
        {
          ++nBSV;
          // BSVIndex.insert (prob->index[i]);   // NMW
        }
      }
      else
      {
        if  (fabs (alpha[i]) >= si.upper_bound_n)
        {
          ++nBSV;
          // BSVIndex.insert (prob->index[i]);
        }
      }
    }
  }


  //**********************************************************************************
  //  Code to normalize by margin width.

  
  double sum=0.0;
  std::vector<int32>::iterator it,it2;
  double kvalue = 0.0;

  for (it = SVIndex.begin();  it < SVIndex.end();  it++)
  {
    for  (it2 = SVIndex.begin(); it2 < SVIndex.end();  it2++)
    {
      int32 k  = *it;
      int32 kk = *it2;

      kvalue = Kernel::k_function (prob.x[k], prob.x[kk], param, prob.SelFeatures ());

      sum += prob.y[k] * prob.y[kk] * alpha[k] * alpha[kk] * kvalue;
    }
  }

  sum /= SVIndex.size();
  sum = sqrt(sum);

  for  (it = SVIndex.begin();  it < SVIndex.end();  it++)
    alpha[*it] /= sum;

  si.rho /= sum;
  
  //info ("nSV = %d, nBSV = %d\n", nSV, nBSV);

  decision_function  f;
  f.alpha  = alpha;
  f.rho    = si.rho;
  return  f;
}  /* svm_train_one */






// Platt's binary SVM Probablistic Output: an improvement from Lin et al.
void  SVM289_MFS::sigmoid_train (int32            l, 
                                 const double*  dec_values, 
                                 const double*  labels, 
                                 double&        A,
                                 double&        B
                                )
{
  double  prior1 = 0;
  double  prior0 = 0;
  int32 i;

  for (i=0;i<l;i++)
  {
    if  (labels[i] > 0)
      prior1 += 1;
    else 
      prior0 += 1;
  }

  int32     max_iter  = 100;    // Maximal number of iterations
  double  min_step  = 1e-10;  // Minimal step taken in line search
  double  sigma     = 1e-12;  // For numerically strict PD of Hessian
  double  eps       = 1e-5;
  double  hiTarget  = (prior1 + 1.0) / (prior1 + 2.0);
  double  loTarget  =  1 / (prior0 + 2.0);
  double* t = new double[l];
  double  fApB, p, q, h11, h22, h21, g1, g2, det, dA, dB, gd, stepsize;
  double  newA, newB, newf, d1, d2;
  int32     iter; 
  
  // Initial Point and Initial Fun Value
  A = 0.0; 
  B = log ((prior0 + 1.0) / (prior1 + 1.0));
  double  fval = 0.0;

  for  (i = 0;  i < l;  i++)
  {
    if  (labels[i] > 0) 
      t[i] = hiTarget;
    else 
      t[i] = loTarget;

    fApB = dec_values[i] * A + B;
    
    if (fApB >= 0)
      fval += t[i] * fApB + log (1 + exp (-fApB));
    else
      fval += (t[i] - 1) * fApB + log (1 + exp (fApB));
  }

  for  (iter=0;  iter < max_iter;  iter++)
  {
    // Update Gradient and Hessian (use H' = H + sigma I)
    h11 = sigma; // numerically ensures strict PD
    h22 = sigma;
    h21 = 0.0;
    g1  = 0.0;
    g2  = 0.0;
    for  (i = 0;  i < l;  i++)
    {
      fApB = dec_values[i] * A + B;
      if  (fApB >= 0)
      {
        p = exp (-fApB) / (1.0 + exp(-fApB));
        q = 1.0 / (1.0 + exp(-fApB));
      }
      else
      {
        p = 1.0 / (1.0 + exp (fApB));
        q = exp (fApB) / (1.0 + exp (fApB));
      }

      d2   = p * q;
      h11  += dec_values[i] * dec_values[i] * d2;
      h22  += d2;
      h21  += dec_values[i] * d2;
      d1   = t[i] - p;
      g1   += dec_values[i] * d1;
      g2   += d1;
    }

    // Stopping Criteria
    if  ((fabs (g1) < eps)  &&  (fabs(g2) < eps))
      break;

    // Finding Newton direction: -inv(H') * g
    det = h11 * h22  -  h21 * h21;
    dA  = -(h22*g1 - h21 * g2) / det;
    dB  = -(-h21 * g1  +  h11 * g2) / det;
    gd  = g1 * dA  +  g2 * dB;


    stepsize = 1;    // Line Search
    while (stepsize >= min_step)
    {
      newA = A + stepsize * dA;
      newB = B + stepsize * dB;

      // New function value
      newf = 0.0;
      for (i = 0;  i < l;  i++)
      {
        fApB = dec_values[i] * newA + newB;
        if (fApB >= 0)
          newf += t[i] * fApB + log (1 + exp (-fApB));
        else
          newf += (t[i] - 1) * fApB + log (1 + exp (fApB));
      }
      // Check sufficient decrease
      if (newf < fval + 0.0001 * stepsize * gd)
      {
        A    = newA;
        B    = newB;
        fval = newf;
        break;
      }
      else
        stepsize = stepsize / 2.0;
    }

    if (stepsize < min_step)
    {
      info("Line search fails in two-class probability estimates\n");
      break;
    }
  }

  if  (iter >= max_iter)
    info ("Reaching maximal iterations in two-class probability estimates\n");

  delete  t;  t = NULL;
}  /* sigmoid_train */



double  SVM289_MFS::sigmoid_predict (double  decision_value, 
                                     double  A, 
                                     double  B
                                    )
{
  double  fApB = decision_value * A + B;
  if  (fApB >= 0)
    return exp (-fApB) / (1.0 + exp (-fApB));
  else
    return 1.0 / (1 + exp (fApB));
}  /* sigmoid_predict */



// Method 2 from the multiclass_prob paper by Wu, Lin, and Weng
void  SVM289_MFS::multiclass_probability (int32       k, /**< Number of Classes.      */
                                          double**  r, /**< Pairwise Probabilites.  */
                                          double*   p  /**< Class Probability       */
                                         )
{
  int32 t,j;
  int32  iter     = 0;
  int32  max_iter = Max (100, k);

  double**  Q  = new double*[k];
  double*   Qp = new double[k];
  double    pQp; 
  double    eps = 0.005 / k;
  
  for  (t = 0;  t < k;  t++)
  {
    p[t]    = 1.0 / k;  // Valid if k = 1
    Q[t]    = new double[k];
    Q[t][t] = 0;
    for  (j = 0;  j < t;  j++)
    {
      Q[t][t] += r[j][t] * r[j][t];
      Q[t][j] =  Q[j][t];
    }

    for  (j = t + 1;  j < k;  j++)
    {
      Q[t][t] +=  r[j][t] * r[j][t];
      Q[t][j] =-  r[j][t] * r[t][j];
    }
  }

  for (iter = 0;  iter <max_iter;  iter++)
  {
    // stopping condition, recalculate QP,pQP for numerical accuracy
    pQp = 0;
    for (t = 0;  t < k;  t++)
    {
      Qp[t] = 0;
      for  (j = 0;  j < k;  j++)
        Qp[t] += Q[t][j] * p[j];
      pQp += p[t] * Qp[t];
    }
    double max_error = 0;
    for (t = 0;  t < k;  t++)
    {
      double error = fabs (Qp[t] - pQp);
      if  (error > max_error)
        max_error = error;
    }

    if  (max_error < eps) 
      break;
    
    for  (t = 0;  t < k;  t++)
    {
      double diff = (-Qp[t] +pQp) / Q[t][t];
      p[t] += diff;
      pQp = (pQp + diff * (diff * Q[t][t] + 2 * Qp[t]))  /  (1 + diff) / (1 + diff);
      for (j = 0;  j < k;  j++)
      {
        Qp[j] = (Qp[j] + diff * Q[t][j]) / (1 + diff);
        p[j] /= (1 + diff);
      }
    }
  }
  if  (iter >= max_iter)
    info ("Exceeds max_iter in multiclass_prob\n");

  for  (t = 0;  t < k;  t++)
  {delete Q[t];  Q[t] = NULL;}

  delete  Q;  Q  = NULL;
  delete  Qp; Qp = NULL;
}  /* multiclass_probability */




// Cross-validation decision values for probability estimates
void  svm_binary_svc_probability (const svm_problem    *prob, 
                                  const svm_parameter  *param,
                                  double               Cp, 
                                  double               Cn, 
                                  double&              probA, 
                                  double&              probB,
                                  RunLog&              log
                                 )
{
  int32 i;
  int32  nr_fold = 5;
  int32 *perm = new int32[prob->l];

  FeatureVectorPtr*  subX    = NULL;
  svm_problem*       subProb = NULL;

  double *dec_values = new double[prob->l];

  // random shuffle
  for  (i = 0;  i < prob->l;  i++) 
    perm[i]=i;
  
  for  (i = 0;  i < prob->l;  i++)
  {
    int32 j = i + rand() % (prob->l-i);
    SVM289_MFS::swap (perm[i], perm[j]);
  }
    
  for  (i = 0;  i < nr_fold;  i++)
  {
    int32 begin = i * prob->l / nr_fold;
    int32 end   = (i + 1) * prob->l / nr_fold;
    int32 j, k;

    int32  subL = prob->l - (end - begin);
    subX = new FeatureVectorPtr[subL];
    for  (j = 0;  j < subL;  j++)
      subX[j] = NULL;
    float*  subY = new float[subL];

    k = 0;
    for  (j = 0;  j < begin;  j++)
    {
      subX[k] = prob->x.IdxToPtr (perm[j]);
      subY[k] = (float)prob->y[perm[j]];
      ++k;
    }

    for  (j = end;  j < prob->l;  j++)
    {
      subX[k] = prob->x.IdxToPtr (perm[j]);
      subY[k] = (float)prob->y[perm[j]];
      ++k;
    }

    {
      FeatureVectorListPtr  subXX = new FeatureVectorList (prob->x.FileDesc (), false, log, subL);
      for  (j = 0;  j < k;  j++)
        subXX->PushOnBack (subX[j]);
      subProb = new svm_problem (*subXX, subY, prob->selFeatures);
      delete  subXX;
    }

    int32  p_count=0, n_count = 0;

    for  (j = 0;  j < k;  j++)
    {
      if  (subY[j] > 0)
        p_count++;
      else
        n_count++;
    }


    if  ((p_count == 0)  &&  (n_count == 0))
    {
      for  (j = begin;  j < end;  j++)
        dec_values[perm[j]] = 0;
    }

    else if  ((p_count > 0) && (n_count == 0))
    {
      for  (j = begin;  j < end;  j++)
        dec_values[perm[j]] = 1;
    }

    else if  ((p_count == 0)  &&  (n_count > 0))
    {
      for  (j = begin;  j < end;  j++)
        dec_values[perm[j]] = -1;
    }
    
    else
    {
      svm_parameter subparam = *param;
      subparam.probability=0;
      subparam.C=1.0;
      subparam.nr_weight=2;
      subparam.weight_label = new int32[2];
      subparam.weight = new double[2];
      subparam.weight_label[0]=+1;
      subparam.weight_label[1]=-1;
      subparam.weight[0]=Cp;
      subparam.weight[1]=Cn;
      svm_model* submodel = svm_train (*subProb, subparam, log);

      for  (j = begin;  j < end;  j++)
      {
        svm_predict_values (submodel, prob->x[perm[j]], &(dec_values[perm[j]])); 
        // ensure +1 -1 order; reason not using CV subroutine
        dec_values[perm[j]] *= submodel->label[0];
      }    

      delete  submodel;  submodel = NULL;
      //svm_destroy_param (&subparam);
    }

    delete  subProb;  subProb = NULL;
    delete  subX;     subX    = NULL;
    delete  subY;     subY    = NULL;
  }    

  sigmoid_train (prob->l, dec_values, prob->y, probA, probB);
  delete  dec_values;  dec_values = NULL;
  delete  perm;        perm       = NULL;
}  /* svm_binary_svc_probability */







// Return parameter of a Laplace distribution 
double  svm_svr_probability (const svm_problem&   prob, 
                             const svm_parameter& param,
                             RunLog&              log
                            )
{
  int32 i;
  int32 nr_fold = 5;
  double *ymv = new double[prob.l];
  double mae = 0;

  svm_parameter  newparam = param;
  newparam.probability = 0;
  svm_cross_validation (prob, newparam, nr_fold, ymv, log);

  for  (i = 0;  i < prob.l;  i++)
  {
    ymv[i] = prob.y[i] - ymv[i];
    mae += fabs (ymv[i]);
  }    

  mae /= prob.l;
  double  std = sqrt (2 * mae * mae);
  int32 count = 0;
  mae = 0;
  for  (i = 0;  i < prob.l;  i++)
  {
    if  (fabs(ymv[i]) > (5 * std)) 
      count = count + 1;
    else 
      mae += fabs (ymv[i]);
  }

  mae /= (prob.l - count);
  
  info("Prob. model for test data: target value = predicted value + z,\nz: Laplace distribution e^(-|z|/sigma)/(2sigma),sigma= %g\n", mae);
  delete  ymv;  ymv = NULL;
  return mae;
}  /* svm_svr_probability */





// label: label name, start: begin of each class, count: #data of classes, perm: indices to the original data
// perm, length l, must be allocated before calling this subroutine
void  svm_group_classes (const svm_problem*  prob, 
                         int32*                nr_class_ret, 
                         int32**               label_ret, 
                         int32**               start_ret, 
                         int32**               count_ret, 
                         int32*                perm
                        )
{
  int32 l = prob->l;
  int32 max_nr_class = 16;
  int32 nr_class = 0;
  int32 *label      = new int32[max_nr_class];
  int32 *count      = new int32[max_nr_class];
  int32 *data_label = new int32[l];
  int32 i;


  // Count number of examples in each class
  for  (i = 0;  i < l;  i++)
  {
    int32 this_label = (int32)prob->y[i];
    int32 j;
    for  (j = 0;  j < nr_class;  j++)
    {
      if  (this_label == label[j])
      {
        ++count[j];
        break;
      }
    }

    data_label[i] = j;
    if  (j == nr_class)
    {
      if  (nr_class == max_nr_class)
      {
        max_nr_class *= 2;
        label = (int32 *)realloc (label, max_nr_class * sizeof(int32));
        count = (int32 *)realloc (count, max_nr_class * sizeof(int32));
      }
      label[nr_class] = this_label;
      count[nr_class] = 1;
      ++nr_class;
    }
  }

  int32 *start = new int32[nr_class];
  start[0] = 0;
  for  (i = 1;  i < nr_class;  i++)
    start[i] = start[i - 1] + count[i - 1];

  for  (i = 0;  i < l;  i++)
  {
    perm[start[data_label[i]]] = i;
    ++start[data_label[i]];
  }

  start[0] = 0;
  for  (i = 1;  i < nr_class;  i++)
    start[i] = start[i - 1] + count[i - 1];

  *nr_class_ret = nr_class;
  *label_ret = label;
  *start_ret = start;
  *count_ret = count;
  delete  data_label;  data_label = NULL;
}  /* svm_group_classes*/




//
// Interface functions
//
svm_model*  SVM289_MFS::svm_train  (const svm_problem&     prob, 
                                const svm_parameter&   param,
                                RunLog&                log
                               )
{
  svm_model*  model = new svm_model (param, prob.SelFeatures (), prob.FileDesc (), log);

  model->weOwnSupportVectors = false;  // XXX

  if  ((param.svm_type == ONE_CLASS)    ||
       (param.svm_type == EPSILON_SVR)  ||
       (param.svm_type == NU_SVR)
      )
  {
    // regression or one-class-svm
    model->nr_class = 2;
    model->label    = NULL;
    model->nSV      = NULL;
    model->probA    = NULL; 
    model->probB    = NULL;
    model->sv_coef  = new double*[1];

    if  (param.probability  &&  (param.svm_type == EPSILON_SVR  ||  param.svm_type == NU_SVR))
    {
      model->probA = new double[1];
      model->probA[0] = svm_svr_probability (prob, param, log);
    }

    decision_function f = svm_train_one (prob, param, 0, 0, log);
    model->rho = new double[1];
    model->rho[0] = f.rho;

    int32 nSV = 0;
    int32 i;
    for  (i = 0;  i < prob.l;  i++)
    {
      if  (fabs(f.alpha[i]) > 0) 
        ++nSV;
    }

    model->l = nSV;
    //model->SV = Malloc(svm_node *,nSV);
    // model->SV is now a FeatureVectorList object that was initialized to empty and not owner in the consructor
    model->SV.Owner (true);
    model->sv_coef[0] = new double[nSV];
    int32 j = 0;
    for  (i = 0;  i < prob.l;  i++)
    {
      if  (fabs (f.alpha[i]) > 0)
      {
        //model->SV[j] = prob->x[i];
        model->SV.PushOnBack (new FeatureVector (prob.x[i]));
        model->sv_coef[0][j] = f.alpha[i];
        ++j;
      }    
    }

    delete  f.alpha;  f.alpha = NULL;
  }
  else
  {
    // Classification
    int32 l = prob.l;
    int32 nr_class;
    int32 *label = NULL;
    int32 *start = NULL;
    int32 *count = NULL;
    int32 *perm = new int32[l];

    // group training data of the same class
    svm_group_classes (&prob, 
                       &nr_class,
                       &label,
                       &start,
                       &count,
                       perm
                      );

    int32  numBinaryCombos = nr_class * (nr_class - 1) / 2;

    //svm_node **x = Malloc(svm_node *,l);
    FeatureVectorList x (prob.FileDesc (), false, log, prob.x.QueueSize ());

    int32 i;
    for  (i = 0;  i < l;  i++)
    {
      //x[i] = prob->x[perm[i]];
      x.PushOnBack (prob.x.IdxToPtr (perm[i]));
    }

    // calculate weighted C
    double*  weighted_C = new double[nr_class];
    for  (i = 0;  i < nr_class;  i++)
      weighted_C[i] = param.C;

    for  (i = 0;  i < param.nr_weight;  i++)
    {  
      int32 j;
      for  (j = 0;  j < nr_class;  j++)
      {
        if  (param.weight_label[i] == label[j])
          break;
      }

      if  (j == nr_class)
        fprintf(stderr,"warning: class label %d specified in weight is not found\n", param.weight_label[i]);
      else
        weighted_C[j] *= param.weight[i];
    }

    // train k*(k-1)/2 models
    
    bool *nonzero = new bool[l];

    for  (i = 0;  i < l;  i++)
      nonzero[i] = false;

    decision_function *f = new decision_function[numBinaryCombos];

    double*  probA = NULL;
    double*  probB = NULL;

    if (param.probability)
    {
      probA = new double[numBinaryCombos];
      probB = new double[numBinaryCombos];
    }

    int32 p = 0;
    for  (i = 0;  i < nr_class;  i++)
    {
      for  (int32 j = i + 1;  j < nr_class;  j++)
      {
        svm_problem  sub_prob (prob.SelFeatures (), log);
        int32 si = start[i], sj = start[j];
        int32 ci = count[i], cj = count[j];
        sub_prob.l = ci + cj;
        //sub_prob.x = Malloc (svm_node *,sub_prob.l);
        sub_prob.y = new double[sub_prob.l];
        int32 k;
        for  (k = 0;  k < ci;  k++)
        {
          //sub_prob.x[k] = x[si+k];
          sub_prob.x.PushOnBack (x.IdxToPtr (si + k));
          sub_prob.y[k] = +1;
        }
        for  (k = 0;  k < cj;  k++)
        {
          //sub_prob.x[ci+k] = x[sj+k];
          sub_prob.x.PushOnBack (x.IdxToPtr (sj + k));
          sub_prob.y[ci + k] = -1;
        }

        if  (param.probability)
          svm_binary_svc_probability (&sub_prob, &param, weighted_C[i], weighted_C[j], probA[p], probB[p], log);


        f[p] = svm_train_one (sub_prob, param, weighted_C[i], weighted_C[j], log);

        for  (k = 0;  k < ci;  k++)
        {
          if  (!nonzero[si + k]  &&  fabs(f[p].alpha[k]) > 0)
            nonzero[si + k] = true;
        }

        for  (k = 0;  k < cj;  k++)
        {
          if  (!nonzero[sj + k]  &&  fabs(f[p].alpha[ci+k]) > 0)
            nonzero[sj + k] = true;
        }

        //free(sub_prob.x);
        delete  sub_prob.y;  
        sub_prob.y = NULL;
        ++p;
      }
    }


    // At this point all the Binary Classifiers have been built.  They are now going 
    // to be packaged into one not so neat model.

    // build output
    model->nr_class = nr_class;
    
    model->label = new int32[nr_class];
    for  (i = 0;  i < nr_class;  i++)
      model->label[i] = label[i];
    
    model->rho = new double[numBinaryCombos];
    for  (i = 0;  i < numBinaryCombos;  i++)
      model->rho[i] = f[i].rho;

    if  (param.probability)
    {
      model->probA = new double[numBinaryCombos];
      model->probB = new double[numBinaryCombos];
      for  (i = 0;  i < numBinaryCombos;  i++)
      {
        model->probA[i] = probA[i];
        model->probB[i] = probB[i];
      }
    }
    else
    {
      model->probA = NULL;
      model->probB = NULL;
    }

    int32 total_sv = 0;
    int32*  nz_count = new int32[nr_class];

    model->nSV = new int32[nr_class];
    for  (i = 0;  i < nr_class;  i++)
    {
      int32 nSV = 0;
      for  (int32 j = 0;  j < count[i];  j++)
      {
        if  (nonzero[start[i] + j])
        {  
          ++nSV;
          ++total_sv;
        }
      }
      model->nSV[i] = nSV;
      nz_count[i]   = nSV;
    }
    
    info("Total nSV = %d\n",total_sv);

    model->l = total_sv;
    //model->SV = Malloc(svm_node *, total_sv);
    model->SV.DeleteContents ();
    model->SV.Owner (false);
    model->weOwnSupportVectors = false;

    p = 0;
    for  (i = 0;  i < l;  i++)
    {
      if  (nonzero[i])
      {
        //model->SV[p++] = x[i];
        model->SV.PushOnBack (x.IdxToPtr (i));
        p++;
      }
    }

    int32 *nz_start = new int32[nr_class];
    nz_start[0] = 0;
    for  (i = 1;  i < nr_class;  i++)
      nz_start[i] = nz_start[i - 1] + nz_count[i - 1];

    model->sv_coef = new double*[nr_class - 1];
    for  (i = 0;  i < nr_class - 1;  i++)
      model->sv_coef[i] = new double[total_sv];

    p = 0;
    for  (i = 0;  i < nr_class;  i++)
    {
      for  (int32 j = i + 1;  j < nr_class;  j++)
      {
        // classifier (i,j): coefficients with
        // i are in sv_coef[j-1][nz_start[i]...],
        // j are in sv_coef[i][nz_start[j]...]

        int32 si = start[i];
        int32 sj = start[j];
        int32 ci = count[i];
        int32 cj = count[j];
        
        int32 q = nz_start[i];
        int32 k;
    
        for  (k = 0;  k < ci;  k++)
        {
          if  (nonzero[si + k])
            model->sv_coef[j - 1][q++] = f[p].alpha[k];
        }

        q = nz_start[j];
        for  (k = 0;  k < cj;  k++)
        {
          if  (nonzero[sj + k])
            model->sv_coef[i][q++] = f[p].alpha[ci + k];
        }
        ++p;
      }
    }

    delete label;  label = NULL;
    delete probA;  probA = NULL;
    delete probB;  probB = NULL;
    delete count;  count = NULL;
    delete perm;   perm  = NULL;
    delete start;  start = NULL;
    //free (x);
    delete weighted_C;  weighted_C = NULL;
    delete nonzero;     nonzero    = NULL;
    for  (i = 0;  i < numBinaryCombos;  i++)
    {
      delete f[i].alpha;
      f[i].alpha = NULL;
    }
    delete f;          f = NULL;
    delete nz_count;   nz_count = NULL;
    delete nz_start;   nz_start = NULL;
  }

  return  model;
}  /* svm_train */





// Stratified cross validation
void  SVM289_MFS::svm_cross_validation (const svm_problem&    prob, 
                                    const svm_parameter&  param, 
                                    int32                   nr_fold, 
                                    double*               target,
                                    RunLog&               log
                                   )
{
  int32 i;
  int32 *fold_start = new int32[nr_fold + 1];
  int32 l = prob.l;
  int32 *perm = new int32[l];
  int32 nr_class;

  // stratified cv may not give leave-one-out rate
  // Each class to l folds -> some folds may have zero elements
  if  ((param.svm_type == C_SVC || param.svm_type == NU_SVC)  && 
       (nr_fold < l)
      )
  {
    int32 *start = NULL;
    int32 *label = NULL;
    int32 *count = NULL;
    svm_group_classes (&prob, &nr_class, &label, &start, &count, perm);

    // random shuffle and then data grouped by fold using the array perm
    int32 *fold_count = new int32[nr_fold];
    int32 c;
    int32 *index = new int32[l];
    for  (i = 0;  i < l;  i++)
      index[i]=perm[i];

    for (c = 0;  c < nr_class;  c++) 
    {
      for  (i = 0;  i < count[c];  i++)
      {
        int32 j = i + rand() % (count[c]-i);
        SVM289_MFS::swap (index[start[c]+j], index[start[c]+i]);
      }
    }

    for  (i = 0;  i < nr_fold;  i++)
    {
      fold_count[i] = 0;
      for  (c = 0;  c < nr_class;  c++)
        fold_count[i] += (i + 1) * count[c] / nr_fold - i * count[c] / nr_fold;
    }

    fold_start[0] = 0;
    for (i = 1;  i <= nr_fold;  i++)
      fold_start[i] = fold_start[i-1] + fold_count[i-1];

    for (c=0; c<nr_class;c++)
    {
      for(i=0;i<nr_fold;i++)
      {
        int32 begin = start[c]+i*count[c]/nr_fold;
        int32 end = start[c]+(i+1)*count[c]/nr_fold;
        for(int32 j=begin;j<end;j++)
        {
          perm[fold_start[i]] = index[j];
          fold_start[i]++;
        }
      }
    }

    fold_start[0]=0;
    for (i=1;i<=nr_fold;i++)
      fold_start[i] = fold_start[i-1]+fold_count[i-1];

    delete start;        start = NULL;
    delete label;       label = NULL;
    delete count;        count = NULL;
    delete index;       index = NULL;
    delete fold_count;  fold_count = NULL;
  }
  else
  {
    for (i = 0;  i < l;  i++) 
      perm[i]=i;

    for (i = 0;  i < l;  i++)
    {
      int32 j = i + rand() % (l - i);
      SVM289_MFS::swap (perm[i], perm[j]);
    }
    for  (i = 0;  i <=  nr_fold; i++)
      fold_start[i] = i * l / nr_fold;
  }

  for  (i = 0;  i < nr_fold;  i++)
  {
    int32 begin = fold_start[i];
    int32 end = fold_start[i+1];
    int32 j,k;

    svm_problem  subprob (prob.SelFeatures (), log);

    subprob.l = l - (end - begin);
    //subprob.x = Malloc(struct svm_node*,subprob.l);
    // subprob.x  will be initilized to an empty FeatureVectorList 
    subprob.y = new double[subprob.l];
      
    k = 0;
    for  (j = 0;  j < begin;  j++)
    {
      //subprob.x[k] = prob->x[perm[j]];
      subprob.x.PushOnBack (prob.x.IdxToPtr (perm[j]));
      subprob.y[k] = prob.y[perm[j]];
      ++k;
    }

    for  (j = end;  j < l;  j++)
    {
      //subprob.x[k] = prob->x[perm[j]];
      subprob.x.PushOnBack (prob.x.IdxToPtr (perm[j]));
      subprob.y[k] = prob.y[perm[j]];
      ++k;
    }

    svm_model*  submodel = svm_train (subprob, param, log);
    if  (param.probability && 
       (param.svm_type == C_SVC || param.svm_type == NU_SVC))
    {
      double *prob_estimates = new double[svm_get_nr_class (submodel)];
      int32    *votes          = new int32   [svm_get_nr_class (submodel)];
      for  (j = begin;  j < end;  j++)
        target[perm[j]] = svm_predict_probability (submodel, prob.x[perm[j]], prob_estimates, votes);
      delete  prob_estimates;
      prob_estimates = NULL;
      delete  votes;
      votes = NULL;
    }
    else
    {
      for  (j = begin;  j < end;  j++)
        target[perm[j]] = svm_predict (submodel, prob.x[perm[j]]);
    }

    svm_destroy_model (submodel);
    delete  submodel;
    submodel = NULL;

    //free(subprob.x);
    delete  subprob.y;   subprob.y = NULL;
  }    

  delete fold_start;  fold_start = NULL;
  delete perm;        perm       = NULL;
}  /* svm_cross_validation */
 






int32 svm_get_svm_type(const svm_model *model)
{
  return model->param.svm_type;
}



int32  SVM289_MFS::svm_get_nr_class(const svm_model *model)
{
  return model->nr_class;
}




void svm_get_labels(const svm_model *model, int32* label)
{
  if (model->label != NULL)
    for(int32 i=0;i<model->nr_class;i++)
      label[i] = model->label[i];
}



double svm_get_svr_probability (const svm_model *model)
{
  if ((model->param.svm_type == EPSILON_SVR || model->param.svm_type == NU_SVR) &&
      model->probA!=NULL)
    return model->probA[0];
  else
  {
    fprintf(stderr,"Model doesn't contain information for SVR probability inference\n");
    return 0;
  }
}




void  SVM289_MFS::svm_predict_values (const svm_model*      model, 
                                  const FeatureVector&  x, 
                                  double*               dec_values
                                 )
{
  if  (model->param.svm_type == ONE_CLASS    ||
       model->param.svm_type == EPSILON_SVR  ||
       model->param.svm_type == NU_SVR
      )
  {
    double *sv_coef = model->sv_coef[0];
    double sum = 0;
    for  (int32 i = 0;  i < model->l;  i++)
      sum += sv_coef[i] * Kernel::k_function (x, 
                                              model->SV[i], 
                                              model->param, 
                                              model->selFeatures
                                             );
    sum -= model->rho[0];
    *dec_values = sum;
  }
  else
  {
    int32 i;
    int32 nr_class = model->nr_class;
    int32 l = model->l;
    
    double *kvalue = new double[l];
    for  (i = 0;  i < l;  i++)
      kvalue[i] = Kernel::k_function (x, model->SV[i], model->param, model->selFeatures);

    int32 *start = new int32[nr_class];
    start[0] = 0;
    for  (i = 1;  i < nr_class;  i++)
      start[i] = start[i-1]+model->nSV[i-1];

    int32  p=0;
    for  (i = 0;  i < nr_class;  i++)
    {
      for  (int32 j = i + 1;  j < nr_class;  j++)
      {
        double sum = 0;
        int32 si = start[i];
        int32 sj = start[j];
        int32 ci = model->nSV[i];
        int32 cj = model->nSV[j];
        
        int32 k;
        double *coef1 = model->sv_coef[j - 1];
        double *coef2 = model->sv_coef[i];
        for  (k = 0;  k < ci;  k++)
          sum += coef1[si + k] * kvalue[si + k];


        for  (k = 0;  k < cj;  k++)
          sum += coef2[sj + k] * kvalue[sj + k];

        sum -= model->rho[p];
        dec_values[p] = sum;
        p++;
      }
    }

    delete  kvalue;   kvalue = NULL;
    delete  start;    start  = NULL;
  }
}  /* svm_predict_values */







double SVM289_MFS::svm_predict (const svm_model*      model, 
                                const FeatureVector&  x
                               )
{
  if  ((model->param.svm_type == ONE_CLASS)     ||
       (model->param.svm_type == EPSILON_SVR)   ||
       (model->param.svm_type == NU_SVR)
      )
  {
    double res;
    svm_predict_values (model, x, &res);
    
    if  (model->param.svm_type == ONE_CLASS)
      return (res > 0) ? 1:-1;
    else
      return res;
  }
  else
  {
    int32 i;
    int32 nr_class = model->nr_class;
    double *dec_values = new double[nr_class * (nr_class - 1) / 2];
    svm_predict_values (model, x, dec_values);

    int32 *vote = new int32[nr_class];
    for  (i = 0;  i < nr_class;  i++)
      vote[i] = 0;

    int32 pos = 0;
    for  (i = 0;  i < nr_class;  i++)
    {
      for  (int32 j = i + 1;  j < nr_class;  j++)
      {
        if  (dec_values[pos++] > 0)
          ++vote[i];
        else
          ++vote[j];
      }
    }

    int32 vote_max_idx = 0;
    for(i=1;i<nr_class;i++)
    {
      if  (vote[i] > vote[vote_max_idx])
        vote_max_idx = i;
    }

    delete  vote;        vote = NULL;
    delete  dec_values;  dec_values = NULL;

    return model->label[vote_max_idx];
  }
}  /* svm_predict */





double  SVM289_MFS::svm_predict_probability (svm_model*             model, 
                                             const FeatureVector&   x, 
                                             double*                classProbabilities,
                                             int32*                 votes
                                            )
{
  double  probParam = model->param.probParam;

  if  ((model->param.svm_type == C_SVC  ||  model->param.svm_type == NU_SVC)  &&  
       ((model->probA != NULL  &&  model->probB != NULL)  ||  (probParam > 0.0))
      )
  {
    int32     i;
    int32     nr_class = model->nr_class;

    double*  prob_estimates = model->ProbEstimates ();
    double*  dec_values     = model->DecValues     ();
    double** pairwise_prob  = model->PairwiseProb  ();

    for  (i = 0;  i < nr_class;  i++)
      votes[i] = 0;

    svm_predict_values (model, x, dec_values);

    double min_prob = 1e-7;

    int32 k=0;
    for (i = 0;  i < nr_class;  i++)
    {
      for (int32 j = i + 1;  j < nr_class;  j++)
      {
        if  (probParam > 0.0)
        {
          double probability = (double)(1.0 / (1.0 + exp (-1.0 * probParam * dec_values[k])));
          pairwise_prob[i][j] = Min (Max (probability, min_prob), 1 - min_prob);
          pairwise_prob[j][i] = 1 - pairwise_prob[i][j];
        }
        else
        {
          pairwise_prob[i][j] = Min (Max (sigmoid_predict (dec_values[k], model->probA[k], model->probB[k]), min_prob), 1 - min_prob);
          pairwise_prob[j][i] = 1 - pairwise_prob[i][j];
        }

        if  (pairwise_prob[i][j] > 0.5)
          votes[model->label[i]]++;
        else
          votes[model->label[j]]++;

        k++;
      }
    }

    // The 'pairwise_prob' and 'prob_estimates' variables are actually located
    // in 'model'.  So by calling 'NormalizeProbability'  we normalize 
    // 'prob_estimates'.
    model->NormalizeProbability (); 

    //multiclass_probability (nr_class, pairwise_prob, prob_estimates);

    int32 prob_max_idx = 0;
    for (i = 1;  i < nr_class;  i++)
    {
      if  (prob_estimates[i] > prob_estimates[prob_max_idx])
        prob_max_idx = i;
    }

    for (i = 0;  i < nr_class;  i++)
      classProbabilities[model->label[i]] = prob_estimates[i];

    return  model->label[prob_max_idx];
  }
  else 
  {
    return  svm_predict (model, x);
  }
}  /* svm_predict_probability */








int32  SVM289_MFS::svm_save_model (const char*      model_file_name, 
                             const svm_model* model
                            )
{
  FILE *fp = osFOPEN (model_file_name,"w");
  if  (fp == NULL) 
    return -1;

  const svm_parameter& param = model->param;

  fprintf (fp, "svm_type %s\n",    SVM_Type_ToStr (param.svm_type).Str ());
  fprintf (fp, "kernel_type %s\n", Kernel_Type_ToStr (param.kernel_type).Str ());

  if  (param.kernel_type == POLY)
    fprintf(fp,"degree %d\n", param.degree);

  if  (param.kernel_type == POLY || param.kernel_type == RBF || param.kernel_type == SIGMOID)
    fprintf(fp,"gamma %g\n", param.gamma);

  if  (param.kernel_type == POLY || param.kernel_type == SIGMOID)
    fprintf(fp,"coef0 %g\n", param.coef0);

  int32 nr_class = model->nr_class;
  int32 l = model->l;
  fprintf (fp, "nr_class %d\n", nr_class);
  fprintf (fp, "total_sv %d\n", l);
  
  {
    fprintf(fp, "rho");
    for  (int32 i = 0;  i < nr_class * (nr_class - 1) / 2;  i++)
      fprintf(fp," %g",model->rho[i]);
    fprintf(fp, "\n");
  }
  
  if  (model->label)
  {
    fprintf(fp, "label");
    for  (int32 i = 0;  i < nr_class;  i++)
      fprintf (fp," %d",  model->label[i]);
    fprintf (fp, "\n");
  }

  if  (model->probA) // regression has probA only
  {
    fprintf(fp, "probA");
    for(int32 i=0;i<nr_class*(nr_class-1)/2;i++)
      fprintf(fp," %g",model->probA[i]);
    fprintf(fp, "\n");
  }
  if  (model->probB)
  {
    fprintf(fp, "probB");
    for(int32 i=0;i<nr_class*(nr_class-1)/2;i++)
      fprintf(fp," %g",model->probB[i]);
    fprintf(fp, "\n");
  }

  if  (model->nSV)
  {
    fprintf (fp, "nr_sv");
    for  (int32 i = 0;  i < nr_class;  i++)
      fprintf(fp," %d", model->nSV[i]);
    fprintf(fp, "\n");
  }


  fprintf(fp, "SV\n");
  const double * const *sv_coef = model->sv_coef;

  //const svm_node * const *SV = model->SV;
  const FeatureVectorList&  SV = model->SV;
  FeatureNumList  selFeatures = model->selFeatures;

  for  (int32 i = 0;  i < l;  i++)
  {
    for  (int32 j = 0;  j < nr_class-1;  j++)
      fprintf (fp, "%.16g ", sv_coef[j][i]);

    //const svm_node *p = SV[i];
    const FeatureVector&  p = SV[i];

    if  (param.kernel_type == PRECOMPUTED)
    {
      //fprintf(fp,"0:%d ",(int32)(p->value));
      fprintf (fp, "0:%d ", (int32)(p.FeatureData (0)));
    }
    else
    {
      int32  zed = 0;  
      for  (zed = 0;  zed < p.NumOfFeatures ();  zed++)
        fprintf (fp, "%d:%.8g ", zed, p.FeatureData (zed));

      //while (p->index != -1)
      //{
      //  fprintf(fp,"%d:%.8g ", p->index, p->value);
      //  p++;
      //}
    }
    fprintf(fp, "\n");
  }

  if  (ferror(fp) != 0 || fclose(fp) != 0) 
    return -1;
  else 
    return 0;
}  /* svm_save_model */





void  SVM289_MFS::svm_save_model_XML (ostream&          o, 
                                  const svm_model&  model
                                 )
{
  int32  origPrecision = o.precision ();

  int32 nr_class = model.nr_class;
  int32 l = model.l;
  int32  numBinarySVM = nr_class * (nr_class - 1) / 2;

  o << "<LibSvmModel>" << std::endl;

  o << "Version"   << "\t" << LIBSVM_VERSION << std::endl;

  o << "Param"     << "\t" << model.param.ToTabDelStr () << std::endl;
  o << "nr_class"  << "\t" << (model.nr_class)          << std::endl;
  o << "total_sv"  << "\t" << (model.l)                 << std::endl;

  {
    o.precision (12);
    o << "rho";
    for  (int32 i = 0;  i < numBinarySVM;  i++)
      o << "\t" << (model.rho[i]);
    o << std::endl;
    o.precision (origPrecision);
  }

  if  (model.label)
  {
    o << "label";
    for  (int32 i = 0;  i < nr_class;  i++)
      o << "\t" << (model.label[i]);
    o << std::endl;
  }

  if  (model.probA) // regression has probA only
  {
    o.precision (12);
    o << "probA";
    for  (int32 i = 0;  i <  numBinarySVM;  i++)
      o << "\t" << (model.probA[i]);
    o << std::endl;
    o.precision (origPrecision);
  }

  if  (model.probB)
  {
    o.precision (12);
    o << "probB";
    for  (int32 i = 0;  i < numBinarySVM;  i++)
      o << "\t" << (model.probB[i]);
    o << std::endl;
    o.precision (origPrecision);
  }

  {
    o << "SelectedFeatures" << "\t" << model.selFeatures.ToCommaDelStr () << std::endl;
  }

  if  (model.nSV)
  {
    o << "nr_sv";
    for  (int32 i = 0;  i < nr_class;  i++)
      o << "\t" << (model.nSV[i]);
    o << std::endl;
  }

  {
    o << "<SupportVectors>" << std::endl;

    const double * const *sv_coef = model.sv_coef;

    //const svm_node * const *SV = model->SV;
    const FeatureVectorList&  SV = model.SV;
    FeatureNumList  selFeatures = model.selFeatures;

    for  (int32 i = 0;  i < l;  i++)
    {
      //const svm_node *p = SV[i];
      const FeatureVector&  p = SV[i];
      o.precision (13);
      o << "SupportVector" << "\t" << p.ImageFileName ()
                           << "\t" << p.MLClassName ();
      for  (int32 j = 0;  j < nr_class - 1;  j++)
        o << "\t" << sv_coef[j][i];

      if  (model.param.kernel_type == PRECOMPUTED)
      {
        o << "\t" << (int32)(p.FeatureData (0));
      }
      else
      {
        o.precision (10);
        int32  zed = 0;  
        for  (zed = 0;  zed < p.NumOfFeatures ();  zed++)
          o << "\t" << zed << ":" << p.FeatureData (zed);
      }
      o << endl;
    }

    o << "</SupportVectors>" << std::endl;
  }

  o << "</LibSvmModel>" << endl;

  o.precision (origPrecision);
}  /* svm_save_model_XML */






void  SVM289_MFS::svm_load_model_XML_SupportVectorSection (istream&     in,
                                                       FileDescPtr  fileDesc,
                                                       svm_model&   model,
                                                       bool&        valid,
                                                       RunLog&      log
                                                      )
{
  KKStr  fieldName;

  bool  eof = false;
  bool  eol = false;

  int32 nr_class = model.nr_class;
  int32 l = model.l;

  int32  fn = 0;

  float  featureValue;

  KKStr  svCoefStr;
  KKStr  fdStr;
  KKStr  fnStr;
  KKStr  fValueStr;
  KKStr  featureFieldStr;

  valid = true;

  {
    int32  x, y;
    model.sv_coef = new double*[nr_class - 1];
    for  (x = 0;  x < nr_class - 1;  x++)
    {
      model.sv_coef[x] = new double[l];
      for  (y = 0;  y < l;  y++)
        model.sv_coef[x][y] = 0.0;
    }
  }

  model.SV.DeleteContents ();
  model.SV.Owner (true);
  model.weOwnSupportVectors = true;

  int32  numSupportVectorsRead = 0;
  while  (!eof)
  {
    fieldName = osReadNextToken (in, "\t", eof, eol);
    if  (eol  ||  (fieldName.Len () < 1))
      continue;

    if  (fieldName.SubStrPart (0, 1) == "//")
      continue;

    if  (fieldName.EqualIgnoreCase ("</SupportVectors>"))
      break;

    if  (fieldName.EqualIgnoreCase ("SupportVector"))
    {
      if  (numSupportVectorsRead >= l)
      {
        log.Level (-1) << endl << endl
          << "SVM289_MFS::svm_load_model_XML_SupportVectorSection    ***ERROR***"
          << "    Number Support Vectors defined in <SupportVectors> section is greater than specified[" << l << "]" << endl
          << endl;
        valid = false;
        break;
      }

      FeatureVectorPtr fv = new FeatureVector (fileDesc->NumOfFields ());
      KKStr  imageFileName = osReadNextToken (in, "\t", eof, eol);
      KKStr  className     = osReadNextToken (in, "\t", eof, eol);
      fv->ImageFileName (imageFileName);
      fv->MLClass (MLClass::CreateNewMLClass (imageFileName));

      for  (int32 j = 0;  j < nr_class - 1;  j++)
      {
        svCoefStr = osReadNextToken (in, "\t", eof, eol);
        model.sv_coef[j][numSupportVectorsRead] = svCoefStr.ToDouble ();
      }


      if  (model.param.kernel_type == PRECOMPUTED)
      {
        fdStr = osReadNextToken (in, "\t", eof, eol);
        fv->FeatureData (0, (float)fdStr.ToDouble ());
      }
      else
      {
        featureFieldStr = osReadNextToken (in, "\t", eof, eol);
        while  ((!eof)  &&  (!eol))
        {
          fnStr     = featureFieldStr.ExtractToken2 (":");
          fValueStr = featureFieldStr;

          fn = fnStr.ToInt ();
          featureValue = (float)fValueStr.ToDouble ();

          if  ((fn < 0)  ||  (fn >= (int32)fileDesc->NumOfFields ()))
          {
            log.Level (-1) << endl << endl 
              << "SVM289_MFS::svm_load_model_XML_SupportVectorSection   ***ERROR***     Invalid Feature Index[" << fn << "] Specified." << endl
              << endl;
            valid = false;
            break;
          }
          else
          {
            fv->FeatureData (fn, featureValue);
          }

          featureFieldStr = osReadNextToken (in, "\t", eof, eol);
        }
      }
    
      model.SV.PushOnBack (fv);
      numSupportVectorsRead++;
    }
  }

  if  (model.SV.QueueSize () != l)
  {
    log.Level (-1) << endl << endl
      << "SVM289_MFS::svm_load_model_XML_SupportVectorSection   ***ERROR***     Wrong number of Support Vectors Specified." << endl
      << "      Expected [" << l << "]   Found[" << model.SV.QueueSize () << "]" << endl
      << endl;

    valid = false;
  }
}  /* svm_load_model_XML_SupportVectorSection */


    


svm_model*  SVM289_MFS::svm_load_model_XML (istream&     in,
                                        FileDescPtr  fileDesc,
                                        RunLog&      log
                                       )
{
  svm_model*  model = new svm_model (fileDesc, log);

  bool  validModel = true;

  int32  version      = -1;
  int32  nr_class     = -1;
  int32  l            = -1;
  int32  numBinarySVM = -1;

  bool  eof  = false;
  bool  eol  = false;

  KKStr  fieldName;
  KKStr  ln;

  while  (!eof)
  {
    fieldName = osReadNextToken (in, "\t", eof, eol);
    if  (eof)
      break;

    if  (fieldName.Len () < 1)
      continue;

    if  ((fieldName[0] == '/')  &&  (fieldName[1] == '/'))
      continue;

    if  (fieldName.EqualIgnoreCase ("</LibSvmModel>"))
      break;

    if  (fieldName.EqualIgnoreCase ("Version"))
    {
      in >> version;
    }

    else if  (fieldName.EqualIgnoreCase ("Param"))
    {
      ln = osReadRestOfLine (in, eof);
      eol = true;
      model->param.ParseTabDelStr (ln);
    }

    else if  (fieldName.EqualIgnoreCase ("nr_class"))
    {
      ln = osReadNextToken (in, "\t", eof, eol);
      nr_class = ln.ToInt ();
      model->nr_class = nr_class;
      numBinarySVM = nr_class * (nr_class - 1) / 2;
    }

    else if  (fieldName.EqualIgnoreCase ("total_sv"))
    {
      ln = osReadNextToken (in, "\t", eof, eol);
      l = ln.ToInt ();
      model->l = l;
    }

    else if  (fieldName.EqualIgnoreCase ("rho"))
    {
      int32  i = 0;
      model->rho = new double[numBinarySVM];
      KKStr  rStr = osReadNextToken (in, "\t", eof, eol);
      while  ((!eol)  &&  (!eof)  &&  (i < numBinarySVM))
      {
        model->rho[i] = rStr.ToDouble ();
        rStr = osReadNextToken (in, "\t", eof, eol);
        i++;
      }
    }

    else if  (fieldName.EqualIgnoreCase ("label"))
    {
      int32  i = 0;
      model->label = new int32[nr_class];
      KKStr  lStr = osReadNextToken (in, "\t", eof, eol);
      while  ((!eol)  &&  (!eof)  &&  (i < nr_class))
      {
        model->label[i] = lStr.ToInt ();
        lStr = osReadNextToken (in, "\t", eof, eol);
        i++;
      }
    }


    else if  (fieldName.EqualIgnoreCase ("probA"))
    {
      int32  i = 0;
      model->probA = new double[numBinarySVM];
      KKStr  pStr = osReadNextToken (in, "\t", eof, eol);
      while  ((!eol)  &&  (!eof)  &&  (i < numBinarySVM))
      {
        model->probA[i] = pStr.ToDouble ();
        pStr = osReadNextToken (in, "\t", eof, eol);
        i++;
      }
    }

    else if  (fieldName.EqualIgnoreCase ("probB"))
    {
      int32  i = 0;
      model->probB = new double[numBinarySVM];
      KKStr  pStr = osReadNextToken (in, "\t", eof, eol);
      while  ((!eol)  &&  (!eof)  &&  (i < numBinarySVM))
      {
        model->probB[i] = pStr.ToDouble ();
        pStr = osReadNextToken (in, "\t", eof, eol);
        i++;
      }
    }

    else if  (fieldName.EqualIgnoreCase ("SelectedFeatures"))
    {
      bool  validFeatureSelected = false;
      KKStr  selFeaturesStr = osReadNextToken (in, "\t", eof, eol);
      model->selFeatures = FeatureNumList (fileDesc, selFeaturesStr, validFeatureSelected);
    }

    else if  (fieldName.EqualIgnoreCase ("nr_sv"))
    {
      int32  i = 0;
      model->nSV = new int32[nr_class];
      KKStr  nSvStr = osReadNextToken (in, "\t", eof, eol);
      while  ((!eol)  &&  (!eof)  &&  (i < nr_class))
      {
        model->nSV[i] = nSvStr.ToInt ();
        nSvStr = osReadNextToken (in, "\t", eof, eol);
        i++;
      }
    }

    else if  (fieldName.EqualIgnoreCase ("<SupportVectors>"))
    {
      // There will be one line of text for each Support Vector.
      bool  validSupportVectorSection = true;
      SVM289_MFS::svm_load_model_XML_SupportVectorSection (in,
                                                       fileDesc,
                                                       *model,
                                                       validSupportVectorSection,
                                                       log
                                                      );
      if  (!validSupportVectorSection)
        validModel = false;
    }
  }


  if  (version != LIBSVM_VERSION)
  {
    log.Level (-1) << endl << endl << "SVM289_MFS::svm_load_model_XML   ***ERROR***   Model Version[" << version << "]  was found.  Expected[" << LIBSVM_VERSION << "]." << endl;
    validModel = false;
  }


  if  (!validModel)
  {
    delete  model;
    model = NULL;
  }

  return  model;
} /* svm_load_model_XML */



SVM289_MFS::svm_model::svm_model (const svm_model&  _model,
                                  FileDescPtr       _fileDesc,
                                  RunLog&           _log
                                 ):
  param       (_model.param),
  nr_class    (_model.nr_class),
  l           (_model.l),
  SV          (_fileDesc, false, _log, 1000),
  sv_coef     (NULL),
  rho         (NULL),
  probA       (NULL),
  probB       (NULL),
  label       (NULL),
  nSV         (NULL),     // number of SVs for each class (nSV[k])
  weOwnSupportVectors (_model.weOwnSupportVectors),
  selFeatures (_model.selFeatures),
  dec_values     (NULL),
  pairwise_prob  (NULL),
  prob_estimates (NULL)

{
  int32  m = nr_class - 1;
  int32  numBinaryCombos = nr_class * (nr_class - 1) / 2;

  {
    // Copy over support vectors.
    FeatureVectorList::const_iterator idx;
    SV.Owner (weOwnSupportVectors);
    for  (idx = _model.SV.begin ();  idx != _model.SV.end ();  idx++)
    {
      FeatureVectorPtr fv = *idx;
      if  (weOwnSupportVectors)
        SV.push_back (new FeatureVector (*fv));
      else
        SV.push_back (fv);
    }
  }

  if  (_model.sv_coef)
  {
    sv_coef = new double*[m];
    for  (int32 j = 0;  j < m;  j++)
    {
      sv_coef[j] = new double[l];
      for  (int32 i = 0;   i < l;  i++)
       sv_coef[j][i] = _model.sv_coef[j][i];
    }
  }

  if  (_model.rho)
  {
    // Copy over RHO
    rho = new double[numBinaryCombos];
    for  (int32 i = 0;  i < numBinaryCombos;  i++)
      rho[i] = _model.rho[i];
  }

  if  (_model.probA)
  {
    probA = new double[numBinaryCombos];
    for  (int32 i = 0;  i < numBinaryCombos;  i++)
      probA[i] = _model.probA[i];
  }

  if  (_model.probB)
  {
    probB = new double[numBinaryCombos];
    for  (int32 i = 0;  i < numBinaryCombos;  i++)
      probB[i] = _model.probB[i];
  }

  if  (_model.label)
  {
    label = new int32[nr_class];
    for  (int32 i = 0;  i < nr_class;  i++)
      label[i] = _model.label[i];
  }

  if  (_model.nSV)
  {
    nSV = new int32[nr_class];
    for (int32 i = 0;  i < nr_class;  i++)
      nSV[i] = _model.nSV[i];
  }
}



SVM289_MFS::svm_model::svm_model (FileDescPtr   _fileDesc,
                                  RunLog&       _log
                                 ):
   param       (),
   nr_class    (0),
   l           (0),
   SV          (_fileDesc, true, _log, 1000),
   sv_coef     (NULL),
   rho         (NULL),
   probA       (NULL),
   probB       (NULL),
   label       (NULL),
   nSV         (NULL),     // number of SVs for each class (nSV[k])
   weOwnSupportVectors (false),
   selFeatures (_fileDesc),
   dec_values     (NULL),
   pairwise_prob  (NULL),
   prob_estimates (NULL)
{
}


SVM289_MFS::svm_model::svm_model (const svm_parameter&  _param,
                                  const FeatureNumList& _selFeatures,
                                  FileDescPtr           _fileDesc,
                                  RunLog&               _log
                                 ):
   param       (_param),
   nr_class    (0),
   l           (0),
   SV          (_fileDesc, true, _log, 1000),
   sv_coef     (NULL),
   rho         (NULL),
   probA       (NULL),
   probB       (NULL),
   label       (NULL),
   nSV         (NULL),     // number of SVs for each class (nSV[k])
   weOwnSupportVectors (false),
   selFeatures    (_selFeatures),
   dec_values     (NULL),
   pairwise_prob  (NULL),
   prob_estimates (NULL)

{
}




SVM289_MFS::svm_model::svm_model (const KKStr&  _fileName,
                                  FileDescPtr   _fileDesc,
                                  RunLog&       _log
                                 ):
   param       (),
   nr_class    (0),
   l           (0),
   SV          (_fileDesc, true, _log, 1000),
   sv_coef     (NULL),
   rho         (NULL),
   probA       (NULL),
   probB       (NULL),
   label       (NULL),
   nSV         (NULL),     // number of SVs for each class (nSV[k])
   weOwnSupportVectors (false),
   selFeatures    (_fileDesc),
   dec_values     (NULL),
   pairwise_prob  (NULL),
   prob_estimates (NULL)
{
  Load (_fileName, _fileDesc, _log);
}



SVM289_MFS::svm_model::svm_model (istream&     _in,
                                  FileDescPtr  _fileDesc,
                                  RunLog&      _log
                                 ):
   param       (),
   nr_class    (0),
   l           (0),
   SV          (_fileDesc, true, _log, 1000),
   sv_coef     (NULL),
   rho         (NULL),
   probA       (NULL),
   probB       (NULL),
   label       (NULL),
   nSV         (NULL),     // number of SVs for each class (nSV[k])
   weOwnSupportVectors (false),
   selFeatures (_fileDesc),
   dec_values     (NULL),
   pairwise_prob  (NULL),
   prob_estimates (NULL)
{
  Read (_in, _fileDesc, _log);
}




SVM289_MFS::svm_model::~svm_model ()
{
  if  (weOwnSupportVectors)
    SV.Owner (true);
  else
    SV.Owner (false);

  int32  i;

  if  (sv_coef)
  {
    for  (i = 0;  i < (nr_class - 1);  i++)
      delete sv_coef[i];
    delete  sv_coef;
    sv_coef = NULL;
  }

  delete  rho;     rho   = NULL;
  delete  probA;   probA = NULL;
  delete  probB;   probB = NULL;
  delete  label;   label = NULL;
  delete  nSV;     nSV   = NULL;

  if  (pairwise_prob)
  {
    for  (i = 0;  i < nr_class;  i++)
    {
      delete  pairwise_prob[i];
      pairwise_prob[i] = NULL;
    }
    delete  pairwise_prob;
    pairwise_prob = NULL;
  }

  delete  dec_values;
  dec_values = NULL;
  delete  prob_estimates;
  prob_estimates = NULL;
}



int32  SVM289_MFS::svm_model::MemoryConsumedEstimated ()  const
{
  int32  numBinaryClassCombos = nr_class * (nr_class - 1) / 2;
  int32  memoryConsumedEstimated = sizeof (*this) + SV.MemoryConsumedEstimated ();

  if  (sv_coef)         memoryConsumedEstimated += sizeof (double) * (nr_class - 1) * l;    // sv_coef
  if  (rho)             memoryConsumedEstimated += sizeof (double) * numBinaryClassCombos;  // rho
  if  (probA)           memoryConsumedEstimated += sizeof (double) * numBinaryClassCombos;  // probA
  if  (probB)           memoryConsumedEstimated += sizeof (double) * numBinaryClassCombos;  // probB
  if  (label)           memoryConsumedEstimated += sizeof (int32)    * nr_class;
  if  (nSV)             memoryConsumedEstimated += sizeof (int32)    * nr_class;
  if  (pairwise_prob)   memoryConsumedEstimated += sizeof (double*) * nr_class + sizeof (double) * nr_class * nr_class;
  if  (dec_values)      memoryConsumedEstimated += sizeof (double) * numBinaryClassCombos;
  if  (prob_estimates)  memoryConsumedEstimated += sizeof (double) * nr_class;
  return  memoryConsumedEstimated;
}  /* svm_model::MemoryConsumedEstimated */



double*  SVM289_MFS::svm_model::DecValues () 
{
  if  (!dec_values)
    dec_values = new double[nr_class * (nr_class - 1) / 2];
  return  dec_values;
}


double*  SVM289_MFS::svm_model::ProbEstimates () 
{
  if  (!prob_estimates)
    prob_estimates = new double[nr_class];
  return  prob_estimates;
}


double** SVM289_MFS::svm_model::PairwiseProb  () 
{
  if  (!pairwise_prob)
  {
    pairwise_prob = new double*[nr_class];
    for  (int32 x = 0;  x < nr_class;  x++)
      pairwise_prob[x] = new double[nr_class];
  }
  return  pairwise_prob;
}


void  SVM289_MFS::svm_model::Save (const KKStr&  fileName,
                               RunLog&       log
                              )
{
  ofstream  o (fileName.Str ());
  if  (!o.is_open ())
  {
    KKStr  errorMsg = "SVM289_MFS::svm_model::Save   ***ERROR***   Could not open File[" + fileName + "].";
    log.Level (-1) << endl << endl << errorMsg << endl << endl;
    throw  errorMsg;
  }

  Write (o);
  o.close ();
}  /* Save */




void  SVM289_MFS::svm_model::Write (ostream& o)
{
  o << "<Svm_Model>"  << endl;
  o << "svm_type"    << "\t" << SVM_Type_ToStr    (param.svm_type)    << endl;
  o << "kernel_type" << "\t" << Kernel_Type_ToStr (param.kernel_type) << endl;
  
  if  (param.kernel_type == POLY)
    o << "degree" << "\t" << param.degree << endl;

  if  (param.kernel_type == POLY || param.kernel_type == RBF || param.kernel_type == SIGMOID)
    o << "gamma" << "\t" << param.gamma << endl;

  if  (param.kernel_type == POLY || param.kernel_type == SIGMOID)
    o << "coef0" << "\t" << param.coef0 << endl;

  o << "SelFeatures" << "\t" << selFeatures.ToCommaDelStr () << endl;

  o << "nr_class" << "\t" << nr_class << endl;
  int32  numBinaryCombos = nr_class * (nr_class - 1) / 2;

  o << "total_sv" << "\t" << l << endl;
  
  {
    o << "rho";
    for  (int32 i = 0;  numBinaryCombos;  i++)
      o << "\t" << rho[i];
    o << endl;
  }
  
  if  (label)
  {
    o << "label";
    for  (int32 i = 0;  i < nr_class;  i++)
      o << "\t" << label[i];
    o << endl;
  }

  if  (probA) // regression has probA only
  {
    o << "probA";
    for  (int32 i = 0;  i < numBinaryCombos;  i++)
      o << "\t" << probA[i];
    o << endl;
  }

  if  (probB)
  {
    o << "probB";
    for  (int32 i = 0;  i < numBinaryCombos;  i++)
      o << "\t" << probB[i];
    o << endl;
  }

  if  (nSV)
  {
    o << "nr_sv";
    for  (int32 i = 0;  i < nr_class;  i++)
      o << "\t" << nSV[i];
    o << endl;
  }

  for  (int32 i = 0;  i < l;  i++)
  {
    const  FeatureVector&  p = SV[i];
    o << "SupportVector" << "\t" << p.ImageFileName ();

    int32  origPrec = o.precision ();
    o.precision (16);
    for  (int32 j = 0;  j < nr_class - 1;  j++)
    {
      //fprintf (fp, "%.16g ", sv_coef[j][i]);
      o << "\t" << sv_coef[j][i];
    }

    //const svm_node *p = SV[i];
    o.precision (8);

    if  (param.kernel_type == PRECOMPUTED)
    {
      //fprintf(fp,"0:%d ",(int32)(p->value));
      o << "\t" << p.FeatureData (0);
    }
    else
    {
      for  (int32 zed = 0;  zed < p.NumOfFeatures ();  zed++)
        o << "\t" << zed << ":" << p.FeatureData (zed);
    }
    o << endl;
  }

  o << "</Svm_Model>"  << endl;
}  /* Write */




void  SVM289_MFS::svm_model::Load (const KKStr&  fileName,
                               FileDescPtr   fileDesc,
                               RunLog&       log
                              )
{
  ifstream  in (fileName.Str ());
  if  (!in.is_open ())
  {
    KKStr  errorMsg = "SVM289_MFS::svm_model::Load   ***ERROR***   Could not open File[" + fileName + "].";
    log.Level (-1) << endl << endl << errorMsg << endl << endl;
    throw  errorMsg;
  }

  Read (in, fileDesc, log);
  in.close ();
}  /* Load */




void  SVM289_MFS::svm_model::Read (istream&     in, 
                               FileDescPtr  fileDesc,
                               RunLog&      log
                              )
{
  // read parameters
  delete  rho;    rho   = NULL;
  delete  probA;  probA = NULL;
  delete  probB;  probB = NULL;
  delete  label;  label = NULL;
  delete  nSV;    nSV   = NULL;

  SV.DeleteContents ();

  int32  buffLen = 80 * 1024;
  char*  buff = new char[buffLen];

  bool  eof = false;
  bool  eol = false;

  int32  numBinaryCombos = 0;

  while  (!in.eof ())
  {
    in.getline (buff, sizeof (buffLen));

    KKStr line = buff;

    if  (line.SubStrPart (0, 1) == "//")
      continue;

    KKStr fieldName = line.ExtractToken2 ("\t\n\r");
    if  (fieldName.EqualIgnoreCase ("</Svm_Model>"))
      break;

    if  (fieldName.EqualIgnoreCase ("svm_type"))
    {
      param.svm_type = SVM_Type_FromStr (line);
      if  (param.svm_type == SVM_NULL)
      {
        KKStr errorMsg = "SVM289_MFS::svm_model::Read   ***ERROR***  Invalid SVM_Type[" + line + "].";
        log.Level (-1) << endl << errorMsg << endl << endl;
        delete  buff;
        buff = NULL;
        throw  errorMsg;
      }
    }

    else if  (fieldName.EqualIgnoreCase ("kernel_type") == 0)
    {    
      param.kernel_type = Kernel_Type_FromStr (line);
      if  (param.kernel_type == Kernel_NULL)
      {
        KKStr errorMsg = "SVM289_MFS::svm_model::Read   ***ERROR***  Invalid kernel_type[" + line + "].";
        log.Level (-1) << endl << errorMsg << endl << endl;
        delete  buff;  buff = NULL;
        throw  errorMsg;
      }
    }

    else if  (fieldName.EqualIgnoreCase ("degree"))
      param.degree = line.ExtractTokenInt ("\t\n\r");

    else if  (fieldName.EqualIgnoreCase ("gamma"))
      param.gamma = line.ExtractTokenDouble ("\t\n\r");

    else if  (fieldName.EqualIgnoreCase ("coef0"))
      param.coef0 = line.ExtractTokenDouble ("\t\n\r");

    else if  (fieldName.EqualIgnoreCase ("nr_class"))
    {
      nr_class = line.ExtractTokenInt ("\t\n\r");
      numBinaryCombos = nr_class * (nr_class - 1) / 2;
    }

    else if  (fieldName.EqualIgnoreCase ("total_sv"))
      l = line.ExtractTokenInt ("\t\n\r");

    else if  (fieldName.EqualIgnoreCase ("rho"))
    {
      rho = new double[numBinaryCombos];
      for (int32 i = 0;  i < numBinaryCombos;  i++)
        rho[i] = line.ExtractTokenDouble ("\t\n\r");
    }

    else if  (fieldName.EqualIgnoreCase ("label"))
    {
      label = new int32[nr_class];
      for (int32 i=0;  i < nr_class;  i++)
        label[i] = line.ExtractTokenInt ("\t\n\r");
    }

    else if  (fieldName.EqualIgnoreCase ("probA"))
    {
      probA = new double[numBinaryCombos];
      for (int32 i = 0;  i < numBinaryCombos;  i++)
        probA[i] = line.ExtractTokenDouble ("\t\n\r");
    }

    else if  (fieldName.EqualIgnoreCase ("probB"))
    {
      probB = new double[numBinaryCombos];
      for  (int32 i = 0;  i < numBinaryCombos;  i++)
        probB[i] = line.ExtractTokenDouble ("\t\n\r");
    }

    else if  (fieldName.EqualIgnoreCase ("nr_sv"))
    {
      nSV = new int32[nr_class];
      for (int32 i = 0;  i < nr_class;  i++)
        nSV[i] = line.ExtractTokenInt ("\t\n\r");
    }

    else if  (fieldName.EqualIgnoreCase ("SelFeatures"))
    {
      bool  valid = false;
      selFeatures = FeatureNumList (fileDesc, line, valid);
    }


    else if  (fieldName.EqualIgnoreCase ("SupportVector"))
    {
      // read sv_coef and SV

      int32 m = nr_class - 1;
      int32 i, j;

      if  (!sv_coef)
      {
        sv_coef = new double*[m];
        for  (i = 0;  i < m;  i++)
        {
          sv_coef[i] = new double[l];
          for  (j = 0;  j < l;  j++)
            sv_coef[i][j] = 0.0;
        }
      }

      if  (SV.QueueSize () >= l)
      {
        KKStr errorMsg = "SVM289_MFS::svm_model::Read   ***ERROR***  To many Support Vector's Defined.";
        log.Level (-1) << endl << errorMsg << endl << endl;
        delete  buff;
        throw  errorMsg;
      }

      // We are now going to process one line per SV.
      j = 0;
      eof = false;

      KKStr  imageFileName = line.ExtractToken2 ("\t");
      //model->SV[i] = &x_space[j];
      FeatureVectorPtr  fv = new FeatureVector (fileDesc->NumOfFields ());
      fv->ImageFileName (imageFileName);

      for  (j = 0;  (j < (nr_class - 1))  &&  (!eol);  j++)
        sv_coef[j][i] = line.ExtractTokenDouble ("\t");

      if  (param.kernel_type == PRECOMPUTED)
      {
        log.Level (-1) << endl << endl
                       << "SVM289_MFS::svm_model::Read  ***ERROR***    PRECOMPUTED   Can not Handle." << endl
                       << endl;
      }
      else
      {
        for  (uint32 zed = 0;  (zed < fileDesc->NumOfFields ())  &&  (!eol);  zed++)
        {
          KKStr  featureField = line.ExtractToken2 ("\t");
          int32  featureNum   = featureField.ExtractTokenInt (":");
          FFLOAT  featureValue = (FFLOAT)featureField.ExtractTokenDouble ("\t\n\r");
          fv->FeatureData (featureNum, featureValue);
        }
      }
      SV.PushOnBack (fv);
    }
  }

  delete  buff;
  buff = NULL;
  weOwnSupportVectors = true;  // XXX
  SV.Owner (true);
}  /* Read */



/**
 @brief Derining multiclass probability as done in "Recognizing Plankton Images From the SIPPER".
 */
void  SVM289_MFS::svm_model::NormalizeProbability ()
{
  // Make sure that the ProbEstimates array exists.
  ProbEstimates ();

  if  (pairwise_prob == NULL)
    return;

  int32  x = 0;
  int32  y = 0;
  double  totalProb = 0.0;

  for  (x = 0;  x < nr_class;  x++)
  {
    prob_estimates[x] = 1.0;
    for  (y = 0;  y < nr_class;  y++)
    {
      if  (x != y)
        prob_estimates[x] *= pairwise_prob[x][y];
    }

    totalProb += prob_estimates[x];
  }

  for  (x = 0;  x < nr_class;  x++)
    prob_estimates[x] /= totalProb;
}  /* NormalizeProbability */






void  SVM289_MFS::readline (FILE*    input,
                            char*&   buff,
                            int32&   buffLen,
                            bool&    eof
                           )
{
  if  (fgets (buff, buffLen, input) == NULL)
  {
    buff[0] = 0;
    eof = true;
    return;
  }

  while (strrchr (buff, '\n') == NULL)
  {
    int32  newBuffLen = buffLen * 2;
    buff = GrowAllocation (buff, buffLen, newBuffLen);
    buffLen = newBuffLen;
    int32  len = (int32)strlen (buff);
    if  (fgets(buff + len, buffLen - len, input) == NULL)
      break;
  }

  eof = false;
  return;
}  /* readline */







svm_model *svm_load_model (const char*  model_file_name,
                           FileDescPtr  fileDesc,
                           RunLog&      log
                          )
{
  FILE*  fp = fopen (model_file_name, "RT");
  if  (fp == NULL)
    return NULL;

  char*  buff    = NULL;
  int32    buffLen = 0;
  bool   eof = false;

  // read parameters

  svm_model *model = new svm_model (fileDesc, log);
  svm_parameter& param = model->param;

  char cmd[82];
  while  (1)
  {
    fscanf (fp, "%80s", cmd);

    if  (strcmp (cmd, "svm_type") == 0)
    {
      fscanf (fp, "%80s", cmd);

      param.svm_type = SVM_Type_FromStr (cmd);
      if  (param.svm_type == SVM_NULL)
      {
        fprintf (stderr, "unknown svm type.\n");
        delete model;
        model = NULL;
        return  NULL;
      }
    }

    else if  (strcmp(cmd, "kernel_type") == 0)
    {    
      fscanf (fp, "%80s", cmd);

      param.kernel_type = Kernel_Type_FromStr (cmd);
      if  (param.kernel_type == Kernel_NULL)
      {
        fprintf(stderr,"unknown kernel function.\n");
        delete  model;
        model = NULL;
        return NULL;
      }
    }
    else if(strcmp(cmd,"degree")==0)
      fscanf(fp,"%d",&param.degree);

    else if  (strcmp (cmd, "gamma") == 0)
      fscanf(fp,"%lf",&param.gamma);

    else if  (strcmp (cmd, "coef0") == 0)
      fscanf (fp, "%lf", &param.coef0);

    else if  (strcmp (cmd, "nr_class") == 0)
      fscanf (fp, "%d", &model->nr_class);

    else if  (strcmp (cmd, "total_sv") == 0)
      fscanf (fp,"%d",&model->l);

    else if  (strcmp(cmd,"rho")==0)
    {
      int32 n = model->nr_class * (model->nr_class - 1) / 2;
      model->rho = new double[n];
      for (int32 i = 0;  i < n;  i++)
        fscanf (fp, "%lf", &model->rho[i]);
    }

    else if  (strcmp (cmd, "label") == 0)
    {
      int32 n = model->nr_class;
      model->label = new int32[n];
      for (int32 i=0;  i < n;  i++)
        fscanf (fp, "%d", &model->label[i]);
    }

    else if  (strcmp (cmd, "probA") == 0)
    {
      int32 n = model->nr_class * (model->nr_class-1) / 2;  // n = Totl num of Binary Combos
      model->probA = new double[n];
      for (int32 i = 0;  i < n;  i++)
        fscanf (fp, "%lf", &model->probA[i]);
    }

    else if  (strcmp (cmd, "probB") == 0)
    {
      int32 n = model->nr_class * (model->nr_class-1)/2;
      model->probB = new double[n];
      for  (int32 i = 0;  i < n;  i++)
        fscanf(fp,"%lf",&model->probB[i]);
    }

    else if  (strcmp (cmd, "nr_sv") == 0)
    {
      int32 n = model->nr_class;
      model->nSV = new int32[n];
      for (int32 i = 0;  i < n;  i++)
        fscanf(fp, "%d", &model->nSV[i]);
    }

    else if  (strcmp (cmd, "SV") == 0)
    {
      while(1)
      {
        int32 c = getc(fp);
        if(c==EOF || c=='\n') break;  
      }
      break;
    }
    else
    {
      fprintf(stderr,"unknown text in model file: [%s]\n",cmd);
      delete  model;
      model = NULL;
      return NULL;
    }
  }

  // read sv_coef and SV

  int32 elements = 0;
  long pos = ftell(fp);

  int32  max_line_len = 1024;
  char*  line = new char[max_line_len];

  char  *p, *endptr, *idx, *val;

  readline (fp, line, max_line_len, eof);

  while  (!eof)
  {
    p = strtok (line,":");
    while(1)
    {
      p = strtok(NULL,":");
      if  (p == NULL)
        break;
      ++elements;
    }

    readline (fp, line, max_line_len, eof);
  }
  elements += model->l;

  fseek (fp, pos, SEEK_SET);

  int32 m = model->nr_class - 1;
  int32 l = model->l;
  model->sv_coef = new double*[m];
  int32 i;
  for  (i = 0;  i < m;  i++)
    model->sv_coef[i] = new double[l];

  //model->SV = Malloc(svm_node*,l);
  model->SV.DeleteContents ();

  //svm_node *x_space = NULL;
  //if  (l > 0) x_space = Malloc(svm_node,elements);

  int32 j=0;
  for  (i = 0;  i < l;  i++)
  {
    readline (fp, line, max_line_len, eof);

    //model->SV[i] = &x_space[j];
    FeatureVectorPtr  fv = new FeatureVector (fileDesc->NumOfFields ());
    
    p = strtok(line, " \t");
    model->sv_coef[0][i] = strtod(p,&endptr);
    for(int32 k=1;k<m;k++)
    {
      p = strtok(NULL, " \t");
      model->sv_coef[k][i] = strtod(p,&endptr);
    }

    while(1)
    {
      idx = strtok(NULL, ":");
      val = strtok(NULL, " \t");

      if(val == NULL)
        break;

      //x_space[j].index = (int32) strtol(idx,&endptr,10);
      //x_space[j].value = strtod(val,&endptr);

      int32     featureNum   = (int32) strtol(idx, &endptr, 10);
      FFLOAT  featureValue = (FFLOAT)strtod(val, &endptr);

      fv->AddFeatureData (featureNum, featureValue);

      ++j;
    }

    //x_space[j++].index = -1;
    model->SV.PushOnBack (fv);
  }
  delete  line;
  line = NULL;

  if  (ferror (fp) != 0 || fclose (fp) != 0)
    return NULL;

  model->weOwnSupportVectors = true;  // XXX
  model->SV.Owner (true);
  return model;
}  /* svm_load_model */




void  SVM289_MFS::svm_destroy_model (svm_model*&  model)
{
  //if  (model->weOwnSupportVectors  &&  (model->l > 0))
  //  free ((void *)(model->SV[0]));
  if  (model->weOwnSupportVectors)
    model->SV.Owner (true);
  else
    model->SV.Owner (false);

  delete  model;
  model = NULL;
}




void svm_destroy_param (svm_parameter*& param)
{
  delete  param;
  param = NULL;
}




const char *svm_check_parameter (const svm_problem*    prob, 
                                 const svm_parameter*  param
                                )
{
  // svm_type

  int32  svm_type = param->svm_type;

  if  (svm_type != C_SVC        &&
       svm_type != NU_SVC       &&
       svm_type != ONE_CLASS    &&
       svm_type != EPSILON_SVR  &&
       svm_type != NU_SVR
      )
    return "unknown svm type";
  
  // kernel_type, degree
  
  int32  kernel_type = param->kernel_type;

  if  (kernel_type != LINEAR       &&
       kernel_type != POLY         &&
       kernel_type != RBF          &&
       kernel_type != SIGMOID      &&
       kernel_type != PRECOMPUTED
      )
    return "unknown kernel type";

  if  (param->degree < 0)
    return "degree of polynomial kernel < 0";

  // cache_size,eps,C,nu,p,shrinking

  if  (param->cache_size <= 0)
    return "cache_size <= 0";

  if  (param->eps <= 0)
    return "eps <= 0";

  if  (svm_type == C_SVC       ||
       svm_type == EPSILON_SVR ||
       svm_type == NU_SVR
      )
    if  (param->C <= 0)
      return "C <= 0";

  if  (svm_type == NU_SVC     ||
       svm_type == ONE_CLASS  ||
       svm_type == NU_SVR
      )
    if  ((param->nu <= 0) || (param->nu > 1))
      return "nu <= 0 or nu > 1";

  if  (svm_type == EPSILON_SVR)
  {
    if  (param->p < 0)
      return "p < 0";
  }

  if  (param->shrinking != 0 &&  param->shrinking != 1)
    return "shrinking != 0 and shrinking != 1";

  if  ((param->probability != 0)  &&  (param->probability != 1))
    return "probability != 0 and probability != 1";

  if  ((param->probability == 1)  &&  (svm_type == ONE_CLASS))
    return "one-class SVM probability output not supported yet";


  // check whether nu-svc is feasible
  
  if  (svm_type == NU_SVC)
  {
    int32 l = prob->l;
    int32 max_nr_class = 16;
    int32 nr_class = 0;
    int32*  label = new int32[max_nr_class];
    int32*  count = new int32[max_nr_class];

    int32 i;
    for  (i = 0;  i < l;  i++)
    {
      int32 this_label = (int32)prob->y[i];
      int32  j;
      for  (j = 0;  j < nr_class;  j++)
      {
        if  (this_label == label[j])
        {
          ++count[j];
          break;
        }
      }

      if  (j == nr_class)
      {
        if  (nr_class == max_nr_class)
        {
          int32  oldMaxNrClass = max_nr_class;
          max_nr_class *= 2;
          label = GrowAllocation (label, oldMaxNrClass, max_nr_class);
          count = GrowAllocation (count, oldMaxNrClass, max_nr_class);
        }
        label[nr_class] = this_label;
        count[nr_class] = 1;
        ++nr_class;
      }
    }
  

    for  (i = 0;  i < nr_class;  i++)
    {
      int32  n1 = count[i];
      for  (int32 j = i + 1;  j < nr_class;  j++)
      {
        int32 n2 = count[j];
        if  ((param->nu * (n1 + n2) / 2) > Min (n1, n2))
        {
          delete  label;  label = NULL;
          delete  count;  count = NULL;
          return "specified nu is infeasible";
        }
      }
    }

    delete  label;  label = NULL;
    delete  count;  count = NULL;
  }

  return NULL;
}  /* svm_check_parameter */




int32  svm_check_probability_model (const svm_model *model)
{
  return ((model->param.svm_type == C_SVC       ||  model->param.svm_type == NU_SVC) &&  model->probA!=NULL && model->probB!=NULL) ||
         ((model->param.svm_type == EPSILON_SVR ||  model->param.svm_type == NU_SVR) &&  model->probA!=NULL);
}
