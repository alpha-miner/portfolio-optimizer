#include <boost/chrono.hpp>
#include <coin/IpIpoptApplication.hpp>
#include <pfopt/utilities.hpp>
#include <pfopt/meanvariance.hpp>
#include <iomanip>

using namespace pfopt;

int main() {
    MatrixXd varMatrix = io::read_csv("../../../data/sec_cov_values.csv");
    VectorXd expectReturn = io::read_csv("../../../data/signal.csv");

    int variableNumber = varMatrix.rows();

    int widths[] = { 25, 14, 14, 14, 14, 14};
    std::cout << std::setw(widths[0]) << std::left << "Scale"
        << std::setw(widths[1]) << std::left << "Time(s)"
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
            bndu[i] = 0.2;

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                start = boost::chrono::high_resolution_clock::now();

        VectorXd expectReturn_sub = expectReturn.block(0, 0, n, 1);
        MatrixXd varMatrix_sub = varMatrix.block(0, 0, n, n);

        Ipopt::SmartPtr<MeanVariance> mynlp = new MeanVariance(expectReturn_sub, varMatrix_sub);
        mynlp->setBoundedConstraint(bndl, bndu);
        Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();
        app->Options()->SetNumericValue("tol", 1e-8);
        app->Options()->SetIntegerValue("print_level", 0);
        app->Options()->SetStringValue("linear_solver", "ma27");
        app->Options()->SetStringValue("hessian_approximation", "limited-memory");
        Ipopt::ApplicationReturnStatus status = app->Initialize();
        status = app->OptimizeTNLP(mynlp);
        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e9;
        std::cout << std::setw(widths[0]) << std::left << n
                << std::fixed << std::setprecision(6)
                << std::setw(widths[1]) << std::left << elapsed
                << std::setw(widths[2]) << std::left << mynlp->feval()
                << std::setw(widths[3]) << std::left << mynlp->xValue().minCoeff()
                << std::setw(widths[4]) << std::left << mynlp->xValue().maxCoeff()
                << std::setw(widths[5]) << std::left << mynlp->xValue().sum()
                << std::endl;
    }

    return 0;
}
