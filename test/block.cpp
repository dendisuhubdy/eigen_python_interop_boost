// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2006-2010 Benoit Jacob <jacob.benoit.1@gmail.com>
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

template<typename MatrixType> void block(const MatrixType& m)
{
  typedef typename MatrixType::Scalar Scalar;
  typedef typename MatrixType::RealScalar RealScalar;
  typedef Matrix<Scalar, MatrixType::RowsAtCompileTime, 1> VectorType;
  typedef Matrix<Scalar, 1, MatrixType::ColsAtCompileTime> RowVectorType;
  typedef Matrix<Scalar, Dynamic, Dynamic> DynamicMatrixType;
  typedef Matrix<Scalar, Dynamic, 1> DynamicVectorType;
  
  int rows = m.rows();
  int cols = m.cols();

  MatrixType m1 = MatrixType::Random(rows, cols),
             m2 = MatrixType::Random(rows, cols),
             m3(rows, cols),
             mzero = MatrixType::Zero(rows, cols),
             ones = MatrixType::Ones(rows, cols);
  VectorType v1 = VectorType::Random(rows),
             v2 = VectorType::Random(rows),
             v3 = VectorType::Random(rows),
             vzero = VectorType::Zero(rows);

  Scalar s1 = ei_random<Scalar>();

  int r1 = ei_random<int>(0,rows-1);
  int r2 = ei_random<int>(r1,rows-1);
  int c1 = ei_random<int>(0,cols-1);
  int c2 = ei_random<int>(c1,cols-1);

  //check row() and col()
  VERIFY_IS_EQUAL(m1.col(c1).transpose(), m1.transpose().row(c1));
  //check operator(), both constant and non-constant, on row() and col()
  m1.row(r1) += s1 * m1.row(r2);
  m1.col(c1) += s1 * m1.col(c2);

  //check block()
  Matrix<Scalar,Dynamic,Dynamic> b1(1,1); b1(0,0) = m1(r1,c1);

  RowVectorType br1(m1.block(r1,0,1,cols));
  VectorType bc1(m1.block(0,c1,rows,1));
  VERIFY_IS_EQUAL(b1, m1.block(r1,c1,1,1));
  VERIFY_IS_EQUAL(m1.row(r1), br1);
  VERIFY_IS_EQUAL(m1.col(c1), bc1);
  //check operator(), both constant and non-constant, on block()
  m1.block(r1,c1,r2-r1+1,c2-c1+1) = s1 * m2.block(0, 0, r2-r1+1,c2-c1+1);
  m1.block(r1,c1,r2-r1+1,c2-c1+1)(r2-r1,c2-c1) = m2.block(0, 0, r2-r1+1,c2-c1+1)(0,0);

  const int BlockRows = EIGEN_ENUM_MIN(MatrixType::RowsAtCompileTime,2);
  const int BlockCols = EIGEN_ENUM_MIN(MatrixType::ColsAtCompileTime,5);
  if (rows>=5 && cols>=8)
  {
    // test fixed block() as lvalue
    m1.template block<BlockRows,BlockCols>(1,1) *= s1;
    // test operator() on fixed block() both as constant and non-constant
    m1.template block<BlockRows,BlockCols>(1,1)(0, 3) = m1.template block<2,5>(1,1)(1,2);
    // check that fixed block() and block() agree
    Matrix<Scalar,Dynamic,Dynamic> b = m1.template block<BlockRows,BlockCols>(3,3);
    VERIFY_IS_EQUAL(b, m1.block(3,3,BlockRows,BlockCols));
  }

  if (rows>2)
  {
    // test sub vectors
    VERIFY_IS_EQUAL(v1.template head<2>(), v1.block(0,0,2,1));
    VERIFY_IS_EQUAL(v1.template head<2>(), v1.head(2));
    VERIFY_IS_EQUAL(v1.template head<2>(), v1.segment(0,2));
    VERIFY_IS_EQUAL(v1.template head<2>(), v1.template segment<2>(0));
    int i = rows-2;
    VERIFY_IS_EQUAL(v1.template tail<2>(), v1.block(i,0,2,1));
    VERIFY_IS_EQUAL(v1.template tail<2>(), v1.tail(2));
    VERIFY_IS_EQUAL(v1.template tail<2>(), v1.segment(i,2));
    VERIFY_IS_EQUAL(v1.template tail<2>(), v1.template segment<2>(i));
    i = ei_random(0,rows-2);
    VERIFY_IS_EQUAL(v1.segment(i,2), v1.template segment<2>(i));
  }

  // stress some basic stuffs with block matrices
  VERIFY(ei_real(ones.col(c1).sum()) == RealScalar(rows));
  VERIFY(ei_real(ones.row(r1).sum()) == RealScalar(cols));

  VERIFY(ei_real(ones.col(c1).dot(ones.col(c2))) == RealScalar(rows));
  VERIFY(ei_real(ones.row(r1).dot(ones.row(r2))) == RealScalar(cols));

  // now test some block-inside-of-block.
  
  // expressions with direct access
  VERIFY_IS_EQUAL( (m1.block(r1,c1,rows-r1,cols-c1).block(r2-r1,c2-c1,rows-r2,cols-c2)) , (m1.block(r2,c2,rows-r2,cols-c2)) );
  VERIFY_IS_EQUAL( (m1.block(r1,c1,r2-r1+1,c2-c1+1).row(0)) , (m1.row(r1).segment(c1,c2-c1+1)) );
  VERIFY_IS_EQUAL( (m1.block(r1,c1,r2-r1+1,c2-c1+1).col(0)) , (m1.col(c1).segment(r1,r2-r1+1)) );
  VERIFY_IS_EQUAL( (m1.block(r1,c1,r2-r1+1,c2-c1+1).transpose().col(0)) , (m1.row(r1).segment(c1,c2-c1+1)) );
  VERIFY_IS_EQUAL( (m1.transpose().block(c1,r1,c2-c1+1,r2-r1+1).col(0)) , (m1.row(r1).segment(c1,c2-c1+1)) );

  // expressions without direct access
  VERIFY_IS_EQUAL( ((m1+m2).block(r1,c1,rows-r1,cols-c1).block(r2-r1,c2-c1,rows-r2,cols-c2)) , ((m1+m2).block(r2,c2,rows-r2,cols-c2)) );
  VERIFY_IS_EQUAL( ((m1+m2).block(r1,c1,r2-r1+1,c2-c1+1).row(0)) , ((m1+m2).row(r1).segment(c1,c2-c1+1)) );
  VERIFY_IS_EQUAL( ((m1+m2).block(r1,c1,r2-r1+1,c2-c1+1).col(0)) , ((m1+m2).col(c1).segment(r1,r2-r1+1)) );
  VERIFY_IS_EQUAL( ((m1+m2).block(r1,c1,r2-r1+1,c2-c1+1).transpose().col(0)) , ((m1+m2).row(r1).segment(c1,c2-c1+1)) );
  VERIFY_IS_EQUAL( ((m1+m2).transpose().block(c1,r1,c2-c1+1,r2-r1+1).col(0)) , ((m1+m2).row(r1).segment(c1,c2-c1+1)) );

  // evaluation into plain matrices from expressions with direct access (stress MapBase)
  DynamicMatrixType dm;
  DynamicVectorType dv;
  dm.setZero();
  dm = m1.block(r1,c1,rows-r1,cols-c1).block(r2-r1,c2-c1,rows-r2,cols-c2);
  VERIFY_IS_EQUAL(dm, (m1.block(r2,c2,rows-r2,cols-c2)));
  dm.setZero();
  dv.setZero();
  dm = m1.block(r1,c1,r2-r1+1,c2-c1+1).row(0).transpose();
  dv = m1.row(r1).segment(c1,c2-c1+1);
  VERIFY_IS_EQUAL(dv, dm);
  dm.setZero();
  dv.setZero();
  dm = m1.col(c1).segment(r1,r2-r1+1);
  dv = m1.block(r1,c1,r2-r1+1,c2-c1+1).col(0);
  VERIFY_IS_EQUAL(dv, dm);
  dm.setZero();
  dv.setZero();
  dm = m1.block(r1,c1,r2-r1+1,c2-c1+1).transpose().col(0);
  dv = m1.row(r1).segment(c1,c2-c1+1);
  VERIFY_IS_EQUAL(dv, dm);
  dm.setZero();
  dv.setZero();
  dm = m1.row(r1).segment(c1,c2-c1+1).transpose();
  dv = m1.transpose().block(c1,r1,c2-c1+1,r2-r1+1).col(0);
  VERIFY_IS_EQUAL(dv, dm);
}


