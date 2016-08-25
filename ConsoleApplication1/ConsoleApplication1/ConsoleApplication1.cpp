// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Matrix.h"
#include <iostream>
#include <string>

int main()
{
	matrix test_A, test_B, test_C, test_D, test_E;
	test_A.col = 2;
	test_A.row = 4;
	test_A.data = { 1,2,
					2,3,
					5,6,
					7,8};
	test_A.name = "M";
	test_B.row = 2;
	test_B.col = 3;
	test_B.data = { 2,3,4,
					5,6,7 };
	test_B.name = "D";
	test_C = matrix_mult( test_A, test_B, "C");
	test_D = matrix_add(test_A, test_B, "D");
	test_E = matrix_subtr(test_A, test_B, "E");
	print_matrix(test_A);
	print_matrix(test_B);
	print_matrix(test_D);
	print_matrix(test_E);
	print_matrix(test_C);
	system("pause");
	Ode45(4);
	system("pause");
    return 0;
}

void print_matrix(matrix A) {
	std::cout << "Matrix " << A.name << ": \n";
	for (int i = 0; i < A.row; i++) {

		for (int j = 0; j < A.col; j++) {
			std::cout << A.data[j + A.col*i] << ", ";
		}
		std::cout << std::endl;
	}
	std::cout << "\n";
}
matrix matrix_mult(matrix A, matrix B, std::string name){
	if (A.col != B.row) {
		std::cout << "Matrix dimension error: Matrices " << A.name << " and " << B.name << std::endl;
		system("pause");
		return A;
	}
	else {
		matrix C;
		C.row = A.row;
		C.col = B.col;
		C.name = name;
		double temp_val;
		for (int k = 0; k < A.row; k++) {
			for (int i = 0; i < B.col; i++) {
				temp_val = 0;
				for (int j = 0; j < A.col; j++) {
					temp_val = temp_val + A.data[j + A.col * k]*B.data[j*B.col+i];
				}
				C.data.push_back (temp_val);
			}
		}
		return C;
	}

}
matrix matrix_add(matrix A, matrix B,std::string name) {
	if ((A.row != B.row) || (A.col != B.col)) {
		std::cout << "Matrix dimension error: Matrices " << A.name << " and " << B.name << std::endl;
		system("pause");
		return A;
	}
	else {
		matrix C;
		C.col = A.col;
		C.row = A.row;
		C.name = name;
		double temp_val = 0;
		for (int i = 0; i < A.data.size(); i++) {
			temp_val = A.data[i] + B.data[i];
			C.data.push_back(temp_val);
		}
		return C;
	}
}
matrix matrix_subtr(matrix A, matrix B, std::string name) {
	if ((A.row != B.row) || (A.col != B.col)) {
		std::cout << "Matrix dimension error: Matrices " << A.name << " and " << B.name << std::endl;
		system("pause");
		return A;
	}
	else {
		matrix C;
		C.col = A.col;
		C.row = A.row;
		C.name = name;
		double temp_val = 0;
		for (int i = 0; i < A.data.size(); i++) {
			temp_val = A.data[i] - B.data[i];
			C.data.push_back(temp_val);
		}
		return C;
	}
}

