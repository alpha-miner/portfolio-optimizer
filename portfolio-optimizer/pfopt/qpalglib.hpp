#ifndef pfopt_qp_alglib_hpp
#define pfopt_qp_alglib_hpp

#include "types.hpp"
#include <optimization.h>

using namespace alglib;

namespace pfopt {

    class AlglibData {
    public:
        AlglibData(int numAssets,
                   const double* expectReturn,
                   const double* varMatrix,
                   const double* lbound,
                   const double* ubound,
                   double riskAversion=1.);

        real_1d_array b() const { return b_;};
        real_2d_array a() const { return a_;};
        real_1d_array x0() const;
        real_1d_array bndl() const { return bndl_;}
        real_1d_array bndu() const { return bndu_;}
        real_1d_array scale() const;
        size_t numAssets() const { return n_;}

    private:
        real_1d_array b_;
        real_2d_array a_;
        real_1d_array bndl_;
        real_1d_array bndu_;
        size_t n_;
    };

    class QPAlglib {
    public:
        QPAlglib(int numAssets,
                 const double* expectReturn,
                 const double* varMatrix,
                 const double* lbound,
                 const double* ubound,
                 double riskAversion=1.);

        std::vector<double> xValue() const { return x_; }
        int status() const { return status_;}

    private:
        AlglibData data_;
        std::vector<double> x_;
        int status_;
    };
}

#endif
