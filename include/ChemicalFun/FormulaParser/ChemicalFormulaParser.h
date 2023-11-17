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


#pragma once

#include <string>
#include <list>

namespace ChemicalFun {

const int ELEMENT_CLASS = 0;
const int ISOTOPE_CLASS = 1;
const int CHARGE_CLASS = 4;
const std::string CHARGE_NAME = "Zz";
bool is_undefined_valence(int valence);

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
    const std::string& str_isotope() const {
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

    int class_() const;
    int isotope() const;

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
    bool is_capital(char ch) const
    {
        return (ch>='A' && ch<='Z') || ch=='$';
    }
    /// Is lowercase letter
    bool is_lowercase(char ch) const
    {
        return (ch>='a' && ch<='z') ||  ch == '_';
    }
    /// Skip escape sequences  (" \n\t\r")
    virtual void xblanc(std::string& str);
    /// Read next real from string
    virtual void get_real(double& real, std::string& cur);
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

