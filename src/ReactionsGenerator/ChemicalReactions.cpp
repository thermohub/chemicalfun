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

#include "ChemicalFun/ReactionsGenerator/ChemicalReactions.h"
#include "ChemicalFun/ReactionsGenerator/Generator.h"
#include "Common/Exception.h"

#include "fstream"
#include "iostream"

namespace ReactionsGenerator {

struct ChemicalReactions::Impl
{
    MatrixXd formulaMatrix;

    IndexSubstancesMap iColSubstancesMap;

    Impl()
    {}

    Impl(std::vector<std::vector<double>> A)
    {
        MatrixXd temp(A.size(), A[0].size());
        for (unsigned r = 0; r<A.size(); r++)
            for (unsigned c = 0; c<A[r].size(); c++)
                temp(r,c) = A[r][c];
        formulaMatrix = temp;
    }

    Impl(MatrixXd A, std::vector<std::string> substancesList)
    {
        formulaMatrix = A;
        for (unsigned i = 0; i<substancesList.size(); i++)
        {
            iColSubstancesMap.insert( std::pair<Index,std::string>(i,substancesList[i]) );
        }
    }

};

ChemicalReactions::ChemicalReactions()
: pimpl(new Impl())
{}

ChemicalReactions::ChemicalReactions(std::vector<std::vector<double>> A)
: pimpl(new Impl(A))
{this->eraseZeroRowsFormulaMatrix(); /*std::cout << pimpl->formulaMatrix << std::endl;*compute(A);*/}

ChemicalReactions::ChemicalReactions(MatrixXd A, std::vector<std::string> substancesList)
: pimpl(new Impl(A, substancesList))
{this->eraseZeroRowsFormulaMatrix(); /*std::cout << pimpl->formulaMatrix << std::endl;*compute(A);*/}

ChemicalReactions::ChemicalReactions(const ChemicalReactions& other)
: pimpl(new Impl(*other.pimpl))
{}

ChemicalReactions::~ChemicalReactions()
{}

auto ChemicalReactions::operator=(ChemicalReactions other) -> ChemicalReactions&
{
    pimpl = std::move(other.pimpl);
    return *this;
}

auto ChemicalReactions::formulaMatrix () -> MatrixXd
{
    return pimpl->formulaMatrix;
}

auto ChemicalReactions::sizeSubstancesMap() -> size_t
{
    return pimpl->iColSubstancesMap.size();
}

auto ChemicalReactions::eraseZeroRowsFormulaMatrix ( ) -> void
{
    MatrixXd M = pimpl->formulaMatrix;
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
    pimpl->formulaMatrix = M;
}

auto ChemicalReactions::mapIndex(std::string symbol) -> int
{
    IndexSubstancesMap::iterator it_;
    IndexSubstancesMap map = pimpl->iColSubstancesMap;
    for (it_ = map.begin(); it_ != map.end(); ++it_ )
        if (it_->second == symbol)
            return it_->first;

    return -1;
}

auto ChemicalReactions::getCharge(std::string symbol) -> int
{
    auto index = mapIndex(symbol);

    if ( index < 0 )
    {
        ChemicalFun::errorSubstanceNotFound(symbol, "Database::getCharge", __LINE__);
    }

    auto R = pimpl->formulaMatrix.rows();
    auto charge = pimpl->formulaMatrix(R-1, index);
    return charge;
}

auto ChemicalReactions::getCharge(Index index) -> int
{
//    if ( index < 0 )
//    {
//        errorSubstanceNotFound(symbol, "Database::getCharge", __LINE__);
//    }

    auto R = pimpl->formulaMatrix.rows();
    auto charge = pimpl->formulaMatrix(R-1, index);
    return charge;
}

auto ChemicalReactions::reactionsChargesMap(MatrixXd reactionM, Indices iSubstances) -> std::vector<ReactionChargesMap>
{
    ChargeCoeffMap reactantsChargeMap;
    ChargeCoeffMap productsChargeMap;
    ChargeCoeffMap::iterator it;
    std::vector<ReactionChargesMap> reactionsChargesMaps;

    int charge;
    auto substMap = pimpl->iColSubstancesMap;
//    iColSubstancesMap.insert( std::pair<Index,std::string>(i,substancesList[i]) );
//    mymap.erase (it);
    for (unsigned c = 0; c<reactionM.cols(); c++)
    {
        for (unsigned r = 0; r<reactionM.rows(); r++)
        {
            charge = getCharge(substMap[iSubstances[r]]);

            if (reactionM(r,c) < 0) // reactants
            {
                it = reactantsChargeMap.find(charge);
                if (it != reactantsChargeMap.end())
                {
                    reactantsChargeMap[charge] +=  std::fabs(reactionM(r,c));
                } else
                {
                    reactantsChargeMap.insert(std::pair<int,double>(charge, std::fabs(reactionM(r,c))));
                }
            }
            if (reactionM(r,c) > 0) // products
            {
                it = productsChargeMap.find(charge);
                if (it != productsChargeMap.end())
                {
                    productsChargeMap[charge] +=  std::fabs(reactionM(r,c));
                } else
                {
                    productsChargeMap.insert(std::pair<int,double>(charge, std::fabs(reactionM(r,c))));
                }
            }
        }

        reactionsChargesMaps.push_back(ReactionChargesMap {reactantsChargeMap, productsChargeMap});
        reactantsChargeMap.clear();
        productsChargeMap.clear();
    }

    return reactionsChargesMaps;
}

auto ChemicalReactions::checkReacMatrixISOC(MatrixXd reactionM, Indices iSubstances) -> std::vector<bool>
{
    std::vector<bool> isoc;
    ChargeCoeffMap::iterator it;

    std::vector<ReactionChargesMap> reacChMap = reactionsChargesMap(reactionM, iSubstances);

    for (unsigned i = 0; i<reactionM.cols(); i++)
    {
        // erasing 0 charge from products and reactants charges map
        it = reacChMap[i].reactantsChargeMap.find(0);
        if (it != reacChMap[i].reactantsChargeMap.end())
            reacChMap[i].reactantsChargeMap.erase(it);
        it = reacChMap[i].productsChargeMap.find(0);
        if (it != reacChMap[i].productsChargeMap.end())
            reacChMap[i].productsChargeMap.erase(it);

        // comparing maps
        if (map_compare(reacChMap[i].reactantsChargeMap, reacChMap[i].productsChargeMap))
            isoc.push_back(true);
        else
            isoc.push_back(false);
    }
    return isoc;
}

auto ChemicalReactions::checkReacMatrixISOE(MatrixXd reactionM, Indices iSubstances) -> std::vector<bool>
{
    std::vector<bool> isoe;
    ChargeCoeffMap::iterator it;
    unsigned int cPosR = 0; unsigned int cPosP = 0;
    unsigned int cNegR = 0; unsigned int cNegP = 0;
    std::vector<ReactionChargesMap> reacChMap = reactionsChargesMap(reactionM, iSubstances);

    for (unsigned i = 0; i<reactionM.cols(); i++)
    {
        // erasing 0 charge from products and reactants charges map
        it = reacChMap[i].reactantsChargeMap.find(0);
        if (it != reacChMap[i].reactantsChargeMap.end())
            reacChMap[i].reactantsChargeMap.erase(it);
        it = reacChMap[i].productsChargeMap.find(0);
        if (it != reacChMap[i].productsChargeMap.end())
            reacChMap[i].productsChargeMap.erase(it);

        // count positive and negative charges
        for (it = reacChMap[i].reactantsChargeMap.begin(); it != reacChMap[i].reactantsChargeMap.end(); ++it )
        {
            if (it->first > 0) cPosR++;
            if (it->first < 0) cNegR++;
        }

        for (it = reacChMap[i].productsChargeMap.begin(); it != reacChMap[i].productsChargeMap.end(); ++it )
        {
            if (it->first > 0) cPosP++;
            if (it->first < 0) cNegP++;
        }

        // comparing charges
        if ((cPosR == cPosP) && (cNegR == cNegP))
            isoe.push_back(true);
        else
            isoe.push_back(false);

        cPosP = 0; cNegP = 0; cPosR = 0; cNegR = 0;
    }
    return isoe;
}


}
