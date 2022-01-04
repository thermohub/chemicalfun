#pragma once

#include <gtest/gtest.h>
#include "ChemicalFun/FormulaParser.h"

using namespace testing;
using namespace ChemicalFun;

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
    },
    {
        "element": {
            "class_": 5,
            "symbol": "Vol"
        },
        "properties": {
            "atomic_mass": 0.0,
            "entropy": 0.0,
            "heat_capacity": 0.0,
            "name": "Vol",
            "number": 100,
            "recid": "",
            "valence": 0,
            "volume": 1.0
        }
    },
    {
        "element": {
            "symbol": "Am"
        },
        "properties": {
            "atomic_mass": 243.061,
            "entropy": 55.4,
            "heat_capacity": 55.4,
            "name": "Am",
            "number": 95,
            "recid": "elements/Am;0:ELEMENT;26:PSI_NAGRA20",
            "valence": 3,
            "volume": 243.061
        }
    },
    {
        "element": {
            "class_": 2,
            "symbol": "Oxa"
        },
        "properties": {
            "atomic_mass": 88.019,
            "entropy": 0.0,
            "heat_capacity": 0.0,
            "name": "Oxa",
            "number": 101,
            "recid": "elements/Oxa;0:ELEMENT;26:PSI_NAGRA20",
            "valence": 0,
            "volume": 88.019
        }
    },
    {
        "element": {
            "class_": 2,
            "symbol": "Edta"
        },
        "properties": {
            "atomic_mass": 288.212,
            "entropy": 0.0,
            "heat_capacity": 0.0,
            "name": "Edta",
            "number": 103,
            "recid": "elements/Edta;0:ELEMENT;26:PSI_NAGRA20",
            "valence": -4,
            "volume": 288.212
        }
    }
])";

const std::string Zz_Record = R"({
     "symbol" :   "Zz",
     "isotope_mass" :   0,
     "name" :   "electric_charge",
     "atomic_mass" :   {
          "values" :   [
               0
          ],
          "name" :   "M0i"
     },
     "class_" :   {
          "4" :   "CHARGE"
     },
     "aggregate_state" :   {
          "6" :   "AS_IONEX"
     },
     "isotope_abundance" :   {
          "values" :   [
               0
          ],
          "name" :   "IXi"
     },
     "entropy" :   {
          "values" :   [
               -65.3399963378906
          ],
          "name" :   "S0i"
     },
     "volume" :   {
          "values" :   [
               0
          ],
          "name" :   "V0i"
     },
     "heat_capacity" :   {
          "values" :   [
               -14.4180002212524
          ],
          "name" :   "Cp0i"
     },
     "number" :   0,
     "sourcetdb" :   {
          "2" :   "SUPCRT"
     },
     "formula" :   "Zz",
     "substance" :   "`",
     "comment" :   "Electrical (proton) charge",
"datasources" :   [
    "Supcrt-Slop98"
    ],
    "radioactivity" :   {
    "decay_type" :   {
        "0" :   "STABLE"
    },
    "half_life_lambda" :   {
        "values" :   [
        0
        ]
    }
},
"time_stamp" :   {
"year" :   2018,
"month" :   12,
"day" :   20,
"hour" :   15,
"minute" :   24,
"second" :   43
}
})";


TEST(ChemicalFormula, ElementKey)
{
//    ChemicalFun::DBElements all_elements;
//    all_elements.readElements(dbElements_str);
//    EXPECT_EQ(all_elements.elements().size(), 14);
//    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ElementKey Zz_el("Zz", 4, 0);
    EXPECT_EQ(Zz_el.Symbol(), "Zz");
    EXPECT_EQ(Zz_el.Class(), 4);
    EXPECT_EQ(Zz_el.Isotope(), 0);

    Zz_el.from_json_string(R"({"symbol" :"Zz","class_":4})");
    EXPECT_EQ(Zz_el.Symbol(), "Zz");
    EXPECT_EQ(Zz_el.Class(), 4);
    EXPECT_EQ(Zz_el.Isotope(), 0);

    Zz_el.from_json_string(Zz_Record);
    EXPECT_EQ(Zz_el.Symbol(), "Zz");
    EXPECT_EQ(Zz_el.Class(), 4);
    EXPECT_EQ(Zz_el.Isotope(), 0);

    ElementKey Zz_el2(Zz_Record);
    EXPECT_EQ(Zz_el2.Symbol(), "Zz");
    EXPECT_EQ(Zz_el2.Class(), 4);
    EXPECT_EQ(Zz_el2.Isotope(), 0);
    EXPECT_TRUE(Zz_el == Zz_el2);

    ElementKey Al_el("Al", 0, 0);
    EXPECT_EQ(Al_el.Symbol(), "Al");
    EXPECT_EQ(Al_el.Class(), 0);
    EXPECT_EQ(Al_el.Isotope(), 0);

    Zz_el2.from_json_string(R"({"symbol":"Al"})");
    EXPECT_TRUE(Al_el == Zz_el2);
    EXPECT_EQ(Al_el.to_json_string(),R"({"symbol":"Al"})");
}

