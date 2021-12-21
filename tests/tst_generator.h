#pragma once

#include <sstream>
#include <gtest/gtest.h>
#include "ChemicalFun/ReactionsGenerator.h"
#include "ChemicalFun/ReactionsGenerator/ReactionGenerator.h"
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
    auto elemens_list = ChemicalFun::elementsInFormulas(formulas, true);
    EXPECT_EQ(to_string(elemens_list), elements_true);
    Eigen::MatrixXd A1 =  stoichiometryMatrix(formulas, true).transpose();
    EXPECT_EQ(A1, A_true);
    EXPECT_EQ(AtoString(A1),AtoString(A_true));

    elemens_list = ChemicalFun::elementsInFormulas(formulas, false);
    EXPECT_EQ(to_string(elemens_list), elements_false);
    Eigen::MatrixXd A2 =  stoichiometryMatrix(formulas, false).transpose();
    EXPECT_EQ(A2, A_false);
    EXPECT_EQ(AtoString(A2),AtoString(A_false));
}

TEST(ReactionsGenerator, DatabaseGeneratorF)
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_false, symbols);
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
    ReactionsGenerator::ChemicalReactions reactionsDB(A_true, symbols);
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
    ReactionsGenerator::ChemicalReactions reactionsDB(A_false, symbols);
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
    ReactionsGenerator::ChemicalReactions reactionsDB(A_true, symbols);
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
    ReactionsGenerator::ChemicalReactions reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(GramSchmidtWe94);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"  0.631625          0          0          0          0          0          0\n"
                                   " -0.527739   0.623023          0          0          0          0          0\n"
                                   "-0.0249326 -0.0211194   0.512554          0          0          0          0\n"
                                   " 0.0249326  0.0211194  -0.512554          0          0          0          0\n"
                                   " -0.311657  -0.263993   0.066136   0.647576          0          0          0\n"
                                   " 0.0997303  0.0844777  -0.099204  -0.199254   0.808608          0          0\n"
                                   " 0.0997303  0.0844777  -0.099204  -0.199254  -0.428086   0.685994          0\n"
                                   "  0.112197  0.0950374   0.132272  -0.249068  -0.380521  -0.685994          0\n"
                                   " 0.0249326  0.0211194   0.462952 -0.0996271  0.0951303   0.171499   0.707107\n"
                                   " 0.0249326  0.0211194   0.462952 -0.0996271  0.0951303   0.171499  -0.707107\n"
                                   "  0.423854    0.35903   0.066136   0.647576          0          0          0\n"
                                   " -0.103886  -0.623023          0          0          0          0          0");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));

    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices{});
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorT_GramSchmidtWe94 )
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(GramSchmidtWe94);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"  0.617213          0          0          0          0\n"
                                   " -0.540062   0.612372          0          0          0\n"
                                   "         0          0          0          0          0\n"
                                   "         0          0          0          0          0\n"
                                   " -0.231455  -0.204124    0.57735          0          0\n"
                                   "         0          0          0   0.707107          0\n"
                                   "         0          0          0  -0.707107          0\n"
                                   "  0.231455   0.204124   -0.57735          0          0\n"
                                   "         0          0          0          0   0.707107\n"
                                   "         0          0          0          0  -0.707107\n"
                                   "   0.46291   0.408248    0.57735          0          0\n"
                                   "-0.0771517  -0.612372          0          0          0");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));

    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices{});
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorF_RowReduceSmMi98 )
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(RowReduceSmMi98);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"   -1    -1    -1     0     0    -1    -1\n"
                                   "    1     1     1     0     0     1     2\n"
                                   " 0.25  0.25     0  -0.5  -0.5     0     0\n"
                                   "-0.25 -0.25     0   0.5   0.5     0     0\n"
                                   "    1     1     1     0     0     0     0\n"
                                   "   -1     0     0     0     0     0     0\n"
                                   "    0    -1     0     0     0     0     0\n"
                                   "    0     0    -1     0     0     0     0\n"
                                   "    0     0     0    -1     0     0     0\n"
                                   "    0     0     0     0    -1     0     0\n"
                                   "    0     0     0     0     0    -1     0\n"
                                   "    0     0     0     0     0     0    -1");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 0, 1, 2, 3, 4 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 5, 6, 7, 8, 9, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorT_RowReduceSmMi98 )
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(RowReduceSmMi98);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"-1  0  0 -1 -1\n"
                                   " 1  0  0  1  2\n"
                                   " 0  0  0  0  0\n"
                                   " 0  0  0  0  0\n"
                                   " 1  0  0  0  0\n"
                                   " 0  1  0  0  0\n"
                                   " 0  0  1  0  0\n"
                                   "-1  0  0  0  0\n"
                                   " 0 -1  0  0  0\n"
                                   " 0  0 -1  0  0\n"
                                   " 0  0  0 -1  0\n"
                                   " 0  0  0  0 -1");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 9, 7, 6, 8, 10, 11 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 0, 1, 2, 3, 4, 5, 9 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 7, 6, 8, 10, 11 }));
}

