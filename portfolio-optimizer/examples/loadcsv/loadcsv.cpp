#include <pfopt/utilities.hpp>
#include <iostream>

using Eigen::MatrixXd;
using namespace std;

int main() {

    vector<double> data = pfopt::io::read_csv("../../../data/signal.csv");

    return 0;
}