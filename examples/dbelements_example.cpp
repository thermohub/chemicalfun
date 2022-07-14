#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "ChemicalFun/FormulaParser.h"
#include "Common/Exception.h"
using namespace std;

const char* const dbElements_str = R"([
      {
          "element" :   {
               "symbol" :   "Al",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   26.9815406799316,
               "entropy" :   28.2999992370605,
               "heat_capacity" :   24.2000007629395,
               "volume" :   9.99300003051758,
               "valence" :   3,
               "number" :   13,
               "name" :   "Al"
          }
     },
     {
          "element" :   {
               "symbol" :   "Cl",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   35.4529991149902,
               "entropy" :   111.540496826172,
               "heat_capacity" :   16.9745006561279,
               "volume" :   12395,
               "valence" :   -1,
               "number" :   17,
               "name" :   "Cl"
          }
     },
     {
          "element" :   {
               "symbol" :   "Fe",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   55.8450012207031,
               "entropy" :   27.2800006866455,
               "heat_capacity" :   24.9610004425049,
               "volume" :   7.09200000762939,
               "valence" :   2,
               "number" :   26,
               "name" :   "Fe"
          }
     },
     {
          "element" :   {
               "symbol" :   "H",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   1.00794994831085,
               "entropy" :   65.3399963378906,
               "heat_capacity" :   14.4090003967285,
               "volume" :   12395,
               "valence" :   1,
               "number" :   1,
               "name" :   "H"
          }
     },
     {
          "element" :   {
               "symbol" :   "K",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   39.0983009338379,
               "entropy" :   64.6799926757812,
               "heat_capacity" :   29.6000003814697,
               "volume" :   45.4630012512207,
               "valence" :   1,
               "number" :   19,
               "name" :   "K"
          }
     },
     {
          "element" :   {
               "symbol" :   "Mg",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   24.3050003051758,
               "entropy" :   32.6699981689453,
               "heat_capacity" :   24.8689994812012,
               "volume" :   13.996000289917,
               "valence" :   2,
               "number" :   12,
               "name" :   "Mg"
          }
     },
     {
          "element" :   {
               "symbol" :   "Na",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   22.9897994995117,
               "entropy" :   51.2999992370605,
               "heat_capacity" :   28.2299995422363,
               "volume" :   23.7999992370605,
               "valence" :   1,
               "number" :   11,
               "name" :   "Na"
          }
     },
     {
          "element" :   {
               "symbol" :   "O",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   15.999400138855,
               "entropy" :   102.569000244141,
               "heat_capacity" :   14.66100025177,
               "volume" :   12395,
               "valence" :   -2,
               "number" :   8,
               "name" :   "O"
          }
     },
     {
          "element" :   {
               "symbol" :   "Si",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   28.0855007171631,
               "entropy" :   18.8099994659424,
               "heat_capacity" :   19.7889995574951,
               "volume" :   12.0559997558594,
               "valence" :   4,
               "number" :   14,
               "name" :   "Si"
          }
     },
     {
          "element" :   {
               "symbol" :   "Zz",
               "class_" :   4,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   0,
               "entropy" :   -65.3399963378906,
               "heat_capacity" :   -14.4180002212524,
               "volume" :   0,
               "valence" :   0,
               "number" :   0,
               "name" :   "Zz"
          }
     }
])";

// Read whole ASCII file into string.
static std::string read_ascii_file(const std::string& file_path)
{
    std::ifstream t(file_path);
    ChemicalFun::funErrorIf(!t.good(), "file open error...  ", file_path, __LINE__, __FILE__);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[])
{
    string test_file;

    if( argc > 1) {
        test_file = argv[1];
    }

    try{
        auto ar_logger = spdlog::get("chemicalfun");
        ar_logger->set_level(spdlog::level::trace);

        ChemicalFun::update_loggers( false, "test.log", 0);

        ChemicalFun::DBElements all_elements;
        std::string str_json_array = dbElements_str;
        if( !test_file.empty() ) {
            str_json_array = read_ascii_file("elementsDB.json");
        }
        all_elements.readElements(str_json_array);

        std::cout << "All elements " <<  std::endl;
        std::cout << all_elements.writeElements(true) <<  std::endl;

        std::cout << "---------\n\nAll elements to csv" <<  std::endl;
        all_elements.printCSV(std::cout);

        std::vector<std::string> formulalist{
            "Al+3",
            "Al(OH)2+",
            "Al(OH)3@",
            "HSiO3-",
            "H+",
            "O|0|2@",
            "O|0|2",
            "HOO|0|-",
            "H2OO|0|@",
            "Al2SiO4(OH)2",
            "FeFe|3|2O4",
            "Mg48Si34O85(OH)62",
            "{Va}:{Al}2:{Si}2:{Si}2:O10(OH)2",
            "Mg5Al(AlSi3)O10(OH)8",
        };
        std::cout << "---------\n\nTest Formula list" <<  std::endl;
        for(const auto& aformula: formulalist) {
           std::cout << aformula <<  std::endl;
        }

        auto used_elements = all_elements.formulasElements(formulalist);
        std::cout << "---------\n\nAll elements from list" <<  std::endl;
        for(const auto& elm: used_elements) {
           std::cout << elm.Symbol() <<  ", ";
        }

        std::cout << "\n---------\n\nStoichiometryMatrix to csv" <<  std::endl;
        all_elements.printStoichiometryMatrix(std::cout, formulalist);

        std::cout << "---------\n\nProperties to csv" <<  std::endl;
        all_elements.formulasPropertiesCSV(std::cout, formulalist);

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

