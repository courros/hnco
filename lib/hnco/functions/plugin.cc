/* Copyright (C) 2016, 2017, 2018, 2019, 2020 Arnaud Berny

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
#include <dlfcn.h>              // dlopen, dlsym, dlclose

#include "hnco/exception.hh"

#include "plugin.hh"


using namespace hnco::function;
using namespace hnco::exception;


FunctionPlugin::FunctionPlugin(int bv_size, std::string path, std::string name):
  _bv_size(bv_size)
{
  _handle = dlopen(path.c_str(), RTLD_LAZY);
  if (!_handle) {
    std::ostringstream stream;
    stream << "FunctionPlugin::FunctionPlugin: "
           << std::string(dlerror());
    throw Error(stream.str());
  }
  dlerror();
  _extern_function = (extern_function_t) dlsym(_handle, name.c_str());
  char *error = dlerror();
  if (error != NULL) {
    std::ostringstream stream;
    stream << "FunctionPlugin::FunctionPlugin: " << std::string(error);
    throw Error(stream.str());
  }
}

FunctionPlugin::~FunctionPlugin()
{
  dlclose(_handle);
}

double FunctionPlugin::eval(const bit_vector_t& x)
{
  assert(int(x.size()) == _bv_size);
  return (*_extern_function)(x.data(), _bv_size);
}
