/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023 Arnaud Berny

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

#ifndef HNCO_MULTIOBJECTIVE_FUNCTIONS_COLLECTION_PYTHON_H
#define HNCO_MULTIOBJECTIVE_FUNCTIONS_COLLECTION_PYTHON_H

#include <string>

#include <pybind11/embed.h>

#include "hnco/multiobjective/functions/function.hh"


namespace hnco {
namespace multiobjective {
namespace function {


/** Python function.

    Uses pybind11.

    The constructor initializes the python interpreter and the
    destructor finalizes it.

    The python code must import the hnco module (built separately) to
    allow for communication between C++ and python. It must also
    define a derived class that inherits Function and an instance of
    it.
*/
class PythonFunction: public Function {

  /// Module
  pybind11::object _scope;

  /// Function
  Function *_function;

public:

  /** Constructor.

      \param path Path of the python file
      \param name Name of the Function instance defined in the python file
  */
  PythonFunction(std::string path, std::string name);

  /// Destructor
  ~PythonFunction();

  /// Get bit vector size
  int get_bv_size() const;

  /// Get output size (number of objectives)
  int get_output_size() const;

  /// Evaluate a bit vector
  void evaluate(const bit_vector_t& bv, value_t& value);

};


} // end of namespace function
} // end of namespace multiobjective
} // end of namespace hnco


#endif
