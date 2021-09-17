#include <fstream>
#include <sstream>
#include <iomanip>
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

static void parse_formula(const std::string& formula)
{
    std::cout << "Tested: " << formula;
    ChemicalFun::FormulaToken token(formula);

    std::cout << "\nParsed";
    for( const auto& term: token.parsed_list()) {
       std::cout << "\n   " << term;
    }
    std::cout << "\nCarge1 " << token.charge();
    token.calculateCharge();
    std::cout << "\nCarge2 " << token.charge();

    ChemicalFun::FormulaProperites properties;
    token.calcFormulaProperites(properties);
    std::cout << "\nProperites\n" << properties.to_json_string(true);

    auto st_row = token.makeStoichiometryRowOld(ChemicalFun::ChemicalFormula::elementsRow());
    std::cout << "\nStoichiometryRow:\n";
    for( const auto& term: st_row ) {
       std::cout << std::setprecision(15) <<  term << " ";
    }

    std::cout << "\nTest elements\n" << token.testElements(formula);
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    string test_formula = "Al(OH)2+";

    if( argc > 1) {
        test_formula = argv[1];
    }

    try{
        auto json_string = read_ascii_file("elementsDB.json");
        ChemicalFun::ChemicalFormula::readDBElements(json_string);
        std::cout << ChemicalFun::ChemicalFormula::writeDBElements(true) <<  std::endl;

        if( !test_formula.empty() ) {
            parse_formula(test_formula);
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

