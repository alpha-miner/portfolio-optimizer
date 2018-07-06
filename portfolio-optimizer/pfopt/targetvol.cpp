//
// Created by wegamekinglc on 18-3-15.
//

#include "targetvol.hpp"
#include "utilities.hpp"

namespace pfopt {

    TargetVol::TargetVol(int numAssets,
                         double* expectReturn,
                         double* varMatrix,
                         double targetVol)
            :numOfAssets_(numAssets), targetVol_(targetVol) {
        expectReturn_ = Map<VectorXd>(expectReturn, numOfAssets_);
        varMatrix_ = Map<MatrixXd>(varMatrix, numOfAssets_, numOfAssets_);
        lb_ = nullptr;
        ub_ = nullptr;
        numCons_ = 1;
        clb_ = nullptr;
        cub_ = nullptr;
    }

    bool TargetVol::setBoundedConstraint(const double* lb, const double* ub) {
        lb_ = lb;
        ub_ = ub;
        return true;
    }

    bool TargetVol::setLinearConstrains(int numCons, const double* consMatrix, const double* clb, const double* cub) {
        numCons_ += numCons;
        clb_ = clb;
        cub_ = cub;
        for (auto i = 0; i != numCons; ++i) {
            for (auto j = 0; j != numOfAssets_; ++j) {
                auto value = consMatrix[i*numOfAssets_ + j];
                if (!is_close(value, 0.)) {
                    iRow_.push_back(i+1);
                    jCol_.push_back(j);
                    g_grad_values_.push_back(value);
                }
            }
        }
        return true;
    }

    bool TargetVol::get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
                                    Index &nnz_h_lag, IndexStyleEnum &index_style) {
        n = numOfAssets_;
        m = numCons_;
        nnz_jac_g = static_cast<Index>(n + iRow_.size());
        index_style = TNLP::C_STYLE;
        return true;
    }

    bool TargetVol::get_bounds_info(Index n, Number *x_l, Number *x_u,
                                       Index m, Number *g_l, Number *g_u) {
        std::copy(&lb_[0], &lb_[0] + n, &x_l[0]);
        std::copy(&ub_[0], &ub_[0] + n, &x_u[0]);
        g_l[0] = -1.e8;
        g_u[0] = 0.5 * targetVol_ * targetVol_;
        if (m > 1) {
            std::copy(&clb_[0], &clb_[0] + m - 1, &g_l[0] + 1);
            std::copy(&cub_[0], &cub_[0] + m - 1, &g_u[0] + 1);
        }
        return true;
    }

    bool TargetVol::get_starting_point(Index n, bool init_x, Number *x,
                                       bool init_z, Number *z_L, Number *z_U,
                                       Index m, bool init_lambda,
                                       Number *lambda) {
        for (auto i = 0; i < numOfAssets_; ++i) {
            x[i] = 0.;
        }
        return true;
    }

    bool TargetVol::eval_f(Index n, const Number *x, bool new_x, Number &obj_value) {
        xReal_  = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);
        grad_f_ = - expectReturn_;
        obj_value = - xReal_.dot(expectReturn_);
        return true;
    }

    bool TargetVol::eval_grad_f(Index n, const Number *x, bool new_x, Number *grad_f) {
        if (new_x) {
            grad_f_ =  - expectReturn_;
            std::copy(&grad_f_.data()[0], &grad_f_.data()[0] + numOfAssets_, &grad_f[0]);
        }
        else
            std::copy(&grad_f_.data()[0], &grad_f_.data()[0] + numOfAssets_, &grad_f[0]);
        return true;
    }

    bool TargetVol::eval_g(Index n, const Number *x, bool new_x, Index m, Number *g) {

        xReal_  = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);

        for (auto i = 0; i != m; ++i) {
            g[i] = 0.;
        }

        g[0] = 0.5 * xReal_.dot(varMatrix_ * xReal_);
        if(m > 1) {
            for (auto i = 0; i != iRow_.size(); ++i) {
                g[iRow_[i]] += x[jCol_[i]] * g_grad_values_[i];
            }
        }
        return true;
    }

    bool TargetVol::eval_jac_g(Index n, const Number *x, bool new_x,
                               Index m, Index nele_jac, Index *iRow, Index *jCol,
                               Number *values) {
        if (values == nullptr) {
            for(auto i=0; i!=n; ++i) {
                iRow[i] = 0;
                jCol[i] = i;
            }
            if(m > 1) {
                std::copy(iRow_.begin(), iRow_.end(), &iRow[0] + n);
                std::copy(jCol_.begin(), jCol_.end(), &jCol[0] + n);
            }
        }
        else {
            xReal_ = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);
            VectorXd gg = varMatrix_ * xReal_;
            for(auto i=0; i!=n; ++i)
                values[i] = gg[i];
            if(m > 1) {
                std::copy(g_grad_values_.begin(), g_grad_values_.end(), &values[0] + n);
            }
        }
        return true;
    }

    void TargetVol::finalize_solution(SolverReturn status,
                                      Index n, const Number *x, const Number *z_L, const Number *z_U,
                                      Index m, const Number *g, const Number *lambda,
                                      Number obj_value,
                                      const IpoptData *ip_data,
                                      IpoptCalculatedQuantities *ip_cq) {
        x_ = std::vector<double>(&x[0], &x[0] + numOfAssets_);
        feval_ = obj_value;
    }

}