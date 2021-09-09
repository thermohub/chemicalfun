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
