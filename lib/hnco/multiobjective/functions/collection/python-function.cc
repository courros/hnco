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

#include <assert.h>

#include "python-function.hh"


namespace py = pybind11;

using namespace hnco::multiobjective::function;


PythonFunction::PythonFunction(std::string path, std::string name)
{
  py::initialize_interpreter();
  _scope = py::module_::import("__main__").attr("__dict__");
  py::eval_file(path, _scope);
  py::object obj = _scope[name.c_str()];
  _function = obj.cast<Function *>();
}

PythonFunction::~PythonFunction()
{
  py::finalize_interpreter();
}

int
PythonFunction::get_bv_size() const
{
  return _function->get_bv_size();
}

int
PythonFunction::get_output_size() const
{
  return _function->get_output_size();
}

void
PythonFunction::evaluate(const bit_vector_t& bv, value_t& value)
{
  assert(int(value.size()) == get_output_size());

  _function->evaluate(bv, value);
}
