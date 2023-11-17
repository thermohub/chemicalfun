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

#if _MSC_VER >= 1929
#include <corecrt.h>
#endif

// pybind11 includes
#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
namespace py = pybind11;
using namespace pybind11::literals;

#include "ChemicalFun/FormulaParser/ChemicalData.h"
#include "ChemicalFun/ReactionsGenerator/ChemicalReactions.h"
#include "ChemicalFun/ReactionsGenerator/Reaction.h"
#include "ChemicalFun/ReactionsGenerator/Generator.h"
#include "ChemicalFun/ReactionsGenerator/Combiner.h"
using namespace ReactionsGenerator;

void exportReactionsGenerator(py::module& m)
{
 //   m.def("stoichiometryMatrix", py::overload_cast<Eigen::MatrixXd&>(&stoichiometryMatrix));
 //   m.def("stoichiometryMatrix", py::overload_cast<Eigen::MatrixXd&(std::vector<std::string>&, bool)>(&stoichiometryMatrix);
 // m.def("stoichiometryMatrix", py::overload_cast<std::vector<std::string>&, py::arg("valence")=false>(&stoichiometryMatrix));

    py::class_<ChemicalReactions>(m, "ChemicalReactions")
            .def(py::init<>())
            .def(py::init<std::vector<std::string>, bool>(), "formulas"_a,"valence"_a=false )
            .def(py::init<std::vector<std::string>, std::vector<std::string>, bool>(), "formulas"_a, "symbols"_a, "valence"_a=false )
            .def(py::init<std::vector<std::vector<double>>>() )
            .def(py::init<MatrixXd, std::vector<std::string>>() )
            .def(py::init<const class ChemicalReactions &>())
            .def("assign", &ChemicalReactions::operator=)
            .def("formulaMatrix", &ChemicalReactions::formulaMatrix)
            .def("reactionsMatrix", &ChemicalReactions::reactionsMatrix)
            .def("generateReactions", &ChemicalReactions::generateReactions, py::arg("formation")  = false)
            .def("sizeSubstancesMap", &ChemicalReactions::sizeSubstancesMap)
            .def("getCharge", py::overload_cast<std::string>(&ChemicalReactions::getCharge))
            .def("getCharge", py::overload_cast<std::size_t>(&ChemicalReactions::getCharge))
            .def("checkReacMatrixISOC", &ChemicalReactions::checkReacMatrixISOC )
            .def("checkReacMatrixISOE", &ChemicalReactions::checkReacMatrixISOE )
            .def("masterSubstances", &ChemicalReactions::masterSubstances )
            .def("dependentSubstances", &ChemicalReactions::dependentSubstances )
            .def("printReactions",[]( ChemicalReactions& self) { std::stringstream ss; self.printReactions(ss); return ss.str(); })
            .def("stringReactions", &ChemicalReactions::stringReactions, py::arg("reactantsOrder")  = false)
            ;

    py::class_<Reaction>(m, "Reaction")
            .def(py::init<>())
            .def(py::init<VectorXd, Indices, ChemicalReactions*, std::string>() )
            .def(py::init<const class Reaction &>())
            .def("assign", &Reaction::operator=)
            .def("chargePattern", &Reaction::chargePattern)
            .def("coefficients", &Reaction::coefficients)
            .def("isIsocoulombic", &Reaction::isIsocoulombic)
            .def("isIsoelectric", &Reaction::isIsoelectric )
            .def("id", &Reaction::id )
            .def("setCoefficients", &Reaction::setCoefficients )
            .def("updateChPattern", &Reaction::updateChPattern )
            .def(py::self += py::self)
            .def(py::self *= double())
            .def(py::self + py::self)
            .def(py::self * double())
            //.def("__mul__", [](Reaction &a, double b) { return a * b; }, py::is_operator())
            .def(py::self == py::self)
            ;

    py::enum_<Method>(m, "Method")
            .value("GramSchmidt", GramSchmidt)
            .value("GramSchmidtWe94", GramSchmidtWe94)
            .value("RowReduceSmMi98", RowReduceSmMi98)
            .value("CanonicalLe16", CanonicalLe16)
            .export_values();

    py::class_<Generator>(m, "Generator")
            .def(py::init<>())
            .def(py::init<MatrixXd>() )
            .def(py::init<const class Generator &>())
            .def("assign", &Generator::operator=)
            .def("reactionMatrix", &Generator::reactionMatrix)
            .def("isubstances", &Generator::isubstances)
            .def("imaster", &Generator::imaster)
            .def("inonmaster", &Generator::inonmaster )
            .def("setMethod", &Generator::setMethod )
            .def("compute", &Generator::compute )
            .def("swap", &Generator::swap )
            ;

    py::class_<Combiner>(m, "Combiner")
            .def(py::init<>())
            .def(py::init<Reaction, Reactions>() )
            .def(py::init<const class Combiner &>())
            .def("assign", &Combiner::operator=)
            .def("resultReactions", &Combiner::resultReactions)
            .def("reaction", &Combiner::reaction)
            .def("combinedReactionsList", &Combiner::combinedReactionsList)
            .def("combineToISOC", &Combiner::combineToISOC )
            ;
}
