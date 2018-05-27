//
// Created by wegamekinglc on 3/16/18.
//

#include "tvoptimizer.hpp"

namespace pfopt {

    TVOptimizer::TVOptimizer(int numAssets, double *expectReturn, double *varMatrix, double *lbound, double *ubound,
                             int numCons, double *consMatrix, double *clb, double *cub, double targetVolLow,
                             double targetVolHigh,
                             int numFactors,
                             double *factorVarMatrix,
                             double *factorLoading,
                             double *idsync) {
        tvImpl_ = new TargetVol(numAssets, expectReturn, varMatrix, targetVolLow, targetVolHigh, numFactors, factorVarMatrix, factorLoading, idsync);
        tvImpl_->setBoundedConstraint(lbound, ubound);

        if (numCons > 0 && consMatrix != nullptr)
            tvImpl_->setLinearConstrains(numCons, consMatrix, clb, cub);

        app_ = IpoptApplicationFactory();
        app_->Options()->SetIntegerValue("print_level", 0);
        app_->Options()->SetNumericValue("tol", 1e-8);
        app_->Options()->SetStringValue("linear_solver", "ma27");
        app_->Options()->SetStringValue("hessian_approximation", "limited-memory");
        app_->Initialize();
        status_ = app_->OptimizeTNLP(tvImpl_);
    }
}