// ChemicalFun is a C++ and Python library for of C++ and Python API
// for Chemical Formula Parser and Reactions Generator.
//
// Copyright (C) 2018-2022 G.D.Miron, D.Kulik, S.Dmytriieva
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 2.1 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <sstream>
#include "ReactionsGenerator/GramSchmidtWeltin94.h"
#include "Common/Exception.h"

namespace ReactionsGenerator {

/**
 * @brief gramSchmidt
 * @param F input formula matrix
 * @param m number of non-master substances, or rank of matrix
 * @return reaction matrix[cols-reactions,rows-substances] with
 * the last m columns representing the independent reactions
 */
auto gramSchmidt(MatrixXd F, unsigned &m)-> MatrixXd
{
    double s = 0.0;
    double t = 0.0;
    auto R = F.rows();
    auto C = F.cols();

    m = C-R;

    // normalize the first column
    for (unsigned i = 0; i < R; i++)
    {
        s += (F(i,0)*F(i,0));
    }

    if ( s > thresholdGramSchmidtWeltin)
    {
        F.col(0) = F.col(0)/sqrt(s);
    }

    // loop
    for (unsigned k = 1; k < C; k++)
    {
        // projection step
        for (unsigned j = 0; j < k; j++)
        {
            t = 0.0;
            for (unsigned i = 0; i < R; i++)
            {
                t += F(i,j)*F(i,k);
            }

            for (unsigned i = 0; i < R; i++)
            {
                F(i,k) = F(i,k)-t*F(i,j);
            }
        }
        s = 0.0;

        // normalize column k
        for (unsigned i = 0; i < R; i++)
        {
            s += (F(i,k)*F(i,k));
        }

        if ( s > thresholdGramSchmidtWeltin)
        {
              F.col(k) = F.col(k)/sqrt(s);
        } else
        {
            if (k <= m)
                m = m-1;
            // delete the current column
            unsigned int numRows = F.rows();
            unsigned int numCols = F.cols()-1;

            if( k < numCols )
            {
                F.block(0,k,numRows,numCols-k) = F.block(0,k+1,numRows,numCols-k);
                F.conservativeResize(numRows,numCols);
                k = k-1;
                C = C-1;
            }
            if (k == numCols)
            {
                F.conservativeResize(numRows,numCols);
                C = C-1;
            }
        }
    }
    if( ChemicalFun::chfun_logger->should_log(spdlog::level::debug)) {
        std::ostringstream ss; ss << F;
        ChemicalFun::chfun_logger->debug("m = {} ", m);
        ChemicalFun::chfun_logger->debug("gramSchmidt \n {} ", ss.str());
    }
  return F;
}

auto weltin94(MatrixXd &M, /*Indices &iMaster,*/ Indices &iNonMaster) -> MatrixXd
{
    unsigned r;

    // prepare matrix
    MatrixXd m;
    m = M.transpose();
    augmentIdentityMatrixRight(m);

    // run Gram-Schmidt algorithm
    M = gramSchmidt(m, r);
    //i = M.rows() - r;
    MatrixXd R = M;

    // Format matrix
//    formatMatrixCoeffToInt(M);
    formatMatrixCoeffToZero(M);
    removeMasterColls(M, r);

    iNonMaster = indices(M.rows());
    if( ChemicalFun::chfun_logger->should_log(spdlog::level::debug)) {
        std::ostringstream ss; ss << R;
        ChemicalFun::chfun_logger->debug("weltin94 \n {} ", ss.str());
    }
    return R;
}

// Wikipedia
auto gramSchmidt_(MatrixXd M)-> MatrixXd
{
    auto n = M.rows();
    auto k = M.cols();

    MatrixXd U(n,k);
    U = MatrixXd::Zero(n,k);

    for (unsigned i = 0; i < n; i++)
    {
        U.col(0) = M.col(0)/sqrt(M.col(0).transpose()*M.col(0));
    }

    for (unsigned i = 1; i < k; i++)
    {
        U.col(i) = M.col(i);
        for (unsigned j = 0; j <= i-1; j++)
        {
           U.col(i) = U.col(i) - (U.col(i).transpose()*U.col(j))*U.col(j);
        }
        U.col(i) = U.col(i)/sqrt(U.col(i).transpose()*U.col(i));
    }
    return U;
}
}
