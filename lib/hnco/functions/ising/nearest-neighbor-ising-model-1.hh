/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_ISING_NEAREST_NEIGHBOR_ISING_MODEL_1_H
#define HNCO_FUNCTIONS_ISING_NEAREST_NEIGHBOR_ISING_MODEL_1_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


  /** Walsh expansion of degree 1.

      Its expression is of the form

      \f$ f(x) = \sum_i a_i (1 - 2x_i) \f$

      or equivalently

      \f$ f(x) = \sum_i a_i (-1)^{x_i} \f$

  */
  class NearestNeighborIsingModel1:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _linear;
    }

    /// Linear part
    std::vector<double> _linear;

  public:

    /// Constructor
    NearestNeighborIsingModel1() {}

    /** Random instance.

        The weights are sampled from the normal distribution.

        \param n Size of bit vector
    */
    void random(int n);

    /** Random instance.

        \param n Size of bit vectors
        \param dist Distribution of weights
    */
    template<class T>
    void random(int n, T dist) {
      assert(n > 0);

      _linear.resize(n);
      for (size_t i = 0; i < _linear.size(); i++)
        _linear[i] = dist();
    }

    /** @name Evaluation
     */
    ///@{

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

    /// Incremental evaluation
    double incremental_eval(const bit_vector_t& x, double v, const hnco::sparse_bit_vector_t& flipped_bits);

    ///@}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _linear.size(); }

    /// Get the global maximum
    double get_maximum();

    /** Check for a known maximum.
        \return true */
    bool has_known_maximum() { return true; }

    /** Check whether the function provides incremental evaluation.
        \return true
    */
    bool provides_incremental_evaluation() { return true; }

    ///@}

  };


} // end of namespace function
} // end of namespace hnco


#endif
