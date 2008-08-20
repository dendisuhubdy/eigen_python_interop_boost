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

#include "main.h"
#include <Eigen/SVD>

template<typename MatrixType> void svd(const MatrixType& m)
{
  /* this test covers the following files:
     SVD.h
  */
  int rows = m.rows();
  int cols = m.cols();

  typedef typename MatrixType::Scalar Scalar;
  MatrixType a = MatrixType::Random(rows,cols);
  Matrix<Scalar, MatrixType::RowsAtCompileTime, 1> b =
    Matrix<Scalar, MatrixType::RowsAtCompileTime, 1>::Random(rows,1);
  Matrix<Scalar, MatrixType::ColsAtCompileTime, 1> x(cols,1), x2(cols,1);

  SVD<MatrixType> svd(a);
  MatrixType sigma = MatrixType::Zero(rows,cols);
  MatrixType matU  = MatrixType::Zero(rows,rows);
  sigma.block(0,0,cols,cols) = svd.singularValues().asDiagonal();
  matU.block(0,0,rows,cols) = svd.matrixU();

  VERIFY_IS_APPROX(a, matU * sigma * svd.matrixV().transpose());

  if (rows==cols)
  {
    svd.solve(b, &x);
    VERIFY_IS_APPROX(a * x, b);
  }
}

void test_svd()
{
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST( svd(Matrix3f()) );
    CALL_SUBTEST( svd(Matrix4d()) );
    CALL_SUBTEST( svd(MatrixXf(7,7)) );
    CALL_SUBTEST( svd(MatrixXf(14,7)) );
    // complex are not implemented yet
//     CALL_SUBTEST( svd(MatrixXcd(6,6)) );
//     CALL_SUBTEST( svd(MatrixXcf(3,3)) );
  }
}