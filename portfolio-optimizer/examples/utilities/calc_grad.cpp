//
// Created by wegamekinglc on 18-5-26.
//

#include <chrono>
#include <iostream>
#include <iomanip>
#include <Eigen/Eigen>
#include <pfopt/utilities.hpp>

using namespace pfopt;
using namespace std;

VectorXd calc_grad_full(const VectorXd& w, const MatrixXd& varMatrix) {
    return varMatrix * w;
}

int main() {

    int widths[] = { 25, 14 };
    int loops = 1000;
    cout << setw(widths[0]) << left << "Type"
         << setw(widths[1]) << left << "Time(ms)" << endl;

    int n = 2000;
    int f = 40;
    VectorXd w = VectorXd::Random(n);
    MatrixXd factorLoading = MatrixXd::Random(n, f);
    MatrixXd factorVarMatrix = MatrixXd::Random(f, f);
    VectorXd idsync = VectorXd::Random(n);

    chrono::time_point<chrono::high_resolution_clock>
            start = chrono::high_resolution_clock::now();

    for(int i=0; i != loops; ++i)
        calculate_grad(w, factorLoading, factorVarMatrix, idsync);

    chrono::time_point<chrono::high_resolution_clock>
            current = chrono::high_resolution_clock::now();

    double elapsed = chrono::nanoseconds(current - start).count() / 1.0e6;

    cout << setw(widths[0]) << left << "factor model" << fixed << setprecision(6)
         << setw(widths[1]) << left << elapsed << endl;

    /*
     * full matrix
     */

    MatrixXd varMatrix = factorLoading * factorVarMatrix * factorLoading.transpose();
    start = chrono::high_resolution_clock::now();

    for(int i=0; i != loops; ++i)
        calc_grad_full(w, varMatrix);

    current = chrono::high_resolution_clock::now();
    elapsed = chrono::nanoseconds(current - start).count() / 1.0e6;

    cout << setw(widths[0]) << left << "full model" << fixed << setprecision(6)
         << setw(widths[1]) << left << elapsed << endl;

    return 0;
}