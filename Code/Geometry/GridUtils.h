//
//   Copyright (C) 2003-2007 Rational Discovery LLC
//
//   @@ All Rights Reserved  @@
//
#ifndef _GRIDUTILS_H_20050126
#define _GRIDUTILS_H_20050126

namespace RDGeom {
  class UniformGrid3D;
  class Point3D;

  //! calculate the tanimoto distance between the shapes encoded on two grids
  /*!

     tanimoto(S1,S2) =  1 - ( | S1&S2 | / | S1|S2 | )

  */
  template<class GRIDTYPE> double tanimotoDistance(const GRIDTYPE &grid1, 
                                                   const GRIDTYPE &grid2);
  //! calculate the protrude distance between the shapes encoded on two grids
  /*!

     protrude(S1,S2) = ( | S1|S2 | - | S1&S2 | ) / | S1 |

  */
  template<class GRIDTYPE> double protrudeDistance(const GRIDTYPE &grid1, 
                                                   const GRIDTYPE &grid2);


  //! calculate the grid centroid within a window of a point
  Point3D computeGridCentroid(const UniformGrid3D &grid,
                              const Point3D &pt,
                              double windowRadius,
                              double &weightSum);
  
}

#endif

