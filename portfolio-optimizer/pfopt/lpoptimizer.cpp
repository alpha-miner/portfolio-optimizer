#include "lpoptimizer.hpp"
#include "utilities.hpp"

namespace pfopt {
    LpOptimizer::LpOptimizer(int numVariables,
                             int numCons,
                             double* constraintMatrix,
                             double* lowerBound,
                             double* upperBound,
                             double* objective) {

        auto numberColumns = numVariables;
        auto numberRows = numCons;

        numberOfProb_ = numberColumns;
       
        std::vector<int> rows;
        std::vector<int> lengths;
        std::vector<CoinBigIndex> starts;
        std::vector<double> elements;

        size_t currentSize = 0;

        for(int j=0; j != numberColumns; ++j) {
            starts.push_back(currentSize);
            for(int i=0; i != numberRows; ++i) {
                double value = constraintMatrix[i*(numberColumns+2) + j];
                if(!is_close(value, 0.)) {
                    elements.push_back(value);
                    rows.push_back(i);
                }
            }
            lengths.push_back(elements.size() - currentSize);
            currentSize = elements.size();
        }

        starts.push_back(currentSize);

        CoinPackedMatrix matrix(true, numberRows, numberColumns, currentSize, &elements[0], &rows[0], &starts[0], &lengths[0]);

        std::vector<double> rowLower;
        std::vector<double> rowUpper;

        for(int i=0; i != numberRows; ++i) {
            rowLower.push_back(constraintMatrix[i*(numberColumns+2) + numberColumns]);
            rowUpper.push_back(constraintMatrix[i*(numberColumns+2) + numberColumns+1]);
        }

        model_.setLogLevel(0);
        model_.loadProblem(matrix, &lowerBound[0], &upperBound[0], &objective[0], &rowLower[0], &rowUpper[0]);
        model_.initialSolve();
        sol_ = new double[numberOfProb_];
        double* tmp = model_.primalColumnSolution();
        std::copy(&tmp[0], &tmp[0] + numberOfProb_, &sol_[0]);
    }

    double LpOptimizer::feval() const {
        return model_.objectiveValue();
    }
}