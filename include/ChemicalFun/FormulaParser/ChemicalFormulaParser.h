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

#include <string>
#include <list>
#include <iostream>

namespace ChemicalFun {

const std::string CHARGE_NAME = "Zz";

/// Description of parsed element
class ElementsTerm final
{
public:
    /// Construct a ElementsTerm instance with given parameters.
    ElementsTerm(const std::string& aname,
                 const std::string& aisotope,
                 int avalence,
                 double astoich_coef):
        element_name(aname),
        element_isotope(aisotope),
        element_valence(avalence),
        element_stoich_coef(astoich_coef)
    {}

    const std::string& name() const {
        return  element_name;
    }
    const std::string& isotope() const {
        return  element_isotope;
    }
    int valence() const {
        return  element_valence;
    }
    double stoich_coef() const {
        return  element_stoich_coef;
    }

    bool no_isotope() const;
    bool default_valence() const;
    bool is_charge() const;
    std::string to_string() const;

protected:
    std::string element_name;
    std::string element_isotope;
    int element_valence;
    double element_stoich_coef;

    friend class ChemicalFormulaParser;
    friend class FormulaToken;
};

std::ostream& operator<<(std::ostream& os, const ElementsTerm& term);

/// Stack for analyzing formula
class BaseParser
{
public:
    BaseParser(){}
    virtual ~BaseParser(){}

protected:
    enum BRACKETS
    {
        LBRACKET1   = '(', RBRACKET1   = ')',
        LBRACKET2   = '[', RBRACKET2   = ']',
        LBRACKET3   = '{', RBRACKET3   = '}',
        B_ISOTOPE   = '/', B_VALENT    = '|',
        PSUR_L_PLUS =  ':'
    };

    /// Is capital letter
    bool is_capital( char ch ) const
    {
        return (ch>='A' && ch<='Z') || ch=='$';
    }
    /// Is lowercase letter
    bool is_lowercase( char ch ) const
    {
        return (ch>='a' && ch<='z') ||  ch == '_';
    }
    /// Skip escape sequences  (" \n\t\r")
    virtual void xblanc( std::string& str );
    /// Read next real from string
    virtual void get_real( double& real, std::string& cur);
};


/// Parser for Chemical Formula
class ChemicalFormulaParser : public BaseParser
{
public:
    ChemicalFormulaParser(){}
    ~ChemicalFormulaParser();

    /// Parse the Chemical Formula terms
    std::list<ElementsTerm> parse(const std::string& formula);

protected:
    std::string formula;
    std::string charge;

    enum  CHARGE_FIELD {
        CHARGE_NUL    ='@',
        CHARGE_PLUS   ='+',
        CHARGE_MINUS  ='-'
    };

    void add_element(std::list<ElementsTerm>& terms_list, const ElementsTerm& new_term);
    int  term_compare(const ElementsTerm& term, const ElementsTerm& new_term) const;
    void add_charge(std::list<ElementsTerm>& terms_list);
    void scan_charge();
    void scan_formula(std::list<ElementsTerm>& terms_list, std::string& start_pos, char end_simb);
    void scan_element(std::list<ElementsTerm>& terms_list, std::string& parsed_string);
    void scan_valence(int& valence, std::string& parsed_string);
    void scan_isotope(std::string& isotop, std::string& parsed_string);
    void scan_symbol(std::string& icname, std::string& parsed_string);
};

}

