#include "meanvariance.hpp"
#include "utilities.hpp"
#include <numeric>
#include <iterator>

namespace pfopt {

    MeanVariance::MeanVariance(const std::vector<double> &expectReturn,
        const std::vector<double> &varMatrix,
        double riskAversion)
        :riskAversion_(riskAversion), feval_(0.), m_(0) {
        assert((expectReturn.size() * expectReturn.size()) == varMatrix.size());
        numOfAssets_ = static_cast<int>(expectReturn.size());
        expectReturn_ = Map<VectorXd>(std::vector<double>(expectReturn).data(), numOfAssets_);
        varMatrix_ = Map<MatrixXd>(std::vector<double>(varMatrix).data(), numOfAssets_, numOfAssets_);

        xReal_.resize(numOfAssets_, 1);
        grad_f_.resize(numOfAssets_, 1);
        x_.resize(numOfAssets_);
    }

    bool MeanVariance::setBoundedConstraint(const std::vector<double> &lb, const std::vector<double> &ub) {
        assert(lb.size() == numOfAssets_);
        assert(ub.size() == numOfAssets_);
        lb_ = lb;
        ub_ = ub;
        return true;
    }

    bool MeanVariance::setLinearConstrains(const std::vector<double>& consMatrix, const std::vector<double>& clb, const std::vector<double>& cub) {
        assert((consMatrix.size() / clb.size()) == numOfAssets_);
        m_ = clb.size();
        clb_ = clb;
        cub_ = cub;
        for (int i = 0; i != m_; ++i) {
            for (int j = 0; j != numOfAssets_; ++j) {
                double value = consMatrix[i*numOfAssets_ + j];
                if (!is_close(value, 0.)) {
                    iRow_.push_back(i);
                    jCol_.push_back(j);
                    g_grad_values_.push_back(value);
                }
            }
        }
    }

    bool MeanVariance::get_nlp_info(Index &n, Index &m, Index &nnz_jac_g,
        Index &nnz_h_lag, IndexStyleEnum &index_style) {
        n = numOfAssets_;
        m = m_;
        nnz_jac_g = iRow_.size();
        index_style = TNLP::C_STYLE;
        return true;
    }

    bool MeanVariance::get_bounds_info(Index n, Number *x_l, Number *x_u,
        Index m, Number *g_l, Number *g_u) {
        std::copy(lb_.begin(), lb_.end(), &x_l[0]);
        std::copy(ub_.begin(), ub_.end(), &x_u[0]);
        if (clb_.size() > 0) {
            std::copy(clb_.begin(), clb_.end(), &g_l[0]);
            std::copy(cub_.begin(), cub_.end(), &g_u[0]);
        }
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
        xReal_  = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);
        // risk grad
        VectorXd riskGrad = varMatrix_ * xReal_;
        obj_value = 0.5 * riskAversion_ * xReal_.dot(riskGrad) - expectReturn_.dot(xReal_);
        grad_f_ = riskAversion_ * riskGrad - expectReturn_;
        return true;
    }

    bool MeanVariance::eval_grad_f(Index n, const Number *x, bool new_x, Number *grad_f) {
        if (new_x) {
            xReal_ = Map<VectorXd>(const_cast<Number *>(x), numOfAssets_);
            // risk grad
            VectorXd riskGrad = varMatrix_ * xReal_;
            grad_f_ = riskAversion_ * riskGrad - expectReturn_;
            std::copy(&grad_f_.data()[0], &grad_f_.data()[0] + numOfAssets_, &grad_f[0]);
        }
        else
            std::copy(&grad_f_.data()[0], &grad_f_.data()[0] + numOfAssets_, &grad_f[0]);
        return true;
    }

    bool MeanVariance::eval_g(Index n, const Number *x, bool new_x, Index m, Number *g) {

        for (Index i = 0; i != m_; ++i) {
            g[i] = 0.;
        }

        for (Index i = 0; i != iRow_.size(); ++i) {
            g[iRow_[i]] += x[jCol_[i]] * g_grad_values_[i];
        }
        return true;
    }

    bool MeanVariance::eval_jac_g(Index n, const Number *x, bool new_x,
        Index m, Index nele_jac, Index *iRow, Index *jCol,
        Number *values) {
        if (values == NULL) {
            std::copy(iRow_.begin(), iRow_.end(), &iRow[0]);
            std::copy(jCol_.begin(), jCol_.end(), &jCol[0]);
        }
        else {
            std::copy(g_grad_values_.begin(), g_grad_values_.end(), &values[0]);
        }
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

