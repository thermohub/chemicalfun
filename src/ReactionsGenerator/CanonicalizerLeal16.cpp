// ChemicalFun is a C++ and Python library for of C++ and Python API
// for Chemical Formula Parser and Reactions Generator.
//
// Copyright (C) 2018-2022 G.D.Miron, D.Kulik, S.Dmytriieva
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "CanonicalizerLeal16.h"
#include "Common/Exception.h"
#include "spdlog/fmt/ostr.h"

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

    if( ChemicalFun::chfun_logger->should_log(spdlog::level::debug)) {
        ChemicalFun::chfun_logger->debug("leal16 \n {} ", R);
    }

    return R;
}

}
