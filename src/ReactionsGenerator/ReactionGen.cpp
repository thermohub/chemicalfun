#include "ReactionGen.h"
#include "DatabaseGenerator.h"

namespace ReactionsGenerator {

struct Reaction::Impl
{
    DatabaseGenerator *db;

    VectorXd reacCoeff;

    Indices iSubst;

    ChargeCoeffMap chPattern;

    std::string idReac;

    Impl()
    {}

    Impl(VectorXd coefficients, Indices iSubstances, DatabaseGenerator* databaseGenerator, std::string idReac_)
    {
        reacCoeff = coefficients;
        iSubst    = iSubstances;
        db        = databaseGenerator;
        idReac    = idReac_;
    }

};

Reaction::Reaction()
: pimpl(new Impl())
{}

Reaction::Reaction(VectorXd coefficients, Indices iSubstances, DatabaseGenerator *databaseGenerator, std::string idReac_)
: pimpl(new Impl(coefficients, iSubstances, databaseGenerator, idReac_))
{this->updateChPattern();}

Reaction::Reaction(const Reaction& other)
: pimpl(new Impl(*other.pimpl))
{}

Reaction::~Reaction()
{}

auto Reaction::operator=(Reaction other) -> Reaction&
{
    pimpl = std::move(other.pimpl);
    return *this;
}

VectorXd Reaction::coefficients () const
{
    return pimpl->reacCoeff;
}

auto Reaction::id () -> std::string
{
    return pimpl->idReac;
}

auto Reaction::setCoefficients (const VectorXd &coefficients) -> void
{
    pimpl->reacCoeff = coefficients;
}

auto Reaction::chargePattern () -> ChargeCoeffMap
{
    return pimpl->chPattern;
}

auto Reaction::operator*(double aa) -> Reaction
{
    this->setCoefficients((this->coefficients()*aa));
    this->updateChPattern();
    return *this;
}

auto Reaction::operator+( Reaction b) -> Reaction
{
    this->setCoefficients((this->coefficients()+b.coefficients()));
    this->updateChPattern();
    return *this;
}

bool Reaction::operator==( const Reaction &b) const
{
    return (this->coefficients() == b.coefficients());
}

auto Reaction::updateChPattern( )-> void
{
    ChargeCoeffMap::iterator it;
    auto db = pimpl->db;
    ChargeCoeffMap chPattern /*= pimpl->chPattern*/;
    auto iSubst = pimpl->iSubst;
    auto reacCoeff = pimpl->reacCoeff;

    int charge;
    for (int i=0; i<reacCoeff.size(); i++ )
    {
        charge = db->getCharge(iSubst[i]);
        it = chPattern.find(charge);
        if (it != chPattern.end()) // found
        {
            chPattern[charge] += reacCoeff[i];
        } else
        {
            chPattern[charge]  = reacCoeff[i];
        }
    }

    // delete charges with coeff 0

    for (it = chPattern.begin(); it != chPattern.end(); )
        if ((it->second == 0) || (it->first == 0) ||
//            (it->second > 100)|| (it->first > 100 ||
            (it->second > -0.00009 && it->second < 0.00009)/*|| (it->first < -100)*/)
        {
            chPattern.erase(it++);
        } else
        {
            it++;
        }
    pimpl->chPattern = chPattern;
}

auto Reaction::isIsocoulombic( ) -> bool
{
    if (pimpl->chPattern.empty())
        return true;
    else
        return false;
}

auto Reaction::isIsoelectric( ) -> bool
{
    auto db = pimpl->db;
    auto iSubst = pimpl->iSubst;
    auto reacCoeff = pimpl->reacCoeff;

//    auto posChProducts = 0;
//    auto posChReactants = 0;
//    auto negChProducts = 0;
//    auto negChReactants = 0;

    auto posChargeSum = 0;
    auto negChargeSum = 0;

    for (int i=0; i<reacCoeff.size(); i++ )
    {
        int charge;
        charge = db->getCharge(iSubst[i]);

        if (reacCoeff[i] != 0)
        {
            if (charge < 0)
                negChargeSum += reacCoeff[i]*charge;
            if (charge > 0)
                posChargeSum += reacCoeff[i]*charge;
        }


//        if (reacCoeff[i] < 0) // reactants
//        {
//            if (charge < 0)
//                negChReactants += abs(reacCoeff[i]*charge);
//            if (charge > 0)
//                posChReactants += abs(reacCoeff[i]*charge);
//        }
//        if (reacCoeff[i] > 0) // products
//        {
//            if (charge < 0)
//                negChProducts += abs(reacCoeff[i]*charge);
//            if (charge > 0)
//                posChProducts += abs(reacCoeff[i]*charge);
//        }
    }

//    if ((negChReactants == negChProducts) && (posChReactants == posChProducts))
//        return true;
//    else
//        return false;

    if ((posChargeSum == 0) && (negChargeSum == 0))
        return true;
    else
        return false;


}


}
