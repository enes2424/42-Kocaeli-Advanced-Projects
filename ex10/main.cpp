#include "Matrix.hpp"

int main()
{
	const Matrix<float> u = {{0.f, 0.f, 1.f}, {1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}};

	std::cout << u.row_echelon() << std::endl;

	const Matrix<float> u1 = {{1.f, 2.f}, {3.f, 4.f}};

	std::cout << u1.row_echelon() << std::endl;

	const Matrix<float> u2 = {{1.f, 2.f}, {2.f, 4.f}};

	std::cout << u2.row_echelon() << std::endl;

	const Matrix<float> u3 = {{8.f, 5.f, -2.f, 4.f, 28.f}, {4.f, 2.5f, 20.f, 4.f, -4.f}, {8.f, 5.f, 1.f, 4.f, 17.f}};

	std::cout << u3.row_echelon() << std::endl;

	const Matrix<float> u4 = {{1.f, 2.f, 3.f, 4.f}, {1.f, 2.f, 3.f, 5.f}, {1.f, 2.f, 4.f, 4.f}, {1.f, 3.f, 4.f, 5.f}};

	std::cout << u4.row_echelon() << std::endl;

	return 0;
}
