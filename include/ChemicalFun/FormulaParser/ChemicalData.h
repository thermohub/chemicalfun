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
#include <vector>
#include <map>
#include <set>
#include <list>

namespace ChemicalFun {

void update_loggers( bool use_cout, const std::string& logfile_name, size_t log_level);

class DBElements;
class ElementsTerm;

/// Key fields of Element descriptions
class ElementKey final
{
public:
    /// Default Constructor
    ElementKey():
        symbol(""), class_(0), isotope(0)
    {}
    /// Constructor
    ElementKey(const std::string& asymbol, int aclass, int aisotope=0):
        symbol(asymbol), class_(aclass), isotope(aisotope)
    {}
    /// Construct key from elements JSON document string.
    ElementKey(const std::string& element) {
        from_json_string(element);
    }

    const std::string& Symbol() const {
        return symbol;
    }
    int Class() const {
        return class_;
    }
    int Isotope() const {
        return isotope;
    }

    void from_json_string(const std::string &json_string);
    std::string to_json_string(bool dense = false) const;
    std::string to_string() const;

    friend bool operator==(const ElementKey& lhs, const ElementKey& rhs);
    friend bool operator!=(const ElementKey& lhs, const ElementKey& rhs){return !operator==(lhs,rhs);}
    friend bool operator< (const ElementKey& lhs, const ElementKey& rhs);
    friend bool operator> (const ElementKey& lhs, const ElementKey& rhs){return  operator< (rhs,lhs);}
    friend bool operator<=(const ElementKey& lhs, const ElementKey& rhs){return !operator> (lhs,rhs);}
    friend bool operator>=(const ElementKey& lhs, const ElementKey& rhs){return !operator< (lhs,rhs);}

protected:
    std::string symbol;
    int class_;
    int isotope;
};

/// Elements values, loaded from Database
struct ElementValues
{
    /// Record id (optional)
    std::string recid;
    /// Elements name
    std::string name;
    /// Index in Periodical (Mendeleev's) table
    int number = 0;
    /// Default valence number (charge) in compounds
    int valence = 0;
    /// Atomic (molar) mass, g/atom (g/mole)
    double atomic_mass = 0;
    /// Atomic entropy S0 at standard state, J/mole/K
    double entropy = 0;
    /// Atomic heat capacity Cp0 at standard state, J/mole/K
    double heat_capacity = 0;
    /// Atomic volume V0 at standard state, cm3/mole
    double volume = 0;

    void from_json_string(const std::string &json_string);
    std::string to_json_string(bool dense = false) const;
};

/// Values extracted from chemical formulae
struct FormulaValues
{
    ElementKey key;
    int   valence;
    double  stoich_coef;

    FormulaValues(const ElementKey& akey, double astoich_coef, int avalence):
        key(akey), valence(avalence), stoich_coef(astoich_coef)
    {}

    std::string to_json_string(bool dense = false) const;
};

/// Values calculated from the formula of substances.
struct FormulaProperties
{
    std::string formula;
    double charge;
    double atomic_mass;
    double elemental_entropy;
    double atoms_formula_unit;

    std::string to_json_string(bool dense = false) const;
};

/// Loading from Database a map of element symbol, valence, entropy and other
using ElementsData = std::map<ElementKey, ElementValues>;
/// List of keys of elements
using ElementsKeys= std::set<ElementKey>;
using StoichiometryRowData = std::vector<double>;
using StoichiometryMatrixData = std::vector<StoichiometryRowData>;

/// Description of disassembled formula token.
class FormulaToken final
{
public:
    /// Global default settings for calculating the charge method.
    /// If get_charge_from_formula false, calculate the charge based on the elements and their default
    /// or specified valence; otherwise, take the charge based on the symbol in the given formula.
    static bool get_charge_from_formula;

    /// Constructor
    FormulaToken(const std::string& aformula, bool valence = false) {
        setFormula(aformula, valence);
    }

    /// Disassemble the formula.
    void setFormula(const std::string& aformula, bool valence = false);
    /// Disassembled formula.
    const std::string& formula() const  {
        return current_formula;
    }
    /// Calculated charge in Mol.
    /// If undefined valence throw exception.
    double charge(const ElementsData& dbelements = {}) const  {
        return calculate_charge(dbelements);
    }
    /// Get stoichiometric coefficients for elements in the formula.
    const std::map<ElementKey, double>& getStoichCoefficients() const {
        return stoich_map;
    }
    /// Get list of elements into the formula.
    const ElementsKeys& getElementsList() const {
        return elements;
    }
    /// Get a row of stoichiometry matrix from the unpacked formula,
    /// all_elemens - list of all element keys.
    StoichiometryRowData makeStoichiometryRow(const std::vector<ElementKey>& all_elemens);

