
#include "ChemicalFun/FormulaParser/ChemicalData.h"
#include "ChemicalFun/FormulaParser/ChemicalFormulaParser.h"
#include "Common/Exception.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace ChemicalFun {

static const std::map<std::string, int> map_elements_valences = {
    {"Ac",	3},
    {"Ag",	1},
    {"Al",	3},
    {"Ar",	0},
    {"Am",	3},
    {"As",	5},
    {"Au",	1},
    {"B",	3},
    {"Ba",	2},
    {"Be",	2},
    {"Bi",	3},
    {"Br",	-1},
    {"C",	4},
    {"Ca",	2},
    {"Cd",	2},
    {"Ce",	3},
    {"Cf",	3},
    {"Cit",	-3},
    {"Cl",	-1},
    {"Co",	2},
    {"Cr",	3},
    {"Cm",	3},
    {"Cs",	1},
    {"Cu",	2},
    {"Dy",	3},
    {"Edta", -4},
    {"Er",	3},
    {"Eu",	3},
    {"F",	-1},
    {"Fr",  1},
    {"Fe",	2},
    {"Ga",	3},
    {"Gd",	3},
    {"Ge",	4},
    {"H",	1},
    {"He",	0},
    {"Hf",	4},
    {"Hg",	2},
    {"Ho",	3},
    {"I",	-1},
    {"In",	3},
    {"Isa",	-4},
    {"Ir",	4},
    {"K",	1},
    {"Kr",	0},
    {"La",	3},
    {"Li",	1},
    {"Lu",	3},
    {"Mg",	2},
    {"Mn",	2},
    {"Mo",	6},
    {"N",	5},
    {"N_atm",	0},
    {"Na",	1},
    {"Nb",	5},
    {"Nd",	3},
    {"Ne",	0},
    {"Ni",	2},
    {"Np",	6},
    {"O",	-2},
    {"Os",	4},
    {"Ox",	-2},
    {"P",	5},
    {"Pa",	5},
    {"Pb",	2},
    {"Pd",	2},
    {"Po",	4},
    {"Pu",	6},
    {"Pr",	3},
    {"Pm",	3},
    {"Pt",	2},
    {"Ra",	2},
    {"Rb",	1},
    {"Re",	4},
    {"Rh",	2},
    {"Rn",	0},
    {"Ru",	2},
    {"S",	6},
    {"Sb",	3},
    {"Sc",	3},
    {"Se",	4},
    {"Si",	4},
    {"Sm",	3},
    {"Sn",	2},
    {"Sr",	2},
    {"Ta",	5},
    {"Tb",	3},
    {"Tc",	7},
    {"Te",	6},
    {"Th",	4},
    {"Ti",	4},
    {"Tl",	1},
    {"Tm",	3},
    {"U",	6},
    {"V",	5},
    {"W",	6},
    {"Xe",	0},
    {"Y",	3},
    {"Yb",	3},
    {"Zn",	2},
    {"Zr",	4},
    {"Zz",	0}
};

// Writes ElementKey data to json
static json element_key_to(const ElementKey& key)
{
    auto object = json::object();
    object["symbol"] = key.Symbol();
    if( key.Isotope() != 0 )
        object["isotope_mass"] =  key.Isotope();
    if( key.Class()!= 0 )
        object["class_"] = key.Class();
    return object;
}

// Reads ElementKey data from json
static ElementKey element_key_from(const json& key_object)
{
    std::string symbol;
    int class_ = 0;
    int isotope = 0;

    if (key_object.contains("symbol")) {
        symbol = key_object["symbol"].get<std::string>();
    }
    if (key_object.contains("isotope_mass")) {
        isotope = key_object["isotope_mass"].get<int>();
    }
    if (key_object.contains("class_")) {
        if(key_object["class_"].is_number()) {
            class_ = key_object["class_"].get<int>();
        }
        else if(key_object["class_"].is_object() && !key_object["class_"].empty()) {
            // from ArangoDB record
            class_ = stoi(key_object["class_"].begin().key());
        }
    }
    funErrorIf( symbol.empty(), "Undefined symbol.", "Element ", __LINE__, __FILE__ );
    return ElementKey{ symbol, class_, isotope };
}

// Writes ElementKey data to json
static json element_values_to(const ElementValues& values)
{
    auto object = json::object();
    object["recid"] = values.recid;
    object["name"] = values.name;
    object["number"] = values.number;
    object["valence"] = values.valence;
    object["atomic_mass"] = values.atomic_mass;
    object["entropy"] = values.entropy;
    object["heat_capacity"] = values.heat_capacity;
    object["volume"] = values.volume;
    return object;
}

