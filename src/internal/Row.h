// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2007 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along
// with Eigen; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
//
// As a special exception, if other files instantiate templates or use macros
// or functions from this file, or you compile this file and link it
// with other works to produce a work based on this file, this file does not
// by itself cause the resulting work to be covered by the GNU General Public
// License. This exception does not invalidate any other reasons why a work
// based on this file might be covered by the GNU General Public License.

#ifndef EI_ROW_H
#define EI_ROW_H

template<typename MatrixType> class EiRow
  : public EiObject<typename MatrixType::Scalar, EiRow<MatrixType> >
{
  public:
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::Ref MatRef;
    friend class EiObject<Scalar, EiRow<MatrixType> >;

    static const int RowsAtCompileTime = 1,
                     ColsAtCompileTime = MatrixType::ColsAtCompileTime;

    EiRow(const MatRef& matrix, int row)
      : m_matrix(matrix), m_row(row)
    {
      EI_CHECK_ROW_RANGE(matrix, row);
    }
    
    EiRow(const EiRow& other)
      : m_matrix(other.m_matrix), m_row(other.m_row) {}
    
    template<typename OtherDerived>
    EiRow& operator=(const EiObject<Scalar, OtherDerived>& other)
    {
      return EiObject<Scalar, EiRow<MatrixType> >::operator=(other);
    }
    
    EI_INHERIT_ASSIGNMENT_OPERATORS(EiRow)
    
  private:
    EiRow& _ref() { return *this; }
    const EiRow& _constRef() const { return *this; }
    
    int _rows() const { return 1; }
    int _cols() const { return m_matrix.cols(); }
    
    Scalar& _write(int row, int col)
    {
      EI_UNUSED(row);
      return m_matrix.write(m_row, col);
    }
    
    Scalar _read(int row, int col) const
    {
      EI_UNUSED(row);
      return m_matrix.read(m_row, col);
    }
    
  protected:
    MatRef m_matrix;
    const int m_row;
};

template<typename Scalar, typename Derived>
EiRow<Derived>
EiObject<Scalar, Derived>::row(int i)
{
  return EiRow<Derived>(static_cast<Derived*>(this)->ref(), i);
}

#endif // EI_ROW_H