    /// Generate a list of values parsed from the formula in JSON format strings.
    std::vector<std::string> parsed_list(bool dense = false) const;

    /// Return true if all elements from formula present into system.
    bool checkElements(const std::string& aformula, const ElementsKeys& dbelementkeys);
    /// Throw exeption if in formula have element not present into system.
    void checkElements(const std::string& document, const std::string& aformula, const ElementsKeys& dbelementkeys);
    /// Build list of elements not present into system.
    std::string testElements(const std::string& aformula, const ElementsKeys& dbelementkeys);
    /// Throw exeption if charge imbalance.
    void testChargeImbalance(const ElementsData& dbelements = {});

    /// Calculate charge, molar mass, elemental entropy, atoms per formula unit
    /// for chemical formulae.
    /// If use_formula_charge false, calculate the charge based on the elements and their default
    /// or specified valence; otherwise, take the charge based on the symbol in the given formula.
    FormulaProperties properties(const ElementsData& dbelements,
                                 bool use_formula_charge = get_charge_from_formula);

protected:
    /// If we need a matrix with separate element valences
    bool elements_keys_with_valences;
    /// Disassembled formula
    std::string current_formula;
    /// List of values calculated from the formula
    std::vector<FormulaValues> extracted_data;
    /// Map of stoichiometric coefficients for elements in the formula
    std::map<ElementKey, double>  stoich_map;
    /// Set of elements into the formula
    ElementsKeys  elements;

    void clear();
    void unpack(std::list<ElementsTerm>& parsed_data);
    double calculate_charge(const ElementsData& dbelements) const;
};

class DBElements final
{
public:
    /// The constructor with default elements list.
    explicit DBElements();

    static int defaultValence(const std::string& symbol);

    /// List of elements collected from the list of formulas.
    static ElementsKeys formulasElements(const std::vector<std::string>& formulalist);

    /// Add new element to internal.
    void addElement(const ElementKey &elkey, const ElementValues &elvalue);

    const ElementsData& elements() const {
        return dbElements_;
    }
    const ElementsKeys& elementsKeys() const {
        return dbElementsKeys_;
    }
    std::vector<ElementKey> elementsKeysList() const {
        return std::vector<ElementKey>(dbElementsKeys_.begin(), dbElementsKeys_.end());
    }

    /// Calculate charge, molar mass, elemental entropy, atoms per formula.
    /// If use_formula_charge false, calculate the charge based on the elements and their default
    /// or specified valence; otherwise, take the charge based on the symbol in the given formula.
    FormulaProperties formulasProperties(const std::string aformula,
                                         bool use_formula_charge = FormulaToken::get_charge_from_formula) const
    {
        return FormulaToken(aformula).properties(this->dbElements_, use_formula_charge);
    }
    /// Calculate charge, molar mass, elemental entropy, atoms per formula list.
    /// If use_formula_charge false, calculate the charge based on the elements and their default
    /// or specified valence; otherwise, take the charge based on the symbol in the given formula.
    std::vector<FormulaProperties> formulasProperties(const std::vector<std::string>& formulalist,
                                                      bool use_formula_charge = FormulaToken::get_charge_from_formula);

    /// Generate stoichiometry matrix from the formula list.
    StoichiometryMatrixData stoichiometryMatrix(const std::vector<std::string>& formulalist);

    /// Restore elements DB from JSON array format output string.
    void readElements(const std::string &json_array);
    /// Download elements DB to JSON array format output string.
    std::string writeElements(bool dense = false) const;

    void printCSV(std::ostream &stream);
    void formulasPropertiesCSV(std::ostream &stream, const std::vector<std::string> &formulalist,
                               bool use_formula_charge = FormulaToken::get_charge_from_formula);
    void printStoichiometryMatrix(std::ostream &stream, const std::vector<std::string> &formulalist);

protected:
    /// Loading from database elements
    ElementsData dbElements_;
    /// Set of keys of elements downloaded from the database
    ElementsKeys dbElementsKeys_;

};

std::string to_string(const std::vector<ElementKey>& keys );

/// Generate stoichiometry matrix from the formula list.
StoichiometryMatrixData substancesStoichiometryMatrix(const std::vector<std::string> &formulalist,
                                                      bool valence = false);

/// Generate stoichiometry matrix from the formula list and elements list.
StoichiometryMatrixData stoichiometryMatrix(const std::vector<std::string> &formulalist,
                                                      std::vector<ElementKey> all_elements);

/// Generate elements used list
std::vector<ElementKey> elementsInFormulas(const std::vector<std::string> &formulalist,
                                                     bool valence = false);
}


