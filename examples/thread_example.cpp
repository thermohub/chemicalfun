#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <thread>
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

// cmake -DCMAKE_BUILD_TYPE=Debug -fsanitize=thread ..

int main(int argc, char* argv[])
{
    string test_formula = "Al(OH)2+";

    if( argc > 1) {
        test_formula = argv[1];
    }

    try{

        auto ar_logger = spdlog::get("chemicalfun");
        ar_logger->set_level(spdlog::level::trace);

        ChemicalFun::DBElements all_elements;
        //auto json_string = read_ascii_file("elementsDB.json");
        //all_elements.readElements(json_string);
        //std::cout << all_elements.writeElements(true) <<  std::endl;

        std::thread th1(parse_formula,"HOO|0|-",all_elements);
        std::thread th2(parse_formula,"(SiO2)1(CaO)0.833333(H2O)1.333333",all_elements);
        std::thread th3(parse_formula,"NaSi10.473Al4.132Mg.737Fe|3|.237Fe|2|.211O44.316H30.737",all_elements);

        if( !test_formula.empty() ) {
            parse_formula(test_formula, all_elements);
        }

        th1.join();
        th2.join();
        th3.join();
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

