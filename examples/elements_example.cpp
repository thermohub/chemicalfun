#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ChemicalFun/FormulaParser.h"
#include "Common/Exception.h"
using namespace std;


// Read whole ASCII file into string.
static std::string read_ascii_file(const std::string& file_path)
{
    std::ifstream t(file_path);
    ChemicalFun::funErrorIf(!t.good(), "file open error...  ", file_path, __LINE__, __FILE__);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

static void parse_formula(const std::string& formula, const ChemicalFun::DBElements& all_elements)
{
    std::cout << "Tested: " << formula;
    ChemicalFun::FormulaToken token(formula);

    std::cout << "\nParsed";
    for( const auto& term: token.parsed_list()) {
       std::cout << "\n   " << term;
    }
    std::cout << "\nCharge " << token.charge();

    ChemicalFun::FormulaProperties properties = token.properties(all_elements.elements());
    std::cout << "\nProperties\n" << properties.to_json_string(true);

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::cout << "\nStoichiometryRow:\n";
    for( const auto& term: st_row ) {
       std::cout << std::setprecision(15) <<  term << " ";
    }

    std::cout << "\nTest elements\n" << token.testElements(formula, all_elements.elementsKeys());
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    string test_formula = "Al(OH)2+";

    if( argc > 1) {
        test_formula = argv[1];
    }

    try{
        ChemicalFun::DBElements all_elements;
        std::cout << "<<<" << all_elements.writeElements(false) <<  ">>>" << std::endl;

        if( !test_formula.empty() ) {
            parse_formula(test_formula, all_elements);
        }
        return 0;
    }
    catch( std::exception& e )
    {
        std::cout << "std::exception: " << e.what() <<  std::endl;
    }
    catch(...)
    {
        std::cout <<  "unknown exception" <<  std::endl;
    }

    return 1;
}