TEST(ChemicalFormula, FormulaTokenAlOH)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("Al(OH)2+");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 4);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"Al"},"stoich_coef":1.0,"valence":3})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"H"},"stoich_coef":2.0,"valence":1})");
    EXPECT_EQ(els_list[2], R"({"key":{"symbol":"O"},"stoich_coef":2.0,"valence":-2})");
    EXPECT_EQ(els_list[3], R"({"key":{"class_":4,"symbol":"Zz"},"stoich_coef":1.0,"valence":0})");
    EXPECT_EQ(token.charge(), 1);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 60.996240854263306, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 6, 1e-30);
    EXPECT_NEAR(properties.charge, 1, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 298.7779960632331, 1e-30);
    EXPECT_EQ(properties.formula, "Al(OH)2+");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 1, 0, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 1 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("Al(OH)2+", all_elements.elementsKeys()).empty()) ;
}

TEST(ChemicalFormula, FormulaTokenH)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("H+");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 2);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"H"},"stoich_coef":1.0,"valence":1})");
    EXPECT_EQ(els_list[1], R"({"key":{"class_":4,"symbol":"Zz"},"stoich_coef":1.0,"valence":0})");
    EXPECT_EQ(token.charge(), 1);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 1.0079499483108501, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 2, 1e-30);
    EXPECT_NEAR(properties.charge, 1, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 0, 1e-30);
    EXPECT_EQ(properties.formula, "H+");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("H+", all_elements.elementsKeys()).empty()) ;
}

TEST(ChemicalFormula, FormulaTokenO2)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("O|0|2@");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 2);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"O"},"stoich_coef":2.0,"valence":0})");
    EXPECT_EQ(els_list[1], R"({"key":{"class_":4,"symbol":"Zz"},"stoich_coef":0.0,"valence":0})");
    EXPECT_EQ(token.charge(), 0);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 31.99880027771, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 2, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 205.138000488282, 1e-30);
    EXPECT_EQ(properties.formula, "O|0|2@");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("O|0|2@", all_elements.elementsKeys()).empty()) ;

    token.setFormula("O|0|2");
    els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 1);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"O"},"stoich_coef":2.0,"valence":0})");
    EXPECT_EQ(token.charge(), 0);

    properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 31.99880027771, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 2, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 205.138000488282, 1e-30);
    EXPECT_EQ(properties.formula, "O|0|2");

    st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    row = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("O|0|2", all_elements.elementsKeys()).empty()) ;
}

TEST(ChemicalFormula, FormulaTokenHOO)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("HOO|0|-");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 4);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"H"},"stoich_coef":1.0,"valence":1})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"O"},"stoich_coef":1.0,"valence":-2})");
    EXPECT_EQ(els_list[2], R"({"key":{"symbol":"O"},"stoich_coef":1.0,"valence":0})");
    EXPECT_EQ(els_list[3], R"({"key":{"class_":4,"symbol":"Zz"},"stoich_coef":-1.0,"valence":0})");
    EXPECT_EQ(token.charge(), -1);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 33.00675022602085, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 2, 1e-30);
    EXPECT_NEAR(properties.charge, -1, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 335.8179931640632, 1e-30);
    EXPECT_EQ(properties.formula, "HOO|0|-");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 0, 0, 0, -1 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("HOO|0|-", all_elements.elementsKeys()).empty()) ;
}

