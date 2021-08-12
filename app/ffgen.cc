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


void generate_linear_function(FfgenOptions& options)
{
  LinearFunction instance;

  switch (options.get_lin_generator()) {

  case 0: {
    double stddev = options.get_stddev();
    auto generator = [stddev]() { return stddev * Generator::normal(); };
    instance.generate(options.get_bv_size(), generator);
    break;
  }

  case 1: {
    double w = options.get_lin_initial_weight();
    double d = options.get_lin_distance();
    instance.generate(options.get_bv_size(), [w, d] () mutable { double result = w; w += d; return result; });
    break;
  }

  case 2: {
    double w = options.get_lin_initial_weight();
    double r = options.get_lin_ratio();
    instance.generate(options.get_bv_size(), [w, r] () mutable { double result = w; w *= r; return result; });
    break;
  }

  default:
    throw std::runtime_error("generate_linear_function: unknown generator: " + std::to_string(options.get_lin_generator()));
  }

  std::cout << "Writing LinearFunction to " << options.get_path() << " ... ";
  instance.save(options.get_path());
  std::cout << "done" << std::endl;
}


void generate_walsh_expansion_2(FfgenOptions& options)
{
  double stddev = options.get_stddev();
  auto generator = [stddev]() { return stddev * Generator::normal(); };
  WalshExpansion2 instance;

  switch (options.get_walsh2_generator()) {

  case 0:
    instance.generate(options.get_bv_size(), generator, generator);
    break;

  case 1:
    instance.generate_ising1_long_range(options.get_bv_size(), options.get_walsh2_ising_alpha());
    break;

  default:
    throw std::runtime_error("generate_walsh_expansion_2: unknown generator: " + std::to_string(options.get_walsh2_generator()));
  }

  std::cout << "Writing WalshExpansion2 to " << options.get_path() << " ... ";
  instance.save(options.get_path());
  std::cout << "done" << std::endl;
}


void generate_nearest_neighbor_ising_model_1(FfgenOptions& options)
{
  auto generator = [&options]() { return options.get_stddev() * Generator::normal(); };
  NearestNeighborIsingModel1 instance;

  switch (options.get_nn1_generator()) {

  case 0:
    instance.generate(options.get_bv_size(),
                      generator,
                      generator);
    break;

  case 1:
    instance.generate(options.get_bv_size(),
                      generator,
                      [&options](){ return options.get_field_constant(); });
    break;

  case 2:
    instance.generate(options.get_bv_size(),
                      [&options](){ return options.get_coupling_constant(); },
                      generator);
    break;

  case 3:
    instance.generate(options.get_bv_size(),
                      [&options](){ return options.get_coupling_constant(); },
                      [&options](){ return options.get_field_constant(); });
    break;

  default:
    throw std::runtime_error("generate_nearest_neighbor_ising_model_1: unknown generator: " + std::to_string(options.get_nn1_generator()));
  }

  instance.set_periodic_boundary_conditions(options.with_periodic_boundary_conditions());
  std::cout << "Writing NearestNeighborIsingModel1 to " << options.get_path() << " ... ";
  instance.save(options.get_path());
  std::cout << "done" << std::endl;
}


void generate_nearest_neighbor_ising_model_2(FfgenOptions& options)
{
  auto generator = [&options]() { return options.get_stddev() * Generator::normal(); };
  NearestNeighborIsingModel2 instance;

  switch (options.get_nn2_generator()) {

  case 0:
    instance.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      generator,
                      generator);
    break;

  case 1:
    instance.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      generator,
                      [&options](){ return options.get_field_constant(); });
    break;

  case 2:
    instance.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      [&options](){ return options.get_coupling_constant(); },
                      generator);
    break;

  case 3:
    instance.generate(options.get_nn2_num_rows(),
                      options.get_nn2_num_columns(),
                      [&options](){ return options.get_coupling_constant(); },
                      [&options](){ return options.get_field_constant(); });
    break;

  default:
    throw std::runtime_error("generate_nearest_neighbor_ising_model_2: unknown generator: " + std::to_string(options.get_nn2_generator()));
  }

  if (options.get_bv_size() != int(instance.get_bv_size()))
    std::cerr
      << "Warning: bv_size changed from " << options.get_bv_size()
      << " to " << instance.get_bv_size() << std::endl;

  instance.set_periodic_boundary_conditions(options.with_periodic_boundary_conditions());
  std::cout << "Writing NearestNeighborIsingModel2 to " << options.get_path() << " ... ";
  instance.save(options.get_path());
  std::cout << "done" << std::endl;
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
    NkLandscape instance;
    instance.generate(options.get_bv_size(), options.get_nk_k(), generator);
    std::cout << "Writing NkLandscape to " << options.get_path() << " ... ";
    instance.save(options.get_path());
    std::cout << "done" << std::endl;
    break;
  }

  case 70: {
    MaxSat instance;
    if (options.with_ms_planted_solution()) {
      bit_vector_t solution(options.get_bv_size());
      bv_random(solution);
      instance.random(solution,
                      options.get_ms_num_literals_per_clause(),
                      options.get_ms_num_clauses());
    } else {
      instance.random(options.get_bv_size(),
                      options.get_ms_num_literals_per_clause(),
                      options.get_ms_num_clauses());
    }
    std::cout << "Writing MaxSat to " << options.get_path() << " ... ";
    instance.save(options.get_path());
    std::cout << "done" << std::endl;
    break;
  }

  case 90: {
    EqualProducts instance;
    double upper_bound = options.get_ep_upper_bound();
    instance.generate(options.get_bv_size(), [upper_bound]() { return upper_bound * Generator::uniform(); });
    std::cout << "Writing EqualProducts to " << options.get_path() << " ... ";
    instance.save(options.get_path());
    std::cout << "done" << std::endl;
    break;
  }

  case 91: {
    Partition instance;
    instance.random(options.get_bv_size(), options.get_part_upper_bound());
    std::cout << "Writing Partition to " << options.get_path() << " ... ";
    instance.save(options.get_path());
    std::cout << "done" << std::endl;
    break;
  }

  case 160: {
    WalshExpansion instance;
    instance.generate(options.get_bv_size(), options.get_walsh_num_features(), generator);
    std::cout << "Writing WalshExpansion to " << options.get_path() << " ... ";
    instance.save(options.get_path());
    std::cout << "done" << std::endl;
    break;
  }

  case 161: {
    WalshExpansion1 instance;
    instance.generate(options.get_bv_size(), generator);
    std::cout << "Writing WalshExpansion1 to " << options.get_path() << " ... ";
    instance.save(options.get_path());
    std::cout << "done" << std::endl;
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
    Sudoku instance;
    instance.random(options.get_sudoku_num_empty_cells());
    instance.save(options.get_path());
    break;
  }

  default:
    throw std::runtime_error("Cannot create any function of type " + std::to_string(options.get_function()));
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
  catch (const std::runtime_error& e) {
    std::cerr << "main: std::runtime_error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
