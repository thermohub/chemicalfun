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

#ifndef ROWREDUCESMITHMISSEN98_H
#define ROWREDUCESMITHMISSEN98_H

#include "ChemicalFun/ReactionsGenerator/MatrixUtils.h"

namespace ReactionsGenerator {

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
