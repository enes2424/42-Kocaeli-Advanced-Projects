#include "Matrix.hpp"

int main()
{
	const Matrix<float> u = {{1.f, 0.f}, {0.f, 1.f}};

	std::cout << u.transpose() << std::endl;

	const Matrix<float> u1 = {{2.f, -5.f, 0.f}, {4.f, 3.f, 7.f}};

	std::cout << u1.transpose() << std::endl;

	const Matrix<float> u2 = {{-2.f, -8.f}, {1.f, -23.f}, {0.f, 6.f}};

	std::cout << u2.transpose() << std::endl;

	return 0;
}
