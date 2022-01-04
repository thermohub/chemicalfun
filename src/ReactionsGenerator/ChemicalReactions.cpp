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

#include "ChemicalFun/FormulaParser/ChemicalData.h"

#include "fstream"
#include "iostream"

namespace ReactionsGenerator {

struct ChemicalReactions::Impl
{
    MatrixXd formulaMatrix;

    MatrixXd reactionsMatrix;

    MatrixXd substancesStoichMatrix;

    IndexSubstancesMap iColFormulasMap;
    
    IndexSubstancesMap iColSymbolsMap;

    Indices iSubstances;

    std::vector<std::string> substanceFormulas;

    std::vector<std::string> substanceSymbols;

    std::vector<Reactants> reactions;

    //std::map<Reactants> mapReactions;

    std::vector<std::string> strReactions;

    Generator generator;

    std::vector<std::string> masterSubstances;

    std::vector<std::string> dependentSubstances;

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

    Impl(std::vector<std::string> substanceFormulasList, bool valence)
    {
        //Impl(ChemicalFun::substancesStoichiometryMatrix(substanceFormulasList));

        formulaMatrix = stoichiometryMatrix(ChemicalFun::substancesStoichiometryMatrix(substanceFormulasList, valence)).transpose();

        for (unsigned i = 0; i<substanceFormulasList.size(); i++)
        {
            iColFormulasMap.insert( std::pair<Index,std::string>(i,substanceFormulasList[i]) );
            iColSymbolsMap.insert( std::pair<Index,std::string>(i,substanceFormulasList[i]) );
        }
    }

    Impl(std::vector<std::string> substanceFormulasList, std::vector<std::string> substanceSymbolsList, bool valence)
    {
        //Impl(ChemicalFun::substancesStoichiometryMatrix(substanceFormulasList));

        ChemicalFun::funErrorIf(substanceFormulasList.size() != substanceSymbolsList.size(), "Chemical Reactions", 
        "Different number of formulas "+ std::to_string(substanceFormulasList.size()) +" and symbols " + std::to_string(substanceSymbolsList.size()) , __LINE__, __FILE__ );

        formulaMatrix = stoichiometryMatrix(ChemicalFun::substancesStoichiometryMatrix(substanceFormulasList, valence)).transpose();

        for (unsigned i = 0; i<substanceFormulasList.size(); i++)
        {
            iColFormulasMap.insert( std::pair<Index,std::string>(i,substanceFormulasList[i]) );
            iColSymbolsMap.insert( std::pair<Index,std::string>(i,substanceSymbolsList[i]) );
        }
    }

    Impl(MatrixXd A, std::vector<std::string> substanceFormulasList)
    {
        formulaMatrix = A;
        for (unsigned i = 0; i<substanceFormulasList.size(); i++)
        {
            iColFormulasMap.insert( std::pair<Index,std::string>(i,substanceFormulasList[i]) );
        }
    }

    auto makeReactions() -> void
    {
        ChemicalFun::funErrorIf(reactionsMatrix.size() == 0, "Chemical Reactions", 
        "Reactions matrix is empty. Call generateReactions() first.", __LINE__, __FILE__ );

        reactions.clear();
        strReactions.clear();

        for(size_t c = 0; c<reactionsMatrix.cols(); c++)
        {
            Reactants reaction;
            for ( size_t r = 0; r<reactionsMatrix.rows(); r++)
            {
                std::string symbol = iColSymbolsMap[iSubstances[r]];
                if (reactionsMatrix(r,c) != 0.0)
                    reaction.push_back( std::pair<std::string, double>(symbol,reactionsMatrix(r,c)) );
            }
            reactions.push_back(reaction);
        }

        auto imaster = generator.imaster();
        auto inonmaster = generator.inonmaster();

        masterSubstances.clear();
        dependentSubstances.clear();

        for (const auto& i : imaster )
            masterSubstances.push_back(iColSymbolsMap[i]);

        for (const auto& i : inonmaster )
            dependentSubstances.push_back(iColSymbolsMap[i]);
    }

    auto halfReaction(std::string& reactR, const std::vector<std::string>& reacts, const std::vector<double>& coeffs, bool reactantsOrder) -> void
    {
        if (reactantsOrder)
            for( std::size_t jj = 0; jj <reacts.size(); jj++)
            {
                if( jj > 0 )
                    reactR += " + ";
                
                coefficient(reactR, coeffs[jj]);
                reactR += reacts[jj];
            }
        else
            for(size_t jj = reacts.size(); jj --> 0 ;)
            {
                if( jj < reacts.size()-1 )
                    reactR += " + ";

                coefficient(reactR, coeffs[jj]);
                reactR += reacts[jj];
            }
    }

    auto coefficient( std::string& reactR, const double& coeff) -> void
    {
        if( coeff != 1)
         {
             long intPart = coeff;
             double fractionalPart = fabs(coeff - intPart);
             if (fractionalPart == 0)
                 reactR += std::to_string( intPart );
             else
             {
                 auto str = std::to_string( coeff );
                 str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
                 reactR += str;
             }
         }
    }

