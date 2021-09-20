#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>

namespace ChemicalFun {

class DBElements;
class ElementsTerm;

/// Key fields of Element descriptions
class ElementKey final
{
public:
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
struct FormulaProperites
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
    /// Constructor
    FormulaToken(const std::string& aformula): aZ(0) {
        setFormula(aformula);
    }

    /// Disassemble the formula.
    void setFormula(const std::string& aformula);
    /// Disassembled formula.
    const std::string& formula() const  {
        return current_formula;
    }
    /// Calculated charge in Mol.
    double charge() const  {
        return aZ;
    }
    /// Get stoichiometric coefficients for elements in the formula.
    const std::map<ElementKey, double>& getStoichCoefficients() const {
        return stoich_map;
    }
    /// Get list of elements into the formula.
    const ElementsKeys& getElementsList() const {
        return elements;
    }

    /// Generate a list of values parsed from the formula in JSON format strings.
    std::vector<std::string> parsed_list(bool dense = false) const;

    /// Return true if all elements from formula present into system.
    bool checkElements(const std::string& aformula, const DBElements& dbelements);
    /// Throw exeption if in formula have element not present into system.
    void checkElements(const std::string& document, const std::string& aformula, const DBElements& dbelements);
    /// Build list of elements not present into system.
    std::string testElements(const std::string& aformula, const DBElements& dbelements);
    /// Throw exeption if charge imbalance.
    void testCargeImbalance();

    /// Calculate charge, molar mass, elemental entropy, atoms per formula unit
    /// for chemical formulae.
    FormulaProperites calculateProperites(const DBElements& dbelements);

    /// Get a row of stoichiometry matrix from the unpacked formula,
    /// all_elemens - list of all element keys.
    StoichiometryRowData makeStoichiometryRow(const std::vector<ElementKey>& all_elemens);

protected:
    /// Disassembled formula
    std::string current_formula;
    /// List of values calculated from the formula
    std::vector<FormulaValues> extracted_data;
    /// Map of stoichiometric coefficients for elements in the formula
    std::map<ElementKey, double>  stoich_map;
    /// Set of elements into the formula
    ElementsKeys  elements;
    /// Calculated charge in Mol
    double aZ;

    void clear();
    void unpack(std::list<ElementsTerm>& parsed_data);
    double calculate_charge();
};

class DBElements final
{
public:
    /// Add new element to internal.
    void addElement(const ElementKey &elkey, const ElementValues &elvalue);

    const ElementsData& getElements() const {
        return dbElements;
    }
    const ElementsKeys& getElementsKeys() const {
        return dbElementsKeys;
    }
    std::vector<ElementKey> getElementsKeysList() const {
        return std::vector<ElementKey>(dbElementsKeys.begin(), dbElementsKeys.end());
    }

    /// List of elements collected from the list of formulas.
    ElementsKeys extractElements(const std::vector<std::string>& formulalist);

    /// Calculate charge, molar mass, elemental entropy, atoms per formula.
    FormulaProperites calcThermo(const std::string aformula) const {
        return FormulaToken(aformula).calculateProperites(*this);
    }
    /// Calculate charge, molar mass, elemental entropy, atoms per formula list.
    std::vector<FormulaProperites> calcThermo(const std::vector<std::string>& formulalist);

    /// Generate stoichiometry matrix from the formula list.
    StoichiometryMatrixData calcStoichiometryMatrix(const std::vector<std::string>& formulalist);

    /// Restore elements DB from JSON array format output string.
    void readElements(const std::string &json_array);
    /// Download elements DB to JSON array format output string.
    std::string writeElements(bool dense = false);

protected:
    /// Loading from database elements
    ElementsData dbElements;
    /// Set of keys of elements downloaded from the database
    ElementsKeys dbElementsKeys;

};


}


