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

#ifndef HNCO_EXCEPTION_H
#define HNCO_EXCEPTION_H

#include <fstream>
#include <sstream>
#include <string>


namespace hnco {
/// All about exceptions
namespace exception {


  /// Basic exception
  class Exception {};


  /// Last evaluation
  class LastEvaluation: public Exception {};

  /// Local maximum
  class LocalMaximum: public Exception {};

  /// Maximum reached
  class MaximumReached: public Exception {};


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
