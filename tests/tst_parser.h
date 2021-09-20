#pragma once

#include <gtest/gtest.h>
#include "ChemicalFun/FormulaParser.h"

using namespace testing;
using namespace ChemicalFun;

TEST(FormulaParser, parser)
{
    ChemicalFun::ChemicalFormulaParser formparser;

    auto icterms = formparser.parse( "Al+3" );
    EXPECT_EQ(icterms.size(), 2);
    auto ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "Al");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 1);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), CHARGE_NAME);
    EXPECT_TRUE(ic_itr->is_charge());
    EXPECT_EQ(ic_itr->valence(),1);
    EXPECT_EQ(ic_itr->stoich_coef(), 3);

    icterms = formparser.parse( "Al(OH)2+" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "Al");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 1);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "H");
    EXPECT_FALSE(ic_itr->is_charge());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 2);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "O");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 2);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), CHARGE_NAME);
    EXPECT_TRUE(ic_itr->is_charge());
    EXPECT_EQ(ic_itr->valence(),1);
    EXPECT_EQ(ic_itr->stoich_coef(), 1);
}

TEST(FormulaParser, ParserStrings)
{
    ChemicalFun::ChemicalFormulaParser formparser;

    auto icterms = formparser.parse( "Al+3" );
    EXPECT_EQ(icterms.size(), 2);
    auto ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:3");

    icterms = formparser.parse( "Al(OH)2+" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:1");

    icterms = formparser.parse( "Al(OH)3@" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:3");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:3");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:0");

    icterms = formparser.parse( "HSiO3-" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "H:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:3");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:-1");

    icterms = formparser.parse( "HCl|3|O2@" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Cl:3:1");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:0");

    icterms = formparser.parse( "H+" );
    EXPECT_EQ(icterms.size(), 2);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "H:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:1");

    icterms = formparser.parse( "O|0|2@" );
    EXPECT_EQ(icterms.size(), 2);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "O:0:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:0");

    icterms = formparser.parse( "O|0|2" );
    EXPECT_EQ(icterms.size(), 1);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "O:0:2");

    icterms = formparser.parse( "HOO|0|-" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "H:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:0:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:-1");

    icterms = formparser.parse( "H2OO|0|@" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:0:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Zz:1:0");

    icterms = formparser.parse( "NaAlSi3O8" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Na:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:8");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:3");

    icterms = formparser.parse( "KAl3Si3O10(OH)2" );
    EXPECT_EQ(icterms.size(), 5);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:3");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "K:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:12");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:3");

    icterms = formparser.parse( "Al2SiO4(OH)2" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:6");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:1");

    icterms = formparser.parse( "FeFe|3|2O4" );
    EXPECT_EQ(icterms.size(), 3);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Fe:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Fe:3:2");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:4");

    icterms = formparser.parse( "Fe|2|4Fe|3|2(OH)12SO4" );
    EXPECT_EQ(icterms.size(), 5);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Fe:2:4");
    EXPECT_EQ((++ic_itr)->to_string(), "Fe:3:2");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:12");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:16");
    EXPECT_EQ((++ic_itr)->to_string(), "S:*:1");

    icterms = formparser.parse( "Mg48Si34O85(OH)62" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "H:*:62");
    EXPECT_EQ((++ic_itr)->to_string(), "Mg:*:48");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:147");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:34");

    icterms = formparser.parse( "{Va}:{Al}2:{Si}2:{Si}2:O10(OH)2" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:12");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:4");

    icterms = formparser.parse( "Na(Ca2Fe5)(AlSi7)O22(OH)2" );
    EXPECT_EQ(icterms.size(), 7);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "Ca:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Fe:*:5");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Na:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:24");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:7");

    icterms = formparser.parse( "Mg5Al(AlSi3)O10(OH)8" );
    EXPECT_EQ(icterms.size(), 5);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:8");
    EXPECT_EQ((++ic_itr)->to_string(), "Mg:*:5");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:18");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:3");

    icterms = formparser.parse( "MnS|-2|" );
    EXPECT_EQ(icterms.size(), 2);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Mn:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "S:-2:1");

    icterms = formparser.parse( "FeS|0|S|-2|" );
    EXPECT_EQ(icterms.size(), 3);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Fe:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "S:-2:1");
    EXPECT_EQ((++ic_itr)->to_string(), "S:0:1");

    icterms = formparser.parse( "(CaSiO3)2(CaSO4)2(CaCO3)2(H2O)30" );
    EXPECT_EQ(icterms.size(), 6);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "C:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Ca:*:6");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:60");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:50");
    EXPECT_EQ((++ic_itr)->to_string(), "S:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:2");

    icterms = formparser.parse( " Na0.99Al0.99Si2.01O6H2O" );
    EXPECT_EQ(icterms.size(), 5);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:0.99");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "Na:*:0.99");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:7");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:2.01");

    icterms = formparser.parse( "K(K3.2Mg1.7Al16Si24.3O69.5(OH)13.8)" );
    EXPECT_EQ(icterms.size(), 6);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:16");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:13.8");
    EXPECT_EQ((++ic_itr)->to_string(), "K:*:4.2");
    EXPECT_EQ((++ic_itr)->to_string(), "Mg:*:1.7");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:83.3");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:24.3");

    icterms = formparser.parse( "(SiO2)1(CaO)0.833333(H2O)1.333333" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Ca:*:0.833333");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2.666666");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:4.166666");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:1");

    icterms = formparser.parse( "NaSi10.473Al4.132Mg.737Fe|3|.237Fe|2|.211O44.316H30.737" );
    EXPECT_EQ(icterms.size(), 8);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Al:*:4.132");
    EXPECT_EQ((++ic_itr)->to_string(), "Fe:2:0.211");
    EXPECT_EQ((++ic_itr)->to_string(), "Fe:3:0.237");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:30.737");
    EXPECT_EQ((++ic_itr)->to_string(), "Mg:*:0.737");
    EXPECT_EQ((++ic_itr)->to_string(), "Na:*:1");
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:44.316");
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:10.473");

    icterms = formparser.parse( "/3/H2/18/O" );
    EXPECT_EQ(icterms.size(), 2);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "H/3/:*:2");
    EXPECT_EQ((++ic_itr)->to_string(), "O/18/:*:1");
}