TEST(ChemicalFormula, FormulaTokenNumber)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("(SiO2)1(CaO)0.833333(H2O)1.333333");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 4);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"Ca"},"stoich_coef":0.833333,"valence":2})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"H"},"stoich_coef":2.666666,"valence":1})");
    EXPECT_EQ(els_list[2], R"({"key":{"symbol":"O"},"stoich_coef":4.166666,"valence":-2})");
    EXPECT_EQ(els_list[3], R"({"key":{"symbol":"Si"},"stoich_coef":1.0,"valence":4})");
    EXPECT_DOUBLE_EQ(token.charge(), 0);

    EXPECT_THROW(token.properties(all_elements.elements()), std::exception);

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 0, 2.666666, 0, 0, 0, 4.166666, 0, 1, 0, 0 };
    ASSERT_EQ(st_row.size(), row.size());
    for (size_t i = 0; i < row.size(); ++i) {
        EXPECT_DOUBLE_EQ(st_row[i], row[i]);
    }

    EXPECT_EQ(token.testElements("(SiO2)1(CaO)0.833333(H2O)1.333333", all_elements.elementsKeys()), "Ca;");
}

TEST(ChemicalFormula, FormulaTokenComplex)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("NaSi10.473Al4.132Mg.737Fe|3|.237Fe|2|.211O44.316H30.737");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 8);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"Al"},"stoich_coef":4.132,"valence":3})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"Fe"},"stoich_coef":0.211,"valence":2})");
    EXPECT_EQ(els_list[2], R"({"key":{"symbol":"Fe"},"stoich_coef":0.237,"valence":3})");
    EXPECT_EQ(els_list[3], R"({"key":{"symbol":"H"},"stoich_coef":30.737,"valence":1})");
    EXPECT_EQ(els_list[4], R"({"key":{"symbol":"Mg"},"stoich_coef":0.737,"valence":2})");
    EXPECT_EQ(els_list[5], R"({"key":{"symbol":"Na"},"stoich_coef":1.0,"valence":1})");
    EXPECT_EQ(els_list[6], R"({"key":{"symbol":"O"},"stoich_coef":44.316,"valence":-2})");
    EXPECT_EQ(els_list[7], R"({"key":{"symbol":"Si"},"stoich_coef":10.473,"valence":4})");
    EXPECT_NEAR(token.charge(), -7.105427357601002e-15, 1e-30);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 1211.5590944863566, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 91.843, 1e-30);
    EXPECT_NEAR(properties.charge, -7.105427357601002e-15, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 6955.335231706635, 1e-30);
    EXPECT_EQ(properties.formula, "NaSi10.473Al4.132Mg.737Fe|3|.237Fe|2|.211O44.316H30.737");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 4.132, 0, 0, 0, 0.448, 30.737, 0, 0.737, 1, 44.316, 0, 10.473, 0, 0 };
    ASSERT_EQ(st_row.size(), row.size());
    for (size_t i = 0; i < row.size(); ++i) {
        EXPECT_DOUBLE_EQ(st_row[i], row[i]);
    }

    EXPECT_TRUE(token.testElements("NaSi10.473Al4.132Mg.737Fe|3|.237Fe|2|.211O44.316H30.737", all_elements.elementsKeys()).empty()) ;
}

TEST(ChemicalFormula, FormulaTokenIsotope)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("/3/H2/18/O");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 2);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"H"},"stoich_coef":2.0,"valence":1})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"O"},"stoich_coef":1.0,"valence":-2})");
    EXPECT_EQ(token.charge(), 0);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 18.0153000354767, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 3, 1e-30);
    EXPECT_NEAR(properties.charge, 0, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 233.2489929199222, 1e-30);
    EXPECT_EQ(properties.formula, "/3/H2/18/O");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 0, 2, 0, 0, 0, 1, 0, 0, 0, 0 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("/3/H2/18/O", all_elements.elementsKeys()).empty()) ;
}

TEST(ChemicalFormula, FormulaTokenNoMnS)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("MnS|-2|");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 2);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"Mn"},"stoich_coef":1.0,"valence":2})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"S"},"stoich_coef":1.0,"valence":-2})");
    EXPECT_DOUBLE_EQ(token.charge(), 0);

    EXPECT_THROW(token.properties(all_elements.elements()), std::exception);

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    ASSERT_EQ(st_row, row);

    EXPECT_EQ(token.testElements("MnS|-2|", all_elements.elementsKeys()), "Mn;S;");
}

TEST(ChemicalFormula, FormulaTokenNoFeSS)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("FeS|0|S|-2|");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 3);
    EXPECT_EQ(els_list[0], R"({"key":{"symbol":"Fe"},"stoich_coef":1.0,"valence":2})");
    EXPECT_EQ(els_list[1], R"({"key":{"symbol":"S"},"stoich_coef":1.0,"valence":-2})");
    EXPECT_EQ(els_list[2], R"({"key":{"symbol":"S"},"stoich_coef":1.0,"valence":0})");
    EXPECT_DOUBLE_EQ(token.charge(), 0);

    EXPECT_THROW(token.properties(all_elements.elements()), std::exception);

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    ASSERT_EQ(st_row, row);

    EXPECT_EQ(token.testElements("FeS|0|S|-2|", all_elements.elementsKeys()), "S;S;");
}

