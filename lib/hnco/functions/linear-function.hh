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

#ifndef HNCO_FUNCTIONS_LINEAR_FUNCTION_H
#define HNCO_FUNCTIONS_LINEAR_FUNCTION_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "function.hh"


namespace hnco {
namespace function {


  /// Linear function
  class LinearFunction:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _weights;
    }

    /// Weights
    std::vector<double> _weights;

  public:

    /// Constructor
    LinearFunction() {}

    /** @name Instance generators
     */
    ///@{

    /** Instance generator.

        \param n Size of bit vectors
        \param generator Weight generator
    */
    template<class Generator>
    void random(int n, Generator generator) {
      assert(n > 0);

      _weights.resize(n);
      for (size_t i = 0; i < _weights.size(); i++)
        _weights[i] = generator();
    }

    /** Random instance.

        The weights are sampled from the normal distribution.

        \param n Size of bit vectors
    */
    void random(int n) {
      assert(n > 0);

      random(n, hnco::random::Random::normal);
    }

    ///@}


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
    size_t get_bv_size() { return _weights.size(); }

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
