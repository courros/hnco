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

#include "hnco/exception.hh"
#include "hnco/map.hh"
#include "hnco/random.hh"

#include "mapgen-options.hh"


using namespace hnco::exception;
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
  Random::generator.seed(options.get_seed());

  switch(options.get_map()) {

  case 1: {
    Translation map;
    map.random(options.get_output_size());
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    cout << "Writing translation to " << options.get_path() << " ... ";
    oa << map;
    cout << "done" << endl;
    break;
  }

  case 2: {
    Permutation map;
    map.random(options.get_output_size());
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    cout << "Writing permutation to " << options.get_path() << " ... ";
    oa << map;
    cout << "done" << endl;
    break;
  }

  case 3: {
    Permutation permutation;
    Translation translation;
    permutation.random(options.get_output_size());
    translation.random(options.get_output_size());
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    cout << "Writing permutation and translation to " << options.get_path() << " ... ";
    oa << permutation << translation;
    cout << "done" << endl;
    break;
  }

  case 4: {
    LinearMap map;
    map.random(options.get_output_size(), options.get_input_size(), options.with_surjective());
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    cout << "Writing linear map to " << options.get_path() << " ... ";
    oa << map;
    cout << "done" << endl;
    break;
  }

  case 5: {
    AffineMap map;
    map.random(options.get_output_size(), options.get_input_size(), options.with_surjective());
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    cout << "Writing affine map to " << options.get_path() << " ... ";
    oa << map;
    cout << "done" << endl;
    break;
  }

  case 6: {
    GlElementAffineMap map;
    if (options.get_input_size() != options.get_output_size())
      cerr << "Only input size is taken into account" << endl;
    map.random(options.get_input_size(), options.get_sequence_length());
    std::ofstream ofs(options.get_path());
    boost::archive::text_oarchive oa(ofs);
    cout << "Writing affine map (product of transvections) to " << options.get_path() << " ... ";
    oa << map;
    cout << "done" << endl;
    break;
  }

  default: {
    cerr << "Cannot create any map of type " << options.get_map() << endl;
    exit(1);
  }

  }

  return 0;
}
