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

#include "ChemicalFun/ReactionsGenerator/Generator.h"
#include "GramSchmidtWeltin94.h"
#include "RowReduceSmithMissen98.h"
#include "CanonicalizerLeal16.h"

namespace ReactionsGenerator {

struct Generator::Impl
{
    /// The reactions matrix
    MatrixXd reactionsXd;

    /// The not formated reactions matrix
    MatrixXd R;

    /// Substances indices
    Indices iSubstances;

    /// Master substances indices
    Indices iMaster;

    /// Non-Master substances indices
    Indices iNonMaster;

    /// The number of master substances
    int nMaster;

    /// The number of non-master substances (also number of independent reactions)
    int nNonMaster;

    /// Reaction genration method (default RowReduceSmMi98)
    Method method = RowReduceSmMi98;

};

Generator::Generator()
: pimpl(new Impl())
{}

Generator::Generator(MatrixXd A)
: pimpl(new Impl())
{
    compute(A);
}

Generator::Generator(const Generator& other)
: pimpl(new Impl(*other.pimpl))
{}

Generator::~Generator()
{}

auto Generator::operator=(Generator other) -> Generator&
{
    pimpl = std::move(other.pimpl);
    return *this;
}

auto Generator::reactionMatrix() const -> const MatrixXd&
{
    return pimpl->reactionsXd;
}

auto Generator::isubstances() const -> Indices
{
    return pimpl->iSubstances;
}

auto Generator::imaster() const -> Indices
{
    return pimpl->iMaster;
}

auto Generator::inonmaster() const -> Indices
{
    return pimpl->iNonMaster;
}

auto Generator::setMethod(const Method& method) -> void
{
    pimpl->method = method;
}

auto Generator::compute(MatrixXd A) -> void
{
    Indices iMaster, iNonMaster;
    MatrixXd R;
    auto method = pimpl->method;

    switch(method) {
    case GramSchmidt:
        break;
    case GramSchmidtWe94:   R = weltin94(A, /*iMaster,*/ iNonMaster);
        break;
    case RowReduceSmMi98:   R = smithMissen98(A, iMaster, iNonMaster);
        break;
    case CanonicalLe16:     R = leal16(A, iMaster, iNonMaster);
        break;
    }

    pimpl->R            = R;
    pimpl->iMaster      = iMaster;
    pimpl->iNonMaster   = iNonMaster;
    pimpl->reactionsXd  = A;
    pimpl->iSubstances  = iMaster;
    pimpl->iSubstances.insert( pimpl->iSubstances.end(), iNonMaster.begin(), iNonMaster.end() );
}

auto Generator::swap(Index iOldMaster, Index iNewMaster) -> void
{
    auto method = pimpl->method;
    auto M = pimpl->R;

    switch(method) {
    case GramSchmidt:
        break;
    case GramSchmidtWe94:
        break;
    case RowReduceSmMi98:  { M.col(iOldMaster).swap(M.col(iNewMaster)); compute(M); }
        break;
    case CanonicalLe16:
        break;
    }
}

}
