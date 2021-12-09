// ChemicalFun is a C++ and Python library for of C++ and Python API
// for Chemical Formula Parser and Reactions Generator.
//
// Copyright (C) 2021 G.D.Miron, D.Kulik, S.Dmytriieva
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

// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
namespace py = pybind11;

#include "ChemicalFun/FormulaParser/ChemicalData.h"
using namespace ChemicalFun;

void exportChemicalData(py::module& m)
{
    py::class_<ElementKey>(m, "ElementKey")
        .def(py::init<>())
        .def(py::init<std::string, int, int>())
        .def(py::init<std::string>())
        .def("Symbol", &ElementKey::Symbol)
        .def("Class", &ElementKey::Class)
        .def("Isotope", &ElementKey::Isotope)
        .def("from_json_string", &ElementKey::from_json_string)
        .def("to_json_string", &ElementKey::to_json_string)
        .def("__repr__", [](const ElementKey& self) { std::stringstream ss; ss << self.to_string(); return ss.str(); })
        ;
}
