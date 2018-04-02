#ifndef pfopt_linear_programming_optimizer_hpp
#define pfopt_linear_programming_optimizer_hpp

#include "types.hpp"
#include <vector>
#include <string>
#include "coin/ClpSimplex.hpp"
#include "coin/ClpInterior.hpp"

namespace pfopt {

    class LpOptimizer {
    public:
        LpOptimizer(int numVariables,
                    int numCons,
                    double* constraintMatrix,
                    double* lowerBound,
                    double* upperBound,
                    double* objective,
                    std::string method="simplex");

        std::vector<double> xValue() const { return sol_; }
        double feval() const;
        int status() const { return status_; }

    private:
        int status_;
        double feval_;
        size_t numberOfProb_;
        std::vector<double> sol_;
    };
}

#endif