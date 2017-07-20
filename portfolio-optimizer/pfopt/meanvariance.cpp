#include "meanvariance.hpp"


namespace pfopt {

    MeanVariance::MeanVariance(const std::vector<double> &expectReturn,
                               const std::vector<double> &varMatrix,
                               double riskAversion)
            :riskAversion_(riskAversion), feval_(0.) {
        assert((expectReturn.size() * expectReturn.size()) == varMatrix.size());
        numOfAssets_ = static_cast<int>(expectReturn.size());
        expectReturn_ = Map<VectorXd>(std::vector<double>(expectReturn).data(), numOfAssets_);
		varMatrix_ = Map<MatrixXd>(std::vector<double>(varMatrix).data(), numOfAssets_, numOfAssets_);

        xReal_.resize(numOfAssets_, 1);
        grad_f_.resize(numOfAssets_, 1);
        x_.resize(numOfAssets_);
    }

    bool MeanVariance::setBoundedConstraint(const VectorXd &lb, const VectorXd &ub) {
        assert(lb.size() == numOfAssets_);
        assert(ub.size() == numOfAssets_);
        lb_ = lb;
        ub_ = ub;
        return true;
    }

    bool MeanVariance::get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
                                    Index &nnz_h_lag, IndexStyleEnum &index_style) {
        n = numOfAssets_;
        m = 1;
        nnz_jac_g = numOfAssets_;
        nnz_h_lag = numOfAssets_ * (numOfAssets_ + 1) / 2;
        index_style = TNLP::C_STYLE;
        return true;
    }

    bool MeanVariance::get_bounds_info(Index n, Number *x_l, Number *x_u,
                                       Index m, Number *g_l, Number *g_u) {
        for (Index i = 0; i < numOfAssets_; ++i) {
            x_l[i] = lb_(i);
            x_u[i] = ub_(i);
        }
        g_l[0] = g_u[0] = 1.;
        return true;
    }

    bool MeanVariance::get_starting_point(Index n, bool init_x, Number *x,
                                          bool init_z, Number *z_L, Number *z_U,
                                          Index m, bool init_lambda,
                                          Number *lambda) {
        double startWeight = 1. / numOfAssets_;
        for (Index i = 0; i < numOfAssets_; ++i) {
            x[i] = startWeight;
        }
        return true;
    }

    bool MeanVariance::eval_f(Index n, const Number *x, bool new_x, Number &obj_value) {
        for (int i = 0; i < numOfAssets_; ++i) xReal_(i) = x[i];
        // risk grad
        VectorXd riskGrad = varMatrix_ * xReal_;
        obj_value = 0.5 * riskAversion_ * xReal_.dot(riskGrad) - expectReturn_.dot(xReal_);
        grad_f_ = riskAversion_ * riskGrad - expectReturn_;
        return true;
    }

    bool MeanVariance::eval_grad_f(Index n, const Number *x, bool new_x, Number *grad_f) {
        if (new_x) {
            for (int i = 0; i < numOfAssets_; ++i) xReal_(i) = x[i];
            // risk grad
            VectorXd riskGrad = varMatrix_ * xReal_;
            grad_f_ = riskAversion_ * riskGrad - expectReturn_;
            for (int i = 0; i < numOfAssets_; ++i) {
                grad_f[i] = grad_f_(i);
            }
        } else
            for (Index i = 0; i < numOfAssets_; ++i) grad_f[i] = grad_f_(i);
        return true;
    }

    bool MeanVariance::eval_g(Index n, const Number *x, bool new_x, Index m, Number *g) {
        g[0] = 0.;
        for (Index i = 0; i < numOfAssets_; ++i)
            g[0] += x[i];
        return true;
    }

    bool MeanVariance::eval_jac_g(Index n, const Number *x, bool new_x,
                                  Index m, Index nele_jac, Index *iRow, Index *jCol,
                                  Number *values) {
        if (values == NULL) {
            for (Index i = 0; i < numOfAssets_; ++i) {
                iRow[i] = 0;
                jCol[i] = i;
            }
        } else {
            for (Index i = 0; i < numOfAssets_; ++i) {
                for (Index i = 0; i < numOfAssets_; ++i) {
                    values[i] = 1.;
                }
            }
        }
        return true;
    }

    bool MeanVariance::eval_h(Index n, const Number *x, bool new_x,
                        Number obj_factor, Index m, const Number *lambda,
                        bool new_lambda, Index nele_hess, Index *iRow,
                        Index *jCol, Number *values) {
        if (values == NULL) {
            Index idx = 0;
            for (Index row = 0; row < numOfAssets_; ++row) {
                for (Index col = 0; col <= row; ++col) {
                    iRow[idx] = row;
                    jCol[idx] = col;
                    ++idx;
                }
            }
        }
        else {
            Index idx=0;
            for (Index row = 0; row < numOfAssets_; ++row) {
                for (Index col = 0; col <= row; ++col) {
                    values[idx] = obj_factor * varMatrix_(row, col);
                    ++idx;
                }
            }
        }

        return true;
    }

    void MeanVariance::finalize_solution(SolverReturn status,
                                         Index n, const Number *x, const Number *z_L, const Number *z_U,
                                         Index m, const Number *g, const Number *lambda,
                                         Number obj_value,
                                         const IpoptData *ip_data,
                                         IpoptCalculatedQuantities *ip_cq) {
        for (Index i = 0; i < numOfAssets_; ++i)
            x_[i] = x[i];
        feval_ = obj_value;
    }

}

