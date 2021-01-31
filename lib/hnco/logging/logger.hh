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

#ifndef HNCO_LOG_LOGGER_H
#define HNCO_LOG_LOGGER_H

#include <iostream>             // std::cout, std::ostream
#include <string>
#include <sstream>              // std::ostringstream

#include "hnco/logging/log-context.hh"


namespace hnco {

/// Logging
namespace logging {


/** %Logger.

    Simple logger inspired by the Log class published in Dr. Dobb's:

    https://www.drdobbs.com/cpp/logging-in-c/201804215
*/
class Logger {

  /// Output stream
  static std::ostream *_stream;

  /// Line
  std::ostringstream _line;

public:

  /// Get the stream
  static std::ostream& stream() { return *_stream; }

  /// Set the stream
  static void set_stream(std::ostream *stream) { _stream = stream; }

  /// Default constructor
  Logger() {}

  /** Constructor.

      The constructor converts the context to a string which it writes
      at the beginning of the line.

      \param context Log context
  */
  Logger(LogContext *context) {
    if (context)
      _line << context->to_string() << " ";
  }

  /// Get the line
  std::ostringstream& line() { return _line; }

  /** Destructor.

      Send the line to the output stream and add an end of line.
  */
  virtual ~Logger() { (*_stream) << _line.str() << std::endl; }

};


/// Helper logging macro
#define HNCO_LOG(context) hnco::logging::Logger(context).line()


} // end of namespace log
} // end of namespace hnco


#endif
