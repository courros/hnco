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
#include <fstream>

#include "hnco/exception.hh"
#include "hnco/maps/map.hh"
#include "hnco/random.hh"

#include "mapgen-options.hh"


using namespace hnco::exception;
using namespace hnco::random;
using namespace hnco;
using namespace hnco::app;


void generate_map(MapgenOptions& options)
{
  switch(options.get_map()) {

  case 1: {
    Translation map;
    map.random(options.get_output_size());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing translation to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  case 2: {
    Permutation map;
    map.random(options.get_output_size());
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing permutation to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  case 3: {
    Permutation permutation;
    Translation translation;
    permutation.random(options.get_output_size());
    translation.random(options.get_output_size());
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
    TsAffineMap map;
    if (options.get_input_size() != options.get_output_size())
      std::cerr << "Only input size is taken into account" << std::endl;
    map.random(options.get_input_size(),
               options.get_ts_length(),
               static_cast<TsAffineMap::SamplingMode>(options.get_ts_sampling_mode()));
    std::ofstream stream(options.get_path());
    boost::archive::text_oarchive archive(stream);
    std::cout << "Writing affine map (transvection sequence) to " << options.get_path() << " ... ";
    archive << map;
    std::cout << "done" << std::endl;
    break;
  }

  default: {
    std::cerr << "Cannot create any map of type " << options.get_map() << std::endl;
    exit(1);
  }

  }
}


int main(int argc, char *argv[])
{
  MapgenOptions options(argc, argv);

  //
  // Seed random number generator
  //

  if (options.set_seed())
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
