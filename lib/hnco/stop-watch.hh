/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_STOP_WATCH_H
#define HNCO_STOP_WATCH_H

#include <ctime>                // clock, clock_t


namespace hnco
{

  /// Stop watch
  class StopWatch
  {
    /// Total time
    double _total = 0;

    /// Start time
    clock_t _start;

  public:

    /// Start
    void start() {
      _start = clock();
    }

    /// Stop
    void stop() {
      _total += double(clock() - _start) / CLOCKS_PER_SEC;
    }

    /// Get total
    double get_total() { return _total; }

  };

}


#endif
