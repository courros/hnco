/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022, 2023, 2024, 2025, 2026 Arnaud Berny

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
#include <fstream>

#include "hnco/exception.hh"
#include "hnco/maps/map.hh"
#include "hnco/random.hh"
#include "hnco/util.hh"         // hnco::fail_with

#include "mapgen-options.hh"

using namespace hnco::app;
using namespace hnco::map;
using namespace hnco::random;
using namespace hnco;

void generate_map(MapgenOptions& options)
{
  switch(options.get_map()) {

  case 1: {
    if (options.get_output_size() != options.get_input_size())
      std::cerr << "Warning: generate_map: Translation: input size and output size are different, using input size" << std::endl;
    Translation map;
    map.random(options.get_input_size());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing translation to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  case 2: {
    if (options.get_output_size() != options.get_input_size())
      std::cerr << "Warning: generate_map: Permutation: input size and output size are different, using input size" << std::endl;
    Permutation map;
    map.random(options.get_input_size());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing permutation to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  case 3: {
    if (options.get_output_size() != options.get_input_size())
      std::cerr << "Warning: generate_map: Composition of permutation and translation: input size and output size are different, using input size" << std::endl;
    Permutation permutation;
    Translation translation;
    permutation.random(options.get_input_size());
    translation.random(options.get_input_size());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing permutation and translation to " << options.get_path() << " ... ";
    archive << permutation << translation;
    std::cout << "done" << std::endl;
    break;
  }

  case 4: {
    LinearMap map;
    map.random(options.get_output_size(), options.get_input_size(), options.with_surjective());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing linear map to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  case 5: {
    AffineMap map;
    map.random(options.get_output_size(), options.get_input_size(), options.with_surjective());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing affine map to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  case 6: {
    if (options.get_output_size() != options.get_input_size())
      std::cerr << "Warning: generate_map: Affine map (transvection sequence): input size and output size are different, using input size" << std::endl;
    TsAffineMap map;
    map.random(options.get_input_size(),
               options.get_ts_length(),
               options.get_ts_sampling_mode());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing affine map (transvection sequence) to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  default: {
    fail_with("Cannot create any map of type ", options.get_map());
  }

  }
}


int main(int argc, char *argv[])
{
  MapgenOptions options(argc, argv);

  //
  // Seed random number generator
  //

  if (options.with_seed())
    Generator::set_seed(options.get_seed());
  else
    Generator::set_seed();

  try {
    generate_map(options);
  }
  catch (const std::runtime_error& e) {
    std::cerr << "main: std::runtime_error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
