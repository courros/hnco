/* Copyright (C) 2016 Arnaud Berny

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

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "hnco/exception.hh"
#include "hnco/functions/all.hh"
#include "hnco/random.hh"
#include "hnco/bit-vector.hh"
#include "hnco/bit-matrix.hh"

#include "ffgen-options.hh"


using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;
using namespace std;


int main(int argc, char *argv[])
{
  Options options(argc, argv);

  // Initialize random number generator
  if (!options.set_seed()) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    options.set_seed(seed);
  }
  Random::engine.seed(options.get_seed());

  switch(options.get_function()) {

  case 1: {
    LinearFunction function;
    function.random(options.get_bv_size());
    cout << "Writing LinearFunction to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    cout << "done" << endl;
    break;
  }

  case 50: {
    QuadraticFunction function;
    function.random(options.get_bv_size(), options.get_quad_stddev_lin(), options.get_quad_stddev_quad());
    cout << "Writing QuadraticFunction to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    cout << "done" << endl;
    break;
  }

  case 60: {
    NkLandscape function;
    function.random(options.get_bv_size(), options.get_nk_k(), options.get_nk_stddev());
    cout << "Writing NkLandscape to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    cout << "done" << endl;
    break;
  }

  case 70: {
    MaxSat function;
    if (options.with_ms_known_maximum()) {
      bit_vector_t solution(options.get_bv_size());
      bv_random(solution);
      function.random(solution,
                      options.get_ms_num_literals_per_clause(),
                      options.get_ms_num_clauses());
    } else {
      function.random(options.get_bv_size(),
                      options.get_ms_num_literals_per_clause(),
                      options.get_ms_num_clauses());
    }
    cout << "Writing MaxSat to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    function.save(ofs);
    cout << "done" << endl;
    break;
  }

  case 90: {
    EqualProducts function;
    function.random(options.get_bv_size(), options.get_ep_upper_bound());
    cout << "Writing EqualProducts to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    cout << "done" << endl;
    break;
  }

  default: {
    cerr << "Cannot create any function of type " << options.get_function() << endl;;
    exit(1);
  }

  }

  return 0;
}
