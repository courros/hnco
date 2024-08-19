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

#ifndef HNCO_LOGGING_LOGGER_H
#define HNCO_LOGGING_LOGGER_H

#include <iostream>             // std::cout
#include <sstream>              // std::ostringstream
#include <string>

#include "log-context.hh"

namespace hnco {
/// Logging
namespace logging {

/**
 * %Logger.
 *
 * Simple logger inspired by the Log class published in Dr. Dobb's:
 *
 * https://www.drdobbs.com/cpp/logging-in-c/201804215
 */
class Logger {
  /// Line
  std::ostringstream _line;
public:
  /// Default constructor
  Logger() {}
  /**
   * Constructor.
   * @param context Log context
   */
  Logger(LogContext *context) {
    if (context)
      _line << context->to_string() << " ";
  }
  /**
   * Destructor.
   * Sends _line to std::cout and flushes it.
   */
  virtual ~Logger() { std::cout << _line.str() << std::endl; }
  /// Append data to the line
  template<class T>
  Logger& operator<<(T value) {
    _line << value << " ";
    return *this;
  }
};

} // end of namespace logging
} // end of namespace hnco

#endif
