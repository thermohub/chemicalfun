# xGEMS is a C++ and Python library for thermodynamic modeling by Gibbs energy minimization
#
# Copyright (C) 2018 Allan Leal, Dmitrii Kulik
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
#from numpy import *


#ElementKey
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
form_values = FormulaValues(element, 1,2)

print(form_values.valence, form_values.key )
print(form_values)

#FormulaProperites
form_prop = FormulaProperites()
print(form_prop)