static ElementValues element_values_from(const json& val_object)
{
    ElementValues data;
    if (val_object.contains("recid")) {
        data.recid = val_object["recid"].get<std::string>();
    }
    if (val_object.contains("name")) {
        data.name = val_object["name"].get<std::string>();
    }
    if (val_object.contains("number")) {
        data.number = val_object["number"].get<int>();
    }
    if (val_object.contains("valence")) {
        data.valence = val_object["valence"].get<int>();
    }
    if (val_object.contains("atomic_mass")) {
        data.atomic_mass = val_object["atomic_mass"].get<double>();
    }
    if (val_object.contains("entropy")) {
        data.entropy = val_object["entropy"].get<double>();
    }
    if (val_object.contains("heat_capacity")) {
        data.heat_capacity = val_object["heat_capacity"].get<double>();
    }
    if (val_object.contains("volume")) {
        data.volume = val_object["volume"].get<double>();
    }
    return data;
}

// Writes FormulaValues data to json
static json formula_key_to(const FormulaValues& key)
{
    auto object = json::object();
    object["key"] = element_key_to(key.key);
    object["valence"] =  key.valence;
    object["stoich_coef"] = key.stoich_coef;
    return object;
}

// Writes FormulaProperites data to json
static json formula_properites_to(const FormulaProperites& data)
{
    auto object = json::object();
    object["formula"] =  data.formula;
    object["charge"] = data.charge;
    object["atomic_mass"] =  data.atomic_mass;
    object["elemental_entropy"] = data.elemental_entropy;
    object["atoms_formula_unit"] =  data.atoms_formula_unit;
    return object;
}

void ElementKey::from_json_string(const std::string &json_string)
{
    auto key_json = json::parse(json_string);
    *this =element_key_from(key_json);
}

std::string ElementKey::to_json_string(bool dense) const
{
    auto el_key = element_key_to(*this);
    return el_key.dump(dense?4:-1);
}

void ElementValues::from_json_string(const std::string &json_string)
{
    auto values_json = json::parse(json_string);
    *this =element_values_from(values_json);
}

std::string ElementValues::to_json_string(bool dense) const
{
    auto el_values = element_values_to(*this);
    return el_values.dump(dense?4:-1);
}

std::string FormulaValues::to_json_string(bool dense) const
{
    auto data = formula_key_to(*this);
    return data.dump(dense?4:-1);
}

std::string FormulaProperites::to_json_string(bool dense) const
{
    auto data = formula_properites_to(*this);
    return data.dump(dense?4:-1);
}

//// Construct key from elements document values
//ElementKey::ElementKey(const std::string& asymbol, const std::string& aclass_, const std::string& aisotope):
//    symbol(asymbol)
//{
//    json j = json::parse(aclass_);
//    isotope = 0;
//    if( !aisotope.empty() )
//           isotope = stoi(aisotope);
//    class_  = stoi(j.begin().key());
//}

bool operator<(const ElementKey &lhs, const ElementKey &rhs)
{
    if(lhs.symbol < rhs.symbol) {
        return true;
    }
    if(lhs.symbol == rhs.symbol) {
        if(lhs.class_ < rhs.class_) {
            return true;
        }
        else {
            if(lhs.class_ == rhs.class_) {
                return (lhs.isotope < rhs.isotope);
            }
        }
    }
    return false;
}

bool operator==(const ElementKey &lhs, const ElementKey &rhs)
{
    return (lhs.symbol == rhs.symbol) &&
            (lhs.class_ == rhs.class_) &&
            (lhs.isotope == rhs.isotope);
}

//-------------------------------------------------------------

void FormulaToken::unpack(std::list<ElementsTerm>& parsed_data)
{
    for(auto& el_data: parsed_data) {
        ElementKey key(el_data.name(), el_data.class_(), el_data.isotope());

        if(el_data.default_valence()) {
            auto itrdb = map_elements_valences.find(key.Symbol());
            if( itrdb !=  map_elements_valences.end() ) {
                el_data.element_valence = itrdb->second;
            }
        }
        extracted_data.push_back( FormulaValues( key, el_data.stoich_coef(), el_data.valence() ));
        elements.insert(key);
        if (stoich_map.find(key) != stoich_map.end()) {
            stoich_map.at(key) += el_data.stoich_coef();
        }
        else {
            stoich_map[key] = el_data.stoich_coef();
        }
    }
}

double FormulaToken::calculate_charge()
{
    double Zz=0.0;
    for(const auto& token: extracted_data) {
        if(token.key.Class()!=CHARGE_CLASS
                && !is_undefined_valence(token.valence)) {
            Zz += token.stoich_coef * token.valence;
        }
    }
    return Zz;
}

void FormulaToken::clear()
{
    current_formula.clear();
    extracted_data.clear();
    elements.clear();
    stoich_map.clear();
    aZ = 0.;
}

void FormulaToken::setFormula(const std::string& aformula)
{
    clear();
    current_formula = aformula;
    current_formula.erase(std::remove(current_formula.begin(), current_formula.end(), '\"'), current_formula.end());
    ChemicalFormulaParser formparser;
    auto icterms = formparser.parse(current_formula);
    unpack(icterms);
    aZ = calculate_charge();
}

std::vector<std::string> FormulaToken::parsed_list(bool dense) const {
    std::vector<std::string> list;
    for(const auto& token: extracted_data) {
        list.push_back(token.to_json_string(dense));
    }
    return list;
}

bool FormulaToken::checkElements(const std::string& aformula, const DBElements& dbelements)
{
    std::string notPresent = testElements(aformula, dbelements);
    return notPresent.empty();
}

