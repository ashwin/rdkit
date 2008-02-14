//
//  Copyright (C) 2004-2008 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved  @@
//
#ifndef __RD_VECTOR_H__
#define __RD_VECTOR_H__

#include <RDGeneral/Invariant.h>
#include <RDGeneral/utils.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <boost/random.hpp>
#include <boost/smart_ptr.hpp>

namespace RDNumeric {

  
  //! A class to represent vectors of numbers.
  template <class TYPE> class Vector {
    
  public:

    typedef boost::shared_array<TYPE> DATA_SPTR;

    //! Initialize with only a size.
    explicit Vector(unsigned int N) {
      d_size = N;
      TYPE *data = new TYPE[N];
      memset(static_cast<void *>(data),0,d_size*sizeof(TYPE));
      d_data.reset(data);
    }
    
    //! Initialize with a size and default value.
    Vector(unsigned int N, TYPE val) { //: Vector(N) {
      d_size = N;
      TYPE *data = new TYPE[N];
      
      unsigned int i;
      for (i = 0; i < N; i++) {
        data[i] = val;
      }
      d_data.reset(data);
    }

    //! Initialize from a smart pointer.
    /*!
      <b>NOTE:</b> the data is not copied in this case
    */
    Vector(unsigned int N, DATA_SPTR data) {//TYPE *data) {
      d_size = N;
      d_data = data;
    }

    //! copy constructor
    /*! We make a copy of the other vector's data.
     */
    Vector(const Vector &other) {
      d_size = other.size();
      const TYPE *otherData = other.getData();
      TYPE *data = new TYPE[d_size];
            
      memcpy(static_cast<void *>(data), static_cast<const void *>(otherData), d_size*sizeof(TYPE));
      d_data.reset(data);
    }
      
    ~Vector() {
    }

    //! return the size (dimension) of the vector
    unsigned int size() const {
      return d_size;
    }

    //! returns the value at a particular index
    inline TYPE getVal(unsigned int i) const {
      RANGE_CHECK(0, i, d_size-1);
      return d_data[i];
    }

    //! sets the index at a particular value
    inline void setVal(unsigned int i, TYPE val) {
      RANGE_CHECK(0, i, d_size-1);
      d_data[i] = val;
    }

    inline TYPE operator[](unsigned int i) const {
      RANGE_CHECK(0, i, d_size-1);
      return d_data[i];
    }

    inline TYPE& operator[](unsigned int i) {
      RANGE_CHECK(0, i, d_size-1);
      return d_data[i];
    }

    //! returns a pointer to our data array
    inline TYPE *getData() {
      return d_data.get();
    }

    //! returns a const pointer to our data array
    inline const TYPE *getData() const {
      //return dp_data;
      return d_data.get();
    }

    //! Copy operator.
    /*! We make a copy of the other Vector's data.
     */
    
    Vector<TYPE>& assign(const Vector<TYPE> &other) {
      CHECK_INVARIANT(d_size == other.size(), "Size mismatch in vector copying");
      const TYPE *otherData = other.getData();
      memcpy(static_cast<void *>(d_data.get()), static_cast<const void *>(otherData), d_size*sizeof(TYPE));
      return *this;
    }

    //! elementwise addition, vectors must be the same size.
    Vector<TYPE>& operator+=(const Vector<TYPE> &other) {
      CHECK_INVARIANT(d_size == other.size(), "Size mismatch in vector addition");
      const TYPE *otherData = other.getData();
      TYPE *data = d_data.get();
      unsigned int i;
      for (i = 0; i < d_size; i++) {
        data[i] += otherData[i];
      }
      return *this;
    }

    //! elementwise subtraction, vectors must be the same size.
    Vector<TYPE>& operator-=(const Vector<TYPE> &other) {
      CHECK_INVARIANT(d_size == other.size(), "Size mismatch in vector subtraction");
      const TYPE *otherData = other.getData();
      TYPE *data = d_data.get();
      unsigned int i;
      for (i = 0; i < d_size; i++) {
        data[i] -= otherData[i];
      }
      return *this;
    }

    //! multiplication by a scalar
    Vector<TYPE>& operator *=(TYPE scale) {
      unsigned int i;
      for (i = 0; i < d_size; i++) {
        d_data[i] *= scale;
      }
      return *this;
    }

    //! division by a scalar
    Vector<TYPE>& operator /=(TYPE scale) {
      unsigned int i;
      for (i = 0; i < d_size; i++) {
        d_data[i] /= scale;
      }
      return *this;
    }

    //! L2 norm squared
    inline TYPE normL2Sq() const {
      TYPE res = (TYPE)0.0;
      unsigned int i;
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
        res += data[i]*data[i];
      }
      return res;
    }

