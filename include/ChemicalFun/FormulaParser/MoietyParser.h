// ChemicalFun is a C++ and Python library for of C++ and Python API
// for Chemical Formula Parser and Reactions Generator.
//
// Copyright (C) 2018-2022 G.D.Miron, D.Kulik, S.Dmytriieva
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

#pragma once

#include <vector>
#include "ChemicalFormulaParser.h"

namespace ChemicalFun {

/// Description of Moiety element
class MoietyTerm final
{
public:

    MoietyTerm(const std::string& aname, int asite, double aoccupancy):
        site(asite), occupancy(aoccupancy)
    {
        m_name = "{" + aname + "}" + std::to_string(site);
    }

    const std::string& name() const {
        return  m_name;
    }
    int sublattice_site() const {
        return  site;
    }
    double moiety_site_occupancy() const {
        return  occupancy;
    }

    std::string to_string() const;

private:
    std::string m_name;
    int  site;
    double occupancy;

};

std::ostream &operator<<(std::ostream& os, const MoietyTerm& dt);


/// Parser for Moiety
class MoietyParser : public BaseParser
{
public:
    MoietyParser(){}
    ~MoietyParser();

    /// Parse the Moiety terms
    std::vector<MoietyTerm> parse(const std::string& aformula, int& nsites);

private:
    std::string formula;
    int number_sites;             // number of sites in formula

};

}