TEST(FormulaParser, Isotope)
{
    ChemicalFun::ChemicalFormulaParser formparser;

    auto icterms = formparser.parse( "/3/H2/18/O" );
    EXPECT_EQ(icterms.size(), 2);
    auto ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "H");
    EXPECT_FALSE(ic_itr->no_isotope());
    EXPECT_EQ(ic_itr->str_isotope(), "3");
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 2);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "O");
    EXPECT_FALSE(ic_itr->no_isotope());
    EXPECT_FALSE(ic_itr->is_charge());
    EXPECT_EQ(ic_itr->str_isotope(), "18");
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 1);
}


TEST(FormulaParser, Valence)
{
    ChemicalFun::ChemicalFormulaParser formparser;

    auto icterms = formparser.parse( "FeS|0|S|-2|" );
    EXPECT_EQ(icterms.size(), 3);
    auto ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "Fe");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_TRUE(ic_itr->default_valence());
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "S");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_FALSE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->valence(), -2);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "S");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_FALSE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->valence(), 0);


    icterms = formparser.parse( "FeFe|3|2O4" );
    EXPECT_EQ(icterms.size(), 3);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "Fe");
    EXPECT_TRUE(ic_itr->default_valence());
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "Fe");
    EXPECT_FALSE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->valence(), 3);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "O");
    EXPECT_TRUE(ic_itr->default_valence());

    icterms = formparser.parse( "HOO|0|-" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "H");
    EXPECT_TRUE(ic_itr->default_valence());
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "O");
    EXPECT_TRUE(ic_itr->default_valence());
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "O");
    EXPECT_FALSE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->valence(), 0);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), CHARGE_NAME);
    EXPECT_FALSE(ic_itr->default_valence());
}


