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

#ifndef HNCO_APP_DECORATED_FUNCTION_FACTORY
#define HNCO_APP_DECORATED_FUNCTION_FACTORY

#include "hnco/maps/map.hh"
#include "hnco/functions/modifiers/all.hh"
#include "hnco/functions/controllers/all.hh"

#include "hnco-options.hh"
#include "function-factory.hh"

namespace hnco {
namespace app {

/// Decorated function factory
class DecoratedFunctionFactory {

  /// HNCO options
  const HncoOptions& _options;

  /// Factory function
  FunctionFactory& _function_factory;

  /// Map
  hnco::map::Map *_map = nullptr;

  /// Tracker controller
  hnco::function::controller::ProgressTracker *_tracker = nullptr;

  /// Cache controller
  hnco::function::controller::Cache *_cache = nullptr;

  /// StopOnTarget controller
  hnco::function::controller::StopOnTarget *_stop_on_target = nullptr;

  /// Make a function
  hnco::function::Function *make_function();

public:

  /// Constructor
  DecoratedFunctionFactory(const HncoOptions& options, FunctionFactory& function_factory)
    : _options(options)
    , _function_factory(function_factory)
  {}

  /// Make a function modifier
  hnco::function::Function *make_function_modifier();

  /** Make a function controller.

      \param function Decorated function
  */
  hnco::function::Function *make_function_controller(hnco::function::Function *function);

  /// Get map
  hnco::map::Map *get_map() { return _map; }

  /// Get tracker controller
  hnco::function::controller::ProgressTracker *get_tracker() { return _tracker; }

  /// Get Cache controller
  hnco::function::controller::Cache *get_cache() { return _cache; }

  /// Get StopOnTarget controller
  hnco::function::controller::StopOnTarget *get_stop_on_target() { return _stop_on_target; }

};

} // end of namespace hnco::app
} // end of namespace hnco

#endif
