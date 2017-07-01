#ifndef pfopt_utilities_hpp
#define pfopt_utilities_hpp

#include <eigen3/Eigen/Eigen>

using Eigen::MatrixXd;

MatrixXd read_csv(const std::string& filePath);

#endif