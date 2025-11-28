#include "Vector.hpp"
#include "Matrix.hpp"
#include "Common.hpp"

int main()
{
	std::cout << lerp(0.f, 1.f, 0.f) << std::endl
						<< std::endl;

	std::cout << lerp(0.f, 1.f, 1.f) << std::endl
						<< std::endl;

	std::cout << lerp(0.f, 1.f, 0.5f) << std::endl
						<< std::endl;

	std::cout << lerp(21.f, 42.f, 0.3f) << std::endl
						<< std::endl;

	std::cout << lerp(Vector<float>({2.f, 1.f}), Vector<float>({4.f, 2.f}), 0.3f) << std::endl;

	std::cout << lerp(Matrix<float>({{2.f, 1.f}, {3.f, 4.f}}), Matrix<float>({{20.f, 10.f}, {30.f, 40.f}}), 0.5f) << std::endl;

	return 0;
}