TEST(FormulaParser, ParserBase)
{
    ChemicalFun::ChemicalFormulaParser formparser;

    auto icterms = formparser.parse( " Al ( OH) 2 + " );
    EXPECT_EQ(icterms.size(), 4);
    auto ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->name(), "Al");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 1);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "H");
    EXPECT_FALSE(ic_itr->is_charge());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 2);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), "O");
    EXPECT_TRUE(ic_itr->no_isotope());
    EXPECT_TRUE(ic_itr->default_valence());
    EXPECT_EQ(ic_itr->stoich_coef(), 2);
    ic_itr++;
    EXPECT_EQ(ic_itr->name(), CHARGE_NAME);
    EXPECT_TRUE(ic_itr->is_charge());
    EXPECT_EQ(ic_itr->valence(),1);
    EXPECT_EQ(ic_itr->stoich_coef(), 1);

    icterms = formparser.parse( "(SiO 2)1(CaO) 0.833333 (H2O) 1.333333" );
    EXPECT_EQ(icterms.size(), 4);
    ic_itr = icterms.begin();
    EXPECT_EQ(ic_itr->to_string(), "Ca:*:0.833333");
    EXPECT_EQ((++ic_itr)->to_string(), "H:*:2.666666");
    EXPECT_NEAR(ic_itr->stoich_coef(), 2.666666, 1e-20);
    EXPECT_EQ((++ic_itr)->to_string(), "O:*:4.166666");
    EXPECT_NEAR(ic_itr->stoich_coef(), 4.166666, 1e-20);
    EXPECT_EQ((++ic_itr)->to_string(), "Si:*:1");
}

TEST(FormulaParser, ParserError)
{
    ChemicalFun::ChemicalFormulaParser formparser;

    EXPECT_THROW( formparser.parse( "FeS|0 S|-2|" ) , std::exception );
    EXPECT_THROW( formparser.parse( "K(K3.2Mg1.7Al16Si24.3O69.5(OH)13.8" ) , std::exception );
    EXPECT_THROW( formparser.parse( "Aoooooooool(OH)2+" ) , std::exception );
    EXPECT_THROW( formparser.parse( "FeS|1234|S|-2|" ) , std::exception );
    EXPECT_THROW( formparser.parse( "FeS|1.2|S|-2|" ) , std::exception );
}

TEST(MoityParser, Parser)
{
    ChemicalFun::MoityParser moiparser;

    int nsites=0;
    auto moieties = moiparser.parse( "{K}:{Fe}:{Fe}2:{Al}{Si}:Si2O10(OH)2", nsites );
    EXPECT_EQ(nsites, 4);
    EXPECT_EQ(moieties.size(), 5);
    EXPECT_EQ(moieties[0].name(), "{K}0");
    EXPECT_EQ(moieties[0].moiety_site_occupancy(), 1);
    EXPECT_EQ(moieties[1].name(), "{Fe}1");
    EXPECT_EQ(moieties[1].moiety_site_occupancy(), 1);
    EXPECT_EQ(moieties[2].name(), "{Fe}2");
    EXPECT_EQ(moieties[2].moiety_site_occupancy(), 2);
    EXPECT_EQ(moieties[3].name(), "{Al}3");
    EXPECT_EQ(moieties[3].moiety_site_occupancy(), 1);
    EXPECT_EQ(moieties[4].name(), "{Si}3");
    EXPECT_EQ(moieties[4].moiety_site_occupancy(), 1);

    moieties = moiparser.parse( "{Fe}3:{Al}2{Si}3:O12", nsites );
    EXPECT_EQ(nsites, 2);
    EXPECT_EQ(moieties.size(), 3);
    EXPECT_EQ(moieties[0].name(), "{Fe}0");
    EXPECT_EQ(moieties[0].sublattice_site(), 0);
    EXPECT_EQ(moieties[0].moiety_site_occupancy(), 3);
    EXPECT_EQ(moieties[1].name(), "{Al}1");
    EXPECT_EQ(moieties[1].sublattice_site(), 1);
    EXPECT_EQ(moieties[1].moiety_site_occupancy(), 2);
    EXPECT_EQ(moieties[2].name(), "{Si}1");
    EXPECT_EQ(moieties[2].moiety_site_occupancy(), 3);
}
