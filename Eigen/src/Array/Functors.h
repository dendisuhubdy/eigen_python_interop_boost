// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
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

#ifndef EIGEN_ARRAY_FUNCTORS_H
#define EIGEN_ARRAY_FUNCTORS_H

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to compute the square root of a scalar
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwiseSqrt()
  */
template<typename Scalar> struct ei_scalar_sqrt_op EIGEN_EMPTY_STRUCT {
  inline const Scalar operator() (const Scalar& a) const { return ei_sqrt(a); }
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_sqrt_op<Scalar> >
{ enum { Cost = 5 * NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to compute the exponential of a scalar
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwiseExp()
  */
template<typename Scalar> struct ei_scalar_exp_op EIGEN_EMPTY_STRUCT {
  inline const Scalar operator() (const Scalar& a) const { return ei_exp(a); }
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_exp_op<Scalar> >
{ enum { Cost = 5 * NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to compute the logarithm of a scalar
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwiseLog()
  */
template<typename Scalar> struct ei_scalar_log_op EIGEN_EMPTY_STRUCT {
  inline const Scalar operator() (const Scalar& a) const { return ei_log(a); }
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_log_op<Scalar> >
{ enum { Cost = 5 * NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to compute the cosine of a scalar
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwiseCos()
  */
template<typename Scalar> struct ei_scalar_cos_op EIGEN_EMPTY_STRUCT {
  inline const Scalar operator() (const Scalar& a) const { return ei_cos(a); }
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_cos_op<Scalar> >
{ enum { Cost = 5 * NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to compute the sine of a scalar
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwiseSin()
  */
template<typename Scalar> struct ei_scalar_sin_op EIGEN_EMPTY_STRUCT {
  inline const Scalar operator() (const Scalar& a) const { return ei_sin(a); }
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_sin_op<Scalar> >
{ enum { Cost = 5 * NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to raise a scalar to a power
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwisePow
  */
template<typename Scalar>
struct ei_scalar_pow_op {
  inline ei_scalar_pow_op(const Scalar& exponent) : m_exponent(exponent) {}
  inline Scalar operator() (const Scalar& a) const { return ei_pow(a, m_exponent); }
  const Scalar m_exponent;
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_pow_op<Scalar> >
{ enum { Cost = 5 * NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

/** \internal
  *
  * \array_module
  * 
  * \brief Template functor to compute the reciprocal of a scalar
  *
  * \sa class CwiseUnaryOp, MatrixBase::cwiseInverse
  */
template<typename Scalar>
struct ei_scalar_inverse_op {
  inline Scalar operator() (const Scalar& a) const { return Scalar(1)/a; }
};
template<typename Scalar>
struct ei_functor_traits<ei_scalar_inverse_op<Scalar> >
{ enum { Cost = NumTraits<Scalar>::MulCost, IsVectorizable = false }; };

// default ei_functor_traits for STL functors:

template<typename T>
struct ei_functor_traits<std::multiplies<T> >
{ enum { Cost = NumTraits<T>::MulCost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::divides<T> >
{ enum { Cost = NumTraits<T>::MulCost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::plus<T> >
{ enum { Cost = NumTraits<T>::AddCost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::minus<T> >
{ enum { Cost = NumTraits<T>::AddCost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::negate<T> >
{ enum { Cost = NumTraits<T>::AddCost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::logical_or<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::logical_and<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::logical_not<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::greater<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::less<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::greater_equal<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::less_equal<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::equal_to<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::not_equal_to<T> >
{ enum { Cost = 1, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::binder2nd<T> >
{ enum { Cost = ei_functor_traits<T>::Cost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::binder1st<T> >
{ enum { Cost = ei_functor_traits<T>::Cost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::unary_negate<T> >
{ enum { Cost = 1 + ei_functor_traits<T>::Cost, IsVectorizable = false }; };

template<typename T>
struct ei_functor_traits<std::binary_negate<T> >
{ enum { Cost = 1 + ei_functor_traits<T>::Cost, IsVectorizable = false }; };

#ifdef EIGEN_STDEXT_SUPPORT

template<typename T0,typename T1>
struct ei_functor_traits<std::project1st<T0,T1> >
{ enum { Cost = 0, IsVectorizable = false }; };

template<typename T0,typename T1>
struct ei_functor_traits<std::project2nd<T0,T1> >
{ enum { Cost = 0, IsVectorizable = false }; };

template<typename T0,typename T1>
struct ei_functor_traits<std::select2nd<std::pair<T0,T1> > >
{ enum { Cost = 0, IsVectorizable = false }; };

template<typename T0,typename T1>
struct ei_functor_traits<std::select1st<std::pair<T0,T1> > >
{ enum { Cost = 0, IsVectorizable = false }; };

template<typename T0,typename T1>
struct ei_functor_traits<std::unary_compose<T0,T1> >
{ enum { Cost = ei_functor_traits<T0>::Cost + ei_functor_traits<T1>::Cost, IsVectorizable = false }; };

template<typename T0,typename T1,typename T2>
struct ei_functor_traits<std::binary_compose<T0,T1,T2> >
{ enum { Cost = ei_functor_traits<T0>::Cost + ei_functor_traits<T1>::Cost + ei_functor_traits<T2>::Cost, IsVectorizable = false }; };

#endif // EIGEN_STDEXT_SUPPORT

#endif // EIGEN_ARRAY_FUNCTORS_H