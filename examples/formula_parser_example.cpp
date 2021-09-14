#include <iostream>
#include <vector>
#include <string>
#include "ChemicalFun/FormulaParser.h"
using namespace std;

std::vector<std::string> testFormulas = {
    "Al+3",
    "Al(OH)2+",
    "Al(OH)3@",
    "HSiO3-",
    "HCl|3|O2@",
    "H+",
    "O|0|2@",
    "O|0|2",
    "HOO|0|-",
    "H2OO|0|@",
    "NaAlSi3O8",
    "KAl3Si3O10(OH)2",
    "Al2SiO4(OH)2",
    "FeFe|3|2O4",
    "Fe|2|4Fe|3|2(OH)12SO4",
    "Mg48Si34O85(OH)62",
    "{Va}:{Al}2:{Si}2:{Si}2:O10(OH)2",
    "Na(Ca2Fe5)(AlSi7)O22(OH)2",
    "Mg5Al(AlSi3)O10(OH)8",
    "MnS|-2|",
    "FeS|0|S|-2|",
    "(CaSiO3)2(CaSO4)2(CaCO3)2(H2O)30",
    "Na0.99Al0.99Si2.01O6H2O",
    "K(K3.2Mg1.7Al16Si24.3O69.5(OH)13.8)",
    "(SiO2)1(CaO)0.833333(H2O)1.333333",
    "NaSi10.473Al4.132Mg.737Fe|3|.237Fe|2|.211O44.316H30.737",
    "/3/H2/18/O"
};

void test_parse( const std::string& chemical_formula ) {

    std::cout << "\nTested: " << chemical_formula;
    ChemicalFun::ChemicalFormulaParser formparser;
    auto icterms = formparser.parse( chemical_formula );
    for( const auto& term: icterms) {
       std::cout << "\n   " << term;
    }
}

int main(int argc, char* argv[])
{
    string test_formula = "";

    if( argc > 1) {
        test_formula = argv[1];
    }

    try{
        if( test_formula.empty() ) {
            for( const auto& formula: testFormulas ) {
                test_parse(formula);
            }
        }
        else {
            test_parse(test_formula);
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

