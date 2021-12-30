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

#include <assert.h>
#include <pybind11/embed.h>
#include <pybind11/eval.h>

#include "hnco/exception.hh"

#include "python.hh"


namespace py = pybind11;

using namespace hnco::function;
using namespace hnco::exception;

PythonInterpreter::PythonInterpreter(std::string path, std::string name)
{
  py::initialize_interpreter();
  py::object scope = py::module_::import("__main__").attr("__dict__");
  py::eval_file(path, scope);
}

PythonInterpreter::~PythonInterpreter()
{
  py::finalize_interpreter();
}

double PythonInterpreter::evaluate(const bit_vector_t& x)
{
  return 0;
}
