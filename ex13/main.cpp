#include "Matrix.hpp"

int main()
{
	const Matrix<float> u = {{1.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f, 1.f}};

	std::cout << u.rank() << std::endl;

	const Matrix<float> u1 = {{1.f, 2.f, 0.f, 0.f}, {2.f, 4.f, 0.f, 0.f}, {-1.f, 2.f, 1.f, 1.f}};

	std::cout << u1.rank() << std::endl;

	const Matrix<float> u2 = {{8.f, 5.f, -2.f}, {4.f, 7.f, 20.f}, {7.f, 6.f, 1.f}, {21.f, 18.f, 7.f}};

	std::cout << u2.rank() << std::endl;

	return 0;
}
