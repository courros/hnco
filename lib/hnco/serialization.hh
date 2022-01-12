/* Copyright (C) 2016, 2017, 2018, 2019, 2020, 2021, 2022 Arnaud Berny

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

#ifndef HNCO_SERIALIZATION_H
#define HNCO_SERIALIZATION_H

#include <fstream>              // std::ifstream, std::ofstream
#include <string>               // std::string

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


namespace hnco {


/** @name Load from and save to boost archives
 */
///@{

/** Load from a boost archive.
    \param object Object to load
    \param path Path of the file
    \param name Class name
*/
template<typename T>
void load_from_archive(T& object, std::string path, std::string name)
{
  std::ifstream stream(path);
  if (!stream.good())
    throw std::runtime_error(name + "::load: Cannot open " + path);
  try {
    boost::archive::text_iarchive archive(stream);
    archive >> object;
  }
  catch (boost::archive::archive_exception& e) {
    throw std::runtime_error(name + "::load: " + std::string(e.what()));
  }
}

/** Save to a boost archive.
    \param object Object to save
    \param path Path of the file
    \param name Class name
*/
template<typename T>
void save_to_archive(const T& object, std::string path, std::string name)
{
  std::ofstream stream(path);
  if (!stream.good())
    throw std::runtime_error(name + "::save: Cannot open " + path);
  try {
    boost::archive::text_oarchive archive(stream);
    archive << object;
  }
  catch (boost::archive::archive_exception& e) {
    throw std::runtime_error(name + "::save: " + std::string(e.what()));
  }
}

///@}


} // end of namespace hnco


#endif
