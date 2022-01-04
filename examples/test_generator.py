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


symbols = [ "Ca+2", "CaOH+", "Cl-", "ClO4-", "H+", "H2", "H2@", "H2O@", "O2", "O2@", "OH-", "Portlandite" ]

formulas = [ "Ca+2", "Ca(OH)+", "Cl-", "Cl|7|O4-", "H+", "H|0|2", "H|0|2@", "H2O@", "O|0|2", "O|0|2@", "OH-", "Ca(OH)2" ]

print( elementsInFormulas(formulas, False) )
A_False = substancesStoichiometryMatrix(formulas, False)
print( A_False )

print( elementsInFormulas(formulas, True) )
A_True = substancesStoichiometryMatrix(formulas, True)
print( A_True )

# ChemicalReactions
print("ChemicalReactions")

reactionsDB = ChemicalReactions(formulas, symbols)
reactionsDB2 = reactionsDB
reactionsDB3 = ChemicalReactions(reactionsDB)

print(reactionsDB.formulaMatrix())
print(reactionsDB.sizeSubstancesMap())

print(reactionsDB.getCharge("Ca+2"))
print(reactionsDB.getCharge("ClO4-"))
print(reactionsDB.getCharge("H2O@"))
print(reactionsDB.getCharge(11))

#GeneratorT_CanonicalLe16
print("Generator")

generator= Generator()
generator.setMethod(RowReduceSmMi98)
generator.compute(reactionsDB.formulaMatrix())
generator2 =  Generator(generator)

reactions   = generator.reactionMatrix()
print("Reactions matrix")
print(reactions)
print("isubstances")
print(generator.isubstances())
print("imaster")
print(generator.imaster())
print("inonmaster")
print(generator.inonmaster())

#Reaction
print("Reaction")
iSubstances = generator.isubstances()

reac_1 = Reaction(reactions[:,1], iSubstances, reactionsDB, "")
reac_1.updateChPattern()
print( reac_1.isIsocoulombic())
print( reac_1.isIsoelectric())
print( reac_1.chargePattern())
print( reac_1.coefficients())

reac_n = Reaction(reactions[:,3], iSubstances, reactionsDB, "")
print( reac_n.isIsocoulombic())
print( reac_n.isIsoelectric())
print( reac_n.chargePattern())
print( reac_n.coefficients())

reac_s = reac_n + reac_1
print( reac_s.isIsocoulombic())
print( reac_s.isIsoelectric())
print( reac_s.chargePattern())
print( reac_s.coefficients())

reac_d = reac_1 * 2.5
print( reac_d.isIsocoulombic())
print( reac_d.isIsoelectric())
print( reac_d.chargePattern())
print( reac_d.coefficients())


