/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_COLLECTION_TSP_H
#define HNCO_FUNCTIONS_COLLECTION_TSP_H

#include <iostream>
#include <vector>

#include <boost/serialization/vector.hpp>

#include "hnco/functions/function.hh"
#include "hnco/permutation.hh"  // hnco::permutation_t
#include "hnco/serialization.hh"


namespace hnco {
namespace function {


/** Traveling salesman problem.

*/
class Tsp: public Function {

private:

  friend class boost::serialization::access;

  /// Serialize
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & _distances;
  }

  /// Distances
  std::vector<std::vector<double>> _distances;

public:

  /// Default constructor
  Tsp() {}


  /** @name Instance generators
   */
  ///@{

  /** Instance generator.

      \param n Number of vertices
      \param generator Generator for distances
  */
  template<class Generator>
  void generate(int n, Generator generator) {
    assert(n > 0);
    _distances.resize(n);
    for (int i = 0; i < n; i++)
      _distances[i].resize(n);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < i; j++) {
        _distances[i][j] = generator();
        _distances[i][j] = _distances[j][i];
      }
  }

  /** Random instance.

      Distances are sampled from the normal distribution.

      \param n Number of vertices
  */
  void random(int n) {
    assert(n > 0);
    generate(n, hnco::random::Generator::normal);
  }

  ///@}


  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) { load_from_archive(*this, path, "Tsp"); }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const { save_to_archive(*this, path, "Tsp"); }

  ///@}

  /// Get the number of elements
  int get_num_elements() const { return _distances.size(); }

  /// Display the problem
  void display(std::ostream& stream) const;

  /// Describe a solution
  void describe(const hnco::permutation_t& permutation, std::ostream& stream);

  /// Evaluate a solution
  double evaluate(const hnco::permutation_t& permutation);

};


} // end of namespace function
} // end of namespace hnco


#endif
