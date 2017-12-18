#include <chrono>
#include <pfopt/lpoptimizer.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace pfopt;
using namespace std;


int main() {

    int widths[] = { 25, 14, 14, 14, 14, 14, 14 };
    cout << setw(widths[0]) << left << "Scale"
        << setw(widths[1]) << left << "Time(ms)"
        << setw(widths[2]) << left << "f(x)"
        << setw(widths[3]) << left << "min(x)"
        << setw(widths[4]) << left << "max(x)"
        << setw(widths[5]) << left << "sum(x)"
        << setw(widths[6]) << left << "x(0) + x(1)"
        << endl;

    for (int n = 200; n <= 3000; n += 200) {
        double* bndl = new double[n];
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        double* bndu = new double[n];
        for (int i = 0; i != n; ++i)
            bndu[i] = 0.01;

        VectorXd objectValues = VectorXd::Random(n);
        double* object = new double[n];
        for (int i = 0; i != n; ++i)
            object[i] = objectValues(i);

        double* cons = new double[2 * (n + 2)];
        memset(cons, 0, 2 * (n + 2) * sizeof(double));

        for (int i = 0; i != n + 2; ++i)
            cons[i] = 1.;

        cons[n + 2] = 1.;
        cons[n + 3] = 1.;
        cons[2 * n + 2] = 0.015;
        cons[2 * n + 3] = 0.015;

        chrono::time_point<chrono::high_resolution_clock>
            start = chrono::high_resolution_clock::now();

        LpOptimizer opt(n, 2, cons, bndl, bndu, object);
        vector<double> sol(&opt.xValue()[0], &opt.xValue()[0] + n);

        chrono::time_point<chrono::high_resolution_clock>
            current = chrono::high_resolution_clock::now();

        double elapsed = chrono::nanoseconds(current - start).count() / 1.0e6;
        cout << setw(widths[0]) << left << n
            << fixed << setprecision(6)
            << setw(widths[1]) << left << elapsed
            << setw(widths[2]) << left << opt.feval()
            << setw(widths[3]) << left << *min_element(sol.begin(), sol.end())
            << setw(widths[4]) << left << *max_element(sol.begin(), sol.end())
            << setw(widths[5]) << left << accumulate(sol.begin(), sol.end(), 0.)
            << setw(widths[6]) << left << accumulate(sol.begin(), sol.begin() + 2, 0.) << endl;

        delete[] bndl;
        delete[] bndu;
        delete[] object;
    }

    return 0;
}