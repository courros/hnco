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

#ifndef HNCO_ALGORITHMS_PV_PV_ALGORITHM_H
#define HNCO_ALGORITHMS_PV_PV_ALGORITHM_H

#include "hnco/algorithms/iterative-algorithm.hh"

#include "probability-vector.hh"


namespace hnco {
namespace algorithm {

/// Probability vector algorithm
class PvAlgorithm: public IterativeAlgorithm {

protected:

  /// Probability vector
  pv_t _pv;

  /// Lower bound of probability
  double _lower_bound;

  /// Upper bound of probability
  double _upper_bound;

  /** @name Logging
   */
  ///@{

  /// Log entropy
  bool _log_entropy = false;

  /// Log probability vector
  bool _log_pv = false;

  /// Number of probability vector components to log
  int _log_num_components = 5;

  ///@}

  /** @name Loop
   */
  ///@{

  /// Log
  void log() override;

  ///@}

  /// Set flag for something to log
  void set_something_to_log() { _something_to_log = _log_entropy || _log_pv; }

public:

  /// Constructor
  PvAlgorithm(int n):
    IterativeAlgorithm(n),
    _pv(n),
    _lower_bound(1 / double(n)),
    _upper_bound(1 - 1 / double(n)) {}

  /** @name Setters for logging
   */
  ///@{

  /// Log entropy
  void set_log_entropy(bool x) { _log_entropy = x; }

  /// Set the number of probability vector components to log
  void set_log_num_components(int x) { _log_num_components = x; }

  /// Log probability vector
  void set_log_pv(bool x) { _log_pv = x; }

  ///@}

};

} // end of namespace algorithm
} // end of namespace hnco


#endif
