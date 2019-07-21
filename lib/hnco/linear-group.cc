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

#include <assert.h>

#include "linear-group.hh"


using namespace hnco;


// Static member
GlGenerator::names = { "Permutation", "Transvection" };


void hnco::gl_display(const gl_element_t& M, std::ostream& stream)
{
  for (auto& gen : M) {
    gen.display(stream);
    stream << " :: ";
  }
  stream << std::endl;
}

void hnco::gl_random(gl_element_t& M, int t)
{
}

void hnco::gl_multiply(const gl_element_t& M, const bit_vector_t& x, bit_vector_t& y)
{
}
