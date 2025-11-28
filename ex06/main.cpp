#include "Vector.hpp"

int main()
{
	const Vector<float> u = {0.f, 0.f, 1.f};
	const Vector<float> v = {1.f, 0.f, 0.f};

	std::cout << cross_product(u, v) << std::endl;

	const Vector<float> u1 = {1.f, 2.f, 3.f};
	const Vector<float> v1 = {4.f, 5.f, 6.f};

	std::cout << cross_product(u1, v1) << std::endl;

	const Vector<float> u2 = {4.f, 2.f, -3.f};
	const Vector<float> v2 = {-2.f, -5.f, 16.f};

	std::cout << cross_product(u2, v2) << std::endl;

	return 0;
}
