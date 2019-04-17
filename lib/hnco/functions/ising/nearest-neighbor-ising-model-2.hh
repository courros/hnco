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

#ifndef HNCO_FUNCTIONS_ISING_NEAREST_NEIGHBOR_ISING_MODEL_2_H
#define HNCO_FUNCTIONS_ISING_NEAREST_NEIGHBOR_ISING_MODEL_2_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


  /** Nearest neighbor Ising model in two dimension.

      Its expression is of the form

      \f$ f(x) = \sum_i J_{i,i+1} (1 - 2x_i)(1 - 2x_{i+1}) + \sum_i h_i (1 - 2x_i)\f$

      or equivalently

      \f$ f(x) = \sum_i J_{i,i+1} (-1)^{x_i + x_{i+1}} + \sum_i h_i
      (-1)^{x_i}\f$

      where \f$J_{i,i+1}\f$ is the interaction between adjacent sites
      i and i+1 and \f$h_i\f$ is the external magnetic field
      interacting with site i.

      In the case of perdiodic boundary conditions, the sum \f$i+1\f$
      is mod n.

      Since we are maximizing f or minimizing -f, the expression of f
      is compatible with what can be found in physics textbooks.

      It should be noted that such an Ising model can be represented
      by a Walsh expansion of degree 2, that is WalshExpansion2.

      Reference: https://en.wikipedia.org/wiki/Ising_model
  */
  class NearestNeighborIsingModel2:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _couplings;
      ar & _field;
      ar & _periodic_boundary_conditions;
    }

    /// Couplings between nearest neighbors
    std::vector<double> _couplings;

    /// External field
    std::vector<double> _field;

    /// Flipped bits
    bit_vector_t _flipped_bits;

    /// Periodic boundary conditions
    bool _periodic_boundary_conditions = false;

    /// Resize data structures
    void resize(int n);

  public:

    /// Constructor
    NearestNeighborIsingModel2() {}

    /** Random instance.

        The weights are sampled from the normal distribution.

        \param n Size of bit vector
    */
    void random(int n);

    /** Random instance.

        \param n Size of bit vectors
        \param generator_couplings Couplings generator
        \param generator_field External field generator
    */
    template<class Generator>
    void random(int n, Generator generator_couplings, Generator generator_field) {
      assert(n > 0);

      resize(n);
      for (size_t i = 0; i < _couplings.size(); i++) {
        _couplings[i] = generator_couplings();
        _field[i] = generator_field();
      }
    }

    /** @name Evaluation
     */
    ///@{

    /** Evaluate a bit vector.

        Complexity: O(n)
    */
    double eval(const bit_vector_t&);

    /// Incremental evaluation
    double incremental_eval(const bit_vector_t& x, double v, const sparse_bit_vector_t& flipped_bits);

    ///@}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    size_t get_bv_size() { return _couplings.size(); }

    /** Check whether the function provides incremental evaluation.
        \return true
    */
    bool provides_incremental_evaluation() { return true; }

    ///@}

    /// Set periodic boundary conditions
    void set_periodic_boundary_conditions(bool x) { _periodic_boundary_conditions = x; }

    /// Display
    void display(std::ostream& stream);

  };


} // end of namespace function
} // end of namespace hnco


#endif
