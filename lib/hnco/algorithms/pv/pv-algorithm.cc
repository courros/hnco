/* Copyright (C) 2016, 2017 Arnaud Berny

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

#include "pv-algorithm.hh"


using namespace hnco::function;
using namespace hnco::algorithm;
using namespace hnco;


void
PvAlgorithm::log()
{
  assert(_log_flags.any());

  if (_log_flags[LOG_ENTROPY])
    _stream << pv_entropy(_pv) << " ";

  if (_log_flags[LOG_PV]) {
    int upper_bound = std::min(_log_num_components, int(_pv.size()));
    for (int i = 0; i < upper_bound; i++)
      _stream << _pv[i] << " ";
  }

  _stream << std::endl;

}
