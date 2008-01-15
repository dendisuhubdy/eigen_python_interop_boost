#include <Eigen/Core>
USING_PART_OF_NAMESPACE_EIGEN
using namespace std;

template<typename Scalar, typename Derived>
const Eigen::Cast<
  typename Eigen::NumTraits<Scalar>::FloatingPoint,
  Derived
>
castToFloatingPoint(const MatrixBase<Scalar, Derived>& m)
{
  return Eigen::Cast<
    typename Eigen::NumTraits<Scalar>::FloatingPoint,
    Derived
  >(m.ref());
}

int main(int, char**)
{
  Matrix2i m = Matrix2i::random();
  cout << "Here's the matrix m. It has coefficients of type int."
       << endl << m << endl;
  cout << "Here's m/20:" << endl << castToFloatingPoint(m)/20 << endl;
  return 0;
}