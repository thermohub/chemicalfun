//#include <math.h>
#include "ChemicalFun/FormulaParser/formuladata.h"
#include "ChemicalFun/FormulaParser/ChemicalFormulaParser.h"
#include "Common/Exception.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

namespace ChemicalFun {
///S using ElementsMap   = std::map<std::string, Element>;
static const short SHORT_EMPTY_  = -32768;
static const char* NOISOTOPE_CLASS  ="n";
static const char* CHARGE_CLASS   ="z";
DBElementsData ChemicalFormula::dbElements= DBElementsData();


const std::map<std::string, int> map_elements_valences = {
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
        symbol = key_object["symbol"].get<string>();
    }
    if (key_object.contains("isotope_mass")) {
        isotope = key_object["isotope_mass"].get<int>();
    }
    if (key_object.contains("class_")) {
        class_ = key_object["class_"].get<int>();
    }
    funErrorIf( symbol.empty(), "Undefined symbol.", "Element ", __LINE__, __FILE__ );
    return ElementKey{ symbol, class_, isotope };
}

// Writes ElementKey data to json
static json element_values_to(const ElementValues& values)
{
    auto object = json::object();
    object["recid"] = values.recid;
    object["atomic_mass"] = values.atomic_mass;
    object["entropy"] = values.entropy;
    object["heat_capacity"] = values.heat_capacity;
    object["volume"] = values.volume;
    object["valence"] = values.valence;
    object["number"] = values.number;
    object["name"] = values.name;
    return object;
}

