#include "meanvariance.hpp"
#include "utilities.hpp"
#include <numeric>

namespace pfopt {

    MeanVariance::MeanVariance(int numAssets,
                               double* expectReturn,
                               double* varMatrix,
                               double riskAversion)
        :numOfAssets_(numAssets), riskAversion_(riskAversion), feval_(0.) {
        expectReturn_ = Map<VectorXd>(expectReturn, numOfAssets_);
        varMatrix_ = Map<MatrixXd>(varMatrix, numOfAssets_, numOfAssets_);
        lb_ = nullptr;
        ub_ = nullptr;
        numCons_ = 0;
        clb_ = nullptr;
        cub_ = nullptr;
    }

    bool MeanVariance::setBoundedConstraint(const double* lb, const double* ub) {
        lb_ = lb;
        ub_ = ub;
        return true;
    }

    bool MeanVariance::setLinearConstrains(int numCons, const double* consMatrix, const double* clb, const double* cub) {
        numCons_ = numCons;
        clb_ = clb;
        cub_ = cub;
        for (auto i = 0; i != numCons_; ++i) {
            for (auto j = 0; j != numOfAssets_; ++j) {
                auto value = consMatrix[i*numOfAssets_ + j];
                if (!is_close(value, 0.)) {
                    iRow_.push_back(i);
                    jCol_.push_back(j);
                    g_grad_values_.push_back(value);
                }
            }
        }
        return true;
    }

    bool MeanVariance::get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
        Index &nnz_h_lag, IndexStyleEnum &index_style) {
        n = numOfAssets_;
        m = numCons_;
        nnz_jac_g = iRow_.size();
        index_style = TNLP::C_STYLE;
        return true;
    }

    bool MeanVariance::get_bounds_info(Index n, Number *x_l, Number *x_u,
        Index m, Number *g_l, Number *g_u) {
        std::copy(&lb_[0], &lb_[0] + numOfAssets_, &x_l[0]);
        std::copy(&ub_[0], &ub_[0] + numOfAssets_, &x_u[0]);
        if (m > 0) {
            std::copy(&clb_[0], &clb_[0] + m, &g_l[0]);
            std::copy(&cub_[0], &cub_[0] + m, &g_u[0]);
        }
        return true;
    }

    bool MeanVariance::get_starting_point(Index n, bool init_x, Number *x,
        bool init_z, Number *z_L, Number *z_U,
        Index m, bool init_lambda,
        Number *lambda) {
        auto startWeight = 1. / numOfAssets_;
        for (auto i = 0; i < numOfAssets_; ++i) {
            x[i] = startWeight;
        }
        return true;
    }

    bool MeanVariance::eval_f(Index n, const Number *x, bool new_x, Number &obj_value) {
        xReal_  = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);
        grad_f_ = riskAversion_ * varMatrix_ * xReal_ - expectReturn_;
        obj_value = 0.5 * xReal_.dot(grad_f_ - expectReturn_);
        return true;
    }

    bool MeanVariance::eval_grad_f(Index n, const Number *x, bool new_x, Number *grad_f) {
        if (new_x) {
            xReal_ = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);
            grad_f_ = riskAversion_ * varMatrix_ * xReal_ - expectReturn_;
            std::copy(&grad_f_.data()[0], &grad_f_.data()[0] + numOfAssets_, &grad_f[0]);
        }
        else
            std::copy(&grad_f_.data()[0], &grad_f_.data()[0] + numOfAssets_, &grad_f[0]);
        return true;
    }

    bool MeanVariance::eval_g(Index n, const Number *x, bool new_x, Index m, Number *g) {

        for (auto i = 0; i != m; ++i) {
            g[i] = 0.;
        }

        for (auto i = 0; i != iRow_.size(); ++i) {
            g[iRow_[i]] += x[jCol_[i]] * g_grad_values_[i];
        }
        return true;
    }

    bool MeanVariance::eval_jac_g(Index n, const Number *x, bool new_x,
        Index m, Index nele_jac, Index *iRow, Index *jCol,
        Number *values) {
        if (values == nullptr) {
            std::copy(iRow_.begin(), iRow_.end(), &iRow[0]);
            std::copy(jCol_.begin(), jCol_.end(), &jCol[0]);
        }
        else 
            std::copy(g_grad_values_.begin(), g_grad_values_.end(), &values[0]);
        return true;
    }

    void MeanVariance::finalize_solution(SolverReturn status,
        Index n, const Number *x, const Number *z_L, const Number *z_U,
        Index m, const Number *g, const Number *lambda,
        Number obj_value,
        const IpoptData *ip_data,
        IpoptCalculatedQuantities *ip_cq) {
        x_ = std::vector<double>(&x[0], &x[0] + numOfAssets_);
        feval_ = obj_value;
    }

}

