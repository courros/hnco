#include <assert.h>

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "hnco/exception.hh"

#include "factorization.hh"


using namespace hnco::function;
using namespace hnco::exception;


void
Factorization::init()
{
  mpz_init(_number);
  mpz_init(_first_factor);
  mpz_init(_second_factor);
  mpz_init(_product);
}


void
Factorization::clear()
{
  mpz_clear(_number);
  mpz_clear(_first_factor);
  mpz_clear(_second_factor);
  mpz_clear(_product);
}


void
Factorization::set_number(const std::string number)
{
  if (mpz_set_str(_number, number.c_str(), 10) != 0) {
    std::ostringstream oss;
    oss << "Factorization::Factorization: mpz_set_str failed";
    throw Error(oss.str());
  }

  _number_size = mpz_sizeinbase(_number, 2);

  if (_number_size % 2 == 0)
    _first_factor_size = _number_size / 2;
  else
    _first_factor_size = _number_size / 2 + 1;

  _second_factor_size = _number_size - 1;

  _bv_size = _first_factor_size + _second_factor_size;

  _first_factor_string = std::string(_first_factor_size, '0');
  _second_factor_string = std::string(_second_factor_size, '0');
}


void
Factorization::convert(const bit_vector_t& x)
{
  assert(int(x.size()) == _bv_size);
  assert(x.size() == _first_factor_size + _second_factor_size);

  for (size_t i = 0; i < _first_factor_size; i++)
    if (x[i])
      _first_factor_string[i] = '1';
    else
      _first_factor_string[i] = '0';
  mpz_set_str(_first_factor, _first_factor_string.c_str(), 2);

  for (size_t i = 0; i < _second_factor_size; i++)
    if (x[_first_factor_size + i])
      _second_factor_string[i] = '1';
    else
      _second_factor_string[i] = '0';
  mpz_set_str(_second_factor, _second_factor_string.c_str(), 2);

}


double
Factorization::evaluate(const bit_vector_t& x)
{
  convert(x);

  assert(mpz_sgn(_first_factor) >= 0);
  assert(mpz_sgn(_second_factor) >= 0);

  mpz_mul(_product, _first_factor, _second_factor);

  size_t product_size = mpz_sizeinbase(_product, 2);
  auto bounds = std::minmax(_number_size, product_size);
  double result = 0;
  for (size_t i = 0; i < bounds.first; i++) {
    if (mpz_tstbit(_product, i) != mpz_tstbit(_number, i))
      result += i + 1;
  }
  if (bounds.second == product_size) {
    for (size_t i = bounds.first; i < bounds.second; i++) {
      if (mpz_tstbit(_product, i))
        result += i + 1;
    }
  } else {
    for (size_t i = bounds.first; i < bounds.second; i++) {
      if (mpz_tstbit(_number, i))
        result += i + 1;
    }
  }
  return -result;
}


void
Factorization::display(std::ostream& stream)
{
  stream << "Factorize " << _number << std::endl;
}


void
Factorization::describe(const bit_vector_t& x, std::ostream& stream)
{
  convert(x);
  mpz_mul(_product, _first_factor, _second_factor);
  stream << "Found " << _first_factor << " (a) x " << _second_factor << " (b) = " << _product
         << " (c), expected " << _number << " (n)" << std::endl;

  mpz_class first_factor(_first_factor);
  std::string first_factor_str = first_factor.get_str(2);
  stream << "a = " << std::setw(_bv_size) << first_factor_str << std::endl;

  mpz_class second_factor(_second_factor);
  std::string second_factor_str = second_factor.get_str(2);
  stream << "b = " << std::setw(_bv_size) << second_factor_str << std::endl;

  mpz_class product(_product);
  std::string product_str = product.get_str(2);
  stream << "c = " << std::setw(_bv_size) << product_str << std::endl;

  mpz_class number(_number);
  std::string number_str = number.get_str(2);
  stream << "n = " << std::setw(_bv_size) << number_str << std::endl;

  stream << "Hamming distance = " << mpz_hamdist(_product, _number) << std::endl;
}
