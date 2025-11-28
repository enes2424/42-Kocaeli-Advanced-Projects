#include "Vector.hpp"
#include "Matrix.hpp"

int main()
{
	Vector<float> u = {2.f, 3.f};
	const Vector<float> v = {5.f, 7.f};
	u.add(v);
	std::cout << u << std::endl;

	Vector<float> u2 = {2.f, 3.f};
	const Vector<float> v2 = {5.f, 7.f};
	u2.sub(v2);
	std::cout << u2 << std::endl;

	Vector<float> u3 = {2.f, 3.f};
	u3.scl(2.f);
	std::cout << u3 << std::endl;

	Matrix<float> u4 = {{1.f, 2.f}, {3.f, 4.f}};
	const Matrix<float> v4 = {{7.f, 4.f}, {-2.f, 2.f}};
	u4.add(v4);
	std::cout << u4 << std::endl;

	Matrix<float> u5 = {{1.f, 2.f}, {3.f, 4.f}};
	const Matrix<float> v5 = {{7.f, 4.f}, {-2.f, 2.f}};
	u5.sub(v5);
	std::cout << u5 << std::endl;

	Matrix<float> u6 = {{1.f, 2.f}, {3.f, 4.f}};
	u6.scl(2.f);
	std::cout << u6 << std::endl;

	return 0;
}
