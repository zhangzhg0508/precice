#pragma once

#ifdef NDEBUG

#define PRECICE_ASSERT(...) \
  {                         \
  }

#else

#include <cassert>
#include <iostream>

#include <boost/current_function.hpp>
#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/seq/seq.hpp> // SEQ_TAIL
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/variadic/elem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "Parallel.hpp"
#include "stacktrace.hpp"

/// Inclusion of this library is required for keywords 'or'/'and' to be used when compiling with MSVC.
#include <ciso646>

/// Helper macro, used by assertion.
#define PRECICE_PRINT_ARGUMENT(r, data, i, elem) \
  BOOST_PP_IF(BOOST_PP_IS_EMPTY(elem), void, std::cerr << "  Argument " << i << ": " << elem << '\n');

/// Asserts that expr evaluates to true, prints all other arguments and calls assert(false).
#define PRECICE_ASSERT(check, ...)                                                                              \
  if (!(check)) {                                                                                                \
    std::cerr << "ASSERTION FAILED\n"                                                                            \
              << "  Location:          " << BOOST_CURRENT_FUNCTION << '\n'                                       \
              << "  File:              " << __FILE__ << ":" << __LINE__ << '\n'                                  \
              << "  Rank:              " << precice::utils::Parallel::getProcessRank() << '\n'                   \
              << "  Failed expression: " << BOOST_PP_STRINGIZE(check)                                            \
              << '\n';                                                                                           \
    BOOST_PP_SEQ_FOR_EACH_I(PRECICE_PRINT_ARGUMENT, , BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__));                    \
    std::cerr << getStacktrace() << '\n';                                                                        \
    std::cerr.flush();                                                                                           \
    std::cout.flush();                                                                                           \
    assert(false);                                                                                               \
  }

#endif
