#include <sstream>
#include <iomanip>
#include "ChemicalFun/FormulaParser/ChemicalFormulaParser.h"
#include "Common/Exception.h"

namespace ChemicalFun {

static const int MAXICNAME = 10;
static const char* NOISOTOPE_CLASS  ="n";
static const char* CHARGE_CLASS_S   ="z";
static const short SHORT_EMPTY_  = -32768;
static const char* CHARGE_TOK    ="+-@";

bool is_undefined_valence(int valence)
{
    return valence == SHORT_EMPTY_;
}

bool ElementsTerm::default_valence() const
{
    return element_valence == SHORT_EMPTY_;
}

bool ElementsTerm::is_charge() const
{
    return element_isotope == CHARGE_CLASS_S;
}

bool ElementsTerm::no_isotope() const
{
    return ( element_isotope == NOISOTOPE_CLASS ||
             element_isotope == CHARGE_CLASS_S ||
             element_isotope == "v" );
}

int ElementsTerm::class_() const
{
    int vclass_ = ELEMENT_CLASS;  // ElementClass::ELEMENT schema.enumdef->getId( "ELEMENT" );
    if( is_charge() )
        vclass_ = CHARGE_CLASS;  // ElementClass::CHARGE schema.enumdef->getId( "CHARGE" );
//    else {
//        int isotope = 0;
//        if( !no_isotope() && is<int>( isotope, element_isotope) )
//            vclass_ = ISOTOPE_CLASS; // ElementClass::ISOTOPE schema.enumdef->getId( "ISOTOPE" );
//    }
    return vclass_;
}

int ElementsTerm::isotope() const
{
    int visotope = 0;
//    if( !no_isotope()  ) {
//        if( !is<int>( visotope, element_isotope) ) {
//            visotope = 0;
//        }
//    }
    return  visotope;
}

std::string ElementsTerm::to_string() const
{
    std::stringstream s_stream;
    s_stream << *this;
    return s_stream.str();
}

std::ostream &operator<<(std::ostream &os, const ElementsTerm &term_data)
{
    os << term_data.name();
    if( !term_data.no_isotope() ) {
        os << '/' << term_data.str_isotope() << '/';
    }
    os << ":" << (  term_data.default_valence() ? "*" : std::to_string(term_data.valence()) )
       << ":" << std::setprecision(15) << term_data.stoich_coef();
    return os;
}

//------------------------------------------------------------------------------------------------

void BaseParser::xblanc(std::string& str)
{
    if(str.empty()) {
        return;
    }
    size_t ti = str.find_first_not_of(" \n\t\r");
    if( ti == std::string::npos ) {
        str = "";
    }
    else {
        str = str.substr(ti);
    }
}

//  <elem_st_coef>  ::= <double>
void BaseParser::get_real(double& value, std::string& parsed_string)
{
    xblanc( parsed_string );
    if( parsed_string.empty() ) {
        return;
    }
    if( isdigit(parsed_string[0]) || parsed_string[0]=='.' || parsed_string[0]=='e')  {
        std::string::size_type sz;
        value = stod(parsed_string,&sz);
        parsed_string = parsed_string.substr(sz);
    }
}

// --------------------------------------------------------------

ChemicalFormulaParser::~ChemicalFormulaParser()
{}

//get <formula>  ::= <fterm> | <fterm><charge>
std::list<ElementsTerm> ChemicalFormulaParser::parse( const std::string& aformula )
{
    formula = aformula;
    charge = "";
    std::list<ElementsTerm> terms_list;

    scan_charge();
    std::string no_charge_part = formula;
    scan_formula( terms_list, no_charge_part, '\0');

    // added charge item
    if( !charge.empty() ) {
        add_charge( terms_list );
    }
    // TEST OUTPUT
    //std::cout << "Formula: " << aformula;
    //for( const auto& aterm: newtt) {
    //   std::cout << "\n   " << aterm;
    //}
    return terms_list;
}

int ChemicalFormulaParser::term_compare(const ElementsTerm& term,
                                        const ElementsTerm& new_term) const
{
    if( term.name() < new_term.name() ) return(-1);
    if( term.name() > new_term.name() ) return(1);
    if( term.valence() < new_term.valence() ) return(-1);
    if( term.valence() > new_term.valence() ) return(1);
    return(0);
}

// add component to sorted list
void ChemicalFormulaParser::add_element(std::list<ElementsTerm>& terms_list,
                                        const ElementsTerm& new_term )
{
    // test for setting element before
    auto  itr = terms_list.begin();
    while( itr != terms_list.end() ) {
        auto iRet = term_compare( *itr, new_term );
        if( iRet == 0 ) {
            //  exist
            itr->element_stoich_coef += new_term.element_stoich_coef;
            return;
        }
        if( iRet > 0 ) {
            break;
        }
        itr++;
    }
    terms_list.insert(itr, new_term);
}

void ChemicalFormulaParser::scan_charge()
{
    size_t ti = formula.find_last_of(CHARGE_TOK );
    if( ti == std::string::npos ) {
        // no charge token
        return;
    }
    size_t pp = formula.find( B_VALENT, ti);
    if( pp != std::string::npos ) {
        // no charge (this is valence)
        return;
    }
    // get charge std::string
    charge = formula.substr(ti);
    formula = formula.substr(0,ti);
}

// read charge
void ChemicalFormulaParser::add_charge(std::list<ElementsTerm>& terms_list)
{
    double cha = 1.0;
    int sign = 1;
    double aZ = 0.0;
    std::string chan = charge;

    switch( chan[0] )
    {
    case CHARGE_NUL:    break;
    case CHARGE_MINUS:  sign = -1;
        [[fallthrough]];
    case CHARGE_PLUS:
        chan = chan.substr(1);
        get_real( cha, chan );
        aZ = cha * sign;
        break;
    default:            break;
    }
    add_element( terms_list, ElementsTerm{CHARGE_NAME, CHARGE_CLASS_S, 0, aZ} );
}

//get <fterm>  ::= <icterm> | <icterm><icterm>
//    <icterm> ::= <elem>   | <elem>< elem_st_coef>
void ChemicalFormulaParser::scan_formula(std::list<ElementsTerm>& terms_list, std::string& start_pos, char end_simb)
{
    double st_coef;
    std::list<ElementsTerm> el_list;

    while( start_pos[0] != end_simb && !start_pos.empty()) {
        // list of <elem><elem_st_coef>
        el_list.clear();
        scan_element(el_list, start_pos);

        if( !start_pos.empty() ) {
            // get elem_st_coef
            st_coef = 1.;
            get_real(st_coef, start_pos);
            for(auto& el_term: el_list) {
                el_term.element_stoich_coef *= st_coef;
            }
        }
        // added elements list to top level elements
        for(const auto& el_term: el_list) {
            add_element(  terms_list, el_term );
        }
        xblanc(start_pos);
        if( start_pos.empty() ) {
            return;
        }
    }
}

//get <elem>    ::= (<fterm>) | [<fterm>] |
//                   <isotope_mass><icsymb><valence> |
//                   <isotope_mass><icsymb> |
//                   <icsymb><valence> | <icsymb>
void ChemicalFormulaParser::scan_element(std::list<ElementsTerm>& terms_list, std::string& parsed_string)
{
    xblanc(parsed_string);
    if( parsed_string.empty() ) {
        return;
    }

    switch( parsed_string[0] ) {
    case LBRACKET1:
        parsed_string = parsed_string.substr(1);
        scan_formula( terms_list, parsed_string, RBRACKET1 );
        funErrorIf(parsed_string[0]!=RBRACKET1, "Formula", "Must be )", __LINE__, __FILE__ );
        parsed_string = parsed_string.substr(1);
        break;
    case LBRACKET2:
        parsed_string = parsed_string.substr(1);
        scan_formula( terms_list, parsed_string, RBRACKET2 );
        funErrorIf(parsed_string[0]!=RBRACKET2, "Formula", "Must be ]", __LINE__, __FILE__ );
        parsed_string = parsed_string.substr(1);
        break;
    case LBRACKET3:
        parsed_string = parsed_string.substr(1);
        scan_formula( terms_list, parsed_string, RBRACKET3 );
        funErrorIf(parsed_string[0]!=RBRACKET3, "Formula", "Must be }", __LINE__, __FILE__ );
        parsed_string = parsed_string.substr(1);
        break;
    case   PSUR_L_PLUS:
        parsed_string = parsed_string.substr(1);
        break;
    case   'V':
        if( parsed_string[1] == 'a' ) {
            // Va - ignore vacancy
            parsed_string = parsed_string.substr(2);
            break;
        } // else goto default - other <icsymb>
        [[fallthrough]];
    default: // <isotope_mass><icsymb><valence>
    {
        std::string isotop = std::string(NOISOTOPE_CLASS);
        std::string el_name = "";
        int val = SHORT_EMPTY_;

        scan_isotope( isotop, parsed_string);
        scan_symbol( el_name, parsed_string);
        scan_valence( val, parsed_string);
        add_element( terms_list, ElementsTerm{ el_name, isotop, val, 1.} );
        break;
    }
    }
}

// get <valence>   ::= |-<integer>| \ |+<integer>| \ |<integer>|
//  <integer>    ::= <num>
void ChemicalFormulaParser::scan_valence(int& valence, std::string& parsed_string)
{
    xblanc(parsed_string);
    if( parsed_string.empty() ) {
        return;
    }
    if( parsed_string[0] != B_VALENT ) {
        // next token no valence
        return;
    }
    parsed_string = parsed_string.substr(1);
    funErrorIf(parsed_string.empty() ,"Valence", "Term valence scan error", __LINE__, __FILE__ );
    size_t ti = parsed_string.find_first_of(B_VALENT);
    funErrorIf(ti >= 3 || ti==std::string::npos, "Valence", "Term valence scan error", __LINE__, __FILE__ );

    if( !sscanf( parsed_string.c_str(), " %d", &valence )) {
        funError("Valence", "Integer number scan error", __LINE__, __FILE__ );
    }
    parsed_string = parsed_string.substr(ti+1);
}

// /3/H2/18/O             isotopic form of water.
//  get <isotope_mass>  ::= /<integer>/
void ChemicalFormulaParser::scan_isotope(std::string& isotop, std::string& parsed_string)
{
    xblanc(parsed_string);
    if( parsed_string.empty() ) {
        return;
    }
    if( parsed_string[0] != B_ISOTOPE ) {
        // next token no isotop
        return;
    }
    parsed_string = parsed_string.substr(1);
    funErrorIf(parsed_string.empty(), "Isotope","Term isotope scan error", __LINE__, __FILE__ );
    auto ti = parsed_string.find_first_of(B_ISOTOPE);
    funErrorIf(ti >= MAXICNAME || ti==std::string::npos, "Isotope","Term isotope scan error", __LINE__, __FILE__ );
    isotop = std::string( parsed_string, 0, ti );
    parsed_string = parsed_string.substr(ti+1);
}

// <icsymb>    ::= <Capital_letter> \ <icsymb><lcase_letter> \ <icsymb>_
void ChemicalFormulaParser::scan_symbol(std::string& icname, std::string& parsed_string)
{
    std::size_t i=1;

    xblanc( parsed_string );
    if( parsed_string.empty() ) {
        return;
    }
    if( !is_capital(parsed_string[0]) ) {
        funError("Fromula Parser"," A symbol of element expected here!", __LINE__, __FILE__ );
    }
    for( i=1; i<=MAXICNAME+2; i++ ) {
        if( !is_lowercase(parsed_string[i]) ) {
            break;
        }
    }
    funErrorIf(i>=MAXICNAME, "Fromula Parser","IC Symbol scan error", __LINE__, __FILE__ );
    icname = std::string( parsed_string, 0, i ); //  strncpy( ic, aFa.cur, len );
    parsed_string = parsed_string.substr(i);
}

}