TEST(ChemicalFormula, FormulaTokenChargeImbalance)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("FeS|0|S|-2|");
    EXPECT_DOUBLE_EQ(token.charge(), 0);
    EXPECT_NO_THROW(token.testChargeImbalance());

    token.setFormula("Al(OH)2+");
    EXPECT_DOUBLE_EQ(token.charge(), 1);
    EXPECT_NO_THROW(token.testChargeImbalance());

    token.setFormula("Al+3");
    EXPECT_DOUBLE_EQ(token.charge(), 3);
    EXPECT_NO_THROW(token.testChargeImbalance());

    token.setFormula("HOO|0|-");
    EXPECT_DOUBLE_EQ(token.charge(), -1);
    EXPECT_NO_THROW(token.testChargeImbalance());

    token.setFormula("HSiO3-");
    EXPECT_DOUBLE_EQ(token.charge(), -1);
    EXPECT_NO_THROW(token.testChargeImbalance());

    token.setFormula("HSiO3-2");
    EXPECT_DOUBLE_EQ(token.charge(), -1);
    EXPECT_THROW(token.testChargeImbalance(), std::exception);
}

//-----------------------------------------------
TEST(ChemicalFormula, FormulaTokenAlOH_WithValences)
{
    ChemicalFun::FormulaToken token("Al(OH)2+", true);
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 4);
    EXPECT_EQ(els_list[0], "{\"key\":{\"symbol\":\"Al(3)\"},\"stoich_coef\":1.0,\"valence\":3}");
    EXPECT_EQ(els_list[1], "{\"key\":{\"symbol\":\"H(1)\"},\"stoich_coef\":2.0,\"valence\":1}");
    EXPECT_EQ(els_list[2], "{\"key\":{\"symbol\":\"O(-2)\"},\"stoich_coef\":2.0,\"valence\":-2}");
    EXPECT_EQ(els_list[3],  "{\"key\":{\"class_\":4,\"symbol\":\"Zz(0)\"},\"stoich_coef\":1.0,\"valence\":0}");
    EXPECT_EQ(token.charge(), 1);

    std::vector<ElementKey> all_elments(token.getElementsList().begin(), token.getElementsList().end());
    auto st_row = token.makeStoichiometryRow(all_elments);
    std::vector<double> row = { 1, 2, 2, 1 };
    EXPECT_EQ(st_row, row);
    EXPECT_EQ( to_string(all_elments), "Al(3);H(1);O(-2);Zz(0);");
}

TEST(ChemicalFormula, FormulaTokenHOO_WithValences)
{
    ChemicalFun::FormulaToken token("HOO|0|-", true);
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 4);
    EXPECT_EQ(els_list[0], "{\"key\":{\"symbol\":\"H(1)\"},\"stoich_coef\":1.0,\"valence\":1}");
    EXPECT_EQ(els_list[1], "{\"key\":{\"symbol\":\"O(-2)\"},\"stoich_coef\":1.0,\"valence\":-2}");
    EXPECT_EQ(els_list[2], "{\"key\":{\"symbol\":\"O(0)\"},\"stoich_coef\":1.0,\"valence\":0}");
    EXPECT_EQ(els_list[3], "{\"key\":{\"class_\":4,\"symbol\":\"Zz(0)\"},\"stoich_coef\":-1.0,\"valence\":0}");
    EXPECT_EQ(token.charge(), -1);

    std::vector<ElementKey> all_elments(token.getElementsList().begin(), token.getElementsList().end());
    auto st_row = token.makeStoichiometryRow(all_elments);
    std::vector<double> row = { 1, 1, 1, -1 };
    EXPECT_EQ(st_row, row);
    EXPECT_EQ( to_string(all_elments), "H(1);O(-2);O(0);Zz(0);");
}

