// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
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

#ifndef EIGEN_SELFADJOINTEIGENSOLVER_H
#define EIGEN_SELFADJOINTEIGENSOLVER_H

/** \eigenvalues_module \ingroup Eigenvalues_Module
  * \nonstableyet
  *
  * \class SelfAdjointEigenSolver
  *
  * \brief Eigen values/vectors solver for selfadjoint matrix
  *
  * \param MatrixType the type of the matrix of which we are computing the eigen decomposition
  *
  * \note MatrixType must be an actual Matrix type, it can't be an expression type.
  *
  * \sa MatrixBase::eigenvalues(), class EigenSolver
  */
template<typename _MatrixType> class SelfAdjointEigenSolver
{
  public:

    typedef _MatrixType MatrixType;
    enum {
      Size = MatrixType::RowsAtCompileTime,
      ColsAtCompileTime = MatrixType::ColsAtCompileTime,
      Options = MatrixType::Options,
      MaxColsAtCompileTime = MatrixType::MaxColsAtCompileTime
    };
    typedef typename MatrixType::Scalar Scalar;
    typedef typename NumTraits<Scalar>::Real RealScalar;
    typedef std::complex<RealScalar> Complex;
    typedef typename ei_plain_col_type<MatrixType, RealScalar>::type RealVectorType;
    typedef Tridiagonalization<MatrixType> TridiagonalizationType;
//     typedef typename TridiagonalizationType::TridiagonalMatrixType TridiagonalMatrixType;

    SelfAdjointEigenSolver()
        : m_eivec(int(Size), int(Size)),
          m_eivalues(int(Size)),
          m_subdiag(int(TridiagonalizationType::SizeMinusOne))
    {
      ei_assert(Size!=Dynamic);
    }

    SelfAdjointEigenSolver(int size)
        : m_eivec(size, size),
          m_eivalues(size),
          m_subdiag(TridiagonalizationType::SizeMinusOne)
    {}

    /** Constructors computing the eigenvalues of the selfadjoint matrix \a matrix,
      * as well as the eigenvectors if \a computeEigenvectors is true.
      *
      * \sa compute(MatrixType,bool), SelfAdjointEigenSolver(MatrixType,MatrixType,bool)
      */
    SelfAdjointEigenSolver(const MatrixType& matrix, bool computeEigenvectors = true)
      : m_eivec(matrix.rows(), matrix.cols()),
        m_eivalues(matrix.cols()),
        m_subdiag()
    {
      if (matrix.rows() > 1) m_subdiag.resize(matrix.rows() - 1);
      compute(matrix, computeEigenvectors);
    }

    /** Constructors computing the eigenvalues of the generalized eigen problem
      * \f$ Ax = lambda B x \f$ with \a matA the selfadjoint matrix \f$ A \f$
      * and \a matB the positive definite matrix \f$ B \f$ . The eigenvectors
      * are computed if \a computeEigenvectors is true.
      *
      * \sa compute(MatrixType,MatrixType,bool), SelfAdjointEigenSolver(MatrixType,bool)
      */
    SelfAdjointEigenSolver(const MatrixType& matA, const MatrixType& matB, bool computeEigenvectors = true)
      : m_eivec(matA.rows(), matA.cols()),
        m_eivalues(matA.cols()),
        m_subdiag()
    {
      if (matA.rows() > 1) m_subdiag.resize(matA.rows() - 1);
      compute(matA, matB, computeEigenvectors);
    }

    SelfAdjointEigenSolver& compute(const MatrixType& matrix, bool computeEigenvectors = true);

    SelfAdjointEigenSolver& compute(const MatrixType& matA, const MatrixType& matB, bool computeEigenvectors = true);

    /** \returns the computed eigen vectors as a matrix of column vectors */
    MatrixType eigenvectors(void) const
    {
      #ifndef NDEBUG
      ei_assert(m_eigenvectorsOk);
      #endif
      return m_eivec;
    }

    /** \returns the computed eigen values */
    RealVectorType eigenvalues(void) const { return m_eivalues; }

    /** \returns the positive square root of the matrix
      *
      * \note the matrix itself must be positive in order for this to make sense.
      */
    MatrixType operatorSqrt() const
    {
      return m_eivec * m_eivalues.cwiseSqrt().asDiagonal() * m_eivec.adjoint();
    }

    /** \returns the positive inverse square root of the matrix
      *
      * \note the matrix itself must be positive definite in order for this to make sense.
      */
    MatrixType operatorInverseSqrt() const
    {
      return m_eivec * m_eivalues.cwiseInverse().cwiseSqrt().asDiagonal() * m_eivec.adjoint();
    }


  protected:
    MatrixType m_eivec;
    RealVectorType m_eivalues;
    typename TridiagonalizationType::SubDiagonalType m_subdiag;
    #ifndef NDEBUG
    bool m_eigenvectorsOk;
    #endif
};

