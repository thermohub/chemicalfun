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

#ifndef REACTIONGEN_H
#define REACTIONGEN_H

// C++ includes
#include <memory>

// ReactionGenerator includes
#include "MatrixUtils.h"

namespace ReactionsGenerator {

class DatabaseGenerator;

class Reaction
{
public:
    /// Construct a default Reaction instance.
    Reaction();

    /// Construct a Reaction instance with given reaction coefficients, indices of substances in the vecotr of coeff,
    /// and vector of charges for substances.
    Reaction(VectorXd coefficients, Indices iSubstances, DatabaseGenerator *DatabaseGenerator, std::string idReac_ );

    /// Construct a copy of a Reaction instance.
    Reaction(const Reaction& other);

    /// Destroy this Reaction instance.
    virtual ~Reaction();

    /// Assign a Reaction instance to this.
    auto operator=(Reaction other) -> Reaction&;

    auto chargePattern  ( ) -> ChargeCoeffMap;

    VectorXd coefficients () const;

    auto isIsocoulombic ( ) -> bool;

    auto isIsoelectric  ( ) -> bool;

    auto id  ( ) -> std::string;

    auto setCoefficients (const VectorXd &coefficients) -> void;

    auto updateChPattern( )-> void;

    Reaction& operator+=(const Reaction& rhs);
    friend Reaction operator+(Reaction lhs, const Reaction& rhs)
    {
        lhs += rhs;
        return lhs;
    }
    Reaction& operator*=(double aa );
    friend Reaction operator*(Reaction lhs, double aa )
    {
        lhs *= aa;
        return lhs;
    }
    //const Reaction operator*( double aa ) const;
    //const Reaction operator+(const Reaction &rhs) const;

    bool operator==( const Reaction &b ) const;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;

};


}

#endif // REACTION_H
