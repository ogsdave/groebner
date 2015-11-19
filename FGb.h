#ifndef FGB_H
#define FGB_H

#include <vector>

#define LIBMODE 2

#include "gmp.h"
extern "C" {
#include "call_fgb.h"
}

#include "style.h"
#include "Polynomial.h"

template<class E = char, uint VC = 5>
struct FGbRunner {
  typedef Polynomial<Term<int, Monomial<E, VC, degrevlex> > > P;
  typedef Monomial<E, VC, degrevlex> M;
  static const uint max_ideal_size = 1000;

  std::vector<Dpol> convertInput(const std::vector<P>& input) {
    Dpol input_basis[input.size()];
    size_t max_mon = 0;
    for (auto p : input) max_mon = std::max(max_mon, p.size());
    I("max_mon = " << max_mon);
    for (typename std::vector<P>::size_type i = 0; i < input.size(); ++i) {
      I32 exp[max_mon][M::VAR_COUNT];
      for (uint j = 0; j < max_mon; ++j) {
        for (uint k = 0; k < M::VAR_COUNT; ++k) {
          exp[j][k] = 0;
        }
      }
      input_basis[i] = FGB(creat_poly)(input[i].size());
      uint j = 0;
      for (auto term : input[i]) {
        for (uint k = 0; k < M::VAR_COUNT; ++k) exp[j][k] = term.m()[k];
        FGB(set_expos2)(input_basis[i], j, exp[j], M::VAR_COUNT);
        mpz_t u;
        mpz_init_set_si(u, term.c());
        FGB(set_coeff_gmp)(input_basis[i], j, u);
        ++j;
      }
      FGB(full_sort_poly2)(input_basis[i]);
    }
    return std::vector<Dpol>(input_basis, input_basis + input.size());
  }

  std::vector<Dpol> run(std::vector<Dpol>& input_basis) {
    SFGB_Options options;
    FGb_set_default_options(&options);
    options._env._index = 30000000;
    options._verb = 1;

    double cputime;
    std::vector<Dpol> output_basis(max_ideal_size);

    int output_count = FGB(fgb)(input_basis.data(), input_basis.size(), output_basis.data(), max_ideal_size, &cputime, &options);
    output_basis.resize(output_count);
    return output_basis;
  }

  std::vector<P> convertOutput(const std::vector<Dpol>& output_basis) {
    std::vector<P> output(output_basis.size());

    for (std::vector<Dpol>::size_type i = 0; i < output_basis.size(); ++i) {
      output[i];
      auto term_count = FGB(nb_terms)(output_basis[i]);
      std::vector<I32> exponents(M::VAR_COUNT * term_count);
      std::vector<mpz_ptr> coefficients(M::VAR_COUNT * term_count);
      FGB(export_poly_INT_gmp2)(M::VAR_COUNT, term_count, coefficients.data(), exponents.data(), output_basis[i]);
      for (I32 j = 0; j < term_count; ++j) {
        M monomial;
        for (uint k = 0; k < M::VAR_COUNT; ++k) {
          monomial[k] = exponents[j * M::VAR_COUNT + k];
        }
//        output[i] += typename P::TermType(mpz_class(coefficients[j]), monomial);
      }
    }

    return output;
  }

  std::vector<P> fgb(const std::vector<P>& input) {
    FGB(saveptr)();
    init_FGb_Integers();
    std::vector<std::string> var_names_as_string(M::VAR_COUNT);
    std::vector<char*> var_names_as_char(M::VAR_COUNT);
    for (uint i = 0; i < M::VAR_COUNT; ++i) {
      var_names_as_string[i] = get_var_name(i);
      var_names_as_char[i] = const_cast<char*>(var_names_as_string[i].c_str());
    }
    FGB(PowerSet)(M::VAR_COUNT, 0, var_names_as_char.data());
    threads_FGb(16);

    auto input_basis = convertInput(input);
    auto output_basis = run(input_basis);

    return convertOutput(output_basis);
  }
};

#endif // FGB_H