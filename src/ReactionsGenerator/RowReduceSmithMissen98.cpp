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

#include "RowReduceSmithMissen98.h"
#include "Common/Exception.h"
#include "spdlog/fmt/ostr.h"

namespace ReactionsGenerator {

// Row reduce algorithm
// http://codegolf.stackexchange.com/questions/59307/reduced-row-echelon-form-of-a-matrix
auto rowReduce(MatrixXd M)-> MatrixXd
{
    auto p = 0;
    auto R = M.rows();
    auto C = M.cols();

    for (unsigned r = 0; r <= (R-1); r++)
    {
        if ((C-1) <= p)
            break;
        auto i = r;

        while (M(i, p) == 0)
        {
            i = i + 1;
            if ((R) == i)
            {
                i = r;
                p = p + 1;
                if ((C) == p)
                    return(M);
            }
        }

        auto Mtemp = M;
        M.row(i) = M.row(r);
        M.row(r) = Mtemp.row(i);
        M.row(r) = M.row(r) / M(r, p);

        for (unsigned i=0; i<=(R-1); i++ )
        {
            if (i != r)
                M.row(i) = M.row(i) - M.row(r) * M(i, p);
        }
        p = p + 1;
    }
    return M;
}

/**
 * @brief arrangeIdentityMatrixLeft ordes the matrix so that the identity part is in the left part
 * of the ractions matrix
 * @param S input and output matrix
 * @param ind indices of the swaped columns
 */
auto arrangeIdentityMatrixLeft(MatrixXd &S, Indices &I) -> void
{
    VectorXd M(S.rows());

    I = indices(S.cols());

    for (unsigned i = 0; i<S.rows(); i++)
    {
        if (i < S.cols())
        {
            M = S.col(i);
            if (M(i) == 1) // makes S(i, i) = 0
                M(i) = 0;
            if (!M.isZero(0)) // if there are non 0 coefficients on the column i, we search for the column which has S(i,i) = 1 and other coeff 0
            {
                for (unsigned j = i+1; j <S.cols(); j++) // go through the rest of the columns
                {
                    M = S.col(j);
                    if (M(i) == 1) // find which of the following columns has 1 on position i
                        M(i) = 0;
                    if (M.isZero(0)) // if all the other coefificents in the column are zero, we found the column with S(i,i) = 1 and other coeff 0
                    {
                        // swap columns
                        S.col(i).swap(S.col(j));
                        int temp = I[i];
                        I[i] = I[j];
                        I[j] = temp;
                    }
                }
            }
        }
    }
}

/**
 * @brief removeZeroRows removes rows of 0
 * @param M input and output matrix
 */
auto removeZeroRows(MatrixXd &M) -> void
{
    VectorXd V(M.cols());

    for (unsigned i = 0; i<M.rows(); i++)
    {
        V = M.row(i);
        // remove row of 0s
        if (V.isZero(0))
        {
            unsigned int numRows = M.rows()-1;
            unsigned int numCols = M.cols();

            M.block(i,0,numRows-i,numCols) = M.block(i+1,0,numRows-i,numCols);
            M.conservativeResize(numRows,numCols);
            i--;
        }
    }
}

auto smithMissen98(MatrixXd &M, Indices &iMaster, Indices &iNonMaster)-> MatrixXd
{
    unsigned i, r;
    Indices indices(M.cols());

    // run row reduce algorithm
    M = rowReduce(M);
    MatrixXd R = M;

    // get rank of matrix r and number of non-master substances or independent reactions
    FullPivLU<MatrixXd> lu_decomp(M);
    r = lu_decomp.rank();
    i = M.cols() - r;

    // format the matrix
    arrangeIdentityMatrixLeft(M, indices);
    removeZeroRows(M);
    addNonMasterCoeffRows(M, i);
    formatMatrixCoeff(M);
//    formatMatrixCoeffToInt(M);

    // get indices of master and non-master substances
    iNonMaster = Indices(i);
    for (Index ind = 0; ind<i; ind++) iNonMaster[ind] = indices[M.cols()-(i-ind)];

    iMaster = Indices(r);
    for (Index ind = 0; ind<r; ind++) iMaster[ind] = indices[ind];

    // remove master coefficients columns
    removeMasterColls(M, iMaster.size());

    if( ChemicalFun::chfun_logger->should_log(spdlog::level::debug)) {
        ChemicalFun::chfun_logger->debug("smithMissen98 \n {} ", R);
    }
    return R;
}

}
