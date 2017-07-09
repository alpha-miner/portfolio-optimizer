#include "qpalglib.hpp"

namespace  pfopt {

    AlglibData::AlglibData(const VectorXd &expectReturn, const MatrixXd &varMatrix, double riskAversion) {
        b_.setlength(expectReturn.size());
        a_.setlength(varMatrix.rows(), varMatrix.cols());

        for(size_t i=0; i != b_.length(); ++i) {
            b_[i] = -expectReturn(i);
            for(size_t j=0; j != a_.cols(); ++j)
                a_[i][j]= riskAversion * varMatrix(i, j);
        }
        n_ = b_.length();
    }

    real_1d_array AlglibData::b() const { return b_;}

    real_2d_array AlglibData::a() const { return a_;}

    real_1d_array AlglibData::x0() const {
        real_1d_array x;
        x.setlength(n_);
        for(size_t i=0; i != n_; ++i)
            x[i] = 1. / n_;
        return x;
    }

    real_1d_array AlglibData::bndl() const {
        real_1d_array x;
        x.setlength(n_);
        for(size_t i=0; i != n_; ++i)
            x[i] = 0.;
        return x;
    }

    real_1d_array AlglibData::bndu() const {
        real_1d_array x;
        x.setlength(n_);
        for(size_t i=0; i != n_; ++i)
            x[i] = 0.01;
        return x;
    }

    real_1d_array AlglibData::scale() const {
        real_1d_array x;
        x.setlength(n_);
        for(size_t i=0; i != n_; ++i)
            x[i] = 1.;
        return x;
    }

    real_2d_array AlglibData::c() const {
        real_2d_array x;
        x.setlength(1, n_ + 1);
        for(size_t i=0; i != n_ + 1; ++i)
            x[0][i] = 1.;
        return x;
    }

    integer_1d_array AlglibData::ct() const{
        integer_1d_array x;
        x.setlength(1);
        x[0] = 0;
        return x;
    }
}