template<typename MatrixType>
void compare_using_data_and_stride(const MatrixType& m)
{
  int rows = m.rows();
  int cols = m.cols();
  int size = m.size();
  int innerStride = m.innerStride();
  int outerStride = m.outerStride();
  int rowStride = m.rowStride();
  int colStride = m.colStride();
  const typename MatrixType::Scalar* data = m.data();

  for(int j=0;j<cols;++j)
    for(int i=0;i<rows;++i)
      VERIFY(m.coeff(i,j) == data[i*rowStride + j*colStride]);

  if(!MatrixType::IsVectorAtCompileTime)
  {
    for(int j=0;j<cols;++j)
      for(int i=0;i<rows;++i)
        VERIFY(m.coeff(i,j) == data[(MatrixType::Flags&RowMajorBit)
                                     ? i*outerStride + j*innerStride
                                     : j*outerStride + i*innerStride]);
  }

  if(MatrixType::IsVectorAtCompileTime)
  {
    VERIFY(innerStride == int((&m.coeff(1))-(&m.coeff(0))));
    for (int i=0;i<size;++i)
      VERIFY(m.coeff(i) == data[i*innerStride]);
  }
}

template<typename MatrixType>
void data_and_stride(const MatrixType& m)
{
  int rows = m.rows();
  int cols = m.cols();

  int r1 = ei_random<int>(0,rows-1);
  int r2 = ei_random<int>(r1,rows-1);
  int c1 = ei_random<int>(0,cols-1);
  int c2 = ei_random<int>(c1,cols-1);

  MatrixType m1 = MatrixType::Random(rows, cols);
  compare_using_data_and_stride(m1.block(r1, c1, r2-r1+1, c2-c1+1));
  compare_using_data_and_stride(m1.transpose().block(c1, r1, c2-c1+1, r2-r1+1));
  compare_using_data_and_stride(m1.row(r1));
  compare_using_data_and_stride(m1.col(c1));
  compare_using_data_and_stride(m1.row(r1).transpose());
  compare_using_data_and_stride(m1.col(c1).transpose());
}

void test_block()
{
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST_1( block(Matrix<float, 1, 1>()) );
    CALL_SUBTEST_2( block(Matrix4d()) );
    CALL_SUBTEST_3( block(MatrixXcf(3, 3)) );
    CALL_SUBTEST_4( block(MatrixXi(8, 12)) );
    CALL_SUBTEST_5( block(MatrixXcd(20, 20)) );
    CALL_SUBTEST_6( block(MatrixXf(20, 20)) );

    CALL_SUBTEST_8( block(Matrix<float,Dynamic,4>(3, 4)) );

#ifndef EIGEN_DEFAULT_TO_ROW_MAJOR
    CALL_SUBTEST_6( data_and_stride(MatrixXf(ei_random(5,50), ei_random(5,50))) );
    CALL_SUBTEST_7( data_and_stride(Matrix<int,Dynamic,Dynamic,RowMajor>(ei_random(5,50), ei_random(5,50))) );
#endif
  }
}
