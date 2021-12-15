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

#include "ChemicalFun/FormulaParser/ChemicalData.h"
using namespace ChemicalFun;
// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
namespace py = pybind11;
using namespace pybind11::literals;


void exportChemicalData(py::module& m)
{
    py::class_<ElementKey>(m, "ElementKey", py::is_final())
            .def(py::init<>())
            .def(py::init<std::string, int, int>(), "asymbol"_a, "aclass"_a, "aisotope"_a=0 )
            .def(py::init<std::string>())
            .def("Symbol", &ElementKey::Symbol)
            .def("Class", &ElementKey::Class)
            .def("Isotope", &ElementKey::Isotope)
            .def("from_json_string", &ElementKey::from_json_string )
            .def("to_json_string", &ElementKey::to_json_string, py::arg("dense")  = false )
            .def("__repr__", [](const ElementKey& self) { std::stringstream ss; ss << self.to_string(); return ss.str(); })
            ;

    py::class_<ElementValues>(m, "ElementValues")
            .def(py::init<>())
            .def_readwrite("recid", &ElementValues::recid)
            .def_readwrite("name", &ElementValues::name)
            .def_readwrite("number", &ElementValues::number)
            .def_readwrite("valence", &ElementValues::valence)
            .def_readwrite("atomic_mass", &ElementValues::atomic_mass)
            .def_readwrite("entropy", &ElementValues::entropy)
            .def_readwrite("heat_capacity", &ElementValues::heat_capacity)
            .def_readwrite("volume", &ElementValues::volume)
            .def("from_json_string", &ElementValues::from_json_string )
            .def("to_json_string", &ElementValues::to_json_string, py::arg("dense")  = false )
            .def("__repr__", [](const ElementValues& self) { std::stringstream ss; ss << self.to_json_string(); return ss.str(); })
            ;

    py::class_<FormulaValues>(m, "FormulaValues")
            .def(py::init<const ElementKey&, double, int>())
            .def_readonly("key", &FormulaValues::key)
            .def_readonly("valence", &FormulaValues::valence)
            .def_readonly("stoich_coef", &FormulaValues::stoich_coef)
            .def("to_json_string", &FormulaValues::to_json_string, py::arg("dense")  = false)
            .def("__repr__", [](const FormulaValues& self) { std::stringstream ss; ss << self.to_json_string(); return ss.str(); })
            ;

    py::class_<FormulaProperites>(m, "FormulaProperites")
            .def(py::init<>())
            .def_readonly("formula", &FormulaProperites::formula)
            .def_readonly("charge", &FormulaProperites::charge)
            .def_readonly("atomic_mass", &FormulaProperites::atomic_mass)
            .def_readonly("elemental_entropy", &FormulaProperites::elemental_entropy)
            .def_readonly("atoms_formula_unit", &FormulaProperites::atoms_formula_unit)
            .def("to_json_string", &FormulaProperites::to_json_string, py::arg("dense")  = false )
            .def("__repr__", [](const FormulaProperites& self) { std::stringstream ss; ss << self.to_json_string(); return ss.str(); })
            ;

    py::class_<FormulaToken>(m, "FormulaToken", py::is_final())
            .def(py::init<const std::string&, bool>(), py::arg("aformula"), py::arg("with_valences")  = false)
            .def("setFormula", &FormulaToken::setFormula, py::arg("aformula"), py::arg("with_valences")  = false)
            .def("formula", &FormulaToken::formula)
            .def("elementsList", &FormulaToken::getElementsList)
            .def("parsed_list", &FormulaToken::parsed_list, py::arg("dense")  = false)
            .def("charge", &FormulaToken::charge, py::arg("dbelements")  = ElementsData({}))
            .def("stoichCoefficients", &FormulaToken::getStoichCoefficients)
            .def("testCargeImbalance", &FormulaToken::testCargeImbalance)
            .def("calcProperites", &FormulaToken::calculateProperites)
            .def("stoichiometryRow", &FormulaToken::makeStoichiometryRow)
            .def("testElements", &FormulaToken::testElements)
            .def("checkElements", py::overload_cast<const std::string&, const ElementsKeys&>(&FormulaToken::checkElements))
            //.def("checkElements", py::overload_cast<const std::string&, const std::string&, const ElementsKeys&>(&FormulaToken::checkElements))
            .def("__repr__", [](const FormulaToken& self) { std::stringstream ss; ss << self.parsed_list().size(); return ss.str(); })
            ;

    py::class_<DBElements>(m, "DBElements", py::is_final())
            .def(py::init<>())
            .def_static("defaultValence", &DBElements::defaultValence)
            .def_static("extractElements", &DBElements::extractElements)
            .def("addElement", &DBElements::addElement)
            .def("getElements", &DBElements::getElements)
            .def("getElementsKeys", &DBElements::getElementsKeys)
            .def("getElementsKeysList", &DBElements::getElementsKeysList)
            .def("calcThermo",[](const DBElements& self, const std::string& data) { return self.calcThermo(data); })
            .def("calcThermo", py::overload_cast<const std::vector<std::string>&>(&DBElements::calcThermo))
            .def("stoichiometryMatrix", &DBElements::calcStoichiometryMatrix)
            .def("readElements", &DBElements::readElements)
            .def("writeElements", &DBElements::writeElements, py::arg("dense")  = false)
            .def("getCSV",[]( DBElements& self) { std::stringstream ss; self.printCSV(ss); return ss.str(); })
            .def("getThermo",[]( DBElements& self, const std::vector<std::string> &formulalist)
    { std::stringstream ss; self.printThermo(ss, formulalist); return ss.str(); })
            .def("getStoichiometryMatrix",[]( DBElements& self, const std::vector<std::string> &formulalist)
    { std::stringstream ss; self.printStoichiometryMatrix(ss, formulalist); return ss.str(); })
            .def("__repr__", []( const DBElements& self) { std::stringstream ss; ss << self.writeElements(); return ss.str(); })
            ;

    m.def("generateStoichiometryMatrixValences", &generateStoichiometryMatrixValences,
          py::arg("formulalist") , py::arg("all_elements"), py::arg("with_valences")=false);
    m.def("generateElementsListValences", &generateElementsListValences,
          py::arg("formulalist"), py::arg("with_valences")=false);
}
