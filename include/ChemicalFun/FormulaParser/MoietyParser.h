//-------------------------------------------------------------------
// $Id: formula-parser.h 1373 2009-07-22 12:25:22Z gems $
//
// Declaration of TFormula - gems chemical formulas parser
//
// Rewritten from C to C++ by S.Dmytriyeva
// Copyright (C) 1995-2016  D.Kulik, S.Dmytriyeva
//
// This file is part of a GEM-Selektor library for thermodynamic
// modelling by Gibbs energy minimization
//
// This file may be distributed under the terms of GEMS4 Development
// Quality Assurance Licence (GEMS4.QAL)
//
// See http://gems.web.psi.ch/ for more information
// E-mail: gems2.support@psi.ch
//-------------------------------------------------------------------
//
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
