/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_EA_CROSSOVER_H
#define HNCO_ALGORITHMS_EA_CROSSOVER_H

#include <assert.h>

#include "hnco/bit-vector.hh"
#include "hnco/random.hh"


namespace hnco {
namespace algorithm {


/// %Crossover
class Crossover {

public:

  /// Destructor
  virtual ~Crossover() {}

  /** Recombine.

      The offspring is the crossover of two parents.

      \param parent1 First parent
      \param parent2 Second parent
      \param offspring Offspring
  */
  virtual void recombine(const bit_vector_t& parent1, const bit_vector_t& parent2, bit_vector_t& offspring) = 0;

};


/// Uniform crossover
class UniformCrossover:
    public Crossover {

public:

  /** Recombine.

      The offspring is the uniform crossover of two parents.

      \param parent1 First parent
      \param parent2 Second parent
      \param offspring Offspring
  */
  void recombine(const bit_vector_t& parent1, const bit_vector_t& parent2, bit_vector_t& offspring);

};


/// Biased crossover
class BiasedCrossover:
    public Crossover {

  /// Bernoulli distribution
  std::bernoulli_distribution _bernoulli_dist;

public:

  /// Constructor
  BiasedCrossover():
    _bernoulli_dist(0.5) {}

  /** Recombine.

      Each offspring's bit is copied from second parent with a fixed
      probability (the crossover bias), from first parent otherwise.

      \param parent1 First parent
      \param parent2 Second parent
      \param offspring Offspring
  */
  void recombine(const bit_vector_t& parent1, const bit_vector_t& parent2, bit_vector_t& offspring);

  /// Set bias
  void set_bias(double b) {
    assert(b > 0);
    assert(b < 1);

    _bernoulli_dist = std::bernoulli_distribution(b);
  }

};


} // end of namespace algorithm
} // end of namespace hnco


#endif
