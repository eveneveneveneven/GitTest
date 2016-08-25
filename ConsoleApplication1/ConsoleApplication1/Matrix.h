#include <vector>


struct matrix {
	int col, row;
	std::string name;
	std::vector<double> data;
};

void print_matrix(matrix A);
matrix matrix_mult(matrix A, matrix B, std::string name);
matrix matrix_add(matrix A, matrix B, std::string name);
matrix matrix_subtr(matrix A, matrix B, std::string name);
void Ode45(double tf);