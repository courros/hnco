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

#ifndef HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_2_H
#define HNCO_FUNCTIONS_WALSH_WALSH_EXPANSION_2_H

#include <iostream>
#include <vector>
#include <fstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


  /** Walsh expansion of degree 2.

      Its expression is of the form

      \f$ f(x) = \sum_i a_i (1 - 2x_i) + \sum_{i < j} a_{ij} (1 - 2x_i) (1 - 2x_j) \f$

      or equivalently

      \f$ f(x) = \sum_i a_i (-1)^{x_i} + \sum_{i < j} a_{ij} (-1)^{x_i + x_j} \f$

  */
  class WalshExpansion2:
    public Function {

  private:

    friend class boost::serialization::access;

    /// Serialize
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & _linear;
      ar & _quadratic;
    }

    /// Linear part
    std::vector<double> _linear;

    /** Quadratic part.

        Represented as a lower triangular matrix (without its
        diagonal).
    */
    std::vector<std::vector<double> > _quadratic;

    /// Resize data structures
    void resize(int n);

  public:

    /// Constructor
    WalshExpansion2() {}


    /** @name Instance generators
     */
    ///@{

    /** Instance generators.

        \param n Size of bit vectors
        \param linear_gen Generator for the linear part
        \param quadratic_gen Generator for the quadratic part
    */
    template<class LinearGen, class QuadraticGen>
    void generate(int n, LinearGen linear_gen, QuadraticGen quadratic_gen) {
      assert(n > 0);

      resize(n);

      // Linear part
      for (size_t i = 0; i < _linear.size(); i++)
        _linear[i] = linear_gen();

      // Quadratic part
      for (size_t i = 1; i < _quadratic.size(); i++)
        for (size_t j = 0; j < i; j++)
          _quadratic[i][j] = quadratic_gen();

    }

    /** Instance generator.

        The weights are sampled from the normal distribution.

        \param n Size of bit vector
    */
    void random(int n) {
      assert(n > 0);

      generate(n, hnco::random::Generator::normal, hnco::random::Generator::normal);
    }

    /** Generate one dimensional Ising model with long range
        interactions.

        Similar to a Dyson-Ising model except for the finite, instead
        of infinite, linear chain of spins.

        Its expression is of the form

        \f$ f(x) = \sum_{ij} J(d_{ij}) (1 - 2x_i)(1 - 2x_j)\f$

        or equivalently

        \f$ f(x) = \sum_{ij} J(d_{ij}) (-1)^{x_i + x_j}\f$

        where \f$J(d_{ij})\f$ is the interaction between sites i and
        j, \f$d_{ij} = |i-j|\f$, and \f$J(n) = n^{-\alpha}\f$.

        Since we are maximizing f or minimizing -f, the expression of f
        is compatible with what can be found in physics textbooks.

        \param n Size of bit vectors
        \param alpha Exponential decay parameter
    */
    void generate_ising1_long_range(int n, double alpha);

    /** Generate one dimensional Ising model with long range
        interactions and periodic boundary conditions.

        Similar to a Dyson-Ising model except for the finite, instead
        of infinite, linear chain of spins.

        Its expression is of the form

        \f$ f(x) = \sum_{ij} J(d_{ij}) (1 - 2x_i)(1 - 2x_j)\f$

        or equivalently

        \f$ f(x) = \sum_{ij} J(d_{ij}) (-1)^{x_i + x_j}\f$

        where \f$J(d_{ij})\f$ is the interaction between sites i and
        j, \f$d_{ij} = \min \{|i-j|, n - |i-j|\}\f$, and \f$J(n) =
        n^{-\alpha}\f$.

        Since we are maximizing f or minimizing -f, the expression of f
        is compatible with what can be found in physics textbooks.

        \param n Size of bit vectors
        \param alpha Exponential decay parameter
    */
    void generate_ising1_long_range_periodic(int n, double alpha);

    ///@}


    /// Get bit vector size
    int get_bv_size() { return _quadratic.size(); }

    /// Evaluate a bit vector
    double evaluate(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
