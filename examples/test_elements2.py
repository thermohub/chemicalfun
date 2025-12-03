# ChemicalFun is a C++ and Python library
# for Chemical Formula Parser and Reactions Generator.
#
# Copyright (C) 2021 G.D.Miron, D.Kulik, S.Dmytriieva
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

from chemicalfun import *


dbElements_str = """[
{      "element" :   {
           "symbol" :   "C",
           "class_" :   0,
           "isotope" :   0
      },
      "properties" :   {
           "recid" :   "",
           "atomic_mass" :   12.0108003616333,
           "entropy" :   5.73999977111816,
           "heat_capacity" :   8.68200016021729,
           "volume" :   4.58440017700195,
           "valence" :   4,
           "number" :   6,
           "name" :   "C"
      }
 },
 {
      "element" :   {
           "symbol" :   "Ca",
           "class_" :   0,
           "isotope" :   0
      },
      "properties" :   {
           "recid" :   "",
           "atomic_mass" :   40.0779991149902,
           "entropy" :   41.5900001525879,
           "heat_capacity" :   25.9290008544922,
           "volume" :   25.8600006103516,
           "valence" :   2,
           "number" :   20,
           "name" :   "Ca"
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
           "symbol" :   "Mg",
           "class_" :   0,
           "isotope" :   0
      },
      "properties" :   {
           "recid" :   "",
           "atomic_mass" :   24.3050003051758,
           "entropy" :   32.6699981689453,
           "heat_capacity" :   24.8689994812012,
           "volume" :   13.996000289917,
           "valence" :   2,
           "number" :   12,
           "name" :   "Mg"
      }
 },
 {
      "element" :   {
           "symbol" :   "Mn",
           "class_" :   0,
           "isotope" :   0
      },
      "properties" :   {
           "recid" :   "",
           "atomic_mass" :   54.9379997253418,
           "entropy" :   32.0099983215332,
           "heat_capacity" :   26.3199996948242,
           "volume" :   7.39400005340576,
           "valence" :   2,
           "number" :   25,
           "name" :   "Mn"
      }
 },
 {
      "element" :   {
           "symbol" :   "Na",
           "class_" :   0,
           "isotope" :   0
      },
      "properties" :   {
           "recid" :   "",
           "atomic_mass" :   22.9897994995117,
           "entropy" :   51.2999992370605,
           "heat_capacity" :   28.2299995422363,
           "volume" :   23.7999992370605,
           "valence" :   1,
           "number" :   11,
           "name" :   "Na"
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
           "symbol" :   "S",
           "class_" :   0,
           "isotope" :   0
      },
      "properties" :   {
           "recid" :   "",
           "atomic_mass" :   32.0670013427734,
           "entropy" :   31.7980003356934,
           "heat_capacity" :   22.7630004882812,
           "volume" :   15.5100002288818,
           "valence" :   6,
           "number" :   16,
           "name" :   "S"
      }
 }
]"""

#DBElements

update_loggers(True, "test2.log", 1)

all_elements = DBElements()
all_elements.readElements(dbElements_str)

token = FormulaToken("CaC2");
print("\nCalculate the charge based on the elements and their default or specified valence")
print( token.properties(all_elements.elements(), False) )

print("\nTake the charge based on the symbol in the given formula")
print( token.properties(all_elements.elements(), True) )


formulalist = [ "CaC2", "Na2S", "Na2SO3", "Mn2O3", "MnO2", "Mn2O7" ]

print("\nCalculate the charge based on charge_from_formula = ",  charge_from_formula())
print( all_elements.formulasProperties(formulalist) )

set_charge_from_formula(True)
print("\n\nCalculate the charge based on  charge_from_formula = ", charge_from_formula())

print("\n token properties")
print( token.properties(all_elements.elements()) )

print("\n one properties from elements")
print( all_elements.formulasProperties("CaC2") )

print("\n properties from elements")
print( all_elements.formulasProperties(formulalist) )

print("\n properties CSV")
print( all_elements.formulasPropertiesCSV(formulalist) )
