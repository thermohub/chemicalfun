#ifndef CANONICALIZERLEAL16_H
#define CANONICALIZERLEAL16_H

#include "MatrixUtils.h"

namespace ReactionsGenerator {

/**
 * @brief leal16 Given a substances formula matrix M[rows-elements, cols-substances], computes
 * the reaction matrix M[rows-substances, cols-reactions (staring from index iMaster.size()-1)],
 * and the indices of master and non-master substances.
 * @ref Leal (2016) - Optima library
 * @param M initial formula matrix and computed reaction matrix
 * @param iMaster indices of master substances
 * @param iNonMaster indices of non-master substances
 */
auto leal16(MatrixXd &M, Indices &iMaster, Indices &iNonMaster) -> MatrixXd;

}

#endif // CANONICALIZERLEAL16_H
