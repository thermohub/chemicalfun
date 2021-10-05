#include "ChemicalFun/ReactionsGenerator/MatrixUtils.h"
#include "ChemicalFun/FormulaParser/ChemicalData.h"

namespace ReactionsGenerator {

// Return a vector of indices with values from 0 up to a given length.
auto indices(Index length) -> Indices
{
    Indices res(length);
    for(Index i = 0; i < length; ++i) res[i] = i;
    return res;
}

// Augment identity matrix to the left of given matrix M
auto augmentIdentityMatrixLeft(MatrixXd &M) -> void
{
    auto rows = M.rows();
    auto cols = M.cols();

    MatrixXd F(rows, rows+cols);
    F << MatrixXd::Identity(rows, rows), M;

    M = F;
}

// Augment identity matrix to the right of given matrix M
auto augmentIdentityMatrixRight(MatrixXd &M) -> void
{
    auto rows = M.rows();
    auto cols = M.cols();

    MatrixXd F(rows, rows+cols);
    F << M, MatrixXd::Identity(rows, rows);

    M = F;
}

double toPrecision(double num, int n) {
    if(num == 0)
    {
        return 0.0;
    }
    std::ostringstream oss;
    oss.precision(n);
    oss<<std::fixed;
    oss<<num;
    double result = atof((oss.str()).c_str());
    return  result;
}

auto roundVectorCoeff(Eigen::VectorXd M) -> Eigen::VectorXd
{
    auto R = M.size(); // number of rows

    for (unsigned j = 0; j < R; j++)
    {
        M(j) = toPrecision(M(j), 3);
    }
    return M;
}

// Format coefficients of vector V from floating point to integer
auto formatCoeffToInt(Eigen::VectorXd V, double &minVal) -> Eigen::VectorXd
{
    auto R = V.size();

    for (unsigned j = 0; j < R; j++)
    {
        long intPart = (long) V(j);
        double fractionalPart = fabs(V(j) - intPart);

        if ((fractionalPart+thresholdCoeffZero) >= 1)
        {
            fractionalPart = 0;
            if (intPart >= 0)
                V(j) = intPart + 1;
            else
                V(j) = intPart - 1;
        }

        // if the fractional part is bigger than the threshold multiply by 10
        if (fabs(fractionalPart) >= thresholdCoeffZero)
        {
            V = V*10;
            minVal = minVal/10;
            V = formatCoeffToInt(V, minVal);
        }
    }
    return V;
}

// fromat 7.999 to 8 and 2.22341e-16 to 0
auto formatVectorCoeff(Eigen::VectorXd M) -> Eigen::VectorXd
{
    auto R = M.size();
    auto m = M;

    // Make 0 the coefficients with values smaller than the threshold
    for (unsigned j = 0; j < R; j++)
    {
        m(j) = fabs(M(j));

        if (m(j) <= thresholdCoeffZero)
        {
            m(j) = 1e30; M(j) = 0;
        }
    }

    for (unsigned j = 0; j < R; j++)
    {
        long intPart = (long) M(j);
        double fractionalPart = fabs(M(j) - intPart);

        if ((fractionalPart+thresholdCoeffZero) >= 1)
        {
            fractionalPart = 0;
            if (intPart >= 0)
                M(j) = intPart + 1;
            else
                M(j) = intPart - 1;
        }

        if (fractionalPart > 0 && fractionalPart < thresholdCoeffZero)
            M(j) = intPart;
    }
    return M;
}

// Format coefficients of vector V from floating point to integer
auto formatVectorCoeffToInt(Eigen::VectorXd M) -> Eigen::VectorXd
{
    int minIndex; // index of minimum value
    double minVal; // niminmum value

    auto R = M.size(); // number of rows
    auto m = M; // temporary vector which will store the absolute values found in M

    // Make 0 the coefficients with values smaller than the threshold
    for (unsigned j = 0; j < R; j++)
    {
        m(j) = fabs(M(j));

        if (m(j) <= thresholdCoeffZero)
        {
            m(j) = 1e30; M(j) = 0;
        }
    }

    minVal = m.minCoeff(&minIndex); // get the minimum value
    M = M / minVal; // divide all coefficients with the minimum values
    M = roundVectorCoeff(M);

/*    long intPart = (long) M(M.size()-1);
    double fractionalPart = fabs(V(M.size()-1) - intPart);
    if ((fractionalPart/10) < 1)
    M = formatCoeffToInt(M, minVal);*/ // repeat recursively until no digits after the floating point are present

    return M;
}

// Format coefficients of Matrix M from floating point to integer
auto formatMatrixCoeffToInt(MatrixXd &M) -> void
{
    for (unsigned i = 0; i <M.cols(); i++)
    {
        VectorXd vec(M.cols());
        vec = VectorXd::Zero(M.cols());
        M.col(i) = formatVectorCoeffToInt(M.col(i));
    }
}

auto formatMatrixCoeff(MatrixXd &M) -> void
{
    for (unsigned i = 0; i <M.cols(); i++)
    {
        M.col(i) = formatVectorCoeff(M.col(i));
    }
}

auto formatMatrixCoeffToZero(MatrixXd &M) -> void
{
    for (unsigned i = 0; i <M.cols(); i++)
    {
        M.col(i) = formatVectorCoeffToZero(M.col(i));
    }
}

auto formatVectorCoeffToZero(Eigen::VectorXd M) -> Eigen::VectorXd
{
    auto R = M.size(); // number of rows
    auto m = M; // temporary vector which will store the absolute values found in M

    // Make 0 the coefficients with values smaller than the threshold
    for (unsigned j = 0; j < R; j++)
    {
        m(j) = fabs(M(j));

        if (m(j) <= thresholdCoeffZero)
        {
            m(j) = 1e30; M(j) = 0;
        }
    }
    return M;
}

// Adds the reaction coefficients rows for non-master substances
auto addNonMasterCoeffRows(MatrixXd &M, unsigned r) -> void
{
    for (unsigned i = 0; i <r; i++)
    {
        VectorXd vec(M.cols());
        vec = VectorXd::Zero(M.cols());

        vec(M.cols()-(r-i)) = -1;
        M.conservativeResize(M.rows()+1, M.cols());
        M.row(M.rows()-1) = vec;
    }
}

auto removeMasterColls(MatrixXd &M, unsigned i) -> void
{
    for (unsigned c = 0; c<i; c++)
    {
        M.block(0,0,M.rows(),M.cols()-1) = M.block(0,1,M.rows(),M.cols()-1);
        M.conservativeResize(M.rows(),M.cols()-1);
    }
}

auto getStoichiometryMatrix( std::vector<std::vector<double>> vMatrix) -> Eigen::MatrixXd
{
    MatrixXd A(vMatrix.size(), vMatrix[0].size());

    for (size_t i = 0; i < vMatrix.size(); i++)
        for (size_t j = 0; j < vMatrix[0].size(); j++)
            A(i,j) = vMatrix[i][j];
    return A;
}

auto calcStoichiometryMatrix(const std::vector<std::string>& vFormulalist,
                            const std::vector<ChemicalFun::ElementKey>& elemens_list) -> Eigen::MatrixXd
{
    MatrixXd A(vFormulalist.size(), elemens_list.size());
    ChemicalFun::FormulaToken formula("");

    for (size_t i = 0; i < vFormulalist.size(); i++) {
        formula.setFormula(vFormulalist[i], true);
        auto row = formula.makeStoichiometryRow(elemens_list);
        for (size_t j = 0; j < row.size(); j++) {
            A(i,j) = row[j];
        }
    }
    return A;
}

}
