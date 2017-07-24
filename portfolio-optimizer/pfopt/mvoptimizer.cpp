#include "mvoptimizer.hpp"

namespace pfopt {
    
    MVOptimizer::MVOptimizer(const std::vector<double>& expectReturn,
        const std::vector<double>& varMatrix,
        const std::vector<double>& lbound,
        const std::vector<double>& ubound,
        const std::vector<double>& consMatrix,
        const std::vector<double>& clb,
        const std::vector<double>& cub,
        double riskAversion) {
        
        mvImpl_ = new MeanVariance(expectReturn, varMatrix, riskAversion);
        mvImpl_->setBoundedConstraint(lbound, ubound);
        mvImpl_->setLinearConstrains(consMatrix, clb, cub);
        app_ = IpoptApplicationFactory();
        app_->Options()->SetIntegerValue("print_level", 0);
        app_->Options()->SetStringValue("linear_solver", "ma27");
        app_->Options()->SetStringValue("hessian_approximation", "limited-memory");
        app_->Initialize();
        status_ = app_->OptimizeTNLP(mvImpl_);
    }

}
