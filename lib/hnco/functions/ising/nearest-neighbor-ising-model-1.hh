/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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


  /** Nearest neighbor Ising model in one dimension.

      Its expression is of the form

      \f$ f(x) = \sum_i J_{i,i+1} (1 - 2x_i)(1 - 2x_{i+1}) + \sum_i h_i (1 - 2x_i)\f$

      or equivalently

      \f$ f(x) = \sum_i J_{i,i+1} (-1)^{x_i + x_{i+1}} + \sum_i h_i
      (-1)^{x_i}\f$

      where \f$J_{i,i+1}\f$ is the interaction between adjacent sites
      i and i+1 and \f$h_i\f$ is the external magnetic field
      interacting with site i.

      In the case of periodic boundary conditions, the sum \f$i+1\f$
      is mod n.

      Since we are maximizing f or minimizing -f, the expression of f
      is compatible with what can be found in physics textbooks.

      It should be noted that such an Ising model can be represented
      by a Walsh expansion of degree 2, that is WalshExpansion2.

      Reference: https://en.wikipedia.org/wiki/Ising_model
  */
  class NearestNeighborIsingModel1:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Save
    template<class Archive>
    void save(Archive& ar, const unsigned int version) const
    {
      ar & _coupling;
      ar & _field;
      ar & _periodic_boundary_conditions;
    }

    /// Load
    template<class Archive>
    void load(Archive& ar, const unsigned int version)
    {
      ar & _coupling;
      ar & _field;
      ar & _periodic_boundary_conditions;

      _flipped_bits = bit_vector_t(get_bv_size(), 0);
    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    /// Coupling with nearest neighbor to the right
    std::vector<double> _coupling;

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
    NearestNeighborIsingModel1() {}


    /** @name Instance generators
     */
    ///@{

    /** Instance generator.

        \param n Size of bit vectors
        \param coupling_gen Coupling generator
        \param field_gen External field generator
    */
    template<class CouplingGen, class FieldGen>
    void generate(int n, CouplingGen coupling_gen, FieldGen field_gen) {
      assert(n > 0);

      resize(n);
      for (size_t i = 0; i < _coupling.size(); i++) {
        _coupling[i] = coupling_gen();
        _field[i] = field_gen();
      }
    }

    /** Random instance.

        The weights are sampled from the normal distribution.

        \param n Size of bit vector
    */
    void random(int n) {
      assert(n > 0);

      generate(n, hnco::random::Generator::normal, hnco::random::Generator::normal);
    }

    ///@}


    /** @name Evaluation
     */
    ///@{

    /** Evaluate a bit vector.

        Complexity: O(n)
    */
    double eval(const bit_vector_t&);

    /// Incrementally evaluate a bit vector
    double evaluate_incrementally(const bit_vector_t& x, double v, const sparse_bit_vector_t& flipped_bits);

    ///@}

    /** @name Information about the function
     */
    ///@{

    /// Get bit vector size
    int get_bv_size() { return _coupling.size(); }

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
