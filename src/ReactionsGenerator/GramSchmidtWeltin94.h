#ifndef GRAMSCHMIDTWELTIN94_H
#define GRAMSCHMIDTWELTIN94_H

#include "ChemicalFun/ReactionsGenerator/MatrixUtils.h"

namespace ReactionsGenerator {

/**
 * @brief weltin94 Given a substances formula matrix M[rows-elements, cols-substances], computes
 * the reaction matrix M[rows-substances, cols-reactions (staring from index iMaster.size()-1)],
 * and the indices of master and non-master substances.
 * @ref Weltin (1994)
 * @param M initial formula matrix and computed reaction matrix
 * @param iMaster indices of master substances
 * @param iNonMaster indices of non-master substances
 */
auto weltin94(MatrixXd &M, Indices &iNonMaster) -> MatrixXd;

}

#endif // GRAMSCHMIDTWELTIN94_H
