#include <boost/chrono.hpp>
#include <pfopt/utilities.hpp>
#include <pfopt/lpoptimizer.hpp>
#include <iostream>
#include <iomanip>

using namespace pfopt;

int main() {

    int widths[] = { 25, 14, 14, 14, 14, 14};
    std::cout << std::setw(widths[0]) << std::left << "Scale"
        << std::setw(widths[1]) << std::left << "Time(ms)"
        << std::setw(widths[2]) << std::left << "f(x)"
        << std::setw(widths[3]) << std::left << "min(x)"
        << std::setw(widths[4]) << std::left << "max(x)"
        << std::setw(widths[5]) << std::left << "sum(x)"
        << std::endl;

    for(int n=200; n <= 3000; n += 200) {
        VectorXd bndl(n);
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        VectorXd bndu(n);
        for (int i = 0; i != n; ++i)
            bndu[i] = 0.01;

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                start = boost::chrono::high_resolution_clock::now();

        VectorXd object = VectorXd::Random(n);
        MatrixXd cons = MatrixXd::Ones(1, n+2);

        LpOptimizer opt(cons, bndl, bndu, object);
        VectorXd sol = opt.solution();

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e6;
        std::cout << std::setw(widths[0]) << std::left << n
                << std::fixed << std::setprecision(6)
                << std::setw(widths[1]) << std::left << elapsed
                << std::setw(widths[2]) << std::left << 0.
                << std::setw(widths[3]) << std::left << sol.minCoeff()
                << std::setw(widths[4]) << std::left << sol.maxCoeff()
                << std::setw(widths[5]) << std::left << sol.sum()
                << std::endl;
    }

    return 0;
}