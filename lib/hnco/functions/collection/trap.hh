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

#ifndef HNCO_FUNCTIONS_COLLECTION_TRAP_H
#define HNCO_FUNCTIONS_COLLECTION_TRAP_H

#include "hnco/exception.hh"

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


  /** %Trap.

      Reference:

      Kalyanmoy Deb and David E. Goldberg. 1993. Analyzing Deception
      in %Trap Functions. In Foundations of Genetic Algorithms 2,
      L. Darrell Whitley (Ed.).  Morgan Kaufmann, San Mateo, CA,
      93–108.

  */
  class Trap:
    public Function {

    /// Bit vector size
    int _bv_size;

    /// Number of traps
    int _num_traps;

    /// Trap size
    int _trap_size;

  public:

    /** Constructor.

        \param bv_size Bit vector size
        \param num_traps Number of traps

        \warning bv_size must be a multiple of num_traps
    */
    Trap(int bv_size, int num_traps):
      _bv_size(bv_size),
      _num_traps(num_traps)
    {
      assert(num_traps > 0);
      if (bv_size % num_traps != 0)
        throw std::runtime_error("Trap::Trap: _bv_size must be a multiple of _num_traps");
      _trap_size = bv_size / num_traps;
    }

    /// Get bit vector size
    int get_bv_size() { return _bv_size; }

    /// Evaluate a bit vector
    double evaluate(const bit_vector_t&);

    /** Check for a known maximum.
     *
     * \return true
     */
    bool has_known_maximum() { return true; }

    /** Get the global maximum.
        \return _bv_size */
    double get_maximum() { return _bv_size; }

  };


} // end of namespace function
} // end of namespace hnco


#endif
