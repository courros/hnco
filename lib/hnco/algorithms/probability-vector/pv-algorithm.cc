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

#include "hnco/logging/logger.hh"

#include "pv-algorithm.hh"


using namespace hnco::function;
using namespace hnco::algorithm;
using namespace hnco;


void
PvAlgorithm::log()
{
  assert(_something_to_log);

  logging::Logger l(_log_context);

  if (_log_entropy)
    l.line() << pv_entropy(_pv) << " ";

  if (_log_pv) {
    int upper_bound = std::min(_log_num_components, int(_pv.size()));
    for (int i = 0; i < upper_bound; i++)
      l.line() << _pv[i] << " ";
  }

}
