/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_MULTIOBJECTIVE_APP_FUNCTION_FACTORY_H
#define HNCO_MULTIOBJECTIVE_APP_FUNCTION_FACTORY_H

#include "hnco/multiobjective/functions/function.hh"

#include "hnco-mo-options.hh"


namespace hnco {
namespace multiobjective {
/// Classes for applications
namespace app {


/// Function factory
class FunctionFactory {
public:
  /// Make a function
  virtual hnco::multiobjective::function::Function *make() = 0;
};


/// Command line function factory
class CommandLineFunctionFactory : public FunctionFactory {
  /// HNCO options
  const HncoOptions& _options;
public:
  /// Constructor
  CommandLineFunctionFactory(const HncoOptions& options)
    : _options(options) {}
  /// Make a function
  hnco::multiobjective::function::Function *make();
};


} // end of namespace app
} // end of namespace multiobjective
} // end of namespace hnco


#endif
