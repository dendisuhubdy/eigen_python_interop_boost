// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2010 Gael Guennebaud <g.gael@free.fr>
// Copyright (C) 2010 Benoit Jacob <jacob.benoit.1@gmail.com>
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

#ifndef EIGEN_GLOBAL_FUNCTIONS_H
#define EIGEN_GLOBAL_FUNCTIONS_H

#define EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(NAME,FUNCTOR) \
  template<typename Derived> \
  inline const Eigen::CwiseUnaryOp<Eigen::FUNCTOR<typename Derived::Scalar>, Derived> \
  NAME(const Eigen::ArrayBase<Derived>& x) { \
    return x.derived(); \
  }

#define EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(NAME,FUNCTOR) \
  \
  template<typename Derived> \
  struct NAME##_retval<ArrayBase<Derived> > \
  { \
    typedef const Eigen::CwiseUnaryOp<Eigen::FUNCTOR<typename Derived::Scalar>, Derived> type; \
  }; \
  template<typename Derived> \
  struct NAME##_impl<ArrayBase<Derived> > \
  { \
    static inline typename NAME##_retval<ArrayBase<Derived> >::type run(const Eigen::ArrayBase<Derived>& x) \
    { \
      return x.derived(); \
    } \
  };


namespace std
{
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(real,ei_scalar_real_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(imag,ei_scalar_imag_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(sin,ei_scalar_sin_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(cos,ei_scalar_cos_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(exp,ei_scalar_exp_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(log,ei_scalar_log_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(abs,ei_scalar_abs_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_STD_UNARY(sqrt,ei_scalar_sqrt_op)
}

namespace Eigen
{
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_real,ei_scalar_real_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_imag,ei_scalar_imag_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_sin,ei_scalar_sin_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_cos,ei_scalar_cos_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_exp,ei_scalar_exp_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_log,ei_scalar_log_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_abs,ei_scalar_abs_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_abs2,ei_scalar_abs2_op)
  EIGEN_ARRAY_DECLARE_GLOBAL_EIGEN_UNARY(ei_sqrt,ei_scalar_sqrt_op)
}

// TODO: cleanly disable those functions that are not suppored on Array (ei_real_ref, ei_random, ei_isApprox...)

#endif // EIGEN_GLOBAL_FUNCTIONS_H
