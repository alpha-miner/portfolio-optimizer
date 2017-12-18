#include <chrono>
#include <pfopt/utilities.hpp>
#include <pfopt/mvoptimizer.hpp>
#include <iomanip>
#include <algorithm>
#include <numeric>

using namespace pfopt;
using namespace std;

int main() {

    string dataFolder;

    cout << "Please input data folder path: ";
    cin >> dataFolder;

    vector<double> varMatrix = io::read_csv(dataFolder + "/sec_cov_values.csv");
    vector<double> expectReturn = io::read_csv(dataFolder + "/signal.csv");

    int variableNumber = expectReturn.size();

    int widths[] = { 25, 14, 14, 14, 14, 14, 18 };
    cout << setw(widths[0]) << left << "Scale"
        << setw(widths[1]) << left << "Time(ms)"
        << setw(widths[2]) << left << "f(x)"
        << setw(widths[3]) << left << "min(x)"
        << setw(widths[4]) << left << "max(x)"
        << setw(widths[5]) << left << "sum(x)"
        << setw(widths[6]) << left << "x(0) + x(1)"
        << endl;

    for (int n = 200; n <= variableNumber; n += 200) {
        double* bndl = new double[n];
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        double* bndu = new double[n];
        for (int i = 0; i != n; ++i)
            bndu[i] = 0.01;

        chrono::time_point<chrono::high_resolution_clock>
            start = chrono::high_resolution_clock::now();

        vector<double> expectReturn_sub(expectReturn.begin(), expectReturn.begin() + n);
        double* er = expectReturn_sub.data();
        double* varMatrix_sub = new double[n*n];

        for (int i = 0; i != n; ++i)
            for (int j = 0; j != n; ++j)
                varMatrix_sub[i*n + j] = varMatrix[i*variableNumber + j];

        double* consMatrix = new double[n];
        memset(consMatrix, 0, n * sizeof (double));
        for (int i = 0; i != n; ++i)
            consMatrix[i] = 1.;

        double clb[] = {1.};
        double cub[] = {1.};

        MVOptimizer mvOptimizer(n, er, varMatrix_sub, bndl, bndu, 1, consMatrix, clb, cub);
        int status = mvOptimizer.status();
        vector<double> sol(&mvOptimizer.xValue()[0], &mvOptimizer.xValue()[0] + n);
        chrono::time_point<chrono::high_resolution_clock>
            current = chrono::high_resolution_clock::now();

        double elapsed = chrono::nanoseconds(current - start).count() / 1.0e6;
        cout << setw(widths[0]) << left << n
            << fixed << setprecision(6)
            << setw(widths[1]) << left << elapsed
            << setw(widths[2]) << left << mvOptimizer.feval()
            << setw(widths[3]) << left << *min_element(sol.begin(), sol.end())
            << setw(widths[4]) << left << *max_element(sol.begin(), sol.end())
            << setw(widths[5]) << left << accumulate(sol.begin(), sol.end(), 0.)
            << setw(widths[6]) << left << sol[0] + sol[1]
            << endl;

        delete [] bndl;
        delete [] bndu;
        delete [] varMatrix_sub;
        delete [] consMatrix;
    }

    return 0;
}
