#include "CanonicalizerLeal16.h"

namespace ReactionsGenerator {

/// Define an alias to a permutation matrix type of the Eigen library
using PermutationMatrix = Eigen::PermutationMatrix<Eigen::Dynamic, Eigen::Dynamic>;

auto canonicalize (MatrixXd &M, PermutationMatrix &Q) -> void
{

    // The number of rows and columns of A
    long m = M.rows();
    long n = M.cols();

    // Compute the full-pivoting LU of A so that P*A*Q = L*U
    FullPivLU<MatrixXd> lu(M);

    // Get the rank of matrix A
    auto r = lu.rank();

    // Get the LU factors of matrix A
    auto Lbb = lu.matrixLU().topLeftCorner(r, r).triangularView<UnitLower>();
    auto Ubb = lu.matrixLU().topLeftCorner(r, r).triangularView<Upper>();
    auto Ubn = lu.matrixLU().topRightCorner(r, n - r);

    // Set the permutation matrices P and Q
    MatrixXd P = lu.permutationP();
    Q = lu.permutationQ();

    // Calculate the regularizer matrix R
    MatrixXd R = P;
    R.conservativeResize(r, m);
    R = Lbb.solve(R);
    R = Ubb.solve(R);

    // Calculate the inverse of the regularizer matrix R
    MatrixXd Rinv = P.transpose();
    Rinv.conservativeResize(m, r);
    Rinv = Rinv * Lbb;
    Rinv = Rinv * Ubb;

    // Calculate matrix S
    MatrixXd S = Ubn;
    S = Ubb.solve(S);

    M = S;
}

auto leal16(MatrixXd &M, Indices &iMaster, Indices &iNonMaster) -> MatrixXd
{
//    // Check if number of columns is greater/equal than number of rows
//    Assert(n >= m, "Could not canonicalize the given matrix.",
//        "The given matrix has more rows than columns.");

    // canonicalize
    PermutationMatrix Q;
    canonicalize(M, Q);
    MatrixXd R = M;

    // get the indices of Master and NonMaster substances
    auto begin = Q.indices().data();
    iNonMaster = Indices(begin + M.rows(), begin + Q.rows());
    iMaster = Indices(begin, begin + M.rows());

    // add identity matrix to the left of the resulted matrix
    augmentIdentityMatrixLeft(M);

    // add NonMaster substances reaction coefficients rows
    addNonMasterCoeffRows(M, iNonMaster.size());

    // format coefficients from double to integer
//    formatMatrixCoeffToInt(M);
    formatMatrixCoeffToZero(M);

    // remove master coefficients columns
    removeMasterColls(M, iMaster.size());

    return R;
}

}
