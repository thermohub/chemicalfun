#ifndef REACTIONGEN_H
#define REACTIONGEN_H

// C++ includes
#include <memory>

// ReactionGenerator includes
#include "MatrixUtils.h"

namespace ChemicalFun {

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

    auto operator*( double aa ) -> Reaction;

    auto operator+( Reaction b) -> Reaction;

    bool operator==( const Reaction &b ) const;


private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;

};


}

#endif // REACTION_H
