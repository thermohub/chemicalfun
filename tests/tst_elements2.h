#ifndef TST_ELEMENTS2_H
#define TST_ELEMENTS2_H

#pragma once

#include <sstream>
#include <gtest/gtest.h>
#include "ChemicalFun/FormulaParser.h"

using namespace testing;
using namespace ChemicalFun;

const char* const dbElements_str2 = R"([
    {      "element" :   {
               "symbol" :   "C",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   12.0108003616333,
               "entropy" :   5.73999977111816,
               "heat_capacity" :   8.68200016021729,
               "volume" :   4.58440017700195,
               "valence" :   4,
               "number" :   6,
               "name" :   "C"
          }
     },
     {
          "element" :   {
               "symbol" :   "Ca",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   40.0779991149902,
               "entropy" :   41.5900001525879,
               "heat_capacity" :   25.9290008544922,
               "volume" :   25.8600006103516,
               "valence" :   2,
               "number" :   20,
               "name" :   "Ca"
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
               "symbol" :   "Mn",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   54.9379997253418,
               "entropy" :   32.0099983215332,
               "heat_capacity" :   26.3199996948242,
               "volume" :   7.39400005340576,
               "valence" :   2,
               "number" :   25,
               "name" :   "Mn"
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
               "symbol" :   "S",
               "class_" :   0,
               "isotope" :   0
          },
          "properties" :   {
               "recid" :   "",
               "atomic_mass" :   32.0670013427734,
               "entropy" :   31.7980003356934,
               "heat_capacity" :   22.7630004882812,
               "volume" :   15.5100002288818,
               "valence" :   6,
               "number" :   16,
               "name" :   "S"
          }
     }
])";

TEST(ChemicalFormula, ChargeFromFormula)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str2);
    EXPECT_EQ(all_elements.elements().size(), 8);
    EXPECT_EQ(all_elements.elementsKeys().size(), 8);

    ChemicalFun::FormulaToken token("CaC2");
    EXPECT_EQ(token.charge(), 10.0);  // from valences

    auto properties = token.properties(all_elements.elements(), true); // charge from formula
    EXPECT_NEAR(properties.atomic_mass, 64.09959983825681, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 3.0, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 53.06999969482422, 1e-30);
    EXPECT_EQ(properties.formula, "CaC2");

    ChemicalFun::FormulaToken token2("Na2S");
    EXPECT_EQ(token2.charge(), 8.0);  // from valences

    properties = token2.properties(all_elements.elements(), true); // charge from formula
    EXPECT_NEAR(properties.atomic_mass, 78.0466003417968, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 3.0, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 134.3979988098144, 1e-30);
    EXPECT_EQ(properties.formula, "Na2S");

    ChemicalFun::FormulaToken token3("Mn2O3");
    EXPECT_EQ(token3.charge(), -2.0);  // from valences

    properties = token3.properties(all_elements.elements(), true); // charge from formula
    EXPECT_NEAR(properties.atomic_mass, 157.8741998672486, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 5.0, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 371.7269973754894, 1e-30);
    EXPECT_EQ(properties.formula, "Mn2O3");
}

TEST(ChemicalFormula, ChargeImbalance)
{
    ChemicalFun::DBElements all_elements;
    ChemicalFun::set_charge_from_formula(false);

    ChemicalFun::FormulaToken token("CaC2");
    EXPECT_EQ(token.charge(), 10.0);  // from valences
    EXPECT_EQ(token.charge({}, true), 0.0);  // from formula
    EXPECT_THROW(token.testChargeImbalance(), std::exception);
    EXPECT_NO_THROW(token.testChargeImbalance(all_elements.elements(), true));
    EXPECT_EQ(token.testChargeImbalance(all_elements.elements(), true), true);

    token.setFormula("HOO|0|-");
    EXPECT_DOUBLE_EQ(token.charge(), -1);
    EXPECT_NO_THROW(token.testChargeImbalance());
    EXPECT_NO_THROW(token.testChargeImbalance(all_elements.elements(), true));
    EXPECT_EQ(token.testChargeImbalance(all_elements.elements(), true), false);
}

TEST(ChemicalFormula, ChargeDefaultFormula)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str2);
    EXPECT_EQ(all_elements.elements().size(), 8);
    EXPECT_EQ(all_elements.elementsKeys().size(), 8);
    ChemicalFun::set_charge_from_formula(true);

    ChemicalFun::FormulaToken token("CaC2");
    EXPECT_EQ(token.charge(), 0.0);

    auto properties = token.properties(all_elements.elements()); // charge from formula
    EXPECT_NEAR(properties.atomic_mass, 64.09959983825681, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 3.0, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 53.06999969482422, 1e-30);
    EXPECT_EQ(properties.formula, "CaC2");

    std::vector<std::string> formulalist = { "CaC2", "Na2S", "Na2SO3", "Mn2O3", "MnO2", "Mn2O7" };
    std::stringstream str;
    all_elements.formulasPropertiesCSV(str, formulalist);
    EXPECT_EQ(str.str(),
"formula,charge,atomic_mass,elemental_entropy,atoms_formula\n"
"CaC2,0,64.0996,53.07,3\n"
"Na2S,0,78.0466,134.398,3\n"
"Na2SO3,0,126.045,442.105,6\n"
"Mn2O3,0,157.874,371.727,5\n"
"MnO2,0,86.9368,237.148,3\n"
"Mn2O7,0,221.872,782.003,9\n"
                  );
}


#endif // TST_ELEMENTS2_H
