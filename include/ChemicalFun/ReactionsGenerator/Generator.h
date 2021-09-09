#ifndef GENERATOR_H
#define GENERATOR_H

// C++ includes
#include <memory>

// ReactionGenerator includes
#include "MatrixUtils.h"

namespace ChemicalFun {

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
