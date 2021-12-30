# ChemicalFun is a C++ and Python library 
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


dbElements_str = """[
      {
          "element" :   {
               "symbol" :   "Al",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   26.9815406799316,
               "entropy" :   28.2999992370605,
               "heat_capacity" :   24.2000007629395,
               "volume" :   9.99300003051758,
               "valence" :   3,
               "number" :   13,
               "name" :   "Al"
          }
     },
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
               "symbol" :   "Fe",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   55.8450012207031,
               "entropy" :   27.2800006866455,
               "heat_capacity" :   24.9610004425049,
               "volume" :   7.09200000762939,
               "valence" :   2,
               "number" :   26,
               "name" :   "Fe"
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
               "symbol" :   "K",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   39.0983009338379,
               "entropy" :   64.6799926757812,
               "heat_capacity" :   29.6000003814697,
               "volume" :   45.4630012512207,
               "valence" :   1,
               "number" :   19,
               "name" :   "K"
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
               "symbol" :   "Si",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   28.0855007171631,
               "entropy" :   18.8099994659424,
               "heat_capacity" :   19.7889995574951,
               "volume" :   12.0559997558594,
               "valence" :   4,
               "number" :   14,
               "name" :   "Si"
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

#DBElements

all_elements = DBElements()
all_elements.readElements(dbElements_str);

print("DBElements")
print(all_elements.elementsKeys())
print("")
print(all_elements.elementsKeysList())
print("")
print(all_elements.elements())
print("")
print(all_elements.writeElements())
print("")
print(all_elements.CSV())
print("")
print(all_elements.formulasProperties("Al(OH)2+"))

formula_list = [
    "H2O",
    "H2O@",
    "H+",
    "OH-",
    "H|0|2",
    "O|0|2",
    "HOO|0|-",
]

print("")
print(all_elements.formulasProperties(formula_list))
print("")
print(all_elements.stoichiometryMatrix(formula_list))
print("")
print(all_elements.CSV())
print("")
print(all_elements.formulasPropertiesCSV(formula_list))
print("")
print(all_elements.stoichiometryMatrixCSV(formula_list))

# functions

test_elments = elementsInFormulas(formula_list);
print(test_elments)
print(substancesStoichiometryMatrix(formula_list))

# addElement

el_key = ElementKey("Vol", 5)
el_val = ElementValues()
el_val.from_json_string("""{
"atomic_mass": 0.0,
"entropy": 0.0,
"heat_capacity": 0.0,
"name": "Vol",
"number": 100,
"recid": "",
"valence": 0,
"volume": 1.0
}""")
all_elements.addElement(el_key, el_val)
print("")
print(all_elements.elementsKeys())

#FormulaToken
print("")
print("FormulaToken")
print("")

form_tok = FormulaToken("Al(OH)2+")
print(form_tok.formula())
print(form_tok.stoichCoefficients())
print(form_tok.elementsList())
print(form_tok.charge())
#form_tok.testChargeImbalance(all_elements.elements())  exception
print(form_tok.stoichiometryRow(all_elements.elementsKeysList()))
print(form_tok.properties(all_elements.elements()))
print(form_tok.testElements("Am|3|(Oxa)3-3", all_elements.elementsKeys()))
print(form_tok.checkElements("Am|3|(Oxa)3-3", all_elements.elementsKeys()))

el_lst = form_tok.parsed_list()
print(el_lst)

form_tok.setFormula("FeS|0|S|-2|", 1);
el_lst = form_tok.parsed_list()
print(el_lst)

#ElementKey
print("ElementKey")
print("")
element = ElementKey("Al", 1)
s = element.Symbol()
c = element.Class()
ist = element.Isotope()
str = element.to_json_string()
element.from_json_string('{"symbol" :"Zz","class_":4}')
print(s,c, ist)
print(str)
print(element)

#ElementValues
print("ElementValues")
print("")
el_val = ElementValues()
print(el_val)
el_val.from_json_string("""{
"recid" :   "",
"atomic_mass" :   26.9815406799316,
"entropy" :   28.2999992370605,
"heat_capacity" :   24.2000007629395,
"volume" :   9.99300003051758,
"valence" :   3,
"number" :   13,
"name" :   "Al"
}""")
print(el_val.heat_capacity)
print(el_val)

#FormulaValues
print("FormulaValues")
print("")
form_values = FormulaValues(element, 1,2)
print(form_values.valence, form_values.key )
print(form_values)

#FormulaProperties
print("FormulaProperties")
#print("")
form_prop = FormulaProperties()
print(form_prop)
