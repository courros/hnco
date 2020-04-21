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

#ifndef HNCO_EXCEPTION_H
#define HNCO_EXCEPTION_H

#include <fstream>
#include <sstream>
#include <string>

#include "bit-vector.hh"


namespace hnco {
/// Exceptions
namespace exception {

  /// Basic exception
  class Exception {};

  /// Point-value exception
  class PointValueException:
    public Exception {
  protected:
    /// Point-value
    point_value_t _pv;
  public:
    /// Constructor
    PointValueException(const point_value_t& pv):
      _pv(pv) {}
    /// Get point-value
    const point_value_t& get_point_value() const { return _pv; }
  };

  /// Maximum reached
  class MaximumReached:
    public PointValueException {
  public:
    /// Constructor
    MaximumReached(const point_value_t& pv):
      PointValueException(pv) {}
  };

  /// target reached
  class TargetReached:
    public PointValueException {
  public:
    /// Constructor
    TargetReached(const point_value_t& pv):
      PointValueException(pv) {}
  };

  /// Local maximum
  class LocalMaximum:
    public PointValueException {
  public:
    /// Const
    LocalMaximum(const point_value_t& pv):
      PointValueException(pv) {}
  };

  /// Last evaluation
  class LastEvaluation:
    public Exception {};

  /// Error
  class Error:
    public Exception {

  protected:

    /// Message
    std::string _what;

  public:

    /// Constructor
    Error(): _what("Unknown error") {}

    /// Constructor
    Error(const std::string& s): _what(s) {}

    /// Destructor
    virtual ~Error() {}

    /// Get message
    virtual const char* what() const { return _what.c_str(); }

  };

} // end of namespace exception
} // end of namespace hnco


#endif