TEST(ReactionsGenerator, GeneratorF_CanonicalLe16 )
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(CanonicalLe16);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"    0     0  0.25   0.5   0.5     0  0.25\n"
                                   "  0.5     0     0     0     0  -0.5   0.5\n"
                                   "    0     1     1     0     0     0     1\n"
                                   "    0     0 -0.25  -0.5  -0.5     0 -0.25\n"
                                   "  0.5     0     0     0     0   0.5  -0.5\n"
                                   "   -1     0     0     0     0     0     0\n"
                                   "    0    -1     0     0     0     0     0\n"
                                   "    0     0    -1     0     0     0     0\n"
                                   "    0     0     0    -1     0     0     0\n"
                                   "    0     0     0     0    -1     0     0\n"
                                   "    0     0     0     0     0    -1     0\n"
                                   "    0     0     0     0     0     0    -1");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 3, 0, 5, 2, 11, 1, 6, 7, 8, 9, 10, 4 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 3, 0, 5, 2, 11 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 1, 6, 7, 8, 9, 10, 4 }));
}

TEST(ReactionsGenerator, GeneratorT_CanonicalLe16 )
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_true, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(CanonicalLe16);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"   0    0    0    0    0\n"
                                   " 0.5  0.5    0 -0.5    0\n"
                                   "   0    0    0    0    1\n"
                                   "   0    1    0    0    0\n"
                                   "   0    0    1    0    0\n"
                                   "   0    0    0    0    0\n"
                                   " 0.5 -0.5    0  0.5    0\n"
                                   "  -1    0    0    0    0\n"
                                   "   0   -1    0    0    0\n"
                                   "   0    0   -1    0    0\n"
                                   "   0    0    0   -1    0\n"
                                   "   0    0    0    0   -1");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 3, 0, 5, 7, 8, 2, 11, 1, 4, 9, 10, 6 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 3, 0, 5, 7, 8, 2, 11 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 1, 4, 9, 10, 6 }));
}


TEST(ReactionsGenerator, ReactionsF_RowReduceSmMi98 )
{
    ReactionsGenerator::ChemicalReactions reactionsDB(A_false, symbols);
    ReactionsGenerator::Generator generator;
    generator.setMethod(RowReduceSmMi98);
    generator.compute(reactionsDB.formulaMatrix());

    auto reactions   = generator.reactionMatrix();
    EXPECT_EQ(AtoString(reactions),"   -1    -1    -1     0     0    -1    -1\n"
                                   "    1     1     1     0     0     1     2\n"
                                   " 0.25  0.25     0  -0.5  -0.5     0     0\n"
                                   "-0.25 -0.25     0   0.5   0.5     0     0\n"
                                   "    1     1     1     0     0     0     0\n"
                                   "   -1     0     0     0     0     0     0\n"
                                   "    0    -1     0     0     0     0     0\n"
                                   "    0     0    -1     0     0     0     0\n"
                                   "    0     0     0    -1     0     0     0\n"
                                   "    0     0     0     0    -1     0     0\n"
                                   "    0     0     0     0     0    -1     0\n"
                                   "    0     0     0     0     0     0    -1");

    auto iSubstances = generator.isubstances();
    EXPECT_EQ(iSubstances, Indices({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 }));
    auto iMaster     = generator.imaster();
    EXPECT_EQ(iMaster, Indices({ 0, 1, 2, 3, 4 }));
    auto iNonMaster     = generator.inonmaster();
    EXPECT_EQ(iNonMaster, Indices({ 5, 6, 7, 8, 9, 10, 11 }));

    std::vector<bool> isoc = reactionsDB.checkReacMatrixISOC(reactions, iSubstances);
    EXPECT_EQ(isoc, std::vector<bool>({ false, false, false, true, true, false, false }));
    std::vector<bool> isoe;

    std::vector<ReactionsGenerator::Reaction> vReac;
    for (int i = 0; i < reactions.cols(); i++)
    {
        VectorXd v(reactions.rows());
        v << reactions.col(i);
        vReac.push_back(ReactionsGenerator::Reaction (v, iSubstances, &reactionsDB, ""));
        isoc[i] = vReac[i].isIsocoulombic();
        isoe.push_back(vReac[i].isIsoelectric());
    }
    EXPECT_EQ(isoc, std::vector<bool>({ false, false, false, true, true, false, false }));
    EXPECT_EQ(isoe, std::vector<bool>({ true, true, true, true, true, false, true }));
}
