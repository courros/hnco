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

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

#include "hnco/bit-vector.hh"
#include "hnco/exception.hh"
#include "hnco/functions/all.hh"
#include "hnco/random.hh"

#include "ffgen-options.hh"


using namespace hnco::exception;
using namespace hnco::function;
using namespace hnco::random;
using namespace hnco;


int generate_function(Options& options)
{
  double stddev = options.get_stddev();
  auto generator = [stddev]() { return stddev * Random::normal(); };

  switch(options.get_function()) {

  case 1: {
    LinearFunction function;
    function.random(options.get_bv_size(), generator);
    std::cout << "Writing LinearFunction to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
    break;
  }

  case 60: {
    NkLandscape function;
    function.random(options.get_bv_size(), options.get_nk_k(), options.get_stddev());
    std::cout << "Writing NkLandscape to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
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
    std::cout << "Writing MaxSat to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    function.save(ofs);
    std::cout << "done" << std::endl;
    break;
  }

  case 90: {
    EqualProducts function;
    function.random(options.get_bv_size(), options.get_ep_upper_bound());
    std::cout << "Writing EqualProducts to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
    break;
  }

  case 160: {
    WalshExpansion function;
    function.random(options.get_bv_size(), options.get_walsh_num_features(), options.get_stddev());
    std::cout << "Writing WalshExpansion to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
    break;
  }

  case 161: {
    WalshExpansion1 function;
    function.random(options.get_bv_size(), generator);
    std::cout << "Writing WalshExpansion1 to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
    break;
  }

  case 162: {
    WalshExpansion2 function;
    function.random(options.get_bv_size(), options.get_stddev(), options.get_stddev());
    std::cout << "Writing WalshExpansion2 to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
    break;
  }

  case 171: {
    NearestNeighborIsingModel1 function;
    function.random(options.get_bv_size(), generator, generator);
    std::cout << "Writing NearestNeighborIsingModel1 to " << options.get_path() << " ... ";
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    oa << function;
    std::cout << "done" << std::endl;
    break;
  }

  default: {
    std::cerr << "Cannot create any function of type " << options.get_function() << std::endl;;
    return 1;
  }

  }

  return 0;
}


int main(int argc, char *argv[])
{
  Options options(argc, argv);

  // Initialize random number generator
  if (!options.set_seed()) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    options.set_seed(seed);
  }
  Random::generator.seed(options.get_seed());

  try {
    return generate_function(options);
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

}
