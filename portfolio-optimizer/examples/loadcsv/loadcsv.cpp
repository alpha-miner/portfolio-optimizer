#include <pfopt/utilities.hpp>
#include <iostream>

using Eigen::MatrixXd;
using namespace std;

int main() {

    MatrixXd table = read_csv("../../../data/20160303_10.csv");
    cout << table;

    return 0;
}