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
#include "hnco/random.hh"


namespace hnco {
namespace function {


  /** Nearest neighbor Ising model in two dimensions.

      We are considering a rectangular lattice in which each site has
      (at most) four neighbors (left, right, above, below).

      The expression of the function is of the form

      \f$ f(x) = \sum_{(i, j)} J_{ij} (1 - 2x_i)(1 - 2x_j) + \sum_i
      h_i (1 - 2x_i)\f$

      or equivalently

      \f$ f(x) = \sum_{(i, j)} J_{ij} (-1)^{x_i + x_j} + \sum_i h_i
      (-1)^{x_i}\f$

      where the first sum is over adjacent sites (i, j), \f$J_{ij}\f$
      is the interaction between adjacent sites i and j, and \f$h_i\f$
      is the external magnetic field interacting with site i.

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
      ar & _coupling_right;
      ar & _coupling_below;
      ar & _field;
      ar & _periodic_boundary_conditions;
    }

    /// Coupling with nearest neighbor to the right
    std::vector<std::vector<double> > _coupling_right;

    /// Coupling with nearest neighbor below
    std::vector<std::vector<double> > _coupling_below;

    /// External field
    std::vector<std::vector<double> > _field;

    /// Flipped bits
    bit_vector_t _flipped_bits;

    /// Periodic boundary conditions
    bool _periodic_boundary_conditions = false;

    /// Resize data structures
    void resize(int num_rows, int num_columns);

  public:

    /// Constructor
    NearestNeighborIsingModel2() {}


    /** @name Instance generators
     */
    ///@{

    /** Instance generator.

        \param num_rows Number of rows
        \param num_columns Number of columns
        \param coupling_gen Coupling generator
        \param field_gen External field generator
    */
    template<class CouplingGen, class FieldGen>
    void random(int num_rows, int num_columns, CouplingGen coupling_gen, FieldGen field_gen) {
      assert(num_rows > 0);
      assert(num_columns > 0);

      resize(num_rows, num_columns);
      for (int i = 0; i < num_rows; i++)
        for (int j = 0; j < num_columns; j++) {
          _coupling_right[i][j] = coupling_gen();
          _coupling_below[i][j] = coupling_gen();
          _field[i][j] = field_gen();
        }
    }

    /** Random instance.

        The weights are sampled from the normal distribution.

        \param num_rows Number of rows
        \param num_columns Number of columns
    */
    void random(int num_rows, int num_columns) {
      assert(num_rows > 0);
      assert(num_columns > 0);

      random(num_rows, num_columns, hnco::random::Random::normal, hnco::random::Random::normal);
    }

    ///@}


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
    size_t get_bv_size() {
      assert(_field.size() > 0);

      return _field.size() * _field[0].size();
    }

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
