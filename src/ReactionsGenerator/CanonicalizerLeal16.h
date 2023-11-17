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

#ifndef CANONICALIZERLEAL16_H
#define CANONICALIZERLEAL16_H

#include "ChemicalFun/ReactionsGenerator/MatrixUtils.h"

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