TEST(ChemicalFormula, FormulaTokenNoFeSS_WithValences)
{
    ChemicalFun::FormulaToken token("FeS|0|S|-2|", true);
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 3);
    EXPECT_EQ(els_list[0], "{\"key\":{\"symbol\":\"Fe(2)\"},\"stoich_coef\":1.0,\"valence\":2}");
    EXPECT_EQ(els_list[1], "{\"key\":{\"symbol\":\"S(-2)\"},\"stoich_coef\":1.0,\"valence\":-2}");
    EXPECT_EQ(els_list[2], "{\"key\":{\"symbol\":\"S(0)\"},\"stoich_coef\":1.0,\"valence\":0}");
    EXPECT_DOUBLE_EQ(token.charge(), 0);

    std::vector<ElementKey> all_elments(token.getElementsList().begin(), token.getElementsList().end());
    auto st_row = token.makeStoichiometryRow(all_elments);
    std::vector<double> row = { 1, 1, 1 };
    EXPECT_EQ(st_row, row);
    EXPECT_EQ( to_string(all_elments), "Fe(2);S(-2);S(0);");
}


TEST(ChemicalFormula, StoichiometryMatrixValences)
{
    std::vector<std::string> formula_list{
        "H2O",
        "H2O@",
        "H+",
        "OH-",
        "H|0|2",
        "O|0|2",
        "HOO|0|-",
    };

    std::vector<ElementKey> all_elments = elementsInFormulas(formula_list);
    auto st_matr = substancesStoichiometryMatrix(formula_list/*, all_elments*/);
    EXPECT_EQ( to_string(all_elments), "H;O;Zz;");
    StoichiometryMatrixData matrix1 = { { 2, 1, 0 },
                                       { 2, 1, 0 },
                                       { 1, 0, 1 },
                                       { 1, 1, -1 },
                                       { 2, 0, 0 },
                                       { 0, 2, 0 },
                                       { 1, 2, -1 } };
    EXPECT_EQ(st_matr, matrix1);

    all_elments = elementsInFormulas(formula_list, true);
    st_matr = substancesStoichiometryMatrix(formula_list/*, all_elments*/, true);
    EXPECT_EQ( to_string(all_elments), "H(0);H(1);O(-2);O(0);Zz(0);");
    StoichiometryMatrixData matrix2 = { { 0, 2, 1, 0, 0 },
                                        { 0, 2, 1, 0, 0 },
                                        { 0, 1, 0, 0, 1 },
                                        { 0, 1, 1, 0, -1 },
                                        { 2, 0, 0, 0, 0 },
                                        { 0, 0, 0, 2, 0 },
                                        { 0, 1, 1, 1, -1 } };
    EXPECT_EQ(st_matr, matrix2);
}

// "Am|3|(Oxa)3-3", "Am|3|(Oxa)+", "Am|3|(HEdta)@", "Am|3|(Edta)OH-2"


TEST(ChemicalFormula, FormulaTokenOxa)
{
    ChemicalFun::DBElements all_elements;
    all_elements.readElements(dbElements_str);
    EXPECT_EQ(all_elements.elements().size(), 14);
    EXPECT_EQ(all_elements.elementsKeys().size(), 14);

    ChemicalFun::FormulaToken token("Am|3|(Oxa)3-3");
    auto els_list = token.parsed_list();
    EXPECT_EQ(els_list.size(), 3);
    EXPECT_EQ(els_list[0], "{\"key\":{\"symbol\":\"Am\"},\"stoich_coef\":1.0,\"valence\":3}");
    EXPECT_EQ(els_list[1], "{\"key\":{\"symbol\":\"Oxa\"},\"stoich_coef\":3.0,\"valence\":-32768}");
    EXPECT_EQ(els_list[2], "{\"key\":{\"class_\":4,\"symbol\":\"Zz\"},\"stoich_coef\":-3.0,\"valence\":0}");
    EXPECT_THROW(token.charge(), std::exception);
    EXPECT_EQ(token.charge(all_elements.elements()), 3);
    EXPECT_THROW(token.testChargeImbalance(), std::exception);

    auto properties = token.properties(all_elements.elements());
    EXPECT_NEAR(properties.atomic_mass, 507.11800000000005, 1e-30);
    EXPECT_NEAR(properties.atoms_formula_unit, 1, 1e-30);
    EXPECT_NEAR(properties.charge, 3, 1e-30);
    EXPECT_NEAR(properties.elemental_entropy, 251.4199890136718, 1e-30);
    EXPECT_EQ(properties.formula, "Am|3|(Oxa)3-3");

    auto st_row = token.makeStoichiometryRow(all_elements.elementsKeysList());
    std::vector<double> row = { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, -3 };
    EXPECT_EQ(st_row, row);

    EXPECT_TRUE(token.testElements("Am|3|(Oxa)3-3", all_elements.elementsKeys()).empty()) ;
}
