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

#ifndef HNCO_ALGORITHMS_LOG_CONTEXT_H
#define HNCO_ALGORITHMS_LOG_CONTEXT_H

#include "hnco/functions/controllers/controller.hh"


namespace hnco {
namespace logging {


  /** Log context.

      A log context gives an algorithm more information about what is
      going on during optimization than what can be gained through its
      function. In particular, its function may not be a function
      controller. Information is provided through a log context in the
      form of a string.
   */
  class LogContext {
  public:

    /// Get context
    virtual std::string to_string() = 0;
  };


  /// Log context for ProgressTracker
  class ProgressTrackerContext:
    public LogContext {

    /// Progress tracker
    hnco::function::controller::ProgressTracker *_pt;

  public:

    /// Constructor
    ProgressTrackerContext(hnco::function::controller::ProgressTracker *pt):
      _pt(pt) {}

    /// Get context
    std::string to_string() {
      assert(_pt);

      std::ostringstream stream;
      stream << _pt->get_num_calls();
      return stream.str();
    }

  };


}
}


#endif
