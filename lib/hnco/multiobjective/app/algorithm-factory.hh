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

#ifndef HNCO_MULTIOBJECTIVE_APP_ALGORITHM_FACTORY_H
#define HNCO_MULTIOBJECTIVE_APP_ALGORITHM_FACTORY_H

namespace hnco {
namespace multiobjective {

namespace algorithm {
class Algorithm;
}

namespace app {

class HncoOptions;

/// Algorithm factory
class AlgorithmFactory {
public:
  /**
   * Make an algorithm.
   * @param bv_size Bit vector size
   * @param num_objectives Number of objectives
   */
  virtual hnco::multiobjective::algorithm::Algorithm *make(int bv_size, int num_objectives) = 0;
};

/// Command line algorithm factory
class CommandLineAlgorithmFactory : public AlgorithmFactory {
  /// HNCO options
  const HncoOptions& _options;

public:
  /// Constructor
  CommandLineAlgorithmFactory(const HncoOptions& options)
    : _options(options) {}
  /** Make an algorithm.
   * @param bv_size Bit vector size
   * @param num_objectives Number of objectives
   */
  hnco::multiobjective::algorithm::Algorithm *make(int bv_size, int num_objectives);
};

} // end of namespace algorithm
} // end of namespace multiobjective
} // end of namespace hnco

#endif
