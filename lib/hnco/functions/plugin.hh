/* Copyright (C) 2016, 2017, 2018, 2019 Arnaud Berny

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

#ifndef HNCO_FUNCTIONS_PLUGIN_H
#define HNCO_FUNCTIONS_PLUGIN_H

#include <string>

#include "function.hh"


namespace hnco {
namespace function {


  /// Function plugin
  class FunctionPlugin:
    public Function {

    /// Bit vector size
    int _bv_size;

    /// Handle returned by dlopen
    void *_handle;

    /// Type of an extern function
    typedef double (*extern_function_t)(const bit_t *, size_t);

    /// Extern function
    extern_function_t _extern_function;

  public:

    /** Constructor.

        \param bv_size Size of bit vectors
        \param path Path to a shared library
        \param name Name of a function of the shared library
    */
    FunctionPlugin(int bv_size, std::string path, std::string name);

    /// Destructor
    ~FunctionPlugin();

    /// Get bit vector size
    int get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double eval(const bit_vector_t&);

  };


} // end of namespace function
} // end of namespace hnco


#endif
