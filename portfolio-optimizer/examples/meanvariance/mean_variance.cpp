#include <boost/chrono.hpp>
#include <coin/IpIpoptApplication.hpp>
#include <pfopt/utilities.hpp>
#include <pfopt/meanvariance.hpp>

using namespace pfopt;

int main() {
    MatrixXd varMatrix = io::read_csv("../../../data/sec_cov_values.csv");
    VectorXd expectReturn = io::read_csv("../../../data/signal.csv");

    int variableNumber = varMatrix.rows();

    for(int n=200; n <= 3000; n += 200) {
        VectorXd bndl(n);
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        VectorXd bndu(n);
        for (int i = 0; i != n; ++i)
            bndu[i] = 1. / n * 30;

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                start = boost::chrono::high_resolution_clock::now();

        VectorXd expectReturn_sub = expectReturn.block(0, 0, n, 1);
        MatrixXd varMatrix_sub = varMatrix.block(0, 0, n, n);

        Ipopt::SmartPtr<MeanVariance> mynlp = new MeanVariance(expectReturn_sub, varMatrix_sub);
        mynlp->setBoundedConstraint(bndl, bndu);
        Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();
        app->Options()->SetNumericValue("tol", 1e-6);
        app->Options()->SetIntegerValue("print_level", 0);
        app->Options()->SetStringValue("linear_solver", "ma27");
        app->Options()->SetStringValue("hessian_approximation", "limited-memory");
        Ipopt::ApplicationReturnStatus status = app->Initialize();
        status = app->OptimizeTNLP(mynlp);
        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e6;
        std::cout << n << ", " << elapsed << " ms" << std::endl;
    }

    return 0;
}
