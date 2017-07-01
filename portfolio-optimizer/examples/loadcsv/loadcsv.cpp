#include <pfopt/utilities.hpp>
#include <iostream>

using Eigen::MatrixXd;
using namespace std;

int main() {

    MatrixXd table = pfopt::io::read_csv("../../../data/signal.csv");
    cout << table;

    return 0;
}