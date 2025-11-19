// ChemicalFun is a C++ and Python library
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

#if _MSC_VER >= 1929
#include <corecrt.h>
#endif
// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
namespace py = pybind11;
using namespace pybind11::literals;

#include "ChemicalFun/FormulaParser/ChemicalData.h"
using namespace ChemicalFun;


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

    py::class_<FormulaProperties>(m, "FormulaProperties")
        .def(py::init<>())
        .def_readonly("formula", &FormulaProperties::formula)
        .def_readonly("charge", &FormulaProperties::charge)
        .def_readonly("atomic_mass", &FormulaProperties::atomic_mass)
        .def_readonly("elemental_entropy", &FormulaProperties::elemental_entropy)
        .def_readonly("atoms_formula_unit", &FormulaProperties::atoms_formula_unit)
        .def("to_json_string", &FormulaProperties::to_json_string, py::arg("dense")  = false )
        .def("__repr__", [](const FormulaProperties& self) { std::stringstream ss; ss << self.to_json_string(); return ss.str(); })
        ;

    py::class_<FormulaToken>(m, "FormulaToken", py::is_final())
        .def(py::init<const std::string&, bool>(), py::arg("aformula"), py::arg("valence")  = false)
        .def_readwrite_static("get_charge_from_formula", &FormulaToken::get_charge_from_formula)
        .def("setFormula", &FormulaToken::setFormula, py::arg("aformula"), py::arg("valence")  = false)
        .def("formula", &FormulaToken::formula)
        .def("elementsList", &FormulaToken::getElementsList)
        .def("parsed_list", &FormulaToken::parsed_list, py::arg("dense")  = false)
        .def("charge", &FormulaToken::charge, py::arg("dbelements")  = ElementsData({}))
        .def("stoichCoefficients", &FormulaToken::getStoichCoefficients)
        .def("testChargeImbalance", &FormulaToken::testChargeImbalance)
        .def("properties", [](FormulaToken& self, const ElementsData& elements, py::object is_formula)
             {
                 bool useformula;
                 if (py::isinstance<py::none>(is_formula)) {
                     useformula = self.get_charge_from_formula;
                 } else {
                     useformula = is_formula.cast<bool>();
                 }
                 return self.properties(elements, useformula);
             },
             py::arg("elements"), py::arg("is_formula") = py::none())
        .def("stoichiometryRow", &FormulaToken::makeStoichiometryRow)
        .def("testElements", &FormulaToken::testElements)
        .def("checkElements", py::overload_cast<const std::string&, const ElementsKeys&>(&FormulaToken::checkElements))
        .def("checkElements", py::overload_cast<const std::string&, const std::string&, const ElementsKeys&>(&FormulaToken::checkElements))
        .def("__repr__", [](const FormulaToken& self) { std::stringstream ss; ss << self.parsed_list().size(); return ss.str(); })
        ;

    py::class_<DBElements>(m, "DBElements", py::is_final())
        .def(py::init<>())
        .def_static("defaultValence", &DBElements::defaultValence)
        .def_static("formulasElements", &DBElements::formulasElements)
        .def("addElement", &DBElements::addElement)
        .def("elements", &DBElements::elements)
        .def("elementsKeys", &DBElements::elementsKeys)
        .def("elementsKeysList", &DBElements::elementsKeysList)
        .def("readElements", &DBElements::readElements)
        .def("writeElements", &DBElements::writeElements, py::arg("dense")  = false)
        .def("CSV",[]( DBElements& self) { std::stringstream ss; self.printCSV(ss); return ss.str(); })

        .def("formulasPropertiesCSV", [](DBElements& self, const std::vector<std::string> &list, py::object is_formula)
             {
                 bool useformula;
                 if (py::isinstance<py::none>(is_formula)) {
                     useformula = self.charge_from_formula();;
                 } else {
                     // Otherwise, cast the provided Python object to a string.
                     useformula = is_formula.cast<bool>();
                 }
                 std::stringstream ss;
                 self.formulasPropertiesCSV(ss, list, useformula);
                 return ss.str();
             },
             py::arg("list"), py::arg("is_formula") = py::none())
        .def("stoichiometryMatrixCSV", [](DBElements& self, const std::vector<std::string> &formulalist)
             { std::stringstream ss; self.printStoichiometryMatrix(ss, formulalist); return ss.str(); })
        .def("formulasProperties", [](const DBElements& self, const std::string& data, py::object is_formula)
             {
                 bool useformula;
                 if (py::isinstance<py::none>(is_formula)) {
                     useformula = self.charge_from_formula();
                 } else {
                     // Otherwise, cast the provided Python object to a string.
                     useformula = is_formula.cast<bool>();
                 }
                 return self.formulasProperties(data, useformula);
             },
             py::arg("data"),  py::arg("is_formula") = py::none())
        .def("formulasProperties", [](DBElements& self, const std::vector<std::string>& list, py::object is_formula)
             {
                 bool useformula;
                 if (py::isinstance<py::none>(is_formula)) {
                     useformula = self.charge_from_formula();;
                 } else {
                     // Otherwise, cast the provided Python object to a string.
                     useformula = is_formula.cast<bool>();
                 }
                 return self.formulasProperties(list, useformula);
             },
             py::arg("list"),  py::arg("is_formula") = py::none())
        .def("stoichiometryMatrix", &DBElements::stoichiometryMatrix)
        .def("__repr__", []( const DBElements& self) { std::stringstream ss; ss << self.writeElements(); return ss.str(); })
        ;


    m.def("substancesStoichiometryMatrix", &substancesStoichiometryMatrix,
          py::arg("formulalist"), py::arg("valence")=false);
    m.def("elementsInFormulas", &elementsInFormulas,
          py::arg("formulalist"), py::arg("valence")=false);
    m.def("update_loggers", &update_loggers);
}
