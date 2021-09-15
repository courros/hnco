#ifndef HNCO_FUNCTIONS_COLLECTION_FACTORIZATION_H
#define HNCO_FUNCTIONS_COLLECTION_FACTORIZATION_H

#include <gmp.h>
#include <gmpxx.h>

#include <string>
#include <vector>
#include <fstream>

#include "hnco/functions/function.hh"


namespace hnco {
namespace function {


/** %Factorization.

    Reference:

    Torbjörn Granlund and the GMP development team. 2012. GNU MP:
    The GNU Multiple Precision Arithmetic Library (5.0.5
    ed.).

    http://gmplib.org/.

*/
class Factorization: public Function {

  /// Number to factorize
  mpz_t _number;

  /// First factor
  mpz_t _first_factor;

  /// Second factor
  mpz_t _second_factor;

  /// Product
  mpz_t _product;

  /// First factor in binary form
  std::string _first_factor_string;

  /// Secon factor in binary form
  std::string _second_factor_string;

  /// Number size in bits
  size_t _number_size;

  /// First factor size in bits
  size_t _first_factor_size;

  /// Second factor size in bits
  size_t _second_factor_size;

  /// Bit vector size
  int _bv_size;

  /// Init GMP data structures
  void init();

  /// Clear GMP data structures
  void clear();

  /// Set number
  void set_number(const std::string number);

  /// Convert a bit vector into two numbers
  void convert(const bit_vector_t& x);

public:

  /// Constructor
  Factorization() { init(); }

  /** Constructor.

      \param number Number to factorize written in decimal form
  */
  Factorization(const std::string number) {
    init();
    set_number(number);
  }

  /// Destructor
  ~Factorization() { clear(); }

  /** @name Load and save instance
   */
  ///@{

  /** Load instance.

      The file referenced by the path is a text file which contains
      exactly one natural number written in base 10 without any space

      \param path Path of the instance to load
      \throw std::runtime_error
  */
  void load(std::string path) {
    std::ifstream stream(path);
    if (!stream.good())
      throw std::runtime_error("Factorization::load: Cannot open " + path);
    std::string number;
    stream >> number;
    set_number(number);
  }

  ///@}

  /// Get bit vector size
  int get_bv_size() const override { return _bv_size; }

  /// Evaluate a bit vector
  double evaluate(const bit_vector_t&) override;

  /// Display
  void display(std::ostream& stream) const override;

  /// Describe a bit vector
  void describe(const bit_vector_t& x, std::ostream& stream) override;

};


} // end of namespace function
} // end of namespace hnco


#endif
