// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2009-2010 Gael Guennebaud <g.gael@free.fr>
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

#ifndef EIGEN_FORCEALIGNEDACCESS_H
#define EIGEN_FORCEALIGNEDACCESS_H

/** \class ForceAlignedAccess
  *
  * \brief Enforce aligned packet loads and stores regardless of what is requested
  *
  * \param ExpressionType the type of the object of which we are forcing aligned packet access
  *
  * This class is the return type of MatrixBase::forceAlignedAccess()
  * and most of the time this is the only way it is used.
  *
  * \sa MatrixBase::forceAlignedAccess()
  */
template<typename ExpressionType>
struct ei_traits<ForceAlignedAccess<ExpressionType> > : public ei_traits<ExpressionType>
{};

template<typename ExpressionType> class ForceAlignedAccess
  : public ei_dense_xpr_base< ForceAlignedAccess<ExpressionType> >::type
{
  public:

    typedef typename ei_dense_xpr_base<ForceAlignedAccess>::type Base;
    EIGEN_DENSE_PUBLIC_INTERFACE(ForceAlignedAccess)

    inline ForceAlignedAccess(const ExpressionType& matrix) : m_expression(matrix) {}

    inline int rows() const { return m_expression.rows(); }
    inline int cols() const { return m_expression.cols(); }
    inline int outerStride() const { return m_expression.outerStride(); }
    inline int innerStride() const { return m_expression.innerStride(); }

    inline const CoeffReturnType coeff(int row, int col) const
    {
      return m_expression.coeff(row, col);
    }

    inline Scalar& coeffRef(int row, int col)
    {
      return m_expression.const_cast_derived().coeffRef(row, col);
    }

    inline const CoeffReturnType coeff(int index) const
    {
      return m_expression.coeff(index);
    }

    inline Scalar& coeffRef(int index)
    {
      return m_expression.const_cast_derived().coeffRef(index);
    }

    template<int LoadMode>
    inline const PacketScalar packet(int row, int col) const
    {
      return m_expression.template packet<Aligned>(row, col);
    }

    template<int LoadMode>
    inline void writePacket(int row, int col, const PacketScalar& x)
    {
      m_expression.const_cast_derived().template writePacket<Aligned>(row, col, x);
    }

    template<int LoadMode>
    inline const PacketScalar packet(int index) const
    {
      return m_expression.template packet<Aligned>(index);
    }

    template<int LoadMode>
    inline void writePacket(int index, const PacketScalar& x)
    {
      m_expression.const_cast_derived().template writePacket<Aligned>(index, x);
    }

    operator const ExpressionType&() const { return m_expression; }

  protected:
    const ExpressionType& m_expression;

  private:
    ForceAlignedAccess& operator=(const ForceAlignedAccess&);
};

/** \returns an expression of *this with forced aligned access
  * \sa forceAlignedAccessIf(),class ForceAlignedAccess
  */
template<typename Derived>
inline const ForceAlignedAccess<Derived>
MatrixBase<Derived>::forceAlignedAccess() const
{
  return ForceAlignedAccess<Derived>(derived());
}

/** \returns an expression of *this with forced aligned access
  * \sa forceAlignedAccessIf(), class ForceAlignedAccess
  */
template<typename Derived>
inline ForceAlignedAccess<Derived>
MatrixBase<Derived>::forceAlignedAccess()
{
  return ForceAlignedAccess<Derived>(derived());
}

/** \returns an expression of *this with forced aligned access if \a Enable is true.
  * \sa forceAlignedAccess(), class ForceAlignedAccess
  */
template<typename Derived>
template<bool Enable>
inline const typename ei_meta_if<Enable,ForceAlignedAccess<Derived>,Derived&>::ret
MatrixBase<Derived>::forceAlignedAccessIf() const
{
  return derived();
}

/** \returns an expression of *this with forced aligned access if \a Enable is true.
  * \sa forceAlignedAccess(), class ForceAlignedAccess
  */
template<typename Derived>
template<bool Enable>
inline typename ei_meta_if<Enable,ForceAlignedAccess<Derived>,Derived&>::ret
MatrixBase<Derived>::forceAlignedAccessIf()
{
  return derived();
}

#endif // EIGEN_FORCEALIGNEDACCESS_H
