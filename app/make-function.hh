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

#ifndef HNCO_SRC_MAKE_FUNCTION
#define HNCO_SRC_MAKE_FUNCTION

#include "hnco/map.hh"

#include "hnco-options.hh"


/// Function factory
class FunctionFactory
{

  /// Map
  hnco::Map *_map = 0;

  /// Cache
  hnco::function::Cache *_cache = 0;

  /// Make a function modifier
  hnco::function::Function *make_function_modifier(hnco::function::Function *function, Options& options);

  /// Make a function controller
  hnco::function::Function *make_function_controller(hnco::function::Function *function, const Options& options);

public:

  /// Make a function
  hnco::function::Function *make_function(Options& options);

  /// Get map
  hnco::Map *get_map() { return _map; }

  /// Get cache
  hnco::function::Cache *get_cache() { return _cache; }

};


#endif
