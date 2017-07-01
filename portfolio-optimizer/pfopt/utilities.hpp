#ifndef pfopt_utilities_hpp
#define pfopt_utilities_hpp

#include "types.hpp"

namespace pfopt {

    namespace io {
        MatrixXd read_csv(const std::string &filePath);
    }
}

#endif