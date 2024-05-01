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

#include <cassert>
#include <iosfwd>
#include <vector>

#include "hnco/permutation.hh"  // hnco::permutation_t
#include "hnco/random.hh"       // hnco::random::Generator::normal

namespace hnco {
namespace function {

/**
 * Traveling salesman problem.
 *
 * Source: TSPLIB 95, Gerhard Reinelt
 *
 * http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/
 */
class Tsp {
private:
  /// Instance name
  std::string _name;
  /// Comment
  std::string _comment;
  /// Number of cities
  int _num_cities;
  /// Abscissas of cities
  std::vector<float> _xs;
  /// Ordinates of cities
  std::vector<float> _ys;
  /// Edge weight type
  enum class EdgeWeightType {
    /// ATT
    ATT,
    /// Euclidean 2D
    EUC_2D
  };
  /// Edge weith type
  EdgeWeightType _edge_weight_type = EdgeWeightType::ATT;
  /// Distances
  std::vector<std::vector<float>> _distances;
  /**
   * @name Load and save instance
   */
  ///@{
  /**
   * Load an instance.
   * \throw std::runtime_error
   */
  void load_(std::istream& stream);
  /// Load coordinates
  void load_coordinates(std::istream& stream);
  /**
   * Save an instance.
   * \warning Does nothing
   */
  void save_(std::ostream& stream) const;
  ///@}
  void compute_distances();
  void compute_distances_att();
  void compute_distances_euc_2d();

public:
  /// Default constructor
  Tsp() {}
  /**
   * @name Instance generators
   */
  ///@{
  /**
   * Instance generator.
   * \param n Number of vertices
   * \param generator Generator for distances
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
  /**
   * Random instance.
   * \param n Number of vertices
   *
   * Distances are sampled from the normal distribution.
   */
  void random(int n) {
    assert(n > 0);
    generate(n, hnco::random::Generator::normal);
  }
  ///@}
  /**
   * @name Load and save instance
   */
  ///@{
  /**
   * Load instance.
   * \param path Path of the instance to load
   * \throw std::runtime_error
   */
  void load(std::string path);
  /**
   * Save instance.
   * \param path Path of the instance to save
   * \throw std::runtime_error
   */
  void save(std::string path) const;
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
