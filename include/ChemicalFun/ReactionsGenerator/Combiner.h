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

#ifndef COMBINER_H
#define COMBINER_H

// C++ includes
#include <memory>

// ReactionGenerator includes
#include "MatrixUtils.h"

namespace ReactionsGenerator {

class Reaction;

using Reactions                 = std::vector<Reaction>;
using CoefficientReactionPair   = std::pair<double, Reaction>;
using CombinedReactions         = std::vector<CoefficientReactionPair>;
using VectorCombinedReactions   = std::vector<CombinedReactions>;

class Combiner
{
public:
    /// Construct a default Combiner instance.
    Combiner();

    /// Construct a Combiner instance with given reaction and vector of model reactions
    Combiner(Reaction reaction, Reactions modelReactions );

    /// Construct a copy of a Combiner instance.
    Combiner(const Combiner& other);

    /// Destroy this Combiner instance.
    virtual ~Combiner();

    /// Assign a Combiner instance to this.
    auto operator=(Combiner other) -> Combiner&;

    /// Returns a vector of resulted isocoulombic reactions
    auto resultReactions () -> Reactions;

    /// Returns the investigated reaction
    auto reaction () -> Reaction;

    /// Returns a vector with the same size as Reactions, containing the combined reactions and thier coefficient
    /// for each result reaction
    auto combinedReactionsList () -> VectorCombinedReactions;

    /// Does all possbile combinations between the investigated reaction and the model reactions to
    /// create isocoulombic reactions
    auto combineToISOC () -> void;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;

    auto makeISOC (Reaction reaction, CombinedReactions PairsCoeffReac, std::vector<int> canceledCharges ) -> void;

    bool pairs_compare (CombinedReactions left, CombinedReactions right);

};

}

#endif // COMBINER_H