    //! L2 norm
    inline TYPE normL2() const {
      return sqrt(this->normL2Sq());
    }
    
    //! L1 norm
    inline TYPE normL1() const {
      TYPE res = (TYPE)0.0;
      unsigned int i;
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
        res += fabs(data[i]);
      }
      return res;
    }

    //! L-infinity norm
    inline TYPE normLinfinity() const {
      TYPE res = (TYPE)(-1.0);
      unsigned int i;
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
        if (fabs(data[i]) > res) {
          res = fabs(data[i]);
        }
      }
      return res;
    }

    //! \brief Gets the ID of the entry that has the largest absolute value
    //! i.e. the entry being used for the L-infinity norm
    inline unsigned int largestAbsValId() const {
      TYPE res = (TYPE)(-1.0);
      unsigned int i, id=d_size;
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
        if (fabs(data[i]) > res) {
          res = fabs(data[i]);
          id = i;
        }
      }
      return id;
    }

    //! \brief Gets the ID of the entry that has the largest value
    inline unsigned int largestValId() const {
      TYPE res = (TYPE)(-1.e8);
      unsigned int i, id=d_size;
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
        if (data[i] > res) {
          res = data[i];
          id = i;
        }
      }
      return id;
    }

    //! returns the dot product between two Vectors
    inline TYPE dotProduct(const Vector<TYPE> other) {
      CHECK_INVARIANT(d_size == other.size(), "Size mismatch in vector doct product");
      const TYPE *oData = other.getData();
      unsigned int i;
      TYPE res = (TYPE)(0.0);
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
        res += (data[i]*oData[i]);
      }
      return res;
    }

    //! Normalize the vector using the L2 norm
    inline void normalize() {
      TYPE val = this->normL2();
      (*this) /= val;
    }

    //! Set to a random unit vector
    inline void setToRandom(unsigned int seed=0) {
      // we want to get our own RNG here instead of using the global
      // one.  This is related to Issue285.
      RDKit::rng_type generator(42u);
      RDKit::uniform_double dist(0,1.0);
      RDKit::double_source_type randSource(generator,dist);
      if (seed > 0) {
        generator.seed(seed);
      } else {
	// we can't initialize using only clock(), because it's possible
	// that we'll get here fast enough that clock() will return 0
	// and generator.seed(0) is an error:
        generator.seed(clock()+1);
      }
            
      unsigned int i;
      TYPE *data = d_data.get();
      for (i = 0; i < d_size; i++) {
	data[i] = randSource();
      }
      this->normalize();
    }
      
  private:
    unsigned int d_size;     //! < our length
    DATA_SPTR d_data;
    Vector<TYPE>& operator=(const Vector<TYPE> &other);
  };

  typedef Vector<double> DoubleVector;
}

//! ostream operator for Vectors
template <typename TYPE> std::ostream & operator<<(std::ostream& target, 
                                                   const RDNumeric::Vector<TYPE> &vec) {
  unsigned int siz = vec.size();
  target << "Size: " << siz << " [";
  unsigned int i;
  for (i = 0; i < siz; i++) {
    target << std::setw(7) << std::setprecision(3) << vec.getVal(i) << ", ";
  }
  target << "]\n";
  return target;
}

#endif

    
