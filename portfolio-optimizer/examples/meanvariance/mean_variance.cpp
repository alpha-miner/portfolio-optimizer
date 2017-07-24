#include <boost/chrono.hpp>
#include <pfopt/utilities.hpp>
#include <pfopt/mvoptimizer.hpp>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace pfopt;
using namespace std;

int main() {

    std::string dataFolder;

    std::cout << "Please input data folder path: ";
    std::cin >> dataFolder;

    vector<double> varMatrix = io::read_csv(dataFolder + "/sec_cov_values.csv");
    vector<double> expectReturn = io::read_csv(dataFolder + "/signal.csv");

    int variableNumber = expectReturn.size();

    int widths[] = { 25, 14, 14, 14, 14, 14, 18 };
    std::cout << std::setw(widths[0]) << std::left << "Scale"
        << std::setw(widths[1]) << std::left << "Time(ms)"
        << std::setw(widths[2]) << std::left << "f(x)"
        << std::setw(widths[3]) << std::left << "min(x)"
        << std::setw(widths[4]) << std::left << "max(x)"
        << std::setw(widths[5]) << std::left << "sum(x)"
        << std::setw(widths[6]) << std::left << "x(0) + x(1)"
        << std::endl;

    for (int n = 200; n <= variableNumber; n += 200) {
        std::vector<double> bndl(n);
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        std::vector<double> bndu(n);
        for (int i = 0; i != n; ++i)
            bndu[i] = 0.01;

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
            start = boost::chrono::high_resolution_clock::now();

        vector<double> expectReturn_sub(expectReturn.begin(), expectReturn.begin() + n);
        vector<double> varMatrix_sub(n*n);

        for (int i = 0; i != n; ++i)
            for (int j = 0; j != n; ++j)
                varMatrix_sub[i*n + j] = varMatrix[i*variableNumber + j];

        std::vector<double> consMatrix(2*n, 0.);
        for (int i = 0; i != n; ++i)
            consMatrix[i] = 1.;

        consMatrix[n] = 1.;
        consMatrix[n + 1] = 1.;

        std::vector<double> clb = { 0.95, 0.010 };
        std::vector<double> cub = { 1., 0.015 };

        MVOptimizer mvOptimizer(expectReturn_sub, varMatrix_sub, bndl, bndu, consMatrix, clb, cub);
        int status = mvOptimizer.status();
        std::vector<double> sol = mvOptimizer.xValue();
        boost::chrono::time_point<boost::chrono::high_resolution_clock>
            current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e6;
        std::cout << std::setw(widths[0]) << std::left << n
            << std::fixed << std::setprecision(6)
            << std::setw(widths[1]) << std::left << elapsed
            << std::setw(widths[2]) << std::left << mvOptimizer.feval()
            << std::setw(widths[3]) << std::left << *min_element(sol.begin(), sol.end())
            << std::setw(widths[4]) << std::left << *max_element(sol.begin(), sol.end())
            << std::setw(widths[5]) << std::left << accumulate(sol.begin(), sol.end(), 0.)
            << std::setw(widths[6]) << std::left << sol[0] + sol[1]
            << std::endl;
    }

    return 0;
}
