#include "Matrix.hpp"

int main()
{
	const Matrix<float> u = {{1.f, 0.f}, {0.f, 1.f}};
	const Vector<float> v = {4.f, 2.f};

	std::cout << u.mul_vec(v) << std::endl;

	const Matrix<float> u1 = {{2.f, 0.f}, {0.f, 2.f}};
	const Vector<float> v1 = {4.f, 2.f};

	std::cout << u1.mul_vec(v1) << std::endl;

	const Matrix<float> u2 = {{2.f, -2.f}, {-2.f, 2.f}};
	const Vector<float> v2 = {4.f, 2.f};

	std::cout << u2.mul_vec(v2) << std::endl;

	const Matrix<float> u3 = {{1.f, 0.f}, {0.f, 1.f}};
	const Matrix<float> v3 = {{1.f, 0.f}, {0.f, 1.f}};

	std::cout << u3.mul_mat(v3) << std::endl;

	const Matrix<float> u4 = {{1.f, 0.f}, {0.f, 1.f}};
	const Matrix<float> v4 = {{2.f, 1.f}, {4.f, 2.f}};

	std::cout << u4.mul_mat(v4) << std::endl;

	const Matrix<float> u5 = {{3.f, -5.f}, {6.f, 8.f}};
	const Matrix<float> v5 = {{2.f, 1.f}, {4.f, 2.f}};

	std::cout << u5.mul_mat(v5) << std::endl;

	return 0;
}