#ifndef EIGEN_HIDE_HEAVY_CODE

/** \internal
  *
  * \eigenvalues_module \ingroup Eigenvalues_Module
  *
  * Performs a QR step on a tridiagonal symmetric matrix represented as a
  * pair of two vectors \a diag and \a subdiag.
  *
  * \param matA the input selfadjoint matrix
  * \param hCoeffs returned Householder coefficients
  *
  * For compilation efficiency reasons, this procedure does not use eigen expression
  * for its arguments.
  *
  * Implemented from Golub's "Matrix Computations", algorithm 8.3.2:
  * "implicit symmetric QR step with Wilkinson shift"
  */
template<typename RealScalar, typename Scalar>
static void ei_tridiagonal_qr_step(RealScalar* diag, RealScalar* subdiag, int start, int end, Scalar* matrixQ, int n);

/** Computes the eigenvalues of the selfadjoint matrix \a matrix,
  * as well as the eigenvectors if \a computeEigenvectors is true.
  *
  * \sa SelfAdjointEigenSolver(MatrixType,bool), compute(MatrixType,MatrixType,bool)
  */
template<typename MatrixType>
SelfAdjointEigenSolver<MatrixType>& SelfAdjointEigenSolver<MatrixType>::compute(const MatrixType& matrix, bool computeEigenvectors)
{
  #ifndef NDEBUG
  m_eigenvectorsOk = computeEigenvectors;
  #endif
  assert(matrix.cols() == matrix.rows());
  int n = matrix.cols();
  m_eivalues.resize(n,1);
  m_eivec.resize(n,n);

  if(n==1)
  {
    m_eivalues.coeffRef(0,0) = ei_real(matrix.coeff(0,0));
    m_eivec.setOnes();
    return *this;
  }

  m_eivec = matrix;

  // FIXME, should tridiag be a local variable of this function or an attribute of SelfAdjointEigenSolver ?
  // the latter avoids multiple memory allocation when the same SelfAdjointEigenSolver is used multiple times...
  // (same for diag and subdiag)
  RealVectorType& diag = m_eivalues;
  m_subdiag.resize(n-1);
  TridiagonalizationType::decomposeInPlace(m_eivec, diag, m_subdiag, computeEigenvectors);

  int end = n-1;
  int start = 0;
  while (end>0)
  {
    for (int i = start; i<end; ++i)
      if (ei_isMuchSmallerThan(ei_abs(m_subdiag[i]),(ei_abs(diag[i])+ei_abs(diag[i+1]))))
        m_subdiag[i] = 0;

    // find the largest unreduced block
    while (end>0 && m_subdiag[end-1]==0)
      end--;
    if (end<=0)
      break;
    start = end - 1;
    while (start>0 && m_subdiag[start-1]!=0)
      start--;

    ei_tridiagonal_qr_step(diag.data(), m_subdiag.data(), start, end, computeEigenvectors ? m_eivec.data() : (Scalar*)0, n);
  }

  // Sort eigenvalues and corresponding vectors.
  // TODO make the sort optional ?
  // TODO use a better sort algorithm !!
  for (int i = 0; i < n-1; ++i)
  {
    int k;
    m_eivalues.segment(i,n-i).minCoeff(&k);
    if (k > 0)
    {
      std::swap(m_eivalues[i], m_eivalues[k+i]);
      m_eivec.col(i).swap(m_eivec.col(k+i));
    }
  }
  return *this;
}

/** Computes the eigenvalues of the generalized eigen problem
  * \f$ Ax = lambda B x \f$ with \a matA the selfadjoint matrix \f$ A \f$
  * and \a matB the positive definite matrix \f$ B \f$ . The eigenvectors
  * are computed if \a computeEigenvectors is true.
  *
  * \sa SelfAdjointEigenSolver(MatrixType,MatrixType,bool), compute(MatrixType,bool)
  */
