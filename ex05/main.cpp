#include "Vector.hpp"

int main()
{
	const Vector<float> u = {1.f, 0.f};
	const Vector<float> v = {1.f, 0.f};

	std::cout << angle_cos(u, v) << std::endl;

	const Vector<float> u1 = {1.f, 0.f};
	const Vector<float> v1 = {0.f, 1.f};

	std::cout << angle_cos(u1, v1) << std::endl;

	const Vector<float> u2 = {-1.f, 1.f};
	const Vector<float> v2 = {1.f, -1.f};

	std::cout << angle_cos(u2, v2) << std::endl;

	const Vector<float> u3 = {2.f, 1.f};
	const Vector<float> v3 = {4.f, 2.f};

	std::cout << angle_cos(u3, v3) << std::endl;

	const Vector<float> u4 = {1.f, 2.f, 3.f};
	const Vector<float> v4 = {4.f, 5.f, 6.f};

	std::cout << angle_cos(u4, v4) << std::endl;

	return 0;
}
