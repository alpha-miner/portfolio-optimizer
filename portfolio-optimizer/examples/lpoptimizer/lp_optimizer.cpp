#include <boost/chrono.hpp>
#include <pfopt/utilities.hpp>
#include <pfopt/lpoptimizer.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace pfopt;
using namespace std;


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
        vector<double> bndl(n);
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        vector<double> bndu(n);
        for (int i = 0; i != n; ++i)
            bndu[i] = 0.01;

        VectorXd objectValues = VectorXd::Random(n);
        vector<double> object(n);
        for (int i = 0; i != n; ++i)
            object[i] = objectValues(i);
            
        vector<double> cons(2*n+4, 0.);

        for(int i=0; i != n+2; ++i)
            cons[i] = 1.;

        cons[n+2] = 1.;
        cons[n+3] = 1.;
        cons[2*n+2] = 0.015;
        cons[2*n+3] = 0.015;

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                start = boost::chrono::high_resolution_clock::now();

        LpOptimizer opt(cons, bndl, bndu, object);
        std::vector<double> sol = opt.xValue();

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e6;
		std::cout << std::setw(widths[0]) << std::left << n
			<< std::fixed << std::setprecision(6)
			<< std::setw(widths[1]) << std::left << elapsed
			<< std::setw(widths[2]) << std::left << opt.feval()
			<< std::setw(widths[3]) << std::left << *min_element(sol.begin(), sol.end())
			<< std::setw(widths[4]) << std::left << *max_element(sol.begin(), sol.end())
			<< std::setw(widths[5]) << std::left << accumulate(sol.begin(), sol.end(), 0.)
            << std::setw(widths[5]) << std::left << accumulate(sol.begin(), sol.begin()+2, 0.) << endl;
    }

    return 0;
}