template<typename MatrixType>
SelfAdjointEigenSolver<MatrixType>& SelfAdjointEigenSolver<MatrixType>::
compute(const MatrixType& matA, const MatrixType& matB, bool computeEigenvectors)
{
  ei_assert(matA.cols()==matA.rows() && matB.rows()==matA.rows() && matB.cols()==matB.rows());

  // Compute the cholesky decomposition of matB = L L'
  LLT<MatrixType> cholB(matB);

  // compute C = inv(L) A inv(L')
  MatrixType matC = matA;
  cholB.matrixL().solveInPlace(matC);
  // FIXME since we currently do not support A * inv(L'), let's do (inv(L) A')' :
  matC.adjointInPlace();
  cholB.matrixL().solveInPlace(matC);
  matC.adjointInPlace();
  // this version works too:
//   matC = matC.transpose();
//   cholB.matrixL().conjugate().template marked<Lower>().solveTriangularInPlace(matC);
//   matC = matC.transpose();
  // FIXME: this should work: (currently it only does for small matrices)
//   Transpose<MatrixType> trMatC(matC);
//   cholB.matrixL().conjugate().eval().template marked<Lower>().solveTriangularInPlace(trMatC);

  compute(matC, computeEigenvectors);

  if (computeEigenvectors)
  {
    // transform back the eigen vectors: evecs = inv(U) * evecs
    cholB.matrixU().solveInPlace(m_eivec);
    for (int i=0; i<m_eivec.cols(); ++i)
      m_eivec.col(i) = m_eivec.col(i).normalized();
  }
  return *this;
}

#endif // EIGEN_HIDE_HEAVY_CODE

/** \eigenvalues_module
  *
  * \returns a vector listing the eigenvalues of this matrix.
  */
template<typename Derived>
inline Matrix<typename NumTraits<typename ei_traits<Derived>::Scalar>::Real, ei_traits<Derived>::ColsAtCompileTime, 1>
MatrixBase<Derived>::eigenvalues() const
{
  ei_assert(Flags&SelfAdjoint);
  return SelfAdjointEigenSolver<typename Derived::PlainObject>(eval(),false).eigenvalues();
}

template<typename Derived, bool IsSelfAdjoint>
struct ei_operatorNorm_selector
{
  static inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real
  operatorNorm(const MatrixBase<Derived>& m)
  {
    // FIXME if it is really guaranteed that the eigenvalues are already sorted,
    // then we don't need to compute a maxCoeff() here, comparing the 1st and last ones is enough.
    return m.eigenvalues().cwiseAbs().maxCoeff();
  }
};

template<typename Derived> struct ei_operatorNorm_selector<Derived, false>
{
  static inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real
  operatorNorm(const MatrixBase<Derived>& m)
  {
    typename Derived::PlainObject m_eval(m);
    // FIXME if it is really guaranteed that the eigenvalues are already sorted,
    // then we don't need to compute a maxCoeff() here, comparing the 1st and last ones is enough.
    return ei_sqrt(
             (m_eval*m_eval.adjoint())
             .template marked<SelfAdjoint>()
             .eigenvalues()
             .maxCoeff()
           );
  }
};

/** \eigenvalues_module
  *
  * \returns the matrix norm of this matrix.
  */
template<typename Derived>
inline typename NumTraits<typename ei_traits<Derived>::Scalar>::Real
MatrixBase<Derived>::operatorNorm() const
{
  return ei_operatorNorm_selector<Derived, Flags&SelfAdjoint>
       ::operatorNorm(derived());
}

#ifndef EIGEN_EXTERN_INSTANTIATIONS
template<typename RealScalar, typename Scalar>
static void ei_tridiagonal_qr_step(RealScalar* diag, RealScalar* subdiag, int start, int end, Scalar* matrixQ, int n)
{
  RealScalar td = (diag[end-1] - diag[end])*RealScalar(0.5);
  RealScalar e2 = ei_abs2(subdiag[end-1]);
  RealScalar mu = diag[end] - e2 / (td + (td>0 ? 1 : -1) * ei_sqrt(td*td + e2));
  RealScalar x = diag[start] - mu;
  RealScalar z = subdiag[start];

  for (int k = start; k < end; ++k)
  {
    PlanarRotation<RealScalar> rot;
    rot.makeGivens(x, z);

    // do T = G' T G
    RealScalar sdk = rot.s() * diag[k] + rot.c() * subdiag[k];
    RealScalar dkp1 = rot.s() * subdiag[k] + rot.c() * diag[k+1];

    diag[k] = rot.c() * (rot.c() * diag[k] - rot.s() * subdiag[k]) - rot.s() * (rot.c() * subdiag[k] - rot.s() * diag[k+1]);
    diag[k+1] = rot.s() * sdk + rot.c() * dkp1;
    subdiag[k] = rot.c() * sdk - rot.s() * dkp1;

    if (k > start)
      subdiag[k - 1] = rot.c() * subdiag[k-1] - rot.s() * z;

    x = subdiag[k];

    if (k < end - 1)
    {
      z = -rot.s() * subdiag[k+1];
      subdiag[k + 1] = rot.c() * subdiag[k+1];
    }

    // apply the givens rotation to the unit matrix Q = Q * G
    if (matrixQ)
    {
      Map<Matrix<Scalar,Dynamic,Dynamic> > q(matrixQ,n,n);
      q.applyOnTheRight(k,k+1,rot);
    }
  }
}
#endif

#endif // EIGEN_SELFADJOINTEIGENSOLVER_H