static ElementValues element_values_from(const json& val_object)
{
    ElementValues data;
    if (val_object.contains("recid")) {
        data.recid = val_object["recid"].get<string>();
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
    if (val_object.contains("valence")) {
        data.valence = val_object["valence"].get<int>();
    }
    if (val_object.contains("number")) {
        data.number = val_object["number"].get<int>();
    }
    if (val_object.contains("name")) {
        data.name = val_object["name"].get<string>();
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

string ElementKey::to_json_string(bool dense) const
{
    auto el_key = element_key_to(*this);
    return el_key.dump(dense?4:-1);
}

void ElementValues::from_json_string(const std::string &json_string)
{
    auto values_json = json::parse(json_string);
    *this =element_values_from(values_json);
}

string ElementValues::to_json_string(bool dense) const
{
  auto el_values = element_values_to(*this);
  return el_values.dump(dense?4:-1);
}

string FormulaValues::to_json_string(bool dense) const
{
    auto data = formula_key_to(*this);
    return data.dump(dense?4:-1);
}

string FormulaProperites::to_json_string(bool dense) const
{
    auto data = formula_properites_to(*this);
    return data.dump(dense?4:-1);
}


//-------------------------------------------------------------------------------

// Construct key from elements document values
ElementKey::ElementKey(const std::string& asymbol, const std::string& aclass_, const std::string& aisotope):
    symbol(asymbol)
{
    json j = json::parse(aclass_);
    isotope = 0;
    if( !aisotope.empty() )
           isotope = stoi(aisotope);
    class_  = stoi(j.begin().key());
}

void ElementKey::class_isotope_from(const string& typeline)
{
  class_ = 0;  // ElementClass::ELEMENT schema.enumdef->getId( "ELEMENT" );
  isotope = 0;

  if( typeline == CHARGE_CLASS )
      class_ = 4; // ElementClass::CHARGE schema.enumdef->getId( "CHARGE" );
  else
      if( typeline == NOISOTOPE_CLASS || typeline == "v" )
      {

      }
      else
//          if( jsonio::is<int>( isotope, typeline.c_str()) )
//             class_ = 1; // ElementClass::ISOTOPE schema.enumdef->getId( "ISOTOPE" );
//          else
             isotope = 0;
  //cout << typeline << " class_ " << class_ << " isotope " << isotope << endl;

}

//// Writes data to json (only key)
//jsonio17::JsonFree toKeyNode(  ElementKey el_key )
//{
//    jsonio17::JsonFree object = jsonio17::JsonFree::object();
//    object[ "symbol" ] = el_key.Symbol();
//    if( el_key.Isotope() != 0 )
//        object["isotope_mass"] =  el_key.Isotope();
//    if( el_key.Class()!= 0 )
//        object["class_"] = el_key.Class();
//    return object;
//}

//// Reads data from JsonDom (only key)
//auto fromKeyNode( const jsonio17::JsonBase& object ) -> ElementKey
//{
//    std::string symbol;
//    int isotope, class_;
//    if(!object.get_value_via_path<std::string>( "symbol", symbol, "" ) )
//        jsonio17::JSONIO_THROW( "ElementKey", 102, "Undefined symbol.");
//    object.get_value_via_path( "isotope_mass", isotope, 0 );
//    object.get_value_via_path("class_", class_, 0 );
//    return ElementKey(symbol,class_,isotope);
//}

string ElementKey::formulaKey() const
{
  string _key= symbol;
  if( class_ != 0 )
   _key+= "|" + to_string(class_);
  if( isotope != 0 )
   _key+= "|" + to_string(isotope);

 return _key;
}

// Reads data from JsonDom (fromm real record)
void ElementKey::fromElementNode( const std::string& element )
{
  json j = json::parse(element);

  if (j.contains("symbol"))
    symbol = j["symbol"].get<string>();
  else
    funError("Undefined symbol.", "Element ", __LINE__, __FILE__ );
  if (j.contains("isotope_mass"))
    isotope = j["isotope_mass"].get<int>();
  else
    isotope = 0;
  if (j.contains("class_"))
    class_ = stoi(j["class_"].begin().key());
  else
      class_ = 0;
}


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

ElementKey::ElementKey(const std::string &asymbol, const std::string &typeline):
    symbol(asymbol)
{ class_isotope_from(typeline); }


//-------------------------------------------------------------

void FormulaToken::setFormula( const string& aformula )
{
  clear();
  formula = aformula;
  formula.erase(std::remove(formula.begin(), formula.end(), '\"'), formula.end());
  ChemicalFormulaParser formparser;
  auto icterms = formparser.parse( formula );
  unpack( icterms );
  aZ = calculateCharge();
}


// unpack list of terms to data
void FormulaToken::unpack( list<ElementsTerm>& itt_ )
{
    auto itr = itt_.begin();
    while( itr != itt_.end() )
    {
        ElementKey key( itr->name(), itr->isotope() );

        if( itr->valence() == SHORT_EMPTY_ )
        {
             auto itrdb = ChemicalFormula::getDBElements().find(key);
             if( itrdb !=  ChemicalFormula::getDBElements().end() )
                itr->element_valence = itrdb->second.valence;
        }
        datamap.push_back( FormulaValues( key, itr->stoich_coef(), itr->valence() ));
        elements.insert(key);
        if (elements_map.find(key) != elements_map.end())
            elements_map.at(key) += itr->stoich_coef();
        else
            elements_map.insert(pair<ElementKey,double>(key,itr->stoich_coef()));
//        coefficients.push_back(itr->stoich);
        itr++;
    }
}

double FormulaToken::calculateCharge()
{
    double  Zz=0.0;
    auto itr =  datamap.begin();
    while( itr != datamap.end() )
    {
      if( itr->key.Class() !=  4 /*CHARGE*/ &&
          itr->valence != SHORT_EMPTY_ )
         Zz += itr->stoich_coef * itr->valence;
      itr++;
    }
    return Zz;
}


void FormulaToken::clear()
{
    datamap.clear();
    elements.clear();
    elements_map.clear();
    aZ = 0.;
}


bool FormulaToken::checkElements( const string& aformula )
{
  string notPresent = testElements( aformula );
  return notPresent.empty();
}

void FormulaToken::exeptionCheckElements( const string& subreacKey, const string& aformula )
{
  string notPresent = testElements( aformula );
  if( !notPresent.empty() )
  {
      string msg = "Invalid Elements: ";
      msg += notPresent;
      msg += "\n in formula in record: \n";
      msg += subreacKey;
      funError("Invalid symbol", msg, __LINE__, __FILE__);
  }
}

// Test all IComps in DB
string FormulaToken::testElements( const string& aformula )
{
    string notPresent = "";
    setFormula( aformula );

    auto itr = datamap.begin();
    while( itr != datamap.end() )
    {
      if( ChemicalFormula::getDBElements().find(itr->key) ==
              ChemicalFormula::getDBElements().end() )
        notPresent += itr->key.Symbol() + ";";
      itr++;
    }
    return notPresent;
}

void FormulaToken::calcFormulaProperites( FormulaProperites& propert )
{
    double Sc;
    int valence;
    propert.formula = formula;
    propert.charge = propert.atomic_mass = 0.0;
    propert.elemental_entropy = propert.atoms_formula_unit = 0.0;

    auto itr = datamap.begin();
    while( itr != datamap.end() )
    {
      auto itrdb = ChemicalFormula::getDBElements().find(itr->key);
      if( itrdb ==  ChemicalFormula::getDBElements().end() )
          funError("Invalid symbol", itr->key.Symbol(), __LINE__, __FILE__);

      Sc = itr->stoich_coef;
      propert.atoms_formula_unit += Sc;
      propert.atomic_mass += Sc * itrdb->second.atomic_mass;
      propert.elemental_entropy += Sc * itrdb->second.entropy;

      valence = itr->valence;
      if( valence == SHORT_EMPTY_ )
          valence = itrdb->second.valence;
      if( itr->key.Class() !=  4 /*CHARGE*/ )
          propert.charge += Sc * valence;

      itr++;
    }
}


// Get a row of stoichiometry matrix
vector<double> FormulaToken::makeStoichiometryRowOld( const vector<ElementKey>& sysElemens )
{
    double ai = 0;
    vector<double> rowA;

    auto itel = sysElemens.begin();
    while( itel != sysElemens.end() )
    {
      ai=0.;
      const ElementKey& elkey = *itel;
      if( elements.find(elkey) != elements.end() )
      {
          for(unsigned int ii=0; ii<datamap.size(); ii++ )
              if( elkey == datamap[ii].key )
                 ai += datamap[ii].stoich_coef;
      }
      rowA.push_back(ai);
      itel++;
    }
    return rowA;
}

void FormulaToken::exeptionCargeImbalance()
{
    ElementKey chargeKey( CHARGE_NAME,4,0 );
    if( elements.find(chargeKey) == elements.end()  )
        return;

    auto itr =  datamap.begin();
    while( itr != datamap.end() )
    {
        if( chargeKey == itr->key )
        {
            double Zzval = itr->stoich_coef;

            if( fabs( (aZ - Zzval) ) > 1e-6 )
            {
                string str = "In the formula: ";
                str +=  formula + "\n calculated charge: ";
                str +=  to_string(aZ) + " != " + to_string(Zzval);
                //aSC[ii] = aZ;  // KD 03.01.04  - temporary workaround (adsorption)
                funError("Charge imbalance", str, __LINE__, __FILE__);
            }
            break;
        }
    }
}


//------------------------------------------

vector<ElementKey> ChemicalFormula::elementsRow()
{
    vector<ElementKey> row;
    auto itr =dbElements.begin();

    while( itr != dbElements.end() )
      row.push_back( ElementKey(itr++->first) );
    return row;
}

map<ElementKey, double> ChemicalFormula::extractElements_map(  const vector<string>& formulalist )
{
   map<ElementKey, double> elements_map;
   FormulaToken formula("");

   for(unsigned int ii=0; ii<formulalist.size(); ii++ )
   {
     formula.setFormula(  formulalist[ii] );
     elements_map.insert( formula.getElements_map().begin(), formula.getElements_map().end());
   }
   return elements_map;
}

set<ElementKey> ChemicalFormula::extractElements(  const vector<string>& formulalist )
{
   set<ElementKey> elements;
   FormulaToken formula("");

   for(unsigned int ii=0; ii<formulalist.size(); ii++ )
   {
     formula.setFormula(  formulalist[ii] );
     elements.insert( formula.getElements().begin(), formula.getElements().end());
   }
   return elements;
}

vector<FormulaProperites> ChemicalFormula::calcThermo(  const vector<string>& formulalist )
{
   vector<FormulaProperites> thermo;
   FormulaToken formula("");
   FormulaProperites val;

   for(unsigned int ii=0; ii<formulalist.size(); ii++ )
   {
     formula.setFormula(  formulalist[ii] );
     formula.calcFormulaProperites( val );
     thermo.push_back( val );
   }

   return thermo;
}

FormulaProperites ChemicalFormula::calcThermo(  const string formula_ )
{
   FormulaToken formula("");
   FormulaProperites val;
   formula.setFormula(  formula_ );
   formula.calcFormulaProperites( val );
   return val;
}

vector<vector<double>> ChemicalFormula::calcStoichiometryMatrixOld(  const vector<string>& formulalist )
{
   vector<vector<double>> matrA;
   vector<ElementKey> sysElemens = elementsRow();
   FormulaToken formula("");

   for(unsigned int ii=0; ii<formulalist.size(); ii++ )
   {
     formula.setFormula( formulalist[ii] );
     matrA.push_back( formula.makeStoichiometryRowOld( sysElemens ) );
   }

   return matrA;
}

void ChemicalFormula::readDBElements(const std::string &json_array)
{
    auto elements_json = json::parse(json_array);
    for( const auto& el_json: elements_json) {

        dbElements[element_key_from(el_json["element"])] =
                element_values_from(el_json["properties"]);
    }
}

std::string ChemicalFormula::writeDBElements(bool dense)
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









/*
void ChemicalFormula::setDBElements(ElementsMap elements )
{
    for (auto e : elements)
        addOneElement(e.second);
}

void ChemicalFormula::addOneElement(Element e)
{
    ElementKey elkey(e.symbol(), e.class_(), e.isotopeMass() );
    ElementValues eldata;

//    eldata.recid =;
    eldata.atomic_mass = e.molarMass();
    eldata.entropy = e.entropy();
    eldata.heat_capacity = e.heatCapacity();
    eldata.volume = e.volume();
    if (e.valence()==777)
    {
        if (elements_valences.find(e.symbol()) != elements_valences.end())
            e.setValence(elements_valences.at(e.symbol()));
        else
            e.setValence(0);
    }
    eldata.valence = e.valence();
    eldata.number = e.number();
    eldata.name = e.symbol(); // was e.name();

    dbElements[elkey] = eldata;
}

auto elementKeyToElement(ElementKey elementKey) -> Element
{
    Element e;
    auto itrdb = ChemicalFormula::getDBElements().find(elementKey);
    if (itrdb == ChemicalFormula::getDBElements().end())
        funError("Invalid symbol", elementKey.Symbol(), __LINE__, __FILE__);

    e.setClass(elementKey.Class());
    e.setIsotopeMass(elementKey.Isotope());
    e.setSymbol(elementKey.Symbol());
    e.setName(itrdb->second.name);
    e.setMolarMass(itrdb->second.atomic_mass);
    e.setEntropy(itrdb->second.entropy);
    e.setHeatCapacity(itrdb->second.heat_capacity);
    e.setVolume(itrdb->second.volume);
    e.setValence(itrdb->second.valence);
    e.setNumber(itrdb->second.number);

    return e;
}
*/

}
