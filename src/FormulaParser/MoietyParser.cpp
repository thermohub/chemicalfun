// ChemicalFun is a C++ and Python library 
// for Chemical Formula Parser and Reactions Generator.
//
// Copyright (C) 2018-2022 G.D.Miron, D.Kulik, S.Dmytriieva
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <sstream>
#include <iomanip>
#include "ChemicalFun/FormulaParser/MoietyParser.h"
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

MoietyParser::~MoietyParser()
{}

// <fterm>  ::= <site_term> : | <fterm> <site_term>:
// <site_term> ::= <moiety>   | <moiety><moiety>
// <moiety>    ::= {<elem>}   | {<elem>} <elem_st_coef> | Va
std::vector<MoietyTerm> MoietyParser::parse(const std::string& aformula, int&  nsites)
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
            [[gnu::fallthrough]];
        default:
            // else other symbol
            parsed_string = parsed_string.substr(1);
            break;
        }
    }

    if( chfun_logger->should_log(spdlog::level::trace)) {
        std::ostringstream logs;
        std::copy(terms.begin(), terms.end(), std::ostream_iterator<MoietyTerm>( logs, "\n "));
        chfun_logger->trace(" {} \n {} ", formula, logs.str());
    }
    return terms;
}

}