    auto reactionString(const Reactants& reaction, bool reactantsOrder) -> std::string
    {
       std::string reactR; // reactants, products;
       std::vector<std::string> reactants, products;
       std::vector<double> reacCoeff, prodCoeff;
       int re=0;
       int pr=0;

       for (const auto& r : reaction )
       {
            if (r.second < 0) // reactants
            {
                reactants.push_back(r.first); 
                reacCoeff.push_back(-1*r.second); 
            }

            if (r.second > 0) // products
            {
                products.push_back(r.first); 
                prodCoeff.push_back(r.second); 
            }
       }

       halfReaction(reactR, reactants, reacCoeff, reactantsOrder);
       reactR += " = ";
       halfReaction(reactR, products, prodCoeff, reactantsOrder);

       return reactR;
    }

};

ChemicalReactions::ChemicalReactions()
: pimpl(new Impl())
{}

ChemicalReactions::ChemicalReactions(std::vector<std::string> substanceFormulasList, std::vector<std::string> substanceSymbolsList, bool valence)
: pimpl(new Impl(substanceFormulasList, substanceSymbolsList, valence))
{this->eraseZeroRowsFormulaMatrix(); /*std::cout << pimpl->formulaMatrix << std::endl;*compute(A);*/}

ChemicalReactions::ChemicalReactions(std::vector<std::string> substanceFormulasList, bool valence)
: pimpl(new Impl(substanceFormulasList, valence))
{this->eraseZeroRowsFormulaMatrix(); /*std::cout << pimpl->formulaMatrix << std::endl;*compute(A);*/}

ChemicalReactions::ChemicalReactions(std::vector<std::vector<double>> A)
: pimpl(new Impl(A))
{this->eraseZeroRowsFormulaMatrix(); /*std::cout << pimpl->formulaMatrix << std::endl;*compute(A);*/}

ChemicalReactions::ChemicalReactions(MatrixXd A, std::vector<std::string> substanceFormulasList)
: pimpl(new Impl(A, substanceFormulasList))
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

auto ChemicalReactions::formulaMatrix () const -> const MatrixXd&
{
    return pimpl->formulaMatrix;
}

auto ChemicalReactions::reactionsMatrix () const -> const MatrixXd&
{
    return pimpl->reactionsMatrix;
}

auto ChemicalReactions::generator () const -> const Generator&
{
    return pimpl->generator;
}

auto ChemicalReactions::masterSubstances () const -> const std::vector<std::string>&
{
    return pimpl->masterSubstances;
}

auto ChemicalReactions::dependentSubstances () const -> const std::vector<std::string>&
{
    return pimpl->dependentSubstances;
}


auto ChemicalReactions::sizeSubstancesMap() -> size_t
{
    return pimpl->iColFormulasMap.size();
}

auto ChemicalReactions::generateReactions(bool formation) const -> const std::vector<Reactants>&
{
    ChemicalFun::funErrorIf(pimpl->formulaMatrix.size() == 0, "Chemical Reactions", 
        "Chemical formulas stoiechiometry matrix is empty.", __LINE__, __FILE__ );

    pimpl->generator = Generator();
    pimpl->generator.compute(pimpl->formulaMatrix);
    
    if (formation)
        pimpl->reactionsMatrix   = pimpl->generator.reactionMatrix()*-1;
    else
        pimpl->reactionsMatrix   = pimpl->generator.reactionMatrix();

    pimpl->iSubstances = pimpl->generator.isubstances();

    pimpl->makeReactions();

    return pimpl->reactions;
    //return pimpl->iColFormulasMap.size();
}

auto ChemicalReactions::stringReactions(bool reactantsOrder) const -> const std::vector<std::string>&
{
    pimpl->strReactions.clear();
    
    if (pimpl->reactions.size() == 0)
        generateReactions();

    for (const auto& rr : pimpl->reactions)
        pimpl->strReactions.push_back(pimpl->reactionString(rr, reactantsOrder));
        
    return pimpl->strReactions;
}

auto ChemicalReactions::printReactions(std::ostream &stream, bool reactantsOrder) -> void
{
    std::string reactants, products;

    for (const auto& rr : pimpl->reactions)
        stream << pimpl->reactionString(rr, reactantsOrder) << std::endl;;
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

auto ChemicalReactions::mapIndex(std::string substance) -> int
{
    IndexSubstancesMap::iterator it_;
    IndexSubstancesMap map = pimpl->iColFormulasMap;
    for (it_ = map.begin(); it_ != map.end(); ++it_ )
        if (it_->second == substance)
            return it_->first;

    return -1;
}

auto ChemicalReactions::getCharge(std::string substance) -> int
{
    auto index = mapIndex(substance);

    if ( index < 0 )
    {
        ChemicalFun::errorSubstanceNotFound(substance, "Database::getCharge", __LINE__);
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
    auto substMap = pimpl->iColFormulasMap;
//    iColFormulasMap.insert( std::pair<Index,std::string>(i,substanceFormulasList[i]) );
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
