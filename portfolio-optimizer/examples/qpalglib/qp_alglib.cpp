#include <chrono>
#include <pfopt/qpalglib.hpp>
#include <pfopt/utilities.hpp>
#include <iostream>
#include <iomanip>

using namespace pfopt;
using namespace std;


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

    string dataFolder;

    cout << "Please input data folder path: ";
    cin >> dataFolder;

    vector<double> varMatrix = io::read_csv(dataFolder + "/sec_cov_values.csv");
    vector<double> expectReturn = io::read_csv(dataFolder + "/signal.csv");

    int variableNumber = expectReturn.size();

    int widths[] = { 25, 14, 14, 14, 14, 14 };
    cout << setw(widths[0]) << left << "Scale"
        << setw(widths[1]) << left << "Time(ms)"
        << setw(widths[2]) << left << "f(x)"
        << setw(widths[3]) << left << "min(x)"
        << setw(widths[4]) << left << "max(x)"
        << setw(widths[5]) << left << "sum(x)"
        << endl;

    for (int n = 200; n <= variableNumber; n += 200) {

        vector<double> expectReturn_sub(expectReturn.begin(), expectReturn.begin() + n);
        vector<double> varMatrix_sub(n*n);

        for (int i = 0; i != n; ++i)
            for (int j = 0; j != n; ++j)
                varMatrix_sub[i*n + j] = varMatrix[i*variableNumber + j];

        chrono::time_point<chrono::high_resolution_clock>
            start = chrono::high_resolution_clock::now();

        vector<double> lbound(n, 0.);
        vector<double> ubound(n, 0.1);

        AlglibData data(n, &expectReturn_sub[0], &varMatrix_sub[0], &lbound[0], &ubound[0]);

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

        minqpsetalgobleic(state, 0.0, 0.0, 0.0, 0);
        minqpoptimize(state);
        minqpresults(state, x, rep);

        chrono::time_point<chrono::high_resolution_clock>
            current = chrono::high_resolution_clock::now();

        double elapsed = chrono::nanoseconds(current - start).count() / 1.0e6;
        cout << setw(widths[0]) << left << n
            << fixed << setprecision(6)
            << setw(widths[1]) << left << elapsed
            << setw(widths[2]) << left << calculate_cost(data.a(), data.b(), x)
            << setw(widths[3]) << left << pfopt::min(x)
            << setw(widths[4]) << left << pfopt::max(x)
            << setw(widths[5]) << left << pfopt::sum(x)
            << endl;

    }

    return 0;
}
