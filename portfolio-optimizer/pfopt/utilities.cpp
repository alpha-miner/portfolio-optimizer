#include "utilities.hpp"
#include "minicsv.hpp"
#include <cmath>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

namespace pfopt {

    namespace io {
        std::vector<double> read_csv(const std::string &filePath) {

            std::vector<double> data;

            mini::csv::ifstream is(filePath);

            if (is.is_open()) {
                while (is.read_line()) {
                    std::string field;
                    is >> field;
                    while (field != "") {
                        data.push_back(boost::lexical_cast<double>(field));
                        is >> field;
                    }
                }
            } else {
                throw std::runtime_error(str(boost::format("file %s is not found") % filePath));
            }

            return data;
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