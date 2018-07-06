//
// Created by wegamekinglc on 3/16/18.
//

#ifndef PFOPT_TVOPTIMIZER_HPP
#define PFOPT_TVOPTIMIZER_HPP

#include "targetvol.hpp"
#include <coin/IpIpoptApplication.hpp>

namespace pfopt {

    class TVOptimizer {
    public:
        TVOptimizer(int numAssets,
                    double* expectReturn,
                    double* varMatrix,
                    double* lbound,
                    double* ubound,
                    int numConstraints,
                    double* consMatrix=nullptr,
                    double* clb=nullptr,
                    double* cub= nullptr,
                    double targetVol=1.0,
                    int numFactors=0,
                    double* factorVarMatrix=nullptr,
                    double* factorLoading=nullptr,
                    double* idsync=nullptr);
        std::vector<double> xValue() const { return tvImpl_->xValue(); }

        double feval() const { return tvImpl_->feval(); }

        int status() const { return status_; }

    private:
    private:
        Ipopt::SmartPtr<TargetVol> tvImpl_;
        Ipopt::SmartPtr<Ipopt::IpoptApplication> app_;
        Ipopt::ApplicationReturnStatus status_;
    };
}


#endif //PFOPT_TVOPTIMIZER_HPP
