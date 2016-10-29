/* Copyright (C) 2016 Arnaud Berny

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

#include <bitset>

#include "hnco/algorithms/algorithm.hh"

#include "probability-vector.hh"


namespace hnco {
namespace algorithm {

  /// Probability Vector Algorithm
  class PvAlgorithm:
    public IterativeAlgorithm {

  public:

    enum {
      /// Log probability vector
      LOG_PV,

      /// Log entropy
      LOG_ENTROPY,

      LAST_LOG
    };

    typedef std::bitset<LAST_LOG> log_flags_t;

  protected:

    /// Probability vector
    pv_t _pv;

    /// Lower bound of probability
    double _lower_bound;

    /// Upper bound of probability
    double _upper_bound;

    /// Log flags
    log_flags_t _log_flags;

    /// Log
    void log();

  public:

    /// Constructor
    PvAlgorithm(int n):
      IterativeAlgorithm(n),
      _pv(n),
      _lower_bound(1 / double(n)),
      _upper_bound(1 - 1 / double(n)) {}

    /// Set log flags
    void set_log_flags(const log_flags_t& lf) {
      _log_flags = lf;
      _something_to_log = _log_flags.any();
    }

    /// Number of probability vector components to log
    int _log_num_components = 5;

  };


} // end of namespace algorithm
} // end of namespace hnco


#endif