void FormulaToken::checkElements(const std::string& document, const std::string& aformula, const DBElements& dbelements)
{
    std::string notPresent = testElements(aformula, dbelements);
    if(!notPresent.empty()) {
        std::string msg = "Invalid Elements: ";
        msg += notPresent;
        msg += "\n in formula in record: \n";
        msg += document;
        funError("Invalid symbol", msg, __LINE__, __FILE__);
    }
}

std::string FormulaToken::testElements(const std::string& aformula, const DBElements& dbelements)
{
    std::string notPresent = "";
    auto elementset = dbelements.getElementsKeys();
    setFormula(aformula);
    for(const auto& token: extracted_data) {
        if( elementset.find(token.key) == elementset.end() ) {
            notPresent += token.key.Symbol() + ";";
        }
    }
    return notPresent;
}

FormulaProperites FormulaToken::calculateProperites(const DBElements& dbelements)
{
    FormulaProperites propert;
    double Sc;
    int valence;
    propert.formula = current_formula;
    propert.charge = propert.atomic_mass = 0.0;
    propert.elemental_entropy = propert.atoms_formula_unit = 0.0;

    for(const auto& token: extracted_data) {
        auto itrdb = dbelements.getElements().find(token.key);
        if(itrdb ==  dbelements.getElements().end()) {
            funError("Invalid symbol", token.key.Symbol(), __LINE__, __FILE__);
        }
        Sc = token.stoich_coef;
        propert.atoms_formula_unit += Sc;
        propert.atomic_mass += Sc * itrdb->second.atomic_mass;
        propert.elemental_entropy += Sc * itrdb->second.entropy;

        valence = token.valence;
        if(is_undefined_valence(valence)) {
            valence = itrdb->second.valence;
        }
        if(token.key.Class() != CHARGE_CLASS) {
            propert.charge += Sc * valence;
        }
    }
    return propert;
}

StoichiometryRowData FormulaToken::makeStoichiometryRow(const std::vector<ElementKey>& all_elemens)
{
    double ai = 0;
    StoichiometryRowData rowA;

    for(const auto& elkey: all_elemens) {
        ai=0.;
        if(elements.find(elkey) != elements.end()) {
            for(size_t ii=0; ii<extracted_data.size(); ++ii) {
                if( elkey == extracted_data[ii].key ) {
                    ai += extracted_data[ii].stoich_coef;
                }
            }
        }
        rowA.push_back(ai);
    }
    return rowA;
}

void FormulaToken::testCargeImbalance()
{
    ElementKey chargeKey(CHARGE_NAME,CHARGE_CLASS,0);
    if(elements.find(chargeKey) == elements.end())
        return;

    auto itZz = std::find_if(extracted_data.rbegin(), extracted_data.rend(),
                             [=](const FormulaValues& token) { return token.key == chargeKey; });
    if(itZz != extracted_data.rend()) {
        double Zzval = itZz->stoich_coef;
        if(fabs(aZ - Zzval) > 1e-6)  {
            std::string str = "In the formula: ";
            str +=  current_formula + "\n calculated charge: ";
            str +=  std::to_string(aZ) + " != " + std::to_string(Zzval);
            funError("Charge imbalance", str, __LINE__, __FILE__);
        }
    }
}

//------------------------------------------

void DBElements::addElement(const ElementKey& elkey, const ElementValues& elvalue)
{
    dbElements[elkey] = elvalue;
    dbElementsKeys.insert(elkey);
}

void DBElements::readElements(const std::string &json_array)
{
    dbElements.clear();
    dbElementsKeys.clear();
    auto elements_json = json::parse(json_array);
    for( const auto& el_json: elements_json) {
        addElement(element_key_from(el_json["element"]),
                element_values_from(el_json["properties"]));
    }
}

std::string DBElements::writeElements(bool dense)
{
    auto elements_json = json::array();
    for( const auto& element: dbElements) {
        auto elem = json::object();
        elem["element"] = element_key_to(element.first);
        elem["properties"] = element_values_to(element.second);
        elements_json.push_back(elem);
    }
    return elements_json.dump(dense?4:-1);
}

ElementsKeys DBElements::extractElements(const std::vector<std::string>& formulalist)
{
    ElementsKeys elements;
    FormulaToken formula("");
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula);
        elements.insert(formula.getElementsList().begin(), formula.getElementsList().end());
    }
    return elements;
}

std::vector<FormulaProperites> DBElements::calcThermo(const std::vector<std::string>& formulalist)
{
    std::vector<FormulaProperites> thermo;
    for(const auto& aformula: formulalist) {
        thermo.push_back(calcThermo(aformula));
    }
    return thermo;
}

StoichiometryMatrixData DBElements::calcStoichiometryMatrix(const std::vector<std::string>& formulalist)
{
    StoichiometryMatrixData matrA;
    FormulaToken formula("");
    std::vector<ElementKey> sysElemens = getElementsKeysList();
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula);
        matrA.push_back(formula.makeStoichiometryRow( sysElemens ));
    }
    return matrA;
}

}