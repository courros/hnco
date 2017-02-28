#include <assert.h>

#include <iostream>

#include "hnco/exception.hh"

#include "factorization.hh"


using namespace hnco::function;
using namespace hnco::exception;


Factorization::Factorization(std::string path)
{
  mpz_init(_number);
  mpz_init(_first_factor);
  mpz_init(_second_factor);
  mpz_init(_product);

  std::ifstream stream(path);
  if (!stream.good()) {
    std::ostringstream oss;
    oss << "Factorization::Factorization: Cannot open " << path;
    throw Error(oss.str());
  }
  std::string decimal;
  stream >> decimal;

  if (mpz_set_str(_number, decimal.c_str(), 10) != 0) {
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


Factorization::~Factorization()
{
  mpz_clear(_number);
  mpz_clear(_first_factor);
  mpz_clear(_second_factor);
  mpz_clear(_product);
}


void
Factorization::convert(const bit_vector_t& x)
{
  assert(x.size() == _bv_size);
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
Factorization::eval(const bit_vector_t& x)
{
  convert(x);
  mpz_mul(_product, _first_factor, _second_factor);

  assert(mpz_sgn(_first_factor) >= 0);
  assert(mpz_sgn(_second_factor) >= 0);

  return -double(mpz_hamdist(_product, _number));
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
  stream << "Found " << _first_factor << " x " << _second_factor << " = " << _product
         << ", expected " << _number << std::endl;

}
