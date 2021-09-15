#include <sstream>
#include <iomanip>
#include "ChemicalFun/FormulaParser/MoityParser.h"
#include "Common/Exception.h"

namespace ChemicalFun {

std::string MoietyTerm::to_string() const
{
    std::stringstream s_stream;
    s_stream << *this;
    return s_stream.str();
}

std::ostream &operator<<(std::ostream &os, const MoietyTerm& moi_data)
{
    os << moi_data.name() <<  ":" << std::setprecision(15) << moi_data.moiety_site_occupancy();
    return os;
}

MoityParser::~MoityParser()
{}

// <fterm>  ::= <site_term> : | <fterm> <site_term>:
// <site_term> ::= <moiety>   | <moiety><moiety>
// <moiety>    ::= {<elem>}   | {<elem>} <elem_st_coef> | Va
std::vector<MoietyTerm> MoityParser::parse(const std::string& aformula, int&  nsites)
{
    formula = aformula;
    std::string parsed_string = formula;
    std::vector<MoietyTerm> terms;
    size_t endmoi;
    std::string moiName;
    double nj;   // moiety-site occupancy.

    nsites = 0;
    while(  !parsed_string.empty() )  // list of {<elem>}< elem_st_coef>
    {
        switch( parsed_string[0]) {
        case PSUR_L_PLUS:  nsites++;
            parsed_string = parsed_string.substr(1);
            continue;
        case LBRACKET3: {
            parsed_string = parsed_string.substr(1);
            funErrorIf(parsed_string.empty(), "scanMoiety","Must be }", __LINE__, __FILE__ );
            endmoi =  parsed_string.find_first_of( RBRACKET3 );
            funErrorIf(endmoi == std::string::npos, "scanMoiety","Must be }", __LINE__, __FILE__ );
            moiName = std::string( parsed_string, 0, endmoi );
            parsed_string = parsed_string.substr(endmoi+1);
            nj = 1.;
            get_real(nj, parsed_string);
            terms.push_back(MoietyTerm{moiName, nsites, nj});
            break;
        }
        case 'V':
            if( parsed_string[1] == 'a' ) {
                // Va vacancy
                parsed_string = parsed_string.substr(2); //Va
                nj = 1.;
                get_real( nj, parsed_string );
                terms.push_back(MoietyTerm{"Va", nsites, nj});
                break;
            }
            [[fallthrough]];
        default:
            // else other symbol
            parsed_string = parsed_string.substr(1);
            break;
        }
    }
    return terms;
}

}
