#include "Vector.hpp"

int main()
{
	const Vector<float> e1 = {1.f, 0.f, 0.f};
	const Vector<float> e2 = {0.f, 1.f, 0.f};
	const Vector<float> e3 = {0.f, 0.f, 1.f};

	const Vector<float> v1 = {1.f, 2.f, 3.f};
	const Vector<float> v2 = {0.f, 10.f, -100.f};

	std::cout << linear_combination<float>({e1, e2, e3}, {10.f, -2.f, 0.5f}) << std::endl;

	std::cout << linear_combination<float>({v1, v2}, {10.f, -2.f}) << std::endl;

	return 0;
}
