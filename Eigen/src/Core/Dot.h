// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2006-2008, 2010 Benoit Jacob <jacob.benoit.1@gmail.com>
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

#ifndef EIGEN_DOT_H
#define EIGEN_DOT_H

// helper function for dot(). The problem is that if we put that in the body of dot(), then upon calling dot
// with mismatched types, the compiler emits errors about failing to instantiate cwiseProduct BEFORE
// looking at the static assertions. Thus this is a trick to get better compile errors.
template<typename T, typename U,
// the NeedToTranspose condition here is taken straight from Assign.h
         bool NeedToTranspose = T::IsVectorAtCompileTime
                && U::IsVectorAtCompileTime
                && ((int(T::RowsAtCompileTime) == 1 && int(U::ColsAtCompileTime) == 1)
                      |  // FIXME | instead of || to please GCC 4.4.0 stupid warning "suggest parentheses around &&".
                         // revert to || as soon as not needed anymore.
                    (int(T::ColsAtCompileTime) == 1 && int(U::RowsAtCompileTime) == 1))
>
struct ei_dot_nocheck
{
  static inline typename ei_traits<T>::Scalar run(const MatrixBase<T>& a, const MatrixBase<U>& b)
  {
    return a.conjugate().cwiseProduct(b).sum();
  }
};

template<typename T, typename U>
struct ei_dot_nocheck<T, U, true>
{
  static inline typename ei_traits<T>::Scalar run(const MatrixBase<T>& a, const MatrixBase<U>& b)
  {
    return a.adjoint().cwiseProduct(b).sum();
  }
};

/** \returns the dot product of *this with other.
  *
  * \only_for_vectors
  *
  * \note If the scalar type is complex numbers, then this function returns the hermitian
  * (sesquilinear) dot product, conjugate-linear in the first variable and linear in the
  * second variable.
  *
  * \sa squaredNorm(), norm()
  */
template<typename Derived>
template<typename OtherDerived>
typename ei_traits<Derived>::Scalar
MatrixBase<Derived>::dot(const MatrixBase<OtherDerived>& other) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(OtherDerived)
  EIGEN_STATIC_ASSERT_SAME_VECTOR_SIZE(Derived,OtherDerived)
  EIGEN_STATIC_ASSERT((ei_is_same_type<Scalar, typename OtherDerived::Scalar>::ret),
    YOU_MIXED_DIFFERENT_NUMERIC_TYPES__YOU_NEED_TO_USE_THE_CAST_METHOD_OF_MATRIXBASE_TO_CAST_NUMERIC_TYPES_EXPLICITLY)

  ei_assert(size() == other.size());

  return ei_dot_nocheck<Derived,OtherDerived>::run(*this, other);
}

/** \returns the squared \em l2 norm of *this, i.e., for vectors, the dot product of *this with itself.
  *
  * \sa dot(), norm()
  */
template<typename Derived>
inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real MatrixBase<Derived>::squaredNorm() const
{
  return ei_real((*this).cwiseAbs2().sum());
}

/** \returns the \em l2 norm of *this, i.e., for vectors, the square root of the dot product of *this with itself.
  *
  * \sa dot(), squaredNorm()
  */
template<typename Derived>
inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real MatrixBase<Derived>::norm() const
{
  return ei_sqrt(squaredNorm());
}

/** \returns an expression of the quotient of *this by its own norm.
  *
  * \only_for_vectors
  *
  * \sa norm(), normalize()
  */
template<typename Derived>
inline const typename MatrixBase<Derived>::PlainObject
MatrixBase<Derived>::normalized() const
{
  typedef typename ei_nested<Derived>::type Nested;
  typedef typename ei_unref<Nested>::type _Nested;
  _Nested n(derived());
  return n / n.norm();
}

/** Normalizes the vector, i.e. divides it by its own norm.
  *
  * \only_for_vectors
  *
  * \sa norm(), normalized()
  */
template<typename Derived>
inline void MatrixBase<Derived>::normalize()
{
  *this /= norm();
}

/** \returns true if *this is approximately orthogonal to \a other,
  *          within the precision given by \a prec.
  *
  * Example: \include MatrixBase_isOrthogonal.cpp
  * Output: \verbinclude MatrixBase_isOrthogonal.out
  */
template<typename Derived>
template<typename OtherDerived>
bool MatrixBase<Derived>::isOrthogonal
(const MatrixBase<OtherDerived>& other, RealScalar prec) const
{
  typename ei_nested<Derived,2>::type nested(derived());
  typename ei_nested<OtherDerived,2>::type otherNested(other.derived());
  return ei_abs2(nested.dot(otherNested)) <= prec * prec * nested.squaredNorm() * otherNested.squaredNorm();
}

/** \returns true if *this is approximately an unitary matrix,
  *          within the precision given by \a prec. In the case where the \a Scalar
  *          type is real numbers, a unitary matrix is an orthogonal matrix, whence the name.
  *
  * \note This can be used to check whether a family of vectors forms an orthonormal basis.
  *       Indeed, \c m.isUnitary() returns true if and only if the columns (equivalently, the rows) of m form an
  *       orthonormal basis.
  *
  * Example: \include MatrixBase_isUnitary.cpp
  * Output: \verbinclude MatrixBase_isUnitary.out
  */
template<typename Derived>
bool MatrixBase<Derived>::isUnitary(RealScalar prec) const
{
  typename Derived::Nested nested(derived());
  for(int i = 0; i < cols(); ++i)
  {
    if(!ei_isApprox(nested.col(i).squaredNorm(), static_cast<RealScalar>(1), prec))
      return false;
    for(int j = 0; j < i; ++j)
      if(!ei_isMuchSmallerThan(nested.col(i).dot(nested.col(j)), static_cast<Scalar>(1), prec))
        return false;
  }
  return true;
}
#endif // EIGEN_DOT_H
