#include "lpoptimizer.hpp"
#include "utilities.hpp"
#include "coin/ClpCholeskyMumps.hpp"

namespace pfopt {
    LpOptimizer::LpOptimizer(int numVariables,
                             int numCons,
                             double* constraintMatrix,
                             double* lowerBound,
                             double* upperBound,
                             double* objective,
                             std::string method) {

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

        if (method == "simplex") {
            ClpSimplex model;
            model.setLogLevel(0);
            model.loadProblem(matrix, &lowerBound[0], &upperBound[0], &objective[0], &rowLower[0], &rowUpper[0]);
            model.initialSolve();
            double *tmp = model.primalColumnSolution();
            sol_ = std::vector<double>(&tmp[0], &tmp[0] + numberOfProb_);
            feval_ = model.objectiveValue();
            status_ = model.status();
        } else {
            ClpInterior model;
            model.setLogLevel(0);
            model.loadProblem(matrix, &lowerBound[0], &upperBound[0], &objective[0], &rowLower[0], &rowUpper[0]);
            ClpCholeskyMumps* cholesky = new ClpCholeskyMumps();
            model.setCholesky(cholesky);
            model.primalDual();
            double *tmp = model.primalColumnSolution();
            sol_ = std::vector<double>(&tmp[0], &tmp[0] + numberOfProb_);
            feval_ = model.objectiveValue();
            status_ = model.status();
        }
    }

    double LpOptimizer::feval() const {
        return feval_;
    }
}