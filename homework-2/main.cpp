#include <iostream>
#include <map>
#include "InfMatrix.h"

int main() {
   
    InfMatrix<int, 0> matrix;

    for (int i = 0; i <= 9; ++i)
        matrix[i][i] = i;

    for (int i = 0; i <= 9; ++i)
        matrix[i][9 - i] = 9 - i;

    matrix[116][53] = 0; // Проверка на добавление пустого элемента
    std::cout << "matrix[116][53] = 0" << std::endl;
    matrix[11][5] = 2;  // Проверка на добавление не пустого элемента
    std::cout << "matrix[11][5] = 2" << std::endl;
    std::cout << "A=" << std::endl;

    for (int row = 0; row <= matrix.max_row(); ++row) 
    {
        for (int col = 0; col <= matrix.max_col(); ++col) 
        {
            std::cout << matrix[row][col];
            if (col != matrix.max_col()) std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    matrix[11][5] = 0; // Зануляем элемент. Он не будет учитываться итератором и храниться в мапе.
    std::cout << "matrix[11][5] = 0" << std::endl;
    std::cout << "A=" << std::endl;
    for (int row = 0; row <= matrix.max_row(); ++row) 
    {
        for (int col = 0; col <= matrix.max_col(); ++col) 
        {
            std::cout << matrix[row][col];
            if (col != matrix.max_col()) std::cout << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    std::cout << "A[1...8][1....8]=" << std::endl;
    for (int row = 1; row <= 8; ++row)  // [1..8][1..8]
    {
        for (int col = 1; col <= 8; ++col) {
            std::cout << matrix[row][col];
            if (col != 8) std::cout << " ";
        }
        std::cout << "\n";
    }

    
    std::cout << "Size= " << matrix.size() << "\n"; // Количество занятых ячеек
    std::cout << "Rows= " << matrix.max_row() << "\n"; //Количество строк
    std::cout << "Cols= " << matrix.max_col() << "\n";   //Количество столбцов
    
   
    for (const auto& cell : matrix)  // Все занятые ячейки
    {
        std::cout << "A[" << cell.row() << "," << cell.col()
                  << "] = " << cell.value() << "\n";
    }



    return 0;
}