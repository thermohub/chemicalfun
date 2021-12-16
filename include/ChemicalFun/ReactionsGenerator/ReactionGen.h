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
