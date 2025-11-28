#include "Vector.hpp"

int main()
{
	Vector<float> u = {0.f, 0.f};
	const Vector<float> v = {1.f, 1.f};

	std::cout << u.dot(v) << std::endl
						<< std::endl;

	Vector<float> u1 = {1.f, 1.f};
	const Vector<float> v1 = {1.f, 1.f};

	std::cout << u1.dot(v1) << std::endl
						<< std::endl;

	Vector<float> u2 = {-1.f, 6.f};
	const Vector<float> v2 = {3.f, 2.f};

	std::cout << u2.dot(v2) << std::endl
						<< std::endl;

	return 0;
}
