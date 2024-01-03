/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024 Arnaud Berny

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

#include "hnco/functions/function.hh"
#include "hnco/permutation.hh"  // hnco::permutation_t
#include "hnco/serialization.hh"


namespace hnco {
namespace function {


/** Traveling salesman problem.

    Source: TSPLIB 95, Gerhard Reinelt
*/
class Tsp {

private:

  std::string _name;
  std::string _comment;
  int _num_cities;
  std::vector<float> _x;
  std::vector<float> _y;
  int _edge_weight_type = ATT;
  enum {
    ATT,
    EUC_2D
  };

  /// Distances
  std::vector<std::vector<float>> _distances;

  /** @name Load and save instance
   */
  ///@{

  /** Load an instance.

      \throw std::runtime_error
  */
  void load_(std::istream& stream);
  void load_coordinates(std::istream& stream);

  /// Save an instance
  void save_(std::ostream& stream) const;

  ///@}

  void compute_distances();
  void compute_distances_att();
  void compute_distances_euc_2d();

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
        double d = generator();
        _distances[i][j] = d;
        _distances[j][i] = d;
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
  void load(std::string path) {
    std::ifstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Tsp::load: Cannot open " + path);
    load_(stream);
  }

  /** Save instance.

      \param path Path of the instance to save
      \throw std::runtime_error
  */
  void save(std::string path) const {
    std::ofstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Tsp::save: Cannot open " + path);
    save_(stream);
  }

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
