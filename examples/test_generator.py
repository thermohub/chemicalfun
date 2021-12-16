# ChemicalFun is a C++ and Python library for of C++ and Python API
# for Chemical Formula Parser and Reactions Generator.
#
# Copyright (C) 2021 G.D.Miron, D.Kulik, S.Dmytriieva
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

from chemicalfun import *
from numpy import *


dbElements_str = """[
     {
          "element" :   {
               "symbol" :   "Cl",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   35.4529991149902,
               "entropy" :   111.540496826172,
               "heat_capacity" :   16.9745006561279,
               "volume" :   12395,
               "valence" :   -1,
               "number" :   17,
               "name" :   "Cl"
          }
     },
     {
          "element" :   {
               "symbol" :   "H",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   1.00794994831085,
               "entropy" :   65.3399963378906,
               "heat_capacity" :   14.4090003967285,
               "volume" :   12395,
               "valence" :   1,
               "number" :   1,
               "name" :   "H"
          }
     },
     {
          "element" :   {
               "symbol" :   "O",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   15.999400138855,
               "entropy" :   102.569000244141,
               "heat_capacity" :   14.66100025177,
               "volume" :   12395,
               "valence" :   -2,
               "number" :   8,
               "name" :   "O"
          }
     },
     {
          "element" :   {
               "symbol" :   "Zz",
               "class_" :   4,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   0,
               "entropy" :   -65.3399963378906,
               "heat_capacity" :   -14.4180002212524,
               "volume" :   0,
               "valence" :   0,
               "number" :   0,
               "name" :   "Zz"
          }
    }
]"""

formula_list = [
    "H2O",
    "H2O@",
    "H+",
    "OH-",
    "H|0|2",
    "O|0|2",
    "HOO|0|-",
]

#DBElements

all_elements = DBElements()
all_elements.readElements(dbElements_str);

print("DBElements:", all_elements.getElementsKeys())

st_matrix =  calcStoichiometryMatrix(formula_list, True)
