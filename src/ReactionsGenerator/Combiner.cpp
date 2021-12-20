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

#include "ChemicalFun/ReactionsGenerator/Combiner.h"
#include "ChemicalFun/ReactionsGenerator/Reaction.h"

namespace ReactionsGenerator {

struct Combiner::Impl
{

    /// Reaction to investigate
    Reaction reaction;

    /// vector of model reactions
    Reactions modelReactions;

    /// vector of resulted reactions
    Reactions resultReactions;

    /// vector of pairs of coefficients and model reactions used to make the resultReactions
    VectorCombinedReactions combinedReactionsList;

    Impl()
    {}

    Impl(Reaction reaction_, Reactions modelReactions_ )
    {
        reaction        = reaction_;
        modelReactions  = modelReactions_;
    }

};

Combiner::Combiner()
: pimpl(new Impl())
{}

Combiner::Combiner(Reaction reaction, Reactions modelReactions )
: pimpl(new Impl(reaction, modelReactions ))
{}

Combiner::Combiner(const Combiner& other)
: pimpl(new Impl(*other.pimpl))
{}

Combiner::~Combiner()
{}

auto Combiner::operator=(Combiner other) -> Combiner&
{
    pimpl = std::move(other.pimpl);
    return *this;
}

auto Combiner::resultReactions () -> Reactions
{
    return pimpl->resultReactions;
}

auto Combiner::reaction () -> Reaction
{
    return pimpl->reaction;
}

auto Combiner::combinedReactionsList () -> VectorCombinedReactions
{
    return pimpl->combinedReactionsList;
}

auto Combiner::combineToISOC () -> void
{
    CombinedReactions pairsCoeffReac;
    std::vector<int> canceledCharges;

    if (pimpl->reaction.chargePattern().empty()) // already isocoulombic
        return;

    makeISOC(pimpl->reaction, pairsCoeffReac, canceledCharges);
}

auto Combiner::makeISOC ( Reaction reaction, CombinedReactions combinedReactions, std::vector<int> canceledCharges) -> void
{
    auto chPattern              = reaction.chargePattern(); // Charge patter of curent reaction
    auto modelReactions         = pimpl->modelReactions;    // vector of model reactions
    auto combinedReactions_     = combinedReactions;        // combined reactions with their coefficient
    auto canceledCharges_       = canceledCharges;          // charges already canceled with previous combined reactions

    if (combinedReactions.size() > 10)
        return;

    ChargeCoeffMap::iterator itCharge;                      // itterator through the charge pattern map

    for (itCharge = chPattern.begin(); itCharge != chPattern.end(); ++itCharge )    // loop through reaction charge pattern
    {
        auto charge = itCharge->first;
        for (size_t i = 0; i<modelReactions.size(); i++)                              // loop through model reactions, test combining each model reaction
        {
            if (reaction.id() != modelReactions[i].id())
            {
                auto modelChPattern     = modelReactions[i].chargePattern();
                auto chFound            = modelChPattern.find(charge);
                bool isCanceledCharge   = false;

                // check if current model reaction doesn't have a charge which was already canceled
                for (size_t j = 0; j < canceledCharges_.size(); j++)
                    if ((modelChPattern.find(canceledCharges_[j])) != modelChPattern.end())
                        isCanceledCharge = true;

                if ((chFound != modelChPattern.end()) && (!isCanceledCharge))   // if charge found and the model reaction does not contain any charge canceled before
                {
                    auto coeff = -itCharge->second/chFound->second;             // calculate how much of the model reaction has to be added to cancel the charge
                    Reaction newReac = modelReactions[i];
                    newReac = newReac*coeff;
                    newReac = newReac + reaction;                               // calculate new reaction resulted from adding to the current reaction with the model reaction*coeff

                    combinedReactions_.push_back(std::make_pair(coeff, modelReactions[i]));

                    // check which charges were canceld by combining the ith model reaction and stores them
                    auto chPatternNew = newReac.chargePattern();
                    ChargeCoeffMap::iterator itCharge2;
                    for (itCharge2 = chPattern.begin(); itCharge2 != chPattern.end(); ++itCharge2 ) // loop through reaction charge pattern
                        if ((chPatternNew.find(itCharge2->first) == chPatternNew.end())) // if charge is not present anymore in the new reaction charge pattern -> was canceled
                            canceledCharges_.push_back(itCharge2->first);

                    makeISOC(newReac, combinedReactions_, canceledCharges_);    // recursive call of the function with the new reaction and the lists of
                    combinedReactions_  = combinedReactions;                    // combined reactions and canceled charges
                    canceledCharges_    = canceledCharges;
                }
            }
        }
    }

    if (chPattern.empty()) // check if the reaction is isocoulombic
    {
        auto found = false;
        for (size_t i = 0; i< pimpl->combinedReactionsList.size(); i++) // check if the combination of model reactions is unique
            if (pairs_compare(pimpl->combinedReactionsList[i], combinedReactions_ ))
                found = true;

        if(!found)          // if the reaction is isocoulombic and the combination of model reactions is unique
        {
            pimpl->resultReactions.push_back(reaction);
            pimpl->combinedReactionsList.push_back(combinedReactions_);
        }
    }
}

bool double_equals(double a, double b)
{
    return std::abs(a - b) < 0.00001;
}

bool Combiner::pairs_compare (CombinedReactions left, CombinedReactions right)
{
    if (left.size() != right.size())
        return false;

    std::vector<bool> found; found.resize(left.size(), false);

    size_t i = 0;
    for (auto itl = left.begin(); itl != left.end(); itl++)
    {
        for (auto itr = right.begin(); itr != right.end(); itr++)
            if ((double_equals(itl->first, itr->first)/*itl->first == itr->first*/) && (itl->second == itr->second) )
                found[i] = true;
        if (!found[i])
            return false;
        i++;
    }

    return true;
}

}
