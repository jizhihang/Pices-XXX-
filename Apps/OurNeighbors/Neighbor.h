#ifndef  _NEIGHBOR_
#define  _NEIGHBOR_

#include  "KKQueue.h"
#include  "Str.h"
#include  "MLClass.h"
#include  "ImageFeatures.h"

using namespace  MLL;


class   Neighbor;
typedef  Neighbor*  NeighborPtr;


typedef  enum  {AnyPlanktonClass, SamePlanktonClass}  NeighborType;


KKStr  NeighborTypeToStr (NeighborType  n);


class   Neighbor
{
public:
  Neighbor (double              _row,
            double              _col,
            const KKStr&        _fileName,
            MLClassConstPtr  _mlClass,
            double              _size
           );

  Neighbor (ImageFeaturesPtr  image);

  const KKStr&        ClassName       ();
  double              Col             () const  {return  col;}
  double              Dist ();
  const KKStr&        FileName        () const  {return  fileName;}
  MLClassConstPtr  MLClass      () const  {return  mlClass;}
  Neighbor*           NearestNeighbor () const  {return  nearestNeighbor;}
  double              Row             () const  {return  row;}
  KKStr               RowCol          ();
  double              Size            () const  {return  size;}
  double              SquareDist      () const  {return  squareDist;}

  MLClassConstPtr  NearestNeighborClass     ()  const;
  const KKStr&        NearestNeighborClassName ()  const;
  double              NearestNeighborSize      ()  const;
  double              NearestNeighborRow       ()  const;
  KKStr               NearestNeighborRowCol    ()  const;



  void  NearestNeighbor (Neighbor*  _nearestNeighbor)  {nearestNeighbor = _nearestNeighbor;}
  void  SquareDist      (double     _squareDist);


  void  CompareDist (NeighborPtr  n);

  void  CompareDist (NeighborPtr  n,
                     double       _squareDist
                    );

private:
  double              col;
  double              dist;
  bool                distCalculated;
  KKStr               fileName;
  MLClassConstPtr  mlClass;
  int32               largestDist;
  Neighbor*           nearestNeighbor;
  double              row;
  double              size;
  double              squareDist;

  static
  KKStr               noClassName;
};


typedef  Neighbor*  NeighborPtr;




class  NeighborList: public KKQueue<Neighbor>
{
public:
  NeighborList (bool     _owner, 
                int32      _size,
                RunLog&  _log
               );

  NeighborList (ImageFeaturesList&  images,
                RunLog&             _log
               );


  //*****************************************************************************  
  // Will update the nearestNeighbor Pointers in each Neighbor object.          *
  //                                                                            *
  //   neighborType      Weather intersted in any class as neighbor or the only *
  //                     the same class as neighbor.                            *
  //                                                                            *
  //   restrictedClass   if specified then will only interested in looking  for *
  //                     that class                                             *
  //*****************************************************************************  
  void  FindNearestNeighbors (NeighborType        neighborType,
                              MLClassConstPtr  restrictedClass = NULL
                             );

  VectorIntPtr  HistogramByDistance (int32  numOfBuckets,
                                     int32  bucketSize
                                    );


  void  CalcStatistics (double&  mean,
                        double&  stdDev,
                        double&  minDist,
                        double&  maxDist
                       ) const;

  double LargestDist () const; 


  void  ReportClassNeighbor (MLClassConstListPtr  mlClasses,
                             ostream&                r
                            );

  void  ReportClassRow (MLClassConstListPtr  mlClasses,
                        ostream&                r
                       );

  void  ReportClassRowRestricted (MLClassConstListPtr  mlClasses,
                                  ostream&                r,
                                  MLClassConstPtr      restrictedClass
                                 );

  void  SortByClass ();

  void  SortByClassRow ();

  void  SortByRow ();

private:
  RunLog&   log;
};  /* NeighborList */


typedef  NeighborList* NeighborListPtr;


#endif
