#include <boost/chrono.hpp>
#include <pfopt/qpalglib.hpp>
#include <pfopt/utilities.hpp>
#include <iostream>
#include <iomanip>

using namespace pfopt;


double calculate_cost(const real_2d_array& a, const real_1d_array& b, const real_1d_array& w) {

    size_t n = w.length();

    double cost1 = alglib::vdotproduct(&b[0], &w[0], n);
    // risk cost
    real_1d_array yVector;
    yVector.setlength(n);
    alglib::rmatrixmv(n, n, a, 0, 0, 0, w, 0, yVector, 0);
    double totalRiskCost = 0.5 * alglib::vdotproduct(&yVector[0], 1, &w[0], 1, n) + cost1;
    return totalRiskCost;
}


int main() {

    std::string dataFolder;

    std::cout << "Please input data folder path: ";
    std::cin >> dataFolder;

    MatrixXd varMatrix = io::read_csv(dataFolder + "/sec_cov_values.csv");
    VectorXd expectReturn = io::read_csv(dataFolder + "/signal.csv");

    int variableNumber = varMatrix.rows();

    int widths[] = { 25, 14, 14, 14, 14, 14};
    std::cout << std::setw(widths[0]) << std::left << "Scale"
              << std::setw(widths[1]) << std::left << "Time(ms)"
              << std::setw(widths[2]) << std::left << "f(x)"
              << std::setw(widths[3]) << std::left << "min(x)"
              << std::setw(widths[4]) << std::left << "max(x)"
              << std::setw(widths[5]) << std::left << "sum(x)"
              << std::endl;

    for(int n=200; n <= variableNumber; n += 200) {

        VectorXd expectReturn_sub = expectReturn.block(0, 0, n, 1);
        MatrixXd varMatrix_sub = varMatrix.block(0, 0, n, n);

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                start = boost::chrono::high_resolution_clock::now();

        AlglibData data(expectReturn_sub, varMatrix_sub);

        minqpstate state;
        minqpreport rep;
        real_1d_array x;

        // create solver, set quadratic/linear terms
        minqpcreate(n, state);
        minqpsetquadraticterm(state, data.a());
        minqpsetlinearterm(state, data.b());
        minqpsetstartingpoint(state, data.x0());
        minqpsetbc(state, data.bndl(), data.bndu());
        minqpsetscale(state, data.scale());
        minqpsetlc(state, data.c(), data.ct());

        minqpsetalgobleic(state, 0.0, 0.0, 0.0, 0);
        //minqpsetalgodenseaul(state, 1.0e-9, 1.0e+4, 5);
        minqpoptimize(state);
        minqpresults(state, x, rep);

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e6;
        std::cout << std::setw(widths[0]) << std::left << n
                  << std::fixed << std::setprecision(6)
                  << std::setw(widths[1]) << std::left << elapsed
                  << std::setw(widths[2]) << std::left << calculate_cost(data.a(), data.b(), x)
                  << std::setw(widths[3]) << std::left << pfopt::min(x)
                  << std::setw(widths[4]) << std::left << pfopt::max(x)
                  << std::setw(widths[5]) << std::left << pfopt::sum(x)
                  << std::endl;

    }

    return 0;
}
