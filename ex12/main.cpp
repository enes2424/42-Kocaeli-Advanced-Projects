#include "Matrix.hpp"

int main()
{
	const Matrix<float> u = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};

	std::cout << u.inverse() << std::endl;

	const Matrix<float> u1 = {{2.f, 0.f, 0.f}, {0.f, 2.f, 0.f}, {0.f, 0.f, 2.f}};

	std::cout << u1.inverse() << std::endl;

	const Matrix<float> u2 = {{8.f, 5.f, -2.f}, {4.f, 7.f, 20.f}, {7.f, 6.f, 1.f}};

	std::cout << u2.inverse() << std::endl;

	return 0;
}
