#include "Matrix.hpp"

int main()
{
	const Matrix<float> u = {{1.f, -1.f}, {-1.f, 1.f}};

	std::cout << u.determinant() << std::endl;

	const Matrix<float> u1 = {{2.f, 0.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 0.f, 2.f}};

	std::cout << u1.determinant() << std::endl;

	const Matrix<float> u2 = {{8.f, 5.f, -2.f}, {4.f, 7.f, 20.f}, {7.f, 6.f, 1.f}};

	std::cout << u2.determinant() << std::endl;

	const Matrix<float> u3 = {{8.f, 5.f, -2.f, 4.f}, {4.f, 2.5f, 20.f, 4.f}, {8.f, 5.f, 1.f, 4.f}, {28.f, -4.f, 17.f, 1.f}};

	std::cout << u3.determinant() << std::endl;

	return 0;
}
