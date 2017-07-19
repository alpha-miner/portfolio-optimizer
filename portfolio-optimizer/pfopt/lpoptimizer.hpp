#ifndef pfopt_linear_programming_optimizer_hpp
#define pfopt_linear_programming_optimizer_hpp

#include "types.hpp"
#include "ClpSimplex.hpp"

namespace pfopt {

    class PFOPT_CLASS LpOptimizer {
        public:
            LpOptimizer(const MatrixXd& constraintsMatraix,
                        const VectorXd& lowerBound,
                        const VectorXd& upperBound,
                        const VectorXd& objective);

            VectorXd solution() const;
            int status() const { return model_.status();}

        private:
            ClpSimplex model_;
			int numberOfProb_;

    };
}

#endif