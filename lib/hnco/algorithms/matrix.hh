/* Copyright (C) 2016, 2017, 2018 Arnaud Berny

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

#ifndef HNCO_ALGORITHMS_MATRIX_H
#define HNCO_ALGORITHMS_MATRIX_H

#include <vector>


namespace hnco {
namespace algorithm {


  /// Check for symmetric matrix
  template<class T>
  bool matrix_is_symmetric(const std::vector<std::vector<T> >& A)
  {
    for (std::size_t i = 0; i < A.size(); i++) {
      for (std::size_t j = 0; j < i; j++) {
        if (A[i][j] != A[j][i])
          return false;
      }
    }
    return true;
  }

  /// Check for strictly lower triangular matrix
  template<class T>
  bool matrix_is_strictly_lower_triangular(const std::vector<std::vector<T> >& A)
  {
    for (std::size_t i = 0; i < A.size(); i++) {
      for (std::size_t j = i; j < A.size(); j++) {
        if (A[i][j] != 0)
          return false;
      }
    }
    return true;
  }

  /// Check for diagonal elements
  template<class T>
  bool matrix_has_diagonal(const std::vector<std::vector<T> >& A, T x)
  {
    for (std::size_t i = 0; i < A.size(); i++) {
      if (A[i][i] != x)
        return false;
    }
    return true;
  }

  /// Check for element range
  template<class T>
  bool matrix_has_range(const std::vector<std::vector<T> >& A, T inf, T sup)
  {
    for (std::size_t i = 0; i < A.size(); i++) {
      for (std::size_t j = 0; j < i; j++) {
        if (A[i][j] < inf)
          return false;
        if (A[i][j] > sup)
          return false;
      }
    }
    return true;
  }

  /// Check for element range
  template<class T>
  bool matrix_has_dominant_diagonal(const std::vector<std::vector<T> >& A)
  {
    for (std::size_t i = 0; i < A.size(); i++) {
      for (std::size_t j = 0; j < i; j++) {
        if (A[i][j] > A[i][j])
          return false;
      }
    }
    return true;
  }


} // end of namespace algorithm
} // end of namespace hnco


#endif
