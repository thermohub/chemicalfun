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

#ifndef GENERATOR_H
#define GENERATOR_H

// C++ includes
#include <memory>

// ReactionGenerator includes
#include "MatrixUtils.h"

namespace ReactionsGenerator {

enum Method { GramSchmidt, GramSchmidtWe94, RowReduceSmMi98, CanonicalLe16 };

class Generator
{
public:
    /// Construct a default Generator instance.
    Generator();

    /// Construct a Generator instance with given formula matrix.
    Generator(MatrixXd A);

    /// Construct a copy of a Generator instance.
    Generator(const Generator& other);

    /// Destroy this Generator instance.
    virtual ~Generator();

    /// Assign a Generator instance to this.
    auto operator=(Generator other) -> Generator&;

    /// Return the generated reaction metrix
    auto reactionMatrix() const -> const MatrixXd&;

    /// Return the indices of the substances in the reaction matrix.
    auto isubstances() const -> Indices;

    /// Return the indices of the master substances.
    auto imaster() const -> Indices;

    /// Return the indices of the non-master substances.
    auto inonmaster() const -> Indices;

    /// Set reaction generator method
    auto setMethod(const Method& method) -> void;

    /// Computes the reaction matrix
    auto compute(MatrixXd A) -> void;

    auto swap(Index iMaster, Index iNonMaster) -> void;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;
};


}

#endif // GENERATOR_H
