#pragma once

#include <sstream>
#include <gtest/gtest.h>
#include "ChemicalFun/ReactionsGenerator.h"
#include "ChemicalFun/FormulaParser.h"

using namespace testing;
using namespace ReactionsGenerator;

std::vector<std::string> symbols =
{ "Ca+2", "CaOH+", "Cl-", "ClO4-", "H+", "H2", "H2@", "H2O@", "O2", "O2@", "OH-", "Portlandite" };

std::vector<std::string> formulas =
{ "Ca+2", "Ca(OH)+", "Cl-", "Cl|7|O4-", "H+", "H|0|2", "H|0|2@", "H2O@", "O|0|2", "O|0|2@", "OH-", "Ca(OH)2" };

std::string elements_false = "Ca;Cl;H;O;Zz;";
Eigen::MatrixXd A_false{
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 2, 2, 2, 0, 0, 1, 2},
    {0, 1, 0, 4, 0, 0, 0, 1, 2, 2, 1, 2},
    {2, 1, -1, -1, 1, 0, 0, 0, 0, 0, -1, 0},
};

std::string elements_true = "Ca(2);Cl(-1);Cl(7);H(0);H(1);O(-2);O(0);Zz(0);";
Eigen::MatrixXd A_true{
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 2, 0, 0, 1, 2},
    {0, 1, 0, 4, 0, 0, 0, 1, 0, 0, 1, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0},
    {2, 1,-1,-1, 1, 0, 0, 0, 0, 0,-1, 0}
};

std::string AtoString( const Eigen::MatrixXd& A)
{
    std::stringstream ss;
    ss << A;
    return ss.str();
}


TEST(ReactionsGenerator, StoichiometryMatrix)
{
    auto elemens_list = ChemicalFun::generateElementsListValences(formulas, true);
    EXPECT_EQ(to_string(elemens_list), elements_true);
    Eigen::MatrixXd A1 =  calcStoichiometryMatrix(formulas, true).transpose();
    EXPECT_EQ(A1, A_true);
    EXPECT_EQ(AtoString(A1),AtoString(A_true));

    elemens_list = ChemicalFun::generateElementsListValences(formulas, false);
    EXPECT_EQ(to_string(elemens_list), elements_false);
    Eigen::MatrixXd A2 =  calcStoichiometryMatrix(formulas, false).transpose();
    EXPECT_EQ(A2, A_false);
    EXPECT_EQ(AtoString(A2),AtoString(A_false));
}

TEST(ReactionsGenerator, DatabaseGeneratorF)
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_false, symbols);
    MatrixXd B = reactionsDB.formulaMatrix();
    EXPECT_EQ(B, A_false);
    EXPECT_EQ(reactionsDB.sizeSubstancesMap(), 12);

    EXPECT_EQ(reactionsDB.getCharge("Ca+2"), 2);
    EXPECT_EQ(reactionsDB.getCharge("ClO4-"), -1);
    EXPECT_EQ(reactionsDB.getCharge("H2O@"), 0);
    EXPECT_EQ(reactionsDB.getCharge(11), 0);
}

TEST(ReactionsGenerator, DatabaseGeneratorT)
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_true, symbols);
    MatrixXd B = reactionsDB.formulaMatrix();
    EXPECT_EQ(B, A_true);
    EXPECT_EQ(reactionsDB.sizeSubstancesMap(), 12);

    EXPECT_EQ(reactionsDB.getCharge("Ca+2"), 2);
    EXPECT_EQ(reactionsDB.getCharge("ClO4-"), -1);
    EXPECT_EQ(reactionsDB.getCharge("H2O@"), 0);
    EXPECT_EQ(reactionsDB.getCharge(11), 0);
}


TEST(ReactionsGenerator, GeneratorF_GramSchmidt )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(GramSchmidt);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices{});

    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices{});
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices{});
}

TEST(ReactionsGenerator, GeneratorT_GramSchmidt )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(GramSchmidt);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(reactions, A_true);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices{});

    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices{});
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices{});
}

TEST(ReactionsGenerator, GeneratorF_GramSchmidtWe94 )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(GramSchmidtWe94);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    //EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));

    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices{});
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorT_GramSchmidtWe94 )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(GramSchmidtWe94);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    //EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));

    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices{});
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorF_RowReduceSmMi98 )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(RowReduceSmMi98);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    //EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 0, 1, 2, 3, 4 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 5, 6, 7, 8, 9, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorT_RowReduceSmMi98 )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(RowReduceSmMi98);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    //EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 9, 7, 6, 8, 10, 11 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 0, 1, 2, 3, 4, 5, 9 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 7, 6, 8, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorF_CanonicalLe16 )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(CanonicalLe16);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    //EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 3, 0, 5, 2, 11, 1, 6, 7, 8, 9, 10, 4 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 3, 0, 5, 2, 11 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 1, 6, 7, 8, 9, 10, 4 }));
}

TEST(ReactionsGenerator, GeneratorT_CanonicalLe16 )
{
    ReactionsGenerator::DatabaseGenerator reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(CanonicalLe16);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    //EXPECT_EQ(reactions, A_false);

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 3, 0, 5, 7, 8, 2, 11, 1, 4, 9, 10, 6 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 3, 0, 5, 7, 8, 2, 11 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 1, 4, 9, 10, 6 }));
}
