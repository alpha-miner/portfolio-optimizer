#include "utilities.hpp"
#include "minicsv.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>


MatrixXd read_csv(const std::string& filePath) {

    std::vector<std::vector<double>> table;

    mini::csv::ifstream is(filePath);

    if(is.is_open()) {
        while(is.read_line()) {
            std::string field;
            is >> field;
            std::vector<double> row;
            while(field != "") {
                row.push_back(boost::lexical_cast<double>(field));
                is >> field;
            }
            table.push_back(row);
        }
    }
    else {
        throw std::runtime_error(str(boost::format("file %s is not found") % filePath));
    }

    MatrixXd res(table.size(), table[0].size());

    for(size_t i=0; i != res.rows(); ++i)
        for(size_t j=0; j!= res.cols(); ++j)
            res(i, j) = table[i][j];

    return res;
    
}