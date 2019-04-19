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


template<class T>
void save_function_to_boost_archive(const T& function, const std::string name, const Options& options)
{
  std::cout << "Writing " << name << " to " << options.get_path() << " ... ";
  std::ofstream ofs(options.get_path());
  if (!ofs.good()) {
    std::ostringstream stream;
    stream << "save_function_to_boost_archive: Cannot open " << options.get_path();
    throw Error(stream.str());
  }
  try {
    boost::archive::text_oarchive oa(ofs);
    oa << function;
  }
  catch (boost::archive::archive_exception& e) {
    throw Error(e.what());
  }
  std::cout << "done" << std::endl;
}


void generate_function(Options& options)
{
  double stddev = options.get_stddev();

  auto generator = [stddev]() { return stddev * Random::normal(); };

  switch(options.get_function()) {

  case 1: {
    LinearFunction function;
    function.random(options.get_bv_size(), generator);
    save_function_to_boost_archive(function, "LinearFunction", options);
    break;
  }

  case 60: {
    NkLandscape function;
    function.random(options.get_bv_size(), options.get_nk_k(), generator);
    save_function_to_boost_archive(function, "NkLandscape", options);
    break;
  }

  case 70: {
    MaxSat function;
    if (options.with_ms_planted_solution()) {
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
    double upper_bound = options.get_ep_upper_bound();
    auto generator = [upper_bound]() { return upper_bound * Random::uniform(); };
    EqualProducts function;
    function.random(options.get_bv_size(), generator);
    save_function_to_boost_archive(function, "EqualProducts", options);
    break;
  }

  case 160: {
    WalshExpansion function;
    function.random(options.get_bv_size(), options.get_walsh_num_features(), options.get_stddev());
    save_function_to_boost_archive(function, "WalshExpansion", options);
    break;
  }

  case 161: {
    WalshExpansion1 function;
    function.random(options.get_bv_size(), generator);
    save_function_to_boost_archive(function, "WalshExpansion1", options);
    break;
  }

  case 162: {
    WalshExpansion2 function;
    function.random(options.get_bv_size(), generator, generator);
    save_function_to_boost_archive(function, "WalshExpansion2", options);
    break;
  }

  case 171: {
    NearestNeighborIsingModel1 function;
    function.random(options.get_bv_size(), generator, generator);
    function.set_periodic_boundary_conditions(options.with_ising_periodic_boundary_condition());
    save_function_to_boost_archive(function, "NearestNeighborIsingModel1", options);
    break;
  }

  case 172: {
    NearestNeighborIsingModel2 function;
    function.random(options.get_ising_num_rows(), options.get_ising_num_columns(), generator, generator);
    function.set_periodic_boundary_conditions(options.with_ising_periodic_boundary_condition());
    save_function_to_boost_archive(function, "NearestNeighborIsingModel2", options);
    if (options.get_bv_size() != int(function.get_bv_size()))
      std::cerr
        << "Warning: bv_size changed from " << options.get_bv_size()
        << " to " << function.get_bv_size() << std::endl;
    break;
  }

  default:
    std::ostringstream stream;
    stream << "Cannot create any function of type " << options.get_function();
    throw Error(stream.str());

  }

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
    generate_function(options);
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
