#include "lpoptimizer.hpp"
#include "utilities.hpp"
#include <vector>

namespace pfopt {
    LpOptimizer::LpOptimizer(const std::vector<std::vector<double>>& constraintsMatrix,
                             const std::vector<double>& lowerBound,
                             const std::vector<double>& upperBound,
                             const std::vector<double>& objective) {
        assert(lowerBound.size() == upperBound.size());
        assert((constraintsMatrix[0].size() - 2) == lowerBound.size());
        assert(objective.size() == lowerBound.size());

        int numberRows = constraintsMatrix.size();
        int numberColumns = lowerBound.size();
        numberOfProb_ = numberColumns;
       
        std::vector<int> rows;
        std::vector<int> lengths;
        std::vector<CoinBigIndex> starts;
        std::vector<double> elements;

        int currentSize = 0;

        for(int j=0; j != numberColumns; ++j) {
            starts.push_back(currentSize);
            for(int i=0; i != numberRows; ++i) {
                double value = constraintsMatrix[i][j];
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
            rowLower.push_back(constraintsMatrix[i][numberColumns]);
            rowUpper.push_back(constraintsMatrix[i][numberColumns+1]);
        }

        model_.setLogLevel(0);
        model_.loadProblem(matrix, &lowerBound[0], &upperBound[0], &objective[0], &rowLower[0], &rowUpper[0]);
        model_.initialSolve();

    }

    std::vector<double> LpOptimizer::solution() const {
        std::vector<double> sol(numberOfProb_);
        double* tmp = model_.primalColumnSolution();
        for(int i=0; i != numberOfProb_; ++i)
            sol[i] = tmp[i];
        return sol;
    }
}