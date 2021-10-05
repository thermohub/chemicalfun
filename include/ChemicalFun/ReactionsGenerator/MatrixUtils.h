#ifndef MATRIXUTILS_H
#define MATRIXUTILS_H

// Eigen includes
#include "eigen3/Eigen/Dense"

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

auto getStoichiometryMatrix( std::vector<std::vector<double>> vMatrix) -> Eigen::MatrixXd;

auto calcStoichiometryMatrix(const std::vector<std::string>& vFormulalist,
                            const std::vector<ChemicalFun::ElementKey>& elemens_list) -> Eigen::MatrixXd;

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
