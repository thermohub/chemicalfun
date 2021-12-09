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

engine = ElementKey("Al", 1, 1)

T = engine.Symbol()
P = engine.Class()
b = engine.Isotope()

#engine.equilibrate(T, P, b)

#print engine
print(engine)
