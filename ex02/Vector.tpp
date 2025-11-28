#include <cmath>

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
