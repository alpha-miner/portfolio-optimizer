#include "utilities.hpp"
#include "minicsv.hpp"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace pfopt {

    namespace io {
        MatrixXd read_csv(const std::string &filePath) {

            std::vector<std::vector<double>> table;

            mini::csv::ifstream is(filePath);

            if (is.is_open()) {
                while (is.read_line()) {
                    std::string field;
                    is >> field;
                    std::vector<double> row;
                    while (field != "") {
                        row.push_back(boost::lexical_cast<double>(field));
                        is >> field;
                    }
                    table.push_back(row);
                }
            } else {
                throw std::runtime_error(str(boost::format("file %s is not found") % filePath));
            }

            MatrixXd res(table.size(), table[0].size());

            for (size_t i = 0; i != res.rows(); ++i)
                for (size_t j = 0; j != res.cols(); ++j)
                    res(i, j) = table[i][j];

            return res;
        }
    }

    double min(const real_1d_array& array, int n)
    {
        double minimum = 1e308;
        if (n == 0)
            n = array.length();
        for (int i = 0; i != n; ++i)
        {
            if (array[i] < minimum)
                minimum = array[i];
        }
        return minimum;
    }

    double max(const real_1d_array& array, int n)
    {
        double maximum = 1e-308;
        if (n == 0)
            n = array.length();
        for (int i = 0; i != n; ++i)
        {
            if (array[i] > maximum)
                maximum = array[i];
        }
        return maximum;
    }

    double sum(const real_1d_array& array, int n)
    {
        double sumValue = 0.0;
        if (n == 0)
            n = array.length();
        for (int i = 0; i != n; ++i)
        {
            sumValue += array[i];
        }
        return sumValue;
    }

    bool is_close(double a, double b, double tol) {
        if(fabs(a - b) >= tol)
            return false;
        else
            return true;
    }
}