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

#include "ChemicalFun/FormulaParser/ChemicalData.h"
#include "ChemicalFun/FormulaParser/ChemicalFormulaParser.h"
#include "Common/Exception.h"
#ifndef __EMSCRIPTEN__
// We don't need to include the strings
#include "ElementsDB.h"
#endif
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

std::string to_string(const std::vector<ElementKey>& keys ) {
   std::string str_keys;
   for( const auto& key: keys ) {
      str_keys += key.Symbol()+";";
   }
   return str_keys;
}

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
    if (key_object.contains("isotope_mass") && key_object["isotope_mass"].is_number()) {
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

// Writes FormulaProperties data to json
static json formula_properties_to(const FormulaProperties& data)
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
    if( json_string.empty() )
    {
      symbol = "";
      class_ = 0;
      isotope = 0;
      return;
    }
    auto key_json = json::parse(json_string);
    *this =element_key_from(key_json);
}

std::string ElementKey::to_json_string(bool dense) const
{
    auto el_key = element_key_to(*this);
    return el_key.dump(dense?4:-1);
}

std::string ElementKey::to_string() const
{
  std::string str_key= symbol;
  if( class_ != 0 )
   str_key+= "|" + std::to_string(class_);
  if( isotope != 0 )
   str_key+= "|" + std::to_string(isotope);

 return str_key;
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

std::string FormulaProperties::to_json_string(bool dense) const
{
    auto data = formula_properties_to(*this);
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
        if( lhs.class_ == 0 || rhs.class_ == 0 || lhs.class_ == rhs.class_) {
            return (lhs.isotope < rhs.isotope);
        }
        else {
            return( lhs.class_ < rhs.class_);
        }
    }
    return false;
}

bool operator==(const ElementKey &lhs, const ElementKey &rhs)
{
    return (lhs.symbol == rhs.symbol) &&
            (lhs.class_ == rhs.class_ || lhs.class_ == 0 || rhs.class_ == 0) &&
            (lhs.isotope == rhs.isotope);
}

//-------------------------------------------------------------

