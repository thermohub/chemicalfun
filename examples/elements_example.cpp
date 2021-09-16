#include <fstream>
#include <sstream>
#include "ChemicalFun/FormulaParser.h"
#include "Common/Exception.h"
using namespace std;


// Read whole ASCII file into string.
std::string read_ascii_file(const std::string& file_path)
{
    std::ifstream t(file_path);
    ChemicalFun::funErrorIf(!t.good(), "file open error...  ", file_path, __LINE__, __FILE__);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
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
        std::cout << ChemicalFun::ChemicalFormula::writeDBElements() <<  std::endl;

        if( !test_formula.empty() ) {
            //test_parse(test_formula);
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

