#ifndef SOPHUS_ROTATION_MATRIX_HPP
#define SOPHUS_ROTATION_MATRIX_HPP

#include <Eigen/Dense>
#include <Eigen/SVD>

#include "types.hpp"

namespace Sophus {

// Takes in arbiray square matrix and returns true if it is
// orthogonal.
template <class D>
EIGEN_DEVICE_FUNC bool isOrthogoal(Eigen::MatrixBase<D> const& R) {
  using Scalar = typename D::Scalar;
  static int const N = D::RowsAtCompileTime;
  static int const M = D::ColsAtCompileTime;

  static_assert(N == M, "must be a square matrix");
  static_assert(N >= 2, "must have compile time dimension >= 2");

  return (R * R.transpose() - Matrix<Scalar, N, N>::Identity()).norm() <
         Constants<Scalar>::epsilon();
}

// Takes in arbiray square matrix (2x2 or larger) and returns closed
// orthogonal matrix with positive determinant.
template <class D>
EIGEN_DEVICE_FUNC
    Matrix<typename D::Scalar, D::RowsAtCompileTime, D::RowsAtCompileTime>
    makeRotationMatrix(Eigen::MatrixBase<D> const& R) {
  using Scalar = typename D::Scalar;
  static int const N = D::RowsAtCompileTime;
  static int const M = D::ColsAtCompileTime;

  static_assert(N == M, "must be a square matrix");
  static_assert(N >= 2, "must have compile time dimension >= 2");

  Eigen::JacobiSVD<Matrix<Scalar, N, N>> svd(
      R, Eigen::ComputeFullU | Eigen::ComputeFullV);

  // Determine determinant of orthogonal matrix U*V'.
  Scalar d = (svd.matrixU() * svd.matrixV().transpose()).determinant();
  // Starting from the identity matrix D, set the last entry to d (+1 or
  // -1),  so that det(U*D*V') = 1.
  Matrix<Scalar, N, N> Diag = Matrix<Scalar, N, N>::Identity();
  Diag(N - 1, N - 1) = d;
  return svd.matrixU() * Diag * svd.matrixV().transpose();
}

}  // namespace Sophus

#endif  // SOPHUS_ROTATION_MATRIX_HPP