#include "matrix.h"

int main(int, char *[]) 
{
    Matrix<int, 2, 0> matrix;
    for (auto i = 0; i <= 9; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }
    for (auto i = 1; i < 9; ++i) {
        for (auto j = 1; j < 9; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Count of elements: " << matrix.count() << std::endl;
    std::cout << std::endl;

    for ( auto it = matrix.begin(); it != matrix.end(); ++it ) {
        std::cout << (*it).second << " -> ";
        for ( auto pos : (*it).first ) {
            std::cout << pos << ", ";
        }
        std::cout << std::endl;

    }
}