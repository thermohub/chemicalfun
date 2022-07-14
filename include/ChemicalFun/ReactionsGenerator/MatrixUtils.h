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

#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

// Eigen includes
#ifdef if _MSC_VER >= 1929
#include "Eigen/Dense"
#else
#include "eigen3/Eigen/Dense"
#endif

#include <map>
#include <vector>

using Eigen::MatrixXd;
using namespace Eigen;

using ChargeCoeffMap = std::map<int, double>;

namespace ChemicalFun
{
class ElementKey;
}

namespace ReactionsGenerator {

/// Threshold viarable below which the matrix coefficients are considered 0
static const double thresholdCoeffZero = 1e-05;

///
static const double thresholdGramSchmidtWeltin = 1e-10;

/// Define a type that represents an index
using Index = std::size_t;

/// Define a type that represents a collection of indices
using Indices = std::vector<Index>;

/// Returns a vector of indices from 0 to lenght
auto indices(Index length) -> Indices;

/// Augments to the left of a matrix M(r,c) an identiy matrix I(r,r)
auto augmentIdentityMatrixLeft(MatrixXd &M) -> void;

/// Augments to the right of a matrix M(r,c) an identiy matrix I(r,r)
auto augmentIdentityMatrixRight(MatrixXd &M) -> void;

/// Formats the matrix reaction coefficients from floating point to integer
auto formatMatrixCoeffToInt(MatrixXd &M) -> void;

/// Adds rows of reaction coefficients for non master substances
auto addNonMasterCoeffRows(MatrixXd &M, unsigned r) -> void;

/// Removes the i number of master substances columns from the matrix M
auto removeMasterColls(MatrixXd &M, unsigned i) -> void;

auto formatMatrixCoeffToZero(MatrixXd &M) -> void;

auto formatMatrixCoeff(MatrixXd &M) -> void;

auto formatVectorCoeffToZero(Eigen::VectorXd M) -> Eigen::VectorXd;

auto formatVectorCoeff(Eigen::VectorXd M) -> Eigen::VectorXd;

auto stoichiometryMatrix(std::vector<std::vector<double>> vMatrix) -> Eigen::MatrixXd;

auto stoichiometryMatrix(const std::vector<std::string>& vFormulalist, bool valence=false) -> Eigen::MatrixXd;

struct ReactionChargesMap
{
    ChargeCoeffMap reactantsChargeMap;
    ChargeCoeffMap productsChargeMap;
};

template <typename Map>
bool map_compare (Map const &lhs, Map const &rhs) {
    // No predicate needed because there is operator== for pairs already.
    return lhs.size() == rhs.size()
        && std::equal(lhs.begin(), lhs.end(),
                      rhs.begin());
}

}

#endif // MATRIXUTILS_H
