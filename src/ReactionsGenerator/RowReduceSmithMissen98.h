#ifndef ROWREDUCESMITHMISSEN98_H
#define ROWREDUCESMITHMISSEN98_H

#include "ChemicalFun/ReactionsGenerator/MatrixUtils.h"

namespace ChemicalFun {

/**
 * @brief smithMissen98 Given a substances formula matrix M[rows-elements, cols-substances], computes
 * the reaction matrix M[rows-substances, cols-reactions (staring from index iMaster.size()-1)],
 * and the indices of master and non-master substances.
 * @ref Smith and Missen (1998)
 * @param M initial formula matrix and computed reaction matrix
 * @param iMaster indices of master substances
 * @param iNonMaster indices of non-master substances
 */
auto smithMissen98(MatrixXd &M, Indices &iMaster, Indices &iNonMaster)-> MatrixXd;

//auto rowReduce(MatrixXd &M)-> void;

}

#endif // ROWREDUCESMITHMISSEN98_H