void FormulaToken::unpack(std::list<ElementsTerm>& parsed_data)
{
    for(auto& el_data: parsed_data) {
        auto element_symbol = el_data.name();
        if(el_data.default_valence()) {
            auto itrdb = map_elements_valences.find(element_symbol);
            if( itrdb !=  map_elements_valences.end() ) {
                el_data.element_valence = itrdb->second;
            }
        }
        if(elements_keys_with_valences ) {
          element_symbol += "("+std::to_string(el_data.element_valence)+")";
        }
        ElementKey key(element_symbol, el_data.class_(), el_data.isotope());
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

double FormulaToken::calculate_charge(const ElementsData& dbelements) const
{
    double Zz=0.0;
    for(const auto& token: extracted_data) {
        auto valence = token.valence;
        if(is_undefined_valence(valence))  {
            auto elm_inf = dbelements.find(token.key);
            if( elm_inf != dbelements.end()) {
                valence =  elm_inf->second.valence;
            }
            else {
                funError("Charge for undefined valence", token.key.to_string(), __LINE__, __FILE__);
            }
        }
        if(token.key.Class()!=CHARGE_CLASS) {
            Zz += token.stoich_coef * valence;
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
}

void FormulaToken::setFormula(const std::string& aformula, bool valence)
{
    clear();
    elements_keys_with_valences = valence;
    current_formula = aformula;
    current_formula.erase(std::remove(current_formula.begin(), current_formula.end(), '\"'), current_formula.end());
    ChemicalFormulaParser formparser;
    auto icterms = formparser.parse(current_formula);
    unpack(icterms);
}

std::vector<std::string> FormulaToken::parsed_list(bool dense) const {
    std::vector<std::string> list;
    for(const auto& token: extracted_data) {
        list.push_back(token.to_json_string(dense));
    }
    return list;
}

bool FormulaToken::checkElements(const std::string& aformula, const ElementsKeys& dbelementkeys)
{
    std::string notPresent = testElements(aformula, dbelementkeys);
    return notPresent.empty();
}

void FormulaToken::checkElements(const std::string& document, const std::string& aformula, const ElementsKeys& dbelementkeys)
{
    std::string notPresent = testElements(aformula, dbelementkeys);
    if(!notPresent.empty()) {
        std::string msg = "Invalid Elements: ";
        msg += notPresent;
        msg += "\n in formula in record: \n";
        msg += document;
        funError("Invalid symbol", msg, __LINE__, __FILE__);
    }
}

std::string FormulaToken::testElements(const std::string& aformula, const ElementsKeys& dbelementkeys)
{
    std::string notPresent = "";
    setFormula(aformula);
    for(const auto& token: extracted_data) {
        if( dbelementkeys.find(token.key) == dbelementkeys.end() ) {
            notPresent += token.key.Symbol() + ";";
        }
    }
    return notPresent;
}

FormulaProperties FormulaToken::properties(const ElementsData& dbelements, bool use_formula_charge)
{
    FormulaProperties propert;
    double Sc, aZ=0.0, Zzval=0.0;
    int valence;
    propert.formula = current_formula;
    propert.charge = propert.atomic_mass = 0.0;
    propert.elemental_entropy = propert.atoms_formula_unit = 0.0;

    for(const auto& token: extracted_data) {
        auto itrdb = dbelements.find(token.key);
        if(itrdb ==  dbelements.end()) {
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
        if(token.key.Class() == CHARGE_CLASS) {
            Zzval += Sc;
        }
        else {
            aZ += Sc * valence;
        }
    }

    if(use_formula_charge) {
        propert.charge = Zzval;
    }
    else {
        propert.charge = aZ;
    }

    if(fabs(aZ - Zzval) > 1e-6)  {
        std::string str = "In the formula: ";
        str +=  current_formula + " (calculated formula charge) ";
        str +=  std::to_string(aZ) + " != " + std::to_string(Zzval)+ " (given formula charge). Set explicit element valence with bars ||.";
        ChemicalFun::chfun_logger->info(str);
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

bool FormulaToken::testChargeImbalance(const ElementsData& dbelements, bool no_throw)
{
    double Zzval = 0.0;  // by default 0
    ElementKey chargeKey(CHARGE_NAME,CHARGE_CLASS,0);

    auto aZ = charge(dbelements);

    auto itZz = std::find_if(extracted_data.rbegin(), extracted_data.rend(),
                             [=](const FormulaValues& token) { return token.key == chargeKey; });
    if(itZz != extracted_data.rend()) {
        Zzval = itZz->stoich_coef;
    }
    if(fabs(aZ - Zzval) > 1e-6)  {
        std::string str = "In the formula: ";
        str +=  current_formula + " (calculated formula charge) ";
        str +=  std::to_string(aZ) + " != " + std::to_string(Zzval)+ " (given formula charge). Set explicit element valence with bars ||.";
        if(no_throw) {
            ChemicalFun::chfun_logger->info(str);
            return true;
        }
        else {
            funError("Charge imbalance", str, __LINE__, __FILE__);
        }
    }

    return false;
}

//------------------------------------------

DBElements::DBElements()
{
#ifdef __EMSCRIPTEN__
    // We can define JSON objects directly, why should we need parse strings?
    // But why even bother with that, we only need to populate dbElements_ and dbElementsKeys_
    /*
    * Generated with:
    std::cout << "dbElementsKeys_ = {" << std::endl;
    for (ElementKey k : dbElementsKeys_) {
        
        std::cout << "{\"" << k.Symbol() << "\", " << std::to_string(k.Class()) << ", " << std::to_string(k.Isotope()) << " }," << std::endl;
    }
    std::cout << "};" << std::endl << std::flush;
    */
    dbElementsKeys_ = {
        {"Ag", 0, 0 },
        {"Al", 0, 0 },
        {"Am", 0, 0 },
        {"Ar", 0, 0 },
        {"As", 0, 0 },
        {"Au", 0, 0 },
        {"B", 0, 0 },
        {"Ba", 0, 0 },
        {"Be", 0, 0 },
        {"Bi", 0, 0 },
        {"Br", 0, 0 },
        {"C", 0, 0 },
        {"Ca", 0, 0 },
        {"Cd", 0, 0 },
        {"Ce", 0, 0 },
        {"Cl", 0, 0 },
        {"Cm", 0, 0 },
        {"Co", 0, 0 },
        {"Cr", 0, 0 },
        {"Cs", 0, 0 },
        {"Cu", 0, 0 },
        {"Dy", 0, 0 },
        {"Er", 0, 0 },
        {"Eu", 0, 0 },
        {"F", 0, 0 },
        {"Fe", 0, 0 },
        {"Ga", 0, 0 },
        {"Gd", 0, 0 },
        {"Ge", 0, 0 },
        {"H", 0, 0 },
        {"He", 0, 0 },
        {"Hf", 0, 0 },
        {"Hg", 0, 0 },
        {"Ho", 0, 0 },
        {"I", 0, 0 },
        {"In", 0, 0 },
        {"Ir", 0, 0 },
        {"K", 0, 0 },
        {"Kr", 0, 0 },
        {"La", 0, 0 },
        {"Li", 0, 0 },
        {"Lu", 0, 0 },
        {"Mg", 0, 0 },
        {"Mn", 0, 0 },
        {"Mo", 0, 0 },
        {"N", 0, 0 },
        {"Na", 0, 0 },
        {"Nb", 0, 0 },
        {"Nd", 0, 0 },
        {"Ne", 0, 0 },
        {"Ni", 0, 0 },
        {"Nia", 2, 0 },
        {"Nit", 2, 0 },
        {"Np", 0, 0 },
        {"O", 0, 0 },
        {"Os", 0, 0 },
        {"P", 0, 0 },
        {"Pa", 0, 0 },
        {"Pb", 0, 0 },
        {"Pd", 0, 0 },
        {"Po", 0, 0 },
        {"Pr", 0, 0 },
        {"Pt", 0, 0 },
        {"Pu", 0, 0 },
        {"Ra", 0, 0 },
        {"Rb", 0, 0 },
        {"Re", 0, 0 },
        {"Rh", 0, 0 },
        {"Rn", 0, 0 },
        {"Ru", 0, 0 },
        {"S", 0, 0 },
        {"Sb", 0, 0 },
        {"Sc", 0, 0 },
        {"Se", 0, 0 },
        {"Si", 0, 0 },
        {"Sm", 0, 0 },
        {"Sn", 0, 0 },
        {"Sr", 0, 0 },
        {"Ta", 0, 0 },
        {"Tb", 0, 0 },
        {"Tc", 0, 0 },
        {"Te", 0, 0 },
        {"Th", 0, 0 },
        {"Ti", 0, 0 },
        {"Tl", 0, 0 },
        {"Tm", 0, 0 },
        {"U", 0, 0 },
        {"V", 0, 0 },
        {"Vol", 5, 0 },
        {"W", 0, 0 },
        {"Xe", 0, 0 },
        {"Y", 0, 0 },
        {"Yb", 0, 0 },
        {"Zn", 0, 0 },
        {"Zr", 0, 0 },
        {"Zz", 4, 0 }
    };
    /*
    * Generated with:
    std::cout << "dbElements_ = {" << std::endl;
    for (const auto& kv : dbElements_) {
        
        std::cout << std::setprecision(19) << "{{\"" << kv.first.Symbol() << "\", " << kv.first.Class() << ", " << kv.first.Isotope() << " }, {\"" <<
            kv.second.recid << "\", \"" << kv.second.name << "\", " << kv.second.number << ", " << 
            kv.second.valence << ", " << kv.second.atomic_mass << ", " << 
            kv.second.entropy << ", " << kv.second.heat_capacity << ", " <<
            kv.second.volume << " }}," << std::endl;
    }
    std::cout << "};" << std::endl << std::flush;
    */
    dbElements_ = {
        {{"Ag", 0, 0 }, {"", "Ag", 47, 1, 107.8682022094729973, 42.54999923706049714, 25.35000038146970169, 10.27200031280520065 }},
        {{"Al", 0, 0 }, {"", "Al", 13, 3, 26.98154067993160155, 28.29999923706050069, 24.20000076293949931, 9.993000030517579901 }},
        {{"Am", 0, 0 }, {"", "Am", 95, 3, 243.0610046386719887, 55.40000152587889914, 25.5, 17.62999916076660156 }},
        {{"Ar", 0, 0 }, {"", "Ar", 18, 0, 39.94800186157230115, 154.8459930419919885, 20.78599929809569957, 24790 }},
        {{"As", 0, 0 }, {"", "As", 33, 5, 74.92159271240230112, 35.68999862670899859, 24.65399932861329901, 12.96300029754639915 }},
        {{"Au", 0, 0 }, {"", "Au", 79, 1, 196.9669952392580115, 47.40000152587889914, 25.41799926757810013, 10.21500015258789951 }},
        {{"B", 0, 0 }, {"", "B", 5, 3, 10.81200027465819957, 5.900000095367429864, 11.08699989318849966, 4.386000156402589667 }},
        {{"Ba", 0, 0 }, {"", "Ba", 56, 2, 137.3280029296880116, 62.41999816894529829, 28.06999969482420099, 39.24000167846679688 }},
        {{"Be", 0, 0 }, {"", "Be", 4, 2, 9.012180328369140625, 9.5, 16.44300079345699928, 4.880000114440919745 }},
        {{"Bi", 0, 0 }, {"", "Bi", 83, 3, 208.9799957275390057, 56.74000167846679688, 25.5200004577637003, 21.30900001525879972 }},
        {{"Br", 0, 0 }, {"", "Br", 35, -1, 79.90399932861329546, 76.10500335693359375, 37.84500122070309658, 27.29000091552730112 }},
        {{"C", 0, 0 }, {"", "C", 6, 4, 12.01080036163330078, 5.739999771118159622, 8.682000160217290485, 4.584400177001949572 }},
        {{"Ca", 0, 0 }, {"", "Ca", 20, 2, 40.07799911499019885, 41.59000015258789773, 25.92900085449220171, 25.86000061035160158 }},
        {{"Cd", 0, 0 }, {"", "Cd", 48, 2, 112.4100036621090055, 51.79999923706049714, 26.0200004577637003, 12.99499988555909979 }},
        {{"Ce", 0, 0 }, {"", "Ce", 58, 3, 140.1199951171880116, 72, 26.94000053405759942, 20.67000007629389913 }},
        {{"Cl", 0, 0 }, {"", "Cl", 17, -1, 35.45299911499019885, 111.5404968261720029, 16.97450065612790127, 12395 }},
        {{"Cm", 0, 0 }, {"", "Cm", 96, 3, 247, 70.80000305175779829, 0, 0 }},
        {{"Co", 0, 0 }, {"", "Co", 27, 2, 58.93320083618159799, 30.04000091552730112, 24.80999946594240058, 6.619999885559080255 }},
        {{"Cr", 0, 0 }, {"", "Cr", 24, 3, 51.99599838256840201, 23.7700004577637003, 23.35000038146970169, 7.230999946594240058 }},
        {{"Cs", 0, 0 }, {"", "Cs", 55, 1, 132.9053955078119884, 85.23000335693359375, 32.20999908447269888, 69.73000335693359375 }},
        {{"Cu", 0, 0 }, {"", "Cu", 29, 2, 63.5460014343262003, 33.15000152587889914, 24.44000053405759942, 7.112999916076660156 }},
        {{"Dy", 0, 0 }, {"", "Dy", 66, 3, 162.5, 74.76999664306640625, 28.15999984741209872, 19.03000068664550071 }},
        {{"Er", 0, 0 }, {"", "Er", 68, 3, 167.2599945068359943, 73.18000030517579546, 28.12000083923339844, 18.4799995422362997 }},
        {{"Eu", 0, 0 }, {"", "Eu", 63, 3, 151.9640045166019888, 77.77999877929690342, 27.65999984741209872, 28.88999938964839842 }},
        {{"F", 0, 0 }, {"", "F", 9, -1, 18.99839973449709873, 101.3955001831049998, 15.65200042724610086, 12395 }},
        {{"Fe", 0, 0 }, {"", "Fe", 26, 2, 55.84500122070309658, 27.28000068664550071, 24.96100044250490058, 7.09200000762939009 }},
        {{"Ga", 0, 0 }, {"", "Ga", 31, 3, 69.71999359130859375, 40.88000106811519885, 25.86000061035160158, 11.7899999618530007 }},
        {{"Gd", 0, 0 }, {"", "Gd", 64, 3, 157.25, 68.06999969482420454, 37.02999877929690342, 19.88999938964839842 }},
        {{"Ge", 0, 0 }, {"", "Ge", 32, 4, 72.58999633789059658, 31.09000015258790128, 23.22200012207029829, 13.63000011444090021 }},
        {{"H", 0, 0 }, {"", "H", 1, 1, 1.007949948310850052, 65.33999633789059658, 14.40900039672849964, 12395 }},
        {{"He", 0, 0 }, {"", "He", 2, 0, 4.002600193023679864, 126.1529998779300001, 20.78599929809569957, 24790 }},
        {{"Hf", 0, 0 }, {"", "Hf", 72, 4, 178.4900054931640057, 43.56000137329100141, 25.7299995422362997, 13.52600002288819958 }},
        {{"Hg", 0, 0 }, {"", "Hg", 80, 2, 200.5899963378909945, 76.01999664306640625, 27.98299980163569955, 14.82600021362300069 }},
        {{"Ho", 0, 0 }, {"", "Ho", 67, 3, 164.9299926757809942, 75.30000305175779829, 27.14999961853029831, 18.73999977111819959 }},
        {{"I", 0, 0 }, {"", "I", 53, -1, 126.903999328612997, 58.06999969482419743, 27.21899986267089844, 25.7940006256104013 }},
        {{"In", 0, 0 }, {"", "In", 49, 3, 114.8199996948240056, 57.81999969482419743, 26.73999977111819959, 15.75 }},
        {{"Ir", 0, 0 }, {"", "Ir", 77, 4, 192.1999969482419885, 35.4799995422362997, 25.10000038146970169, 8.541999816894529474 }},
        {{"K", 0, 0 }, {"", "K", 19, 1, 39.09830093383789773, 64.67999267578119316, 29.60000038146970169, 45.46300125122070312 }},
        {{"Kr", 0, 0 }, {"", "Kr", 36, 0, 83.80000305175779829, 164.0850067138669885, 20.78599929809569957, 24789 }},
        {{"La", 0, 0 }, {"", "La", 57, 3, 138.9100036621090055, 56.90000152587889914, 27.11000061035160158, 22.46999931335449929 }},
        {{"Li", 0, 0 }, {"", "Li", 3, 1, 6.940999984741210049, 29.12000083923339844, 24.86000061035160158, 13.09599971771240057 }},
        {{"Lu", 0, 0 }, {"", "Lu", 71, 3, 174.9700012207030113, 50.95999908447269888, 26.86000061035160158, 17.7700004577637003 }},
        {{"Mg", 0, 0 }, {"", "Mg", 12, 2, 24.30500030517579901, 32.66999816894529829, 24.8689994812012003, 13.99600028991699929 }},
        {{"Mn", 0, 0 }, {"", "Mn", 25, 2, 54.93799972534179688, 32.00999832153320312, 26.31999969482420099, 7.394000053405759942 }},
        {{"Mo", 0, 0 }, {"", "Mo", 42, 6, 95.93998718261720171, 28.65999984741209872, 23.89999961853029831, 9.399999618530269885 }},
        {{"N", 0, 0 }, {"", "N", 7, 5, 14.00669956207280009, 95.80449676513670454, 14.56700038909909978, 12395 }},
        {{"Na", 0, 0 }, {"", "Na", 11, 1, 22.98979949951170099, 51.29999923706049714, 28.2299995422362997, 23.79999923706050069 }},
        {{"Nb", 0, 0 }, {"", "Nb", 41, 5, 92.90599822998049717, 36.40000152587889914, 24.60000038146970169, 10.86600017547610086 }},
        {{"Nd", 0, 0 }, {"", "Nd", 60, 3, 144.2400054931640057, 71.5, 27.45000076293949931, 20.56999969482420099 }},
        {{"Ne", 0, 0 }, {"", "Ne", 10, 0, 20.17900085449220171, 146.3280029296880116, 20.78599929809569957, 24789 }},
        {{"Ni", 0, 0 }, {"", "Ni", 28, 2, 58.69300079345700283, 29.87000083923339844, 26.06999969482420099, 6.58900022506713956 }},
        {{"Nia", 2, 0 }, {"", "Nia", 7, 0, 14.00669956207280009, 95.80449676513670454, 14.56700038909909978, 12395 }},
        {{"Nit", 2, 0 }, {"", "Nit", 99, 0, 14.00669956207280009, 95.80449676513670454, 14.56700038909909978, 12395 }},
        {{"Np", 0, 0 }, {"", "Np", 93, 6, 237.0482025146480112, 50.45999908447269888, 29.62000083923339844, 11.58920001983639914 }},
        {{"O", 0, 0 }, {"", "O", 8, -2, 15.99940013885500001, 102.5690002441409945, 14.66100025176999999, 12395 }},
        {{"Os", 0, 0 }, {"", "Os", 76, 4, 190.1999969482419885, 32.59999847412110086, 24.70000076293949931, 8.423999786376949572 }},
        {{"P", 0, 0 }, {"", "P", 15, 5, 30.97376060485839844, 41.09000015258789773, 23.82399940490720169, 17.0200004577637003 }},
        {{"Pa", 0, 0 }, {"", "Pa", 91, 5, 231.0359039306640057, 51.59999847412110086, 26.83600044250490058, 14.9799995422362997 }},
        {{"Pb", 0, 0 }, {"", "Pb", 82, 2, 207.1999969482419885, 64.80000305175779829, 26.64999961853029831, 18.18000030517579901 }},
        {{"Pd", 0, 0 }, {"", "Pd", 46, 2, 106.4199981689449999, 37.81999969482419743, 25.34000015258790128, 8.862000465393069959 }},
        {{"Po", 0, 0 }, {"", "Po", 84, 4, 209.9799957275390057, 62.79999923706049714, 26.39999961853029831, 23 }},
        {{"Pr", 0, 0 }, {"", "Pr", 59, 3, 140.9069976806640057, 73.19999694824220171, 27.20000076293949931, 20.80999946594240058 }},
        {{"Pt", 0, 0 }, {"", "Pt", 78, 2, 195.0899963378909945, 41.63000106811519885, 25.86000061035160158, 9.090999603271480822 }},
        {{"Pu", 0, 0 }, {"", "Pu", 94, 6, 244.0639953613280113, 54.45999908447269888, 31.48999977111819959, 12.0850000381469993 }},
        {{"Ra", 0, 0 }, {"", "Ra", 88, 2, 226.0249938964840055, 71, 28.70000076293949931, 45.20000076293950286 }},
        {{"Rb", 0, 0 }, {"", "Rb", 37, 1, 85.46779632568359375, 76.77999877929690342, 31.05999946594240058, 55.84999847412110086 }},
        {{"Re", 0, 0 }, {"", "Re", 75, 4, 186.1999969482419885, 36.86000061035159803, 25.4799995422362997, 8.859999656677249646 }},
        {{"Rh", 0, 0 }, {"", "Rh", 45, 2, 102.9049987792970029, 31.51000022888180041, 24.9799995422362997, 8.281999588012700642 }},
        {{"Rn", 0, 0 }, {"", "Rn", 86, 0, 222, 176.2100067138669885, 20.78599929809569957, 24789 }},
        {{"Ru", 0, 0 }, {"", "Ru", 44, 2, 101.0699996948240056, 28.53000068664550071, 24.05999946594240058, 8.170999526977540839 }},
        {{"S", 0, 0 }, {"", "S", 16, 6, 32.06700134277340197, 31.79800033569339845, 22.76300048828120026, 15.51000022888180041 }},
        {{"Sb", 0, 0 }, {"", "Sb", 51, 3, 121.75, 45.68999862670899859, 25.2299995422362997, 18.2299995422362997 }},
        {{"Sc", 0, 0 }, {"", "Sc", 21, 3, 44.95600128173830257, 34.63999938964840197, 25.5200004577637003, 15.0399999618530007 }},
        {{"Se", 0, 0 }, {"", "Se", 34, 4, 78.95999908447269888, 42.09000015258789773, 25.09000015258790128, 16.42000007629389913 }},
        {{"Si", 0, 0 }, {"", "Si", 14, 4, 28.08550071716310015, 18.80999946594240058, 19.78899955749509942, 12.05599975585939987 }},
        {{"Sm", 0, 0 }, {"", "Sm", 62, 3, 150.3500061035159945, 69.58000183105470171, 29.54000091552730112, 19.9799995422362997 }},
        {{"Sn", 0, 0 }, {"", "Sn", 50, 2, 118.7109985351559942, 51.21200180053710227, 26.35199928283689985, 16.29000091552730112 }},
        {{"Sr", 0, 0 }, {"", "Sr", 38, 2, 87.62000274658200283, 55.70000076293950286, 26.39999961853029831, 33.91999816894529829 }},
        {{"Ta", 0, 0 }, {"", "Ta", 73, 5, 180.947998046875, 41.50999832153320312, 25.36000061035160158, 10.89999961853030008 }},
        {{"Tb", 0, 0 }, {"", "Tb", 65, 3, 158.9239959716800001, 73.22000122070309658, 28.90999984741209872, 19.21999931335449929 }},
        {{"Tc", 0, 0 }, {"", "Tc", 43, 7, 97.90699768066410513, 32.5, 24.89999961853029831, 8.199999809265140271 }},
        {{"Te", 0, 0 }, {"", "Te", 52, 6, 127.5999984741209943, 49.22200012207029829, 25.56459999084470169, 2.045000076293950197 }},
        {{"Th", 0, 0 }, {"", "Th", 90, 4, 232.0381011962890057, 51.79999923706049714, 26.2299995422362997, 19.79000091552730112 }},
        {{"Ti", 0, 0 }, {"", "Ti", 22, 4, 47.88000106811519885, 30.71999931335449929, 25.05999946594240058, 10.63000011444090021 }},
        {{"Tl", 0, 0 }, {"", "Tl", 81, 1, 204.3699951171880116, 64.17999267578119316, 26.31999969482420099, 17.20999908447269888 }},
        {{"Tm", 0, 0 }, {"", "Tm", 69, 3, 168.9340057373050001, 74.01000213623049717, 27.03000068664550071, 18.12999916076660156 }},
        {{"U", 0, 0 }, {"", "U", 92, 6, 238.0289001464840055, 50.20000076293950286, 27.65999984741209872, 12.49699974060060015 }},
        {{"V", 0, 0 }, {"", "V", 23, 5, 50.94200134277340197, 28.90999984741209872, 24.88999938964839842, 8.782999992370610798 }},
        {{"Vol", 5, 0 }, {"", "Vol", 100, 0, 0, 0, 0, 1 }},
        {{"W", 0, 0 }, {"", "W", 74, 6, 183.8500061035159945, 32.63999938964840197, 24.25, 9.526000022888180041 }},
        {{"Xe", 0, 0 }, {"", "Xe", 54, 0, 131.2899932861330115, 169.6849975585940058, 20.78599929809569957, 24789 }},
        {{"Y", 0, 0 }, {"", "Y", 39, 3, 88.90499877929690342, 44.43000030517580257, 26.53000068664550071, 15.0399999618530007 }},
        {{"Yb", 0, 0 }, {"", "Yb", 70, 3, 173.0399932861330115, 59.86999893188480115, 26.73999977111819959, 24.82999992370610087 }},
        {{"Zn", 0, 0 }, {"", "Zn", 30, 2, 65.37999725341799717, 41.63000106811519885, 25.38999938964839842, 9.156999588012700642 }},
        {{"Zr", 0, 0 }, {"", "Zr", 40, 4, 91.2239990234375, 39.08000183105470171, 26.07999992370610087, 14.05000019073490058 }},
        {{"Zz", 4, 0 }, {"", "Zz", 0, 0, 0, -65.33999633789059658, -14.41800022125240055, 0 }}
    };
#else
    readElements(dbElements_default1+dbElements_default2);
#endif
}


int DBElements::defaultValence(const std::string& symbol)
{
    auto itv = map_elements_valences.find(symbol);
    if (itv != map_elements_valences.end())
        return itv->second;
    else
        return 0;
}

void DBElements::addElement(const ElementKey& elkey, const ElementValues& elvalue)
{
    dbElements_[elkey] = elvalue;
    dbElementsKeys_.insert(elkey);
}

void DBElements::readElements(const std::string &json_array)
{
    dbElements_.clear();
    dbElementsKeys_.clear();
    auto elements_json = json::parse(json_array);
    for( const auto& el_json: elements_json) {
        addElement(element_key_from(el_json["element"]),
                element_values_from(el_json["properties"]));
    }
}

std::string DBElements::writeElements(bool dense) const
{
    auto elements_json = json::array();
    for( const auto& element: dbElements_) {
        auto elem = json::object();
        elem["element"] = element_key_to(element.first);
        elem["properties"] = element_values_to(element.second);
        elements_json.push_back(elem);
    }
    return elements_json.dump(dense?4:-1);
}

ElementsKeys DBElements::formulasElements(const std::vector<std::string>& formulalist)
{
    ElementsKeys elements;
    FormulaToken formula("");
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula);
        elements.insert(formula.getElementsList().begin(), formula.getElementsList().end());
    }
    return elements;
}

std::vector<FormulaProperties> DBElements::formulasProperties(const std::vector<std::string>& formulalist, bool use_formula_charge)
{
    std::vector<FormulaProperties> thermo;
    ChemicalFun::chfun_logger->debug("get_charge_from_formula {} ", charge_from_formula());
    for(const auto& aformula: formulalist) {
        thermo.push_back(formulasProperties(aformula, use_formula_charge));
    }
    return thermo;
}

StoichiometryMatrixData DBElements::stoichiometryMatrix(const std::vector<std::string>& formulalist)
{
    StoichiometryMatrixData matrA;
    FormulaToken formula("");
    std::vector<ElementKey> allelemens = elementsKeysList();
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula);
        matrA.push_back(formula.makeStoichiometryRow(allelemens));
    }
    return matrA;
}

void DBElements::printStoichiometryMatrix(std::ostream& stream, const std::vector<std::string>& formulalist)
{
    StoichiometryMatrixData matrA = stoichiometryMatrix(formulalist);
    stream << "formula,";
    for(const auto& elname: elementsKeysList()) {
        stream << elname.Symbol() <<",";
    }
    stream << std::endl;
    for(size_t jj=0; jj<matrA.size(); ++jj) {
        stream << formulalist[jj] << "," ;
        for(size_t ii=0; ii<matrA[jj].size(); ii++) {
            stream << matrA[jj][ii] << ",";
        }
        stream << std::endl;
    }
}

void DBElements::formulasPropertiesCSV(std::ostream& stream,
                                       const std::vector<std::string>& formulalist,
                                       bool use_formula_charge)
{
    std::vector<FormulaProperties> thermo = formulasProperties(formulalist, use_formula_charge);
    stream << "formula,charge,atomic_mass,elemental_entropy,atoms_formula\n";
    for( const auto& row: thermo) {
        stream << row.formula << ","<< row.charge << "," << row.atomic_mass << ",";
        stream << row.elemental_entropy << ","<< row.atoms_formula_unit << std::endl;
    }
}

void DBElements::printCSV(std::ostream& stream)
{
    stream << "symbol,class_,isotope,atomic_mass,";
    stream << "entropy,heat_capacity,volume,valence,number" << std::endl;
    for( const auto& eldata: dbElements_ ) {
        stream << eldata.first.Symbol() << ","<< eldata.first.Class() << ",";
        stream << eldata.first.Isotope() << ","<< eldata.second.atomic_mass << ",";
        stream << eldata.second.entropy << ","<< eldata.second.heat_capacity << ",";
        stream << eldata.second.volume << ","<< eldata.second.valence << ",";
        stream << eldata.second.number;
        stream << std::endl;
    }
}

std::vector<ElementKey> elementsInFormulas(const std::vector<std::string> &formulalist, bool valence)
{
    FormulaToken formula("");
    ElementsKeys all_elements_set;
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula, valence);
        all_elements_set.insert(formula.getElementsList().begin(), formula.getElementsList().end());
    }
    return {all_elements_set.begin(), all_elements_set.end()};
}

StoichiometryMatrixData substancesStoichiometryMatrix(const std::vector<std::string> &formulalist, bool valence)
{
    StoichiometryMatrixData matrA;
    auto all_elements = elementsInFormulas(formulalist, valence);
    FormulaToken formula("");
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula, valence);
        matrA.push_back(formula.makeStoichiometryRow(all_elements));
    }
    return matrA;
}

StoichiometryMatrixData stoichiometryMatrix(const std::vector<std::string> &formulalist,
                                                      std::vector<ElementKey> all_elements)
{
    StoichiometryMatrixData matrA;
    FormulaToken formula("");
    for(const auto& aformula: formulalist) {
        formula.setFormula(aformula, false);
        matrA.push_back(formula.makeStoichiometryRow(all_elements));
    }
    return matrA;
}

/// Global default settings for calculating the charge method.
/// If get_charge_from_formula false, calculate the charge based on the elements and their default
/// or specified valence; otherwise, take the charge based on the symbol in the given formula.
static bool get_charge_from_formula = false;

bool charge_from_formula()
{
    return get_charge_from_formula;
}

void set_charge_from_formula(bool cond)
{
    get_charge_from_formula = cond;
}

}
