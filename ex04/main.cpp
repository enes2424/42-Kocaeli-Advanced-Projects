#include "Vector.hpp"

int main()
{
	const Vector<float> u = {0.f, 0.f, 0.f};

	std::cout << u.norm_1() << ", " << u.norm() << ", " << u.norm_inf() << std::endl
						<< std::endl;

	const Vector<float> u1 = {1.f, 2.f, 3.f};

	std::cout << u1.norm_1() << ", " << u1.norm() << ", " << u1.norm_inf() << std::endl
						<< std::endl;

	const Vector<float> u2 = {-1.f, -2.f};

	std::cout << u2.norm_1() << ", " << u2.norm() << ", " << u2.norm_inf() << std::endl
						<< std::endl;

	Vector<std::complex<float>> u3{{3.0f, 4.0f}, {-1.0f, 2.0f}};

	std::cout << u3.norm_1() << ", " << u3.norm() << ", " << u3.norm_inf() << std::endl
						<< std::endl;

	return 0;
}
