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

#ifndef DATABASEGEN_H
#define DATABASEGEN_H

// C++ includes
#include <memory>

#include "MatrixUtils.h"

namespace ReactionsGenerator {

using IndexSubstancesMap    = std::map<Index, std::string>;
using IndexElementsMap      = std::map<Index, std::string>;

class Generator;

class ChemicalReactions
{
public:
    /// Construct a default Database instance.
    ChemicalReactions();

    /// Construct a Database instance with given formula matrix.
    ChemicalReactions(std::vector<std::vector<double>> A);

    /// Construct a Database instance with given formula matrix and substances (columns) list.
    ChemicalReactions(MatrixXd A, std::vector<std::string> substancesList);

    /// Construct a copy of a Database instance.
    ChemicalReactions(const ChemicalReactions &other);

    /// Destroy this Database instance.
    virtual ~ChemicalReactions();

    /// Assign a Database instance to this.
    auto operator=(ChemicalReactions other) -> ChemicalReactions&;

    /// Returns the substances formula matrix
    auto formulaMatrix () -> MatrixXd;

    auto sizeSubstancesMap() -> size_t;

    /// Generates reactions
//    auto generateReactions(MatrixXd &Reactions, Indices &iSubstances, Indices &iMaster) -> void;

//    /// Returns and instantce of the reactions generator
//    auto reactionGenerator() -> Generator;

    /// Outputs reactions in txt file
//    auto outToTxtReactions(std::string filename) -> void;

//    auto swap(std::string oldMaster, std::string newMaster) -> void;

    /// Returns the charge of the substance
    /// @param symbol substance symbol
    auto getCharge(std::string symbol) -> int;

    /// Returns the charge of the substance
    /// @param index index of the substance in the formula matrix
    auto getCharge(Index index) -> int;

    /**
     * @brief checkReacMatrixISOC checks if reactions in the reaction matrix are isocoulombic
     * @param reactionMatrix matrix of reactions on columns
     * @param iSubstances indices of substances for rows 0,1..,i from the substances map
     * @return a vector of size -number of reactions with true if the reaction is isocoulombic and false if not
     */
    auto checkReacMatrixISOC(MatrixXd reactionMatrix, Indices iSubstances) -> std::vector<bool>;

    /**
     * @brief checkReacMatrixISOE checks if reactions in the reaction matrix are isocoulombic
     * @param reactionMatrix matrix of reactions on columns
     * @param iSubstances indices of substances for rows 0,1..,i from the substances map
     * @return a vector of size -number of reactions with true if the reaction is isocoulombic and false if not
     */
    auto checkReacMatrixISOE(MatrixXd reactionM, Indices iSubstances) -> std::vector<bool>;

//    auto setFormulaMatrix () -> void;

//    auto setSubstances () -> void;

//    auto setElements () -> void;

//    auto reduceFormulaMatrix () -> void;

private:

    /// Cleans the formula matrix of element rows not present in the work subsystem
    auto eraseZeroRowsFormulaMatrix ( ) -> void;

    /// Returns a vector of all reactions with their maps of reactants / products and thier charges
    auto reactionsChargesMap(MatrixXd reactionMatrix, Indices iSubstances) -> std::vector<ReactionChargesMap>;

    /// Returns the index of the column for a substance from the formula matrix
    auto mapIndex(std::string symbol) -> int;

    struct Impl;

    std::unique_ptr<Impl> pimpl;
};

}

#endif // DATABASEGEN_H
