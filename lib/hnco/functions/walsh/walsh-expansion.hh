/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

   This file is part of HNCO.

   HNCO is free software: you can redistribute it and/or modify it
   under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   HNCO is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
   Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with HNCO. If not, see
   <http://www.gnu.org/licenses/>.

*/

#ifndef HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_H
#define HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_H

#include <assert.h>

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"
#include "hnco/bit-matrix.hh"


namespace hnco {
namespace function {


  /** Walsh expansion.

      Its expression is of the form

      \f$ f(x) = \sum_u a_u (-1)^{x \cdot u} \f$

      where the sum is over a subset of \f$ \{0, 1\}^n \f$ and \f$ x
      \cdot u = \sum_i x_i u_i \f$ is mod 2. The real numbers \f$ a_u
      \f$ are the coefficients of the expansion and the bit vectors
      \f$ u \f$ are its feature vectors.
  */
  class WalshExpansion:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & _terms;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & _terms;

      _image.resize(_features.size());
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Terms
    std::vector<WalshTransformTerm> _terms;

    /// Features
    hnco::bit_matrix_t _features;

    /// Coefficients
    std::vector<double> _coefficients;

    /// Image of bit vectors under the feature matrix
    bit_vector_t _image;

  public:

    /// Constructor
    WalshExpansion() {}

    /// Get bit vector size
    size_t get_bv_size() {
      assert(_terms.size() > 0);
      return _terms[0].feature.size();
    }

    /** Random instance.

        \param n                Size of bit vector
        \param num_features     Number of feature vectors
        \param stddev           Standard deviation of the coefficients
    */
    void random(int n, int num_features, double stddev);

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Display
    void display(std::ostream& stream);

  };


} // end of namespace function
} // end of namespace hnco


#endif
