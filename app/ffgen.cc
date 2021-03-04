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
using namespace hnco::app;


template<class T>
void save_function(const T& function, const std::string class_name, const std::string path)
{
  std::cout << "Writing " << class_name << " to " << path << " ... ";
  std::ofstream ofs(path);
  if (!ofs.good()) {
    std::ostringstream stream;
    stream << "save_function: Cannot open " << path;
    throw Error(stream.str());
  }
  function.save(ofs);
  std::cout << "done" << std::endl;
}

template<class T>
void save_function_to_boost_archive(const T& function, const std::string name, const std::string path)
{
  std::cout << "Writing " << name << " to " << path << " ... ";
  std::ofstream ofs(path);
  if (!ofs.good()) {
    std::ostringstream stream;
    stream << "save_function_to_boost_archive: Cannot open " << path;
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

void generate_linear_function(FfgenOptions& options)
{
  LinearFunction function;

  switch (options.get_lin_generator()) {

  case 0: {
    double stddev = options.get_stddev();
    auto generator = [stddev]() { return stddev * Generator::normal(); };
    function.generate(options.get_bv_size(), generator);
    break;
  }

  case 1: {
    double w = options.get_lin_initial_weight();
    double d = options.get_lin_distance();
    function.generate(options.get_bv_size(), [w, d] () mutable { double result = w; w += d; return result; });
    break;
  }

  case 2: {
    double w = options.get_lin_initial_weight();
    double r = options.get_lin_ratio();
    function.generate(options.get_bv_size(), [w, r] () mutable { double result = w; w *= r; return result; });
    break;
  }

  default:
    std::ostringstream stream;
    stream << "generate_linear_function: unknown generator: " << options.get_lin_generator();
    throw Error(stream.str());

  }

  save_function_to_boost_archive(function, "LinearFunction", options.get_path());
}


void generate_walsh_expansion_2(FfgenOptions& options)
{
  double stddev = options.get_stddev();
  auto generator = [stddev]() { return stddev * Generator::normal(); };
  WalshExpansion2 function;

  switch (options.get_walsh2_generator()) {

  case 0:
    function.generate(options.get_bv_size(), generator, generator);
    break;

  case 1:
    function.generate_ising1_long_range(options.get_bv_size(), options.get_walsh2_ising_alpha());
    break;

  default:
    std::ostringstream stream;
    stream << "generate_walsh_expansion_2: unknown generator: " << options.get_walsh2_generator();
    throw Error(stream.str());

  }

  save_function_to_boost_archive(function, "WalshExpansion2", options.get_path());
}


void generate_nearest_neighbor_ising_model_1(FfgenOptions& options)
{
  auto generator = [&options]() { return options.get_stddev() * Generator::normal(); };
  NearestNeighborIsingModel1 function;

  switch (options.get_nn1_generator()) {

  case 0:
    function.generate(options.get_bv_size(),
                      generator,
                      generator);
    break;

  case 1:
    function.generate(options.get_bv_size(),
                      generator,
                      [&options](){ return options.get_field_constant(); });
    break;

  case 2:
    function.generate(options.get_bv_size(),
                      [&options](){ return options.get_coupling_constant(); },
                      generator);
    break;

  case 3:
    function.generate(options.get_bv_size(),
                      [&options](){ return options.get_coupling_constant(); },
                      [&options](){ return options.get_field_constant(); });
    break;

  default:
    std::ostringstream stream;
    stream << "generate_nearest_neighbor_ising_model_1: unknown generator: " << options.get_nn1_generator();
    throw Error(stream.str());

  }

  function.set_periodic_boundary_conditions(options.with_periodic_boundary_conditions());
  save_function_to_boost_archive(function, "NearestNeighborIsingModel1", options.get_path());
}


void generate_nearest_neighbor_ising_model_2(FfgenOptions& options)
{
  auto generator = [&options]() { return options.get_stddev() * Generator::normal(); };
  NearestNeighborIsingModel2 function;

  switch (options.get_nn2_generator()) {

  case 0:
    function.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      generator,
                      generator);
    break;

  case 1:
    function.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      generator,
                      [&options](){ return options.get_field_constant(); });
    break;

  case 2:
    function.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      [&options](){ return options.get_coupling_constant(); },
                      generator);
    break;

  case 3:
    function.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      [&options](){ return options.get_coupling_constant(); },
                      [&options](){ return options.get_field_constant(); });
    break;

  default:
    std::ostringstream stream;
    stream << "generate_nearest_neighbor_ising_model_2: unknown generator: " << options.get_nn2_generator();
    throw Error(stream.str());

  }

  if (options.get_bv_size() != int(function.get_bv_size()))
    std::cerr
      << "Warning: bv_size changed from " << options.get_bv_size()
      << " to " << function.get_bv_size() << std::endl;

  function.set_periodic_boundary_conditions(options.with_periodic_boundary_conditions());
  save_function_to_boost_archive(function, "NearestNeighborIsingModel2", options.get_path());
}


void generate_function(FfgenOptions& options)
{
  double stddev = options.get_stddev();
  auto generator = [stddev]() { return stddev * Generator::normal(); };

  switch(options.get_function()) {

  case 1:
    generate_linear_function(options);
    break;

  case 60: {
    NkLandscape function;
    function.generate(options.get_bv_size(), options.get_nk_k(), generator);
    save_function_to_boost_archive(function, "NkLandscape", options.get_path());
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
    save_function(function, "MaxSat", options.get_path());
    break;
  }

  case 90: {
    EqualProducts function;
    double upper_bound = options.get_ep_upper_bound();
    function.generate(options.get_bv_size(), [upper_bound]() { return upper_bound * Generator::uniform(); });
    save_function_to_boost_archive(function, "EqualProducts", options.get_path());
    break;
  }

  case 91: {
    Partition function;
    function.random(options.get_bv_size(), options.get_part_upper_bound());
    save_function_to_boost_archive(function, "Partition", options.get_path());
    break;
  }

  case 160: {
    WalshExpansion function;
    function.generate(options.get_bv_size(), options.get_walsh_num_features(), generator);
    save_function_to_boost_archive(function, "WalshExpansion", options.get_path());
    break;
  }

  case 161: {
    WalshExpansion1 function;
    function.generate(options.get_bv_size(), generator);
    save_function_to_boost_archive(function, "WalshExpansion1", options.get_path());
    break;
  }

  case 162:
    generate_walsh_expansion_2(options);
    break;

  case 171:
    generate_nearest_neighbor_ising_model_1(options);
    break;

  case 172:
    generate_nearest_neighbor_ising_model_2(options);
    break;

  case 190: {
    Sudoku function;
    function.random(options.get_sudoku_num_empty_cells());
    save_function(function, "Sudoku", options.get_path());
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
  FfgenOptions options(argc, argv);

  //
  // Seed random number generator
  //

  if (options.set_seed())
    Generator::set_seed(options.get_seed());
  else
    Generator::set_seed();

  try {
    generate_function(options);
  }
  catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
