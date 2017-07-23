#include "MVOptimizer.hpp"

namespace pfopt {
    
    MVOptimizer::MVOptimizer(const std::vector<double>& expectReturn,
        const std::vector<double>& varMatrix,
        const std::vector<double>& lbound,
        const std::vector<double>& ubound) {
        
        mvImpl_ = new MeanVariance(expectReturn, varMatrix);
        mvImpl_->setBoundedConstraint(lbound, ubound);
        app_ = IpoptApplicationFactory();
        app_->Options()->SetNumericValue("tol", 1e-8);
        app_->Options()->SetIntegerValue("print_level", 0);
        app_->Options()->SetStringValue("linear_solver", "ma27");
        app_->Options()->SetStringValue("hessian_approximation", "limited-memory");
        app_->Initialize();
        status_ = app_->OptimizeTNLP(mvImpl_);
    }

}