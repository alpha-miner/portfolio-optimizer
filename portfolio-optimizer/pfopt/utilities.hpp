#ifndef pfopt_utilities_hpp
#define pfopt_utilities_hpp

#include "types.hpp"

namespace pfopt {

    namespace io {
		PFOPT_API MatrixXd read_csv(const std::string &filePath);
    }

    double min(const real_1d_array& array, int n = 0);
    double max(const real_1d_array& array, int n = 0);
    double sum(const real_1d_array& array, int n = 0);
}

#endif