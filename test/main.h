// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2006-2008 Benoit Jacob <jacob.benoit.1@gmail.com>
// Copyright (C) 2008 Gael Guennebaud <g.gael@free.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>

#ifndef EIGEN_TEST_FUNC
#error EIGEN_TEST_FUNC must be defined
#endif

#define DEFAULT_REPEAT 10

namespace Eigen
{
  static std::vector<std::string> g_test_stack;
  static int g_repeat;
}

#define EI_PP_MAKE_STRING2(S) #S
#define EI_PP_MAKE_STRING(S) EI_PP_MAKE_STRING2(S)

#define EIGEN_DEFAULT_IO_FORMAT IOFormat(4, AlignCols, "  ", "\n", "", "", "", "")

#ifndef EIGEN_NO_ASSERTION_CHECKING

  namespace Eigen
  {
    static const bool should_raise_an_assert = false;

    // Used to avoid to raise two exceptions at a time in which
    // case the exception is not properly caught.
    // This may happen when a second exceptions is raise in a destructor.
    static bool no_more_assert = false;

    struct ei_assert_exception
    {
      ei_assert_exception(void) {}
      ~ei_assert_exception() { Eigen::no_more_assert = false; }
    };
  }

  // If EIGEN_DEBUG_ASSERTS is defined and if no assertion is raised while
  // one should have been, then the list of excecuted assertions is printed out.
  //
  // EIGEN_DEBUG_ASSERTS is not enabled by default as it
  // significantly increases the compilation time
  // and might even introduce side effects that would hide
  // some memory errors.
  #ifdef EIGEN_DEBUG_ASSERTS

    namespace Eigen
    {
      static bool ei_push_assert = false;
      static std::vector<std::string> ei_assert_list;
    }

    #define ei_assert(a)                       \
      if( (!(a)) && (!no_more_assert) )     \
      {                                     \
        Eigen::no_more_assert = true;       \
        throw Eigen::ei_assert_exception(); \
      }                                     \
      else if (Eigen::ei_push_assert)       \
      {                                     \
        ei_assert_list.push_back(std::string(EI_PP_MAKE_STRING(__FILE__)" ("EI_PP_MAKE_STRING(__LINE__)") : "#a) ); \
      }

    #define VERIFY_RAISES_ASSERT(a)                                               \
      {                                                                           \
        Eigen::no_more_assert = false;                                            \
        try {                                                                     \
          Eigen::ei_assert_list.clear();                                          \
          Eigen::ei_push_assert = true;                                           \
          a;                                                                      \
          Eigen::ei_push_assert = false;                                          \
          std::cerr << "One of the following asserts should have been raised:\n"; \
          for (uint ai=0 ; ai<ei_assert_list.size() ; ++ai)                       \
            std::cerr << "  " << ei_assert_list[ai] << "\n";                      \
          VERIFY(Eigen::should_raise_an_assert && # a);                           \
        } catch (Eigen::ei_assert_exception e) {                                  \
          Eigen::ei_push_assert = false; VERIFY(true);                            \
        }                                                                         \
      }

  #else // EIGEN_DEBUG_ASSERTS

    #define ei_assert(a) \
      if( (!(a)) && (!no_more_assert) )     \
      {                                     \
        Eigen::no_more_assert = true;       \
        throw Eigen::ei_assert_exception(); \
      }

    #define VERIFY_RAISES_ASSERT(a) {                             \
        Eigen::no_more_assert = false;                            \
        try { a; VERIFY(Eigen::should_raise_an_assert && # a); }  \
        catch (Eigen::ei_assert_exception e) { VERIFY(true); }    \
      }

  #endif // EIGEN_DEBUG_ASSERTS

  #define EIGEN_USE_CUSTOM_ASSERT

#else // EIGEN_NO_ASSERTION_CHECKING

  #define VERIFY_RAISES_ASSERT(a) {}

#endif // EIGEN_NO_ASSERTION_CHECKING


#define EIGEN_INTERNAL_DEBUGGING
#define EIGEN_NICE_RANDOM
#include <Eigen/Array>
#include <Eigen/QR> // required for createRandomMatrixOfRank


#define VERIFY(a) do { if (!(a)) { \
    std::cerr << "Test " << g_test_stack.back() << " failed in "EI_PP_MAKE_STRING(__FILE__) << " (" << EI_PP_MAKE_STRING(__LINE__) << ")" \
      << std::endl << "    " << EI_PP_MAKE_STRING(a) << std::endl << std::endl; \
    exit(2); \
  } } while (0)

#define VERIFY_IS_APPROX(a, b) VERIFY(test_ei_isApprox(a, b))
#define VERIFY_IS_NOT_APPROX(a, b) VERIFY(!test_ei_isApprox(a, b))
#define VERIFY_IS_MUCH_SMALLER_THAN(a, b) VERIFY(test_ei_isMuchSmallerThan(a, b))
#define VERIFY_IS_NOT_MUCH_SMALLER_THAN(a, b) VERIFY(!test_ei_isMuchSmallerThan(a, b))
#define VERIFY_IS_APPROX_OR_LESS_THAN(a, b) VERIFY(test_ei_isApproxOrLessThan(a, b))
#define VERIFY_IS_NOT_APPROX_OR_LESS_THAN(a, b) VERIFY(!test_ei_isApproxOrLessThan(a, b))

#define CALL_SUBTEST(FUNC) do { \
    g_test_stack.push_back(EI_PP_MAKE_STRING(FUNC)); \
    FUNC; \
    g_test_stack.pop_back(); \
  } while (0)

namespace Eigen {

template<typename T> inline typename NumTraits<T>::Real test_precision();
template<> inline int test_precision<int>() { return 0; }
template<> inline float test_precision<float>() { return 1e-3f; }
template<> inline double test_precision<double>() { return 1e-6; }
template<> inline float test_precision<std::complex<float> >() { return test_precision<float>(); }
template<> inline double test_precision<std::complex<double> >() { return test_precision<double>(); }
template<> inline long double test_precision<long double>() { return 1e-6; }

inline bool test_ei_isApprox(const int& a, const int& b)
{ return ei_isApprox(a, b, test_precision<int>()); }
inline bool test_ei_isMuchSmallerThan(const int& a, const int& b)
{ return ei_isMuchSmallerThan(a, b, test_precision<int>()); }
inline bool test_ei_isApproxOrLessThan(const int& a, const int& b)
{ return ei_isApproxOrLessThan(a, b, test_precision<int>()); }

inline bool test_ei_isApprox(const float& a, const float& b)
{ return ei_isApprox(a, b, test_precision<float>()); }
inline bool test_ei_isMuchSmallerThan(const float& a, const float& b)
{ return ei_isMuchSmallerThan(a, b, test_precision<float>()); }
inline bool test_ei_isApproxOrLessThan(const float& a, const float& b)
{ return ei_isApproxOrLessThan(a, b, test_precision<float>()); }

inline bool test_ei_isApprox(const double& a, const double& b)
{ return ei_isApprox(a, b, test_precision<double>()); }
inline bool test_ei_isMuchSmallerThan(const double& a, const double& b)
{ return ei_isMuchSmallerThan(a, b, test_precision<double>()); }
inline bool test_ei_isApproxOrLessThan(const double& a, const double& b)
{ return ei_isApproxOrLessThan(a, b, test_precision<double>()); }

inline bool test_ei_isApprox(const std::complex<float>& a, const std::complex<float>& b)
{ return ei_isApprox(a, b, test_precision<std::complex<float> >()); }
inline bool test_ei_isMuchSmallerThan(const std::complex<float>& a, const std::complex<float>& b)
{ return ei_isMuchSmallerThan(a, b, test_precision<std::complex<float> >()); }

inline bool test_ei_isApprox(const std::complex<double>& a, const std::complex<double>& b)
{ return ei_isApprox(a, b, test_precision<std::complex<double> >()); }
inline bool test_ei_isMuchSmallerThan(const std::complex<double>& a, const std::complex<double>& b)
{ return ei_isMuchSmallerThan(a, b, test_precision<std::complex<double> >()); }

inline bool test_ei_isApprox(const long double& a, const long double& b)
{ return ei_isApprox(a, b, test_precision<long double>()); }
inline bool test_ei_isMuchSmallerThan(const long double& a, const long double& b)
{ return ei_isMuchSmallerThan(a, b, test_precision<long double>()); }
inline bool test_ei_isApproxOrLessThan(const long double& a, const long double& b)
{ return ei_isApproxOrLessThan(a, b, test_precision<long double>()); }

template<typename Type1, typename Type2>
inline bool test_ei_isApprox(const Type1& a, const Type2& b)
{
  return a.isApprox(b, test_precision<typename Type1::Scalar>());
}

template<typename Derived1, typename Derived2>
inline bool test_ei_isMuchSmallerThan(const MatrixBase<Derived1>& m1,
                                   const MatrixBase<Derived2>& m2)
{
  return m1.isMuchSmallerThan(m2, test_precision<typename ei_traits<Derived1>::Scalar>());
}

template<typename Derived>
inline bool test_ei_isMuchSmallerThan(const MatrixBase<Derived>& m,
                                   const typename NumTraits<typename ei_traits<Derived>::Scalar>::Real& s)
{
  return m.isMuchSmallerThan(s, test_precision<typename ei_traits<Derived>::Scalar>());
}

template<typename Derived>
void createRandomMatrixOfRank(int desired_rank, int rows, int cols, Eigen::MatrixBase<Derived>& m)
{
  typedef Derived MatrixType;
  typedef typename ei_traits<MatrixType>::Scalar Scalar;
  typedef Matrix<Scalar, MatrixType::ColsAtCompileTime, 1> VectorType;

  MatrixType a = MatrixType::Random(rows,rows);
  MatrixType d = MatrixType::Identity(rows,cols);
  MatrixType  b = MatrixType::Random(cols,cols);

  // set the diagonal such that only desired_rank non-zero entries reamain
  const int diag_size = std::min(d.rows(),d.cols());
  d.diagonal().segment(desired_rank, diag_size-desired_rank) = VectorType::Zero(diag_size-desired_rank);

  QR<MatrixType> qra(a);
  QR<MatrixType> qrb(b);
  m = (qra.matrixQ() * d * qrb.matrixQ()).lazy();
}

} // end namespace Eigen

template<typename T> struct GetDifferentType;

template<> struct GetDifferentType<float> { typedef double type; };
template<> struct GetDifferentType<double> { typedef float type; };
template<typename T> struct GetDifferentType<std::complex<T> >
{ typedef std::complex<typename GetDifferentType<T>::type> type; };

// forward declaration of the main test function
void EIGEN_CAT(test_,EIGEN_TEST_FUNC)();

using namespace Eigen;

int main(int argc, char *argv[])
{
    bool has_set_repeat = false;
    bool has_set_seed = false;
    bool need_help = false;
    unsigned int seed = 0;
    int repeat = DEFAULT_REPEAT;

    for(int i = 1; i < argc; i++)
    {
      if(argv[i][0] == 'r')
      {
        if(has_set_repeat)
        {
          std::cout << "Argument " << argv[i] << " conflicting with a former argument" << std::endl;
          return 1;
        }
        repeat = atoi(argv[i]+1);
        has_set_repeat = true;
        if(repeat <= 0)
        {
          std::cout << "Invalid \'repeat\' value " << argv[i]+1 << std::endl;
          return 1;
        }
      }
      else if(argv[i][0] == 's')
      {
        if(has_set_seed)
        {
          std::cout << "Argument " << argv[i] << " conflicting with a former argument" << std::endl;
          return 1;
        }
        seed = int(strtoul(argv[i]+1, 0, 10));
        has_set_seed = true;
        bool ok = seed!=0;
        if(!ok)
        {
          std::cout << "Invalid \'seed\' value " << argv[i]+1 << std::endl;
          return 1;
        }
      }
      else
      {
        need_help = true;
      }
    }

    if(need_help)
    {
      std::cout << "This test application takes the following optional arguments:" << std::endl;
      std::cout << "  rN     Repeat each test N times (default: " << DEFAULT_REPEAT << ")" << std::endl;
      std::cout << "  sN     Use N as seed for random numbers (default: based on current time)" << std::endl;
      return 1;
    }

    if(!has_set_seed) seed = (unsigned int) time(NULL);
    if(!has_set_repeat) repeat = DEFAULT_REPEAT;

    std::cout << "Initializing random number generator with seed " << seed << std::endl;
    srand(seed);
    std::cout << "Repeating each test " << repeat << " times" << std::endl;

    Eigen::g_repeat = repeat;
    Eigen::g_test_stack.push_back(EI_PP_MAKE_STRING(EIGEN_TEST_FUNC));

    EIGEN_CAT(test_,EIGEN_TEST_FUNC)();
    return 0;
}



