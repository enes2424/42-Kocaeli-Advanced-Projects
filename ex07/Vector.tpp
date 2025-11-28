#include <cmath>
#include <complex>

template <typename K>
Vector<K>::Vector()
{
	size = 0;

	data = new K[0];
}

template <typename K>
Vector<K>::Vector(std::initializer_list<K> init)
{
	size = init.size();

	data = new K[size];

	size_t i = 0;

	for (const K &value : init)
	{
		data[i] = value;

		i++;
	}
}

template <typename K>
Vector<K>::Vector(const std::vector<K> &v)
{
	size = v.size();

	data = new K[size];

	size_t i = 0;

	for (const K &value : v)
	{
		data[i] = value;

		i++;
	}
}

template <typename K>
Vector<K>::Vector(const Vector<K> &other)
{
	size = other.size;

	data = new K[size];

	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];
}

template <typename K>
Vector<K> &Vector<K>::operator=(std::initializer_list<K> init)
{
	if (size != init.size())
	{
		delete[] data;

		size = init.size();

		data = new K[size];
	}

	size_t i = 0;

	for (const K &value : init)
	{
		data[i] = value;

		i++;
	}

	return *this;
}

template <typename K>
Vector<K> &Vector<K>::operator=(const std::vector<K> &v)
{
	if (size != v.size())
	{
		delete[] data;

		size = v.size();

		data = new K[size];
	}

	size_t i = 0;

	for (const K &value : v)
	{
		data[i] = value;

		i++;
	}

	return *this;
}

template <typename K>
Vector<K> &Vector<K>::operator=(const Vector<K> &other)
{
	if (size != other.size)
	{
		delete[] data;

		size = other.size;

		data = new K[size];
	}

	for (size_t i = 0; i < size; i++)
		data[i] = other.data[i];

	return *this;
}

template <typename K>
Vector<K>::~Vector()
{
	delete[] data;
}

template <typename K>
void Vector<K>::add(const Vector<K> &v)
{
	if (size != v.size)
		throw std::runtime_error("Vector sizes do not match for addition.");

	for (size_t i = 0; i < size; i++)
		data[i] += v.data[i];
}

template <typename K>
void Vector<K>::sub(const Vector<K> &v)
{
	if (size != v.size)
		throw std::runtime_error("Vector sizes do not match for subtraction.");

	for (size_t i = 0; i < size; i++)
		data[i] -= v.data[i];
}

template <typename K>
void Vector<K>::scl(K a)
{
	for (size_t i = 0; i < size; i++)
		data[i] *= a;
}

template <typename K>
K Vector<K>::dot(const Vector<K> &v) const
{
	if (size != v.size)
		throw std::runtime_error("Vector sizes do not match for dot product.");

	K result = K{};

	for (size_t i = 0; i < size; i++)
		result += data[i] * v.data[i];

	return result;
}

template <typename K>
float Vector<K>::norm_1() const
{
	float result = 0;

	for (size_t i = 0; i < size; i++)
	{
		float value;

		if constexpr (std::is_floating_point_v<K>)
		{
			value = static_cast<float>(data[i]);
			if (value < 0)
				value = -value;
		}
		else if constexpr (std::is_same_v<K, std::complex<float>>)
		{
			value = std::pow(std::fma(data[i].real(), data[i].real(), data[i].imag() * data[i].imag()), 0.5f);
		}

		result += value;
	}

	return result;
}

template <typename K>
float Vector<K>::norm() const
{
	float result = 0;

	for (size_t i = 0; i < size; i++)
	{
		float value;

		if constexpr (std::is_floating_point_v<K>)
		{
			value = data[i];
		}
		else if constexpr (std::is_same_v<K, std::complex<float>>)
		{
			value = std::pow(std::fma(data[i].real(), data[i].real(), data[i].imag() * data[i].imag()), 0.5f);
		}

		result = std::fma(value, value, result);
	}

	return std::pow(result, 0.5f);
}

