#include "qpalglib.hpp"

namespace  pfopt {

    AlglibData::AlglibData(int numAssets,
                           const double* expectReturn,
                           const double* varMatrix,
                           const double* lbound,
                           const double* ubound,
                           double riskAversion) {
        b_.setlength(numAssets);
        a_.setlength(numAssets, numAssets);

        for(size_t i=0; i != numAssets; ++i) {
            b_[i] = -expectReturn[i];
            for(size_t j=0; j != numAssets; ++j)
                a_[i][j]= riskAversion * varMatrix[i*numAssets + j];
        }

        bndl_.setlength(numAssets);
        bndu_.setlength(numAssets);

        for(size_t i=0; i != numAssets; ++i) {
            bndl_[i] = lbound[i];
            bndu_[i] = ubound[i];
        }

        n_ = b_.length();
    }

    real_1d_array AlglibData::x0() const {
        real_1d_array x;
        x.setlength(n_);
        for(size_t i=0; i != n_; ++i)
            x[i] = 1. / n_;
        return x;
    }

    real_1d_array AlglibData::scale() const {
        real_1d_array x;
        x.setlength(n_);
        for(size_t i=0; i != n_; ++i)
            x[i] = 1.;
        return x;
    }

    QPAlglib::QPAlglib(int numAssets, const double *expectReturn, const double *varMatrix, const double *lbound,
                       const double *ubound, double riskAversion)
    :data_(numAssets, expectReturn, varMatrix, lbound, ubound, riskAversion) {
        alglib::minqpstate state;
        alglib::minqpreport rep;

        alglib::minqpcreate(data_.numAssets(), state);
        alglib::minqpsetquadraticterm(state, data_.a());
        alglib::minqpsetlinearterm(state, data_.b());
        alglib::minqpsetstartingpoint(state, data_.x0());
        alglib::minqpsetbc(state, data_.bndl(), data_.bndu());
        alglib::minqpsetscale(state, data_.scale());

        alglib::minqpsetalgoquickqp(state, 1e-8, 1e-8, 1e-8, 0, true);
        alglib::minqpoptimize(state);
        real_1d_array x;
        alglib::minqpresults(state, x, rep);

        for(size_t i=0; i != data_.numAssets(); ++i)
            x_.push_back(x[i]);

        status_ = rep.terminationtype;
    }
}

