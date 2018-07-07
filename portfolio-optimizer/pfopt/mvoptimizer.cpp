#include "mvoptimizer.hpp"

namespace pfopt {
    
    MVOptimizer::MVOptimizer(int numAssets,
                             double* expectReturn,
                             double* varMatrix,
                             double* lbound,
                             double* ubound,
                             int numCons,
                             double* consMatrix,
                             double* clb,
                             double* cub,
                             double riskAversion,
                             int numFactors,
                             double* factorVarMatrix,
                             double* factorLoading,
                             double* idsync,
                             std::string linearSolver) {
        
        mvImpl_ = new MeanVariance(numAssets, expectReturn, varMatrix, riskAversion, numFactors, factorVarMatrix, factorLoading, idsync);
        mvImpl_->setBoundedConstraint(lbound, ubound);

        if(numCons > 0 && consMatrix != nullptr) {
            mvImpl_->setLinearConstrains(numCons, consMatrix, clb, cub);
        }
        app_ = IpoptApplicationFactory();
        app_->Options()->SetIntegerValue("print_level", 0);
        app_->Options()->SetNumericValue("tol", 1e-8);
        app_->Options()->SetStringValue("linear_solver", linearSolver);
        app_->Options()->SetStringValue("hessian_approximation", "limited-memory");
        app_->Initialize();
        status_ = app_->OptimizeTNLP(mvImpl_);
    }

}
