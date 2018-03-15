//
// Created by wegamekinglc on 18-3-15.
//

#ifndef pfopt_target_vol_hpp
#define pfopt_target_vol_hpp

#include "types.hpp"
#include <vector>
#include <coin/IpTNLP.hpp>

using Ipopt::TNLP;
using Ipopt::Number;
using Ipopt::Index;


namespace pfopt {

    class TargetVol : public TNLP {
    public:
        TargetVol(int numAssets,
                  double* expectReturn,
                  double* varMatrixm,
                  double targetVol);

        bool setBoundedConstraint(const double* lb, const double* ub);
        bool setLinearConstrains(int numCons, const double* consMatrix, const double* clb, const double* cub);

        virtual bool get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
                                  Index &nnz_h_lag, IndexStyleEnum &index_style);

        virtual bool get_bounds_info(Index n, Number *x_l, Number *x_u,
                                     Index m, Number *g_l, Number *g_u);

        virtual bool get_starting_point(Index n, bool init_x, Number *x,
                                        bool init_z, Number *z_L, Number *z_U,
                                        Index m, bool init_lambda,
                                        Number *lambda);

        virtual bool eval_f(Index n, const Number *x, bool new_x, Number &obj_value);
        virtual bool eval_grad_f(Index n, const Number *x, bool new_x, Number *grad_f);
        virtual bool eval_g(Index n, const Number *x, bool new_x, Index m, Number *g);
        virtual bool eval_jac_g(Index n, const Number *x, bool new_x,
                                Index m, Index nele_jac, Index *iRow, Index *jCol,
                                Number *values);


    private:
        VectorXd expectReturn_;
        MatrixXd varMatrix_;
        const double targetVol_;
        const int numOfAssets_;
        int numCons_;
        VectorXd xReal_;

        const double* lb_;
        const double* ub_;
        VectorXd grad_f_;
        const double* clb_;
        const double* cub_;

        std::vector<Index> iRow_;
        std::vector<Index> jCol_;
        std::vector<double> g_grad_values_;
    };

}

#endif
