#include <iostream>
#include "Matrix.h"
#include <vector>
int main()
{
	std::vector <Matrix<int>> vec;//template int
	int a, b,d;
	char c;
	for (auto i = 0; i < 2; i++)
	{
		std::cout << "Inpuut num row for "<<i+1<<" Matrix: ";
		std::cin >> a;
		std::cout << std::endl << "Inpuut num col for 1 Matrix: ";
		std::cin >> b;
		std::cout << std::endl << "Fill in the matrix automatically? y/n: ";
		std::cin >> c;
		if (c == 'y')
		{
			std::cout << std::endl << "Inpuut num: ";
			std::cin >> d;
			vec.push_back(Matrix<int>(a, b, d));//template int
		}
		else
		{
			std::cout << std::endl << "Inpuut " << a * b << " values by space: ";
			Matrix<int> mt(a, b);//template int
			mt.inputMatrix();
			vec.push_back(mt);
		}
	}
	try {
		vec.push_back(vec[0] * vec[1]);
	}
	catch(Matrix<decltype(a)>::Exept x)
	{
		std::cout << "Col Matrix 1 != Row Matrix 2: " << x.val1 << " != " << x.val2;
		std::cin.ignore();
		std::cin.get();
		return 0;
	}
	std::cout << "Matrix-Result" << std::endl << vec[2] << std::endl;
	std::cin.ignore();
	std::cin.get();
	return 0;
}