template <typename K>
float Vector<K>::norm_inf() const
{
	if (size == 0)
		throw std::runtime_error("Cannot compute infinity norm of an empty vector.");

	float result;

	if constexpr (std::is_floating_point_v<K>)
	{
		result = static_cast<float>(data[0]);
		if (result < 0)
			result = -result;
	}
	else if constexpr (std::is_same_v<K, std::complex<float>>)
	{
		result = std::pow(std::fma(data[0].real(), data[0].real(), data[0].imag() * data[0].imag()), 0.5f);
	}

	for (size_t i = 1; i < size; i++)
	{
		float value;

		if constexpr (std::is_floating_point_v<K>)
		{
			value = static_cast<float>(data[i]);
			if (value < 0)
				value = -value;
		}
		else if constexpr (std::is_same_v<K, std::complex<float>>)
		{
			value = std::pow(std::fma(data[i].real(), data[i].real(), data[i].imag() * data[i].imag()), 0.5f);
		}

		result = std::max(result, value);
	}

	return result;
}

template <typename K>
Vector<K> Vector<K>::operator+(const Vector<K> &v) const
{
	Vector<K> result = *this;

	result.add(v);

	return result;
}

template <typename K>
Vector<K> &Vector<K>::operator+=(const Vector<K> &v)
{
	this->add(v);

	return *this;
}

template <typename K>
Vector<K> Vector<K>::operator-(const Vector<K> &v) const
{
	Vector<K> result = *this;

	result.sub(v);

	return result;
}

template <typename K>
Vector<K> &Vector<K>::operator-=(const Vector<K> &v)
{
	this->sub(v);

	return *this;
}

template <typename K>
Vector<K> Vector<K>::operator*(K a) const
{
	Vector<K> result = *this;

	result.scl(a);

	return result;
}

template <typename K>
Vector<K> &Vector<K>::operator*=(K a)
{
	this->scl(a);

	return *this;
}

template <typename K>
std::ostream &operator<<(std::ostream &os, const Vector<K> &v)
{
	if (v.size == 0)
	{
		os << "[]" << std::endl;
		return os;
	}

	for (size_t i = 0; i < v.size; i++)
		os << "[" << v.data[i] << "]" << std::endl;

	return os;
}

template <typename K>
Vector<K> linear_combination(const std::vector<Vector<K>> &u,
														 const std::vector<K> &coefs)
{
	size_t num_vectors = u.size();

	if (num_vectors != coefs.size())
		throw std::runtime_error("Number of vectors and coefficients do not match.");

	if (num_vectors == 0)
		throw std::runtime_error("At least one vector is required for linear combination.");

	const Vector<K> &begin = u[0];

	for (const Vector<K> &vec : u)
		if (vec.size != begin.size)
			throw std::runtime_error("All vectors must have the same size for linear combination.");

	Vector<K> result = begin;

	result.scl(coefs[0]);

	for (size_t i = 1; i < num_vectors; i++)
	{
		Vector<K> const &vec = u[i];

		K const &coef = coefs[i];

		for (size_t j = 0; j < result.size; j++)
			result.data[j] = std::fma(vec.data[j], coef, result.data[j]);
	}

	return result;
}

template <typename K>
float angle_cos(const Vector<K> &u, const Vector<K> &v)
{
	if (u.size != v.size)
		throw std::runtime_error("Vector sizes do not match for angle cosine computation.");

	float norm_u = u.norm();

	if (norm_u == 0)
		throw std::runtime_error("Cannot compute angle cosine with zero vector.");

	float norm_v = v.norm();

	if (norm_v == 0)
		throw std::runtime_error("Cannot compute angle cosine with zero vector.");

	K dot_product = u.dot(v);

	float dot_product_real = static_cast<float>(dot_product);

	return dot_product_real / (norm_u * norm_v);
}

template <typename K>
Vector<K> cross_product(const Vector<K> &u, const Vector<K> &v)
{
	if (u.size != 3 || v.size != 3)
		throw std::runtime_error("Cross product is only defined for 3-dimensional vectors.");

	return {u.data[1] * v.data[2] - u.data[2] * v.data[1],
					-u.data[0] * v.data[2] + u.data[2] * v.data[0],
					u.data[0] * v.data[1] - u.data[1] * v.data[0]};
}